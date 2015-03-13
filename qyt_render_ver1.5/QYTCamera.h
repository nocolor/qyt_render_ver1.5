//
//  QYTCamera.h
//  qyt_render_ver1.5
//
//  Created by nocolor on 15/3/13.
//  Copyright (c) 2015年 ___JinJiangTiaoZhanBei___. All rights reserved.
//

#ifndef __qyt_render_ver1_5__QYTCamera__
#define __qyt_render_ver1_5__QYTCamera__

#include "qytr.h"
#include "QYTAnimatedTransform.h"
#include "QYTFilm.h"
#include "qyt_ray.h"
#include "QYTSampler.h"

namespace QYT
{
    /**
     @class QYTCamera
     摄影机类负责创建视见坐标系，并且负责产生光线。
     所有形式的摄影机都必须是QYTCamera的子类。
     */
    class QYTCamera
    {
    public:
        ///一个从相机坐标到世界坐标的变换，由于支持动态模糊，因此这个变换是可进行插值的。
        QYTAnimatedTransform cameraToWorld;
        
        ///分别代表快门打开的时间和快门关闭的时间
        const QYTReal shutterOpen, shtterClose;
        
        ///底片类，用于最终生成图像
        QYTFilm* film;
        
        QYTCamera(const QYTAnimatedTransform& cam2World,
                  QYTReal sopen, QYTReal sclose,
                  QYTFilm* iFilm);
        virtual ~QYTCamera();
        
        
        /**
         产生一条相机光线。
         根据采样点信息来生成一条光线，产生的光线经由指针参数传出。
         @param sample      （NDC）空间中的采样点
         @param ray         产生的光线，由这个指针传出
         @return            ray对当前像素的贡献权值（模拟景深效果时使用）
         */
        virtual QYTReal generateRay(const QYTCameraSample& sample,
                                    QYTRay* ray) const = 0;
        
        /**
         产生一条主光线以及两条附加光线，用于反走样算法。
         @see generateRay(const QYTCameraSample& sample, QYTRay* ray) const
         */
        virtual QYTReal generateRayDifferential(const QYTCameraSample& sample,
                                                QYTRayDifferential* rd) const;
        
    };
    
    /**
     @class QYTProjectiveCamera
     带有各类坐标变换的相机，包括从相机坐标投影到屏幕坐标、再到光栅坐标以及它们的逆变换。
     同时支持景深，包含透镜半径以及焦距参数。
     */
    class QYTProjectiveCamera:public QYTCamera
    {
    protected:
        ///相机架标到屏幕架标，光栅架标到相机架标
        QYTTransform cameraToScreen, rasterToCamera;
        
        ///屏幕架标到光栅架标，光栅架标到屏幕架标
        QYTTransform screenToRaster, rasterToScreen;
        
        ///透镜的半径以及焦距
        QYTReal lenRadius, focalDistance;
        
    public:
        QYTProjectiveCamera(const QYTAnimatedTransform& cam2World,
                            const QYTTransform& proj, const QYTReal screenWindow[4],
                            QYTReal sopen, QYTReal sclose, QYTReal lensr, QYTReal focld, QYTFilm* iFilm);
    };
}

#endif /* defined(__qyt_render_ver1_5__QYTCamera__) */
