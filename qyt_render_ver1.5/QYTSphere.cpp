//
//  QYTShpere.cpp
//  qyt_render_ver1.5
//
//  Created by nocolor on 15/2/19.
//  Copyright (c) 2015年 ___JinJiangTiaoZhanBei___. All rights reserved.
//

#include "QYTSphere.h"

namespace QYT
{
    QYTSphere::QYTSphere(Transfrom_sptr w2o, bool ro, QYTReal rad,
                         QYTReal z0, QYTReal z1, QYTReal pMax):
    QYTShape(w2o, ro)
    {
        radius = rad;
        zmin = QYTClamp(std::min(z0, z1), -radius, radius);
        zmax = QYTClamp(std::max(z0, z1), -radius, radius);
        thetaMin = acosf(QYTClamp(zmin/radius, -1.f, 1.f));
        thetaMax = acosf(QYTClamp(zmax/radius, -1.f, 1.f));
        phiMax = QYTRadians(QYTClamp(pMax, 0.0f, 360.0f));
    }
    
    QYTBBox QYTSphere::objectBound() const
    {
        return QYTBBox(QYTPoint3(-radius, -radius, zmin),
                       QYTPoint3( radius,  radius, zmax));
    }
    
    bool QYTSphere::intersect(const QYTRay &r, QYTReal *tHit, QYTReal *rayEpsilon, QYT::QYTDifferentialGeometry *dg) const
    {
        QYTReal phi;
        QYTPoint3 phit;
        //将光线从世界坐标转换到局部坐标
        QYTRay ray = (*worldToObject)(r);
        
        //计算求交方程的参数
        QYTReal A = ray.d.x*ray.d.x + ray.d.y*ray.d.y + ray.d.z*ray.d.z;
        QYTReal B = 2 * (ray.d.x*ray.o.x + ray.d.y*ray.o.y + ray.d.z*ray.o.z);
        QYTReal C = ray.o.x*ray.o.x + ray.o.y*ray.o.y +
        ray.o.z*ray.o.z - radius*radius;
        
        //联合射线方程和球体方程求解
        QYTReal t0, t1;
        if (!QYTQuadratic(A, B, C, &t0, &t1))
            return false;
        
        //计算交点处的t值
        if (t0 > ray.max_t || t1 < ray.min_t)
            return false;
        QYTReal thit = t0;
        if (t0 < ray.min_t) {
            thit = t1;
            if (thit > ray.max_t) return false;
        }
        
        //计算交点（如果有交点）的球面坐标
        phit = ray(thit);
        if (phit.x == 0.f && phit.y == 0.f) phit.x = 1e-5f * radius;
        phi = atan2f(phit.y, phit.x);
        if (phi < 0.) phi += 2.f*M_PI;
        
        //将交点的球面坐标和球体参数比较，看交点是否位于定义的球面区域内
        if ((zmin > -radius && phit.z < zmin) ||
            (zmax <  radius && phit.z > zmax) || phi > phiMax) {
            if (thit == t1) return false;
            if (t1 > ray.max_t) return false;
            thit = t1;
            //计算交点坐标
            phit = ray(thit);
            if (phit.x == 0.f && phit.y == 0.f) phit.x = 1e-5f * radius;
            phi = atan2f(phit.y, phit.x);
            if (phi < 0.) phi += 2.f*M_PI;
            if ((zmin > -radius && phit.z < zmin) ||
                (zmax <  radius && phit.z > zmax) || phi > phiMax)
                return false;
        }
        
        //交点的参数化形式
        QYTReal u = phi / phiMax;
        QYTReal theta = acosf(QYTClamp(phit.z / radius, -1.f, 1.f));
        QYTReal v = (theta - thetaMin) / (thetaMax - thetaMin);
        
        //计算交点沿着u方向和v方向的偏导数
        QYTReal zradius = sqrtf(phit.x*phit.x + phit.y*phit.y);
        QYTReal invzradius = 1.f / zradius;
        QYTReal cosphi = phit.x * invzradius;
        QYTReal sinphi = phit.y * invzradius;
        QYTVec3 dpdu(-phiMax * phit.y, phiMax * phit.x, 0);
        QYTVec3 dpdv = (thetaMax-thetaMin) *
        QYTVec3(phit.z * cosphi, phit.z * sinphi,
               -radius * sinf(theta));
        
        //计算法向量沿着u方向和v方向的偏导数
        QYTVec3 d2Pduu = -phiMax * phiMax * QYTVec3(phit.x, phit.y, 0);
        QYTVec3 d2Pduv = (thetaMax - thetaMin) * phit.z * phiMax *
        QYTVec3(-sinphi, cosphi, 0.);
        QYTVec3 d2Pdvv = -(thetaMax - thetaMin) * (thetaMax - thetaMin) *
        QYTVec3(phit.x, phit.y, phit.z);
        
        //计算参数的标准化形式
        QYTReal E = QYTVec3::Dot(dpdu, dpdu);
        QYTReal F = QYTVec3::Dot(dpdu, dpdv);
        QYTReal G = QYTVec3::Dot(dpdv, dpdv);
        QYTVec3 N = QYTVec3::Normalize(QYTVec3::Cross(dpdu, dpdv));
        QYTReal e = QYTVec3::Dot(N, d2Pduu);
        QYTReal f = QYTVec3::Dot(N, d2Pduv);
        QYTReal g = QYTVec3::Dot(N, d2Pdvv);
        
        QYTReal invEGF2 = 1.f / (E*G - F*F);
        QYTNormal3 dndu = QYTNormal3((f*F - e*G) * invEGF2 * dpdu +
                                     (e*F - f*E) * invEGF2 * dpdv);
        QYTNormal3 dndv = QYTNormal3((g*F - f*G) * invEGF2 * dpdu +
                                     (f*F - g*E) * invEGF2 * dpdv);
        
        //填充QYTDifferentialGeometry
        const QYTTransform &o2w = !(*worldToObject);
        *dg = QYTDifferentialGeometry(o2w(phit), o2w(dpdu), o2w(dpdv),
                                      o2w(dndu), o2w(dndv), u, v, this);
        
        //设置交点的t值
        *tHit = thit;
        
        //设置rayEpsilon用于避免自相交
        *rayEpsilon = 5e-4f * *tHit;
        return true;
    }
    
    bool QYTSphere::intersectP(const QYTRay &r) const
    {
        QYTReal phi;
        QYTPoint3 phit;
        //将光线从世界坐标转换到局部坐标
        QYTRay ray = (*worldToObject)(r);
        
        //计算求交方程的参数
        QYTReal A = ray.d.x*ray.d.x + ray.d.y*ray.d.y + ray.d.z*ray.d.z;
        QYTReal B = 2 * (ray.d.x*ray.o.x + ray.d.y*ray.o.y + ray.d.z*ray.o.z);
        QYTReal C = ray.o.x*ray.o.x + ray.o.y*ray.o.y +
        ray.o.z*ray.o.z - radius*radius;
        
        //联合射线方程和球体方程求解
        QYTReal t0, t1;
        if (!QYTQuadratic(A, B, C, &t0, &t1))
            return false;
        
        //计算交点处的t值
        if (t0 > ray.max_t || t1 < ray.min_t)
            return false;
        QYTReal thit = t0;
        if (t0 < ray.min_t) {
            thit = t1;
            if (thit > ray.max_t) return false;
        }
        
        //计算交点（如果有交点）的球面坐标
        phit = ray(thit);
        if (phit.x == 0.f && phit.y == 0.f) phit.x = 1e-5f * radius;
        phi = atan2f(phit.y, phit.x);
        if (phi < 0.) phi += 2.f*M_PI;
        
        //将交点的球面坐标和球体参数比较，看交点是否位于定义的球面区域内
        if ((zmin > -radius && phit.z < zmin) ||
            (zmax <  radius && phit.z > zmax) || phi > phiMax) {
            if (thit == t1) return false;
            if (t1 > ray.max_t) return false;
            thit = t1;
            //计算交点坐标
            phit = ray(thit);
            if (phit.x == 0.f && phit.y == 0.f) phit.x = 1e-5f * radius;
            phi = atan2f(phit.y, phit.x);
            if (phi < 0.) phi += 2.f*M_PI;
            if ((zmin > -radius && phit.z < zmin) ||
                (zmax <  radius && phit.z > zmax) || phi > phiMax)
                return false;
        }
        return true;
    }
    
    QYTReal QYTSphere::area() const
    {
        return phiMax*radius*(zmax-zmin);
    }
    
    QYTPoint3 QYTSphere::sample(QYTReal u1, QYTReal u2, QYTNormal3 *Ns) const
    {
        //将u1和u2映射到球面坐标
        QYTReal z = 1.f - 2.f * u1;
        QYTReal r = sqrtf(std::max(0.f, 1.f - z*z));
        QYTReal phi = 2.f * M_PI * u2;
        QYTReal x = r * cosf(phi);
        QYTReal y = r * sinf(phi);
        
        QYTPoint3 p = QYTPoint3(0.f) + radius*QYTVec3(x, y, z);
        *Ns = QYTVec3::Normalize((!(*worldToObject))(QYTNormal3(p.x, p.y, p.z)));
        if(reverseOrientation)
            *Ns = -(*Ns);
        return (!(*worldToObject))(p);
    }
    
    QYTPoint3 QYTSphere::sample(const QYT::QYTPoint3 &p, QYTReal u1, QYTReal u2, QYTNormal3 *Ns) const
    {
        // Compute coordinate system for sphere sampling
        QYTPoint3 Pcenter = (!(*worldToObject))(QYTPoint3(0,0,0));
        QYTVec3 wc = QYTVec3::Normalize(Pcenter - p);
        QYTVec3 wcX, wcY;
        QYTVec3::CoordinateSystem(wc, &wcX, &wcY);
        
        // Sample uniformly on sphere if $\pt{}$ is inside it
        if (QYTPoint3::DistanceSquared(p, Pcenter) - radius*radius < 1e-4f)
            return sample(u1, u2, Ns);
        
        // Sample sphere uniformly inside subtended cone
        QYTReal sinThetaMax2 = radius*radius / QYTPoint3::DistanceSquared(p, Pcenter);
        QYTReal cosThetaMax = sqrtf(std::max(0.f, 1.f - sinThetaMax2));
        QYTDifferentialGeometry dgSphere;
        QYTReal thit, rayEpsilon;
        QYTPoint3 ps;
        
        QYTReal costheta = QYTLerp(u1, cosThetaMax, 1.f);
        QYTReal sintheta = sqrtf(1.f - costheta*costheta);
        QYTReal phi = u2 * 2.f * M_PI;
        QYTVec3 tmp_v(cosf(phi)*sintheta*wcX + sinf(phi)*sintheta*wcY+ costheta*wc);
        
        QYTRay r(p, tmp_v, 1e-3f);
        if (!intersect(r, &thit, &rayEpsilon, &dgSphere))
            thit = QYTVec3::Dot(Pcenter - p, QYTVec3::Normalize(r.d));
        ps = r(thit);
        *Ns = QYTNormal3(QYTVec3::Normalize(ps - Pcenter));
        if (reverseOrientation) *Ns *= -1.f;
        return ps;
    }
    
    QYTReal QYTSphere::pdf(const QYT::QYTPoint3 &p, const QYT::QYTVector3 &wi) const
    {
        QYTPoint3 Pcenter = (!(*worldToObject))(QYTPoint3(0,0,0));
        // Return uniform weight if point inside sphere
        if (QYTPoint3::DistanceSquared(p, Pcenter) - radius*radius < 1e-4f)
            return QYTShape::pdf(p, wi);
        
        // Compute general sphere weight
        float sinThetaMax2 = radius*radius / QYTPoint3::DistanceSquared(p, Pcenter);
        float cosThetaMax = sqrtf(std::max(0.f, 1.f - sinThetaMax2));
        return 1.f / (2.f * M_PI * (1.f - cosThetaMax));
    }
}
