#include "stdafx.h"
#include "Skill.h"
#include "resource.h"

CImageList CSkill::m_Images;


CSkill::CSkill(int x, int y) :CGameObject(x, y)
{

}

CSkill::~CSkill(void)
{
}
BOOL CSkill::LoadImage()
{
	return CGameObject::LoadImage(m_Images, IDB_SKILL, RGB(0, 0, 0), 128, 100, 1);
}
BOOL CSkill::Draw(CDC* pDC, BOOL bPause)
{
	if (!bPause)
	{
		m_ptPos.y = m_ptPos.y - 10;
	}

	if (m_ptPos.y < -SK_HEIGHT)
		return FALSE;
	if (m_ptPos.x < 0)
		return FALSE;
	if (m_ptPos.x > GAME_WIDTH + SK_WIDTH)
		return FALSE;


	m_Images.Draw(pDC, 0, m_ptPos, ILD_TRANSPARENT);

	return TRUE;
}