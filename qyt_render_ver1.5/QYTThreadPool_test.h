//
//  QYTThreadPool_test.h
//  qyt_render_ver1.5
//
//  Created by nocolor on 15/2/19.
//  Copyright (c) 2015年 ___JinJiangTiaoZhanBei___. All rights reserved.
//

#ifndef __qyt_render_ver1_5__QYTThreadPool_test__
#define __qyt_render_ver1_5__QYTThreadPool_test__

#include "qyt_thread_pool.h"
#include "QYTSphere.h"
#include <vector>


using namespace QYT;

class SampleInterctTest
{
public:
    
    struct interectResult
    {
        int                     rayIndex;
        QYTDifferentialGeometry dg;
        QYTReal                 rayEpsilon;
        QYTReal                 tHit;
    };
    
    std::shared_ptr<std::vector<QYTSphere>> shapes;
    std::shared_ptr<std::vector<QYTRay>>   rays;
    
    std::shared_ptr<std::vector<interectResult>> operator()() const;
};

void QYTThreadPool_test();

const int numOfTasks = 8;

//8*8个区域，每个区域100个像素，每个像素32个采样点
const int numOfRays = 8*8*100*128;

#endif /* defined(__qyt_render_ver1_5__QYTThreadPool_test__) */
