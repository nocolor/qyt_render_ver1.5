//
//  QYTTexture.h
//  qyt_render_ver1.5
//
//  Created by nocolor on 15/3/17.
//  Copyright (c) 2015年 ___JinJiangTiaoZhanBei___. All rights reserved.
//

#ifndef __qyt_render_ver1_5__QYTTexture__
#define __qyt_render_ver1_5__QYTTexture__

#include "qytr.h"
#include "QYTDifferentialGeometry.h"

namespace QYT
{
    class QYTTextureMapping2D
    {
    public:
        virtual ~QYTTextureMapping2D() { }
        virtual void map(const QYTDifferentialGeometry &dg,
                         float *s, float *t, float *dsdx, float *dtdx,
                         float *dsdy, float *dtdy) const = 0;
    };
    
    class QYTUVMapping2D:public QYTTextureMapping2D
    {
    private:
        float su, sv, du, dv;
        
    public:
        QYTUVMapping2D(float _su, float _sv, float _du, float _dv):
        su(_su), sv(_sv), du(_du), dv(_dv){}
        
        void map(const QYTDifferentialGeometry &dg,
                 float *s, float *t, float *dsdx, float *dtdx,
                 float *dsdy, float *dtdy) const override;
    };
    
    template <typename T>
    class QYTTexture
    {
    public:
        virtual T evaluate(const QYTDifferentialGeometry &) const = 0;
        virtual ~QYTTexture() { }
    };
}

#endif /* defined(__qyt_render_ver1_5__QYTTexture__) */
