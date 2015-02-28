//
//  QYTSampler_test.cpp
//  qyt_render_ver1.5
//
//  Created by nocolor on 15/2/22.
//  Copyright (c) 2015年 ___JinJiangTiaoZhanBei___. All rights reserved.
//

#include "QYTSampler_test.h"
#include "qyt_thread_pool.h"
void QYTMultiJitteredSampler_test()
{
    SamplerTestUnit testUnit[8];
    
    SamplerTestUnit::Sam_sptr mulJittSamp_ptr = std::make_shared<QYTMultiJitteredSampler>(256, 16);
    
    mulJittSamp_ptr->mapSamplesToSphere();
    
    std::future<QYTPoint3> result[8];
    QYTThreadPool threadPool;
    
    for (int i = 0; i < 8; ++i) {
        testUnit[i].sampler_ptr = mulJittSamp_ptr;
        result[i] = threadPool.submit(testUnit[i]);
    }
    
    for (int i = 0; i < 8; ++i) {
        std::cout << result[i].get() << std::endl;
    }
    
}