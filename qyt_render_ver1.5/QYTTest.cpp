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

<<<<<<< HEAD
void QYTCamera_test()
{
    QYTPoint3 p(1, 0, 100);
    QYTTransform world2camera = QYTLookAt(QYTPoint3(1, 0, 0),
                                          QYTPoint3(1, 0, 100),
                                          QYTVec3(0, 1, 0));
    
    std::cout << (world2camera)(p);
    
//    QYTVec3 a(1, 0, 0), b(0, 1, 0);
//    std::cout << QYTVec3::Cross(a, b);
}


///测试图像管线、采样器、相机、线程池、基础图元
int QYTRender_ver_1_5_test()
=======
void QYTImageFilm_test()
>>>>>>> parent of e9f0f1b... 对现行版本的组件进行了功能和可行性测试
{
    QYTBoxFilter boxFilter(1, 1);
    
<<<<<<< HEAD
    //设置相机开始的位置
    std::shared_ptr<QYTTransform> cameraStart =
    std::make_shared<QYTTransform>(!QYTLookAt(QYTPoint3(0, 0, 400),
                                              QYTPoint3(0, 0, 0),
                                              QYTVec3(0, 1, 0)));
    
    //设置相机结束的位置
    std::shared_ptr<QYTTransform> cameraEnd =
    std::make_shared<QYTTransform>(!QYTLookAt(QYTPoint3(100, 0, 400),
                                              QYTPoint3(0, 0, 0),
                                              QYTVec3(0, 1, 0)));
    
    //定义一个可以插值的动画变换，这个变换被摄影机用到
    QYTAnimatedTransform cameraToWorld(cameraStart, 0.0, cameraEnd, 0.5);

    
    //定义滤波器
    QYTBoxFilter boxFilter(.5, .5);
    
    //定义底片的裁剪窗口
    const float crop[4] = {0,1,0,1};
    
    //定义底片
    QYTImageFilm *film = new QYTImageFilm(imageWight, imageHight,                     //x，y分辨率
                                          &boxFilter,                   //滤波器
                                          crop,                         //底片裁剪窗口
                                          "/Users/nocolor/Desktop/test.tga");   //图片储存路径
    
    //定义摄影机裁剪窗口
    const float screenWindow[4] = {-1,1, -1,1};
    
    //定义一个相机
    QYTPerspectiveCamera camera(cameraToWorld,      //世界坐标到相机坐标的逆
                                screenWindow,       //屏幕窗口
                                0.0,                //开启快门的时间
                                0.05,               //关闭快门的时间
                                5.0f,               //透镜半径
                                100.f,               //焦平面距离
                                60,                 //视角
                                film);              //底片
    
    /**********************************************/
    /*定义采样器，用采样点产生光线*/
    
    QYTRNG rng;
    
    QYTStratifiedSampler sampler(0, film->xResolution,
                                 0, film->yResolution,
                                 stratifX, stratifY,
                                 true,
                                 0, 0.05);
    
    for (int x = 0; x < film->xResolution; ++x)
        for (int y = 0; y < film->yResolution; ++y)
        {

            
            QYTSample cameraSample[samplesPerPixel];
            
            sampler.getMoreSamples(cameraSample, rng);
            
            for (int i = 0; i < samplesPerPixel; ++i)
            {
                QYTDifferentialGeometry dg; //储存交点处的详细信息
                QYTReal rayEpsilon;           //避免自相交
                QYTReal tHit;               //交点的t值
                
                QYTRayDifferential cameraRay;//(QYTPoint3(cameraSample[i].imageX, cameraSample[i].imageY, 100), QYTVec3(0, 0, -1), 0.0);
                

                float cameraRayWight = camera.generateRay(cameraSample[i], &cameraRay);
                
                //判断求交
                bool isIntersect = sphere.intersect(cameraRay,
                                                    &tHit,
                                                    &rayEpsilon,
                                                    &dg);
                
                if(isIntersect)
                {
                    film->addSample(cameraSample[i], cameraRayWight*QYTSpectrum(0, 1, 0));
                }

            }
        }
    
    film->writeImage();
=======
    const float crop[4] = {0,1,0,1};
    
    QYTImageFilm film(400, 400,
                      &boxFilter,
                      crop,
                      "/Users/nocolor/Desktop/test.tga");
>>>>>>> parent of e9f0f1b... 对现行版本的组件进行了功能和可行性测试
    
    film.writeImage();
}