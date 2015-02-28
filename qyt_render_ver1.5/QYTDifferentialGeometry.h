//
//  QYTDifferentialGeometry.h
//  lightCoreRender
//
//  Created by nocolor on 14/11/26.
//  Copyright (c) 2014年 ___NOCOLOR___. All rights reserved.
//

#ifndef __lightCoreRender__QYTDifferentialGeometry__
#define __lightCoreRender__QYTDifferentialGeometry__

#include <iostream>
#include "QYTVector3.h"
#include "QYTPoint3.h"
#include "qytr.h"


namespace QYT
{
    class QYTShape;
    
    class QYTDifferentialGeometry
    {
    public:
        QYTPoint3      p;
        QYTNormal3     n;
        float          u, v;
        QYTVec3        dpdu, dpdv;
        QYTVec3        dndu, dndv;
        const QYTShape* shape;
        
        mutable QYTVec3    dpdx, dpdy;
        mutable float       dudx, dvdx, dudy, dvdy;
        
        QYTDifferentialGeometry()
        {
            u = v = dudx = dvdx = dudy = dvdy = 0.;
            shape = NULL;
        }
        
        QYTDifferentialGeometry(const QYTPoint3& _p,
                                const QYTVec3& _dpdu,
                                const QYTVec3& _dpdv,
                                const QYTNormal3& _dndu,
                                const QYTNormal3& _dndv,
                                float _u, float _v,
                                const QYTShape* _shape);
    };
}

#endif /* defined(__lightCoreRender__QYTDifferentialGeometry__) */
