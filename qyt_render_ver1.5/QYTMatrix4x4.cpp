//
//  QYTMatrix4x4.cpp
//  qyt_render_ver1.5
//
//  Created by nocolor on 15/2/18.
//  Copyright (c) 2015年 ___JinJiangTiaoZhanBei___. All rights reserved.
//

#include "QYTMatrix4x4.h"

namespace QYT
{
    QYTMatrix4x4::QYTMatrix4x4()
    {
        m[0][0] = m[1][1] = m[2][2] = m[3][3] = 1.f;
        m[0][1] = m[0][2] = m[0][3] = m[1][0] =
        m[1][2] = m[1][3] = m[2][0] = m[2][1] = m[2][3] =
        m[3][0] = m[3][1] = m[3][2] = 0.f;
    }
    
    QYTMatrix4x4::QYTMatrix4x4(QYTReal mat[4][4])
    {
        memcpy(m, mat, 16*sizeof(QYTReal));
    }
    
    QYTMatrix4x4::QYTMatrix4x4(QYTReal t00, QYTReal t01, QYTReal t02, QYTReal t03,
                               QYTReal t10, QYTReal t11, QYTReal t12, QYTReal t13,
                               QYTReal t20, QYTReal t21, QYTReal t22, QYTReal t23,
                               QYTReal t30, QYTReal t31, QYTReal t32, QYTReal t33)
    {
        m[0][0] = t00; m[0][1] = t01; m[0][2] = t02; m[0][3] = t03;
        m[1][0] = t10; m[1][1] = t11; m[1][2] = t12; m[1][3] = t13;
        m[2][0] = t20; m[2][1] = t21; m[2][2] = t22; m[2][3] = t23;
        m[3][0] = t30; m[3][1] = t31; m[3][2] = t32; m[3][3] = t33;
    }
    
    QYTMatrix4x4::QYTMatrix4x4(const QYTMatrix4x4& rhs)
    {
        memcpy(m, rhs.m, 16*sizeof(QYTReal));
    }
    
    QYTMatrix4x4& QYTMatrix4x4::operator=(const QYT::QYTMatrix4x4 &rhs)
    {
        if(this != &rhs)
            memcpy(m, rhs.m, 16*sizeof(QYTReal));
        return *this;
    }
    
    QYTMatrix4x4 QYTMatrix4x4::operator*(const QYT::QYTMatrix4x4 &rhs) const
    {
        QYTMatrix4x4 r;
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                r.m[i][j] =
                m[i][0] * rhs.m[0][j] +
                m[i][1] * rhs.m[1][j] +
                m[i][2] * rhs.m[2][j] +
                m[i][3] * rhs.m[3][j];
        return r;
    }
    
    QYTMatrix4x4 QYTMatrix4x4::operator~() const
    {
        return QYTMatrix4x4(m[0][0], m[1][0], m[2][0], m[3][0],
                            m[0][1], m[1][1], m[2][1], m[3][1],
                            m[0][2], m[1][2], m[2][2], m[3][2],
                            m[0][3], m[1][3], m[2][3], m[3][3]);
    }
    
    QYTMatrix4x4 QYTMatrix4x4::operator!() const
    {
        int indxc[4], indxr[4];
        int ipiv[4] = { 0, 0, 0, 0 };
        float minv[4][4];
        memcpy(minv, m, 4*4*sizeof(float));
        
        for (int i = 0; i < 4; i++) {
            int irow = -1, icol = -1;
            float big = 0.;
            // Choose pivot
            for (int j = 0; j < 4; j++) {
                if (ipiv[j] != 1) {
                    for (int k = 0; k < 4; k++) {
                        if (ipiv[k] == 0) {
                            if (fabsf(minv[j][k]) >= big) {
                                big = float(fabsf(minv[j][k]));
                                irow = j;
                                icol = k;
                            }
                        }
                        else if (ipiv[k] > 1)
                            QYTError("Singular matrix in MatrixInvert");
                    }
                }
            }
            ++ipiv[icol];
            // Swap rows _irow_ and _icol_ for pivot
            if (irow != icol) {
                for (int k = 0; k < 4; ++k)
                    std::swap(minv[irow][k], minv[icol][k]);
            }
            indxr[i] = irow;
            indxc[i] = icol;
            if (minv[icol][icol] == 0.)
                QYTError("Singular matrix in MatrixInvert");
            
            // Set $m[icol][icol]$ to one by scaling row _icol_ appropriately
            float pivinv = 1.f / minv[icol][icol];
            minv[icol][icol] = 1.f;
            for (int j = 0; j < 4; j++)
                minv[icol][j] *= pivinv;
            
            // Subtract this row from others to zero out their columns
            for (int j = 0; j < 4; j++) {
                if (j != icol) {
                    float save = minv[j][icol];
                    minv[j][icol] = 0;
                    for (int k = 0; k < 4; k++)
                        minv[j][k] -= minv[icol][k]*save;
                }
            }
        }
        // Swap columns to reflect permutation
        for (int j = 3; j >= 0; j--) {
            if (indxr[j] != indxc[j]) {
                for (int k = 0; k < 4; k++)
                    std::swap(minv[k][indxr[j]], minv[k][indxc[j]]);
            }
        }
        
        return QYTMatrix4x4(minv[0][0], minv[0][1], minv[0][2], minv[0][3],
                            minv[1][0], minv[1][1], minv[1][2], minv[1][3],
                            minv[2][0], minv[2][1], minv[2][2], minv[2][3],
                            minv[3][0], minv[3][1], minv[3][2], minv[3][3]);
    }
    
    bool QYTMatrix4x4::operator==(const QYT::QYTMatrix4x4 &m2) const
    {
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                if (m[i][j] != m2.m[i][j]) return false;
        return true;
    }
    
    bool QYTMatrix4x4::operator!=(const QYT::QYTMatrix4x4 &m2) const
    {
        return !(*this == m2);
    }
    
    std::ostream& operator<<(std::ostream& os, const QYTMatrix4x4& m)
    {
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                os.precision(5);
                os.setf(std::ios_base::showpoint);
                std::cout << "|" << m.m[i][j] << ", ";
                if(j == 3)
                    std::cout << "|" << std::endl;
            }
        }
        return os;
    }
    
    bool QYTMatrix4x4::isIdentity() const
    {
        return (m[0][0] == 1.f && m[0][1] == 0.f &&
                m[0][2] == 0.f && m[0][3] == 0.f &&
                m[1][0] == 0.f && m[1][1] == 1.f &&
                m[1][2] == 0.f && m[1][3] == 0.f &&
                m[2][0] == 0.f && m[2][1] == 0.f &&
                m[2][2] == 1.f && m[2][3] == 0.f &&
                m[3][0] == 0.f && m[3][1] == 0.f &&
                m[3][2] == 0.f && m[3][3] == 1.f);
    }
    
}