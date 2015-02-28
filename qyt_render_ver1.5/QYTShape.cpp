//
//  QYTShape.cpp
//  lightCoreRender
//
//  Created by nocolor on 14/11/26.
//  Copyright (c) 2014年 ___NOCOLOR___. All rights reserved.
//

#include "QYTShape.h"

namespace QYT
{
    
    QYTBBox QYTShape::worldBound() const
    {
        return objectBound();
    }
    
    bool QYTShape::intersect(const QYTRay &ray, QYTReal *tHit, QYTReal *rayEpsilon, QYTDifferentialGeometry *dg) const
    {
        QYTSevere("Unimplemented QYTShape::intersect() method called");
        return false;
    }
    
    bool QYTShape::intersectP(const QYTRay &ray) const
    {
        QYTSevere("Unimplemented QYTShape::intersectP() method called");
        return false;
    }
    
    QYTReal QYTShape::area() const
    {
        QYTSevere("Unimplemented QYTShape::area() method called");
        return false;
    }
    
    QYTPoint3 QYTShape::sample(QYTReal u1, QYTReal u2, QYTNormal3 *Ns) const
    {
        QYTSevere("Unimplemented QYTShape::sample() method called");
        return QYTPoint3();
    }
    
    QYTPoint3 QYTShape::pdf(const QYT::QYTPoint3 &Pshape) const
    {
        return 1.f/area();
    }
    
    QYTPoint3 QYTShape::sample(const QYT::QYTPoint3 &P, QYTReal u1, QYTReal u2, QYTNormal3 *Ns) const
    {
        return sample(u1, u2, Ns);
    }
    
    QYTReal QYTShape::pdf(const QYT::QYTPoint3 &p, const QYT::QYTVector3 &wi) const
    {
        // Intersect sample ray with area light geometry
        QYTDifferentialGeometry dgLight;
        QYTRay ray(p, wi, 1e-3f);
        ray.depth = -1; // temporary hack to ignore alpha mask
        QYTReal thit, rayEpsilon;
        if (!intersect(ray, &thit, &rayEpsilon, &dgLight))
            return 0.;
        
        // Convert light sample weight to solid angle measure
        QYTReal pdf = QYTPoint3::DistanceSquared(p, ray(thit)) /
        (QYTVector3::AbsDot(dgLight.n, -wi) * area());
        if (isinf(pdf))
            pdf = 0.f;
        return pdf;
    }
}