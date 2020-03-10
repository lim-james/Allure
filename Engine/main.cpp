//extern "C" {
//	_declspec(dllexport) unsigned NvOptimusEnablement = 0x00000001;
//}

#include <Handler/Handler.hpp>

#include "Application.h"

void VoidHandler() {
	Debug::Log << "Void handler\n";
}

void StringHandler(std::string string) {
	Debug::Log << string << " handler\n";
}

void StandardHandler(unsigned id) {
	Debug::Log << "id: " << id << " handler\n";
}
//
//template<typename ... arg_t>
//void Bind(void (*callback)(arg_t...), arg_t... arg) {
//	std::bind(callback, arg...)();
//}

struct Test {
	void VoidHandler() {
		Debug::Log << "method Void handler\n";
	}

	void StringHandler(std::string string) const {
		Debug::Log << string << " handler method\n";
	}

	void StandardHandler(unsigned id) {
		Debug::Log << "id: " << id << " handler method\n";
	}
};

int main() { 
	Handler<void, unsigned> handler;

	handler.Bind(VoidHandler);
	handler.Bind(StringHandler, std::string("Hello"));
	handler.Bind(StandardHandler);

	Test test;

	handler.Bind(&Test::StringHandler, &test, std::string("Wahtsup"));
	handler.Bind(&Test::StandardHandler, &test);

	handler();
	handler(100);

	return 0;

	Application app;

	app.Initialize(1600, 900, "Allure 2D", false);
	app.Run();
	app.Exit();

	return 0;
}