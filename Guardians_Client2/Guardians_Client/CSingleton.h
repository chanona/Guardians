#pragma once


template <class T>
class CSingleton
{
public :
	T* GetInstance()
	{
		static T instnace;
		return &T;
	}
};