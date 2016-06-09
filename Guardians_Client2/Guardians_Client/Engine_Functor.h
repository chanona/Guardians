/*!
 * \file Engine_Functor.h
 * \date 2015/04/04 23:39
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/
#ifndef Engine_Functor_h__
#define Engine_Functor_h__

namespace Engine
{
	class CTagFinder
	{
	public:
		explicit CTagFinder(const TCHAR* pTag) : m_pTag(pTag) {}
		~CTagFinder(void) {}
	public:
		template <typename T> bool operator () (T& Pair)
		{
			int iResult = lstrcmp(m_pTag, Pair.first);

			if(0 == iResult)
				return true;
			return false;
		}
	private:
		const TCHAR*				m_pTag;
	};

	class CRelease_Pair
	{
	public:
		explicit CRelease_Pair() {}
		~CRelease_Pair(void) {}
	public:
		template <typename T>		
		void operator () (T& Pair)
		{
			if(NULL != Pair.second)
			{
				Pair.second->Release();
				Pair.second = NULL;
			}
		}
	};

	class CRelease_Single
	{
	public:
		explicit CRelease_Single() {}
		~CRelease_Single(void) {}
	public:
		template <typename T>		
		void operator () (T& Pointer)
		{
			if(NULL != Pointer)
			{
				Pointer->Release();
				Pointer = NULL;
			}
		}
	};
}

#endif // Engine_Functor_h__