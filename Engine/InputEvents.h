#ifndef INPUT_EVENTS_H
#define INPUT_EVENTS_H

#include <Events/Event.h>
#include <Math/Vectors.h>

namespace Events {

	typedef AnyType<vec2i> WindowResize;

	struct KeyInput : Event {
		const int key;
		const int scancode;
		const int action;
		const int mods;

		KeyInput(int const& key, int const& scancode, int const& action, int const& mods)
			: key(key)
			, scancode(scancode)
			, action(action)
			, mods(mods) {}
	};

	typedef AnyType<char> TextInput;

	struct CursorPositionInput : Event {
		const vec2f position;
		const vec2f delta;

		CursorPositionInput(vec2f const& position, vec2f const& delta)
			: position(position)
			, delta(delta) {}
	};

	struct MouseButtonInput : Event {
		const int button;
		const int action;
		const int mods;

		MouseButtonInput(int const& button, int const& action, int const& mods)
			: button(button)
			, action(action)
			, mods(mods) {}
	};

	typedef AnyType<vec2f> ScrollInput;

	struct InputMode : Event {
		const int mode;
		const int value;

		InputMode(int const& mode, int const& value)
			: mode(mode)
			, value(value) {}
	};

	struct DropInput : Event {
		const int count;
		const char** paths;

		DropInput(int const& count, const char** paths)
			: count(count)
			, paths(paths) {}
	};

	struct JoystickState : Event {
		const int id;
		const int state;

		JoystickState(int const& id, int const& state)
			: id(id)
			, state(state) {}
	};

}

#endif

