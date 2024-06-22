#include "Delegate.h"

template<class ...Args>
inline void TSingleDelegateNoReturn<Args...>::Bind(void(*func)(Args...args))
{
	DelegateFunction = func;
}

template<class ...Args>
void TSingleDelegateNoReturn<Args...>::Broadcast(Args ...args)
{
	DelegateFunction(args...);
}

template<typename Return, class ...Args>
inline void TSingleDelegateReturn<Return, Args...>::Bind(Return(*func)(Args...args))
{
	DelegateFunction = func;
}

template<typename Return, class ...Args>
Return TSingleDelegateReturn<Return, Args...>::Broadcast(Args ...args)
{
	return DelegateFunction(args...);
}