/*!
 * \file Component.h
 * \date 2015/04/03 18:15
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
#ifndef Component_h__
#define Component_h__

#include "Base.h"

BEGIN(Engine)

class  CComponent : public Engine::CBase
{
protected:
	explicit CComponent(void);
	virtual ~CComponent(void);
public:
	virtual _int Update(const _float& fTimeDelta) {return 0;}
public:
	virtual _ulong Release(void);
};

END

#endif // Component_h__