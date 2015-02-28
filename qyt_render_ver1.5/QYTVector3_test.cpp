//
//  QYTVector3_test.cpp
//  qyt_render_ver1.5
//
//  Created by nocolor on 15/2/18.
//  Copyright (c) 2015年 ___JinJiangTiaoZhanBei___. All rights reserved.
//

#include "QYTVector3_test.h"

void QYTVector3_sample_test()
{
    Vec3 v1(1.f, 2.f, 3.f);
    Vec3 v2(2.f, 4.f, 6.f);
    std::cout << "----------------------------------\n";
    std::cout << v1+v2 << std::endl;
    std::cout << v1-v2 << std::endl;
    std::cout << v1*v2 << std::endl;
    std::cout << v1/v2 << std::endl;
    std::cout << (v1==v2) << std::endl;
    std::cout << (v1!=v2) << std::endl;
    std::cout << v1*4.f << std::endl;
    std::cout << 3.14f*v2 << std::endl;
    std::cout << IsEqualDirection(v1, v2) << std::endl;
    std::cout << Cross(v1, v2) << std::endl;
    std::cout << Cos(v1, v2) << std::endl;
    std::cout << Normalize(v1) << std::endl;
    
    
}