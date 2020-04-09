#ifndef BIT_FIELD_H
#define BIT_FIELD_H

#include <iostream>

class BitField {

	unsigned field;

public:

	BitField();
	BitField(unsigned const& flags);

	bool Is(unsigned const& flag) const;
	void Set(unsigned const& flag, bool const& state);
	void Set(unsigned const& flags);

	BitField& operator=(unsigned const& field);

	friend std::ostream& operator<<(std::ostream& os, BitField const& value);

	friend BitField operator+(BitField const& field, unsigned const& flag);
	friend BitField operator+(unsigned const& flag, BitField const& field);
	friend BitField& operator+=(BitField& field, unsigned const& flag);

	friend BitField operator-(BitField const& field, unsigned const& flag);
	friend BitField& operator-=(BitField& field, unsigned const& flag);

	friend bool operator==(BitField const& field, unsigned const& flag);
	friend bool operator==(unsigned const& flag, BitField const& field);
	friend bool operator==(BitField const& lhs, BitField const& rhs);

	friend bool operator!=(BitField const& field, unsigned const& flag);
	friend bool operator!=(unsigned const& flag, BitField const& field);
	friend bool operator!=(BitField const& lhs, BitField const& rhs);

};
#endif
