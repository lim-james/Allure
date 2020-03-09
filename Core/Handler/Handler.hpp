#ifndef HANDLER_H
#define HANDLER_H

#include "variadic_placeholder.h"

#include <vector>

template<typename ReturnType, typename ... ParameterTypes>
class Handler {

private:

	std::vector<std::function<ReturnType(void)>> customCallbacks;
	std::vector<std::function<ReturnType(ParameterTypes...)>> standardCallbacks;

public:

	// void class methods
	template<typename Context>
	void Bind(ReturnType (Context::*callback)(void), Context* context);
	template<typename Context>
	void Bind(ReturnType (Context::*callback)(void) const, Context* context);
	// void functions
	void Bind(ReturnType (*callback)(void));

	// standard methods
	template<typename Context>
	void Bind(ReturnType (Context::*callback)(ParameterTypes...), Context* context);
	template<typename Context>
	void Bind(ReturnType (Context::*callback)(ParameterTypes...) const, Context* context);
	// void functions
	void Bind(ReturnType (*callback)(ParameterTypes...));

	// custom methods
	template<typename Context, typename ... ArgumentTypes>
	void Bind(ReturnType (Context::*callback)(ArgumentTypes...), Context* context, ArgumentTypes... arguments);
	template<typename Context, typename ... ArgumentTypes>
	void Bind(ReturnType (Context::*callback)(ArgumentTypes...) const, Context* context, ArgumentTypes... arguments);
	// void functions
	template<typename ... ArgumentTypes>
	void Bind(ReturnType (*callback)(ArgumentTypes...), ArgumentTypes... arguments);

	void Invoke();
	void Invoke(ParameterTypes... parameters);

private:

	template<unsigned... Sequence, typename Context>
	std::function<ReturnType(ParameterTypes...)> Bind(
		std::index_sequence<Sequence...>,
		ReturnType(Context::*callback)(ParameterTypes...),
		Context* context
	);

	template<unsigned... Sequence>
	std::function<ReturnType(ParameterTypes...)> Bind(
		std::index_sequence<Sequence...>,
		ReturnType(*callback)(ParameterTypes...)
	);

};

template<typename ReturnType, typename ... ParameterTypes>
template<typename Context>
void Handler<ReturnType, ParameterTypes...>::Bind(
	ReturnType (Context::* callback)(void), 
	Context * context
) {
	customCallbacks.push_back(std::bind(callback, context));
}

template<typename ReturnType, typename ... ParameterTypes>
template<typename Context>
void Handler<ReturnType, ParameterTypes...>::Bind(
	ReturnType (Context::* callback)(void) const, 
	Context * context
) {
	customCallbacks.push_back(std::bind(callback, context));
}

template<typename ReturnType, typename ...ParameterTypes>
void Handler<ReturnType, ParameterTypes...>::Bind(ReturnType (*callback)(void)) {
	customCallbacks.push_back(callback);
}

template<typename ReturnType, typename ...ParameterTypes>
template<typename Context>
void Handler<ReturnType, ParameterTypes...>::Bind(
	ReturnType (Context::* callback)(ParameterTypes...), 
	Context * context
) {
	standardCallbacks.push_back(Bind(
		std::make_index_sequence<sizeof...(ParameterTypes)>{},
		callback,
		context
	));
}

template<typename ReturnType, typename ...ParameterTypes>
template<typename Context>
void Handler<ReturnType, ParameterTypes...>::Bind(
	ReturnType (Context::* callback)(ParameterTypes...) const, 
	Context * context
) {
	standardCallbacks.push_back(Bind(
		std::make_index_sequence<sizeof...(ParameterTypes)>{},
		callback,
		context
	));
}

template<typename ReturnType, typename ...ParameterTypes>
void Handler<ReturnType, ParameterTypes...>::Bind(
	ReturnType(*callback)(ParameterTypes...)
) {
	standardCallbacks.push_back(Bind(
		std::make_index_sequence<sizeof...(ParameterTypes)>{}, 
		callback
	));
}

template<typename ReturnType, typename ...ParameterTypes>
template<typename Context, typename ...ArgumentTypes>
void Handler<ReturnType, ParameterTypes...>::Bind(
	ReturnType (Context::* callback)(ArgumentTypes...), 
	Context * context, 
	ArgumentTypes... arguments
) {
	customCallbacks.push_back(std::bind(callback, context, arguments...));
}

template<typename ReturnType, typename ...ParameterTypes>
template<typename Context, typename ...ArgumentTypes>
void Handler<ReturnType, ParameterTypes...>::Bind(
	ReturnType (Context::* callback)(ArgumentTypes...) const, 
	Context * context, 
	ArgumentTypes... arguments
) {
	customCallbacks.push_back(std::bind(callback, context, arguments...));
}

template<typename ReturnType, typename ...ParameterTypes>
template<typename ...ArgumentTypes>
void Handler<ReturnType, ParameterTypes...>::Bind(
	ReturnType (*callback)(ArgumentTypes...), 
	ArgumentTypes... arguments
) {
	customCallbacks.push_back(std::bind(callback, arguments...));
}

template<typename ReturnType, typename ...ParameterTypes>
void Handler<ReturnType, ParameterTypes...>::Invoke() {
	for (auto& callback : customCallbacks)
		callback();
}

template<typename ReturnType, typename ...ParameterTypes>
void Handler<ReturnType, ParameterTypes...>::Invoke(ParameterTypes ...parameters) {
	for (auto& callback : customCallbacks)
		callback();
	for (auto& callback : standardCallbacks)
		callback(parameters...);
}

template<typename ReturnType, typename ...ParameterTypes>
template<unsigned ...Sequence, typename Context>
std::function<ReturnType(ParameterTypes...)> Handler<ReturnType, ParameterTypes...>::Bind(
	std::index_sequence<Sequence...>, 
	ReturnType(Context::* callback)(ParameterTypes...),
	Context* context
) {
	return std::bind(callback, context, variadic_placeholder<Sequence>{}...);
}

template<typename ReturnType, typename ...ParameterTypes>
template<unsigned ...Sequence>
std::function<ReturnType(ParameterTypes...)> Handler<ReturnType, ParameterTypes...>::Bind(
	std::index_sequence<Sequence...>, 
	ReturnType(*callback)(ParameterTypes...)
) {
	return std::bind(callback, variadic_placeholder<Sequence>{}...);
}

#endif
