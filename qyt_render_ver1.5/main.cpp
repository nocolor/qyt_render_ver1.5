//
//  main.cpp
//  qyt_render_ver1.5
//
//  Created by nocolor on 15/2/15.
//  Copyright (c) 2015年 ___JinJiangTiaoZhanBei___. All rights reserved.
//

#include <iostream>
#include "QYTTest.h"

using namespace std;
<<<<<<< HEAD
using namespace QYT;

//测试一下gitHub同步

//再次熟悉gitHub

int main(int argc, const char * argv[])
{
=======

int main(int argc, const char * argv[]) {
>>>>>>> parent of e9f0f1b... 对现行版本的组件进行了功能和可行性测试
    
    cout << "light core render Ver:" << QYTR_VERSION << endl;
    
//    QYTStratifiedSampler_test();
    QYTImageFilm_test();
    
<<<<<<< HEAD
//    QYTRender_ver_1_5_test();
    QYTCamera_test();

=======
>>>>>>> parent of e9f0f1b... 对现行版本的组件进行了功能和可行性测试
    return 0;
}
