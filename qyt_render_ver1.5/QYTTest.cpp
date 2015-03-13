//
//  QYTTest.cpp
//  qyt_render_ver1.5
//
//  Created by nocolor on 15/3/6.
//  Copyright (c) 2015年 ___JinJiangTiaoZhanBei___. All rights reserved.
//

#include "QYTTest.h"
#include "QYTBoxFilter.h"
using namespace QYT;

void QYTStratifiedSampler_test()
{
    QYTStratifiedSampler stratifiedSampler(0.f, 1.f,
                                           0.f, 1.f,
                                           16, 16,
                                           true,
                                           0.f, 0.1f);
    
    QYTSample *samples = new QYTSample[stratifiedSampler.maximumSampleCount()];
    
//    for (int i = 0; i < stratifiedSampler.maximumSampleCount(); ++i)
//    {
//        samples[i].add1D(<#uint32_t num#>)
//    }
    
    QYTRNG rng;
    
    stratifiedSampler.getMoreSamples(samples, rng);
    
    for (uint32_t i = 0; i < stratifiedSampler.maximumSampleCount(); ++i)
    {
        for (uint32_t j = 0; j < samples[i].n1D.size(); ++j)
        {
            for (uint32_t u = 0; u < samples[i].n1D[j]; ++u)
            {
                std::cout << samples[i].oneD[j][u] << std::endl;
            }
        }
    }
    
//    QYTRNG rng;
//    QYTSample samples;
//    samples.add1D(16);
//    samples.allocateSampleMemory();
//    
//    QYTLatinHypercube(samples.oneD[0], 256, 1, rng);
//    
//    for (int i = 0; i < samples.n1D.size(); ++i)
//    {
//        for (int j = 0; j < samples.n1D[i]; j++)
//        {
//            std::cout << samples.oneD[i][j] << std::endl;
//        }
//    }
    
}

void QYTImageFilm_test()
{
    QYTBoxFilter boxFilter(1, 1);
    
    const float crop[4] = {0,1,0,1};
    
    QYTImageFilm film(400, 400,
                      &boxFilter,
                      crop,
                      "/Users/nocolor/Desktop/test.tga");
    
    film.writeImage();
}