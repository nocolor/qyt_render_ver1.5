//
//  QYTTest.h
//  qyt_render_ver1.5
//
//  Created by nocolor on 15/3/6.
//  Copyright (c) 2015年 ___JinJiangTiaoZhanBei___. All rights reserved.
//

#ifndef __qyt_render_ver1_5__QYTTest__
#define __qyt_render_ver1_5__QYTTest__

#include "QYTStratifiedSampler.h"
#include "QYTImageFilm.h"

void QYTStratifiedSampler_test();
void QYTImageFilm_test();

///测试图像管线、采样器、相机、线程池、基础图元
int QYTRender_ver_1_5_test();

int QYTLambertian_test();

///测试引用计数的类，特别是在多线程中
int QYTReference_test();

///同shared_ptr进行比较测试
int QYTReference_test_2();

#endif /* defined(__qyt_render_ver1_5__QYTTest__) */
