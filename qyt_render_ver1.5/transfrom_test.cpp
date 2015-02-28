//
//  transfrom_test.cpp
//  qyt_render_ver1.5
//
//  Created by nocolor on 15/2/18.
//  Copyright (c) 2015年 ___JinJiangTiaoZhanBei___. All rights reserved.
//

#include "transfrom_test.h"

void transform_test()
{
    using namespace std;
    QYTPoint3 p(0.f, 1.f, 0.f);
    cout << LCR_TO_PBRT << endl;
    cout << LCR_TO_PBRT(p) << std::endl;
    cout << "After some affine transfrom:\n";
    
    QYTTransform t = QYTRotateX(M_PI_2)*QYTTranslate(0.f, 3.13f, 0.f);
    
    cout << t(p) << endl;
    
    cout << "Transfrom p to PBRT:\n";
    cout << LCR_TO_PBRT(t(p)) << endl;
    
    cout << "Compute the transfrom matrix:\n";
    cout << (LCR_TO_PBRT*t)(p) << endl;
    
    if(LCR_TO_PBRT.swapsHandedness())
        QYTWarning("LCR_TO_PBRT need to swap handedness.");
    
    
}