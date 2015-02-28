//
//  qyt_thread_pool.h
//  thread_pool
//
//  Created by nocolor on 15/1/15.
//  Copyright (c) 2015年 ___JinJiangTiaoZhanBei___. All rights reserved.
//

#ifndef __thread_pool__qyt_thread_pool__
#define __thread_pool__qyt_thread_pool__

#include "qyt_function_wrapper.h"
#include "qyt_thread_safe_queue.h"
#include "qyt_join_thread.h"
#include "qyt_work_stealing_queue.h"
#include <future>

namespace QYT
{
    /**
     @class qyt_thread_pool
     @brief 简单地线程池对象，默认情况下会开启和当前系统中支持的最大逻辑内核数目相等的
     线程，并且会维持一个任务队列。线程池对象使用简单的负载平衡算法将任务队列中
     的任务派发到空闲的线程上并执行。
     
     可以在任何时候向线程池对象提交任务，也可以在任何时候从线程池对象那里获得结果。
     
     线程池有一个全局的任务队列，它所维护的每个线程又拥有一个线程本地的任务队列。
     当线程池被初始化之后，不论是否已经提交过任何任务，所有的线程都会立刻启动。
     它们首先试图从它们自己本地的任务队列中获得任务，如果本地队列中没有任务，它们
     会试图从全局队列里获取任务，如果全局队列中也没有任务，它们会试图从其他线程的
     本地队列中获取任务。一旦一个线程获取到一个任务，那么它会立刻开始执行这个任务，
     在这个任务完成之前，这个线程不能被取消，如果任务完成，那么空闲的线程会立刻开
     始用上述规则寻找新的任务。
     
     当提交任务时，这个任务也会优先被提交到线程的本地任务队列。
     */
    class qyt_thread_pool
    {
        typedef qyt_function_wrapper task_type;
        std::atomic_bool done;
        
        qyt_thread_safe_queue<task_type> work_queue;
        
        std::vector<std::unique_ptr<qyt_work_stealing_queue>> queues;
        
        std::vector<std::thread> threads;
        qyt_join_thread joiner;
        
        
        static __thread qyt_work_stealing_queue* local_work_queue;
        static __thread unsigned index;
        
        void run_pending_task()
        {

            task_type task;
            if(pop_task_from_local_queue(task) ||
               pop_task_from_pool_queue(task) ||
               pop_task_from_other_thread_queue(task))
            {
                task();
            }
            else
                std::this_thread::yield();

        }
        
        void worker_thread(unsigned my_index_)
        {
            index = my_index_;
            local_work_queue = queues[index].get();
            
            while (!done)
                run_pending_task();
        }
        
        bool pop_task_from_local_queue(task_type& task)
        {
            return local_work_queue && local_work_queue->try_pop(task);
        }
        
        bool pop_task_from_pool_queue(task_type& task)
        {
            return work_queue.try_pop(task);
        }
        
        bool pop_task_from_other_thread_queue(task_type& task)
        {
            for(unsigned i=0;i<queues.size();++i)
            {
                unsigned const my_index=(index+i+1)%queues.size();
                if(queues[my_index]->try_steal(task))
                {
                    return true;
                }
            }
            return false;
        }
        
    public:
        qyt_thread_pool():done(false), joiner(threads)
        {
            const unsigned thread_count = std::thread::hardware_concurrency();
            try
            {
                for (unsigned i = 0; i < thread_count; ++i)
                {
                    queues.push_back(std::unique_ptr<qyt_work_stealing_queue>(new qyt_work_stealing_queue));
                    threads.push_back(std::thread(&qyt_thread_pool::worker_thread, this, i));
                }
            }
            catch (...)
            {
                done = true;
                throw;
            }

        }
        
        ~qyt_thread_pool()
        {
            done = true;
        }
        
        /**
         提交任务
         FunctionType类型的函数对象会作为任务被提交给线程池对象，并且在合适的时候被派分到一个
         线程上并执行。该调用返回一个与FunctionType类型的任务的返回值绑定的future对象，可以在
         需要得到任务的结果时使用future.get()方法获取计算结果。如果调用get()方法时还未得到计算
         结果，那么调用线程会被阻塞。
         */
        template <typename FunctionType>
        std::future<typename std::result_of<FunctionType()>::type> submit(FunctionType _f)
        {
            typedef typename std::result_of<FunctionType()>::type resut_type;
            std::packaged_task<resut_type()> task(std::move(_f));
            std::future<resut_type> res(task.get_future());
            
            if(local_work_queue)
                local_work_queue->push(std::move(task));
            else
                work_queue.push(std::move(task));
            
            return res;
        }
        
    };
    
    typedef qyt_thread_pool QYTThreadPool;
        
}

#endif /* defined(__thread_pool__qyt_thread_pool__) */
