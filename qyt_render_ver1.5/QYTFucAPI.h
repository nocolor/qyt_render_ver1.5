//
//  QYTFucAPI.h
//  qyt_render_ver1.5
//
//  Created by nocolor on 15/3/17.
//  Copyright (c) 2015年 ___JinJiangTiaoZhanBei___. All rights reserved.
//

#ifndef __qyt_render_ver1_5__QYTFucAPI__
#define __qyt_render_ver1_5__QYTFucAPI__

#include "qytr.h"
#include "QYTVector3.h"
#include "QYTPoint3.h"
#include "QYTSampler.h"

namespace QYT
{
    QYTVec3 qyt_normalize(const QYTVec3& v);
    QYTVec3 qyt_cross(const QYTVec3& v1, const QYTVec3& v2);
    QYTVec3 qyt_dot(const QYTVec3& v1, const QYTVec3& v2);
    
    void qyt_concentric_sample_disk(float u1, float u2, float *dx, float *dy);
    QYTVec3 qyt_cosine_sample_hemisphere(float u1, float u2);
    QYTVec3 qyt_uniform_sample_hemisphere(float u1, float u2);
    void qyt_stratified_sample2D(float *samp, int nx, int ny, QYTRNG &rng,
                                 bool jitter = true);
}

#endif /* defined(__qyt_render_ver1_5__QYTFucAPI__) */
