#pragma once


template <class T>
class CSingleton
{
public :
	static T* GetInstance()
	{
		static T instnace;
		return &instance;
	}
};