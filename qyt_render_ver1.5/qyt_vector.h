//
//  qyt_vector.h
//  qyt_fundation
//
//  Created by nocolor on 15/2/2.
//  Copyright (c) 2015年 ___JinJiangTiaoZhanBei___. All rights reserved.
//

#ifndef __qyt_fundation__qyt_vector__
#define __qyt_fundation__qyt_vector__

#include <stdio.h>
#include <algorithm>
#include <assert.h>
#include <math.h>
#include <iostream>

namespace QYT
{
    typedef float QYTReal;
    
    template<typename Type, size_t dimension>
    class qyt_vector;
    
    template<typename Type, size_t dimension>
    inline std::ostream& operator << (std::ostream& os, const qyt_vector<Type, dimension>& _rhs);

    template<typename Type, size_t row, size_t col>
    class qyt_matrix;
    
    /**
     * @class qyt_vector
     *  这是一个表示向量的基类，支持基本向量的数学计算。
     * @code
     *  qyt_vector<float, 3> v; //三维float型向量(0, 0, 0)
     *  float vec_value[3] = {1, 2, 3}
     *  qyt_vector<float, 3> v2(vec_value); //(1, 2, 3)
     *  std::cout << v+v2;      //(1, 2, 3)
     * @endcode
     */
    template<typename Type, size_t dimension>
    class qyt_vector
    {
    protected:
        Type v[dimension];
        
    public:
        inline qyt_vector();
        
        inline explicit qyt_vector(const Type& _value);
        inline explicit qyt_vector(Type* _values);
        inline explicit qyt_vector(const Type* _values);
        inline qyt_vector(const qyt_vector<Type, dimension>& _src);
        
        template <typename E, size_t _d>
        inline qyt_vector(const qyt_vector<E, _d>& _src);
        
        inline qyt_vector& operator = (const qyt_vector<Type, dimension>& _rhs);
        template <typename E, size_t _d>
        inline qyt_vector& operator = (const qyt_vector<E, _d>& _rhs);
        
        inline qyt_vector& operator = (const Type& _value);
        inline Type& operator[](size_t i);
        inline Type operator[](size_t i) const;
        
        inline const qyt_vector<Type, dimension>& operator+() const;
        inline qyt_vector<Type, dimension> operator+(const qyt_vector<Type, dimension>& _rhs) const;
        inline qyt_vector<Type, dimension> operator+(const Type& _rhs) const;
        
        inline friend qyt_vector<Type, dimension> operator+(const Type& _lhs, const qyt_vector<Type, dimension>& _rhs)
        {
            qyt_vector<Type, dimension> tmp;
            for (size_t i = 0; i < dimension; ++i)
                tmp[i] = _rhs.v[i] + _lhs;
            return tmp;
        }
        
        inline qyt_vector<Type, dimension>& operator+=(const qyt_vector<Type, dimension>& _rhs);
        inline qyt_vector<Type, dimension>& operator+=(const Type& _rhs);
        

        inline qyt_vector<Type, dimension> operator-(const Type& _rhs) const;
        inline qyt_vector<Type, dimension> operator-(const qyt_vector<Type, dimension>& _rhs) const;
        inline qyt_vector<Type, dimension>& operator-=(const qyt_vector<Type, dimension>& _rhs);
        inline qyt_vector<Type, dimension>& operator-=(const Type& _rhs);
        inline qyt_vector<Type, dimension> operator-() const;
        
        inline qyt_vector<Type, dimension> operator*(const Type& _rhs) const;
        inline qyt_vector<Type, dimension> operator*(const qyt_vector<Type, dimension>& _rhs) const;
        
        inline friend qyt_vector<Type, dimension> operator * (const Type& _lhs, const qyt_vector<Type, dimension>& _rhs)
        {
            return _rhs*_lhs;
        }
        
        inline qyt_vector<Type, dimension>& operator*=(const Type& _rhs);
        
        inline qyt_vector<Type, dimension> operator/(const Type& _rhs) const;
        inline qyt_vector<Type, dimension>& operator/=(const Type& _s);
        
        operator QYTReal*()
        {
            return &v[0];
        }
        
        inline bool operator == (const qyt_vector<Type, dimension>& _rhs) const;
        inline bool operator != (const qyt_vector<Type, dimension>& _rhs) const;
        
        inline size_t size() const;
        
        inline operator qyt_matrix<Type, 1, dimension>();
        
        inline friend std::ostream& operator << <Type, dimension> (std::ostream& os, const qyt_vector<Type, dimension>& _rhs);
    };
    
    /// 在默认基类的基础上，使用默认的类型为qyt_vec，默认类型是QYTReal类型
    /// qyt_vec<3> //等价于qyt_vector<QYTReal, 3>
    template <size_t dimension>
    using qyt_vec = qyt_vector<QYTReal, dimension>;
}

#include "qyt_vector_def.h"

#endif /* defined(__qyt_fundation__qyt_vector__) */
