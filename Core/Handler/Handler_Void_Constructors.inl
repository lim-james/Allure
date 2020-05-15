#include "Handler.h"

template<typename ReturnType>
Handler<ReturnType, void>::Handler() {}

template<typename ReturnType>
Handler<ReturnType, void>::Handler(std::nullptr_t) {
	UnbindAll();
}

template<typename ReturnType>
template<typename Context>
Handler<ReturnType, void>::Handler(
	ReturnType(Context::* callback)(void),
	Context * context
) {
	customCallbacks.push_back(std::bind(callback, context));
}

template<typename ReturnType>
template<typename Context>
Handler<ReturnType, void>::Handler(
	ReturnType(Context::* callback)(void) const,
	Context * context
) {
	customCallbacks.push_back(std::bind(callback, context));
}

template<typename ReturnType>
template<typename MethodContext, typename Context>
Handler<ReturnType, void>::Handler(
	ReturnType(MethodContext::* callback)(void),
	Context * context
) {
	customCallbacks.push_back(std::bind(callback, static_cast<MethodContext*>(context)));
}

template<typename ReturnType>
template<typename MethodContext, typename Context>
Handler<ReturnType, void>::Handler(
	ReturnType(MethodContext::* callback)(void) const,
	Context * context
) {
	customCallbacks.push_back(std::bind(callback, static_cast<MethodContext*>(context)));
}

template<typename ReturnType>
Handler<ReturnType, void>::Handler(std::function<ReturnType(void)> callback) {
	customCallbacks.push_back(callback);
}

template<typename ReturnType>
template<typename Context, typename ...ArgumentTypes>
Handler<ReturnType, void>::Handler(
	ReturnType(Context::* callback)(ArgumentTypes...),
	Context * context,
	ArgumentTypes... arguments
) {
	customCallbacks.push_back(std::bind(callback, context, arguments...));
}

template<typename ReturnType>
template<typename Context, typename ...ArgumentTypes>
Handler<ReturnType, void>::Handler(
	ReturnType(Context::* callback)(ArgumentTypes...) const,
	Context * context,
	ArgumentTypes... arguments
) {
	customCallbacks.push_back(std::bind(callback, context, arguments...));
}

template<typename ReturnType>
template<typename MethodContext, typename Context, typename ...ArgumentTypes>
Handler<ReturnType, void>::Handler(
	ReturnType(MethodContext::* callback)(ArgumentTypes...),
	Context * context,
	ArgumentTypes... arguments
) {
	customCallbacks.push_back(std::bind(callback, static_cast<MethodContext*>(context), arguments...));
}

template<typename ReturnType>
template<typename MethodContext, typename Context, typename ...ArgumentTypes>
Handler<ReturnType, void>::Handler(
	ReturnType(MethodContext::* callback)(ArgumentTypes...) const,
	Context * context,
	ArgumentTypes... arguments
) {
	customCallbacks.push_back(std::bind(callback, static_cast<MethodContext*>(context), arguments...));
}

template<typename ReturnType>
template<typename ...ArgumentTypes>
Handler<ReturnType, void>::Handler(
	ReturnType(*callback)(ArgumentTypes...),
	ArgumentTypes... arguments
) {
	customCallbacks.push_back(std::bind(callback, arguments...));
}
