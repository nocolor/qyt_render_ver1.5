//
//  QYTThinLensCamera.h
//  QYTRayTracer
//
//  Created by nocolor on 14-4-14.
//  Copyright (c) 2014年 QinYuTong. All rights reserved.
//

#ifndef __QYTRayTracer__QYTThinLensCamera__
#define __QYTRayTracer__QYTThinLensCamera__

#include "QYTCamera.h"
#include "QYTSampler.h"

namespace QYT
{
    /**
     @class QYTThinLensCamera
     简化的薄透镜相机，支持调整焦距的操作，可以产生聚焦和失焦的模糊效果。
     薄透镜相机忽略透镜的厚度以及相关的光学折射现象。
     */
    class QYTThinLensCamera: public QYTCamera
    {
    private:
        ///薄透镜的半径
        float lensRadius;
        
        ///薄透镜与视点的距离
        float d;
        
        ///
        float f;
        float zoom;
        std::weak_ptr<QYTSampler> samplerPtr;
        
    public:
        
        inline void setLensRadius(float _lensRadius)
        {
            lensRadius = _lensRadius;
        }
        
        inline void setViewDistance(float _d)
        {
            d = _d;
        }
        
        inline void setFocalDistance(float _f)
        {
            f = _f;
        }
        
        inline void setZoom(float _zoom)
        {
            zoom = _zoom;
        }
        
        QYTThinLensCamera():
        QYTCamera(),
        lensRadius(1.0),
        d(40.0),
        f(1.0),
        zoom(1.0),
        samplerPtr()
        {}
        
        QYTThinLensCamera(const QYTThinLensCamera& rhs):
        QYTCamera(rhs),
        lensRadius(rhs.lensRadius),
        d(rhs.d),
        f(rhs.f),
        zoom(rhs.zoom),
        samplerPtr(rhs.samplerPtr)
        {

        }
        
        QYTThinLensCamera& operator = (const QYTThinLensCamera& rhs)
        {
            if(this == &rhs)
                return *this;
            
            QYTCamera::operator=(rhs);
            lensRadius = rhs.lensRadius;
            d = rhs.d;
            f = rhs.f;
            zoom = rhs.zoom;
            
            samplerPtr = rhs.samplerPtr;
            
            return *this;
        }

        
        virtual QYTThinLensCamera* copy() const
        {
            return new QYTThinLensCamera(*this);
        }
        
        void setSampler(std::shared_ptr<QYTSampler> _sampler)
        {
            samplerPtr = _sampler;
        }
        
        QYTVec3 getDirection(const QYTSampler::QYTPoint2d& pixelPoint, const QYTSampler::QYTPoint2d& lensPoint) const;
    };
    
}

#endif /* defined(__QYTRayTracer__QYTThinLensCamera__) */
