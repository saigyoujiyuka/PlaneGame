#pragma once
#include "GameObject.h"

class CWingman :public CGameObject
{
public:
	CWingman(void);
	CWingman(int x,int y);
	~CWingman(void);

	BOOL Draw(CDC* pDC, BOOL bPause);

	static BOOL LoadImage();

	void SetHorMotion(int nMotion)
	{
		m_nHorMotion = nMotion;
	}
	int GetHorMotion() const
	{
		return m_nHorMotion;
	}

	void SetVerMotion(int nMotion)
	{
		m_nVerMotion = nMotion;
	}
	int GetVerMotion() const
	{
		return m_nVerMotion;
	}
	void SetPoint(int x,int y);
	CRect GetRect()
	{
		return CRect(m_ptPos, CPoint(m_ptPos.x + WM_WIDTH, m_ptPos.y + WM_HEIGHT));
	}

	//是否可以开火发射导弹
	BOOL Fired();

public:
	static const int WM_WIDTH = 30;
	static const int WM_HEIGHT = 40;                      //战机的宽高
	int life;  //僚机生命
private:

	static CImageList m_Images;
	int    m_nHorMotion;//飞机水平运行方向0->静止，1->右 -1->左
	int    m_nVerMotion;//飞机垂直运行方向0->静止，1->上 -1->下
	int    m_nWait;//发射延时
	
};
