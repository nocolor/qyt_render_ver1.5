//
//  QYTStratifiedSampler.h
//  qyt_render_ver1.5
//
//  Created by nocolor on 15/3/6.
//  Copyright (c) 2015年 ___JinJiangTiaoZhanBei___. All rights reserved.
//

#ifndef __qyt_render_ver1_5__QYTStratifiedSampler__
#define __qyt_render_ver1_5__QYTStratifiedSampler__

#include "qytr.h"
#include "QYTSampler.h"

namespace QYT
{
    /**
     @class QYTStratifiedSampler
     分层采样生成器把图像平面分割为矩形区域，并在每个区域中生成一个采样。这些区域通常称为层(strata)，
     这个采样器就被称为分层采样器(Stratified Sampler)。分层的关键思想是把采样区域分为不重叠的区域，
     并从每个区域中取一个采样，这样我们就有可能丢失图像的重要特征，因为采样无法保证保持足够近的距离。
     从另一个角度讲，如果许多的采样都来自采样空间中的临近点，这也对我们没有什么好处，因为每个新采样对
     图像函数而言没有增加什么新的信息。从信号处理的角度来看，我们隐性地定义了总体采样速率：层(strata)
     越小，则层就越多，采样速率就越高。
     
     分层采样器利用“颤动”(jittering)的方式在每个层中生成采样的随机点：即把层的中心点随机地偏移一个位
     移，最大不超过层的宽或高的一半。这种颤动所产生的不均匀性把走样转换成了噪声。这个采样器也提供了非颤
     动模式，即在层中进行均匀采样，这个模式有益于不同的采样技术的比较，但并不用来生成最后的图像。
     */
    class QYTStratifiedSampler: public QYTSampler
    {
    private:
        ///x和y方向上的层数
        int xPixelSamples, yPixelSamples;
        
        ///是否使用抖动，true为使用抖动，否则不使用
        bool jitterSamples;
        
        ///当前像素的坐标
        int xPos, yPos;
        float *sampleBuf;
        
    public:
        QYTStratifiedSampler(int xstart, int xend, int ystart, int yend,
                             int xs, int ys, bool jitter, float sopen, float sclose);
        
        ~QYTStratifiedSampler();
        
        int roundSize(int size) const override { return size; }
        
        int getMoreSamples(QYTSample *sample,
                           const QYTRNG& rng) override;
        
        QYTSampler *getSubSampler(int num, int count) override;
        int maximumSampleCount() override
        { return xPixelSamples * yPixelSamples; }
    };
}

#endif /* defined(__qyt_render_ver1_5__QYTStratifiedSampler__) */
