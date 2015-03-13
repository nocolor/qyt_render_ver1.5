//
//  QYTTransform.h
//  qyt_render_ver1.5
//
//  Created by nocolor on 15/2/18.
//  Copyright (c) 2015年 ___JinJiangTiaoZhanBei___. All rights reserved.
//

#ifndef __qyt_render_ver1_5__QYTTransform__
#define __qyt_render_ver1_5__QYTTransform__

#include "qyt_bbox.h"
#include "qyt_ray.h"
#include "QYTPoint3.h"
#include "QYTMatrix4x4.h"

namespace QYT
{
    /**
     @class QYTTransform
     表示一组仿射变换，并且可以应用到基本几何对象。可以对点、向量、
     包围盒、光线应用仿射变换。
     @note
     在LCR中，世界坐标系是左手坐标系。
     如果您正对着您的电脑屏幕，那么X轴的方向是您右手的方向，Y轴方向是您
     上方的方向，Z轴方向垂直于您的屏幕并且指向您的屏幕外（即是指向您的眼睛）。
     点、向量、射线、包围盒、相机、视平面都是直接定义在世界坐标下的。
     在LCR中，术语LCR坐标系就是指世界坐标。
     
     一些局部参数化的几何体或者NUBRS曲面会定义在其局部坐标系中，LRC中定义
     局部对象的局部坐标系如下：
     如果您正对着您的电脑屏幕，X方向是您的右手的方向，Y方向垂直于您的屏幕向
     外（即指向您的眼睛），Z方向是您头上方的方向。显然，PBRT坐标系是左手坐标系。
     在LCR中，术语PBRT坐标系指的是上述的局部坐标系统（名称PBRT来自著名
     渲染技术书籍《基于物理的渲染：从理论到实践》）。
     
     除了上述两种坐标系，常见的坐标系还有相机坐标系、切平面空间坐标系等等。
     
     确保在应用变换时处于您期望的坐标系下。
     */
    class QYTTransform
    {
    protected:
        ///代表仿射变换的矩阵
        QYTMat4 m;
        
        ///m的逆矩阵，表示仿射变换的逆变换,
        ///直接储存逆矩阵的原因是计算逆矩阵的开销比较大，因此直接储存这个矩阵
        QYTMat4 mInv;
    public:
        //构造函数
        QYTTransform(){}
        QYTTransform(const QYTMat4& transfrom):m(transfrom), mInv(!transfrom){}
        QYTTransform(const QYTMat4& transfrom, const QYTMat4& transfromInv):m(transfrom), mInv(transfromInv)
        {
#ifndef NDEBUG
            if(!(m*mInv).isIdentity())
                QYTWarning("Matrixes may not be normal affine transfrom.");
#endif
        }
        //对象的复制和赋值可以直接依赖QYTMat4的实现，因此不需要特殊的复制构造函数以及赋值操作
        
        //比较判断--------------------------------------
        
        ///判断两个仿射变换是否相等，比较变换矩阵和内矩阵
        ///@return 如果相等，返回true，否则返回false
        bool operator==(const QYTTransform& rhs) const;
        bool operator!=(const QYTTransform& rhs) const;
        
        //基本运算--------------------------------------
        
        ///组合仿射变换，本质上是变换矩阵相乘，可以表示仿射变换的线性组合
        QYTTransform operator*(const QYTTransform& t) const;
        
        QYTTransform& operator*=(const QYTTransform& t);
        
        ///对变换求逆，即逆变换
        QYTTransform operator!() const;
        
        ///求变换的转置
        QYTTransform operator~() const;
        
        //其他操作--------------------------------------
        friend std::ostream& operator<<(std::ostream& os, const QYTTransform& t)
        {
            os << " -----------------------------------\n";
            os << "|m:                                 |\n";
            os << t.m;
            os << " -----------------------------------\n";
            os << "|mInv:                              |\n";
            os << t.mInv;
            os << " -----------------------------------";
            return os;
        }
        
        //公有接口--------------------------------------
        
        ///返回变换矩阵
        QYTMat4 getTransfromMatrix() const
        {return m;}
        
        ///返回逆变换矩阵
        QYTMat4 getInvTransfromMatrix() const
        {return mInv;}
        
        ///设置新的变换矩阵
        ///@attention 如果有可能，尽量使用operator*运算符构造变换，不要直接设置变换矩阵
        void setMatrix(const QYTMat4& t, const QYTMat4& tInv)
        {
            m = t;
            mInv = tInv;
#ifndef NDEBUG
            if(!(m*mInv).isIdentity())
                QYTWarning("Matrixes may not be normal affine transfrom.");
#endif
        }
        
        ///对向量执行变换，返回变换后的向量
        ///@attention   对向量进行平移变换是没有意义的，在数学上，向量只有方向，平移之后不会有任何改变
        QYTVector3 operator()(const QYTVector3& v) const;
        
        ///对点执行变换，返回变换后的点
        QYTPoint3 operator()(const QYTPoint3& p) const;
        
        ///对射线执行变换，返回变换后的射线
        QYTRay operator()(const QYTRay& ray) const;
        
        ///对微分射线执行变换，返回变换后的微分射线
        QYTRayDifferential operator()(const QYTRayDifferential& ray) const;
        
        ///对包围盒进行变换，返回变换后的包围盒
        QYTBBox operator()(const QYTBBox& b) const;
        
        ///如果当前的变换矩阵是单位矩阵，返回true，否则返回false
        bool isIdentity() const
        {
            return m.isIdentity();
        }
        
        /**
         判断当前变换中是否包含非均匀缩放。
         非均匀缩放指沿各个轴方向缩放不同的比例，当物体被非均匀缩放后，其表面法向量需要被重新计算。
         */
        bool hasScale() const;
        
        /**
         判断变换是否改变了坐标系的左右手法则。
         一个变换除了可以表示平移、缩放之类的基本变换，还可以表示架标变换。
         例如一个定义在LCR中的点需要被变换到PBRT坐标系时，就需要用到LCR_TO_PBRT变换对象。
         当对这个点进行变换时，意味着变换了坐标系的左右手法则（从右手坐标系
         转换到了左手坐标系）。
         @attention 改变了左右手法则时，会带来一系列重要的变化，例如需要转变法线向量的方向等等。当您不确定某个变换是否会改变左右手法则时，您应当在执行变换时进行检查。
         @return 如果当前变换改变了左右手法则，返回true，否则返回false
         */
        bool swapsHandedness() const;
        
        /**
         产生一个从世界坐标变换到局部坐标QYTTransform对象。
         在声明几何对象时，它是被定义在其局部坐标系中的，但是
         由于往往需要在世界坐标中对几何对象进行仿射变换，因此
         需要小心地构建变换矩阵。
         当在构建某个几何体的仿射变换（及其线性组合的时候），
         并不是直接将这个变换应用到几何体对象上，而是在对其进行
         求交计算时，将变换应用到光线上，即是说，是将光线变换到
         局部坐标下进行求交，而不是把几何体变换到世界坐标。
         @note 定义在世界坐标中的放射变换与真实应用到局部坐标下
         对光线的变换时相反的，因此除非您确信您能分辨这些微妙的差距，
         您应当使用LCR的接口产生变换，而不是直接指定。
         @param T 在世界坐标中的仿射变换组合
         @return T在局部坐标中的表示
         */
        static QYTTransform MakeLCRTransfrom(const QYTTransform& T);
    };
    
    ///LRC_TO_PBRT表示从世界坐标（LRC坐标系）变换到局部对象的局部坐标（PBRT坐标）
    const QYTTransform LCR_TO_PBRT(QYTMat4 (1.f, 0.f, 0.f, 0.f,
                                            0.f, 0.f, 1.f, 0.f,
                                            0.f, 1.f, 0.f, 0.f,
                                            0.f, 0.f, 0.f, 1.f));
    
    ///使用智能指针保存
    const std::shared_ptr<QYTTransform> LCR_TO_PBRT_S_PTR = std::make_shared<QYTTransform>(LCR_TO_PBRT);
    
    /**
     @brief 平移变换
     @return 代表沿着向量[x, y, z]平移的变换
     */
    inline QYTTransform QYTTranslate(QYTReal x, QYTReal y, QYTReal z)
    {
        
        QYTMat4 m(1, 0, 0, x,
                  0, 1, 0, y,
                  0, 0, 1, z,
                  0, 0, 0, 1);
        
        QYTMat4 mInv(1, 0, 0, -x,
                     0, 1, 0, -y,
                     0, 0, 1, -z,
                     0, 0, 0, 1);
        
        return QYTTransform(m, mInv);
        
    }
    
    /**
     @brief 缩放变换
     @return 代表在X轴、Y轴、Z轴方向分别缩放x，y，z个单位的变换
     */
    inline QYTTransform QYTScale(QYTReal x, QYTReal y, QYTReal z)
    {
        QYTMat4 m(x, 0, 0, 0,
                  0, y, 0, 0,
                  0, 0, z, 0,
                  0, 0, 0, 1);
        
        QYTMat4 mInv(1.f/x, 0, 0, 0,
                     0, 1.f/y, 0, 0,
                     0, 0, 1.f/z, 0,
                     0, 0, 0, 1);
        
        return QYTTransform(m, mInv);
    }
    
    /**
     沿着X轴顺时针方向旋转theta角度（以弧度为单位）
     */
    inline QYTTransform QYTRotateX(QYTReal theta)
    {
        QYTMat4 m(1,0,0,0,
                  0,cos(theta),-sin(theta),0,
                  0,sin(theta),cos(theta),0,
                  0,0,0,1);
        
        return QYTTransform(m, ~m);
    }
    
    /**
     沿着Y轴顺时针方向旋转theta角度（以弧度为单位）
     */
    inline QYTTransform QYTRotateY(QYTReal theta)
    {
        QYTMat4 m(cos(theta),0,sin(theta),0,
                  0,1,0,0,
                  -sin(theta),0,cos(theta),0,
                  0,0,0,1);
        
        return QYTTransform(m, ~m);
    }
    
    /**
     沿着Z轴顺时针方向旋转theta角度（以弧度为单位）
     */
    inline QYTTransform QYTRotateZ(QYTReal theta)
    {
        QYTMat4 m(cos(theta),-sin(theta),0,0,
                  sin(theta),cos(theta),0,0,
                  0,0,1,0,
                  0,0,0,1);
        
        return QYTTransform(m, ~m);
    }
    
    /**
     沿着轴[x, y, z]顺时针方向旋转theta角度（以弧度为单位）
     */
    inline QYTTransform QYTRotate(QYTReal theta, QYTReal x, QYTReal y, QYTReal z)
    {
        QYTVec3 v(x, y, z);
        if(v[0] == 1.0f && v[1] == 0.0f && v.z == 0.0f)
            return QYTRotateX(theta);
        if(v[0] == 0.0f && v[1] == 1.0f && v.z == 0.0f)
            return QYTRotateY(theta);
        if(v[0] == 0.0f && v[1] == 0.0f && v.z == 1.0f)
            return QYTRotateZ(theta);
        
        QYTReal total = sqrt(v[0]*v[0]+v[1]*v[1]+v[2]*v[2]);
        v[0] = v[0]/total;
        v[1] = v[1]/total;
        v[2] = v[2]/total;
        QYTReal a1 = 2*cos(theta/2)*sin(theta/2);
        QYTReal a3 = 2 * sin(theta/2) * sin(theta/2);
        QYTReal a4 = v[0]*v[0]+v[1]*v[1];
        QYTReal a5 = v[1]*v[1]+v[2]*v[2];
        QYTReal a6 = v[0]*v[0]+v[2]*v[2];
        QYTReal a7 = v[0]*v[1];
        QYTReal a8 = v[1]*v[2];
        QYTReal a9 = v[0]*v[2];
        
        QYTMat4 RotateXYZ(1-a3*a5,a3*a7-a1*v[2],a3*a9+a1*v[1],0,
                          a3*a7+a1*v[2],1-a3*a6,a3*a8-a1*v[0],0,
                          a3*a9-a1*v[1],a3*a8+a1*v[0],1-a3*a4,0,
                          0,0,0,1);
        
        return QYTTransform(RotateXYZ, ~RotateXYZ);
    }
    
    inline QYTTransform QYTPerspective(float fov, float n, float f)
    {
        QYTMat4 persp = QYTMat4(1, 0,           0,              0,
                                0, 1,           0,              0,
                                0, 0, f / (f - n), -f*n / (f - n),
                                0, 0,           1,              0);
        
        float invTanAng = 1.f / tanf(QYTRadians(fov) / 2.f);
        return QYTScale(invTanAng, invTanAng, 1) * QYTTransform(persp);
    }


}

#endif /* defined(__qyt_render_ver1_5__QYTTransform__) */
