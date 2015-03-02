//
//  QYTQuaternion.cpp
//  qyt_render_ver1.5
//
//  Created by nocolor on 15/3/2.
//  Copyright (c) 2015年 ___JinJiangTiaoZhanBei___. All rights reserved.
//

#include "QYTQuaternion.h"

namespace QYT
{
    QYTTransform QYTQuaternion::toTransform() const
    {
        QYTReal xx = v.x * v.x, yy = v.y * v.y, zz = v.z * v.z;
        QYTReal xy = v.x * v.y, xz = v.x * v.z, yz = v.y * v.z;
        QYTReal wx = v.x * w,   wy = v.y * w,   wz = v.z * w;
        
        QYTMat4 m;
        m.m[0][0] = 1.f - 2.f * (yy + zz);
        m.m[0][1] =       2.f * (xy + wz);
        m.m[0][2] =       2.f * (xz - wy);
        m.m[1][0] =       2.f * (xy - wz);
        m.m[1][1] = 1.f - 2.f * (xx + zz);
        m.m[1][2] =       2.f * (yz + wx);
        m.m[2][0] =       2.f * (xz + wy);
        m.m[2][1] =       2.f * (yz - wx);
        m.m[2][2] = 1.f - 2.f * (xx + yy);
        
        return QYTTransform(~m, m);
    }
    
    QYTQuaternion::QYTQuaternion(const QYTTransform& t)
    {
        const QYTMat4 &m = t.getTransfromMatrix();
        QYTReal trace = m.m[0][0] + m.m[1][1] + m.m[2][2];
        if (trace > 0.f) {
            // Compute w from matrix trace, then xyz
            // 4w^2 = m[0][0] + m[1][1] + m[2][2] + m[3][3] (but m[3][3] == 1)
            QYTReal s = sqrtf(trace + 1.0);
            w = s / 2.0f;
            s = 0.5f / s;
            v.x = (m.m[2][1] - m.m[1][2]) * s;
            v.y = (m.m[0][2] - m.m[2][0]) * s;
            v.z = (m.m[1][0] - m.m[0][1]) * s;
        }
        else {
            // Compute largest of $x$, $y$, or $z$, then remaining components
            const int nxt[3] = {1, 2, 0};
            QYTReal q[3];
            int i = 0;
            if (m.m[1][1] > m.m[0][0]) i = 1;
            if (m.m[2][2] > m.m[i][i]) i = 2;
            int j = nxt[i];
            int k = nxt[j];
            QYTReal s = sqrtf((m.m[i][i] - (m.m[j][j] + m.m[k][k])) + 1.0);
            q[i] = s * 0.5f;
            if (s != 0.f) s = 0.5f / s;
            w = (m.m[k][j] - m.m[j][k]) * s;
            q[j] = (m.m[j][i] + m.m[i][j]) * s;
            q[k] = (m.m[k][i] + m.m[i][k]) * s;
            v.x = q[0];
            v.y = q[1];
            v.z = q[2];
        }
    }
    
    QYTQuaternion QYTQuaternion::slrep(QYTReal t, const QYT::QYTQuaternion &q1, const QYT::QYTQuaternion &q2)
    {
        QYTReal cosTheta = QYTQuaternion::Dot(q1, q2);
        if (cosTheta > .9995f)
            return QYTQuaternion::Normalize((1.f - t) * q1 + t * q2);
        else {
            QYTReal theta = acosf(QYTClamp(cosTheta, -1.f, 1.f));
            QYTReal thetap = theta * t;
            QYTQuaternion qperp = Normalize(q2 - q1 * cosTheta);
            return q1 * cosf(thetap) + qperp * sinf(thetap);
        }
    }
}