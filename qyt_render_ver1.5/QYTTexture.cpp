//
//  QYTTexture.cpp
//  qyt_render_ver1.5
//
//  Created by nocolor on 15/3/17.
//  Copyright (c) 2015年 ___JinJiangTiaoZhanBei___. All rights reserved.
//

#include "QYTTexture.h"

namespace QYT
{
    void QYTUVMapping2D::map(const QYT::QYTDifferentialGeometry &dg, float *s, float *t, float *dsdx, float *dtdx, float *dsdy, float *dtdy) const
    {
        *s = su * dg.u + du;
        *t = sv * dg.v + dv;

        *dsdx = su * dg.dudx;
        *dtdx = sv * dg.dvdx;
        *dsdy = su * dg.dudy;
        *dtdy = sv * dg.dvdy;
    }
}