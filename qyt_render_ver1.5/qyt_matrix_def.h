//
//  qyt_matrix_def.h
//  qyt_fundation
//
//  Created by nocolor on 15/2/2.
//  Copyright (c) 2015年 ___JinJiangTiaoZhanBei___. All rights reserved.
//

#ifndef qyt_fundation_qyt_matrix_def_h
#define qyt_fundation_qyt_matrix_def_h

#include "qyt_matrix.h"

namespace QYT
{
    template <typename Type, size_t row, size_t col>
    qyt_matrix<Type, row, col>::qyt_matrix(const Type& _value)
    {
        for (size_t i = 0; i < row; ++i)
            m[i] = qyt_vector<Type, col>(_value);
    }
    
    template <typename Type, size_t row, size_t col>
    qyt_matrix<Type, row, col>::qyt_matrix(Type* _values)
    {
        for (size_t i = 0; i < row*col; ++i)
            m[i/col][i - col*(i/col)] = _values[i];
    }
    
    template <typename Type, size_t row, size_t col>
    qyt_matrix<Type, row, col>::qyt_matrix(const Type* _values)
    {
        for (size_t i = 0; i < row*col; ++i)
            m[i/col][i - col*(i/col)] = _values[i];
    }
    
    template <typename Type, size_t row, size_t col>
    qyt_matrix<Type, row, col>::qyt_matrix(const qyt_matrix<Type, row, col>& _src)
    {
        for (size_t i = 0; i < row; ++i)
            m[i] = _src.m[i];
    }
    
    template <typename Type, size_t row, size_t col>
    qyt_matrix<Type, row, col>& qyt_matrix<Type, row, col>::operator=(const qyt_matrix<Type, row, col> &_rhs)
    {
        if(this != &_rhs)
        {
            for (size_t i = 0; i < row; ++i)
                m[i] = _rhs.m[i];
        }
        return *this;
    }
    
    template <typename Type, size_t row, size_t col>
    qyt_vector<Type, col> qyt_matrix<Type, row, col>::operator[](size_t i) const
    {
        return m[i];
    }
    
    template <typename Type, size_t row, size_t col>
    qyt_vector<Type, col>& qyt_matrix<Type, row, col>::operator[](size_t i)
    {
        return m[i];
    }
    
    template <typename Type, size_t row, size_t col>
    void qyt_matrix<Type, row, col>::set_row(size_t _row, const qyt_vector<Type, col> &_vec)
    {
        m[_row] = _vec;
    }
    
    template <typename Type, size_t row, size_t col>
    void qyt_matrix<Type, row, col>::set_col(size_t _col, const qyt_vector<Type, row> &_vec)
    {
        for (size_t i = 0; i < row; ++i)
            m[i][_col] = _vec[i];
    }
    
    template <typename Type, size_t row, size_t col>
    qyt_matrix<Type, row, col> qyt_matrix<Type, row, col>::operator+(const qyt_matrix<Type, row, col> &_rhs) const
    {
        qyt_matrix<Type, row, col> tmp;
        for (size_t i = 0; i < row; ++i)
            tmp[i] = m[i] + _rhs[i];
        return tmp;
    }
    
    template <typename Type, size_t row, size_t col>
    qyt_matrix<Type, row, col> qyt_matrix<Type, row, col>::operator+(const Type &_rhs) const
    {
        qyt_matrix<Type, row, col> tmp;
        for (size_t i = 0; i < row; ++i)
            tmp[i] = m[i] + _rhs;
        return tmp;
    }
    
    template <typename Type, size_t row, size_t col>
    qyt_matrix<Type, row, col>& qyt_matrix<Type, row, col>::operator+=(const qyt_matrix<Type, row, col> &_rhs)
    {
        *this = *this + _rhs;
        return *this;
    }
    
    template <typename Type, size_t row, size_t col>
    qyt_matrix<Type, row, col>& qyt_matrix<Type, row, col>::operator+=(const Type &_rhs)
    {
        *this = *this + _rhs;
        return *this;
    }
    
    template <typename Type, size_t row, size_t col>
    qyt_matrix<Type, row, col> qyt_matrix<Type, row, col>::operator-(const qyt_matrix<Type, row, col> &_rhs) const
    {
        qyt_matrix<Type, row, col> tmp;
        for (size_t i = 0; i < row; ++i)
            tmp[i] = m[i] - _rhs[i];
        return tmp;
    }
    
    template <typename Type, size_t row, size_t col>
    qyt_matrix<Type, row, col> qyt_matrix<Type, row, col>::operator-(const Type &_rhs) const
    {
        qyt_matrix<Type, row, col> tmp;
        for (size_t i = 0; i < row; ++i)
            tmp[i] = m[i] - _rhs;
        return tmp;
    }
    
    template <typename Type, size_t row, size_t col>
    qyt_matrix<Type, row, col>& qyt_matrix<Type, row, col>::operator-=(const qyt_matrix<Type, row, col> &_rhs)
    {
        *this = *this - _rhs;
        return *this;
    }
    
    template <typename Type, size_t row, size_t col>
    qyt_matrix<Type, row, col>& qyt_matrix<Type, row, col>::operator-=(const Type &_rhs)
    {
        *this = *this - _rhs;
        return *this;
    }
    
    template <typename Type, size_t row, size_t col>
    qyt_matrix<Type, row, col> qyt_matrix<Type, row, col>::operator-() const
    {
        qyt_matrix<Type, row, col> tmp;
        for (size_t i = 0; i < row; ++i)
            tmp[i] = -m[i];
        return tmp;
    }
    
    template <typename Type, size_t row, size_t col>
    template<size_t _r_col>
    qyt_matrix<Type, row, _r_col> qyt_matrix<Type, row, col>::operator*(const qyt_matrix<Type, col, _r_col> &_rhs) const
    {
        qyt_matrix<Type, row, _r_col> tmp_m;
        Type tmp = (Type)0.f;
        size_t i, j, k, n=0;
        
        for (i = 0; i < row; i++)
        {
            for (j = 0; j < _r_col; j++)
            {
                for (k = 0; k < col; k++)
                    tmp += m[i][k]*_rhs[k][j];
                
                tmp_m[i][n++] = tmp;
                if(n == _r_col)
                    n = 0;
                tmp = (Type)0.f;
            }
        }
        return tmp_m;
    }
    
    template <typename Type, size_t row, size_t col>
    qyt_matrix<Type, row, col> qyt_matrix<Type, row, col>::operator*(const Type &_rhs) const
    {
        qyt_matrix<Type, row, col> tmp;
        for (size_t i = 0; i < row; ++i)
            tmp[i] = m[i]*_rhs;
        return tmp;
    }
    
    template <typename Type, size_t row, size_t col>
    qyt_matrix<Type, row, col> qyt_matrix<Type, row, col>::operator/(const Type &_rhs) const
    {
        qyt_matrix<Type, row, col> tmp;
        for (size_t i = 0; i < row; ++i)
            tmp[i] = m[i]/_rhs;
        return tmp;
    }
    
    template <typename Type, size_t row, size_t col>
    qyt_matrix<Type, col, row> qyt_matrix<Type, row, col>::operator~() const
    {
        qyt_matrix<Type, col, row> tmp;
        for (size_t i = 0; i < col; ++i)
            for (size_t j = 0; j < row; ++j) {
                tmp[i][j] = m[j][i];
            }
        return tmp;
    }
    
    template <typename Type, size_t row, size_t col>
    std::ostream& operator << (std::ostream& os, const qyt_matrix<Type, row, col>& _rhs)
    {
        for (size_t i = 0; i < row; ++i)
            os << _rhs.m[i] << std::endl;
        return os;
    }
    
}

#endif
