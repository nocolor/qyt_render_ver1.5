//
//  qyt_vector_def.h
//  qyt_fundation
//
//  Created by nocolor on 15/2/2.
//  Copyright (c) 2015年 ___JinJiangTiaoZhanBei___. All rights reserved.
//

#ifndef qyt_fundation_qyt_vector_def_h
#define qyt_fundation_qyt_vector_def_h

#include "qyt_vector.h"
#include "qyt_matrix.h"

namespace QYT
{
    template <typename Type, size_t dimension>
    qyt_vector<Type, dimension>::qyt_vector()
    {
        for (size_t i = 0; i < dimension; ++i)
            v[i] = (Type)0.f;
    }
    
    template <typename Type, size_t dimension>
    qyt_vector<Type, dimension>::qyt_vector(const Type& _value)
    {
        for (size_t i = 0; i < dimension; ++i)
            v[i] = _value;
    }
    
    template <typename Type, size_t dimension>
    qyt_vector<Type, dimension>::qyt_vector(Type* _values)
    {
        for (size_t i = 0; i < dimension; ++i)
            v[i] = _values[i];
    }
    
    template <typename Type, size_t dimension>
    qyt_vector<Type, dimension>::qyt_vector(const Type* _values)
    {
        for (size_t i = 0; i < dimension; ++i)
            v[i] = _values[i];
    }
    
    template <typename Type, size_t dimension>
    qyt_vector<Type, dimension>::qyt_vector(const qyt_vector<Type, dimension>& _src)
    {
        std::copy(_src.v, _src.v+dimension, v);
    }
    
    template <typename Type, size_t dimension>
    template <typename E, size_t _d>
    qyt_vector<Type, dimension>::qyt_vector(const qyt_vector<E, _d>& _src)
    {
        for (size_t i = 0; i < dimension; ++i)
            v[i] = (Type)0.f;
        if(_d > dimension)
        {
            for (size_t i = 0; i < dimension; ++i) {
                v[i] = _src[i];
            }
        }
        else
        {
            for (size_t i = 0; i < _d; ++i) {
                v[i] = _src[i];
            }
        }
    }
    
    template <typename Type, size_t dimension>
    qyt_vector<Type, dimension>& qyt_vector<Type, dimension>::operator=(const qyt_vector<Type, dimension>& _rhs)
    {
        if(this != &_rhs)
            std::copy(_rhs.v, _rhs.v+dimension, v);
        return *this;
    }
    
    template <typename Type, size_t dimension>
    template <typename E, size_t _d>
    qyt_vector<Type, dimension>& qyt_vector<Type, dimension>::operator=(const qyt_vector<E, _d>& _rhs)
    {

        for (size_t i = 0; i < dimension; ++i)
            v[i] = (Type)0.f;
        if(dimension > _d)
            for (size_t i = 0; i < _d; ++i)
                v[i] = _rhs[i];
        else
            for (size_t i = 0; i < dimension; ++i)
                v[i] = _rhs[i];
        return *this;
    }
    
    template <typename Type, size_t dimension>
    qyt_vector<Type, dimension>& qyt_vector<Type, dimension>::operator=(const Type& _value)
    {
        for (size_t i = 0; i < dimension; ++i)
            v[i] = _value;
        return *this;
    }
    
    template <typename Type, size_t dimension>
    Type& qyt_vector<Type, dimension>::operator[](size_t i)
    {
        assert(i < dimension);
        return v[i];
    }
    
    template <typename Type, size_t dimension>
    Type qyt_vector<Type, dimension>::operator[](size_t i) const
    {
        assert(i < dimension);
        return v[i];
    }
    
    template <typename Type, size_t dimension>
    const qyt_vector<Type, dimension>& qyt_vector<Type, dimension>::operator+() const
    {
        return *this;
    }
    
    template <typename Type, size_t dimension>
    qyt_vector<Type, dimension> qyt_vector<Type, dimension>::operator+(const qyt_vector<Type, dimension>& _rhs) const
    {
        qyt_vector<Type, dimension> tmp;
        for (size_t i = 0; i < dimension; ++i)
            tmp[i] = v[i] + _rhs.v[i];
        return tmp;
    }
    
    template <typename Type, size_t dimension>
    qyt_vector<Type, dimension> qyt_vector<Type, dimension>::operator+(const Type &_rhs) const
    {
        qyt_vector<Type, dimension> tmp;
        for (size_t i = 0; i < dimension; ++i)
            tmp[i] = v[i] + _rhs;
        return tmp;
    }
    
    template <typename Type, size_t dimension>
    qyt_vector<Type, dimension>& qyt_vector<Type, dimension>::operator+=(const qyt_vector<Type, dimension> &_rhs)
    {
        *this = *this + _rhs;
        return *this;
    }
    
    template <typename Type, size_t dimension>
    qyt_vector<Type, dimension>& qyt_vector<Type, dimension>::operator+=(const Type& _rhs)
    {
        *this = *this + _rhs;
        return *this;
    }
    
    template <typename Type, size_t dimension>
    qyt_vector<Type, dimension> qyt_vector<Type, dimension>::operator-(const qyt_vector<Type, dimension> &_rhs) const
    {
        qyt_vector<Type, dimension> tmp;
        for (size_t i = 0; i < dimension; ++i)
            tmp[i] = v[i] - _rhs[i];
        return tmp;
    }
    
    template <typename Type, size_t dimension>
    qyt_vector<Type, dimension> qyt_vector<Type, dimension>::operator-(const Type &_rhs) const
    {
        qyt_vector<Type, dimension> tmp;
        for (size_t i = 0; i < dimension; ++i)
            tmp[i] = v[i] - _rhs;
        return tmp;
    }
    
    template <typename Type, size_t dimension>
    qyt_vector<Type, dimension> operator-(const Type& _lhs, const qyt_vector<Type, dimension>& _rhs)
    {
        qyt_vector<Type, dimension> tmp;
        for (size_t i = 0; i < dimension; ++i)
            tmp[i] = _rhs.v[i] - _lhs;
        return tmp;
    }
    
    template <typename Type, size_t dimension>
    qyt_vector<Type, dimension>& qyt_vector<Type, dimension>::operator-=(const qyt_vector<Type, dimension> &_rhs)
    {
        *this = *this - _rhs;
        return *this;
    }
    
    template <typename Type, size_t dimension>
    qyt_vector<Type, dimension>& qyt_vector<Type, dimension>::operator-=(const Type& _rhs)
    {
        *this = *this - _rhs;
        return *this;
    }
    
    template <typename Type, size_t dimension>
    qyt_vector<Type, dimension> qyt_vector<Type, dimension>::operator-() const
    {
        qyt_vector<Type, dimension> tmp;
        for (size_t i = 0; i < dimension; ++i)
            tmp[i] = -v[i];
        return tmp;
    }
    
    template <typename Type, size_t dimension>
    qyt_vector<Type, dimension> qyt_vector<Type, dimension>::operator*(const Type &_rhs) const
    {
        qyt_vector<Type, dimension> tmp;
        for (size_t i = 0; i < dimension; ++i)
            tmp[i] = v[i] * _rhs;
        return tmp;
    }
    
    template <typename Type, size_t dimension>
    qyt_vector<Type, dimension> qyt_vector<Type, dimension>::operator*(const qyt_vector<Type, dimension>& _rhs) const
    {
        qyt_vector<Type, dimension> tmp;
        for (size_t i = 0; i < dimension; ++i)
            tmp[i] = v[i] * _rhs[i];
        return tmp;
    }
    
    template <typename Type, size_t dimension>
    qyt_vector<Type, dimension>& qyt_vector<Type, dimension>::operator*=(const Type& _rhs)
    {
        *this = *this * _rhs;
        return *this;
    }
    
    template <typename Type, size_t dimension>
    qyt_vector<Type, dimension> qyt_vector<Type, dimension>::operator/(const Type& _rhs) const
    {
        assert(_rhs != 0.0);
        qyt_vector<Type, dimension> tmp;
        for (size_t i = 0; i < dimension; ++i)
            tmp[i] = v[i] / _rhs;
        return tmp;
    }
    
    template <typename Type, size_t dimension>
    qyt_vector<Type, dimension>& qyt_vector<Type, dimension>::operator/=(const Type& _rhs)
    {
        *this = *this / _rhs;
        return *this;
    }
    
    template <typename Type, size_t dimension>
    bool qyt_vector<Type, dimension>::operator==(const qyt_vector<Type, dimension> &_rhs) const
    {
        for (size_t i = 0; i < dimension; ++i)
            if(v[i] != _rhs.v[i])
                return false;
        return true;
    }
    
    template <typename Type, size_t dimension>
    bool qyt_vector<Type, dimension>::operator!=(const qyt_vector<Type, dimension> &_rhs) const
    {
        return !((*this) == _rhs);
    }
    
    template <typename Type, size_t dimension>
    size_t qyt_vector<Type, dimension>::size() const
    {
        return dimension;
    }
    
    template <typename Type, size_t dimension>
    qyt_vector<Type, dimension>::operator qyt_matrix<Type, 1, dimension>()
    {
        qyt_matrix<Type, 1, dimension> tmp;
        for (size_t i = 0; i < dimension; ++i) {
            tmp[0][i] = v[i];
        }
        return tmp;
    }
    
    template<typename Type, size_t dimension>
    std::ostream& operator << (std::ostream& os, const qyt_vector<Type, dimension>& _rhs)
    {
        os << "[";
        os.precision(5);
        os.setf(std::ios_base::showpoint);
        for(int i = 0; i < dimension; ++i)
        {
            os.width(13);
            os << _rhs.v[i] << " ";
        }
        
        os << "]";
        return os;
    }
    
}
#endif
