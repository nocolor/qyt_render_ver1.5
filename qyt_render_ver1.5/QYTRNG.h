//
//  QYTRNG.h
//  QYTRNG
//
//  Created by nocolor on 15/3/6.
//  Copyright (c) 2015年 ___JinJiangTiaoZhanBei___. All rights reserved.
//

#ifndef __QYTRNG__QYTRNG__
#define __QYTRNG__QYTRNG__

#include <random>
#include <iostream>
#include <ctype.h>

namespace QYT
{
    typedef std::default_random_engine rEngine;
    typedef std::uniform_int_distribution<uint32_t> distrUInt;
    typedef std::uniform_real_distribution<float> distrFloat;
    
    class QYTRNG
    {
    public:
        QYTRNG(){}
        float randomFloat(float min, float max) const
        {
            static rEngine e{std::random_device{}()};
            static distrFloat floatDistriubtion;
            return floatDistriubtion(e, decltype(floatDistriubtion)::param_type(min, max));
        }
        uint32_t randomUInt(uint32_t min, uint32_t max) const
        {
            static rEngine e{std::random_device{}()};
            static distrUInt uintDistriubtion;
            return uintDistriubtion(e, decltype(uintDistriubtion)::param_type(min, max));
        }
    };
}

#endif /* defined(__QYTRNG__QYTRNG__) */
