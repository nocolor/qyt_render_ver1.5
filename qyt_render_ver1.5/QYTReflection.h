//
//  QYTReflection.h
//  qyt_render_ver1.5
//
//  Created by nocolor on 15/3/14.
//  Copyright (c) 2015年 ___JinJiangTiaoZhanBei___. All rights reserved.
//

#ifndef __qyt_render_ver1_5__QYTReflection__
#define __qyt_render_ver1_5__QYTReflection__

#include "qytr.h"
#include "QYTVector3.h"
#include "QYTSpectrum.h"
#include "QYTRNG.h"
#include "QYTSampler.h"
#include "QYTDifferentialGeometry.h"

namespace QYT
{
    /**
     该函数返回在切空间中，方向向量w与法线的夹角的余弦值。
     由于在切空间中，法线向量是（0，0，1），则n dot w
     显然就是返回w.z。
     @attention 该函数只适用于切空间。
     */
    inline QYTReal qyt_cos_theta(const QYTVec3& w)
    {
        return w.z;
    }
    
    /**
     该函数返回在切空间中，方向向量w与法线的夹角的余弦的绝对值。
     由于在切空间中，法线向量是（0，0，1），则n dot w
     显然就是返回w.z。
     @attention 该函数只适用于切空间。
     @see QYTReal qyt_cos_theta(const QYTVec3& w)
     */
    inline QYTReal qyt_abs_cos_theta(const QYTVec3& w)
    {
        return fabsf(qyt_cos_theta(w));
    }
    
    /**
     该函数返回在切空间中，方向向量w与法线的夹角的正弦的平方。
     @attention 该函数只适用于切空间。
     */
    inline QYTReal qyt_sin_theta_square(const QYTVec3& w)
    {
        return std::max(0.f, 1.f - qyt_cos_theta(w)*qyt_cos_theta(w) );
    }
    
    /**
     该函数返回在切空间中，方向向量w与法线的夹角的正弦。
     @attention 该函数只适用于切空间。
     */
    inline QYTReal qyt_sin_theta(const QYTVec3& w)
    {
        return sqrtf(qyt_sin_theta_square(w));
    }
    
    /**
     该函数返回在切空间中，方向向量w在xy平面的投影与x轴夹角的余弦值。
     @attention 该函数只适用于切空间。
     */
    inline QYTReal qyt_cos_phi(const QYTVec3& w)
    {
        QYTReal sinTheta = qyt_sin_theta(w);
        if(sinTheta == 0.f)
            return 1.f;
        return QYTClamp(w.x/sinTheta, -1.f, 1.f);
    }
    
    /**
     该函数返回在切空间中，方向向量w在xy平面的投影与x轴夹角的正弦值。
     @attention 该函数只适用于切空间。
     */
    inline QYTReal qyt_sin_phi(const QYTVec3& w)
    {
        QYTReal sinTheta = qyt_sin_theta(w);
        if(sinTheta == 0.f)
            return 1.f;
        return QYTClamp(w.y/sinTheta, -1.f, 1.f);
    }
    
    /**
     若向量wi和wo位于同一半球平面，返回true，否则返回false。
     @attention 该函数只适用于切空间。
     */
    inline bool QYTSameHemisphere(const QYTVec3& wi, const QYTVec3& wo)
    {
        return wi.z*wo.z > 0.f;
    }
    
    /**
     由于BRDF和BTDF都用通用的BxDF作为基类，因此需要定义一些基础类型来分辨它们
     到底代表那种反射模型。
     */
    enum QYTBxDF_TYPE
    {
        BSDF_REFLECTION   = 1<<0,
        BSDF_TRANSMISSION = 1<<1,
        BSDF_DIFFUSE      = 1<<2,
        BSDF_GLOSSY       = 1<<3,
        BSDF_SPECULAR     = 1<<4,
        BSDF_ALL_TYPES        = BSDF_DIFFUSE |
        BSDF_GLOSSY |
        BSDF_SPECULAR,
        BSDF_ALL_REFLECTION   = BSDF_REFLECTION |
        BSDF_ALL_TYPES,
        BSDF_ALL_TRANSMISSION = BSDF_TRANSMISSION |
        BSDF_ALL_TYPES,
        BSDF_ALL              = BSDF_ALL_REFLECTION |
        BSDF_ALL_TRANSMISSION
    };
    
    /**
     @class QYTBxDF
     QYTBxDF是反射和传输模型的基类，它是可以表示BRDF和BTDF。
     两种函数都用完全一样的接口，因此它们共同使用一个基类。它们
     靠一个QYTBxDF_TYPE量来区分。
     */
    class QYTBxDF
    {
    public:
        const QYTBxDF_TYPE type;
        
        QYTBxDF(QYTBxDF_TYPE t):type(t){}
        bool matchesFlags(QYTBxDF_TYPE flags) const
        {return (type & flags) == type;}
        
        /**
         该函数返回给定的一对方向所对应的分布函数值。这个接口隐性地假定不同波长上的光是
         不相干(decoupled)的，即一个波长上的能量不会以不同的波长被反射出去。
         @attention 并不是所有的BxDF都用该函数求值。例如，象镜子、玻璃或水这样的全镜
         面反射物体只在单一出射方向上对单一方向的入射光
         */
        virtual QYTSpectrum f(const QYTVec3& wo, const QYTVec3& wi) const = 0;
        
        /**
         这个函数处理由delta分布来描述的散射，也用它来对BxDF所散射出的多个光线的方向进
         行随机采样。给定了出射方向之后，BxDF::Sample_f()计算入射光的方向ωi，并根据这
         一对方向来计算BxDF值。
         @attention 对于delta分布而言，需要利用这种方式来生成适当的ωi方向，因为调用者
         无法生成相应的ωi方向。利用delta分布的BxDF并不需要参数u1，u2和pdf。
         */
        virtual QYTSpectrum sample_f(const QYTVec3& wo, QYTVec3* vi,
                                     QYTReal u1, QYTReal u2, QYTReal* pdf) const;
        
        /**
         反射率函数给出了对半球上的恒定照明下的在某个给定方向上的总反射量，也可以被等价
         地认为是来自某个给定方向上的光在半球上的总反射量。
         @attention 大多数的BxDF用Monte Carlo积分来计算其近似值。对于这些BxDF而言，
         参数nSamples和samples用来控制Monte Carlo算法的行为
         */
        virtual QYTSpectrum rho(const QYTVec3& wo, int nSamples,
                                const QYTReal* samples) const;
        
        /**
         表面的半球-半球反射率(hemispherical- hemispherical reflectance)是一个光
         谱值常量，记作ρhh，它给出了在所有方向有相同的入射光的情况下一个表面上被反射的入
         射光的比率。
         @attention 大多数的BxDF用Monte Carlo积分来计算其近似值。对于这些BxDF而言，
         参数nSamples和samples用来控制Monte Carlo算法的行为
         */
        virtual QYTSpectrum rho(int nSamples,
                                const QYTReal* samples1, const QYTReal* samples2) const;
        
        /**
         重要性采样需要的概率分布函数。
         */
        virtual QYTReal pdf(const QYTVec3& wi, const QYTVec3& wo) const;
        
        virtual ~QYTBxDF(){}
        
    };
    
    /**
     @class QYT_BRDFAdapter
     QYT_BTDFAdapter是一个利用BxDF代码实现光线传输模型的适配器。
     它使用BxDF的接口，并在此基础上稍作封装。
     */
    class QYT_BTDFAdapter:public QYTBxDF
    {
    private:
        ///由于包含一个BxDF的指针，因此通过此指针调用BxDF的方法
        QYTBxDF* brdf;
        
    public:
        QYT_BTDFAdapter(QYTBxDF* b):
        
        /**
         构造函数将反射模型的type转化成传输标志。
         */
        QYTBxDF(QYTBxDF_TYPE(b->type ^ (BSDF_REFLECTION | BSDF_TRANSMISSION)))
        {
            brdf = b;
        }
        
        ///将w转换到另一个半球空间
        static QYTVec3 otherHemisphere(const QYTVec3 &w)
        {
            return QYTVec3(w.x, w.y, -w.z);
        }
        
        QYTSpectrum f(const QYTVec3& wo, const QYTVec3& wi) const;
        QYTSpectrum sample_f(const QYTVec3& wo, QYTVec3* wi, QYTReal u1,
                             QYTReal u2, QYTReal* pdf) const;
        QYTSpectrum rho(const QYTVec3& w, int nSamples, const QYTReal *samples) const
        {
            return brdf->rho(otherHemisphere(w), nSamples, samples);
        }
        QYTSpectrum rho(const int nSamples, const QYTReal* samples1,
                        const QYTReal* samples2) const
        {
            return brdf->rho(nSamples, samples1, samples2);
        }
        QYTReal pdf(const QYTVec3& wo, const QYTVec3& wi) const;
        
    };
    
    /**
     可以用线性方法对BxDF进行缩放。
     */
    class QYTScaledBxDF:public QYTBxDF
    {
    private:
        QYTSpectrum s;
        QYTBxDF*    bxdf;
        
    public:
        QYTScaledBxDF(QYTBxDF* b, const QYTSpectrum& sc):
        QYTBxDF(QYTBxDF_TYPE(b->type)), bxdf(b), s(sc)
        {}
        
        QYTSpectrum rho(const QYTVec3& w, int nSamples, const QYTReal* samples) const
        {
            return s * bxdf->rho(w, nSamples, samples);
        }
        
        QYTSpectrum rho(int nSamples, const QYTReal* samples1, const QYTReal* samples2) const
        {
            return s * bxdf->rho(nSamples, samples1, samples2);
        }
        
        QYTSpectrum f(const QYTVec3& wo, const QYTVec3& wi) const;
        QYTSpectrum sample_f(const QYTVec3& wo, QYTVec3* wi, QYTReal u1, QYTReal u2, QYTReal* pdf) const;
    };
    
    /**
     @class QYTFresnel
     表示菲涅尔折射模型的类。
     */
    class QYTFresnel
    {
        virtual ~QYTFresnel();
        virtual QYTSpectrum evaluate(QYTReal cosi) const = 0;
    };
    
    struct QYTBSDFSampleOffset
    {
        QYTBSDFSampleOffset() { }
        QYTBSDFSampleOffset(int count, QYTSample *sample);
        int nSamples, componentOffset, dirOffset;
    };
    
    struct QYTBSDFSample
    {
        QYTBSDFSample(float up0, float up1, float ucomp)
        {
            Assert(up0 >= 0.f && up0 < 1.f);
            Assert(up1 >= 0.f && up1 < 1.f);
            Assert(ucomp >= 0.f && ucomp < 1.f);
            uDir[0] = up0;
            uDir[1] = up1;
            uComponent = ucomp;
        }
        QYTBSDFSample(QYTRNG &rng) {
            uDir[0] = rng.randomFloat(0, 1);
            uDir[1] = rng.randomFloat(0, 1);
            uComponent = rng.randomFloat(0, 1);
        }
        QYTBSDFSample(const QYTSample *sample, const QYTBSDFSampleOffset &offsets, uint32_t num);
        QYTBSDFSample() { }
        float uDir[2], uComponent;
    };
    
    /**
     @class QYTBSDF
     QYTBSDF类代表了一组BRDF和BTDF。将它们以这种方式组织在一起，目的是允许
     系统的其它部分能够直接跟复合型的BSDF打交道，而不是必须考虑其中每一个组成部分。
     */
    class QYTBSDF
    {
    private:
        QYTNormal3 nn, ng;
        QYTVec3 sn, tn;
        int nBxDFs;
#define MAX_BxDFS 8
        QYTBxDF* bxdfs[MAX_BxDFS];
        
    public:
        QYTSpectrum sample_f(const QYTVec3& wo, QYTVec3* wi, const QYTBSDFSample& bsdfSample,
                             float* pdf, QYTBxDF_TYPE flags = BSDF_ALL,
                             QYTBxDF_TYPE* sampledType = nullptr) const;
        
        float pdf(const QYTVec3& wo, const QYTVec3& wi, QYTBxDF_TYPE flags = BSDF_ALL) const;
        
        QYTBSDF(const QYTDifferentialGeometry& dgs, const QYTNormal3& ngeom, float eta = 1.f);
        void add(QYTBxDF* bxdf)
        {
            Assert(nBxDFs < MAX_BxDFS);
            bxdfs[nBxDFs++] = bxdf;
        }
        
        int numComponents() const {return nBxDFs;}
        
        int numComponents(QYTBxDF_TYPE flags) const
        {
            int num = 0;
            for (int i = 0; i < nBxDFs; ++i)
                if (bxdfs[i]->matchesFlags(flags)) ++num;
            return num;
        }
        
        QYTVec3 worldToLocal(const QYTVec3& v) const
        {
            return QYTVec3(QYTVec3::Dot(v, sn), QYTVec3::Dot(v, tn), QYTVec3::Dot(v, nn));
        }
        
        QYTVec3 localToWorld(const QYTVec3 &v) const
        {
            return QYTVec3(sn.x * v.x + tn.x * v.y + nn.x * v.z,
                           sn.y * v.x + tn.y * v.y + nn.y * v.z,
                           sn.z * v.x + tn.z * v.y + nn.z * v.z);
        }
        
        QYTSpectrum f(const QYTVec3& woW, const QYTVec3& wiW,
                      QYTBxDF_TYPE flag = BSDF_ALL) const;
        
        QYTSpectrum rho(QYTRNG& rng, QYTBxDF_TYPE flags = BSDF_ALL,
                        int sqrtSamples = 6) const;
        QYTSpectrum rho(const QYTVec3& wo, QYTRNG& rng, QYTBxDF_TYPE flags = BSDF_ALL,
                        int sqrtSamples = 6) const;
        
        const QYTDifferentialGeometry dgShading;
        const float eta;
    };
    
    class QYTBSSRDF
    {
    public:
        // BSSRDF Public Methods
        QYTBSSRDF(const QYTSpectrum &sa, const QYTSpectrum &sps, float et)
        : e(et), sig_a(sa), sigp_s(sps) { }
        float eta() const { return e; }
        QYTSpectrum sigma_a() const { return sig_a; }
        QYTSpectrum sigma_prime_s() const { return sigp_s; }
    private:
        // BSSRDF Private Data
        float e;
        QYTSpectrum sig_a, sigp_s;
    };
}

#endif /* defined(__qyt_render_ver1_5__QYTReflection__) */
