//
//  QYTMatrix4x4.h
//  qyt_render_ver1.5
//
//  Created by nocolor on 15/2/18.
//  Copyright (c) 2015年 ___JinJiangTiaoZhanBei___. All rights reserved.
//

#ifndef __qyt_render_ver1_5__QYTMatrix4x4__
#define __qyt_render_ver1_5__QYTMatrix4x4__

#include "qytr.h"

namespace QYT
{
    ///4*4的矩阵，用于表示仿射变换。由于仿射变换中只有乘法计算，因此4*4矩阵也只支持乘法运算。
    ///如果需要较为完整的矩阵支持，可以使用qyt_matrix。
    class QYTMatrix4x4
    {
    public:
        QYTReal m[4][4];
        
        //构造函数
        
        ///默认构造函数构造单位4*4矩阵
        QYTMatrix4x4();
        QYTMatrix4x4(QYTReal mat[4][4]);
        QYTMatrix4x4(QYTReal t00, QYTReal t01, QYTReal t02, QYTReal t03,
                     QYTReal t10, QYTReal t11, QYTReal t12, QYTReal t13,
                     QYTReal t20, QYTReal t21, QYTReal t22, QYTReal t23,
                     QYTReal t30, QYTReal t31, QYTReal t32, QYTReal t33);
        ///复制构造
        QYTMatrix4x4(const QYTMatrix4x4& rhs);
        
        ///赋值
        QYTMatrix4x4& operator=(const QYTMatrix4x4& rhs);
        
        ///乘法运算
        QYTMatrix4x4 operator*(const QYTMatrix4x4& rhs) const;
        
        ///转置运算
        QYTMatrix4x4 operator~() const;
        
        ///求逆矩阵
        QYTMatrix4x4 operator!() const;
        
        ///判断两个矩阵是否相等，如果两个4*4矩阵中每行每列的数值都相等，则返回true，否则返回false。
        bool operator==(const QYTMatrix4x4 &m2) const;
        bool operator!=(const QYTMatrix4x4 &m2) const;
        
        friend std::ostream& operator<<(std::ostream& os, const QYTMatrix4x4& m);
        
        //其他公有接口
        
        ///如果当前矩阵是单位矩阵返回true，否则返回false
        bool isIdentity() const;
    };
    
    typedef QYTMatrix4x4 QYTMat4;
}

#endif /* defined(__qyt_render_ver1_5__QYTMatrix4x4__) */
