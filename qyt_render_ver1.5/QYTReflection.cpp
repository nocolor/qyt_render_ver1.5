//
//  QYTReflection.cpp
//  qyt_render_ver1.5
//
//  Created by nocolor on 15/3/14.
//  Copyright (c) 2015年 ___JinJiangTiaoZhanBei___. All rights reserved.
//

#include "QYTReflection.h"
#include "QYTFucAPI.h"

namespace QYT
{
    QYTSpectrum QYTBxDF::sample_f(const QYTVec3 &wo, QYTVec3 *wi, QYTReal u1, QYTReal u2, QYTReal *pdf) const
    {
        *wi = qyt_cosine_sample_hemisphere(u1, u2);
        
        if (wo.z < 0.) wi->z *= -1.f;
        *pdf = this->pdf(wo, *wi);
        return f(wo, *wi);
    }
    
    QYTReal QYTBxDF::pdf(const QYTVec3 &wi, const QYTVec3 &wo) const
    {
        return QYTSameHemisphere(wo, wi) ? qyt_abs_cos_theta(wi) * INV_PI : 0.f;
    }
    
    QYTSpectrum QYTBxDF::rho(const QYTVec3 &wo, int nSamples, const QYTReal *samples) const
    {
        QYTSpectrum r = 0.;
        for (int i = 0; i < nSamples; ++i) {
            // Estimate one term of $\rho_\roman{hd}$
            QYTVec3 wi;
            float pdf = 0.f;
            QYTSpectrum f = sample_f(wo, &wi, samples[2*i], samples[2*i+1], &pdf);
            if (pdf > 0.)
                r += f * qyt_abs_cos_theta(wi) / pdf;
        }
        return r / float(nSamples);
    }
    
    QYTSpectrum QYTBxDF::rho(int nSamples, const QYTReal *samples1, const QYTReal *samples2) const
    {
        QYTSpectrum r = 0.;
        for (int i = 0; i < nSamples; ++i) {
            // Estimate one term of $\rho_\roman{hh}$
            QYTVec3 wo, wi;
            wo = qyt_uniform_sample_hemisphere(samples1[2*i], samples1[2*i+1]);
            float pdf_o = INV_TWOPI, pdf_i = 0.f;
            QYTSpectrum f = sample_f(wo, &wi, samples2[2*i], samples2[2*i+1], &pdf_i);
            if (pdf_i > 0.)
                r += f * qyt_abs_cos_theta(wi) * qyt_abs_cos_theta(wo) / (pdf_o * pdf_i);
        }
        return r / (M_PI*nSamples);
    }
}
