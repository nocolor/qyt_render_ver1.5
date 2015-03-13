//
//  QYTVector3.h
//  qyt_render_ver1.5
//
//  Created by nocolor on 15/2/18.
//  Copyright (c) 2015年 ___JinJiangTiaoZhanBei___. All rights reserved.
//

#ifndef __qyt_render_ver1_5__QYTVector3__
#define __qyt_render_ver1_5__QYTVector3__

#include "qytr.h"

namespace QYT
{
    class QYTPoint3;
    
    /**
     @class QYTVector3
     @brief 代表三维向量的类，遵循三维向量的基本运算。
     */
    class QYTVector3
    {
    public:
        QYTReal x, y, z;
        
        ///检查向量中是否含有NaN。有返回true，没有返回false。
        bool hasNan() const;
        
        //构造函数
        QYTVector3():x(0.f),y(0.f),z(0.f){}
        QYTVector3(QYTReal _val):x(_val),y(_val),z(_val){Assert(!hasNan());}
        QYTVector3(QYTReal _x, QYTReal _y, QYTReal _z):x(_x),y(_y),z(_z){Assert(!hasNan());}
        explicit QYTVector3(const QYTPoint3& _p);
        
        //复制构造函数
        QYTVector3(const QYTVector3& _v):x(_v.x),y(_v.y),z(_v.z){Assert(!hasNan());}
        
        //赋值操作
        QYTVector3& operator=(const QYTVector3& _rhs);
        
        //加法运算--------------------------------------------------
        
        ///向量+向量=向量
        QYTVector3 operator+(const QYTVector3& _rhs) const;
        
        ///向量+标量=向量
        QYTVector3 operator+(QYTReal _rhs) const;
        
        ///向量+点=点（点的平移）
        QYTPoint3 operator+(const QYTPoint3& _rhs) const;
        
        ///标量+向量=向量
        friend QYTVector3 operator+(QYTReal _lhs, const QYTVector3& _rhs);
        
        //减法运算--------------------------------------------------
        
        ///向量-向量=向量
        QYTVector3 operator-(const QYTVector3& _rhs) const;
        
        ///对自身求负
        QYTVector3 operator-() const;
        
        ///向量-标量=向量
        QYTVector3 operator-(QYTReal _rhs) const;
        
        //没有定义 标量-向量
        
        //没有定义 向量-点
        
        //乘法运算--------------------------------------------------
        
        ///向量*标量=向量
        QYTVector3 operator*(QYTReal _rhs) const;
        
        ///向量*向量=向量（各分量分别相乘）
        QYTVector3 operator*(const QYTVector3& _rhs) const;
        
        ///标量*向量 = 向量
        friend QYTVector3 operator*(QYTReal _lhs, const QYTVector3& _rhs);
        
        //除法运算
        ///向量/向量=向量（各个分量相除）
        QYTVector3 operator/(const QYTVector3& _rhs) const;
        
        ///向量/标量=向量
        QYTVector3 operator/(QYTReal _rhs) const;
        
        //判断比较--------------------------------------------------
        
        /**
        两个向量相等返回true，否则返回false。
        相等指的是数值相等，不是数学上的方向相等，例如
        [1, 1, 1]和[2, 2, 2]数学上是相等的，因为它们指向相同的方向，
        但是数值上不相等。若要判断数学上的向量相等，应该先将向量归一化，
        或者使用函数isEqual()。
        @see bool isEqualDirection(const QYTVector3& _rhs) const
         */
        bool operator==(const QYTVector3& _rhs) const;
        
        ///判断向量数值上是否不相等，若需要判断数学上的相等，先归一化向量，
        ///或者使用函数isEqualDirection()。
        bool operator!=(const QYTVector3& _rhs) const;
        
        //其他操作--------------------------------------------------
        
        QYTVector3& operator+=(const QYTVector3& _v)
        {
            if(this != &_v)
                *this = *this+_v;
            return *this;
        }
        
        QYTVector3& operator+=(QYTReal _s)
        {
            *this = *this + _s;
            return *this;
        }
        
        QYTVector3& operator-=(const QYTVector3& _v)
        {
            if(this != &_v)
                *this = *this - _v;
            return *this;
        }
        
        QYTVector3& operator-=(const QYTReal _s)
        {
            *this = *this-_s;
            return *this;
        }
        
        QYTVector3& operator*=(const QYTVector3& _v)
        {
            if(this != &_v)
                *this = *this - _v;
            return *this;
        }
        
        QYTVector3& operator*=(QYTReal _s)
        {
            *this = *this*_s;
            return *this;
        }
        
        QYTVector3& operator/=(const QYTVector3& _v)
        {
            if(this != &_v)
                *this = *this/_v;
            return *this;
        }
        
        QYTVector3& operator/=(QYTReal _s)
        {
            *this = *this/_s;
            return *this;
        }
        
        ///0->x, 1->y, 2->z，下标越界会导致异常退出。
        QYTReal operator[](int _i) const;
        
        ///0->x, 1->y, 2->z，下标越界会导致异常退出。
        QYTReal& operator[](int _i);
        
        ///按照[x, y, z]的格式输出向量，默认小数点后4位。
        friend std::ostream& operator << (std::ostream& os, const QYTVector3& _rhs);
        
        //工具函数--------------------------------------------------
        
        /**
         判断_v1与_v2是否是数学上的相等（是否指向相同的方向，而不是单纯比较数值）。
         例如，向量[1, 1, 1]和[99, 99, 99]是数学上相同的，因为它们指向相同的方向。
         @return 如果当前向量和_v方向相同，返回true，否则返回false
         */
        static bool IsEqualDirection(const QYTVector3& _v1, const QYTVector3& _v2);
        
        /**
         归一化向量。
         @return 方向与_v相同，但是长度（模）为1的单位向量
         */
        static QYTVector3 Normalize(const QYTVector3& _v);
        
        ///计算_v的模。
        static QYTReal Length(const QYTVector3& _v);
        
        ///计算_v的模的平方。
        static QYTReal LengthSquared(const QYTVector3& _v);
        
        ///计算_v1和_v2之间夹角的余弦值
        static QYTReal Cos(const QYTVector3& _v1, const QYTVector3& _v2);
        
        ///计算_v1和_v2的点积
        static QYTReal Dot(const QYTVector3& _v1, const QYTVector3& _v2);
        
        ///计算_v和_v2的点积的绝对值
        static QYTReal AbsDot(const QYTVector3& _v1, const QYTVector3& _v2);
        
        ///返回_v1和_v2的叉积，叉积是与两个向量所在的平面垂直的一个新向量。
        static QYTVector3 Cross(const QYTVector3& _v1, const QYTVector3& _v2);
        
        /**
         计算一个两两垂直的坐标系的基向量。
         @param v1  给定的向量
         @param v2，v3   与v1垂直并且自身也相互垂直的新向量，以指针形式保存
         */
        static void CoordinateSystem(const QYTVector3 &v1, QYTVector3 *v2, QYTVector3 *v3);
    };
    
 
    
    typedef QYTVector3 QYTVec3;
    typedef QYTVector3 QYTNormal3;
}

#endif /* defined(__qyt_render_ver1_5__QYTVector3__) */
