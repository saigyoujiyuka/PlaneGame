#include "StdAfx.h"
#include "GiftWeapon.h"
#include "resource.h"

CImageList CGiftWeapon::m_Images;

CGiftWeapon::CGiftWeapon(void)
{
	//随机确定X位置
	m_ptPos.x = rand() % (GAME_WIDTH - GIFTWP_HEIGHT) + 1;

	//随机确定图像索引
	//m_nImgIndex = rand()%2;

	//根据图像索引确定方向
	m_nMotion = 1;
	m_ptPos.y = -GIFTWP_HEIGHT;
	//if (m_nImgIndex%2!=0)//如果是图像索引是偶数
	//{
	//	m_nMotion=0;
	//	m_ptPos.y = GAME_HEIGHT+ENEMY_HEIGHT;
	//}
	//随机确定速度
	m_V = rand() % 6 + 2;

	//m_nWait = 0;
}

CGiftWeapon::~CGiftWeapon(void)
{
}
BOOL CGiftWeapon::LoadImage()
{
	return CGameObject::LoadImage(m_Images, IDB_WEAPON, RGB(0, 0, 0), 50, 50, 1);
}
BOOL CGiftWeapon::Draw(CDC* pDC, BOOL bPause)
{
	if (!bPause)
	{
		m_ptPos.y = m_ptPos.y + m_nMotion * m_V;
	}

	if (m_ptPos.y > GAME_HEIGHT + GIFTWP_HEIGHT)
		return FALSE;
	if (m_ptPos.y < -GIFTWP_HEIGHT)
		return FALSE;

	m_Images.Draw(pDC, 0, m_ptPos, ILD_TRANSPARENT);

	return TRUE;
}
