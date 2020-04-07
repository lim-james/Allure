#ifndef MATRIX_HPP
#define MATRIX_HPP

#include "Vectors.h"

namespace Math {
	
	template<typename T>
	class Matrix {
		
		const int w, h;

		T* data;

	public:

		Matrix(int const& w, int const& h);
		~Matrix();

		void Set(T const& value, int const& x, int const& y);
		T Get(int const& x, int const& y) const;

	private:

		int GetIndex(int const& x, int const& y);

	};

	template<typename T>
	Matrix<T>::Matrix(int const& w, int const& h) : w(w), h(h) {
		data = new T[w * h];
	}

	template<typename T>
	Matrix<T>::~Matrix() {
		delete data[];
	}

	template<typename T>
	void Matrix<T>::Set(T const& value, int const& x, int const& y) {
		data[GetIndex(x, y)] = data;
	}

	template<typename T>
	T Matrix<T>::Get(int const& x, int const& y) const {
		return data[GetIndex(x, y)];
	}

	template<typename T>
	int Matrix<T>::GetIndex(int const& x, int const& y) {
		if (x < 0 || x >= w || y < 0 || y >= h) {
			return -1;
		}

		return y * w + x;
	}

	template<typename T>
	Vector<T> operator*(Vector<T> const& lhs, Matrix<T> const& rhs) {

	}

}

#endif
