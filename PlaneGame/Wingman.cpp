#include "stdafx.h"
#include "Wingman.h"
#include "resource.h"

CImageList CWingman::m_Images;

CWingman::~CWingman(void)
{
}
CWingman::CWingman(void) :m_nHorMotion(0), m_nVerMotion(0)
{
	m_nWait = 0;
	life = 10;
}
CWingman::CWingman(int x,int y) :CGameObject(x,y),m_nHorMotion(0),m_nVerMotion(0),m_nWait(0)
{
	life = 10;
}



BOOL CWingman::Fired()
{
	if (m_nWait == 0)
		return TRUE;
	else
		return FALSE;

}
BOOL CWingman::LoadImage()
{
	return CGameObject::LoadImage(m_Images, IDB_WINGMAN, RGB(0, 0, 0), 30, 40, 1);
}
BOOL CWingman::Draw(CDC* pDC, BOOL bPause)
{
	m_nWait++;
	if (m_nWait>8)
		m_nWait = 0;


	if (!bPause)
	{
		m_ptPos.x = m_ptPos.x + m_nHorMotion * 8;
		m_ptPos.y = m_ptPos.y - m_nVerMotion * 8;
	}

	if (m_ptPos.x >= GAME_WIDTH - WM_WIDTH)
		m_ptPos.x = GAME_WIDTH - WM_WIDTH;
	if (m_ptPos.x <= 0)
		m_ptPos.x = 0;

	if (m_ptPos.y <= 0)
		m_ptPos.y = 0;

	if (m_ptPos.y >= GAME_HEIGHT - WM_HEIGHT)
		m_ptPos.y = GAME_HEIGHT - WM_HEIGHT;


	m_Images.Draw(pDC, 0, m_ptPos, ILD_TRANSPARENT);
	return TRUE;
}
void CWingman::SetPoint(int x,int y)
{
	m_ptPos.x = x;
	m_ptPos.y = y;
}