#include "StdAfx.h"
#include "Bomb1.h"
#include "resource.h"


CImageList CBomb1::m_Images;


CBomb1::CBomb1(int x, int y) :CGameObject(x, y), direct(0)
{

}
CBomb1::CBomb1(int x, int y, int dr) : CGameObject(x, y), direct(dr)
{

}
CBomb1::~CBomb1(void)
{
}
BOOL CBomb1::LoadImage()
{
	return CGameObject::LoadImage(m_Images, IDB_BOMB3, RGB(0, 0, 0), 15, 50, 1);
}
BOOL CBomb1::Draw(CDC* pDC, BOOL bPause)
{
	if (!bPause)
	{
		if (direct == 0)
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

	if (m_ptPos.y < -BOMB_HEIGHT)
		return FALSE;
	if (m_ptPos.x < 0)
		return FALSE;
	if (m_ptPos.x > GAME_WIDTH + BOMB_WIDTH)
		return FALSE;


	m_Images.Draw(pDC, 0, m_ptPos, ILD_TRANSPARENT);

	return TRUE;
}