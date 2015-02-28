//
//  QYTThinLensCamera.cpp
//  QYTRayTracer
//
//  Created by nocolor on 14-4-14.
//  Copyright (c) 2014年 QinYuTong. All rights reserved.
//

#include "QYTThinLensCamera.h"

namespace QYT
{
    QYTVec3 QYTThinLensCamera::getDirection(const QYTSampler::QYTPoint2d &pixelPoint, const QYTSampler::QYTPoint2d &lensPoint) const
    {
        QYTSampler::QYTPoint2d p;
        p.x = pixelPoint.x*f/d;
        p.y = pixelPoint.y*f/d;
        
        QYTVec3 dir = (p.x - lensPoint.x)*u +
        (p.y - lensPoint.y)*v - f*w;
        dir = QYT::QYTVector3::Normalize(dir);
        
        return dir;
    }
    
    
}