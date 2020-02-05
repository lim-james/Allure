#ifndef VECTORS_HPP
#define VECTORS_HPP

#include <iostream>
#include <sstream>

namespace Math {

	template<typename T>
	class Vector {

		const int size;
		T* data;

	public:

		Vector(const int& size);
		~Vector();

		inline T& operator[](const unsigned& i);
		inline const T& operator[](const unsigned& i) const;

	};

	template<typename T>
	Vector<T>::Vector(const int & size) :size(size) {
		data = new T[size];
	}

	template<typename T>
	Vector<T>::~Vector() {
		delete data[];
	}

	template<typename T>
	inline T & Vector<T>::operator[](const unsigned & i) {
		return data[i];
	}

	template<typename T>
	inline const T & Vector<T>::operator[](const unsigned & i) const {
		return data[i];
	}

	template<typename T> struct vec2;
	template<typename T> struct vec3;
	template<typename T> struct vec4;

	template<typename T>
	struct vec2 {

		union {
			struct {
				T x, y;
			};

			struct {
				T w, h;
			};

			struct {
				T u, v;
			};
		};

		vec2(vec2<T>&&) = default;
		vec2(const vec2<T>&) = default;

		vec2(const T& i = static_cast<T>(0)) : x(i), y(i) {}
		vec2(const T& x, const T& y) : x(x), y(y) {}
		vec2(const vec3<T>& v) : x(v.x), y(v.y) {}
		vec2(const vec4<T>& v) : x(v.x), y(v.y) {}

		template<typename t>
		vec2(const vec2<t>& v) 
			: x(static_cast<T>(v.x))
			, y(static_cast<T>(v.y)) {}
		
		vec2<T>& operator=(vec2<T>&&) = default;

		vec2<T>& operator=(const T& i) {
			x = y = i;
			return *this;
		}

		vec2<T>& operator=(const vec2<T>& v) {
			x = v.x;
			y = v.y;
			return *this;
		}

		void Set(const T& i) {
			x = y = i;
		}

		void Set(const T& x, const T& y) {
			this->x = x;
			this->y = y;
		}

		void Set(const vec2<T>& v) {
			x = v.x;
			y = v.y;
		}

		void Set(const vec3<T>& v) {
			x = v.x;
			y = v.y;
		}

		void Set(const vec4<T>& v) {
			x = v.x;
			y = v.y;
		}
	};

	// standard maths
	// vec2 + vec2 = vec2
	template<typename T>
	inline vec2<T> operator+(const vec2<T>& lhs, const vec2<T>& rhs) {
		return vec2<T>(lhs.x + rhs.x, lhs.y + rhs.y);
	}
	// vec2 + T = vec2
	template<typename T>
	inline vec2<T> operator+(const vec2<T>& lhs, const T& rhs) {
		return vec2<T>(lhs.x + rhs, lhs.y + rhs);
	}
	// T + vec2 = vec2
	template<typename T>
	inline vec2<T> operator+(const T& lhs, const vec2<T>& rhs) {
		return vec2<T>(lhs + rhs.x, lhs + rhs.y);
	}
	// vec2 - vec2 = vec2
	template<typename T>
	inline vec2<T> operator-(const vec2<T>& lhs, const vec2<T>& rhs) {
		return vec2<T>(lhs.x - rhs.x, lhs.y - rhs.y);
	}
	// vec2 - T = vec2
	template<typename T>
	inline vec2<T> operator-(const vec2<T>& lhs, const T& rhs) {
		return vec2<T>(lhs.x - rhs, lhs.y - rhs);
	}
	// T - vec2 = vec2
	template<typename T>
	inline vec2<T> operator-(const T& lhs, const vec2<T>& rhs) {
		return vec2<T>(lhs - rhs.x, lhs - rhs.y);
	}
	// vec2 * vec2 = vec2
	template<typename T>
	inline vec2<T> operator*(const vec2<T>& lhs, const vec2<T>& rhs) {
		return vec2<T>(lhs.x * rhs.x, lhs.y * rhs.y);
	}
	// vec2 * T = vec2
	template<typename T>
	inline vec2<T> operator*(const vec2<T>& lhs, const T& rhs) {
		return vec2<T>(lhs.x * rhs, lhs.y * rhs);
	}
	// T * vec2 = vec2
	template<typename T>
	inline vec2<T> operator*(const T& lhs, const vec2<T>& rhs) {
		return vec2<T>(lhs * rhs.x, lhs * rhs.y);
	}
	// vec2 * vec2 = vec2
	template<typename T>
	inline vec2<T> operator/(const vec2<T>& lhs, const vec2<T>& rhs) {
		return vec2<T>(lhs.x / rhs.x, lhs.y / rhs.y);
	}
	// vec2 / T = vec2
	template<typename T>
	inline vec2<T> operator/(const vec2<T>& lhs, const T& rhs) {
		return vec2<T>(lhs.x / rhs, lhs.y / rhs);
	}
	// shorthand operators
	// += vec2
	template<typename T>
	vec2<T>& operator+=(vec2<T>& lhs, const vec2<T>& rhs) {
		lhs.x += rhs.x;
		lhs.y += rhs.y;
		return lhs;
	}
	// -= vec2
	template<typename T>
	vec2<T>& operator-=(vec2<T>& lhs, const vec2<T>& rhs) {
		lhs.x -= rhs.x;
		lhs.y -= rhs.y;
		return lhs;
	}
	// *= vec2
	template<typename T>
	vec2<T>& operator*=(vec2<T>& lhs, const vec2<T>& rhs) {
		lhs.x *= rhs.x;
		lhs.y *= rhs.y;
		return lhs;
	}
	// *= T
	template<typename T>
	vec2<T>& operator*=(vec2<T>& lhs, const T& rhs) {
		lhs.x *= rhs;
		lhs.y *= rhs;
		return lhs;
	}
	// /= vec2
	template<typename T>
	vec2<T>& operator/=(vec2<T>& lhs, const vec2<T>& rhs) {
		lhs.x /= rhs.x;
		lhs.y /= rhs.y;
		return lhs;
	}
	// /= T
	template<typename T>
	vec2<T>& operator/=(vec2<T>& lhs, const T& rhs) {
		lhs.x /= rhs;
		lhs.y /= rhs;
		return lhs;
	}
	// unary operators
	template<typename T>
	inline vec2<T> operator-(const vec2<T>& v) {
		return vec2<T>(-v.x, -v.y);
	}
	// comparison operator
	// equal
	template<typename T>
	inline bool operator==(const vec2<T>& lhs, const vec2<T>& rhs) {
		return lhs.x == rhs.x && lhs.y == rhs.y;
	}
	// not equal
	template<typename T>
	inline bool operator!=(const vec2<T>& lhs, const vec2<T>& rhs) {
		return lhs.x != rhs.x || lhs.y != rhs.y;
	}
	// >
	template<typename T>
	inline bool operator>(const vec2<T>& lhs, const vec2<T>& rhs) {
		return lhs.x > rhs.x && lhs.y > rhs.y;
	}
	// >=
	template<typename T>
	inline bool operator>=(const vec2<T>& lhs, const vec2<T>& rhs) {
		return lhs.x >= rhs.x && lhs.y >= rhs.y;
	}
	// <
	template<typename T>
	inline bool operator<(const vec2<T>& lhs, const vec2<T>& rhs) {
		return lhs.x < rhs.x && lhs.y < rhs.y;
	}
	// <=
	template<typename T>
	inline bool operator<=(const vec2<T>& lhs, const vec2<T>& rhs) {
		return lhs.x <= rhs.x && lhs.y <= rhs.y;
	}
	// io operators
	// output stream
	template<typename T>
	std::ostream& operator<<(std::ostream& os, const vec2<T>& v) {
		os << v.x << ',' << v.y;
		return os;
	}
	// length of vector
	template<typename T>
	inline T Length(const vec2<T>& v) {
		return sqrt(v.x * v.x + v.y * v.y);
	}
	// length squared
	template<typename T>
	inline T LengthSquared(const vec2<T>& v) {
		return v.x * v.x + v.y * v.y;
	}
	// dot product
	template<typename T>
	inline T Dot(const vec2<T>& lhs, const vec2<T>& rhs) {
		return lhs.x * rhs.x + lhs.y * rhs.y;
	}
	// normalize
	template<typename T>
	inline void Normalize(vec2<T>& v) {
		v /= Length(v);
	}
	// normalized
	template<typename T>
	inline vec2<T> Normalized(const vec2<T>& v) {
		return v / Length(v);
	}
	// projection
	template<typename T>
	inline vec2<T> Project(const vec2<T>& v, const vec2<T>& onto) {
		return Dot(v, onto) / LengthSquared(onto) * onto;
	}
	// abs
	template<typename T>
	inline vec2<T> Abs(const vec2<T>& v) {
		return vec2<T>(std::abs(v.x), std::abs(v.y));
	}

	template<typename T>
	struct vec3 {

		union {
			struct {
				T x, y, z;
			};

			struct {
				T r, g, b;
			};

			struct {
				vec2<T> xy;
				T z;
			};

			struct {
				T x;
				vec2<T> yz;
			};
		};

		vec3(vec3<T>&&) = default;
		vec3(const vec3<T>&) = default;

		vec3(const T& i = static_cast<T>(0)) : x(i), y(i), z(i) {}
		vec3(const T& x, const T& y, const T& z) : x(x), y(y), z(z) {}
		vec3(const vec2<T>& v, const T& z = static_cast<T>(0)) : x(v.x), y(v.y), z(z) {}
		vec3(const T& x, const vec2<T>& v) : x(x), y(v.x), z(v.y) {}
		vec3(const vec4<T>& v) : x(v.x), y(v.y), z(v.z) {}

		template<typename t>
		vec3(const vec3<t>& v) 
			: x(static_cast<T>(v.x))
			, y(static_cast<T>(v.y))
			, z(static_cast<T>(v.z)) {}
		
		vec3<T>& operator=(vec3<T>&&) = default;

		vec3<T>& operator=(const T& i) {
			x = y = z = i;
			return *this;
		}

		vec3<T>& operator=(const vec2<T>& v) {
			x = v.x;
			y = v.y;
			return *this;
		}

		vec3<T>& operator=(const vec3<T>& v) {
			x = v.x;
			y = v.y;
			z = v.z;
			return *this;
		}

		vec3<T>& operator=(const vec4<T>& v) {
			x = v.x;
			y = v.y;
			z = v.z;
			return *this;
		}

		void Set(const T& i) {
			x = y = z = i;
		}

		void Set(const T& x, const T& y, const T& z) {
			this->x = x;
			this->y = y;
			this->z = z;
		}

		void Set(const vec3<T>& v) {
			x = v.x;
			y = v.y;
			z = v.z;
		}

		void Set(const vec4<T>& v) {
			x = v.x;
			y = v.y;
			z = v.z;
		}

		void Set(const vec2<T>& v, const T& i) {
			x = v.x;
			y = v.y;
			z = i;
		}

		void Set(const T& i, const vec2<T>& v) {
			x = i;
			y = v.x;
			z = v.y;
		}
	};

	// standard maths
	// vec3 + vec3 = vec3
	template<typename T>
	inline vec3<T> operator+(const vec3<T>& lhs, const vec3<T>& rhs) {
		return vec3<T>(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
	}
	// vec3 + T = vec3
	template<typename T>
	inline vec3<T> operator+(const vec3<T>& lhs, const T& rhs) {
		return vec3<T>(lhs.x + rhs, lhs.y + rhs, lhs.z + rhs);
	}
	// T + vec3 = vec3
	template<typename T>
	inline vec3<T> operator+(const T& lhs, const vec3<T>& rhs) {
		return vec3<T>(lhs + rhs.x, lhs + rhs.y, lhs + rhs.z);
	}
	// vec3 - vec3 = vec3
	template<typename T>
	inline vec3<T> operator-(const vec3<T>& lhs, const vec3<T>& rhs) {
		return vec3<T>(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
	}
	// vec3 - T = vec3
	template<typename T>
	inline vec3<T> operator-(const vec3<T>& lhs, const T& rhs) {
		return vec3<T>(lhs.x - rhs, lhs.y - rhs, lhs.z - rhs);
	}
	// T - vec3 = vec3
	template<typename T>
	inline vec3<T> operator-(const T& lhs, const vec3<T>& rhs) {
		return vec3<T>(lhs - rhs.x, lhs - rhs.y, lhs - rhs.z);
	}
	// vec3 * vec3 = vec3
	template<typename T>
	inline vec3<T> operator*(const vec3<T>& lhs, const vec3<T>& rhs) {
		return vec3<T>(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z);
	}
	// vec3 * T = vec3
	template<typename T>
	inline vec3<T> operator*(const vec3<T>& lhs, const T& rhs) {
		return vec3<T>(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs);
	}
	// T * vec3 = vec3
	template<typename T>
	inline vec3<T> operator*(const T& lhs, const vec3<T>& rhs) {
		return vec3<T>(lhs * rhs.x, lhs * rhs.y, lhs * rhs.z);
	}
	// vec3 / vec3 = vec3
	template<typename T>
	inline vec3<T> operator/(const vec3<T>& lhs, const vec3<T>& rhs) {
		return vec3<T>(lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z);
	}
	// vec3 / T = vec3
	template<typename T>
	inline vec3<T> operator/(const vec3<T>& lhs, const T& rhs) {
		return vec3<T>(lhs.x / rhs, lhs.y / rhs, lhs.z / rhs);
	}
	// shorthand operators
	// += vec3
	template<typename T>
	vec3<T>& operator+=(vec3<T>& lhs, const vec3<T>& rhs) {
		lhs.x += rhs.x;
		lhs.y += rhs.y;
		lhs.z += rhs.z;
		return lhs;
	}
	// -= vec3
	template<typename T>
	vec3<T>& operator-=(vec3<T>& lhs, const vec3<T>& rhs) {
		lhs.x -= rhs.x;
		lhs.y -= rhs.y;
		lhs.z -= rhs.z;
		return lhs;
	}
	// *= vec3
	template<typename T>
	vec3<T>& operator*=(vec3<T>& lhs, const vec3<T>& rhs) {
		lhs.x *= rhs.x;
		lhs.y *= rhs.y;
		lhs.z *= rhs.z;
		return lhs;
	}
	// *= T
	template<typename T>
	vec3<T>& operator*=(vec3<T>& lhs, const T& rhs) {
		lhs.x *= rhs;
		lhs.y *= rhs;
		lhs.z *= rhs;
		return lhs;
	}
	// /= vec3
	template<typename T>
	vec3<T>& operator/=(vec3<T>& lhs, const vec3<T>& rhs) {
		lhs.x /= rhs.x;
		lhs.y /= rhs.y;
		lhs.z /= rhs.z;
		return lhs;
	}
	// /= T
	template<typename T>
	vec3<T>& operator/=(vec3<T>& lhs, const T& rhs) {
		lhs.x /= rhs;
		lhs.y /= rhs;
		lhs.z /= rhs;
		return lhs;
	}
	// unary operators
	template<typename T>
	inline vec3<T> operator-(const vec3<T>& v) {
		return vec3<T>(-v.x, -v.y, -v.z);
	}
	// comparison operator
	// equal
	template<typename T>
	inline bool operator==(const vec3<T>& lhs, const vec3<T>& rhs) {
		return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
	}
	// not equal
	template<typename T>
	inline bool operator!=(const vec3<T>& lhs, const vec3<T>& rhs) {
		return lhs.x != rhs.x || lhs.y != rhs.y || lhs.z != rhs.z;
	}
	// >
	template<typename T>
	inline bool operator>(const vec3<T>& lhs, const vec3<T>& rhs) {
		return lhs.x > rhs.x && lhs.y > rhs.y && lhs.z > rhs.z;
	}
	// >=
	template<typename T>
	inline bool operator>=(const vec3<T>& lhs, const vec3<T>& rhs) {
		return lhs.x >= rhs.x && lhs.y >= rhs.y && lhs.z >= rhs.z;
	}
	// <
	template<typename T>
	inline bool operator<(const vec3<T>& lhs, const vec3<T>& rhs) {
		return lhs.x < rhs.x && lhs.y < rhs.y && lhs.z < rhs.z;
	}
	// <=
	template<typename T>
	inline bool operator<=(const vec3<T>& lhs, const vec3<T>& rhs) {
		return lhs.x <= rhs.x && lhs.y <= rhs.y && lhs.z <= rhs.z;
	}
	// io operators
	// output stream
	template<typename T>
	std::ostream& operator<<(std::ostream& os, const vec3<T>& v) {
		os << v.x << ',' << v.y << ',' << v.z;
		return os;
	}
	// length of vector
	template<typename T>
	inline T Length(const vec3<T>& v) {
		return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
	}
	// length squared
	template<typename T>
	inline T LengthSquared(const vec3<T>& v) {
		return v.x * v.x + v.y * v.y + v.z * v.z;
	}
	// dot product
	template<typename T>
	inline T Dot(const vec3<T>& lhs, const vec3<T>& rhs) {
		return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
	}
	// cross product
	template<typename T>
	inline vec3<T> Cross(const vec3<T>& lhs, const vec3<T>& rhs) {
		return vec3<T>(
			lhs.y * rhs.z - lhs.z * rhs.y,
			lhs.z * rhs.x - lhs.x * rhs.z,
			lhs.x * rhs.y - lhs.y * rhs.x
		);
	}
	// normalize
	template<typename T>
	inline void Normalize(vec3<T>& v) {
		v /= Length(v);
	}
	// normalized
	template<typename T>
	inline vec3<T> Normalized(const vec3<T>& v) {
		return v / Length(v);
	}
	// projection
	template<typename T>
	inline vec3<T> Project(const vec3<T>& v, const vec3<T>& onto) {
		return Dot(v, onto) / LengthSquared(onto) * onto;
	}
	// abs
	template<typename T>
	inline vec3<T> Abs(const vec3<T>& v) {
		return vec3<T>(std::abs(v.x), std::abs(v.y), std::abs(v.z));
	}

	template<typename T>
	struct vec4 {

		union {
			struct {
				T x, y, z, w;
			};

			struct {
				T r, g, b, a;
			};

			struct {
				vec2<T> xy;
				vec2<T> zw;
			};

			struct {
				T x;
				vec2<T> yz;
				T w;
			};

			struct {
				vec3<T> xyz;
				T w;
			};

			struct {
				T x;
				vec3<T> yzw;
			};

			struct {
				vec2<T> origin;
				vec2<T> size;
			};
		};

		vec4(vec4<T>&&) = default;
		vec4(const vec4<T>&) = default;

		vec4(const T& i = static_cast<T>(0)) : x(i), y(i), z(i), w(i) {}
		vec4(const T& x, const T& y, const T& z, const T& w) : x(x), y(y), z(z), w(w) {}
		vec4(const vec2<T>& v, const T& z = static_cast<T>(0), const T& w = static_cast<T>(0)) : x(v.x), y(v.y), z(z), w(w) {}
		vec4(const vec2<T>& origin, const vec2<T>& size) : origin(origin), size(size) {}
		vec4(const T& x, const vec2<T>& v, const T& w = static_cast<T>(0)) : x(x), y(v.x), z(v.y), w(w) {}
		vec4(const T& x, const T& y, const vec2<T>& v) : x(x), y(y), z(v.x), w(v.y) {}
		vec4(const vec3<T>& v, const T& w = static_cast<T>(0)) : x(v.x), y(v.y), z(v.z), w(w) {}
		vec4(const T& x, const vec3<T>& v) : x(x), y(v.x), z(v.y), w(w.z) {}

		template<typename t>
		vec4(const vec4<t>& v)
			: x(static_cast<T>(v.x))
			, y(static_cast<T>(v.y))
			, z(static_cast<T>(v.z))
			, w(static_cast<T>(v.w)) {}

		vec4<T>& operator=(vec4<T>&&) = default;

		vec4<T>& operator=(const vec2<T>& v) {
			x = v.x;
			y = v.y;
			return *this;
		}

		vec4<T>& operator=(const vec3<T>& v) {
			x = v.x;
			y = v.y;
			z = v.z;
			return *this;
		}

		vec4<T>& operator=(const vec4<T>& v) {
			x = v.x;
			y = v.y;
			z = v.z;
			w = v.w;
			return *this;
		}

		vec4<T>& operator=(const T& i) {
			x = i;
			y = i;
			z = i;
			w = i;
			return *this;
		}

		void Set(const T& i) {
			x = i;
			y = i;
			z = i;
			w = i;
		}

		void Set(const T& x, const T& y, const T& z, const T& w) {
			this->x = x;
			this->y = y;
			this->z = z;
			this->w = w;
		}

		void Set(const vec4<T>& v) {
			x = v.x;
			y = v.y;
			z = v.z;
			w = v.w;
		}

		void Set(const vec2<T>& v, const T& i, const T& j) {
			x = v.x;
			y = v.y;
			z = i;
			w = j;
		}

		void Set(const vec2<T>& a, const vec2<T>& b) {
			origin.Set(a);
			size.Set(b);
		}

		void Set(const T& i, const vec2<T>& v, const T& j) {
			x = i;
			y = v.x;
			z = v.y;
			w = j;
		}

		void Set(const T& i, const T& j, const vec2<T>& v) {
			x = i;
			y = j;
			z = v.x;
			w = v.y;
		}

		void Set(const vec3<T>& v, const T& i) {
			x = v.x;
			y = v.y;
			z = v.z;
			w = i;
		}

		void Set(const T& i, const vec3<T>& v) {
			x = i;
			y = v.x;
			z = v.y;
			w = v.z;
		}
	};

	// standard maths
	// vec4 + vec4 = vec4
	template<typename T>
	inline vec4<T> operator+(const vec4<T>& lhs, const vec4<T>& rhs) {
		return vec4<T>(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w);
	}
	// vec4 + T = vec4
	template<typename T>
	inline vec4<T> operator+(const vec4<T>& lhs, const T& rhs) {
		return vec4<T>(lhs.x + rhs, lhs.y + rhs, lhs.z + rhs, lhs.w + rhs);
	}
	// T + vec4 = vec4
	template<typename T>
	inline vec4<T> operator+(const T& lhs, const vec4<T>& rhs) {
		return vec4<T>(lhs + rhs.x, lhs + rhs.y, lhs + rhs.z, lhs + rhs.w);
	}
	// vec4 - vec4 = vec4
	template<typename T>
	inline vec4<T> operator-(const vec4<T>& lhs, const vec4<T>& rhs) {
		return vec4<T>(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w);
	}
	// vec4 - T = vec4
	template<typename T>
	inline vec4<T> operator-(const vec4<T>& lhs, const T& rhs) {
		return vec4<T>(lhs.x - rhs, lhs.y - rhs, lhs.z - rhs, lhs.w - rhs);
	}
	// T - vec4 = vec4
	template<typename T>
	inline vec4<T> operator-(const T& lhs, const vec4<T>& rhs) {
		return vec4<T>(lhs - rhs.x, lhs - rhs.y, lhs - rhs.z, lhs - rhs.w);
	}
	// vec4 * vec4 = vec4
	template<typename T>
	inline vec4<T> operator*(const vec4<T>& lhs, const vec4<T>& rhs) {
		return vec4<T>(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z, lhs.w * rhs.w);
	}
	// vec4 * T = vec4
	template<typename T>
	inline vec4<T> operator*(const vec4<T>& lhs, const T& rhs) {
		return vec4<T>(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs, lhs.w * rhs);
	}
	// T * vec4 = vec4
	template<typename T>
	inline vec4<T> operator*(const T& lhs, const vec4<T>& rhs) {
		return vec4<T>(lhs * rhs.x, lhs * rhs.y, lhs * rhs.z, lhs * rhs.w);
	}
	// vec4 / vec4 = vec4
	template<typename T>
	inline vec4<T> operator/(const vec4<T>& lhs, const vec4<T>& rhs) {
		return vec4<T>(lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z, lhs.w / rhs.w);
	}
	// vec4 / T = vec4
	template<typename T>
	inline vec4<T> operator/(const vec4<T>& lhs, const T& rhs) {
		return vec4<T>(lhs.x / rhs, lhs.y / rhs, lhs.z / rhs, lhs.w / rhs);
	}
	// shorthand operators
	// += vec4
	template<typename T>
	vec4<T>& operator+=(vec4<T>& lhs, const vec4<T>& rhs) {
		lhs.x += rhs.x;
		lhs.y += rhs.y;
		lhs.z += rhs.z;
		lhs.w += rhs.w;
		return lhs;
	}
	// -= vec4
	template<typename T>
	vec4<T>& operator-=(vec4<T>& lhs, const vec4<T>& rhs) {
		lhs.x -= rhs.x;
		lhs.y -= rhs.y;
		lhs.z -= rhs.z;
		lhs.w -= rhs.w;
		return lhs;
	}
	// *= vec4
	template<typename T>
	vec4<T>& operator*=(vec4<T>& lhs, const vec4<T>& rhs) {
		lhs.x *= rhs.x;
		lhs.y *= rhs.y;
		lhs.z *= rhs.z;
		lhs.w *= rhs.w;
		return lhs;
	}
	// *= T
	template<typename T>
	vec4<T>& operator*=(vec4<T>& lhs, const T& rhs) {
		lhs.x *= rhs;
		lhs.y *= rhs;
		lhs.z *= rhs;
		lhs.w *= rhs;
		return lhs;
	}
	// /= vec4
	template<typename T>
	vec4<T>& operator/=(vec4<T>& lhs, const vec4<T>& rhs) {
		lhs.x /= rhs.x;
		lhs.y /= rhs.y;
		lhs.z /= rhs.z;
		lhs.w /= rhs.w;
		return lhs;
	}
	// /= T
	template<typename T>
	vec4<T>& operator/=(vec4<T>& lhs, const T& rhs) {
		lhs.x /= rhs;
		lhs.y /= rhs;
		lhs.z /= rhs;
		lhs.w /= rhs;
		return lhs;
	}
	// unary operators
	template<typename T>
	inline vec4<T> operator-(const vec4<T>& v) {
		return vec4<T>(-v.x, -v.y, -v.z, -v.w);
	}
	// comparison operator
	// equal
	template<typename T>
	inline bool operator==(const vec4<T>& lhs, const vec4<T>& rhs) {
		return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z && lhs.w == rhs.w;
	}
	// not equal
	template<typename T>
	inline bool operator!=(const vec4<T>& lhs, const vec4<T>& rhs) {
		return lhs.x != rhs.x || lhs.y != rhs.y || lhs.z != rhs.z || lhs.w != rhs.w;
	}
	// >
	template<typename T>
	inline bool operator>(const vec4<T>& lhs, const vec4<T>& rhs) {
		return lhs.x > rhs.x && lhs.y > rhs.y && lhs.z > rhs.z && lhs.w > rhs.w;
	}
	// >=
	template<typename T>
	inline bool operator>=(const vec4<T>& lhs, const vec4<T>& rhs) {
		return lhs.x >= rhs.x && lhs.y >= rhs.y && lhs.z >= rhs.z && lhs.w >= rhs.w;
	}
	// <
	template<typename T>
	inline bool operator<(const vec4<T>& lhs, const vec4<T>& rhs) {
		return lhs.x < rhs.x && lhs.y < rhs.y && lhs.z < rhs.z && lhs.w < rhs.w;
	}
	// <=
	template<typename T>
	inline bool operator<=(const vec4<T>& lhs, const vec4<T>& rhs) {
		return lhs.x <= rhs.x && lhs.y <= rhs.y && lhs.z <= rhs.z && lhs.w <= rhs.w;
	}
	// io operators
	// output stream
	template<typename T>
	std::ostream& operator<<(std::ostream& os, const vec4<T>& v) {
		os << v.x << ',' << v.y << ',' << v.z << ',' << v.w;
		return os;
	}
	// length of vector
	// length
	template<typename T>
	inline T Length(const vec4<T>& v) {
		return sqrt(v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w);
	}
	// length squared
	template<typename T>
	inline T LengthSquared(const vec4<T>& v) {
		return v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w;
	}
	// dot product
	template<typename T>
	inline T Dot(const vec4<T>& lhs, const vec4<T>& rhs) {
		return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z + lhs.w * rhs.w;
	}
	// normalize
	template<typename T>
	inline void Normalize(vec4<T>& v) {
		v /= Length(v);
	}
	// normalized
	template<typename T>
	inline vec4<T> Normalized(const vec4<T>& v) {
		return v / Length(v);
	}
	// projection
	template<typename T>
	inline vec4<T> Project(const vec4<T> v, const vec4<T> onto) {
		return Dot(v, onto) / LengthSquared(onto) * onto;
	}
	// abs
	template<typename T>
	inline vec4<T> Abs(const vec4<T>& v) {
		return vec4<T>(std::abs(v.x), std::abs(v.y), std::abs(v.z), std::abs(v.w));
	}

}

typedef Math::vec2<int> vec2i;
typedef Math::vec3<int> vec3i;
typedef Math::vec4<int> vec4i;

typedef Math::vec2<unsigned> vec2u;
typedef Math::vec3<unsigned> vec3u;
typedef Math::vec4<unsigned> vec4u;

typedef Math::vec2<float> vec2f;
typedef Math::vec3<float> vec3f;
typedef Math::vec4<float> vec4f;

typedef Math::vec2<double> vec2d;
typedef Math::vec3<double> vec3d;
typedef Math::vec4<double> vec4d;

#endif