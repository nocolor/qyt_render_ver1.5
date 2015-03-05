//
//  QYTImageFilm.cpp
//  qyt_render_ver1.5
//
//  Created by nocolor on 15/3/2.
//  Copyright (c) 2015年 ___JinJiangTiaoZhanBei___. All rights reserved.
//

#include "QYTImageFilm.h"

namespace QYT
{
    QYTImageFilm::QYTImageFilm(int xres, int yres, QYTFilter *filt, const float crop[4],
                               const std::string &fn):QYTFilm(xres, yres)
    {
        filter = filt;
        memcpy(cropWindow, crop, 4*sizeof(QYTReal));
        filename = fn;
        xPixelStart = QYTCeil2Int(xResolution*cropWindow[0]);
        xPixelCount = std::max(1, QYTCeil2Int(xResolution * cropWindow[1]) - xPixelStart);
        yPixelStart = QYTCeil2Int(yResolution * cropWindow[2]);
        yPixelCount = std::max(1, QYTCeil2Int(yResolution * cropWindow[3]) - yPixelStart);
    }
}
