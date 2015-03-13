//
//  QYTPerspectiveCamera.cpp
//  qyt_render_ver1.5
//
//  Created by nocolor on 15/3/13.
//  Copyright (c) 2015年 ___JinJiangTiaoZhanBei___. All rights reserved.
//

#include "QYTPerspectiveCamera.h"

namespace QYT
{
    QYTPerspectiveCamera::QYTPerspectiveCamera(const QYTAnimatedTransform& cam2World,
                                               const QYTReal screenWindow[4],
                                               QYTReal sopen, QYTReal sclose,
                                               QYTReal lensr, QYTReal focald,
                                               QYTReal fov, QYTFilm* iFilm):
    QYT::QYTProjectiveCamera(cam2World, QYTPerspective(fov, 1e-2f, 1000.f),
                             screenWindow, sopen, sclose, lensr, focald, iFilm)
    {
        dxCamera = rasterToCamera(QYTPoint3(1,0,0)) - rasterToCamera(QYTPoint3(0, 0, 0));
        dyCamera = rasterToCamera(QYTPoint3(0,1,0)) - rasterToCamera(QYTPoint3(0, 0, 0));
    }
    
    QYTReal QYTPerspectiveCamera::generateRay(const QYTCameraSample &sample, QYTRay *ray) const
    {
        QYTPoint3 pRas(sample.imageX, sample.imageY, 0);
        QYTPoint3 pCamera;
        pCamera = rasterToCamera(pRas);
        
        *ray = QYTRay(QYTPoint3(0, 0, 0), QYTVector3::Normalize(QYTVector3(pCamera)), 0.f, INFINITY);
        
        if(lenRadius > 0.f)
        {
            float lensU, lensV;
            QYTConcentricSampleDisk(sample.lensU, sample.lensV, &lensU, &lensV);
            lensU *= lenRadius;
            lensV *= lenRadius;
            
            float ft = focalDistance/ray->d.z;
            QYTPoint3 pFoucs = (*ray)(ft);
            
            ray->o = QYTPoint3(lensU, lensV, 0.f);
            ray->d = QYTVector3::Normalize(pFoucs - ray->o);
        }
        
        ray->time = sample.time;
        *ray = cameraToWorld(*ray);
        
        return 1.f;
    }
    
    QYTReal QYTPerspectiveCamera::generateRayDifferential(const QYT::QYTCameraSample &sample, QYT::QYTRayDifferential *rd) const
    {
        QYTPoint3 pRas(sample.imageX, sample.imageY, 0);
        QYTPoint3 pCamera;
        pCamera = rasterToCamera(pRas);
        
        QYTVec3 dir = QYTVector3::Normalize(QYTVec3(pCamera.x, pCamera.y, pCamera.z));
        *rd = QYTRayDifferential(QYTPoint3(0, 0, 0), dir, 0.f, INFINITY);
        
        if(lenRadius > 0.f)
        {
            float lensU, lensV;
            QYTConcentricSampleDisk(sample.lensU, sample.lensV, &lensU, &lensV);
            lensU *= lenRadius;
            lensV *= lenRadius;
            
            float ft = focalDistance/rd->d.z;
            QYTPoint3 pFoucs = (*rd)(ft);
            
            rd->o = QYTPoint3(lensU, lensV, 0.f);
            rd->d = QYTVector3::Normalize(pFoucs - rd->o);
        }
        
        rd->rxOrigin = rd->ryOrigin = rd->o;
        rd->rxDirection = QYTVec3::Normalize(QYTVec3(pCamera) + dxCamera);
        rd->ryDirection = QYTVec3::Normalize(QYTVec3(pCamera) + dyCamera);
        rd->time = sample.time;
        cameraToWorld(*rd, rd);
        
        rd->hasDifferentials = true;
        return 1.f;
    }
}