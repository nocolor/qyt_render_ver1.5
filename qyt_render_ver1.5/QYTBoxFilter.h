//
//  QYTBoxFilter.h
//  qyt_render_ver1.5
//
//  Created by nocolor on 15/3/2.
//  Copyright (c) 2015年 ___JinJiangTiaoZhanBei___. All rights reserved.
//

#ifndef __qyt_render_ver1_5__QYTBoxFilter__
#define __qyt_render_ver1_5__QYTBoxFilter__

#include "QYTFilter.h"

namespace QYT
{
    /**
     @class QYTBoxFilter
     图形学中最常见的滤波器是盒滤波器，它对图像中的正方形区域中的所有采样
     都赋给相同的权值。虽然计算效率很高，但效果最糟糕。
     */
    class QYTBoxFilter:public QYTFilter
    {
    public:
        QYTBoxFilter(float xw, float yw) : QYTFilter(xw, yw) { }
        
        ///由于权值相同，其实这个函数返回常量（1.f）。
        QYTReal evaluate(float x, float y) const override;
    };
}

#endif /* defined(__qyt_render_ver1_5__QYTBoxFilter__) */
