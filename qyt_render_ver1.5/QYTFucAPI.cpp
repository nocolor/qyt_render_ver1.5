//
//  QYTFucAPI.cpp
//  qyt_render_ver1.5
//
//  Created by nocolor on 15/3/17.
//  Copyright (c) 2015年 ___JinJiangTiaoZhanBei___. All rights reserved.
//

#include "QYTFucAPI.h"

namespace QYT
{
    void qyt_concentric_sample_disk(float u1, float u2, float *dx, float *dy)
    {
        QYTConcentricSampleDisk(u1, u2, dx, dy);
    }
    
    QYTVec3 qyt_cosine_sample_hemisphere(float u1, float u2)
    {
        QYTVec3 ret;
        qyt_concentric_sample_disk(u1, u2, &ret.x, &ret.y);
        ret.z = sqrtf(std::max(0.f, 1.f - ret.x*ret.x - ret.y*ret.y));
        return ret;
    }
    
    QYTVec3 qyt_uniform_sample_hemisphere(float u1, float u2)
    {
        float z = u1;
        float r = sqrtf(std::max(0.f, 1.f - z*z));
        float phi = 2 * M_PI * u2;
        float x = r * cosf(phi);
        float y = r * sinf(phi);
        return QYTVec3(x, y, z);
    }
}