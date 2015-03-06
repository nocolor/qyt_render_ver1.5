//
//  QYTImageFilm.h
//  qyt_render_ver1.5
//
//  Created by nocolor on 15/3/2.
//  Copyright (c) 2015年 ___JinJiangTiaoZhanBei___. All rights reserved.
//

#ifndef __qyt_render_ver1_5__QYTImageFilm__
#define __qyt_render_ver1_5__QYTImageFilm__

#include "qytr.h"
#include "QYTFilter.h"
#include "QYTFilm.h"
#include "QYTSampler.h"

namespace QYT
{
    /**
     @class QYTImageFilm
     这个类用给定的重构滤波器对图像采样值进行滤波，并把结果图像写入磁盘。
     */
    class QYTImageFilm:public QYTFilm
    {
    private:
        ///滤波器
        QYTFilter* filter;
        
        /**
         裁剪窗口，定义在NDC空间中。前两个数代表左上角，后两个代表右下角。
         @note
         正规化设备空间(Normalized device coordinate, NDC)：
         这是图像渲染所用的坐标系。在x,y平面上，其空间范围是从(0,0)
         到(1,1)，并且(0,0)对应于图像的左上角。深度值z跟屏幕空间中的
         值相同。从屏幕空间到NDC空间是一个简单的线性变换。
         */
        float cropWindow[4];
        
        ///文件名
        std::string filename;
        
        /**
         xPixelStart和yPixelStart存放裁剪窗口的左上角的坐标位置，
         xPixelCount和yPixelCount分别给出了在各个方向的像素总数。
         */
        int xPixelStart, yPixelStart, xPixelCount, yPixelCount;
        
        ///代表像素的结构，图像中每个像素对应一个这样的结构
        struct Pixel
        {
            Pixel()
            {
                for (int i = 0; i < 3; ++i)
                    Lxyz[i] = splatXYZ[i] = 0.f;
                weightSum = 0.f;
            }
            ///像素辐射亮度的加权和
            QYTReal Lxyz[3];
            
            ///像素采样点贡献的权值
            QYTReal weightSum;
            
            ///像素辐射亮度的和（未加权）
            QYTReal splatXYZ[3];
        };
        
        /**
         根据像素滤波器的缺省设置，每个图像采样大约对16个像素有贡献值。特别
         是对于简单的场景，虽然花在光线求交测试和着色计算上的时间很少，但花
         在为每个采样更新图像的时间却很长。因此，QYTImageFilm预先计算好一
         个滤波器值的表，这样QYTFilm::addSample()函数就可以避免对QYTFilter::evalulate()
         的虚函数调用以及滤波器求值的开销，而是直接使用表里的值进行滤波。这里
         没有使用精确的采样位置来滤波，但是所带来的误差在实际应用中几乎可以忽
         略不计。
         
         这里的实现合理地假设滤波器可以满足f(x , y) = f(|x|, |y|)，这样表
         格只需存放滤波器偏置值的正值部分（即第一象限）。这个假设对pbrt中所介
         绍的所有滤波器都是成立的，这样只需要四分之一的表格大小，从而提高了内
         存存取的一致性和缓存效率。
         */
        QYTReal* filterTable;
        
        ///储存像素结构的数组
        Pixel* pixels;
        
    public:
        /**
         构造函数接受很多额外的参数。

         
         @param xres    x方向的分辨率
         @param yres    y方向的分辨率
         @param filt    滤波器
         @param crop    一个裁剪窗口（即一个位于[0,1]x[0,1]区域内的矩形）
         @param filename    输出图像文件名
         */
//        QYTImageFilm(int xres, int yres, QYTFilter *filt, const float crop[4],
//                     const std::string &filename);
//        
//
//        void addSample(const QYTCameraSample &sample,
//                               const QYTSpectrum &L) = 0;
//
//        void splat(const QYTCameraSample &sample, const QYTSpectrum &L) override;
//        
//        void getSampleExtent(int *xstart, int *xend,
//                            int *ystart, int *yend) const override;
//
//        void getPixelExtent(int *xstart, int *xend,
//                            int *ystart, int *yend) const override;
//        
//        void writeImage(float splatScale = 1.f) override;
//        
//        void updateDisplay(int x0, int y0, int x1, int y1, float splatScale) override;
    };
}

#endif /* defined(__qyt_render_ver1_5__QYTImageFilm__) */
