//
//  QYTMaterial.h
//  qyt_render_ver1.5
//
//  Created by nocolor on 15/3/17.
//  Copyright (c) 2015年 ___JinJiangTiaoZhanBei___. All rights reserved.
//

#ifndef __qyt_render_ver1_5__QYTMaterial__
#define __qyt_render_ver1_5__QYTMaterial__

#include "qytr.h"
#include "QYTDifferentialGeometry.h"
#include "QYTReflection.h"
#include "QYTTexture.h"

namespace QYT
{
    class QYTMaterial
    {
    public:
        virtual QYTBSDF* getBSDF(const QYTDifferentialGeometry& dgGeom,
                                 const QYTDifferentialGeometry& dgShading) const = 0;
        virtual QYTBSSRDF* getBSSDF(const QYTDifferentialGeometry& dgGeom,
                                    const QYTDifferentialGeometry& dgShading) const
        {
            return nullptr;
        }
        
        virtual ~QYTMaterial();
        
        static void Bump(const QYTTexture<float>& d, const QYTDifferentialGeometry& dgGeom,
                         const QYTDifferentialGeometry& dgShading,
                         QYTDifferentialGeometry* dgBump);
    };
}

#endif /* defined(__qyt_render_ver1_5__QYTMaterial__) */
