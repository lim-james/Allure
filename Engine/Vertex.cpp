#include "Vertex.h"

bool Vertex::operator==(Vertex const& rhs) const {
	return position == rhs.position
		&& texCoords == rhs.texCoords
		&& normal == rhs.normal;
}