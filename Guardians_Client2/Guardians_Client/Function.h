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

// ��ü���� �׻� �ڽ��� ���۷���ī��Ʈ�� ������ �ִ´�.
// Release�Լ��� ���ϰ� : ������� �õ��� �� �����Ȱ���(���۷���ī��Ʈ)�� �����Ѵ�.
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
