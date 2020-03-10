#include "Handler.hpp"

template<typename ReturnType, typename ... ParameterTypes>
Handler<ReturnType, ParameterTypes...>::Handler() {}

template<typename ReturnType, typename ... ParameterTypes>
Handler<ReturnType, ParameterTypes...>::Handler(std::nullptr_t) {
	UnbindAll();
}

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
void Handler<ReturnType, ParameterTypes...>::UnbindAll() {
	customCallbacks.clear();
	standardCallbacks.clear();
}

template<typename ReturnType, typename ...ParameterTypes>
void Handler<ReturnType, ParameterTypes...>::operator()() const {
	for (auto& callback : customCallbacks)
		callback();
}

template<typename ReturnType, typename ...ParameterTypes>
void Handler<ReturnType, ParameterTypes...>::operator()(ParameterTypes ...parameters) const {
	for (auto& callback : customCallbacks)
		callback();
	for (auto& callback : standardCallbacks)
		callback(parameters...);
}

template<typename ReturnType, typename ...ParameterTypes>
void Handler<ReturnType, ParameterTypes...>::operator=(std::nullptr_t) {
	UnbindAll();
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

