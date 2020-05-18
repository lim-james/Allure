#ifndef SCORE_EVENT_H
#define SCORE_EVENT_H

#include <Events/Event.h>
#include <Math/Vectors.h>

namespace Events {

	struct Score : Event {
		unsigned points;
		vec3f position;

		Score(unsigned const& points, vec3f const& position)
			: points(points)
			, position(position) {}
	};

}

#endif
