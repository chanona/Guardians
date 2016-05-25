#pragma once
#include "GameObject.h"

#define TEXTURE_MANAGER CTextureManager::GetInstance();

class CTextureManager
{
public:
	map<string, CTexture*> m_mapTexture;

	CTextureManager();
	~CTextureManager();

	static CTextureManager* GetInstance()
	{
		static CTextureManager instance;
		return &instance;
	}

	const map<string, CTexture*>& GetTextureMap() { return m_mapTexture; }

	CTexture* FindTexture(string id);
	void	DeleteTexture(string id);
	void	DeleteTexture(CTexture* pTexture);
	void	ShutDown();
};