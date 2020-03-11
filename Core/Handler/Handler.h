#ifndef HANDLER_H
#define HANDLER_H

#include "variadic_placeholder.h"

#include <vector>

template<typename ReturnType, typename ... ParameterTypes>
class Handler {

private:

	std::vector<std::function<ReturnType(ParameterTypes...)>> standardCallbacks;
	std::vector<std::function<ReturnType(void)>> customCallbacks;

public:

	Handler();
	Handler(std::nullptr_t);

	// standard methods
	template<typename Context>
	void Bind(ReturnType (Context::*callback)(ParameterTypes...), Context* context);
	template<typename Context>
	void Bind(ReturnType (Context::*callback)(ParameterTypes...) const, Context* context);
	template<typename MethodContext, typename Context>
	void Bind(ReturnType (MethodContext::*callback)(ParameterTypes...), Context* context);
	template<typename MethodContext, typename Context>
	void Bind(ReturnType (MethodContext::*callback)(ParameterTypes...) const, Context* context);
	// non members
	void Bind(ReturnType (*callback)(ParameterTypes...));
	void Bind(std::function<ReturnType(ParameterTypes...)> callback);

	// void class methods
	template<typename Context>
	void Bind(ReturnType (Context::*callback)(void), Context* context);
	template<typename Context>
	void Bind(ReturnType (Context::*callback)(void) const, Context* context);
	template<typename MethodContext, typename Context>
	void Bind(ReturnType (MethodContext::*callback)(void), Context* context);
	template<typename MethodContext, typename Context>
	void Bind(ReturnType (MethodContext::*callback)(void) const, Context* context);
	// non members
	void Bind(ReturnType (*callback)(void));
	void Bind(std::function<ReturnType(void)> callback);

	// custom methods
	template<typename Context, typename ... ArgumentTypes>
	void Bind(ReturnType (Context::*callback)(ArgumentTypes...), Context* context, ArgumentTypes... arguments);
	template<typename Context, typename ... ArgumentTypes>
	void Bind(ReturnType (Context::*callback)(ArgumentTypes...) const, Context* context, ArgumentTypes... arguments);
	template<typename MethodContext, typename Context, typename ... ArgumentTypes>
	void Bind(ReturnType (MethodContext::*callback)(ArgumentTypes...), Context* context, ArgumentTypes... arguments);
	template<typename MethodContext, typename Context, typename ... ArgumentTypes>
	void Bind(ReturnType (MethodContext::*callback)(ArgumentTypes...) const, Context* context, ArgumentTypes... arguments);
	// non member
	template<typename ... ArgumentTypes>
	void Bind(ReturnType (*callback)(ArgumentTypes...), ArgumentTypes... arguments);

	void UnbindAll();

	void operator()(ParameterTypes... parameters) const;
	void operator()() const;

	void operator=(std::nullptr_t);

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

template<typename ReturnType>
class Handler <ReturnType, void> {

private:

	std::vector<std::function<ReturnType(void)>> customCallbacks;

public:

	Handler();
	Handler(std::nullptr_t);

	// void class methods
	template<typename Context>
	void Bind(ReturnType (Context::*callback)(void), Context* context);
	template<typename Context>
	void Bind(ReturnType (Context::*callback)(void) const, Context* context);
	template<typename MethodContext, typename Context>
	void Bind(ReturnType (MethodContext::*callback)(void), Context* context);
	template<typename MethodContext, typename Context>
	void Bind(ReturnType (MethodContext::*callback)(void) const, Context* context);
	// non member
	void Bind(ReturnType (*callback)(void));
	void Bind(std::function<ReturnType(void)> callback);

	// custom methods
	template<typename Context, typename ... ArgumentTypes>
	void Bind(ReturnType (Context::*callback)(ArgumentTypes...), Context* context, ArgumentTypes... arguments);
	template<typename Context, typename ... ArgumentTypes>
	void Bind(ReturnType (Context::*callback)(ArgumentTypes...) const, Context* context, ArgumentTypes... arguments);
	template<typename MethodContext, typename Context, typename ... ArgumentTypes>
	void Bind(ReturnType (MethodContext::*callback)(ArgumentTypes...), Context* context, ArgumentTypes... arguments);
	template<typename MethodContext, typename Context, typename ... ArgumentTypes>
	void Bind(ReturnType (MethodContext::*callback)(ArgumentTypes...) const, Context* context, ArgumentTypes... arguments);
	// non member
	template<typename ... ArgumentTypes>
	void Bind(ReturnType (*callback)(ArgumentTypes...), ArgumentTypes... arguments);

	void UnbindAll();

	void operator()() const;

	void operator=(std::nullptr_t);

};

#include "Handler_NonVoid.inl"
#include "Handler_Void.inl"

#endif
