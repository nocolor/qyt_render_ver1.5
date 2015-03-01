//
//  QYTSpectrum_test.cpp
//  qyt_render_ver1.5
//
//  Created by nocolor on 15/2/26.
//  Copyright (c) 2015年 ___JinJiangTiaoZhanBei___. All rights reserved.
//

#include "QYTSpectrum_test.h"

using namespace QYT;

void QYTSpectrum_test()
{
    QYTRGBSpectrum spc(1.f), spc2(3.f);
    QYTRGBSpectrum res = spc * spc2;
    
    std::cout << .5f*res;
    
    
    

}