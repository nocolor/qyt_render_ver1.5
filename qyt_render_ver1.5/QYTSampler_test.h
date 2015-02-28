//
//  QYTSampler_test.h
//  qyt_render_ver1.5
//
//  Created by nocolor on 15/2/22.
//  Copyright (c) 2015年 ___JinJiangTiaoZhanBei___. All rights reserved.
//

#ifndef __qyt_render_ver1_5__QYTSampler_test__
#define __qyt_render_ver1_5__QYTSampler_test__

#include "QYTMultiJitteredSampler.h"

using namespace QYT;

class SamplerTestUnit
{
public:
    typedef std::shared_ptr<QYTSampler> Sam_sptr;
    
    Sam_sptr sampler_ptr;
    
    QYTPoint3 operator()()
    {
//        Sam_sptr temp = sampler_ptr;
        
//        sampler_ptr->mapSamplesToSphere();
        std::cout << "mapSamplesToSphere.." << std::endl;
        return sampler_ptr->nextSampleInSphere();
    }
};

void QYTMultiJitteredSampler_test();

#endif /* defined(__qyt_render_ver1_5__QYTSampler_test__) */
