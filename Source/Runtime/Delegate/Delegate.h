#pragma once

#include <vector>

template <class... Args>
class TSingleDelegateNoReturn {
protected:
	void (*DelegateFunction)(Args... args) = nullptr;
public:
	void Bind(void (*func)(Args... args));
	void Broadcast(Args... args);
};

template <typename Return, class... Args>
class TSingleDelegateReturn {
protected:
	Return (*DelegateFunction)(Args... args) = nullptr;
public:
	void Bind(Return (*func)(Args... args));
	Return Broadcast(Args... args);
};

template <class... Args>
class TMulticastDelegate {
protected:
	struct _DelegateContainer
	{
		void (*Function)(Args... args) = nullptr;
		_DelegateContainer(void (*func)(Args... args))
		{
			Function = func;
		}
	};

	std::vector<_DelegateContainer> DelegateFunctions;
public:

	void Add(void(*func)(Args... args))
	{
		DelegateFunctions.push_back(_DelegateContainer(func));
	}

	void Broadcast(Args... args)
	{
		for (auto delegate : DelegateFunctions)
		{
			delegate.Function(args...);
		}
	}
};