//
//  qyt_work_stealing_queue.h
//  thread_pool
//
//  Created by nocolor on 15/1/16.
//  Copyright (c) 2015年 ___JinJiangTiaoZhanBei___. All rights reserved.
//

#ifndef __thread_pool__qyt_work_stealing_queue__
#define __thread_pool__qyt_work_stealing_queue__

#include <memory>
#include <mutex>
#include <deque>
#include "qyt_function_wrapper.h"

namespace QYT
{
    class qyt_work_stealing_queue
    {
    private:
        std::deque<qyt_function_wrapper> m_queue;
        mutable std::mutex m_mutex;
        
    public:
        qyt_work_stealing_queue(){}
        qyt_work_stealing_queue(const qyt_work_stealing_queue& other) = delete;
        qyt_work_stealing_queue& operator = (const qyt_work_stealing_queue&) = delete;
        
        void push(qyt_function_wrapper data)
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_queue.push_front(std::move(data));
        }
        
        bool empty() const
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            return m_queue.empty();
        }
        
        bool try_pop(qyt_function_wrapper& res)
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            if(m_queue.empty())
                return false;
            res = std::move(m_queue.front());
            m_queue.pop_front();
            return true;
        }
        
        bool try_steal(qyt_function_wrapper& res)
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            if(m_queue.empty())
                return false;
            res = std::move(m_queue.back());
            m_queue.pop_back();
            return true;
        }
    };
}

#endif /* defined(__thread_pool__qyt_work_stealing_queue__) */
