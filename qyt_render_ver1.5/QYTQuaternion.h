//
//  QYTQuaternion.h
//  qyt_render_ver1.5
//
//  Created by nocolor on 15/3/2.
//  Copyright (c) 2015年 ___JinJiangTiaoZhanBei___. All rights reserved.
//

#ifndef __qyt_render_ver1_5__QYTQuaternion__
#define __qyt_render_ver1_5__QYTQuaternion__

#include "qytr.h"
#include "QYTVector3.h"
#include "QYTTransform.h"

namespace QYT
{
    /**
     @class QYTQuaternion
     四元数是最简单的超复数。 复数是由实数加上元素 i 组成，其中i^2 = -1。 
     相似地，四元数都是由实数加上三个元素 i、j、k 组成，而且它们有如下的关
     系： i^2 = j^2 = k^2 = -1 , 每个四元数都是 1、i、j 和 k 的线性组
     合，即是四元数一般可表示为a + bi + cj + dk，其中a、b、c 、d是实数。
     
     
     我们要关心的是三维空间上任意的伸缩旋转变换是否可用四元数的乘积来表示，而
     这一点对四元数来说是完全能够胜任的。如果已知一个三维空间的伸缩旋转的转轴
     方向、旋转角度和伸缩比例，来求相应的四元数，是比较容易的。
     
     特别地，单位化的四元数用来描述旋转：
     以原点为旋转中心，旋转的轴是(α, β, γ)（ α^2 + β^2 + γ^2 = 1），
     （右手系的坐标定义的话，望向向量(α, β, γ)的前进方向反时针） 转θ角的旋转，
     用四元数表示就是，Q = (cos(θ/2); α sin(θ/2), β sin(θ/2), γ sin(θ/2))
     四元数的乘法的意义类似于Matrix的乘法－可以将两个旋转合并，例如：
     Q=Q1*Q2 表示Q的是先做Q2的旋转，再做Q1的旋转的结果，而多个四元数的旋转也是可以
     合并的，当有多次旋转操作时，使用四元数可以获得更高的计算效率。
     
     另一方面在实现动态模糊时，对四元数表示的旋转进行插值要比传统旋转矩阵更加
     方便，更高效，而且更精确。
     */
    class QYTQuaternion
    {
    public:
        
        ///相当于a，b，c
        QYTVec3 v;
        
        ///相当于d
        QYTReal w;
        
        QYTQuaternion():v(0.f), w(1.f){}
        
        ///将当期四元数代表的变化转变成变化矩阵对象
        QYTTransform toTransform() const;
        
        ///将变换矩阵对象转成四元数
        QYTQuaternion(const QYTTransform &t);
        
        QYTQuaternion& operator+=(const QYTQuaternion& q)
        {
            v += q.v;
            w += q.w;
            return *this;
        }
        
        friend QYTQuaternion operator+(const QYTQuaternion& q1, const QYTQuaternion& q2)
        {
            QYTQuaternion ret = q1;
            return ret += q2;
        }
        
        QYTQuaternion &operator-=(const QYTQuaternion &q)
        {
            v -= q.v;
            w -= q.w;
            return *this;
        }
        
        friend QYTQuaternion operator-(const QYTQuaternion &q1, const QYTQuaternion &q2)
        {
            QYTQuaternion ret = q1;
            return ret -= q2;
        }
        
        QYTQuaternion &operator*=(QYTReal f)
        {
            v *= f;
            w *= f;
            return *this;
        }
        
        QYTQuaternion operator*(QYTReal f) const
        {
            QYTQuaternion ret = *this;
            ret.v *= f;
            ret.w *= f;
            return ret;
        }
        
        QYTQuaternion &operator/=(QYTReal f)
        {
            v /= f;
            w /= f;
            return *this;
        }
        
        QYTQuaternion operator/(QYTReal f) const
        {
            QYTQuaternion ret = *this;
            ret.v /= f;
            ret.w /= f;
            return ret;
        }
        
        inline friend QYTQuaternion operator*(QYTReal f, const QYTQuaternion &q) {
            return q * f;
        }
        
        
        inline static QYTReal Dot(const QYTQuaternion &q1, const QYTQuaternion &q2) {
            return QYTVector3::Dot(q1.v, q2.v) + q1.w * q2.w;
        }
        
        
        inline static QYTQuaternion Normalize(const QYTQuaternion &q) {
            return q / sqrtf(Dot(q, q));
        }
        
        /**
         对q1和q2进行插值运算。
         */
        static QYTQuaternion Slerp(QYTReal t,
                                   const QYTQuaternion& q1,
                                   const QYTQuaternion& q2);
    };
    

    
    
}

#endif /* defined(__qyt_render_ver1_5__QYTQuaternion__) */
