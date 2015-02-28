//
//  qyt_function_wrapper.h
//  thread_pool
//
//  Created by nocolor on 15/1/15.
//  Copyright (c) 2015年 ___JinJiangTiaoZhanBei___. All rights reserved.
//

#ifndef __thread_pool__qyt_function_wrapper__
#define __thread_pool__qyt_function_wrapper__

#include <iostream>
#include <memory>

namespace QYT
{
    /**
     @class qyt_function_wrapper
     这是对线程任务的封装，也可以认为是函数对象。
     该对象可以封装一个独立的任务，这个任务会被绑定到一个
     独立的线程。
     qyt_function_wrapper对象时不可以被复制的，对这类对象
     进行赋值操作会导致转移语义。
     */
    class qyt_function_wrapper
    {
        struct impl_base
        {
            virtual void call() = 0;
            virtual ~impl_base(){}
        };
        
        ///类内部私有的任务类型，它是一个函数对象
        ///@param Func 函数类型，代表一类函数对象
        template<typename Func>
        struct impl_type: impl_base
        {
            Func f;
            impl_type(Func&& _f):f(std::move(_f)){}
            void call() {f();}
        };
        
        std::unique_ptr<impl_base> impl;
    public:
        ///Func是函数类型（不是一般的数据类型）
        template<typename Func>
        qyt_function_wrapper(Func&& _f):impl(new impl_type<Func>(std::move(_f))){}
        
        ///通过这个操作调用函数
        void operator()() {impl->call();}
        qyt_function_wrapper() = default;
        qyt_function_wrapper(qyt_function_wrapper&& _src):impl(std::move(_src.impl)){}
        
        ///赋值操作会导致转移语义，左操作数获得对象的所有权，右操作数将成为无效对象
        qyt_function_wrapper& operator=(qyt_function_wrapper&& _rhs)
        {
            impl = std::move(_rhs.impl);
            return *this;
        }
        
        //qyt_function_wrapper不能被拷贝
        qyt_function_wrapper(const qyt_function_wrapper&) = delete;
        qyt_function_wrapper(qyt_function_wrapper&) = delete;
        qyt_function_wrapper& operator = (const qyt_function_wrapper&) = delete;
    };
}

#endif /* defined(__thread_pool__qyt_function_wrapper__) */
