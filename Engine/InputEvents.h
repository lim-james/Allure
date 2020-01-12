#ifndef INPUT_EVENTS_H
#define INPUT_EVENTS_H

#include <Events/Event.h>
#include <Math/Vectors.hpp>

namespace Events {

	struct KeyInput : Event {
		const int key;
		const int scancode;
		const int action;
		const int mods;

		KeyInput(const int& key, const int& scancode, const int& action, const int& mods)
			: key(key)
			, scancode(scancode)
			, action(action)
			, mods(mods) {}
	};

	typedef AnyType<char> TextInput;

	struct CursorPositionInput : Event {
		const vec2f position;
		const vec2f offset;

		CursorPositionInput(const vec2f& position, const vec2f& offset)
			: position(position)
			, offset(offset) {}
	};

	struct MouseButtonInput : Event {
		const int button;
		const int action;
		const int mods;

		MouseButtonInput(const int& button, const int& action, const int& mods)
			: button(button)
			, action(action)
			, mods(mods) {}
	};

	typedef AnyType<vec2f> ScrollInput;

	struct InputMode : Event {
		const int mode;
		const int value;

		InputMode(const int& mode, const int& value)
			: mode(mode)
			, value(value) {}
	};

	struct DropInput : Event {
		const int count;
		const char** paths;

		DropInput(const int& count, const char** paths)
			: count(count)
			, paths(paths) {}
	};

}

#endif

