//
//  QYTLambertian.h
//  qyt_render_ver1.5
//
//  Created by nocolor on 15/3/17.
//  Copyright (c) 2015年 ___JinJiangTiaoZhanBei___. All rights reserved.
//

#ifndef __qyt_render_ver1_5__QYTLambertian__
#define __qyt_render_ver1_5__QYTLambertian__

#include "QYTReflection.h"

namespace QYT
{
    class QYTLambertian:public QYTBxDF
    {
    private:
        QYTSpectrum r;
        
    public:
        QYTLambertian(const QYTSpectrum& reflectance):
        QYTBxDF(QYTBxDF_TYPE(BSDF_REFLECTION | BSDF_DIFFUSE)), r(reflectance){}
        QYTSpectrum f(const QYTVec3& wo, const QYTVec3& wi) const override
        {return r*INV_PI;}
        
        QYTSpectrum rho(const QYTVec3& wo, int nSamples, const QYTReal* samples) const override
        {return r;}
        
        QYTSpectrum rho(const int nSamples, const QYTReal* samples1,
                        const QYTReal* samples2) const override
        {return r;}
    };
}

#endif /* defined(__qyt_render_ver1_5__QYTLambertian__) */
