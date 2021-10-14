// Todo
// ...
#pragma once

// Singleton pattern.
// The class that want to use singleton mode should inherit this class, 
// and transport self as the template parameter <T>
// ...
template<typename T>
class Singleton
{

public:

	static T& Instance()
	{
		static T* instance_ = new T();
		return *instance_;
	}

protected:

	Singleton() {}
};
