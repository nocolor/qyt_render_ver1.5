//
//  QYTThreadPool_test.cpp
//  qyt_render_ver1.5
//
//  Created by nocolor on 15/2/19.
//  Copyright (c) 2015年 ___JinJiangTiaoZhanBei___. All rights reserved.
//

#include "QYTThreadPool_test.h"
#include "QYTTimer.h"
#include <random>

std::shared_ptr<std::vector<SampleInterctTest::interectResult>> SampleInterctTest::operator()() const
{
    std::vector<interectResult> result;
    
    for (int i = 0; i < rays->size(); ++i)
    {
        for (int j = 0; j < shapes->size(); ++j)
        {
            interectResult tmp;
            if((*shapes)[j].intersect((*rays)[i],
                                      &tmp.tHit,
                                      &tmp.rayEpsilon,
                                      &tmp.dg))
            {
                tmp.rayIndex = i;
                result.push_back(std::move(tmp));
            }
        }
    }
    std::shared_ptr<std::vector<interectResult>> sptr = std::make_shared<std::vector<interectResult>>(std::move(result));
    
    return sptr;
    
}

void QYTThreadPool_test()
{
    QYTSphere sphere(LCR_TO_PBRT_S_PTR, //转换到局部坐标的变换
                     false,             //不翻转法线
                     1.f,               //半径
                     -1.f,              //纵向起点
                     1.f,               //纵向终点
                     360.f);            //旋转角（注意使用角度不是弧度）
    
    
    std::vector<QYTRay> rays[numOfTasks];
    std::vector<QYTSphere> spheres;
    spheres.push_back(sphere);
    
    std::default_random_engine e;
    
    std::uniform_real_distribution<float> u(0, 1);
    
    //8个vector<QYTRay>，每个里面numOfRays条光线
    for (int i = 0; i < numOfTasks; ++i) {
        for (int j = 0; j < numOfRays; ++j) {
            QYTRay tmpRay(QYTPoint3(u(e), u(e), -100.f),
                          QYTVec3(0.f, 0.f, 1.f),
                          0.f);
            rays[i].push_back(std::move(tmpRay));
        }
    }
    
    //8个SampleInterctTest对象
    SampleInterctTest task[numOfTasks];
    
    for (int i = 0; i < numOfTasks; ++i) {
        task[i].shapes = std::make_shared<std::vector<QYTSphere>>(spheres);
        task[i].rays = std::make_shared<std::vector<QYTRay>>(rays[i]);
    }
    
    typedef  std::shared_ptr<std::vector<SampleInterctTest::interectResult>> ResultType;
    
    QYTTimer timer;
    
    std::future<ResultType> results[numOfTasks];
    ResultType              resultsWithoutThread[numOfTasks];

    //如果不使用多线程，一个线程执行所有求交...
    std::cout << "Tasks without QYTThreadPool: " << numOfRays*numOfTasks << " rays, " << spheres.size() << " sphere(s).\n";
    timer.start();
    for (int i = 0; i < numOfTasks; ++i) {
        resultsWithoutThread[i] = task[i]();
    }
    std::cout << "...cost " << timer.time() << "s.\n";
    timer.stop();
    timer.reset();
    
    std::cout << "__________________________________________________________\n";

    std::cout << "Tasks uising QYTThreadPool: " << numOfTasks << "tasks, " << numOfRays << " rays/task, " << spheres.size() << " sphere(s).\n";
    QYTThreadPool threadPool;
    timer.start();
    for (int i = 0; i < 8; ++i) {
        results[i] = threadPool.submit(task[i]);
    }

    //获取任意任务中的结果...
    for (int i; i < numOfTasks; ++i) {
        resultsWithoutThread[i] = results[i].get();
    }
    std::cout << "...cost " << timer.time() << "s.\n";
    timer.stop();
}