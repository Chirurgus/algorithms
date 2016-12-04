#ifndef GUARD_MATRIX_H
#define GUARD_MATRIX_H

#include <initializer_list>

template<class Ty>
class Matrix {

public:
	//typedef unsigned long long size_type;
	using size_type = unsigned long long;

	/*Matrix Default Constructor*/
	Matrix<Ty>() : Matrix<Ty>(1, 1) { }

	/*Matrix Constructor, taking _height(ht) and _width(wd) as arguments*/
	Matrix<Ty>(size_type ht, size_type wd, const Ty &inital_value = Ty());

	/*Matrix Constructor, taking an initilizer list.*/
	Matrix<Ty>(std::initializer_list<std::initializer_list<Ty> > init_list);

	/*Matrix Copy Constructor*/
	Matrix<Ty>(const Matrix &old_mtx);

	/*Copy Assignment Cperator*/
	Matrix<Ty>& operator= (const Matrix &old_mtx);

	/*Destructor*/
	~Matrix<Ty>();

	/* Moves */
	Matrix<Ty>(Matrix<Ty>&& m);
	Matrix<Ty>& operator=(Matrix<Ty>&& m);

	/*Interface*/

	/*Getters*/
	unsigned GetHeight();
	unsigned GetWidth();

	unsigned GetHeight()const;
	unsigned GetWidth()const;


	/*Operators*/

	/*Subscript Operators*/
	Ty& operator() (const size_type i_pos, const size_type j_pos);
	Ty& operator() (const size_type i_pos, const size_type j_pos) const;

	/*Addition Operatosr*/
	Matrix<Ty> &operator+= (const Matrix &rhs_mtx);
	Matrix<Ty> operator+ (const Matrix &rhs_mtx) const;

	/*Substration Operators*/
	Matrix<Ty> &operator-= (const Matrix &rhs_mtx);
	Matrix<Ty> operator- (const Matrix &rhs_mtx) const;

	/*Multiplication Operators*/
	Matrix<Ty> &operator*= (const Matrix &rhs_mtx);
	Matrix<Ty> operator* (const Matrix &rhs_mtx) const;



private:
	size_type _height;
	size_type _width;
	Ty* _mtx;

	/*Memory allocator*/
	void arrayAllocator(size_type ht, size_type wd);

	/*Memory freeyer*/
	void arrayDelocator();
};

/*Non-Member funcitons*/

/* Multiplicators by a scalar */
template<class Ty>
Matrix<Ty> operator* (const Ty &scalar, const Matrix<Ty> &mtx);

template<class Ty>
Matrix<Ty> operator* (const Matrix<Ty> &mtx, const Ty &scalar);

template<class Ty>
Matrix<Ty>& operator*= (Matrix<Ty> &mtx, const Ty &scalar);

/* Trace */
template<class Ty>
Ty tr(const Matrix<Ty> &mtx);

/* Basic line operations */
template<class Ty>
Matrix<Ty>& swap_lines(Matrix<Ty> &mtx,
	typename Matrix<Ty>::size_type L1,
	typename Matrix<Ty>::size_type L2);

template<class Ty>
Matrix<Ty>& swap_columns(Matrix<Ty> &mtx,
	typename Matrix<Ty>::size_type C1,
	typename Matrix<Ty>::size_type C2);

template<class Ty>
Matrix<Ty>& scale_a_line(Matrix<Ty> &mtx,
	typename Matrix<Ty>::size_type L1,
	const Ty &coef);

template<class Ty>
Matrix<Ty>& add_lines(Matrix<Ty> &mtx,
	typename Matrix<Ty>::size_type L1,
	typename Matrix<Ty>::size_type L2,
	const Ty &coef);

template<class Ty>
Matrix<Ty>& substract_lines(Matrix<Ty> &mtx,
	typename Matrix<Ty>::size_type L1,
	typename  Matrix<Ty>::size_type L2,
	const Ty &coef);

#include "matrix.cpp"

#endif
