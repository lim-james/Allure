#include "Handler.h"

template<typename ReturnType>
template<typename Context>
void Handler<ReturnType, void>::Bind(
	ReturnType(Context::* callback)(void),
	Context * context
) {
	customCallbacks.push_back(std::bind(callback, context));
}

template<typename ReturnType>
template<typename Context>
void Handler<ReturnType, void>::Bind(
	ReturnType(Context::* callback)(void) const,
	Context * context
) {
	customCallbacks.push_back(std::bind(callback, context));
}

template<typename ReturnType>
template<typename MethodContext, typename Context>
void Handler<ReturnType, void>::Bind(
	ReturnType(MethodContext::* callback)(void),
	Context * context
) {
	customCallbacks.push_back(std::bind(callback, static_cast<MethodContext*>(context)));
}

template<typename ReturnType>
template<typename MethodContext, typename Context>
void Handler<ReturnType, void>::Bind(
	ReturnType(MethodContext::* callback)(void) const,
	Context * context
) {
	customCallbacks.push_back(std::bind(callback, static_cast<MethodContext*>(context)));
}

template<typename ReturnType>
template<typename Context, typename ...ArgumentTypes>
void Handler<ReturnType, void>::Bind(
	ReturnType(Context::* callback)(ArgumentTypes...),
	Context * context,
	ArgumentTypes... arguments
) {
	customCallbacks.push_back(std::bind(callback, context, arguments...));
}

template<typename ReturnType>
template<typename Context, typename ...ArgumentTypes>
void Handler<ReturnType, void>::Bind(
	ReturnType(Context::* callback)(ArgumentTypes...) const,
	Context * context,
	ArgumentTypes... arguments
) {
	customCallbacks.push_back(std::bind(callback, context, arguments...));
}

template<typename ReturnType>
template<typename MethodContext, typename Context, typename ...ArgumentTypes>
void Handler<ReturnType, void>::Bind(
	ReturnType(MethodContext::* callback)(ArgumentTypes...),
	Context * context,
	ArgumentTypes... arguments
) {
	customCallbacks.push_back(std::bind(callback, static_cast<MethodContext*>(context), arguments...));
}

template<typename ReturnType>
template<typename MethodContext, typename Context, typename ...ArgumentTypes>
void Handler<ReturnType, void>::Bind(
	ReturnType(MethodContext::* callback)(ArgumentTypes...) const,
	Context * context,
	ArgumentTypes... arguments
) {
	customCallbacks.push_back(std::bind(callback, static_cast<MethodContext*>(context), arguments...));
}

template<typename ReturnType>
template<typename ...ArgumentTypes>
void Handler<ReturnType, void>::Bind(
	ReturnType(*callback)(ArgumentTypes...),
	ArgumentTypes... arguments
) {
	customCallbacks.push_back(std::bind(callback, arguments...));
}

template<typename ReturnType>
void Handler<ReturnType, void>::UnbindAll() {
	customCallbacks.clear();
}

template<typename ReturnType>
void Handler<ReturnType, void>::Invoke() const {
	for (auto& callback : customCallbacks)
		callback();
}

template<typename ReturnType>
std::vector<ReturnType> Handler<ReturnType, void>::InvokeReturn() const {
	std::vector<ReturnType> result;

	for (auto& callback : customCallbacks)
		result.push_back(callback());

	return result;
}

template<typename ReturnType>
void Handler<ReturnType, void>::operator=(std::nullptr_t) {
	UnbindAll();
}

template<typename ReturnType>
void Handler<ReturnType, void>::operator=(Handler<ReturnType, void> const& rhs) {
	customCallbacks = rhs.customCallbacks;
}
