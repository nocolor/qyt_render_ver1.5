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
    QYTCoefficientSpectrum<nSpectralSamples> spc;
    QYTCoefficientSpectrum<nSpectralSamples> spc2(2.0);
    
    
    spc = Sqrt(spc2);
    
    std::cout << spc;
    
    

}