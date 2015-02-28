//
//  QYTPoint3.h
//  qyt_render_ver1.5
//
//  Created by nocolor on 15/2/18.
//  Copyright (c) 2015年 ___JinJiangTiaoZhanBei___. All rights reserved.
//

#ifndef __qyt_render_ver1_5__QYTPoint3__
#define __qyt_render_ver1_5__QYTPoint3__

#include "qytr.h"

namespace QYT
{
    class QYTVector3;
    
    ///表示三维点的类，支持数学上的点的基本运算。
    class QYTPoint3
    {
    public:
        QYTReal x, y, z;
        
        ///检查向量中是否含有NaN。有返回true，没有返回false。
        bool hasNan() const;
        
        //构造函数
        QYTPoint3():x(0.f),y(0.f),z(0.f){}
        QYTPoint3(QYTReal _val):x(_val),y(_val),z(_val){Assert(!hasNan());}
        QYTPoint3(QYTReal _x, QYTReal _y, QYTReal _z):x(_x),y(_y),z(_z){Assert(!hasNan());}
        explicit QYTPoint3(const QYTVector3& _p);
        
        //赋值操作
        QYTPoint3& operator=(const QYTPoint3& _rhs);
        
        //加法运算--------------------------------------------------
        
        ///点+向量 = 点
        QYTPoint3 operator+(const QYTVector3& _rhs) const;
        
        //无定义：点+标量
        //无定义：标量+点
        //无定义：点+点
    
        //减法运算--------------------------------------------------
        
        ///求负
        QYTPoint3 operator-() const;
        
        ///点-向量=点
        QYTPoint3 operator-(const QYTVector3& _rhs) const;
        
        ///点-点=向量
        QYTVector3 operator-(const QYTPoint3& _rhs) const;
        
        //无定义：点-标量
        //无定义：标量-点
        
        //乘法运算--------------------------------------------------
        
        ///点*标量=点
        QYTPoint3 operator*(QYTReal _rhs) const;
        
        ///标量*点=点
        friend QYTPoint3 operator*(QYTReal _lhs, const QYTPoint3& _rhs);
        
        //无定义：点*点
        //无定义：点*向量
        
        //除法运算--------------------------------------------------
        
        ///点/标量=点
        QYTPoint3 operator/(QYTReal _rhs) const;
        
        //无定义：点/点
        //无定义：点/向量
        //无定义：向量/点
        //无定义：标量/向量
        
        //判断比较--------------------------------------------------
        bool operator==(const QYTPoint3& _rhs) const;
        bool operator!=(const QYTPoint3& _rhs) const;
        
        //其他操作--------------------------------------------------
        
        QYTPoint3& operator+=(const QYTVector3& _v)
        {
            *this = *this+_v;
            return *this;
                
        }
        
        QYTPoint3& operator-=(const QYTVector3& _v)
        {
            *this = *this-_v;
            return *this;
        }
        
        QYTPoint3& operator*=(QYTReal _s)
        {
            *this = *this*_s;
            return *this;
        }
        
        QYTPoint3& operator/=(QYTReal _s)
        {
            *this = *this / _s;
            return *this;
        }
        
        ///0->x, 1->y, 2->z，下标越界会导致异常退出。
        QYTReal operator[](int _i) const;
        
        ///0->x, 1->y, 2->z，下标越界会导致异常退出。
        QYTReal& operator[](int _i);
        
        ///按照[x, y, z]的格式输出向量，默认小数点后4位。
        friend std::ostream& operator << (std::ostream& os, const QYTPoint3& _rhs);
        
        //工具函数-----------------------------------------
        
        ///计算两个点之间的距离的平方
        static QYTReal DistanceSquared(const QYTPoint3& p1, const QYTPoint3& p2);
        
        ///计算两个点之间的距离
        static QYTReal Distance(const QYTPoint3& p1, const QYTPoint3& p2);
    };

}

#endif /* defined(__qyt_render_ver1_5__QYTPoint3__) */
