//
//  QYTShpere.h
//  qyt_render_ver1.5
//
//  Created by nocolor on 15/2/19.
//  Copyright (c) 2015年 ___JinJiangTiaoZhanBei___. All rights reserved.
//

#ifndef __qyt_render_ver1_5__QYTShpere__
#define __qyt_render_ver1_5__QYTShpere__

#include "QYTShape.h"

namespace QYT
{
    /**
     @class QYTSphere
     参数化的球体对象，定义在局部坐标系中。
     通过指定纵向切角和水平切角来产生完整球体或者半球、部分球面。
     */
    class QYTSphere:public QYTShape
    {
    private:
        QYTReal radius;
        QYTReal phiMax;
        QYTReal zmin, zmax;
        QYTReal thetaMin, thetaMax;
        
    public:
        QYTSphere(Transfrom_sptr w2o, bool ro, QYTReal rad,
                  QYTReal z0, QYTReal z1, QYTReal phiMax);
        
        QYTBBox objectBound() const override;
        
        virtual bool intersect(const QYTRay& ray,
                               QYTReal* tHit,
                               QYTReal* rayEpsilon,
                               QYTDifferentialGeometry* dg) const override;
        
        QYTReal area() const override;
        
        bool intersectP(const QYTRay& ray) const override;

        
        QYTPoint3 sample(QYTReal u1, QYTReal u2, QYTNormal3 *Ns) const override;
        
        
        QYTPoint3 sample(const QYTPoint3 &P,
                                 QYTReal u1, QYTReal u2,
                                 QYTNormal3 *Ns) const override;
        
        QYTReal pdf(const QYTPoint3 &p, const QYTVector3 &wi) const override;
    };
}

#endif /* defined(__qyt_render_ver1_5__QYTShpere__) */
