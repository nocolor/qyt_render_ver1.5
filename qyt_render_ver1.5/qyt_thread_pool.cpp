//
//  qyt_thread_pool.cpp
//  thread_pool
//
//  Created by nocolor on 15/1/15.
//  Copyright (c) 2015年 ___JinJiangTiaoZhanBei___. All rights reserved.
//

#include "qyt_thread_pool.h"

namespace QYT
{
    __thread unsigned qyt_thread_pool::index = 0;
    __thread qyt_work_stealing_queue* qyt_thread_pool::local_work_queue = nullptr;
}