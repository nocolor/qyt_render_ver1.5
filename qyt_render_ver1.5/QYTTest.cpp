//
//  QYTTest.cpp
//  qyt_render_ver1.5
//
//  Created by nocolor on 15/3/6.
//  Copyright (c) 2015年 ___JinJiangTiaoZhanBei___. All rights reserved.
//

#include "QYTTest.h"
#include "QYTBoxFilter.h"

#include "QYTSphere.h"
#include "QYTPerspectiveCamera.h"
#include "QYTAnimatedTransform.h"
#include "QYTRNG.h"

using namespace QYT;
const int imageWight = 400, imageHight = 400;
const int stratifX = 2, stratifY = 2;
const int samplesPerPixel = stratifX*stratifY;


//void QYTStratifiedSampler_test()
//{
//    QYTStratifiedSampler stratifiedSampler(0.f, 1.f,
//                                           0.f, 1.f,
//                                           16, 16,
//                                           true,
//                                           0.f, 0.1f);
//    
//    QYTSample *samples = new QYTSample[stratifiedSampler.maximumSampleCount()];
//    
////    for (int i = 0; i < stratifiedSampler.maximumSampleCount(); ++i)
////    {
////        samples[i].add1D(<#uint32_t num#>)
////    }
//    
//    QYTRNG rng;
//    
//    stratifiedSampler.getMoreSamples(samples, rng);
//    
//    for (uint32_t i = 0; i < stratifiedSampler.maximumSampleCount(); ++i)
//    {
//        for (uint32_t j = 0; j < samples[i].n1D.size(); ++j)
//        {
//            for (uint32_t u = 0; u < samples[i].n1D[j]; ++u)
//            {
//                std::cout << samples[i].oneD[j][u] << std::endl;
//            }
//        }
//    }
//    
////    QYTRNG rng;
////    QYTSample samples;
////    samples.add1D(16);
////    samples.allocateSampleMemory();
////    
////    QYTLatinHypercube(samples.oneD[0], 256, 1, rng);
////    
////    for (int i = 0; i < samples.n1D.size(); ++i)
////    {
////        for (int j = 0; j < samples.n1D[i]; j++)
////        {
////            std::cout << samples.oneD[i][j] << std::endl;
////        }
////    }
//    
//}
//
void QYTImageFilm_test()
{
    QYTBoxFilter boxFilter(0.5, 0.5);
    
    const float crop[4] = {0,1,0,1};
    
    QYTImageFilm film(32, 32,
                      &boxFilter,
                      crop,
                      "/Users/nocolor/Desktop/QYTImageFilm_test.tga");
    
    QYTStratifiedSampler sampler(0, 5,
                                 0, 5,
                                 stratifX, stratifY,
                                 true,
                                 0, 0.05);
    
    
    QYTSample samples[samplesPerPixel];
    QYTRNG rng;
    

    
    for (int x = 0; x < imageWight; ++x)
    {
        for (int y = 0; y < imageHight; ++y)
        {
            sampler.getMoreSamples(samples, rng);
            
            for (int i = 0; i < samplesPerPixel; ++i)
            {
                std::cout << samples[i].imageX << ", " << samples[i].imageY << std::endl;
                film.addSample(samples[i], QYTSpectrum(1, 0, 0));
            }
        }
    }
    
    film.writeImage();
}

///测试图像管线、采样器、相机、线程池、基础图元
int QYTRender_ver_1_5_test()
{
    //一个定义在世界坐标中的变换，T可以是各种仿射变换的级联
    QYTTransform T = QYTTranslate(50, 0, 0);
    
    //创建一个从世界坐标到局部坐标的变换，
    //这个变换将会把定义在世界坐标中的物体转换到某个局部坐标系中
    std::shared_ptr<QYTTransform> w2o =
    std::make_shared<QYTTransform>(T);
    
    QYTSphere sphere(w2o,               //转换到局部坐标的变换
                     false,             //不翻转法线
                     60.f,               //半径
                     -60,                 //纵向起点
                     60,               //纵向终点
                     360.f);            //旋转角（注意使用角度不是弧度）
    
    
    
    //设置相机开始的位置
    std::shared_ptr<QYTTransform> cameraStart =
    std::make_shared<QYTTransform>(QYTLookAt(QYTPoint3(0, 0, 400),
                                              QYTPoint3(0, 0, 0),
                                              QYTVec3(0, 1, 0)));
    
    //设置相机结束的位置
    std::shared_ptr<QYTTransform> cameraEnd =
    std::make_shared<QYTTransform>(QYTLookAt(QYTPoint3(0, 0, 400),
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
                    film->addSample(cameraSample[i], cameraRayWight*QYTSpectrum(1, 0, 0));
                }

            }
        }
    
    film->writeImage();
    
    return 0;
}