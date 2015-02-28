//
//  qyt_join_thread.h
//  thread_pool
//
//  Created by nocolor on 15/1/15.
//  Copyright (c) 2015年 ___JinJiangTiaoZhanBei___. All rights reserved.
//

#ifndef __thread_pool__qyt_join_thread__
#define __thread_pool__qyt_join_thread__

#include <iostream>
#include <vector>
#include <thread>

namespace QYT
{
    /**
     @class qyt_join_thread
     这是对线程对象的简单封装，该对象在析构时调用线程的join()函数，
     可以避免产生僵尸线程。但是代价是线程对象必须在析构时等待与父线程
     结合，这会导致析构函数被阻塞。
     */
    class qyt_join_thread
    {
    protected:
        std::vector<std::thread>& threads;
    public:
        explicit qyt_join_thread(std::vector<std::thread>& _threads):threads(_threads){}
        ~qyt_join_thread()
        {
            for (size_t i = 0; i < threads.size(); ++i)
            {
                if(threads[i].joinable())
                    threads[i].join();
            }
        }
    };
}

#endif /* defined(__thread_pool__qyt_join_thread__) */
