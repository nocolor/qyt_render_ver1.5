//
//  qyt_ray.h
//  qyt_fundation
//
//  Created by nocolor on 15/2/6.
//  Copyright (c) 2015年 ___JinJiangTiaoZhanBei___. All rights reserved.
//

#ifndef __qyt_fundation__qyt_ray__
#define __qyt_fundation__qyt_ray__

#include "QYTMatrix4x4.h"
#include "QYTPoint3.h"
#include "QYTVector3.h"

namespace QYT
{
    /**
     @class qyt_ray
     表示射线的类，射线是数学上的抽象概念，它从一个点出发，并朝着某个方向无限延伸。
     射线可以由方程p=o + d*t描述，其中o是射线的源点，d是方向向量，t是常数（负无穷到正无穷）。
     射线可以用来代表光线追踪算法中的光线。
     */
    class qyt_ray
    {
    public:
        ///射线的源点
        QYTPoint3 o;
        
        ///射线的方向
        QYTVec3   d;
        
        ///t的取值，可以为其制定范围，范围是min_t~max_t
        mutable QYTReal min_t, max_t;
        
        ///光线作用在物体表面的时间，动态模糊效果需要这个变量
        QYTReal   time;
        
        ///递归深度，在全局光照算法中，要递归的追踪一条光线，当depth大于规定的最大递归深度时，要停止递归
        int depth;
        
        qyt_ray():min_t(0.f), max_t(INFINITY), time(0.f), depth(0){}
        
        qyt_ray(const QYTPoint3& _origin, const QYTVec3& _direction,
                QYTReal _start, QYTReal _end = INFINITY, QYTReal _time = 0.f, int _depth = 0)
        :o(_origin), d(_direction), min_t(_start), max_t(_end), time(_time), depth(_depth){}
        
        qyt_ray(const QYTPoint3& _origin, const QYTVec3& _direction, const qyt_ray& _parent,
                QYTReal _start, QYTReal _end = INFINITY)
        :o(_origin), d(_direction), min_t(_start), max_t(_end), time(_parent.time), depth(_parent.depth+1){}
        
        /**
         返回距离源点沿着d方向t个单位长度的点
         */
        QYTPoint3 operator()(QYTReal t) const
        {return  o+d*t;}
        
        friend std::ostream& operator << (std::ostream& os, const qyt_ray& _ray)
        {
            os << "origin:" << _ray.o << std::endl;
            os << "direction:" << _ray.d << std::endl;
            os << "start:" << _ray.min_t << " end:" << _ray.max_t << std::endl;
            os << "time:" << _ray.time << " depth:" << _ray.depth;
            return os;
            
        }
        
        bool hasNaNs() const {
            return (o.hasNan() || d.hasNan() ||
                    isnan(min_t) || isnan(max_t));
        }
    };
    
    typedef qyt_ray QYTRay;
    
    
    /**
     @classQYTRayDifferential
     为了更好地利用纹理函数进行反走样，对每条被追踪的光线都保持着一些附加的信息。 
     这些信息用在QYTTexture类中估算一小部分的场景在图像平面上的投影面积。这样，
     QYTTexture类就可以计算出纹理在这个面积上的平均值，从而得到更好的图像。
     
     QYTRayDifferential是QYTRay的子类，并包含两条辅助光线的附加信息。 这两条
     光线表示从主光线向x和y方向分别偏置一个像素而得到的相机光线。确定了这三条光线
     投射到被着色物体上的区域，Texture就可以估算出用于反走样的平均值。
     */
    class QYTRayDifferential : public QYTRay
    {
    public:

        QYTRayDifferential() { hasDifferentials = false; }
        QYTRayDifferential(const QYTPoint3 &org, const QYTVector3 &dir, QYTReal start,
                        QYTReal end = INFINITY, QYTReal t = 0.f, int d = 0)
        : QYTRay(org, dir, start, end, t, d) {
            hasDifferentials = false;
        }
        QYTRayDifferential(const QYTPoint3 &org, const QYTVector3 &dir, const QYTRay &parent,
                        QYTReal start, QYTReal end = INFINITY)
        : QYTRay(org, dir, start, end, parent.time, parent.depth+1) {
            hasDifferentials = false;
        }
        explicit QYTRayDifferential(const QYTRay &ray) : QYTRay(ray) {
            hasDifferentials = false;
        }
        bool hasNaNs() const {
            return QYTRay::hasNaNs() ||
            (hasDifferentials && (rxOrigin.hasNan() || ryOrigin.hasNan() ||
                                  rxDirection.hasNan() || ryDirection.hasNan()));
        }
        void ScaleDifferentials(float s) {
            rxOrigin = o + (rxOrigin - o) * s;
            ryOrigin = o + (ryOrigin - o) * s;
            rxDirection = d + (rxDirection - d) * s;
            ryDirection = d + (ryDirection - d) * s;
        }
        

        ///如果该值为true，则表示当前光线有两条附加的辅助光线，为false则表示没有
        bool hasDifferentials;
        
        ///附加光线的两个源点
        QYTPoint3 rxOrigin, ryOrigin;
        
        ///附加光线的两个方向
        QYTVector3 rxDirection, ryDirection;
    };
}

#endif /* defined(__qyt_fundation__qyt_ray__) */
