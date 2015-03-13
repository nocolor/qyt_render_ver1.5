//
//  qyt_matrix.h
//  qyt_fundation
//
//  Created by nocolor on 15/2/2.
//  Copyright (c) 2015年 ___JinJiangTiaoZhanBei___. All rights reserved.
//

#ifndef __qyt_fundation__qyt_matrix__
#define __qyt_fundation__qyt_matrix__

#include "qyt_vector.h"
#include "QYTError.h"

namespace QYT
{
    template <typename Type, size_t row, size_t col>
    class qyt_matrix;
    
    template <typename Type, size_t row, size_t col>
    std::ostream& operator << (std::ostream& os, const qyt_matrix<Type, row, col>& _rhs);
    
    template <typename Type, size_t row, size_t col>
    class qyt_matrix
    {
    protected:
        qyt_vector<Type, col> m[row];
        
    public:
        qyt_matrix(){}
        qyt_matrix(const Type& _value);
        qyt_matrix(Type* _values);
        qyt_matrix(const Type* _values);
        
        qyt_matrix(const qyt_matrix<Type, row, col>& _src);
        qyt_matrix& operator = (const qyt_matrix<Type, row, col>& _rhs);
        qyt_vector<Type, col>& operator[](size_t i);
        qyt_vector<Type, col> operator[](size_t i) const;
        
        void set_row(size_t _row, const qyt_vector<Type, col>& _vec);
        
        void set_col(size_t _col, const qyt_vector<Type, row>& _vec);
        
        qyt_matrix<Type, row, col> operator+(const qyt_matrix<Type, row, col>& _rhs) const;
        qyt_matrix<Type, row, col> operator+(const Type& _rhs) const;
        qyt_matrix<Type, row, col>& operator+=(const qyt_matrix<Type, row, col>& _rhs);
        qyt_matrix<Type, row, col>& operator+=(const Type& _rhs);
        
        qyt_matrix<Type, row, col> operator-(const qyt_matrix<Type, row, col>& _rhs) const;
        qyt_matrix<Type, row, col> operator-(const Type& _rhs) const;
        qyt_matrix<Type, row, col>& operator-=(const qyt_matrix<Type, row, col>& _rhs);
        qyt_matrix<Type, row, col>& operator-=(const Type& _rhs);
        qyt_matrix<Type, row, col> operator-() const;
        
        template<size_t _r_col>
        qyt_matrix<Type, row, _r_col> operator*(const qyt_matrix<Type, col, _r_col>& _rhs) const;
        
        qyt_matrix<Type, row, col> operator*(const Type& _rhs) const;
        friend qyt_matrix<Type, row, col> operator*(const Type& _lhs, const qyt_matrix<Type, row, col>& _rhs)
        {
            return _rhs*_lhs;
        }
        
        qyt_matrix<Type, row, col> operator/(const Type& _rhs) const;
        
        qyt_matrix<Type, col, row> operator~() const;
        
        
        friend std::ostream& operator << <Type, row, col>(std::ostream& os, const qyt_matrix<Type, row, col>& _rhs);

    };
}



#include "qyt_matrix_def.h"

#endif /* defined(__qyt_fundation__qyt_matrix__) */
