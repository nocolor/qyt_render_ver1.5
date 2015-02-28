//
//  QYTCamera.h
//  QYTRayTracer
//
//  Created by nocolor on 14-4-14.
//  Copyright (c) 2014年 QinYuTong. All rights reserved.
//

#ifndef __QYTRayTracer__QYTCamera__
#define __QYTRayTracer__QYTCamera__

#include "qytr.h"
#include "QYTVector3.h"
#include "QYTPoint3.h"

namespace QYT
{
    /**
     @class QYTCamera
     *  QYTCamera是相机类的基类，也是视见系统的基础。它代表一类相机，用于透视投影，
     *  可以由此派生出针孔相机、透镜相机或者鱼眼相机等等。
     QYTCamera并不指定任何观察算法，只是定义了初级观察接口，并且计算相机架标。
     在设置了新的视点或者方向之后，必须调用compute_uvw()方法重新计算架标让新观察
     生效。
     */
    class QYTCamera
    {
    protected:

        QYTCamera& operator = (const QYTCamera& rhs);
        QYTVec3 u, v, w;
        
    public:
        ///视点，即相机所在的位置
        QYTPoint3   eye;
        
        ///看点，即相机看向的位置
        QYTPoint3   lookAt;
        
        ///偏航角，及相机沿着观察方向顺时针旋转的角度（弧度）
        QYTReal     ra;
        
        ///相机的上方向
        QYTVec3     up;
        
        QYTCamera();
        QYTCamera(const QYTCamera& rhs);
        virtual QYTCamera* copy() const = 0;
        virtual ~QYTCamera();
        
        inline void setEye(const QYTPoint3& _eye)
        {
            eye = _eye;
        }
        
        inline  void setEye(QYTReal x, QYTReal y, QYTReal z)
        {
            eye = QYTPoint3(x, y, z);
        }
        
        inline void setLookAt(const QYTPoint3& _lookAt)
        {
            lookAt = _lookAt;
        }
        
        inline void setLookAt(QYTReal x, QYTReal y, QYTReal z)
        {
            lookAt = QYTPoint3(x, y, z);
        }
        
        inline void setUp(const QYTVec3& _up)
        {
            up = _up;
        }
        
        inline void setUp(QYTReal x, QYTReal y, QYTReal z)
        {
            up = QYTVec3(x, y, z);
        }
        
        inline void setRoll(QYTReal _ra)
        {
            ra = _ra;
        }
        
        ///根据相机的当前设置来计算相机架标，每次更新相机设置后，都要调用此方法令设置生效。
        void compute_uvw();
    };
}
#endif /* defined(__QYTRayTracer__QYTCamera__) */
