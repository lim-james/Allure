#include "BitField.h"

#include <bitset>

BitField::BitField() : field(0) {}

BitField::BitField(unsigned const & flags) : field(flags) {}

bool BitField::Is(unsigned const & flag) const {
	return field & flag;
}

void BitField::Set(unsigned const & flag, bool const & state) {
	field = (field & ~flag) | (flag * state);
}

void BitField::Set(unsigned const & flags) {
	field = flags;
}

std::ostream & operator<<(std::ostream & os, BitField const & value) {
	os << value.field;
	return os;
}

BitField operator+(BitField const & field, unsigned const & flag) {
	return BitField(field.field | flag);
}

BitField operator+(unsigned const & flag, BitField const & field) {
	return BitField(field.field | flag);
}

BitField & operator+=(BitField & field, unsigned const & flag) {
	field = BitField(field.field | flag);
	return field;
}

BitField operator-(BitField const & field, unsigned const & flag) {
	return BitField(field.field & !flag);
}

BitField & operator-=(BitField & field, unsigned const & flag) {
	field = BitField(field.field & ~flag);
	return field;
}

bool operator==(BitField const & field, unsigned const & flag) {
	return field.field & flag;
}

bool operator==(BitField const & lhs, BitField const & rhs)
{
	return lhs.field == rhs.field;
}
