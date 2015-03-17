//
//  QYTMaterial.cpp
//  qyt_render_ver1.5
//
//  Created by nocolor on 15/3/17.
//  Copyright (c) 2015年 ___JinJiangTiaoZhanBei___. All rights reserved.
//

#include "QYTMaterial.h"
#include "QYTShape.h"

namespace QYT
{
    QYTMaterial::~QYTMaterial()
    {}
    
    void QYTMaterial::Bump(const QYTTexture<float> &d, const QYT::QYTDifferentialGeometry &dgGeom, const QYT::QYTDifferentialGeometry &dgs, QYT::QYTDifferentialGeometry *dgBump)
    {
        QYTDifferentialGeometry dgEval = dgs;
        
        // Shift _dgEval_ _du_ in the $u$ direction
        float du = .5f * (fabsf(dgs.dudx) + fabsf(dgs.dudy));
        if (du == 0.f) du = .01f;
        dgEval.p = dgs.p + du * dgs.dpdu;
        dgEval.u = dgs.u + du;
        dgEval.n = QYTVec3::Normalize((QYTNormal3)QYTVec3::Cross(dgs.dpdu, dgs.dpdv) +
                              du * dgs.dndu);
        float uDisplace = d.evaluate(dgEval);
        
        // Shift _dgEval_ _dv_ in the $v$ direction
        float dv = .5f * (fabsf(dgs.dvdx) + fabsf(dgs.dvdy));
        if (dv == 0.f) dv = .01f;
        dgEval.p = dgs.p + dv * dgs.dpdv;
        dgEval.u = dgs.u;
        dgEval.v = dgs.v + dv;
        dgEval.n = QYTVec3::Normalize((QYTNormal3)QYTVec3::Cross(dgs.dpdu, dgs.dpdv) +
                              dv * dgs.dndv);
        float vDisplace = d.evaluate(dgEval);
        float displace = d.evaluate(dgs);
        
        // Compute bump-mapped differential geometry
        *dgBump = dgs;
        dgBump->dpdu = dgs.dpdu + (uDisplace - displace) / du * QYTVec3(dgs.n) +
        displace * QYTVec3(dgs.dndu);
        dgBump->dpdv = dgs.dpdv + (vDisplace - displace) / dv * QYTVec3(dgs.n) +
        displace * QYTVec3(dgs.dndv);
        dgBump->n = QYTNormal3(QYTVec3::Normalize(QYTVec3::Cross(dgBump->dpdu, dgBump->dpdv)));
        if (dgs.shape->reverseOrientation ^ dgs.shape->transformSwapsHandedness)
            dgBump->n *= -1.f;
        
        // Orient shading normal to match geometric normal
        dgBump->n = QYTFaceforward(dgBump->n, dgGeom.n);
    }
}