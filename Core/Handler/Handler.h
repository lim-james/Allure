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

	// constructors
	// standard member methods
	template<typename Context>
	Handler(ReturnType (Context::*callback)(ParameterTypes...), Context* context);
	template<typename Context>
	Handler(ReturnType (Context::*callback)(ParameterTypes...) const, Context* context);
	template<typename MethodContext, typename Context>
	Handler(ReturnType (MethodContext::*callback)(ParameterTypes...), Context* context);
	template<typename MethodContext, typename Context>
	Handler(ReturnType (MethodContext::*callback)(ParameterTypes...) const, Context* context);
	// standard non member methods
	Handler(std::function<ReturnType(ParameterTypes...)> callback);

	// void member methods
	template<typename Context>
	Handler(ReturnType (Context::*callback)(void), Context* context);
	template<typename Context>
	Handler(ReturnType (Context::*callback)(void) const, Context* context);
	template<typename MethodContext, typename Context>
	Handler(ReturnType (MethodContext::*callback)(void), Context* context);
	template<typename MethodContext, typename Context>
	Handler(ReturnType (MethodContext::*callback)(void) const, Context* context);
	// void non members methods
	Handler(std::function<ReturnType(void)> callback);

	// custom members methods
	template<typename Context, typename ... ArgumentTypes>
	Handler(ReturnType (Context::*callback)(ArgumentTypes...), Context* context, ArgumentTypes... arguments);
	template<typename Context, typename ... ArgumentTypes>
	Handler(ReturnType (Context::*callback)(ArgumentTypes...) const, Context* context, ArgumentTypes... arguments);
	template<typename MethodContext, typename Context, typename ... ArgumentTypes>
	Handler(ReturnType (MethodContext::*callback)(ArgumentTypes...), Context* context, ArgumentTypes... arguments);
	template<typename MethodContext, typename Context, typename ... ArgumentTypes>
	Handler(ReturnType (MethodContext::*callback)(ArgumentTypes...) const, Context* context, ArgumentTypes... arguments);
	// custom non member methods
	template<typename ... ArgumentTypes>
	Handler(ReturnType (*callback)(ArgumentTypes...), ArgumentTypes... arguments);

	// standard member methods
	template<typename Context>
	void Bind(ReturnType (Context::*callback)(ParameterTypes...), Context* context);
	template<typename Context>
	void Bind(ReturnType (Context::*callback)(ParameterTypes...) const, Context* context);
	template<typename MethodContext, typename Context>
	void Bind(ReturnType (MethodContext::*callback)(ParameterTypes...), Context* context);
	template<typename MethodContext, typename Context>
	void Bind(ReturnType (MethodContext::*callback)(ParameterTypes...) const, Context* context);
	// standard non member methods
	void Bind(std::function<ReturnType(ParameterTypes...)> callback);

	// void member methods
	template<typename Context>
	void Bind(ReturnType (Context::*callback)(void), Context* context);
	template<typename Context>
	void Bind(ReturnType (Context::*callback)(void) const, Context* context);
	template<typename MethodContext, typename Context>
	void Bind(ReturnType (MethodContext::*callback)(void), Context* context);
	template<typename MethodContext, typename Context>
	void Bind(ReturnType (MethodContext::*callback)(void) const, Context* context);
	// void non members methods
	void Bind(std::function<ReturnType(void)> callback);

	// custom members methods
	template<typename Context, typename ... ArgumentTypes>
	void Bind(ReturnType (Context::*callback)(ArgumentTypes...), Context* context, ArgumentTypes... arguments);
	template<typename Context, typename ... ArgumentTypes>
	void Bind(ReturnType (Context::*callback)(ArgumentTypes...) const, Context* context, ArgumentTypes... arguments);
	template<typename MethodContext, typename Context, typename ... ArgumentTypes>
	void Bind(ReturnType (MethodContext::*callback)(ArgumentTypes...), Context* context, ArgumentTypes... arguments);
	template<typename MethodContext, typename Context, typename ... ArgumentTypes>
	void Bind(ReturnType (MethodContext::*callback)(ArgumentTypes...) const, Context* context, ArgumentTypes... arguments);
	// custom non member methods
	template<typename ... ArgumentTypes>
	void Bind(ReturnType (*callback)(ArgumentTypes...), ArgumentTypes... arguments);

	void UnbindAll();

	// invoke without return value
	void Invoke(ParameterTypes... parameters) const;
	void Invoke() const;

	// invoke returning result
	std::vector<ReturnType> InvokeReturn(ParameterTypes... parameters) const;
	std::vector<ReturnType> InvokeReturn() const;

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

	// constructors
	// void member methods
	template<typename Context>
	Handler(ReturnType (Context::*callback)(void), Context* context);
	template<typename Context>
	Handler(ReturnType (Context::*callback)(void) const, Context* context);
	template<typename MethodContext, typename Context>
	Handler(ReturnType (MethodContext::*callback)(void), Context* context);
	template<typename MethodContext, typename Context>
	Handler(ReturnType (MethodContext::*callback)(void) const, Context* context);
	// void non member methods
	Handler(std::function<ReturnType(void)> callback);

	// custom member methods
	template<typename Context, typename ... ArgumentTypes>
	Handler(ReturnType(Context::*callback)(ArgumentTypes...), Context* context, ArgumentTypes... arguments);
	template<typename Context, typename ... ArgumentTypes>
	Handler(ReturnType(Context::*callback)(ArgumentTypes...) const, Context* context, ArgumentTypes... arguments);
	template<typename MethodContext, typename Context, typename ... ArgumentTypes>
	Handler(ReturnType(MethodContext::*callback)(ArgumentTypes...), Context* context, ArgumentTypes... arguments);
	template<typename MethodContext, typename Context, typename ... ArgumentTypes>
	Handler(ReturnType(MethodContext::*callback)(ArgumentTypes...) const, Context* context, ArgumentTypes... arguments);
	// custom non member methods
	template<typename ... ArgumentTypes>
	Handler(ReturnType (*callback)(ArgumentTypes...), ArgumentTypes... arguments);

	// void member methods
	template<typename Context>
	void Bind(ReturnType (Context::*callback)(void), Context* context);
	template<typename Context>
	void Bind(ReturnType (Context::*callback)(void) const, Context* context);
	template<typename MethodContext, typename Context>
	void Bind(ReturnType (MethodContext::*callback)(void), Context* context);
	template<typename MethodContext, typename Context>
	void Bind(ReturnType (MethodContext::*callback)(void) const, Context* context);
	// void non members methods
	void Bind(std::function<ReturnType(void)> callback);

	// custom member methods
	template<typename Context, typename ... ArgumentTypes>
	void Bind(ReturnType (Context::*callback)(ArgumentTypes...), Context* context, ArgumentTypes... arguments);
	template<typename Context, typename ... ArgumentTypes>
	void Bind(ReturnType (Context::*callback)(ArgumentTypes...) const, Context* context, ArgumentTypes... arguments);
	template<typename MethodContext, typename Context, typename ... ArgumentTypes>
	void Bind(ReturnType (MethodContext::*callback)(ArgumentTypes...), Context* context, ArgumentTypes... arguments);
	template<typename MethodContext, typename Context, typename ... ArgumentTypes>
	void Bind(ReturnType (MethodContext::*callback)(ArgumentTypes...) const, Context* context, ArgumentTypes... arguments);
	// custom non member methods
	template<typename ... ArgumentTypes>
	void Bind(ReturnType (*callback)(ArgumentTypes...), ArgumentTypes... arguments);

	void UnbindAll();

	// invoke without return values
	void Invoke() const;

	// invoke with return values
	std::vector<ReturnType> InvokeReturn() const;

	void operator=(std::nullptr_t);
	void operator=(Handler<ReturnType, void> const& rhs);

};

#include "Handler_NonVoid_Constructors.inl"
#include "Handler_NonVoid.inl"
#include "Handler_Void_Constructors.inl"
#include "Handler_Void.inl"

#endif
