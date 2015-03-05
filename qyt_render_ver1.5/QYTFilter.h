//
//  QYTFilter.h
//  qyt_render_ver1.5
//
//  Created by nocolor on 15/3/2.
//  Copyright (c) 2015年 ___JinJiangTiaoZhanBei___. All rights reserved.
//

#ifndef __qyt_render_ver1_5__QYTFilter__
#define __qyt_render_ver1_5__QYTFilter__

#include "qytr.h"

namespace QYT
{
    /**
     @class QYTFilter
     滤波器的实现都是从抽象类QYTFilter来导出的，该抽象类提供了使用滤波函数f(x,y)的接口。
     滤波器是这样一种对象，它通过某些滤波算法，来确定像素中心周围的采样点得到的辐射亮度对
     这个像素的贡献（系数）。使用的理论公式基于“前置滤波”，这种理论用来在开始计算采样点的
     贡献之前就将那些超过极限采样频率（Nyquist极限频率）过滤掉，已得到更好地重构效果。
     */
    class QYTFilter
    {
    public:
        virtual ~QYTFilter(){}
        QYTFilter(float xw, float yw)
        : xWidth(xw), yWidth(yw), invXWidth(1.f/xw), invYWidth(1.f/yw) {}
        
        /**QYTFilter要实现的唯一函数是evaluate()。它的变量x，y给出了采样点相对于滤波器中心的采样点位置。
         @return 这个采样点经滤波后，其贡献值得权重
         */
        virtual QYTReal evaluate(QYTReal x, QYTReal y) const = 0;
        
        ///采样区域是在（x,y）点为中心，2*xWidth, 2*yWidth围起来的矩形区域。
        const float xWidth, yWidth;
        const float invXWidth, invYWidth;
    };
}

#endif /* defined(__qyt_render_ver1_5__QYTFilter__) */
