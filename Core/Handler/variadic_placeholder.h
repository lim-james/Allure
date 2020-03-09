#ifndef VARIADIC_PLACEHOLDER_H
#define VARIADIC_PLACEHOLDER_H

#include <functional>

template <int>
struct variadic_placeholder {};

namespace std {
	template <int N>
	struct is_placeholder<variadic_placeholder<N>>
		: integral_constant<int, N + 1> { };
}

#endif
