//
//  qyt_thread_safe_queue.h
//  qyt_foundation
//
//  Created by nocolor on 15/1/20.
//  Copyright (c) 2015年 ___JinJiangTiaoZhanBei___. All rights reserved.
//

#ifndef __qyt_foundation__qyt_thread_safe_queue__
#define __qyt_foundation__qyt_thread_safe_queue__

#include <queue>
#include <memory>
#include <condition_variable>

namespace QYT
{
    /**
     @class qyt_thread_safe_queue
     这是一个线程安全的队列数据结构，该结构的一般的队列没有
     组织和使用上的差别，但是它使用互斥量和锁保证多个线程在
     同时访问队列时不会产生条件竞争。
     也是基于同样的理由，它的读写效率比普通队列低。
     当有多个线程试图同时访问这个队列时，只有一个线程可以获
     得读写的锁，其他线程会被阻塞，直到读写完成。
     */
    template<typename Type>
    class qyt_thread_safe_queue
    {
    protected:
        ///互斥量，用于给队列加锁
        mutable std::mutex      m_mutex;
        
        ///队列结构
        std::queue<Type>        m_queue;
        
        ///信号量，用于唤醒被阻塞的线程
        std::condition_variable m_sync;
        
    public:
        qyt_thread_safe_queue(){}
        
        qyt_thread_safe_queue(const qyt_thread_safe_queue& _src)
        {
            std::lock_guard<std::mutex> lock(_src.m_mutex);
            m_queue = _src.m_queue;
        }
        
        ///向队列中添加元素
        void push(Type _value)
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_queue.push(std::move(_value));
            m_sync.notify_one();
        }
        
        ///获取队列中第一个元素，然后将它从队列中移除
        ///@attention   如果队列为空，那么该调用会被阻塞，直到向队列中添加新元素
        void wait_pop(Type& _value)
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            m_sync.wait(lock, [this]{return !m_queue.empty();});
            _value = std::move(m_queue.front());
            m_queue.pop();
        }
        
        ///获取队列中第一个元素的指针，然后将它移除
        ///@attention   如果队列为空，那么该调用会被阻塞，直到向队列中添加新元素
        std::shared_ptr<Type> wait_pop()
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            m_sync.wait(lock, [this]{return !m_queue.empty();});
            std::shared_ptr<Type> res(std::make_shared<Type>(std::move(m_queue.front())));
            m_queue.pop();
            return res;
        }
        
        ///获取队列中第一个元素，然后将它从队列中移除
        ///@return  如果队列为空，返回false，否则返回true
        bool try_pop(Type& _value)
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            if(m_queue.empty())
                return false;
            _value = std::move(m_queue.front());
            m_queue.pop();
            return true;
        }
        
        ///获取队列中第一个元素的指针，然后将它从队列中移除
        ///@return  如果队列为空，返回false，否则返回true
        std::shared_ptr<Type> try_pop()
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            if(m_queue.empty())
                return std::shared_ptr<Type>();
            std::shared_ptr<Type> res(std::make_shared<Type>(std::move(m_queue.front())));
            m_queue.pop();
            return res;
        }
        
        ///如果队列为空返回true，否则返回false
        bool empty() const
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            return m_queue.empty();
        }
    };
}

#endif /* defined(__qyt_foundation__qyt_thread_safe_queue__) */
