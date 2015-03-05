//
//  QYTAnimatedTransform.h
//  qyt_render_ver1.5
//
//  Created by nocolor on 15/3/2.
//  Copyright (c) 2015年 ___JinJiangTiaoZhanBei___. All rights reserved.
//

#ifndef __qyt_render_ver1_5__QYTAnimatedTransform__
#define __qyt_render_ver1_5__QYTAnimatedTransform__

#include "QYTQuaternion.h"

namespace QYT
{
    /**
     @class QYTAnimatedTransform
     关键帧矩阵变换的，提供一系列关键帧变换(keyframe transformation)，
     每一个变换与某个时间点相关联。这样就运行移动相机，或者让场景中的物体在相机的快
     门开启和关闭这段时间中移动。
     每一个QYTAnimatedTransform对象代表从某个开始时间的变换状态到结束时间的变换
     状态之间的某一个状态，这个状态取决于传入的时间参数，当传入一个时间参数之后，就
     可以通过QYTAnimatedTransform获取由插值计算得到的那个状态的确切变换状态。
     */
    class QYTAnimatedTransform
    {
    private:
        ///定义了开始动画的时间和结束时间
        const float startTime, endTime;
        
        ///定义了开始的变换和结束时的变换
        const std::shared_ptr<QYTTransform> startTransform, endTransform;
        
        ///如果开始变换与结束变换实际上是一样的，那么便意味着其实没有产生变换
        const bool actuallyAnimated;
        
        ///将变换分解后的平移分量
        QYTVec3 T[2];
        
        ///将变换分解后的旋转分量
        QYTQuaternion R[2];
        
        ///将变换分解后的缩放分量
        QYTMat4 S[2];
        
    public:
        QYTAnimatedTransform(const std::shared_ptr<QYTTransform> startT,
                             float sTime,
                             const std::shared_ptr<QYTTransform> endT,
                             float eTime):startTime(sTime),endTime(eTime),
        startTransform(startT), endTransform(endT), actuallyAnimated(*startTransform != *endTransform)
        {
            Decompose(startTransform->getTransfromMatrix(), &T[0], &R[0], &S[0]);
            Decompose(endTransform->getTransfromMatrix(), &T[1], &R[1], &S[1]);
        }
        
        /**
         分解变换，把一个任意的变换矩阵分解为TRS，即平移、旋转、缩放的组合。
         @param m   需要被分解的变换矩阵
         @param T   用来储存分解后的平移分量的向量
         @param R   用来储存分解后的旋转分量的四元数
         @param S   用来储存分解后的缩放分量的矩阵
         */
        static void Decompose(const QYTMat4 &m, QYTVec3 *T, QYTQuaternion *R, QYTMat4 *S);
        
        /**
         对t所代表的变换在time时间内进行插值。
         */
        void interpolate(float time, QYTTransform *t) const;
        
        void operator()(const QYTRay &r, QYTRay *tr) const;
        void operator()(const QYTRayDifferential &r, QYTRayDifferential *tr) const;
        QYTPoint3 operator()(float time, const QYTPoint3 &p) const;
        QYTVec3 operator()(float time, const QYTVec3 &v) const;
        QYTRay operator()(const QYTRay &r) const;
        QYTBBox motionBounds(const QYTBBox &b, bool useInverse) const;
        
        bool hasScale() const
        { return startTransform->hasScale() || endTransform->hasScale(); }
    };
}

#endif /* defined(__qyt_render_ver1_5__QYTAnimatedTransform__) */
