//
//  QYTMemory.h
//  qyt_render_ver1.5
//
//  Created by nocolor on 15/3/26.
//  Copyright (c) 2015年 ___JinJiangTiaoZhanBei___. All rights reserved.
//

#ifndef __qyt_render_ver1_5__QYTMemory__
#define __qyt_render_ver1_5__QYTMemory__

#include "qytr.h"
#include <atomic>
namespace QYT
{
    /**
     @class qyt_reference_counted
     “引用计数”计数可以用以管理内存问题，因此qyt_reference_counted类提供这个计数。
     成员references代表当前对象（或者是指向动态分配内存的指针）持有的计数，这样可以
     跟踪对象的生命周期。
     @attention 无法处理“循环引用”！
     */
    class qyt_reference_counted
    {
    public:
        std::atomic<int32_t> references;
        
        qyt_reference_counted()
        {references = 0;}
        
        qyt_reference_counted(const qyt_reference_counted&) = delete;
        qyt_reference_counted& operator = (const qyt_reference_counted&) = delete;
    };
    
    /**
     @class qyt_reference
     提供引用计数管理的模板类，使用这个类来进行动态内存管理，可以免去手动释放内存。
     这个类被设计为线程安全。
     @code
     class Foo:public qyt_reference_counted
     {
     public:
     ~Foo()
     {std::cout << this << ": deleted.\n";}
     
     Foo()
     {std::cout << this << ": allocated.\n";}
     
     int getReference() const
     {return this->references;}
     };
     
     ...
     
     qyt_reference<Foo> r1 = new Foo;
     qyt_reference<Foo> r2 = r1;
     
     r1 = new Foo;
     r2 = r1;
     ...
     
     //无需手动调用delete，内存会被自动释放。
     @endcode
     @attention 模板参数Type类型必须是qyt_reference_counted的子类。
     */
    template<typename Type>
    class qyt_reference
    {
    private:
        Type* ptr;
        
    public:
        qyt_reference(Type* p = nullptr)
        {
            ptr = p;
            if(ptr != nullptr)
                ++ptr->references;
        }
        
        qyt_reference(const qyt_reference<Type>& src)
        {
            ptr = src.ptr;
            if(ptr != nullptr)
                ++ptr->references;
        }
        
        qyt_reference& operator = (const qyt_reference<Type>& rhs)
        {
            if(rhs.ptr != nullptr)
                ++rhs.ptr->references;
            if(ptr != nullptr && (--ptr->references == 0))
                delete ptr;
            ptr = rhs.ptr;
            return *this;
        }
        
        qyt_reference& operator = (Type* p)
        {
            if(p != nullptr)
                ++p->references;
            if(ptr != nullptr && (--ptr->references == 0))
                delete ptr;
            ptr = p;
            return *this;
        }
        
        ~qyt_reference()
        {
            if(ptr != nullptr && (--ptr->references == 0))
                delete ptr;
        }
        
        Type* operator->()
        {return ptr;}
        
        const Type* operator->() const
        {return ptr;}
        
        operator bool() const
        {return ptr != nullptr;}
        
        const Type* getPtr() const
        {return ptr;}
    };
}

#endif /* defined(__qyt_render_ver1_5__QYTMemory__) */
