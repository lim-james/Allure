#ifndef VECTOR_HELPERS_H
#define VECTOR_HELPERS_H

#include <vector>

namespace Helpers {

	template<typename T>
	bool Insert(std::vector<T>& v, T const& item);

	template<typename T>
	bool Insert(std::vector<T>& v, T const& item, bool const& ascending);

	template<typename T>
	bool Remove(std::vector<T>& v, T const& item);

	template<typename T>
	inline bool Contains(std::vector<T>& v, T const& item);

}

template<typename T>
bool Helpers::Insert(std::vector<T>& v, T const& item) {
	const auto position = std::find(v.begin(), v.end(), item);
	if (position == v.end()) {
		v.push_back(item);
		return true;
	}

	return false;
}

template<typename T>	
bool Helpers::Insert(std::vector<T>& v, T const& item, bool const& ascending) {
	const auto position = std::find(v.begin(), v.end(), item);
	if (position == v.end()) {
		if (ascending) {
			for (unsigned i = 0; i < v.size(); ++i) {
				if (v[i] > item) {
					v.insert(v.begin() + i);
					return true;
				}
			}
		} else {
			for (unsigned i = 0; i < v.size(); ++i) {
				if (v[i] < item) {
					v.insert(v.begin() + i);
					return true;
				}
			}
		}
		v.push_back(item);
		return true;
	}

	return false;
}

template<typename T>
bool Helpers::Remove(std::vector<T>& v, T const& item) {
	const auto position = std::find(v.begin(), v.end(), item);
	if (position != v.end()) {
		v.erase(position);
		return true;
	}

	return false;
}

template<typename T>
inline bool Helpers::Contains(std::vector<T>& v, T const& item) {
	return std::find(v.begin(), v.end(), item) != v.end();
}

#endif
