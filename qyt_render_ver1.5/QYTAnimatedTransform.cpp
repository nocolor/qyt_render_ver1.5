//
//  QYTAnimatedTransform.cpp
//  qyt_render_ver1.5
//
//  Created by nocolor on 15/3/2.
//  Copyright (c) 2015年 ___JinJiangTiaoZhanBei___. All rights reserved.
//

#include "QYTAnimatedTransform.h"

namespace QYT
{
    void QYTAnimatedTransform::Decompose(const QYTMat4 &m, QYTVec3 *T, QYT::QYTQuaternion *Rquat, QYTMat4 *S)
    {

        T->x = m.m[0][3];
        T->y = m.m[1][3];
        T->z = m.m[2][3];
        

        QYTMat4 M = m;
        for (int i = 0; i < 3; ++i)
            M.m[i][3] = M.m[3][i] = 0.f;
        M.m[3][3] = 1.f;

        

        float norm;
        int count = 0;
        QYTMat4 R = M;
        
        do {

            QYTMat4 Rnext;
            QYTMat4 Rit = !(~R);
            for (int i = 0; i < 4; ++i)
                for (int j = 0; j < 4; ++j)
                    Rnext.m[i][j] = 0.5f * (R.m[i][j] + Rit.m[i][j]);
            
            norm = 0.f;
            for (int i = 0; i < 3; ++i) {
                float n = fabsf(R.m[i][0] - Rnext.m[i][0]) +
                fabsf(R.m[i][1] - Rnext.m[i][1]) +
                fabsf(R.m[i][2] - Rnext.m[i][2]);
                norm = std::max(norm, n);
            }
            R = Rnext;
        } while (++count < 100 && norm > .0001f);
        
        *Rquat = QYTQuaternion(R);
        *S = (!R)*M;
    }
    
    void QYTAnimatedTransform::interpolate(float time, QYT::QYTTransform *t) const
    {
        if (!actuallyAnimated || time <= startTime)
        {
            *t = *startTransform;
            return;
        }
        if (time >= endTime)
        {
            *t = *endTransform;
            return;
        }
        
        float dt = (time - startTime) / (endTime - startTime);

        QYTVec3 trans = (1.f - dt) * T[0] + dt * T[1];
        

        QYTQuaternion rotate = QYTQuaternion::Slerp(dt, R[0], R[1]);
        
        QYTMat4 scale;
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
                scale.m[i][j] = QYTLerp(dt, S[0].m[i][j], S[1].m[i][j]);
        
        *t = QYTTranslate(trans.x, trans.y, trans.z) * rotate.toTransform() * QYTTransform(scale);
    }
    
    QYTBBox QYTAnimatedTransform::motionBounds(const QYT::QYTBBox &b, bool useInverse) const
    {
        if(!actuallyAnimated)
            return (!(*startTransform))(b);
        QYTBBox ret;
        const int nSteps = 128;
        for (int i = 0; i < nSteps; ++i) {
            QYTTransform t;
            float time = QYTLerp(float(i)/float(nSteps-1), startTime, endTime);
            interpolate(time, &t);
            if (useInverse)
                t = !t;
            ret = unionWithBBox(ret, t(b));
        }
        return ret;

    }
    
    void QYTAnimatedTransform::operator()(const QYTRay &r, QYTRay *tr) const
    {
        if (!actuallyAnimated || r.time <= startTime)
            *tr = (*startTransform)(r);
        else if (r.time >= endTime)
            *tr = (*endTransform)(r);
        else {
            QYTTransform t;
            interpolate(r.time, &t);
            *tr = t(r);
        }
        tr->time = r.time;
    }
    
    void QYTAnimatedTransform::operator()(const QYTRayDifferential &r,
                                       QYTRayDifferential *tr) const
    {
        if (!actuallyAnimated || r.time <= startTime)
            *tr = (*startTransform)(r);
        else if (r.time >= endTime)
            *tr = (*endTransform)(r);
        else {
            QYTTransform t;
            interpolate(r.time, &t);
            *tr = t(r);
        }
        tr->time = r.time;
    }
    
    QYTPoint3 QYTAnimatedTransform::operator()(float time, const QYTPoint3 &p) const
    {
        if (!actuallyAnimated || time <= startTime)
            return (*startTransform)(p);
        else if (time >= endTime)
            return (*endTransform)(p);
        QYTTransform t;
        interpolate(time, &t);
        return t(p);
    }
    
    QYTVec3 QYTAnimatedTransform::operator()(float time, const QYTVec3 &v) const
    {
        if (!actuallyAnimated || time <= startTime)
            return (*startTransform)(v);
        else if (time >= endTime)
            return (*endTransform)(v);
        QYTTransform t;
        interpolate(time, &t);
        return t(v);
    }
    
    QYTRay QYTAnimatedTransform::operator()(const QYTRay &r) const {
        QYTRay ret;
        (*this)(r, &ret);
        return ret;
    }
}
