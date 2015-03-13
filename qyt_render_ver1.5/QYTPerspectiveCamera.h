//
//  QYTPerspectiveCamera.h
//  qyt_render_ver1.5
//
//  Created by nocolor on 15/3/13.
//  Copyright (c) 2015年 ___JinJiangTiaoZhanBei___. All rights reserved.
//

#ifndef __qyt_render_ver1_5__QYTPerspectiveCamera__
#define __qyt_render_ver1_5__QYTPerspectiveCamera__

#include "QYTCamera.h"

namespace QYT
{
    /**
     @class QYTPerspectiveCamera
     带有精典三维透视的摄影机，支持景深。
     */
    class QYTPerspectiveCamera:public QYTProjectiveCamera
    {
    private:
        QYTVec3 dxCamera, dyCamera;
        
    public:
        QYTPerspectiveCamera(const QYTAnimatedTransform& cam2World,
                             const QYTReal screenWindow[4],
                             QYTReal sopen, QYTReal sclose,
                             QYTReal lensr, QYTReal focald,
                             QYTReal fov, QYTFilm* iFilm);
        
        QYTReal generateRay(const QYTCameraSample& sample,
                            QYTRay* ray) const override;
        
        QYTReal generateRayDifferential(const QYTCameraSample& sample,
                                        QYTRayDifferential* rd) const override;
    };
}

#endif /* defined(__qyt_render_ver1_5__QYTPerspectiveCamera__) */
