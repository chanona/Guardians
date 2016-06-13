#ifndef Contant_h__
#define Contant_h__
/*!
 * \file Contant.h
 *
 * \author Administrator
 * \date 9월 2015
 *
 * 상수값을 보관한다.
 */

const _ushort WINSIZEX = 800;
const _ushort WINSIZEY = 600;
const _ushort VTXCNTX = 129;
const _ushort VTXCNTZ = 129;
const _ushort VTXITV = 5;

const _vec3 g_vLook = _vec3(0.f, 0.f, -1.f);

enum RESOURCETYPE {RESOURCE_STATIC, RESOURCE_LOGO, RESOURCE_STAGE, RESOURCE_END};


#endif // Contant_h__
