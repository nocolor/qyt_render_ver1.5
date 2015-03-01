//
//  main.cpp
//  qyt_render_ver1.5
//
//  Created by nocolor on 15/2/15.
//  Copyright (c) 2015年 ___JinJiangTiaoZhanBei___. All rights reserved.
//

#include <iostream>
#include "transfrom_test.h"
#include "QYTSphere_test.h"
#include "QYTThreadPool_test.h"
#include "QYTSpectrum_test.h"

using namespace std;

int main(int argc, const char * argv[]) {
    
    cout << "light core render Ver:" << QYTR_VERSION << endl;
    
//    transform_test();
//    QYTSphere_test();

//    QYTThreadPool_test();
//    QYTMultiJitteredSampler_test();
    QYTSpectrum_test();
    
    return 0;
}
