#include "stdafx.h"
#include "TextureManager.h"
#include "Memory.h"

CTextureManager::CTextureManager()
{
	m_mapTexture.clear();
}

CTextureManager::~CTextureManager()
{

}

CTexture* CTextureManager::FindTexture(string id)
{
	auto pTexture = m_mapTexture.find(id);

	return pTexture->second;
}

void CTextureManager::DeleteTexture(string id)
{
}

void CTextureManager::DeleteTexture(CTexture* pTexture)
{

}

void CTextureManager::ShutDown()
{
	for (auto tex : m_mapTexture)
	{
		Memory::SAFE_DELETE(tex.second);
	}
	m_mapTexture.clear();
}

