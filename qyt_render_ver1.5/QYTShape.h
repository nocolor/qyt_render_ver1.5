//
//  QYTShape.h
//  lightCoreRender
//
//  Created by nocolor on 14/11/26.
//  Copyright (c) 2014年 ___NOCOLOR___. All rights reserved.
//

#ifndef __lightCoreRender__QYTShape__
#define __lightCoreRender__QYTShape__

#include <iostream>
#include "QYTVector3.h"
#include "QYTTransform.h"
#include "qyt_bbox.h"
#include "QYTDifferentialGeometry.h"

namespace QYT
{
    class QYTDifferentialGeometry;
    
    /**
     @class QYTShape
     几何形体对象的基类，一般情况下，QYTShape的子类定义在PBRT坐标系下。
     QYTShape及其子类是一类可以和光线判断求交的对象，例如球体、圆环、圆锥
     这样的基本几何体，也可以是NUBRS曲面和三角形网格这类对象。
     */
    class QYTShape
    {
    public:
        typedef std::shared_ptr<QYTTransform> Transfrom_sptr;
        
        ///从世界坐标到局部坐标的变换
        const Transfrom_sptr worldToObject;
        
        const bool reverseOrientation;
        
        ///worldToObject中的变换是否会改变坐标系的左右手法则
        const bool transformSwapsHandedness;
        
        QYTShape(Transfrom_sptr w2o,
                 bool ro):worldToObject(w2o), reverseOrientation(ro), transformSwapsHandedness(worldToObject->swapsHandedness()){}
        
        virtual ~QYTShape(){}
        
        ///返回几何体局部坐标下的包围盒。
        ///@attention 显然这个包围盒定义在物体的局部坐标系中。
        virtual QYTBBox objectBound() const = 0;
        
        ///返回几何体在世界坐标中的包围盒。
        ///@attention 这个包围盒定义在世界坐标系下，但是计算紧密的包围盒有时候并非易事。
        virtual QYTBBox worldBound() const;
        
        /**
         将几何体与光线求交，并且计算交点处的坐标、法线向量、纹理坐标等等信息。
         @param ray         需要与当前物体求交的光线
         @param tHit        交点处的t值，这个值是光线方程中的t参数
         @param rayEpsilon  用于避免光线-物体自相交而计算的最小偏差值
         @param dg          用于储存交点坐标、法向量、纹理坐标、uv各个方向的偏导数的对象
         @return    如果ray与物体相交，返回true，否则返回false
         */
        virtual bool intersect(const QYTRay& ray,
                               QYTReal* tHit,
                               QYTReal* rayEpsilon,
                               QYTDifferentialGeometry* dg) const = 0;
        
        ///只判断ray与物体是否相交，不计算其他额外的信息。
        ///@return 如果ray与物体相交，返回true，否则返回false
        virtual bool intersectP(const QYTRay& ray) const = 0;
        
        ///返回当前几何体的面积，
        ///@note 几何体的表面积对于在几何体表面采样时有重要作用。它影响概率密度分布函数(pdf)。
        virtual QYTReal area() const;
        
        /**
         将采样点(u1, u2)映射到几何体表面，并且计算那里的法线。
         @param u1, u2 位于采样空间内的二维采样点（通常是0~1的点）
         @param Ns 储存映射之后采样点处法线向量的指针
         @return 将(u1, u2)映射到物体表面之后的三位坐标，这个点定义在世界坐标系中
         */
        virtual QYTPoint3 sample(QYTReal u1, QYTReal u2, QYTNormal3 *Ns) const;
        
        ///概率密度分布函数
        virtual QYTPoint3 pdf(const QYTPoint3 &Pshape) const;
        
        virtual QYTPoint3 sample(const QYTPoint3 &P, QYTReal u1, QYTReal u2,
                                 QYTNormal3 *Ns) const;
        
        virtual QYTReal pdf(const QYTPoint3 &p, const QYTVector3 &wi) const;
        
        
    };
    
    inline QYTVec3 QYTFaceforward(const QYTVec3 &v, const QYTVec3 &v2) {
        return (QYTVec3::Dot(v, v2) < 0.f) ? -v : v;
    }

}

#endif /* defined(__lightCoreRender__QYTShape__) */
