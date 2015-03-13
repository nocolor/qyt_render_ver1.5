//
//  QYTCamera.cpp
//  qyt_render_ver1.5
//
//  Created by nocolor on 15/3/13.
//  Copyright (c) 2015年 ___JinJiangTiaoZhanBei___. All rights reserved.
//

#include "QYTCamera.h"

namespace QYT
{
    QYTCamera::~QYTCamera()
    {
        delete film;
    }
    
    QYTCamera::QYTCamera(const QYTAnimatedTransform& cam2World,
                         QYTReal sopen, QYTReal sclose,
                         QYTFilm* iFilm):cameraToWorld(cam2World), shutterOpen(sopen), shtterClose(sclose)
    {
        film = iFilm;
        if(cameraToWorld.hasScale())
            QYTWarning("Scaling detected in world-to-camera transformation!\n"
                       "The system has numerous assumptions, implicit and explicit,\n"
                       "that this transform will have no scale factors in it.\n"
                       "Proceed at your own risk; your image may have errors or\n"
                       "the system may crash as a result of this.");
    }
    
    QYTReal QYTCamera::generateRayDifferential(const QYT::QYTCameraSample &sample, QYT::QYTRayDifferential *rd) const
    {
        QYTReal wt = generateRay(sample, rd);
        
        QYTCameraSample sshift = sample;
        ++(sshift.imageX);
        QYTRay rx;
        
        QYTReal wtx = generateRay(sshift, &rx);
        rd->rxOrigin = rx.o;
        rd->rxDirection = rx.d;
        
        --(sshift.imageX);
        ++(sshift.imageY);
        QYTRay ry;
        
        QYTReal wty = generateRay(sshift, &ry);
        rd->ryOrigin = ry.o;
        rd->ryDirection = ry.d;
        
        if(wtx == 0.f || wty == 0.f)
            return 0.f;
        rd->hasDifferentials = true;
        return wt;
    }
    
    
    QYTProjectiveCamera::QYTProjectiveCamera(const QYTAnimatedTransform& cam2World,
                                             const QYTTransform& proj, const QYTReal screenWindow[4],
                                             QYTReal sopen, QYTReal sclose, QYTReal lensr, QYTReal focld, QYTFilm* iFilm):QYT::QYTCamera(cam2World, sopen, sclose, iFilm)
    {
        lenRadius = lensr;
        focalDistance = focld;
        
        cameraToScreen = proj;
        
        screenToRaster = QYTScale((float)film->xResolution,
                                  (float)film->yResolution, 1.f)*
        QYTScale(1.f / (screenWindow[1] - screenWindow[0]),
                 1.f / (screenWindow[2] - screenWindow[3]), 1.f)*
        QYTTranslate(-screenWindow[0], -screenWindow[3], 0.f);
        rasterToScreen = !screenToRaster;
        rasterToCamera = (!cameraToScreen) * rasterToScreen;
    }
}