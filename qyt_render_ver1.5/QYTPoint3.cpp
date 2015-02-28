//
//  QYTPoint3.cpp
//  qyt_render_ver1.5
//
//  Created by nocolor on 15/2/18.
//  Copyright (c) 2015年 ___JinJiangTiaoZhanBei___. All rights reserved.
//

#include "QYTPoint3.h"
#include "QYTVector3.h"

namespace QYT
{
    bool QYTPoint3::hasNan() const
    {
        return isnan(x) || isnan(y) || isnan(z);
    }
    
    QYTPoint3::QYTPoint3(const QYTVector3& _v)
    {
        x = _v.x;
        y = _v.y;
        z = _v.z;
    }
    
    QYTPoint3& QYTPoint3::operator=(const QYT::QYTPoint3 &_rhs)
    {
        Assert(!_rhs.hasNan());
        if(this != &_rhs)
        {
            x = _rhs.x;
            y = _rhs.y;
            z = _rhs.z;
        }
        return *this;
    }
    
    QYTPoint3 QYTPoint3::operator+(const QYT::QYTVector3 &_rhs) const
    {
        Assert(!_rhs.hasNan());
        return QYTPoint3(x+_rhs.x,
                         y+_rhs.y,
                         z+_rhs.z);
    }
    
    QYTPoint3 QYTPoint3::operator-() const
    {
        return QYTPoint3(-x, -y, -z);
    }
    
    QYTPoint3 QYTPoint3::operator-(const QYT::QYTVector3 &_rhs) const
    {
        Assert(!_rhs.hasNan());
        return QYTPoint3(x-_rhs.x,
                         y-_rhs.y,
                         z-_rhs.z);
    }
    
    QYTVector3 QYTPoint3::operator-(const QYT::QYTPoint3 &_rhs) const
    {
        Assert(!_rhs.hasNan());
        return QYTVector3(x-_rhs.x,
                          y-_rhs.y,
                          z-_rhs.z);
    }
    
    QYTPoint3 QYTPoint3::operator*(QYTReal _rhs) const
    {
        return QYTPoint3(x*_rhs,
                         y*_rhs,
                         z*_rhs);
    }
    
    QYTPoint3 operator*(QYTReal _lhs, const QYTPoint3& _rhs)
    {
        return _rhs*_lhs;
    }
    
    QYTPoint3 QYTPoint3::operator/(QYTReal _rhs) const
    {
#ifndef NDEBUG
        if (_rhs == 0) {
            QYTWarning("%s(inf) fund at file:%s, line: %d.\n",
                       __func__,
                       __FILE__,
                       __LINE__);
        }
#endif
        return QYTPoint3(x/_rhs,
                         y/_rhs,
                         z/_rhs);
    }
    
    bool QYTPoint3::operator==(const QYT::QYTPoint3 &_rhs) const
    {
        Assert(!_rhs.hasNan());
        return (x==_rhs.x) &&(y == _rhs.y) && (z == _rhs.z);
    }
    
    bool QYTPoint3::operator!=(const QYT::QYTPoint3 &_rhs) const
    {
        return !(*this==_rhs);
    }
    
    QYTReal QYTPoint3::operator[](int _i) const
    {
        Assert(_i >= 0 && _i <= 2);
        return (&x)[_i];
    }
    
    QYTReal& QYTPoint3::operator[](int _i)
    {
        Assert(_i >= 0 && _i <= 2);
        return (&x)[_i];
    }
    
    std::ostream& operator << (std::ostream& os, const QYTPoint3& _rhs)
    {
        os << "[";
        os.precision(5);
        os.setf(std::ios_base::showpoint);
        os << _rhs.x << ", " << _rhs.y << ", " << _rhs.z;
        os << "]";
        return os;
    }
    
    QYTReal QYTPoint3::DistanceSquared(const QYTPoint3& p1, const QYTPoint3& p2)
    {
        return QYTVec3::LengthSquared(p1 - p2);
    }
    
    QYTReal QYTPoint3::Distance(const QYTPoint3& p1, const QYTPoint3& p2)
    {
        return QYTVec3::Length(p1 - p2);
    }
}