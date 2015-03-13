//
//  QYTImageFilm.cpp
//  qyt_render_ver1.5
//
//  Created by nocolor on 15/3/2.
//  Copyright (c) 2015年 ___JinJiangTiaoZhanBei___. All rights reserved.
//

#include "QYTImageFilm.h"
#include "targa.h"

namespace QYT
{
    void QYTWriteImageTGA(const std::string &name, float *pixels,
                       float *alpha, int xRes, int yRes,
                       int totalXRes, int totalYRes,
                       int xOffset, int yOffset)
    {
        // Reformat to BGR layout.
        uint8_t *outBuf = new uint8_t[3 * xRes * yRes];
        uint8_t *dst = outBuf;
        for (int y = 0; y < yRes; ++y) {
            for (int x = 0; x < xRes; ++x) {
#define TO_BYTE(v) (uint8_t(QYTClamp(255.f * powf((v), 1.f/2.2f), 0.f, 255.f)))
                dst[0] = TO_BYTE(pixels[3*(y*xRes+x)+2]);
                dst[1] = TO_BYTE(pixels[3*(y*xRes+x)+1]);
                dst[2] = TO_BYTE(pixels[3*(y*xRes+x)+0]);
                dst += 3;
            }
        }
        
        tga_result result;
        if ((result = tga_write_bgr(name.c_str(), outBuf, xRes, yRes, 24)) != TGA_NOERR)
            QYTError("Unable to write output file \"%s\" (%s)", name.c_str(),
                     tga_error(result));
        
        delete[] outBuf;
    }
    
    QYTImageFilm::QYTImageFilm(int xres, int yres, QYTFilter *filt, const float crop[4],
                               const std::string &fn):QYTFilm(xres, yres)
    {
        //TODO:如果有可能，尽量使用智能指针避免浅拷贝引起的内存问题（特别注意多线程情况）
        filter = filt;
        
        memcpy(cropWindow, crop, 4*sizeof(QYTReal));
        filename = fn;
        
        //计算裁剪窗口和图像范围
        xPixelStart = QYTCeil2Int(xResolution*cropWindow[0]);
        xPixelCount = std::max(1, QYTCeil2Int(xResolution * cropWindow[1]) - xPixelStart);
        yPixelStart = QYTCeil2Int(yResolution * cropWindow[2]);
        yPixelCount = std::max(1, QYTCeil2Int(yResolution * cropWindow[3]) - yPixelStart);
        
        pixels = new Pixel[xPixelCount*yPixelCount];
        
#define FILTER_TABLE_SIZE 16
        filterTable = new float[FILTER_TABLE_SIZE * FILTER_TABLE_SIZE];
        float *ftp = filterTable;
        for (int y = 0; y < FILTER_TABLE_SIZE; ++y)
        {
            float fy = ((float)y + .5f) * filter->yWidth / FILTER_TABLE_SIZE;
            for (int x = 0; x < FILTER_TABLE_SIZE; ++x)
            {
                float fx = ((float)x + .5f) * filter->xWidth / FILTER_TABLE_SIZE;
                *ftp++ = filter->evaluate(fx, fy);
            }
        }
    }
    
    void QYTImageFilm::addSample(const QYT::QYTCameraSample &sample, const QYTSpectrum &L)
    {
        /*
         为了找出那些被采样所影响的像素，addSample()将采样的连续坐标转换为离散坐标，
         即把x,y坐标分别减去0.5。然后，将两个坐标值分别在x,y方向上偏移，偏移量为滤波
         器的宽度，然后对最小的坐标值取Ceiling值，对最大的坐标值取floor值，因为超出
         滤波器范围的像素肯定不会受当前采样的影响。
         */
        
        //计算采样点的光栅范围
        float dimageX = sample.imageX - 0.5f;
        float dimageY = sample.imageY - 0.5f;
        int x0 = QYTCeil2Int (dimageX - filter->xWidth);
        int x1 = QYTFloor2Int(dimageX + filter->xWidth);
        int y0 = QYTCeil2Int (dimageY - filter->yWidth);
        int y1 = QYTFloor2Int(dimageY + filter->yWidth);
        x0 = std::max(x0, xPixelStart);
        x1 = std::min(x1, xPixelStart + xPixelCount - 1);
        y0 = std::max(y0, yPixelStart);
        y1 = std::min(y1, yPixelStart + yPixelCount - 1);
        if ((x1-x0) < 0 || (y1-y0) < 0)
            return;
        
        float xyz[3];
        L.toXYZ(xyz);
        
        int* ifx = ALLOCA(int, x1-x0+1);
        for (int x = x0; x <= x1; ++x)
        {
            float fx = fabsf((x - dimageX) *
                             filter->invXWidth * FILTER_TABLE_SIZE);
            ifx[x-x0] = std::min(QYTFloor2Int(fx), FILTER_TABLE_SIZE-1);
        }
        int *ify = ALLOCA(int, y1 - y0 + 1);
        for (int y = y0; y <= y1; ++y) {
            float fy = fabsf((y - dimageY) *
                             filter->invYWidth * FILTER_TABLE_SIZE);
            ify[y-y0] = std::min(QYTFloor2Int(fy), FILTER_TABLE_SIZE-1);
        }
        
        bool syncNeeded = (filter->xWidth > 0.5f || filter->yWidth > 0.5f);
        for (int y = y0; y <= y1; ++y)
        {
            for (int x = x0; x <= x1; ++x)
            {
                /// Evaluate filter value at $(x,y)$ pixel
                int offset = ify[y-y0]*FILTER_TABLE_SIZE + ifx[x-x0];
                float filterWt = filterTable[offset];
                
                // Update pixel values with filtered sample contribution
                
                int i = x - xPixelStart, j = y - yPixelStart;
                Pixel &pixel = pixels[i*xPixelCount + j];
                if (!syncNeeded) {
                    pixel.Lxyz[0] += filterWt * xyz[0];
                    pixel.Lxyz[1] += filterWt * xyz[1];
                    pixel.Lxyz[2] += filterWt * xyz[2];
                    pixel.weightSum += filterWt;
                }
                else
                {
                    static std::mutex mutex;
                    std::unique_lock<std::mutex> lock(mutex);
                    
                    pixel.Lxyz[0] += filterWt * xyz[0];
                    pixel.Lxyz[1] += filterWt * xyz[1];
                    pixel.Lxyz[2] += filterWt * xyz[2];
                    pixel.weightSum += filterWt;
                    
                    lock.unlock();
                }
            }
        }
    }
    
    void QYTImageFilm::splat(const QYT::QYTCameraSample &sample, const QYTSpectrum &L)
    {
        if(L.hasNaNs())
        {
            QYTWarning("QYTImageFilm ignoring splatted spectrum with NaN values");
            return;
        }
        float xyz[3];
        L.toXYZ(xyz);
        
        int x = QYTFloat2Int(sample.imageX), y = QYTFloat2Int(sample.imageY);
        if (x < xPixelStart || x - xPixelStart >= xPixelCount ||
            y < yPixelStart || y - yPixelStart >= yPixelCount) return;
        int i = x - xPixelStart, j = y - yPixelStart;
        Pixel &pixel = pixels[i*xPixelCount + j];
        
        static std::mutex mutex;
        std::unique_lock<std::mutex> lock(mutex);
        
        pixel.splatXYZ[0] += xyz[0];
        pixel.splatXYZ[1] += xyz[1];
        pixel.splatXYZ[2] += xyz[2];
        
        lock.unlock();
    }
    
    void QYTImageFilm::getSampleExtent(int *xstart, int *xend, int *ystart, int *yend) const
    {
        *xstart = QYTFloor2Int(xPixelStart + 0.5f - filter->xWidth);
        *xend   = QYTCeil2Int(xPixelStart + 0.5f + xPixelCount +
                              filter->xWidth);
        
        *ystart = QYTFloor2Int(yPixelStart + 0.5f - filter->yWidth);
        *yend   = QYTCeil2Int(yPixelStart + 0.5f + yPixelCount +
                              filter->yWidth);
    }
    
    void QYTImageFilm::getPixelExtent(int *xstart, int *xend, int *ystart, int *yend) const
    {
        *xstart = xPixelStart;
        *xend   = xPixelStart + xPixelCount;
        *ystart = yPixelStart;
        *yend   = yPixelStart + yPixelCount;
    }
    
    void QYTImageFilm::updateDisplay(int x0, int y0, int x1, int y1, float splatScale)
    {
        
    }
    
    void QYTImageFilm::writeImage(float splatScale)
    {
        int nPix = xPixelCount * yPixelCount;
        float *rgb = new float[3*nPix];
        int offset = 0;
        for (int y = 0; y < yPixelCount; ++y) {
            for (int x = 0; x < xPixelCount; ++x) {
                // Convert pixel XYZ color to RGB
                XYZToRGB(pixels[x*xPixelCount+y].Lxyz, &rgb[3*offset]);
                
                // Normalize pixel with weight sum
                float weightSum = pixels[x*xPixelCount+y].weightSum;
                if (weightSum != 0.f) {
                    float invWt = 1.f / weightSum;
                    rgb[3*offset  ] = std::max(0.f, rgb[3*offset  ] * invWt);
                    rgb[3*offset+1] = std::max(0.f, rgb[3*offset+1] * invWt);
                    rgb[3*offset+2] = std::max(0.f, rgb[3*offset+2] * invWt);
                }
                
                // Add splat value at pixel
                float splatRGB[3];
                XYZToRGB(pixels[x*xPixelCount+y].splatXYZ, splatRGB);
                rgb[3*offset  ] += splatScale * splatRGB[0];
                rgb[3*offset+1] += splatScale * splatRGB[1];
                rgb[3*offset+2] += splatScale * splatRGB[2];
                ++offset;
            }
        }
        
        QYTWriteImageTGA(filename, rgb, nullptr,
                         xPixelCount, yPixelCount, xResolution,
                         yResolution, xPixelStart, yPixelStart);
    }
}
