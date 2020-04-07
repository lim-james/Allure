#ifndef EVENT_H
#define EVENT_H

#include <string>

namespace Events {

	struct Event {
		std::string name;

		virtual ~Event() {}
	};

	template<typename Data>
	struct AnyType : public Event {
		Data data;

		AnyType(Data const& data) : data(data) {}
	};

	typedef AnyType<std::string> String;

}

#endif
