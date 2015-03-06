//
//  QYTImageFilm.cpp
//  qyt_render_ver1.5
//
//  Created by nocolor on 15/3/2.
//  Copyright (c) 2015年 ___JinJiangTiaoZhanBei___. All rights reserved.
//

#include "QYTImageFilm.h"

namespace QYT
{
//    QYTImageFilm::QYTImageFilm(int xres, int yres, QYTFilter *filt, const float crop[4],
//                               const std::string &fn):QYTFilm(xres, yres)
//    {
//        //TODO:如果有可能，尽量使用智能指针避免浅拷贝引起的内存问题（特别注意多线程情况）
//        filter = filt;
//        
//        memcpy(cropWindow, crop, 4*sizeof(QYTReal));
//        filename = fn;
//        
//        //计算裁剪窗口和图像范围
//        xPixelStart = QYTCeil2Int(xResolution*cropWindow[0]);
//        xPixelCount = std::max(1, QYTCeil2Int(xResolution * cropWindow[1]) - xPixelStart);
//        yPixelStart = QYTCeil2Int(yResolution * cropWindow[2]);
//        yPixelCount = std::max(1, QYTCeil2Int(yResolution * cropWindow[3]) - yPixelStart);
//        
//        pixels = new Pixel[xPixelCount*yPixelCount];
//        
//#define FILTER_TABLE_SIZE 16
//        filterTable = new float[FILTER_TABLE_SIZE * FILTER_TABLE_SIZE];
//        float *ftp = filterTable;
//        for (int y = 0; y < FILTER_TABLE_SIZE; ++y)
//        {
//            float fy = ((float)y + .5f) * filter->yWidth / FILTER_TABLE_SIZE;
//            for (int x = 0; x < FILTER_TABLE_SIZE; ++x)
//            {
//                float fx = ((float)x + .5f) * filter->xWidth / FILTER_TABLE_SIZE;
//                *ftp++ = filter->evaluate(fx, fy);
//            }
//        }
//    }
    
//    void QYTImageFilm::addSample(const QYT::QYTCameraSample &sample, const QYTSpectrum &L)
//    {
//        /*
//         为了找出那些被采样所影响的像素，addSample()将采样的连续坐标转换为离散坐标，
//         即把x,y坐标分别减去0.5。然后，将两个坐标值分别在x,y方向上偏移，偏移量为滤波
//         器的宽度，然后对最小的坐标值取Ceiling值，对最大的坐标值取floor值，因为超出
//         滤波器范围的像素肯定不会受当前采样的影响。
//         */
//        
//        //计算采样点的光栅范围
//        float dimageX = sample.imageX - 0.5f;
//        float dimageY = sample.imageY - 0.5f;
//        int x0 = QYTCeil2Int (dimageX - filter->xWidth);
//        int x1 = QYTFloor2Int(dimageX + filter->xWidth);
//        int y0 = QYTCeil2Int (dimageY - filter->yWidth);
//        int y1 = QYTFloor2Int(dimageY + filter->yWidth);
//        x0 = std::max(x0, xPixelStart);
//        x1 = std::min(x1, xPixelStart + xPixelCount - 1);
//        y0 = std::max(y0, yPixelStart);
//        y1 = std::min(y1, yPixelStart + yPixelCount - 1);
//        if ((x1-x0) < 0 || (y1-y0) < 0)
//            return;
//        
//        float xyz[3];
//        L.toXYZ(xyz);
//        
//        int* ifx = ALLOCA(int, x1-x0+1);
//        for (int x = x0; x <= x1; ++x)
//        {
//            float fx = fabsf((x - dimageX) *
//                             filter->invXWidth * FILTER_TABLE_SIZE);
//            ifx[x-x0] = std::min(QYTFloor2Int(fx), FILTER_TABLE_SIZE-1);
//        }
//        int *ify = ALLOCA(int, y1 - y0 + 1);
//        for (int y = y0; y <= y1; ++y) {
//            float fy = fabsf((y - dimageY) *
//                             filter->invYWidth * FILTER_TABLE_SIZE);
//            ify[y-y0] = std::min(QYTFloor2Int(fy), FILTER_TABLE_SIZE-1);
//        }
//        
//        bool syncNeeded = (filter->xWidth > 0.5f || filter->yWidth > 0.5f);
//        for (int y = y0; y <= y1; ++y)
//        {
//            for (int x = x0; x <= x1; ++x)
//            {
//                /// Evaluate filter value at $(x,y)$ pixel
//                int offset = ify[y-y0]*FILTER_TABLE_SIZE + ifx[x-x0];
//                float filterWt = filterTable[offset];
//                
//                // Update pixel values with filtered sample contribution
//                Pixel &pixel = (*pixels)(x - xPixelStart, y - yPixelStart);
//                if (!syncNeeded) {
//                    pixel.Lxyz[0] += filterWt * xyz[0];
//                    pixel.Lxyz[1] += filterWt * xyz[1];
//                    pixel.Lxyz[2] += filterWt * xyz[2];
//                    pixel.weightSum += filterWt;
//                }
//                else {
//                    // Safely update _Lxyz_ and _weightSum_ even with concurrency
//                    AtomicAdd(&pixel.Lxyz[0], filterWt * xyz[0]);
//                    AtomicAdd(&pixel.Lxyz[1], filterWt * xyz[1]);
//                    AtomicAdd(&pixel.Lxyz[2], filterWt * xyz[2]);
//                    AtomicAdd(&pixel.weightSum, filterWt);
//                }
//            }
//        }
//    }
}
