//
//  QYTMultiJitteredSampler.h
//  QYTRayTracer
//
//  Created by nocolor on 14-4-11.
//  Copyright (c) 2014年 QinYuTong. All rights reserved.
//

#ifndef __QYTRayTracer__QYTMultiJitteredSampler__
#define __QYTRayTracer__QYTMultiJitteredSampler__

#include "QYTSampler.h"
#include <random>

namespace QYT
{
    /**
     *  QYTMultiJitteredSampler实现了多重抖动采样的技术。
     */
    class QYTMultiJitteredSampler: public QYTSampler
    {
    public:
        QYTMultiJitteredSampler():QYTSampler()
        {}
        
        QYTMultiJitteredSampler(int _numOfSamples):QYTSampler(_numOfSamples)
        {
            generateSamples();
        }
        
        QYTMultiJitteredSampler(int _numOfSamples, int _numOfSets):QYTSampler(_numOfSamples, _numOfSets)
        {
            generateSamples();
        }
        
        QYTMultiJitteredSampler(const QYTMultiJitteredSampler& rhs):QYTSampler(rhs)
        {
            generateSamples();
        }
        
        QYTMultiJitteredSampler& operator = (const QYTMultiJitteredSampler& rhs)
        {
            if(this == &rhs)
                return *this;
            QYTSampler::operator=(rhs);
            return *this;
        }
        
        inline virtual QYTMultiJitteredSampler* copy() const
        {
            return (new QYTMultiJitteredSampler(*this));
        }
        
        inline virtual ~QYTMultiJitteredSampler(){}
        
    private:
        virtual void generateSamples();
    };
}

#endif /* defined(__QYTRayTracer__QYTMultiJitteredSampler__) */
