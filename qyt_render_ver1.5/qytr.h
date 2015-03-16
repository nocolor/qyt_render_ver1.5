//
//  qytr.h
//  lightCoreRender
//
//  Created by nocolor on 14/11/5.
//  Copyright (c) 2014年 ___NOCOLOR___. All rights reserved.
//

#if defined(_MSC_VER)
#pragma once
#endif

#ifndef lightCoreRender_qytr_h
#define lightCoreRender_qytr_h

#if defined(_WIN32) || defined(_WIN64)
    #define QYTR_IS_WINDOWS
    #elif defined(__linux__)
    #define QYTR_IS_LINUX
    #elif defined(__APPLE__)
        #define QYTR_IS_APPLE
        #if !(defined(__i386__) || defined(__amd64__))
        #define QYTR_IS_APPLE_PPC
        #else
        #define QYTR_IS_APPLE_X86
        #endif
    #elif defined(__OpenBSD__)
    #define QYTR_IS_OPENBSD
#endif

#include "QYTError.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <string.h>
#include <vector>
#include <memory>

#if !defined(QYTR_IS_APPLE) && !defined(QYTR_IS_OPENBSD)
#include <malloc.h>
#endif
#if !defined(QYTR_IS_WINDOWS) && !defined(QYTR_IS_APPLE) && !defined(QYTR_IS_OPENBSD)
#include <alloca.h>
#endif
#include <assert.h>
#include <algorithm>

#if defined(QYTR_IS_WINDOWS)
#include <float.h>
#define isnan _isnan
#define isinf(f) (!_finite((f)))
typedef __int8 int8_t;
typedef unsigned __int8 uint8_t;
typedef __int16 int16_t;
typedef unsigned __int16 uint16_t;
typedef __int32 int32_t;
typedef unsigned __int32 uint32_t;
typedef __int64 int64_t;
typedef unsigned __int64 uint64_t;


#pragma warning (disable : 4305) // double constant assigned to float
#pragma warning (disable : 4244) // int -> float conversion
#pragma warning (disable : 4267) // size_t -> unsigned int conversion
#endif

#if defined(QYTR_IS_LINUX) || defined(QYTR_IS_APPLE)
#include <stdint.h>
#endif

#define ALLOCA(TYPE, COUNT) (TYPE*)alloca((COUNT)*sizeof(TYPE))

#define QYTR_VERSION "0.1.5"
#ifdef M_PI
#undef M_PI
#endif
#define M_PI       3.14159265358979323846f
#define INV_PI     0.31830988618379067154f
#define INV_TWOPI  0.15915494309189533577f
#define INV_FOURPI 0.07957747154594766788f
#ifndef INFINITY
#define INFINITY FLT_MAX

#endif

#ifndef QYTR_POINTER_SIZE
#if defined(__amd64__) || defined(_M_X64)
#define QYTR_POINTER_SIZE 8
#elif defined(__i386__) || defined(_M_IX86)
#define QYTR_POINTER_SIZE 4
#endif
#endif

#ifndef  QYTR_HAS_64_BIT_ATOMICS
#if (QYTR_POINTER_SIZE == 8)
#define QYTR_HAS_64_BIT_ATOMICS
#endif
#endif

#ifdef NDEBUG
#define Assert(expr) ((void)0)
#else
#define Assert(expr) \
        ((expr) ? (void)0 : \
        QYT::QYTSevere("Assertion \"%s\" failed in %s, line %d", \
                        #expr, __FILE__, __LINE__))
#endif




namespace QYT
{
    const float 	invRAND_MAX = 1.0 / (float)RAND_MAX;
    static const float OneMinusEpsilon=0x1.fffffep-1;
    
    ///代表实数的类型
    typedef float QYTReal;
    
    /**
     *线性插值函数，在v1和v2中插值。
     @param v1  左端的值
     @param v2  右端的值
     @param t   插值的比例
     @return    (1.f - t)*v1 + t*v2
     */
    inline float QYTLerp(float t, float v1, float v2)
    {
        return (1.f - t)*v1 + t*v2;
    }
    
    /**
     *  钳制函数， 将val钳制在low和hight之间。注意high应当大于low。
     @param val 需要进行钳制对比的值
     @param low 较小值
     @param high    较大值
     @return    如果val小于low，返回low，如果val大于high，返回high，否则返回val
     */
    template<class Type>
    inline Type QYTClamp(Type val, Type low, Type high)
    {
        if (val < low) return low;
        else if (val > high) return high;
        else return val;
    }
    
    /**
     *  返回a除以b的余数
     */
    inline int QYTMod(int a, int b)
    {
        int n = int(a/b);
        a -= n*b;
        if (a < 0) a += b;
        return a;
    }
    
    /**
     *  将角度单位换算为弧度单位。
     @param deg 角度单位下的角度值（一般为0~360度）
     @return 弧度值（一般为0~2Pai）
     */
    inline float QYTRadians(float deg)
    {
        return ((float)M_PI/180.f) * deg;
    }
    
    /**
     *  将弧度单位换算为角度单位。
     @param rad 弧度单位下的弧度值
     @return 角度值
     */
    inline float QYTDegrees(float rad)
    {
        return (180.f/(float)M_PI) * rad;
    }
    
    /**
     *  以2为底，x的对数。
     */
    inline float QYTLog2(float x)
    {
        float invLog2 = 1.f / logf(2.f);
        return logf(x) * invLog2;
    }
    
    /**
     *  判断v是否是2的整数次幂。
     */
    inline bool QYTIsPowerOf2(int v)
    {
        return (v & (v - 1)) == 0;
    }
    
    /**
     *  返回与v最接近的2的整数次幂的数。例如v=1，则返回2(2^1)；v=5，返回8(2^3)。
     */
    inline uint32_t QYTRoundUpPow2(uint32_t v) {
        v--;
        v |= v >> 1;    v |= v >> 2;
        v |= v >> 4;    v |= v >> 8;
        v |= v >> 16;
        return v+1;
    }
    
    /**
     *  对一元二次方程求解。
     *  形如Ax^2 + Bx + C = 0的方程的系数一次对应A，B和C。根会储存在t0和t1中。
     @return    如果方程有解，返回true，否则返回false
     */
    inline bool QYTQuadratic(float A, float B, float C, float *t0, float *t1)
    {
        // Find quadratic discriminant
        float discrim = B * B - 4.f * A * C;
        if (discrim < 0.) return false;
        float rootDiscrim = sqrtf(discrim);
        
        // Compute quadratic _t_ values
        float q;
        if (B < 0) q = -.5f * (B - rootDiscrim);
        else       q = -.5f * (B + rootDiscrim);
        *t0 = q / A;
        *t1 = C / q;
        if (*t0 > *t1) std::swap(*t0, *t1);
        return true;
    }
    
    inline int QYTFloor2Int(float val);
    
    inline int QYTLog2Int(float v) {
        return QYTFloor2Int(QYTLog2(v));
    }
    
    ///计算参数的下限，并返回（int）
    inline int QYTFloor2Int(float val) {
        return (int)floorf(val);
    }
    
    
    inline int QYTRound2Int(float val) {
        return QYTFloor2Int(val + 0.5f);
    }
    
    
    inline int QYTFloat2Int(float val) {
        return (int)val;
    }
    
    ///返回大于或者等于指定表达式的最小整数(int)
    inline int QYTCeil2Int(float val) {
        return (int)ceilf(val);
    }
    
    inline int QYTRandInt()
    {
        return(rand());
    }
    
    inline float QYTRandFloat()
    {
        return((float)QYTRandInt() * invRAND_MAX);
    }
    
    inline float QYTRandFloat(float l, float h)
    {
        return (QYTRandFloat() * (h - l) + l);
    }
    
    inline int QYTRandInt(int l, int h)
    {
        return ((int) (QYTRandFloat(0, h - l + 1) + l));
    }
};

#endif
