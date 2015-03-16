//
//  QYTStratifiedSampler.cpp
//  qyt_render_ver1.5
//
//  Created by nocolor on 15/3/6.
//  Copyright (c) 2015年 ___JinJiangTiaoZhanBei___. All rights reserved.
//

#include "QYTStratifiedSampler.h"

namespace QYT
{
    QYTStratifiedSampler::QYTStratifiedSampler(int xstart, int xend,
                                               int ystart, int yend,
                                               int xs, int ys, bool jitter,
                                               float sopen, float sclose):
    QYTSampler(xstart, xend, ystart, yend, xs * ys, sopen, sclose)
    {
        jitterSamples = jitter;
        xPos = xPixelStart;
        yPos = yPixelStart;
        xPixelSamples = xs;
        yPixelSamples = ys;
        
        sampleBuf = new float[5 * xPixelSamples * yPixelSamples];
    }
    
    QYTStratifiedSampler::~QYTStratifiedSampler()
    {
        delete [] sampleBuf;
    }
    
    QYTSampler* QYTStratifiedSampler::getSubSampler(int num, int count)
    {
        int x0, x1, y0, y1;
        computeSubWindow(num, count, &x0, &x1, &y0, &y1);
        if (x0 == x1 || y0 == y1)
            return nullptr;
        return new QYTStratifiedSampler(x0, x1, y0, y1,
                                        xPixelSamples, yPixelSamples,
                                        jitterSamples,
                                        shutterOpen,
                                        shutterClose);
    }
    
    int QYTStratifiedSampler::getMoreSamples(QYT::QYTSample *samples, const QYT::QYTRNG &rng)
    {
        if(yPos == yPixelEnd)
            return 0;
        int nSamples = xPixelSamples*yPixelSamples;
        
        //一次性地计算出图像、时间和镜头采样，跟单独计算出各个采样值相比，这样做可以计算出具有更好分布模式的时间和镜头采样。
        float *bufp = sampleBuf;
        float *imageSamples = bufp; bufp += 2 * nSamples;
        float *lensSamples = bufp;  bufp += 2 * nSamples;
        float *timeSamples = bufp;
        
        QYTStratifiedSample2D(imageSamples, xPixelSamples, yPixelSamples, rng, jitterSamples);
        
        QYTStratifiedSample2D(lensSamples, xPixelSamples, yPixelSamples, rng, jitterSamples);
        
        QYTStratifiedSample1D(timeSamples, xPixelSamples*yPixelSamples, rng, jitterSamples);
        
        
        for (int o = 0; o < 2 * xPixelSamples * yPixelSamples; o += 2)
        {
            imageSamples[o]   += xPos;
            imageSamples[o+1] += yPos;
            
//            std::cout << "##" << imageSamples[o] << ", " << imageSamples[o+1] << "##\n";
            
        }
        
        for (int i = 0; i < nSamples; ++i)
        {
            samples[i].imageX = imageSamples[2*i];
            samples[i].imageY = imageSamples[2*i+1];
            samples[i].lensU = lensSamples[2*i];
            samples[i].lensV = lensSamples[2*i+1];
            samples[i].time = QYTLerp(timeSamples[i], shutterOpen, shutterClose);
            // Generate stratified samples for integrators
            for (uint32_t j = 0; j < samples[i].n1D.size(); ++j)
                QYTLatinHypercube(samples[i].oneD[j], samples[i].n1D[j], 1, rng);
            for (uint32_t j = 0; j < samples[i].n2D.size(); ++j)
                QYTLatinHypercube(samples[i].twoD[j], samples[i].n2D[j], 2, rng);
        }
        
        // Advance to next pixel for stratified sampling
        if (++xPos == xPixelEnd)
        {
            xPos = xPixelStart;
            ++yPos;
        }
        return nSamples;
    }
    
}
