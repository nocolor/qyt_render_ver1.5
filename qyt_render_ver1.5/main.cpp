//
//  main.cpp
//  qyt_render_ver1.5
//
//  Created by nocolor on 15/2/15.
//  Copyright (c) 2015年 ___JinJiangTiaoZhanBei___. All rights reserved.
//

#include <iostream>
#include "QYTBoxFilter.h"
#include "QYTSphere.h"
#include "QYTPerspectiveCamera.h"
#include "QYTAnimatedTransform.h"
#include "QYTRNG.h"
#include "QYTImageFilm.h"
#include "QYTStratifiedSampler.h"

#include "QYTTest.h"

using namespace std;
using namespace QYT;



int main(int argc, const char * argv[])
{
    
    cout << "light core render Ver:" << QYTR_VERSION << endl;
    
//    QYTImageFilm_test();
    
//    QYTRender_ver_1_5_test();
    QYTCamera_test();

    return 0;
}
