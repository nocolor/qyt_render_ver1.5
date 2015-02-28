//
//  QYTVector3.cpp
//  qyt_render_ver1.5
//
//  Created by nocolor on 15/2/18.
//  Copyright (c) 2015年 ___JinJiangTiaoZhanBei___. All rights reserved.
//

#include "QYTVector3.h"
#include "QYTPoint3.h"
#include <math.h>

namespace QYT
{
    bool QYTVector3::hasNan() const
    {
         return isnan(x) || isnan(y) || isnan(z);
    }
 
    QYTVector3::QYTVector3(const QYTPoint3& _p)
    {
        x = _p.x;
        y = _p.y;
        z = _p.z;
    }
    
    QYTVector3& QYTVector3::operator=(const QYTVector3& _rhs)
    {
        if(this != &_rhs)
        {
            x = _rhs.x;
            y = _rhs.y;
            z = _rhs.z;
        }
        Assert(!hasNan());
        return *this;
    }
    

    
    QYTVector3 QYTVector3::operator+(const QYTVector3& _rhs) const
    {
        Assert(!_rhs.hasNan());
        return QYTVector3(x+_rhs.x,
                          y+_rhs.y,
                          z+_rhs.z);
    }
    
    QYTVector3 QYTVector3::operator+(QYTReal _rhs) const
    {
        Assert(!hasNan());
        return QYTVector3(x + _rhs,
                          y + _rhs,
                          z + _rhs);
    }
    
    QYTVector3 operator+(QYTReal _lhs, const QYTVector3& _rhs)
    {
        Assert(!isnan(_lhs));
        return _rhs+_lhs;
    }
    
    QYTVector3 QYTVector3::operator-(const QYTVector3& _rhs) const
    {
        Assert(!_rhs.hasNan());
        return QYTVector3(x - _rhs.x,
                          y - _rhs.y,
                          z - _rhs.z);
    }
    
    QYTVector3 QYTVector3::operator-() const
    {
        return QYTVector3(-x, -y, -z);
    }
    
    QYTVector3 QYTVector3::operator-(QYTReal _rhs) const
    {
        return QYTVector3(x-_rhs, y-_rhs, z-_rhs);
    }
    
    QYTVector3 QYTVector3::operator*(QYTReal _rhs) const
    {
        return QYTVector3(x*_rhs, y*_rhs, z*_rhs);
    }
    
    QYTVector3 QYTVector3::operator*(const QYT::QYTVector3 &_rhs) const
    {
        Assert(!_rhs.hasNan());
        return QYTVector3(x*_rhs.x, y*_rhs.y, z*_rhs.z);
    }
    
    QYTVector3 operator*(QYTReal _lhs, const QYTVector3& _rhs)
    {
        Assert(!isnan(_lhs));
        return _rhs*_lhs;
    }
    
    QYTVector3 QYTVector3::operator/(const QYT::QYTVector3 &_rhs) const
    {
        Assert(!_rhs.hasNan());
#ifndef NDEBUG
        if (_rhs.x == 0 || _rhs.y == 0 || _rhs.z == 0) {
            QYTWarning("%s(inf) fund at file:%s, line: %d.\n",
                       __func__,
                       __FILE__,
                       __LINE__);
        }
#endif
        return QYTVector3(x/_rhs.x, y/_rhs.y, z/_rhs.z);
    }
    
    QYTVector3 QYTVector3::operator/(QYTReal _rhs) const
    {
#ifndef NDEBUG
        if (_rhs == 0) {
            QYTWarning("%s(inf) fund at file:%s, line: %d.\n",
                       __func__,
                       __FILE__,
                       __LINE__);
        }
#endif
        return QYTVector3(x/_rhs, y/_rhs, z/_rhs);
    }
    
    bool QYTVector3::operator==(const QYT::QYTVector3 &_rhs) const
    {
        Assert(!_rhs.hasNan());
        return (x==_rhs.x && y == _rhs.y && z==_rhs.z);
    }
    
    bool QYTVector3::operator!=(const QYT::QYTVector3 &_rhs) const
    {
        return !(this->operator==(_rhs));
    }
    
    QYTReal QYTVector3::operator[](int _i) const
    {
        Assert(_i >= 0 && _i <= 2);
        return (&x)[_i];
    }
    
    QYTReal& QYTVector3::operator[](int _i)
    {
        Assert(_i >= 0 && _i <= 2);
        return (&x)[_i];
    }
    
    std::ostream& operator<<(std::ostream& os, const QYTVector3& _rhs)
    {
        os << "[";
        os.precision(5);
        os.setf(std::ios_base::showpoint);
        os << _rhs.x << ", " << _rhs.y << ", " << _rhs.z;
        os << "]";
        return os;
    }
    

    
    bool QYTVector3::IsEqualDirection(const QYTVector3& _v1, const QYTVector3& _v2)
    {
        return Normalize(_v1) == Normalize(_v2);
    }
    
    QYTVector3 QYTVector3::Normalize(const QYT::QYTVector3 &_v)
    {
        Assert(!_v.hasNan());
        return _v/Length(_v);
    }
    
    QYTReal QYTVector3::Length(const QYT::QYTVector3 &_v)
    {
        return sqrtf(LengthSquared(_v));
    }
    
    QYTReal QYTVector3::LengthSquared(const QYT::QYTVector3 &_v)
    {
        Assert(!_v.hasNan());
        return _v.x*_v.x + _v.y*_v.y + _v.z*_v.z;
    }
    
    QYTReal QYTVector3::Dot(const QYT::QYTVector3 &_v1, const QYT::QYTVector3 &_v2)
    {
        Assert(!_v1.hasNan());
        Assert(!_v2.hasNan());
        return _v1.x*_v2.x + _v1.y*_v2.y + _v1.z*_v2.z;
    }
    
    QYTReal QYTVector3::AbsDot(const QYTVector3& _v1, const QYTVector3& _v2)
    {
        return fabs(Dot(_v1, _v2));
    }
    
    QYTReal QYTVector3::Cos(const QYT::QYTVector3 &_v1, const QYT::QYTVector3 &_v2)
    {
        return Dot(Normalize(_v1), Normalize(_v2));
    }
    
    QYTVector3 QYTVector3::Cross(const QYT::QYTVector3 &_v1, const QYT::QYTVector3 &_v2)
    {
        QYTReal v1x = _v1.x, v1y = _v1.y, v1z = _v1.z;
        QYTReal v2x = _v2.x, v2y = _v2.y, v2z = _v2.z;
        return QYTVec3((v1y * v2z) - (v1z * v2y),
                       (v1z * v2x) - (v1x * v2z),
                       (v1x * v2y) - (v1y * v2x));
    }
    
    void QYTVector3::CoordinateSystem(const QYT::QYTVector3 &v1, QYT::QYTVector3 *v2, QYT::QYTVector3 *v3)
    {
        if (fabsf(v1.x) > fabsf(v1.y)) {
            QYTReal invLen = 1.f / sqrtf(v1.x*v1.x + v1.z*v1.z);
            *v2 = QYTVector3(-v1.z * invLen, 0.f, v1.x * invLen);
        }
        else {
            QYTReal invLen = 1.f / sqrtf(v1.y*v1.y + v1.z*v1.z);
            *v2 = QYTVector3(0.f, v1.z * invLen, -v1.y * invLen);
        }
        *v3 = Cross(v1, *v2);
    }
}