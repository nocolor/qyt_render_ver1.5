//
//  qyt_bbox.h
//  qyt_fundation
//
//  Created by nocolor on 15/2/6.
//  Copyright (c) 2015年 ___JinJiangTiaoZhanBei___. All rights reserved.
//

#ifndef __qyt_fundation__qyt_bbox__
#define __qyt_fundation__qyt_bbox__

#include "QYTVector3.h"
#include "qyt_ray.h"
#include "qytr.h"

namespace QYT
{
    /**
     @class QYTBBox
     轴对称包围盒，用于加速光线求交。
     */
    class QYTBBox
    {
    public:
        QYTBBox()
        {
            p_min = QYTPoint3( INFINITY,  INFINITY,  INFINITY);
            p_max = QYTPoint3(-INFINITY, -INFINITY, -INFINITY);
        }
        
        QYTBBox(const QYTPoint3 &p) : p_min(p), p_max(p) { }
        
        QYTBBox(const QYTPoint3 &p1, const QYTPoint3 &p2)
        {
            p_min = QYTPoint3(std::min(p1.x, p2.x), std::min(p1.y, p2.y), std::min(p1.z, p2.z));
            p_max = QYTPoint3(std::max(p1.x, p2.x), std::max(p1.y, p2.y), std::max(p1.z, p2.z));
        }
        
        friend QYTBBox unionWithPoint(const QYTBBox &b, const QYTPoint3 &p)
        {
            QYTBBox ret = b;
            ret.p_min[0] = std::min(b.p_min[0], p[0]);
            ret.p_min[1] = std::min(b.p_min[1], p[1]);
            ret.p_min[2] = std::min(b.p_min[2], p[2]);
            ret.p_max[0] = std::max(b.p_max[0], p[0]);
            ret.p_max[1] = std::max(b.p_max[1], p[1]);
            ret.p_max[2] = std::max(b.p_max[2], p[2]);
            return ret;
        }
        
        friend QYTBBox unionWithBBox(const QYTBBox &b, const QYTBBox &b2)
        {
            QYTBBox ret;
            ret.p_min[0] = std::min(b.p_min[0], b2.p_min[0]);
            ret.p_min[1] = std::min(b.p_min[1], b2.p_min[1]);
            ret.p_min[2] = std::min(b.p_min[2], b2.p_min[2]);
            ret.p_max[0] = std::max(b.p_max[0], b2.p_max[0]);
            ret.p_max[1] = std::max(b.p_max[1], b2.p_max[1]);
            ret.p_max[2] = std::max(b.p_max[2], b2.p_max[2]);
            return ret;
        }
        
        bool overlaps(const QYTBBox &b) const
        {
            bool x = (p_max[0] >= b.p_min[0]) && (p_min[0] <= b.p_max[0]);
            bool y = (p_max[1] >= b.p_min[1]) && (p_min[1] <= b.p_max[1]);
            bool z = (p_max[2] >= b.p_min[2]) && (p_min[2] <= b.p_max[2]);
            return (x && y && z);
        }
        
        bool inside(const QYTPoint3 &pt) const
        {
            return (pt[0] >= p_min[0] && pt[0] <= p_max[0] &&
                    pt[1] >= p_min[1] && pt[1] <= p_max[1] &&
                    pt[2] >= p_min[2] && pt[2] <= p_max[2]);
        }
        
        void expand(float delta)
        {
            p_min -= QYTVec3(delta, delta, delta);
            p_max += QYTVec3(delta, delta, delta);
        }
        
        float surfaceArea() const
        {
            QYTVec3 d = p_max - p_min;
            return 2.f * (d[0] * d[1] + d[0] * d[2] + d[1] * d[2]);
        }
        
        float volume() const
        {
            QYTVec3 d = p_max - p_min;
            return d.x * d.y * d.z;
        }
        
        int maximumExtent() const
        {
            QYTVec3 diag = p_max - p_min;
            if (diag.x > diag.y && diag.x > diag.z)
                return 0;
            else if (diag.y > diag.z)
                return 1;
            else
                return 2;
        }
        
        const QYTPoint3 &operator[](int i) const
        {
            assert(i == 0 || i == 1);
            return (&p_min)[i];
        }
        
        QYTPoint3 &operator[](int i)
        {
            assert(i == 0 || i == 1);
            return (&p_min)[i];
        }
        
        QYTPoint3 lerp(float tx, float ty, float tz) const
        {
            return QYTPoint3(QYT::QYTLerp(tx, p_min.x, p_max.x),
                             QYT::QYTLerp(ty, p_min.y, p_max.y),
                             QYT::QYTLerp(tz, p_min.z, p_max.z));
        }
        
        QYTVec3 offset(const QYTPoint3 &p) const
        {
            return QYTVec3((p.x - p_min.x) / (p_max.x - p_min.x),
                            (p.y - p_min.y) / (p_max.y - p_min.y),
                            (p.z - p_min.z) / (p_max.z - p_min.z));
        }
        
        void boundingSphere(QYTPoint3 *c, float *rad) const;
        
        bool intersectP(const QYTRay &ray, float *hitt0 = NULL, float *hitt1 = NULL) const
        {
            float t0 = ray.min_t, t1 = ray.max_t;
            for (int i = 0; i < 3; ++i) {

                float invRayDir = 1.f / ray.d[i];
                float tNear = (p_min[i] - ray.o[i]) * invRayDir;
                float tFar  = (p_max[i] - ray.o[i]) * invRayDir;
                

                if (tNear > tFar) std::swap(tNear, tFar);
                t0 = tNear > t0 ? tNear : t0;
                t1 = tFar  < t1 ? tFar  : t1;
                if (t0 > t1) return false;
            }
            if (hitt0) *hitt0 = t0;
            if (hitt1) *hitt1 = t1;
            return true;
        }
        
        bool operator==(const QYTBBox &b) const
        {
            return b.p_min == p_min && b.p_max == p_max;
        }
        
        bool operator!=(const QYTBBox &b) const
        {
            return b.p_min != p_min || b.p_max != p_max;
        }
        
        QYTPoint3 p_min, p_max;
    };
}

#endif /* defined(__qyt_fundation__qyt_bbox__) */
