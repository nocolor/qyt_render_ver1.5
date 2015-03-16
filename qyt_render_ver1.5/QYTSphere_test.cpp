//
//  QYTSphere_test.cpp
//  qyt_render_ver1.5
//
//  Created by nocolor on 15/2/19.
//  Copyright (c) 2015年 ___JinJiangTiaoZhanBei___. All rights reserved.
//

#include "QYTSphere_test.h"


void QYTSphere_test()
{
    
    QYTTransform CTM = QYTTransform::MakeLCRTransfrom(QYTTranslate(0.f, 0.f, 0.f));
    
    std::shared_ptr<QYTTransform> w2o = std::make_shared<QYTTransform>(CTM);
    
    QYTSphere sphere(w2o,               //转换到局部坐标的变换
                     false,             //不翻转法线
                     1.f,               //半径
                     -1.f,              //纵向起点
                     1.f,               //纵向终点
                     360.f);            //旋转角（注意使用角度不是弧度）
    
    QYTRay r(QYTPoint3(0.f, 0.f, -100.f),   //光线的源点
             QYTVec3(0.f, 0.f, 1.f),        //光线方向
             0.f);                          //起始t
    
    QYTDifferentialGeometry dg; //储存交点处的详细信息
    QYTReal rayEpsilon;           //避免自相交
    QYTReal tHit;               //交点的t值
    
    //判断求交
    bool isIntersect = sphere.intersect(r,
                                        &tHit,
                                        &rayEpsilon,
                                        &dg);
    
    if(isIntersect)
    {
        std::cout << "Intersect point:" << dg.p << std::endl;
        std::cout << "t:" << tHit << std::endl;
        std::cout << "rayEpsilon:" << rayEpsilon << std::endl;
        std::cout << "Normal:" << dg.n << std::endl;
        
    }
    
}