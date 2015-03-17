//
//  QYTUVTexture.h
//  qyt_render_ver1.5
//
//  Created by nocolor on 15/3/17.
//  Copyright (c) 2015年 ___JinJiangTiaoZhanBei___. All rights reserved.
//

#ifndef __qyt_render_ver1_5__QYTUVTexture__
#define __qyt_render_ver1_5__QYTUVTexture__

#include "QYTTexture.h"
#include "QYTSpectrum.h"

namespace QYT
{
    class QYTUVTexture:public QYTTexture<QYTSpectrum>
    {
    private:
        QYTTextureMapping2D* mapping;
    public:
        QYTUVTexture(QYTTextureMapping2D* map)
        {
            mapping = map;
        }
        
        ~QYTUVTexture()
        {
            delete mapping;
        }
        
        QYTSpectrum evaluate(const QYTDifferentialGeometry & dg) const override
        {
            float s, t, dsdx, dtdx, dsdy, dtdy;
            mapping->map(dg, &s, &t, &dsdx, &dtdx, &dsdy, &dtdy);
            float rgb[3] = { s - QYTFloor2Int(s), t - QYTFloor2Int(t), 0.f };
            return QYTSpectrum::FromRGB(rgb);
        }
    };
}

#endif /* defined(__qyt_render_ver1_5__QYTUVTexture__) */
