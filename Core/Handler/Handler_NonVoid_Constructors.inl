#include "Handler.h"

template<typename ReturnType, typename ... ParameterTypes>
Handler<ReturnType, ParameterTypes...>::Handler() {}

template<typename ReturnType, typename ... ParameterTypes>
Handler<ReturnType, ParameterTypes...>::Handler(std::nullptr_t) {
	UnbindAll();
}


template<typename ReturnType, typename ...ParameterTypes>
template<typename Context>
Handler<ReturnType, ParameterTypes...>::Handler(
	ReturnType(Context::* callback)(ParameterTypes...),
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
Handler<ReturnType, ParameterTypes...>::Handler(
	ReturnType(Context::* callback)(ParameterTypes...) const,
	Context * context
) {
	standardCallbacks.push_back(Bind(
		std::make_index_sequence<sizeof...(ParameterTypes)>{},
		callback,
		context
	));
}

template<typename ReturnType, typename ...ParameterTypes>
template<typename MethodContext, typename Context>
Handler<ReturnType, ParameterTypes...>::Handler(
	ReturnType(MethodContext::* callback)(ParameterTypes...),
	Context * context
) {
	standardCallbacks.push_back(Bind(
		std::make_index_sequence<sizeof...(ParameterTypes)>{},
		callback,
		static_cast<MethodContext*>(context)
	));
}

template<typename ReturnType, typename ...ParameterTypes>
template<typename MethodContext, typename Context>
Handler<ReturnType, ParameterTypes...>::Handler(
	ReturnType(MethodContext::* callback)(ParameterTypes...) const,
	Context * context
) {
	standardCallbacks.push_back(Bind(
		std::make_index_sequence<sizeof...(ParameterTypes)>{},
		callback,
		static_cast<MethodContext*>(context)
	));
}

template<typename ReturnType, typename ...ParameterTypes>
Handler<ReturnType, ParameterTypes...>::Handler(std::function<ReturnType(ParameterTypes...)> callback) {
	customCallbacks.push_back(callback);
}

template<typename ReturnType, typename ... ParameterTypes>
template<typename Context>
Handler<ReturnType, ParameterTypes...>::Handler(
	ReturnType(Context::* callback)(void),
	Context * context
) {
	customCallbacks.push_back(std::bind(callback, context));
}

template<typename ReturnType, typename ... ParameterTypes>
template<typename Context>
Handler<ReturnType, ParameterTypes...>::Handler(
	ReturnType(Context::* callback)(void) const,
	Context * context
) {
	customCallbacks.push_back(std::bind(callback, context));
}

template<typename ReturnType, typename ... ParameterTypes>
template<typename MethodContext, typename Context>
Handler<ReturnType, ParameterTypes...>::Handler(
	ReturnType(MethodContext::* callback)(void),
	Context * context
) {
	customCallbacks.push_back(std::bind(callback, static_cast<MethodContext*>(context)));
}

template<typename ReturnType, typename ... ParameterTypes>
template<typename MethodContext, typename Context>
Handler<ReturnType, ParameterTypes...>::Handler(
	ReturnType(MethodContext::* callback)(void) const,
	Context * context
) {
	customCallbacks.push_back(std::bind(callback, static_cast<MethodContext*>(context)));
}

template<typename ReturnType, typename ...ParameterTypes>
Handler<ReturnType, ParameterTypes...>::Handler(std::function<ReturnType(void)> callback) {
	customCallbacks.push_back(callback);
}

template<typename ReturnType, typename ...ParameterTypes>
template<typename Context, typename ...ArgumentTypes>
Handler<ReturnType, ParameterTypes...>::Handler(
	ReturnType(Context::* callback)(ArgumentTypes...),
	Context * context,
	ArgumentTypes... arguments
) {
	customCallbacks.push_back(std::bind(callback, context, arguments...));
}

template<typename ReturnType, typename ...ParameterTypes>
template<typename Context, typename ...ArgumentTypes>
Handler<ReturnType, ParameterTypes...>::Handler(
	ReturnType(Context::* callback)(ArgumentTypes...) const,
	Context * context,
	ArgumentTypes... arguments
) {
	customCallbacks.push_back(std::bind(callback, context, arguments...));
}

template<typename ReturnType, typename ...ParameterTypes>
template<typename MethodContext, typename Context, typename ...ArgumentTypes>
Handler<ReturnType, ParameterTypes...>::Handler(
	ReturnType(MethodContext::* callback)(ArgumentTypes...),
	Context * context,
	ArgumentTypes... arguments
) {
	customCallbacks.push_back(std::bind(callback, static_cast<MethodContext*>(context), arguments...));
}

template<typename ReturnType, typename ...ParameterTypes>
template<typename MethodContext, typename Context, typename ...ArgumentTypes>
Handler<ReturnType, ParameterTypes...>::Handler(
	ReturnType(MethodContext::* callback)(ArgumentTypes...) const,
	Context * context,
	ArgumentTypes... arguments
) {
	customCallbacks.push_back(std::bind(callback, static_cast<MethodContext*>(context), arguments...));
}

template<typename ReturnType, typename ...ParameterTypes>
template<typename ...ArgumentTypes>
Handler<ReturnType, ParameterTypes...>::Handler(
	ReturnType(*callback)(ArgumentTypes...),
	ArgumentTypes... arguments
) {
	customCallbacks.push_back(std::bind(callback, arguments...));
}