#include "StdAfx.h"
#include "Bomb.h"
#include "resource.h"


CImageList CBomb::m_Images;


CBomb::CBomb(int x,int y):CGameObject(x,y),direct(0)
{

}
CBomb::CBomb(int x,int y, int dr):CGameObject(x,y),direct(dr)
{

}
CBomb::~CBomb(void)
{
}
BOOL CBomb::LoadImage()
{
	return CGameObject::LoadImage(m_Images,IDB_BOMB1,RGB(0,0,0),20,50,1);
}
BOOL CBomb::Draw(CDC* pDC,BOOL bPause)
{
	if(!bPause)
	{
		if(direct==0)
			m_ptPos.y = m_ptPos.y - 8;
		if (direct == 1)
		{
			m_ptPos.y = m_ptPos.y - 8;
			m_ptPos.x = m_ptPos.x + 2;
		}
		if (direct == -1)
		{
			m_ptPos.y = m_ptPos.y - 8;
			m_ptPos.x = m_ptPos.x - 2;
		}
		if (direct == 2)
		{
			m_ptPos.y = m_ptPos.y - 8;
			m_ptPos.x = m_ptPos.x + 5;
		}
		if (direct == -2)
		{
			m_ptPos.y = m_ptPos.y - 8;
			m_ptPos.x = m_ptPos.x - 5;
		}
		//m_ptPos.x = m_ptPos.x + 4;
	}

	if(m_ptPos.y < -BOMB_HEIGHT)
		return FALSE;
	if (m_ptPos.x < 0)
		return FALSE;
	if (m_ptPos.x > GAME_WIDTH + BOMB_WIDTH)
		return FALSE;


	m_Images.Draw(pDC,0,m_ptPos,ILD_TRANSPARENT);

	return TRUE;
}