#include "stdafx.h"
#include "Shield.h"
#include "resource.h"


CImageList CShield::m_Images;

CShield::~CShield(void)
{
}
CShield::CShield(void)
{
	
	m_ptPos.x = 450;
	m_ptPos.y = 650;
}
void CShield::SetPoint(CPoint t)
{
	m_ptPos.x = t.x;
	m_ptPos.y = t.y;
}

BOOL CShield::LoadImage()
{
	return CGameObject::LoadImage(m_Images, IDB_SHIELD, RGB(0, 0, 0), 100, 100, 1);
}
BOOL CShield::Draw(CDC* pDC, BOOL bPause)
{



	//if (!bPause)
	//{
	//	m_ptPos.x = m_ptPos.x + m_nHorMotion * 8;
	//	m_ptPos.y = m_ptPos.y - m_nVerMotion * 8;
	//}

	//if (m_ptPos.x >= GAME_WIDTH - PLANE_WIDTH)
	//	m_ptPos.x = GAME_WIDTH - PLANE_WIDTH;
	//if (m_ptPos.x <= 0)
	//	m_ptPos.x = 0;

	//if (m_ptPos.y <= 0)
	//	m_ptPos.y = 0;

	//if (m_ptPos.y >= GAME_HEIGHT - PLANE_HEIGHT)
	//	m_ptPos.y = GAME_HEIGHT - PLANE_HEIGHT;


	m_Images.Draw(pDC, 0, m_ptPos, ILD_TRANSPARENT);
	return TRUE;
}