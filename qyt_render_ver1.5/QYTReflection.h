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
        
        virtual ~QYTBxDF();
        
    };
    
    class QYT_BTDFAdapter:public QYTBxDF
    {
    private:
        QYTBxDF* brdf;
        
    public:
        QYT_BTDFAdapter(QYTBxDF* b):
        QYTBxDF(QYTBxDF_TYPE(b->type ^ (BSDF_REFLECTION | BSDF_TRANSMISSION)))
        {
            brdf = b;
        }
        
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
}

#endif /* defined(__qyt_render_ver1_5__QYTReflection__) */
