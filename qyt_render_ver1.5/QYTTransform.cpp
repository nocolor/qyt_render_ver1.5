//
//  QYTTransform.cpp
//  qyt_render_ver1.5
//
//  Created by nocolor on 15/2/18.
//  Copyright (c) 2015年 ___JinJiangTiaoZhanBei___. All rights reserved.
//

#include "QYTTransform.h"

namespace QYT
{
    bool QYTTransform::operator==(const QYT::QYTTransform &rhs) const
    {
        return (m == rhs.m) && (mInv == rhs.mInv);
    }
    
    bool QYTTransform::operator!=(const QYT::QYTTransform &rhs) const
    {
        return !(*this == rhs);
    }
    
    QYTTransform QYTTransform::operator*(const QYT::QYTTransform &t) const
    {
        QYTMat4 m1 = m*t.m;
        QYTMat4 m2 = t.mInv*mInv;
        return QYTTransform(m1, m2);
    }
    
    QYTTransform& QYTTransform::operator*=(const QYT::QYTTransform &t)
    {
        if(this != &t)
            *this = *this*t;
        return *this;
    }
    
    QYTTransform QYTTransform::operator~() const
    {
        return QYTTransform(~m, ~mInv);
    }
    
    QYTTransform QYTTransform::operator!() const
    {
        return QYTTransform(mInv, m);
    }
    
    QYTVector3 QYTTransform::operator()(const QYT::QYTVector3 &v) const
    {
        QYTReal x = v.x, y = v.y, z = v.z;
        return QYTVector3(m.m[0][0]*x + m.m[0][1]*y + m.m[0][2]*z,
                          m.m[1][0]*x + m.m[1][1]*y + m.m[1][2]*z,
                          m.m[2][0]*x + m.m[2][1]*y + m.m[2][2]*z);
    }
    
    QYTPoint3 QYTTransform::operator()(const QYT::QYTPoint3 &p) const
    {
        QYTReal x = p.x, y = p.y, z = p.z;
        QYTReal xp = m.m[0][0]*x + m.m[0][1]*y + m.m[0][2]*z + m.m[0][3];
        QYTReal yp = m.m[1][0]*x + m.m[1][1]*y + m.m[1][2]*z + m.m[1][3];
        QYTReal zp = m.m[2][0]*x + m.m[2][1]*y + m.m[2][2]*z + m.m[2][3];
        QYTReal wp = m.m[3][0]*x + m.m[3][1]*y + m.m[3][2]*z + m.m[3][3];
        Assert(wp != 0);
        if (wp == 1.)
            return QYTPoint3(xp, yp, zp);
        else
            return QYTPoint3(xp, yp, zp)/wp;
    }
    
    QYTRay QYTTransform::operator()(const QYTRay &ray) const
    {
        return QYTRay((*this)(ray.o),
                      (*this)(ray.d),
                      ray.min_t,
                      ray.max_t,
                      ray.time,
                      ray.depth);
    }
    
    QYTBBox QYTTransform::operator()(const QYT::QYTBBox &b) const
    {
        const QYTTransform &M = *this;
        QYTBBox ret(M(QYTPoint3(b.p_min.x, b.p_min.y, b.p_min.z)));
        ret = unionWithPoint(ret, M(QYTPoint3(b.p_max.x, b.p_min.y, b.p_min.z)));
        ret = unionWithPoint(ret, M(QYTPoint3(b.p_min.x, b.p_max.y, b.p_min.z)));
        ret = unionWithPoint(ret, M(QYTPoint3(b.p_min.x, b.p_min.y, b.p_max.z)));
        ret = unionWithPoint(ret, M(QYTPoint3(b.p_min.x, b.p_max.y, b.p_max.z)));
        ret = unionWithPoint(ret, M(QYTPoint3(b.p_max.x, b.p_max.y, b.p_min.z)));
        ret = unionWithPoint(ret, M(QYTPoint3(b.p_max.x, b.p_min.y, b.p_max.z)));
        ret = unionWithPoint(ret, M(QYTPoint3(b.p_max.x, b.p_max.y, b.p_max.z)));
        return ret;
    }
    
    bool QYTTransform::hasScale() const
    {
        float la2 = QYTVector3::LengthSquared((*this)(QYTVector3(1,0,0)));
        float lb2 = QYTVector3::LengthSquared((*this)(QYTVector3(0,1,0)));
        float lc2 = QYTVector3::LengthSquared((*this)(QYTVector3(0,0,1)));
#define NOT_ONE(x) ((x) < .999f || (x) > 1.001f)
        return (NOT_ONE(la2) || NOT_ONE(lb2) || NOT_ONE(lc2));
#undef NOT_ONE
    }
    
    bool QYTTransform::swapsHandedness() const
    {
        QYTReal det = ((m.m[0][0] *
                      (m.m[1][1] * m.m[2][2] -
                       m.m[1][2] * m.m[2][1])) -
                     (m.m[0][1] *
                      (m.m[1][0] * m.m[2][2] -
                       m.m[1][2] * m.m[2][0])) +
                     (m.m[0][2] *
                      (m.m[1][0] * m.m[2][1] -
                       m.m[1][1] * m.m[2][0])));
        return det < 0.f;
    }
    
    QYTTransform QYTTransform::MakeLCRTransfrom(const QYT::QYTTransform &T)
    {
        return LCR_TO_PBRT*(!T);
    }
    
}