#pragma once
#include "NonCopyable.h"

template<typename T> 
class Singleton : NonCopyable
{
protected:
	static T* instance;

public:
	static T& getInstance()
	{
		return *instance;
	}

	static T* getInstancePtr()
	{
		return instance;
	}

	Singleton()
	{
		instance = static_cast<T*>(this);
	}

	~Singleton()
	{
		instance = nullptr;
	}
};