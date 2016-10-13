#ifndef MATRIX_MATRIX_CPP
#define MATRIX_MATRIX_CPP

#include "matrix.h"
#include <iostream>

/*Member functions*/

/*Private member-functions*/
template<class Ty>
void Matrix<Ty>::arrayAllocator(size_type ht, size_type wd) {
	_height = ht;
	_width = wd;
	_mtx = new Ty[_height * _width];
}

template<class Ty>
void Matrix<Ty>::arrayDelocator() {
	delete[] _mtx;
}

/*Constructors*/
template<class Ty>
Matrix<Ty>::Matrix(size_type ht, size_type wd, const Ty &inital_value = Ty()) {
	arrayAllocator(ht, wd);
	for (size_type i = 0, max_i = _height * _width; i < max_i; i++)
		this->_mtx[i] = inital_value;
}

template<class Ty>
Matrix<Ty>::Matrix
(std::initializer_list<std::initializer_list<Ty> > init_list) {
	arrayAllocator(init_list.size(), init_list.begin()->size());
	size_type i, j;
	std::initializer_list<std::initializer_list<Ty> >::iterator i_it;
	std::initializer_list<Ty>::iterator j_it;
	for (i_it = init_list.begin(), i = 0; i_it != init_list.end(); i_it++, i++)
		for (j_it = i_it->begin(), j = 0; j_it != i_it->end(); j_it++, j++)
			this->_mtx[_width * i + j] = *j_it;
}
template<class Ty>
Matrix<Ty>::Matrix(const Matrix &old_mtx) {
	arrayAllocator(old_mtx._height, old_mtx._width);
	for (size_type i = 0, max_i = _height * _width; i < max_i; i++)
		this->_mtx[i] = old_mtx._mtx[i];
}

template<class Ty>
Matrix<Ty>& Matrix<Ty>::operator= (const Matrix &old_mtx) {
	if (this != &old_mtx) {
		arrayDelocator();
		arrayAllocator(old_mtx._height, old_mtx._width);
		for (size_type i = 0, max_i = _height * _width; i < max_i; i++)
			this->_mtx[i] = old_mtx._mtx[i];
	}
	return *this;//This is supposed to return a reference to this object, not an instance of the object, but it seems that C++ should add & implicity
}

template<class Ty>
Matrix<Ty>::~Matrix() {
	arrayDelocator();
}

template<class Ty>
Matrix<Ty>::Matrix(Matrix<Ty>&& m)
	:_height {m._height},
	_width {m._width},
	_mtx {m._mtx} {
	m._height = m._width = 0;
	m.arrayAllocator(m._height, m._width);
}

template<class Ty>
Matrix<Ty>& Matrix<Ty>::operator=(Matrix<Ty>&& m) {
	this->_height = m._height;
	this->_width = m._width;
	this->_mtx = m._mtx;
	m._height = m._width = 0;
	m.arrayAllocator(m._height, m._width);
	return *this;
}

/*Interface*/

/*Acessors*/
template<class Ty>
unsigned Matrix<Ty>::GetHeight() { return _height; }

template<class Ty>
unsigned Matrix<Ty>::GetWidth() { return _width; }

template<class Ty>
unsigned Matrix<Ty>::GetHeight() const { return _height; }

template<class Ty>
unsigned Matrix<Ty>::GetWidth() const { return _width; }

/*Operators*/

/*Funcotor*/
/*Subscript Operators*/

template<class Ty>
Ty& Matrix<Ty>::operator() (const size_type i_pos, const size_type j_pos) {
	return _mtx[_width * i_pos + j_pos];
}

template<class Ty>
Ty& Matrix<Ty>::operator() (const size_type i_pos, const size_type j_pos) const {
	return _mtx[_width * i_pos + j_pos];
}

/*Addition Operatosr*/
template<class Ty>
Matrix<Ty>& Matrix<Ty>::operator+= (const Matrix &rhs__mtx) {
	for (size_type i = 0; i < _height; i++)
		for (size_type j = 0; j < _width; j++)
			this->_mtx[i * _width + j] = this->_mtx[i * _width + j] + rhs__mtx._mtx[i * _width + j];
	return *this;
}

template<class Ty>
Matrix<Ty> Matrix<Ty>::operator+ (const Matrix &rhs__mtx) const {
	return Matrix<Ty>(*this) += rhs__mtx;
}

/*Substration Operators*/
template<class Ty>
Matrix<Ty>& Matrix<Ty>::operator-= (const Matrix &rhs__mtx) {
	for (size_type i = 0; i < _height; i++)
		for (size_type j = 0; j < _width; j++)
			this->_mtx[i * _width + j] = this->_mtx[i * _width + j] - rhs__mtx._mtx[i * _width + j];
	return *this;
}

template<class Ty>
Matrix<Ty> Matrix<Ty>::operator- (const Matrix &rhs__mtx) const {
	return Matrix<Ty>(*this) -= rhs__mtx;
}

/*Multiplication Operators*/
template<class Ty>
Matrix<Ty>& Matrix<Ty>::operator*= (const Matrix &rhs_mtx) {
	return *this = *this * rhs_mtx;
}

template<class Ty>
Matrix<Ty> Matrix<Ty>::operator* (const Matrix &rhs_mtx) const {
	Matrix<Ty>::size_type ret_mtx_height = this->GetHeight();
	Matrix<Ty>::size_type ret_mtx_width = rhs_mtx.GetWidth();

	/*The count variable is based on left hand side width*/
	Matrix<Ty>::size_type side_length = this->GetWidth();
	
	Matrix<Ty> ret_mtx(ret_mtx_height, ret_mtx_width);

	for (Matrix<Ty>::size_type i = 0; i < ret_mtx_height; i++) {
		for (Matrix<Ty>::size_type j = 0; j < ret_mtx_width; j++) {
			Ty sum = Ty();

			for (Matrix<Ty>::size_type count = 0; count < side_length; count++)
				sum += this->_mtx[i * _width + count] * rhs_mtx._mtx[count * _width + j];

			ret_mtx._mtx[i * _width + j] = sum;
		}
	}
	return ret_mtx;
}
/*</Member_functions>*/

/*Non-Member Functions*/

/*Multiplication by a scalar*/
template<class Ty>
Matrix<Ty> operator* (const Ty &scalar, const Matrix<Ty> &mtx) {
	Matrix<Ty> ret_mtx(mtx);

	Matrix<Ty>::size_type height = mtx.GetHeight();
	Matrix<Ty>::size_type width = mtx.GetWidth();

	for (Matrix<Ty>::size_type i = 0; i < height; i++)
		for (Matrix<Ty>::size_type j = 0; j < width; j++)
			ret_mtx(i, j) *= scalar;
	return ret_mtx;
}

template<class Ty>
Matrix<Ty> operator* (const Matrix<Ty> &mtx, const Ty &scalar) {
	return scalar * mtx;
}

template<class Ty>
Matrix<Ty>& operator*= (Matrix<Ty> &mtx, const Ty &scalar) {
	return mtx = mtx * scalar;
}

/*Trace*/
template<class Ty>
Ty tr(const Matrix<Ty> &mtx) {
	Matrix<Ty>::size_type min_side = mtx.GetHeight();
	Matrix<Ty>::size_type width = mtx.GetWidth();

	if (min_side > width)
		min_side = width;

	Ty sum = 0;
	for (size_type i = 0; i < min_side; i++)
		sum += _mtx[i][i];

	return sum;
}

/*Basic line opertions*/
template<class Ty>
Matrix<Ty>& swap_lines(Matrix<Ty> &mtx,
	typename Matrix<Ty>::size_type L1,
	typename Matrix<Ty>::size_type L2) {
	Matrix<Ty>::size_type width = mtx.GetWidth();
	for (Matrix<Ty>::size_type i = 0; i < width; i++) {
		Ty tmp = mtx(L1, i);
		mtx(L1, i) = mtx(L2, i);
		mtx(L2, i) = tmp;
	}
	return mtx;
}

template<class Ty>
Matrix<Ty>& swap_columns(Matrix<Ty> &mtx,
	typename Matrix<Ty>::size_type C1,
	typename Matrix<Ty>::size_type C2) {
	Matrix<Ty>::size_type height = mtx.GetHeight();
	for (Matrix<Ty>::size_type i = 0; i < height; i++) {
		Ty tmp = mtx(i, C1);
		mtx(i, C1) = mtx(i, C2);
		mtx(i, C2) = tmp;
	}
	return mtx;
}

template<class Ty>
Matrix<Ty>& scale_a_line(Matrix<Ty> &mtx,
	typename Matrix<Ty>::size_type L1,
	const Ty &coef) {
	Matrix<Ty>::size_type width = mtx.GetWidth();
	for (Matrix<Ty>::size_type i = 0; i < width; i++) {
		mtx[L1][i] *= coef;
	}
	return mtx;
}

template<class Ty>
Matrix<Ty>& add_lines(Matrix<Ty> &mtx,
	typename Matrix<Ty>::size_type L1,
	typename Matrix<Ty>::size_type L2,
	const Ty &coef = 1) {

	Matrix<Ty>::size_type width = mtx.GetWidth();

	for (Matrix<Ty>::size_type i = 0; i < width; i++) {
		mtx(L1, i) += mtx(L2, i)*coef;
	}

	return mtx;
}

template<class Ty>
Matrix<Ty>& substract_lines(Matrix<Ty> &mtx,
	typename Matrix<Ty>::size_type L1,
	typename  Matrix<Ty>::size_type L2,
	const Ty &coef) {

	Matrix<Ty>::size_type width = mtx.GetWidth();

	for (Matrix<Ty>::size_type i = 0; i < width; i++) {
		mtx(L1, i) -= mtx(L2, i)*coef;
	}

	return mtx;
}


#endif