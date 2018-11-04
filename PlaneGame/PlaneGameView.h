// PlaneGameView.h : CPlaneGameView 类的接口
//


#pragma once

#include "Shield.h"     //护盾
#include "Wingman.h"    //僚机

class CMyPlane;
class CPlaneGameView : public CView
{

	enum ObjType { enEnemy, enBomb, enBomb1, enBall,enExplosion, enBOSS, enBOSS2, enBOSS3, enBall1, enBall2, enGiftwp, enGiftlf, enGiftwm, enGiftShield, enSkill};

protected: // 仅从序列化创建
	CPlaneGameView();
	DECLARE_DYNCREATE(CPlaneGameView)

	// 属性
public:
	CPlaneGameDoc * GetDocument() const;

	// 操作
public:

	// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

	// 实现
public:
	virtual ~CPlaneGameView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	//内存DC
	CDC * m_pMemDC;
	//设备DC
	CClientDC*  m_pDC;
	//内存位图
	CBitmap*    m_pMemBitmap;


protected:
	//初始化游戏
	BOOL InitGame();
	//结束游戏
	void StopGame();

	//刷新游戏的帧画面
	void UpdateFrame(CDC* pMemDC);


	void AI();

	//获得键的状态1->down
	int GetKey(int nVirtKey)
	{
		return (GetKeyState(nVirtKey) & 0x8000) ? 1 : 0;
	}

	CMyPlane*  m_pMe;
	CShield* m_pShield;
	CWingman* m_pWingmanLeft;  //左僚机
	CWingman* m_pWingmanRight;  //右僚机

	CObList    m_ObjList[15];

	void SetStage(int s);

	// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	int scrolls;  //按帧变化，实现背景滚动
	int mylife;  //玩家生命
	int Invincible;  //无敌模式开关
	int shield;  //护盾开关
	int delay;  //按键延迟
	int score;  //得分，通过得分判断是否生成boss
	int selectbackground;  //选择背景
	int weaponlevel;  //武器等级
	int energy;     //能量
	int shieldtime;  //护盾时间
	int stage;  //关卡数
	int bossnum; //boss数量，最多一个
	int selectboss;  //选择生成的boss
	int ispause;  //是否按下暂停触发键
	int pausetrue;  //是否真正需要暂停

};

#ifndef _DEBUG  // PlaneGameView.cpp 中的调试版本
inline CPlaneGameDoc* CPlaneGameView::GetDocument() const
{
	return reinterpret_cast<CPlaneGameDoc*>(m_pDocument);
}
#endif




