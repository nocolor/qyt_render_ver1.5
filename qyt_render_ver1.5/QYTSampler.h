//
//  QYTSampler.h
//  qyt_render_ver1.5
//
//  Created by nocolor on 15/3/1.
//  Copyright (c) 2015年 ___JinJiangTiaoZhanBei___. All rights reserved.
//

#ifndef __qyt_render_ver1_5__QYTSampler__
#define __qyt_render_ver1_5__QYTSampler__

#include "QYTSpectrum.h"
#include "QYTRNG.h"

namespace QYT
{
    struct QYTSample;
    
    /**
     @class QYTSampler
     所有采样器的实现都继承了抽象类QYTSampler，而QYTSampler定义了它们的共用接口。
     采样器的任务是生成一个多维采样位置的序列。其中两个维给出了光栅空间的图像
     采样位置，另一个维给出了采样的时间，时间值的范围是从0到1，并比例变换到相
     机快门打开的时间周期中。还有两个采样值给出了为了计算景深的镜头位置(u,v)；
     它们的范围也是从0到1。
     */
    class QYTSampler
    {
    protected:
        /**
         computeSubWindow() is a utility function that computes a pixel
         sampling range given a tile number num and a total number of tiles
         count.
         */
        void computeSubWindow(int num, int count, int *xstart,
                              int *xend, int *ystart, int *yend) const;
        
    public:
        ///QYTSampler为x坐标范围是xPixelStart到xPixelEnd-1以及y坐标范围是
        ///yPixelStart到yPixelEnd-1的像素生成采样
        const int xPixelStart, xPixelEnd, yPixelStart, yPixelEnd;
        
        ///为每个像素所生成的采样个数
        const int samplesPerPixel;
        
        ///实现动态模糊需要的参数，分别是快门打开的时间和快门关闭的时间
        const QYTReal shutterOpen, shutterClose;
        
        
        virtual ~QYTSampler();
        
        ///所有Sampler的实现都传给基类构造器几个共同的参数，包括图像的分辨率以及为每个像素所生成的采样个数。
        QYTSampler(int xstart, int xend, int ystart, int yend,
                   int spp, QYTReal sopen, QYTReal sclose);
        
        /**
         产生一个或者多个采样点数组。这些数组中的值取决于需求。
         @param sample            设置了具体参数的采样器指针
         @param distributions     分布器，能够把generator产生的均匀分布值映射到其他常见分布，如均匀分布uniform，正态分布normal，二项分布binomial，泊松分布poisson
         @param generator         生成器，能够产生离散的等可能分布数值，
         @return    如果真实产生了采样点，就返回采样点的数目，否则返回0
         */
        virtual int getMoreSamples(QYTSample *sample,
                                   const QYTRNG& rng)=0;
        
        ///返回一次性产生的采样点数量的最大值。
        virtual int maximumSampleCount() = 0;
        
//        virtual bool reportResults(QYTSampler *samples, const RayDifferential *rays,
//                                   const Spectrum *Ls, const Intersection *isects, int count);
        
        /**
         返回一个负责为图像的一部分生成采样点的子采样器。
         @param num     从0到（总采样数-1），总采样数是当前子采样器的采样点的总数
         @param count   总采样数，即当前子采样器的采样点的总数
         */
        virtual QYTSampler *getSubSampler(int num, int count) = 0;
        
        /**
         返回一个当前采样器（使用当前的某种采样技术）最适宜产生的采样点的数量。
         某些采样技术会在特定的采样数量时获得更好地采样效果，比如如果采样数是
         2的指数倍（比如256个采样点），那么采样算法会工作的更高效。
         */
        virtual int roundSize(int size) const = 0;
    };
    
    ///在Sampler中，相机所使用的数据是固定的
    struct QYTCameraSample
    {
        QYTReal imageX, imageY;
        QYTReal lensU, lensV;
        QYTReal time;
    };
    
    /**
     采样器用QYTSample结构来存放一个采样。
     对于要生成的每条相机光线，我们把这个QYTSample的指针传给Sampler进行初始化。然后把这个
     QYTSample传给相机和积分器，利用它的值来构造相机光线和进行照明计算。

     QYTSampler负责把所生成的采样存贮在QYTSample:oneD和QYTSample::twoD两个数组里。对
     于一维采样模式，QYTSampler要生成n1D.size()个相互独立的模式，第i个模式有n1D[i]个采
     样值。这些值分别存放在数组元素oneD[0] 到oneD[n1D-1]中。
     @code
     //一维采样的储存情况，经典的（动态）二维数组
     QYTReal **oneD
     |
     |
     0---->new QYTReal*[n1D[0]](1.f, 0.312f, 1.3134f...)    //有n1D[0]个采样值
     1---->new QYTReal*[n1D[1]](...)                        //有n1D[1]个采样值
     2---->new QYTReal*[n1D[2]](...)                        //有n1D[2]个采样值
     3---->new QYTReal*[n1D[3]](...)                        //有n1D[3]个采样值
     ...
     n1D.size()-1---->new QYTReal*[n1D.size()-1](...)       //有n1D[n1D.size()-1]个采样值
     @endcode
     二维的处理过程跟一维相同，只不过第i个采样是两个数。它一次性地为两个采样数组oneD和twoD
     申请了足够的空间，而不是分两次申请。只需把twoD设置为正确的偏置值，即oneD的最后一个指针
     之后。这样的一次性申请可以保证oneD和twoD指向彼此靠近的内存位置，从而可以提高缓存效率。
     */
    struct QYTSample:public QYTCameraSample
    {
        
    public:
        std::vector<uint32_t> n1D, n2D;
        
        QYTReal **oneD, **twoD;
        
        QYTSample() { oneD = twoD = nullptr; }
        
        QYTSample(const QYTSample& s) = delete;
        QYTSample& operator=(const QYTSample& s) = delete;
        
        ~QYTSample()
        {
            if(oneD != nullptr)
            {
                int nPtrs = (uint32_t)(n1D.size() + n2D.size());
                for (int i = 0; i < nPtrs; ++i)
                    delete [] oneD[i];
            }
        }
        
        uint32_t add1D(uint32_t num)
        {
            n1D.push_back(num);
            return (uint32_t)n1D.size()-1;
        }
        
        uint32_t add2D(uint32_t num)
        {
            n2D.push_back(num);
            return (uint32_t)n2D.size()-1;
        }
        
        /**
         为当前采样重新分配内存，该操作会抹除之前的内存和采样。
         调用该操作后，当前的采样中被置为当前n1D和n2D中指定的
         采样模式。
         */
        void allocateSampleMemory();
        
        /**
         返回count个当前采样的副本的数组（的指针）。
         @code
         
         QYTSample sample;
         
         //添加采样模式
         sample.add1D(16);
         sample.add2D(8);
         ...
         
         //分配内存（但是采样中还没有采样值）
         sample.allocateSampleMemory();
         
         ///...添加采样点
         
         //获得3个sample的副本，并且以指针形式返回
         QYTSample *samples = sample.copy(3);
         
         //samples[0]，samples[1]，samples[2]都是sample的深拷贝
         @endcode
         */
        QYTSample* copy(int count) const;
    };
    
    void QYTStratifiedSample2D(float *samp, int nx, int ny,
                               const QYTRNG& rng,
                               bool jitter);
    
    void QYTStratifiedSample1D(float *samp, int nSamples,
                               const QYTRNG& rng,
                               bool jitter);
    
    template <typename T>
    void QYTShuffle(T *samp, uint32_t count, uint32_t dims,
                    const QYTRNG& rng)
    {
        for (uint32_t i = 0; i < count; ++i)
        {
            uint32_t other = i + (rng.randomUInt(0, RAND_MAX) % (count - i));
            for (uint32_t j = 0; j < dims; ++j)
                swap(samp[dims*i + j], samp[dims*other + j]);
        }
    }
    
    void QYTLatinHypercube(float *samples, uint32_t nSamples, uint32_t nDim,
                           const QYTRNG &rng);
    
}

#endif /* defined(__qyt_render_ver1_5__QYTSampler__) */
