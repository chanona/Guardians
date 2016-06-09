#ifndef Function_h__
#define Function_h__

template <typename T> void Safe_Delete (T& pointer)
{
	if(NULL != pointer)
	{
		delete pointer;
		pointer = NULL;
	}
}


template <typename T> void Safe_Delete_Array (T& pointer)
{
	if(NULL != pointer)
	{
		delete [] pointer;
		pointer = NULL;
	}
}

// 객체들은 항상 자신의 레퍼런스카운트를 가지고 있는다.
// Release함수의 리턴값 : 지우려고 시도할 때 참조된갯수(레퍼런스카운트)를 리턴한다.
// 0 != m_pGraphicDev->Release();
template <typename T> _ulong Safe_Release (T& pointer)
{
	_ulong	iRefCnt = 0;

	if(NULL != pointer)
	{
		iRefCnt = pointer->Release();

		if(0 == iRefCnt)
			pointer = NULL;
	}

	return iRefCnt;
}

#endif // Function_h__
