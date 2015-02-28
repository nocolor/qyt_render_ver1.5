//
//  QYTSampler.h
//  QYTRayTracer
//
//  Created by nocolor on 14-4-11.
//  Copyright (c) 2014年 QinYuTong. All rights reserved.
//

#ifndef __QYTRayTracer__QYTSampler__
#define __QYTRayTracer__QYTSampler__

#include "qytr.h"
#include "QYTPoint3.h"
#include <vector>

namespace QYT
{
    /**
     *  QYTSampler是采样器的基类，所有的采样器都将从这个类派生。
     *  采样技术在渲染中会被广泛的应用，其中包括抗锯齿的细节，或者
     *  当向场景中添加区域光照和软阴影的时候，会对光照表面进行采样，
     *  如果需要实现全局光照和光泽反射，则需要对BRDF和BRTF进行采样。
     *  鉴于采样技术如此重要，有必要专门开发相应的采样框架。
     *  采样器一般在初始化的时候就生成了所有的采样点，并保存在对象中，
     *  这样可以避免在光线追踪的过程中进行采样而提高渲染效率。
     */
    class QYTSampler
    {
    public:
        //大体上，除了采样器中，没有其他地方用得到二维点，因此只提供简单定义
        struct QYTPoint2d
        {
            QYTReal x, y;
        };
        
    protected:
        ///每一个采样集中的采样点数量，一般来说，这个数是2的幂。
        int                         numOfSamples;
        
        ///采样集的数量。
        int                         numOfSets;
        
        ///当前使用过的采样点的数量。
        size_t                      countOfSamplesUsed;
        
        ///随机索引偏移。
        int                         jump;
        
        ///混合索引。
        std::vector<int>            shuffledIndices;
        
        ///一个单位正方形内的采样点，采样点被储存在这个向量中。
        std::vector<QYTPoint2d>     squareSamples;
        
        ///一个单位圆盘内的采样点，采样点被储存在这个向量中。
        std::vector<QYTPoint2d>     diskSamples;
        
        ///一个单位半球内的采样点，采样点被储存在这个向量中。
        std::vector<QYTPoint3>     hemisphereSamples;
        
        ///一个单位球体内的采样点，采样点被储存在这个向量中。
        std::vector<QYTPoint3>     sphereSamples;
        
    public:
        QYTSampler():
        numOfSamples(1),
        numOfSets(83),
        countOfSamplesUsed(0),
        jump(0)
        {
            squareSamples.reserve(numOfSamples*numOfSets);
            setupShuffledIndices();
        }
        
        QYTSampler(int num):
        numOfSamples(num),
        numOfSets(83),
        countOfSamplesUsed(0),
        jump(0)
        {
            squareSamples.reserve(numOfSamples*numOfSets);
            setupShuffledIndices();
        }
        
        QYTSampler(int num, int numSet):
        numOfSamples(num),
        numOfSets(numSet),
        countOfSamplesUsed(0),
        jump(0)
        {
            squareSamples.reserve(numOfSamples*numOfSets);
            setupShuffledIndices();
        }
        
        QYTSampler(const QYTSampler& rhs):
        numOfSamples(rhs.numOfSamples),
        numOfSets(rhs.numOfSets),
        squareSamples(rhs.squareSamples),
        shuffledIndices(rhs.shuffledIndices),
        diskSamples(rhs.diskSamples),
        hemisphereSamples(rhs.hemisphereSamples),
        sphereSamples(rhs.sphereSamples),
        countOfSamplesUsed(rhs.countOfSamplesUsed),
        jump(rhs.jump)
        {}
        
        virtual ~QYTSampler(){}
        
        QYTSampler& operator = (const QYTSampler& rhs)
        {
            if(this == &rhs)
                return *this;
            
            numOfSamples = rhs.numOfSamples;
            numOfSets = rhs.numOfSets;
            squareSamples = rhs.squareSamples;
            shuffledIndices = rhs.shuffledIndices;
            diskSamples = rhs.diskSamples;
            hemisphereSamples = rhs.hemisphereSamples;
            sphereSamples = rhs.sphereSamples;
            countOfSamplesUsed = rhs.countOfSamplesUsed;
            jump = rhs.jump;
            return *this;
        }
        
        virtual QYTSampler* copy() const = 0;
        
        /**
         *  设置采样集的数量。
         *  @param  num     采样集的数量
         */
        inline void setNumberOfSets(int num)
        {
            numOfSets = num;
        }
        
        /**
         *  设置采样点的数量。
         *  @param  num     采样点的数量
         */
        inline void setNumberOfSamples(int num)
        {
            numOfSamples = num;
        }
        
        /**
         *  获得采样点的数量。
         *  @return 返回当前采样器的采样数量
         */
        inline int getNumberOfSamples() const
        {
            return numOfSamples;
        }
        
        /**
         *  获得采样集的数量。
         *  @return 返回当前采样器的采样集数量
         */
        inline int getNumberOfSets() const
        {
            return numOfSets;
        }
        
        /**
         *  创建采样点，根据设置的采样数量和采样集数量创建采样，
         *  采样结果将被储存到对象中的向量中。
         */
        virtual void generateSamples() = 0;
        
        /**
         *  混合采样索引。
         */
        void setupShuffledIndices();
        
        /**
         *  将采样点映射到单位圆盘中。
         */
        void mapSamplesToUnitDisk();
        
        /**
         *  降采样点映射到半球中。
         *  采样点的表面密度值d与极角∂之间的关系为：d = （cos∂）^e，
         *  其中e ≥ 0。
         *  @param  e   控制极角变大时d值的下降速度，e越大，则下降的速度愉快。
         */
        void mapSamplesToHemisphere(float e);
        
        /**
         *  将采样点映射到单位球体中。
         */
        void mapSamplesToSphere();
        
        /**
         *  获得单位矩形中的下一个采样点。
         *  @return 单位矩形中的下一个采样点
         */
        QYTPoint2d nextSampleInUnitSquare();
        
        /**
         *  获得单位圆盘中的下一个采样点。
         *  @return 单位圆盘中的下一个采样点
         */
        QYTPoint2d nextSampleInUnitDisk();
        
        /**
         *  获得单位半球中的下一个采样点。
         *  @return 单位半球中的下一个采样点
         */
        QYTPoint3 nextSampleInHemishpere();
        
        /**
         *  获得单位球中的下一个采样点。
         *  @return 单位球中的下一个采样点
         */
        QYTPoint3 nextSampleInSphere();
    };
}

#endif /* defined(__QYTRayTracer__QYTSampler__) */
