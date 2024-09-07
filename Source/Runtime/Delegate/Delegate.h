#pragma once

#include <vector>
#include <memory>
#include <functional>

#include "DelegateInstance.h"

template <class... Args>
class TSingleDelegateNoReturn {
protected:
	std::unique_ptr<void(Args...args)> delegatefunc;
	void (*DelegateFunction)(Args... args) = nullptr;

	DelegateInstance<void*(Args...)>* instance;
public:
	void Bind(void (*func)(Args... args));
	void Broadcast(Args... args);
};

template <typename Return, class... Args>
class TSingleDelegateReturn {
protected:
	Return (*DelegateFunction)(Args... args) = nullptr;

	DelegateInstance<Return(Args...)>* instance;
public:
	void Bind(Return (*func)(Args... args));
	Return Broadcast(Args... args);
};

template <class... Args>
class TMulticastDelegate {
protected:
	std::vector<std::function<void(Args...)>> DelegateFunctions;

	std::vector<DelegateInstance<void*(Args...)>*> instances;
public:

	// Deprecated, should never call
	void AddBind(std::function<void(Args...)> bind)
	{
		DelegateFunctions.push_back(bind);
	}

	void Add(void(*func)(Args... args))
	{
		DelegateFunctions.push_back(std::bind(func));
	}

	void AddDynamic(void(*func)(Args... args), std::nullptr_t* This)
	{
		DelegateFunctions.push_back(std::bind(func, This));
	}

	void Broadcast(Args... args)
	{
		for (auto delegate : DelegateFunctions)
		{
			delegate(args...);
		}
	}
};