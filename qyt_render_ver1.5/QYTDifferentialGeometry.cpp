//
//  QYTDifferentialGeometry.cpp
//  lightCoreRender
//
//  Created by nocolor on 14/11/26.
//  Copyright (c) 2014年 ___NOCOLOR___. All rights reserved.
//

#include "QYTDifferentialGeometry.h"
#include "QYTShape.h"

namespace QYT
{
    QYTDifferentialGeometry::QYTDifferentialGeometry(const QYTPoint3& _p,
                                                     const QYTVec3& _dpdu,
                                                     const QYTVec3& _dpdv,
                                                     const QYTNormal3& _dndu,
                                                     const QYTNormal3& _dndv,
                                                     float _u, float _v,
                                                     const QYTShape* _shape):p(_p), dpdu(_dpdu), dpdv(_dpdv),dndu(_dndu), dndv(_dndv)
    {
        n = QYTNormal3(QYTVector3::Normalize(QYTVector3::Cross(dpdu, dpdv)));
        u = _u;
        v = _v;
        shape = _shape;
        
        dudx = dvdx = dudy = dvdy = 0;
        
        if (shape &&
            (shape->reverseOrientation ^ shape->transformSwapsHandedness))
            n *= -1.f;
    }
}