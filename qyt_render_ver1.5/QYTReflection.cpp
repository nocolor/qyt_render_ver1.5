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
    //*****************************************************************************************
    //QYTBxDF
    
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
    
    //end QYTBxDF
    //*****************************************************************************************
    
    //*****************************************************************************************
    //QYTBSDFSampleOffsets
    
    QYTBSDFSampleOffset::QYTBSDFSampleOffset(int count, QYTSample* sample)
    {
        nSamples = count;
        componentOffset = sample->add1D(nSamples);
        dirOffset = sample->add2D(nSamples);
    }
    //end QYTBSDFSampleOffsets
    //*****************************************************************************************
    
    //*****************************************************************************************
    //QYTBSDF
    
    QYTSpectrum QYTBSDF::sample_f(const QYTVec3& woW, QYTVec3* wiW,
                                  const QYTBSDFSample& bsdfSample,
                                  float* pdf, QYTBxDF_TYPE flags,
                                  QYTBxDF_TYPE* sampledType) const
    {
        int matchingComps = numComponents(flags);
        if (matchingComps == 0)
        {
            *pdf = 0.f;
            if(sampledType)
                *sampledType = QYTBxDF_TYPE(0);
            return QYTSpectrum(0.f);
        }
        
        int which = std::min(QYTFloor2Int(bsdfSample.uComponent*matchingComps),
                             matchingComps-1);
        QYTBxDF* bxdf = nullptr;
        int count = which;
        for (int i = 0; i < nBxDFs; ++i)
        {
            if(bxdfs[i]->matchesFlags(flags) && count-- == 0)
            {
                bxdf = bxdfs[i];
                break;
            }
        }
        Assert(bxdf);
        
        QYTVec3 wo = worldToLocal(woW);
        QYTVec3 wi;
        *pdf =0.f;
        
        QYTSpectrum f = bxdf->sample_f(wo, &wi, bsdfSample.uDir[0],
                                       bsdfSample.uDir[1], pdf);
        if(*pdf == 0.f)
        {
            if(sampledType)
                *sampledType = QYTBxDF_TYPE(0);
            return 0.f;
        }
        if (sampledType)
            *sampledType = bxdf->type;
        *wiW = localToWorld(wi);
        
        if(!(bxdf->type & BSDF_SPECULAR) && matchingComps > 1)
            for (int i = 0; i < nBxDFs; ++i)
                if (bxdfs[i] != bxdf && bxdfs[i]->matchesFlags(flags))
                    *pdf += bxdfs[i]->pdf(wo, wi);
        
        if (matchingComps > 1)
            *pdf /= matchingComps;
        
        if (!(bxdf->type & BSDF_SPECULAR)) {
            f = 0.;
            if (QYTVec3::Dot(*wiW, ng) * QYTVec3::Dot(woW, ng) > 0) // ignore BTDFs
                flags = QYTBxDF_TYPE(flags & ~BSDF_TRANSMISSION);
            else // ignore BRDFs
                flags = QYTBxDF_TYPE(flags & ~BSDF_REFLECTION);
            for (int i = 0; i < nBxDFs; ++i)
                if (bxdfs[i]->matchesFlags(flags))
                    f += bxdfs[i]->f(wo, wi);
        }
        return f;
    }
    
    float QYTBSDF::pdf(const QYTVec3& woW, const QYTVec3& wiW, QYTBxDF_TYPE flags) const
    {
        if (nBxDFs == 0.) return 0.;

        QYTVec3 wo = worldToLocal(woW), wi = worldToLocal(wiW);
        float pdf = 0.f;
        int matchingComps = 0;
        for (int i = 0; i < nBxDFs; ++i)
            if (bxdfs[i]->matchesFlags(flags)) {
                ++matchingComps;
                pdf += bxdfs[i]->pdf(wo, wi);
            }
        float v = matchingComps > 0 ? pdf / matchingComps : 0.f;
        return v;
    }
    
    QYTBSDF::QYTBSDF(const QYTDifferentialGeometry& dg, const QYTNormal3& ngeom, float e):
    dgShading(dg), eta(e)
    {
        ng = ngeom;
        nn = dgShading.n;
        sn = QYTVec3::Normalize(dgShading.dpdu);
        tn = QYTVec3::Cross(nn, sn);
        nBxDFs = 0;
    }
    
    QYTSpectrum QYTBSDF::f(const QYTVec3& woW, const QYTVec3& wiW,
                           QYTBxDF_TYPE flags) const
    {
        QYTVec3 wi = worldToLocal(wiW), wo = worldToLocal(woW);
        if (QYTVec3::Dot(wiW, ng) * QYTVec3::Dot(woW, ng) > 0) // ignore BTDFs
            flags = QYTBxDF_TYPE(flags & ~BSDF_TRANSMISSION);
        else // ignore BRDFs
            flags = QYTBxDF_TYPE(flags & ~BSDF_REFLECTION);
        QYTSpectrum f = 0.;
        for (int i = 0; i < nBxDFs; ++i)
            if (bxdfs[i]->matchesFlags(flags))
                f += bxdfs[i]->f(wo, wi);
        return f;
    }
    
    QYTSpectrum QYTBSDF::rho(QYTRNG &rng, QYTBxDF_TYPE flags, int sqrtSamples) const
    {
        int nSamples = sqrtSamples * sqrtSamples;
        float *s1 = ALLOCA(float, 2 * nSamples);
        qyt_stratified_sample2D(s1, sqrtSamples, sqrtSamples, rng);
        float *s2 = ALLOCA(float, 2 * nSamples);
        qyt_stratified_sample2D(s2, sqrtSamples, sqrtSamples, rng);
        
        QYTSpectrum ret(0.);
        for (int i = 0; i < nBxDFs; ++i)
            if (bxdfs[i]->matchesFlags(flags))
                ret += bxdfs[i]->rho(nSamples, s1, s2);
        return ret;
    }
    
    QYTSpectrum QYTBSDF::rho(const QYTVec3 &wo, QYTRNG &rng, QYTBxDF_TYPE flags,
                       int sqrtSamples) const
    {
        int nSamples = sqrtSamples * sqrtSamples;
        float *s1 = ALLOCA(float, 2 * nSamples);
        qyt_stratified_sample2D(s1, sqrtSamples, sqrtSamples, rng);
        QYTSpectrum ret(0.);
        for (int i = 0; i < nBxDFs; ++i)
            if (bxdfs[i]->matchesFlags(flags))
                ret += bxdfs[i]->rho(wo, nSamples, s1);
        return ret;
    }
}
