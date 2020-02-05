#ifndef MATRIX_HPP
#define MATRIX_HPP

#include "Vectors.hpp"

namespace Math {
	
	template<typename T>
	class Matrix {
		
		const int w, h;

		T* data;

	public:

		Matrix(const int& w, const int& h);
		~Matrix();

		void Set(const T& value, const int& x, const int& y);
		T Get(const int& x, const int& y) const;

	private:

		int GetIndex(const int& x, const int& y);

	};

	template<typename T>
	Matrix<T>::Matrix(const int & w, const int & h) : w(w), h(h) {
		data = new T[w * h];
	}

	template<typename T>
	Matrix<T>::~Matrix() {
		delete data[];
	}

	template<typename T>
	void Matrix<T>::Set(const T & value, const int & x, const int & y) {
		data[GetIndex(x, y)] = data;
	}

	template<typename T>
	T Matrix<T>::Get(const int & x, const int & y) const {
		return data[GetIndex(x, y)];
	}

	template<typename T>
	int Matrix<T>::GetIndex(const int & x, const int & y) {
		if (x < 0 || x >= w || y < 0 || y >= h) {
			return -1;
		}

		return y * w + x;
	}

	template<typename T>
	Vector<T> operator*(const Vector<T>& lhs, const Matrix<T>& rhs) {

	}

}

#endif
