//
//  QYTFilm.h
//  qyt_render_ver1.5
//
//  Created by nocolor on 15/3/2.
//  Copyright (c) 2015年 ___JinJiangTiaoZhanBei___. All rights reserved.
//

#ifndef __qyt_render_ver1_5__QYTFilm__
#define __qyt_render_ver1_5__QYTFilm__

#include "qytr.h"
#include "QYTSampler.h"

namespace QYT
{
    /**
     @class QYTFilm
     相机中的胶片类型对入射光转换为图像颜色的方式有很大的影响。我们用QYTFilm类描述对模拟相机中的感光设备。
     在求得每条相机光线的辐射亮度以后，QYTFilm类的实现确定采样对附近像素的贡献值，并更新图像表示。当程序退
     出主渲染循环时，QYTFilm通常还要把最终的图像写入磁盘文件。
     它使用像素重构公式来计算最终的像素值，并把图像的浮点颜色值写入磁盘。对于一个基于物理的渲染器而言，用浮
     点格式来创建图像要比其它用8位无符号整数的图像格式要灵活得多，因为浮点数格式可以避免在图像量化过程中丢失
     重要信息。
     为了在现代显示设备上显示这样的图像，必须把这些浮点像素值映射到显示器上的离散值。
     */
    class QYTFilm {
    public:
        // Film Interface
        QYTFilm(int xres, int yres)
        : xResolution(xres), yResolution(yres) { }
        virtual ~QYTFilm(){}
        
        /**
         以一个采样及其对应的相机光线，辐射亮度值和alpha值做为参数来更新图像。
         */
        virtual void addSample(const QYTCameraSample &sample,
                               const QYTSpectrum &L) = 0;
        
        /**
         更新像素的颜色，但是最终的颜色值并非采样点颜色值的加权平均值，而只是
         单纯的把颜色相加。显然，某个像素如果对用的采样点越多，那么那个像素就
         越明亮。这个方法会被某些光线传输算法使用。
         */
        virtual void splat(const QYTCameraSample &sample, const QYTSpectrum &L) = 0;
        
        /**
         QYTFilm一项责任是负责决定采样器进行采样所需要的整数像素值的范围。
         虽然对于简单的QYTFilm实现而言，像素范围是从(0, 0)到(xResolution - 1, yResolution - 1)，
         但是由于像素重构滤波器的范围的有限，通常需要在图像边界稍微靠外的地方采样。
         */
        virtual void getSampleExtent(int *xstart, int *xend,
                                     int *ystart, int *yend) const = 0;
        

        ///给出实际图像的像素范围。
        virtual void getPixelExtent(int *xstart, int *xend,
                                    int *ystart, int *yend) const = 0;
        
        /**
         Some QYTFilm implementations find it useful to be notified that a region 
         of the pixels has recently been updated. In particular, an implementation 
         that opened a window to show the image as it was being rendered could use 
         this information to trigger an update of a subregion of the window after 
         its samples had been generated. Here we provide a default empty implementation 
         of this method so that Film implementations that don’t need this information
         don’t need to implement it themselves.
         
         ---PBRT，secend Edition
         */
        virtual void updateDisplay(int x0, int y0, int x1, int y1, float splatScale = 1.f){}
        
        ///该函数可以允许Film对图像任意的处理工作，然后再显示或存盘。
        virtual void writeImage(float splatScale = 1.f) = 0;
        
        ///整个图像在x,y方向上的分辨率
        const int xResolution, yResolution;
    };
}

#endif /* defined(__qyt_render_ver1_5__QYTFilm__) */
