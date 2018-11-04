// PlaneGameView.cpp : CPlaneGameView 类的实现
//

#include "stdafx.h"
#include "PlaneGame.h"

#include "PlaneGameDoc.h"
#include "PlaneGameView.h"
#include "MyPlane.h"
#include "Enemy.h"
#include "Bomb.h"
#include "Ball.h"
#include "Explosion.h"
#include <atlimage.h>
#include "BOSS.h"
#include "BOSS2.h"
#include "BOSS3.h"
#include "Ball1.h"
#include "Ball2.h"
#include "GiftWeapon.h"
#include "Bomb1.h"
#include "Skill.h"
#include "GiftLife.h"
#include "GiftShield.h"
#include "GiftWM.h"
#include "Shield.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CPlaneGameView

IMPLEMENT_DYNCREATE(CPlaneGameView, CView)

BEGIN_MESSAGE_MAP(CPlaneGameView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_TIMER()

END_MESSAGE_MAP()

// CPlaneGameView 构造/析构

CPlaneGameView::CPlaneGameView():m_pMe(NULL)
{
	// TODO: 在此处添加构造代码
}

CPlaneGameView::~CPlaneGameView()
{
}

BOOL CPlaneGameView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CPlaneGameView 绘制

void CPlaneGameView::OnDraw(CDC* /*pDC*/)
{
	CPlaneGameDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}


// CPlaneGameView 打印

BOOL CPlaneGameView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CPlaneGameView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CPlaneGameView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}


// CPlaneGameView 诊断

#ifdef _DEBUG
void CPlaneGameView::AssertValid() const
{
	CView::AssertValid();
}

void CPlaneGameView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CPlaneGameDoc* CPlaneGameView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CPlaneGameDoc)));
	return (CPlaneGameDoc*)m_pDocument;
}
#endif //_DEBUG


// CPlaneGameView 消息处理程序
void CPlaneGameView::OnInitialUpdate()
{
	CView::OnInitialUpdate();
	// TODO: 在此添加专用代码和/或调用基类
	//初始化游戏
		InitGame();

}
void CPlaneGameView::StopGame()
{
	delete m_pMe;
	delete m_pMemDC;
	delete m_pDC;
	delete m_pMemBitmap;
}


BOOL CPlaneGameView::InitGame()
{
	CRect rc;
	GetClientRect(rc);

	//产生随机数种子
	srand( (unsigned)time( NULL ) );

	//建立设备DC
	m_pDC = new CClientDC(this);

	//建立内存DC
	m_pMemDC = new CDC;
	m_pMemDC->CreateCompatibleDC(m_pDC);

	//建立内存位图
	m_pMemBitmap = new CBitmap;
	m_pMemBitmap->CreateCompatibleBitmap(m_pDC,GAME_WIDTH,GAME_HEIGHT);

	//将位图选入内存DC
	m_pMemDC->SelectObject(m_pMemBitmap);

	CMyPlane::LoadImage();
	CEnemy::LoadImage();
	CBomb::LoadImage();
	CBall::LoadImage();
	CExplosion::LoadImage();
	CBOSS::LoadImage();
	CBOSS2::LoadImage();
	CBOSS3::LoadImage();
	CBall1::LoadImage();
	CBall2::LoadImage();
	CGiftWeapon::LoadImage();
	CBomb1::LoadImage();
	CSkill::LoadImage();
	CGiftLife::LoadImage();
	CGiftShield::LoadImage();
	CGiftWM::LoadImage();
	CShield::LoadImage();
	CWingman::LoadImage();
	
	//产生主角(战机)w
	m_pMe = new CMyPlane;
	m_pShield = NULL;
	m_pWingmanLeft = NULL;
	m_pWingmanRight = NULL;

	//启动游戏
	SetTimer(1,30,NULL);

	scrolls = 0;  //初始化窗口滚动变量
	mylife = 50;  //初始化玩家生命
	Invincible = 0;  //无敌模式默认关
	shield = 0;  //护盾
	delay = 0;   //按键延迟
	score = 0;   //得分
	weaponlevel = 1;   //武器等级
	energy = 0;   //能量
	shieldtime = 0;
	SetStage(1);  //初始关卡为1
	ispause = 0;
	pausetrue = 1;
	return TRUE;
}

void CPlaneGameView::UpdateFrame(CDC* pMemDC)
{
	//绘制天空
	pMemDC->FillSolidRect(0,0,GAME_WIDTH,GAME_HEIGHT,RGB(84, 142, 239));

	//背景
	CBitmap bitmap;                           //位图类对象

	if (selectbackground == 1)
		bitmap.LoadBitmap(IDB_BACKGROUND1);   //从资源中装载入位图
	else if (selectbackground == 2)
		bitmap.LoadBitmap(IDB_BACKGROUND2);     
	else if (selectbackground == 3)
		bitmap.LoadBitmap(IDB_BACKGROUND3);
	else if (selectbackground == 4)
		bitmap.LoadBitmap(IDB_BACKGROUND4);
	else if (selectbackground == 5)
		bitmap.LoadBitmap(IDB_BACKGROUND5);


	//load DC
	CDC comdc;  //创建CDC对象
	comdc.CreateCompatibleDC(m_pMemDC);   //创建设备环境
	comdc.SelectObject(&bitmap);      //选择位图对象
	
	//将兼容性DC的信息拷贝到当前DC
	CRect rect;
	GetWindowRect(&rect);
	ScreenToClient(&rect);

	//m_pMemDC->BitBlt(rect.left, rect.top, rect.Width(), rect.Height(), &comdc, 0, 0, SRCCOPY);

	m_pMemDC->BitBlt(0, 0, GAME_WIDTH, scrolls, &comdc, 0, GAME_HEIGHT-scrolls, SRCCOPY);
	m_pMemDC->BitBlt(0, scrolls, GAME_WIDTH, GAME_HEIGHT-scrolls, &comdc, 0, 0, SRCCOPY);

	if (scrolls <= GAME_HEIGHT)    //控制背景滚动
		scrolls += 6;
	else
		scrolls = 0;


	// 绘制血条
	CString hp;
	hp.Format(_T("HP:"));
	pMemDC->SetBkMode(TRANSPARENT);
	pMemDC->SetTextAlign(TA_CENTER);
	pMemDC->SetTextColor(RGB(0, 255, 255));
	pMemDC->TextOut(20, 30, hp);
	CBrush br;  // 血条
	br.CreateSolidBrush(RGB(0, 255, 102));
	m_pMemDC->SelectObject(br);
	m_pMemDC->Rectangle(42, 30, 42 + 4 * mylife, 45);
	CBrush br1;
	br1.CreateSolidBrush(RGB(0, 0, 0));
	m_pMemDC->SelectObject(br1);
	m_pMemDC->Rectangle(42 + 4 * mylife, 30, 42 + 200, 45);


	// 绘制能量条
	CString ene;
	ene.Format(_T("SP:"));
	pMemDC->SetBkMode(TRANSPARENT);
	pMemDC->SetTextAlign(TA_CENTER);
	pMemDC->SetTextColor(RGB(0, 255, 255));
	pMemDC->TextOut(20, 50, ene);
	CBrush brene;  
	brene.CreateSolidBrush(RGB(249, 200, 2));
	m_pMemDC->SelectObject(brene);
	m_pMemDC->Rectangle(42, 50, 42 + energy/3, 65);
	CBrush brene1;
	brene1.CreateSolidBrush(RGB(0, 0, 0));
	m_pMemDC->SelectObject(brene1);
	m_pMemDC->Rectangle(42 + energy/3, 50, 42 + 200, 65);

	if (energy < 600)
		energy++;



	CString tip;
	tip.Format(_T("Z键使用技能 X键开启无敌模式 R键复活 P键暂停"));
	pMemDC->SetBkMode(TRANSPARENT);
	pMemDC->SetTextAlign(TA_CENTER);
	pMemDC->SetTextColor(RGB(0, 255, 255));
	pMemDC->TextOut(205, 5, tip);




	CString wudi;
	if (Invincible == 0)
		wudi.Format(_T("无敌模式已关闭"));
	else
		wudi.Format(_T("无敌模式已开启"));
	pMemDC->SetBkMode(TRANSPARENT);
	pMemDC->SetTextAlign(TA_CENTER);
	pMemDC->SetTextColor(RGB(0, 255, 255));
	pMemDC->TextOut(72, 75, wudi);


	CString sc;
	sc.Format(_T("得分：%d"),score);
	pMemDC->SetBkMode(TRANSPARENT);
	pMemDC->SetTextAlign(TA_CENTER);
	pMemDC->SetTextColor(RGB(0, 255, 255));
	pMemDC->TextOut(45, 95, sc);

	sc.Format(_T("当前关卡：%d"), stage);
	pMemDC->TextOut(57, 115, sc);




	//绘制我方战机
	if(m_pMe!=NULL)
	{
	   m_pMe->Draw(m_pMemDC,FALSE);
	   if (shield == 1)
	   {
		   CPoint s_point = m_pMe->GetPoint();
		   s_point.x -= 10;
		   s_point.y -= 20;
		   m_pShield->SetPoint(s_point);
		   m_pShield->Draw(m_pMemDC, FALSE);
	   }
	   if (m_pWingmanLeft != NULL)
	   {
		   CPoint s_point = m_pMe->GetPoint();
		   m_pWingmanLeft->SetPoint(s_point.x-35,s_point.y+10);
		   m_pWingmanLeft->Draw(m_pMemDC, FALSE);
	   }
	   if (m_pWingmanRight != NULL)
	   {
		   CPoint s_point = m_pMe->GetPoint();
		   m_pWingmanRight->SetPoint(s_point.x + m_pMe->PLANE_WIDTH +5, s_point.y+10);
		   m_pWingmanRight->Draw(m_pMemDC, FALSE);
	   }

	}
	else
	{   //Game Over
		CString str=_T("Game Over!");
		pMemDC->SetBkMode(TRANSPARENT);
		pMemDC->SetTextAlign(TA_CENTER);
		pMemDC->SetTextColor(RGB(0,255,255));
		pMemDC->TextOut(GAME_WIDTH/2,GAME_HEIGHT/2,str);
		pMemDC->TextOut(GAME_WIDTH / 2, GAME_HEIGHT / 2 + 15, _T("按R复活"));
		
	}
	
	//table

	//绘制 导弹、爆炸、敌机、子弹    
	for(int i=0;i<15;i++)
	{
		POSITION pos1,pos2;
		for( pos1 = m_ObjList[i].GetHeadPosition(); ( pos2 = pos1 ) != NULL; )
		{
			CGameObject* pObj = (CGameObject*)m_ObjList[i].GetNext( pos1 );
			if(!pObj->Draw(pMemDC,FALSE))
			{
				m_ObjList[i].RemoveAt(pos2);
				delete pObj;
			}
		}
	}

	//复制内存DC到设备DC
	m_pDC->BitBlt(0,0,GAME_WIDTH,GAME_HEIGHT,m_pMemDC,0,0,SRCCOPY);
}
void CPlaneGameView::AI()
{
	static int nCreator = rand() % 5 + 10;    //控制生成频率

	//随机产生敌机
	if (nCreator <= 0)
	{
		nCreator = rand() % 5 + 10;
		m_ObjList[enEnemy].AddTail(new CEnemy);
	}
	nCreator--;



	//随机产生敌机boss
	static int ncreatorboss = 400;     /*rand() % 500 + 500; */
	if (ncreatorboss <= 0 && bossnum == 0)
	{
		ncreatorboss = 400;        /*rand() % 500 + 500;*/
		if (selectboss == 1)
			m_ObjList[enBOSS].AddTail(new CBOSS);
		else if (selectboss == 2)
			m_ObjList[enBOSS2].AddTail(new CBOSS2);
		else if (selectboss == 3)
			m_ObjList[enBOSS3].AddTail(new CBOSS3);
		bossnum++;
	}
	ncreatorboss--;

	//随机产生武器升级道具
	static int ncreatorgiftwp = rand() % 50 + 200;
	if (ncreatorgiftwp <= 0)
	{
		ncreatorgiftwp = rand() % 50 + 200;
		m_ObjList[enGiftwp].AddTail(new CGiftWeapon);
	}
	ncreatorgiftwp--;
	

	//随机产生血包
	static int ncreatorgiftlf = rand() % 50 + 200;
	if (ncreatorgiftlf <= 0)
	{
		ncreatorgiftlf = rand() % 50 + 200;
		m_ObjList[enGiftlf].AddTail(new CGiftLife);
	}
	ncreatorgiftlf--;
	

	//随机产生护盾礼物
	static int ncreatorgiftsd = rand() % 50 + 200;
	if (ncreatorgiftsd <= 0)
	{
		ncreatorgiftsd = rand() % 50 + 200;
		m_ObjList[enGiftShield].AddTail(new CGiftShield);
	}
	ncreatorgiftsd--;
	
	//随机产生僚机礼物
	static int ncreatorgiftwm = rand() % 50 + 200;
	if (ncreatorgiftwm <= 0)
	{
		ncreatorgiftwm = rand() % 50 + 200;
		m_ObjList[enGiftwm].AddTail(new CGiftWM);
	}
	ncreatorgiftwm--;
	



	if (GetKey('R') && m_pMe == NULL)
	{
		m_pMe = new CMyPlane;
		m_pWingmanLeft = NULL;
		m_pWingmanRight = NULL;
		mylife = 50;
		weaponlevel = 1;
		energy = 0;
	}

	if (m_pMe == NULL)
		return;

	//检测四个方向键，移动战机
	for (int i = 0; i < 4; i++)
	{
		int nMeMotion = 0;
		m_pMe->SetVerMotion(0);
		m_pMe->SetHorMotion(0);

		nMeMotion = GetKey(VK_UP);
		if (nMeMotion == 1)
			m_pMe->SetVerMotion(1);

		nMeMotion = GetKey(VK_DOWN);
		if (nMeMotion == 1)
			m_pMe->SetVerMotion(-1);

		nMeMotion = GetKey(VK_RIGHT);
		if (nMeMotion == 1)
			m_pMe->SetHorMotion(1);

		nMeMotion = GetKey(VK_LEFT);
		if (nMeMotion == 1)
			m_pMe->SetHorMotion(-1);

	}

	//产生战机导弹
	if (GetKey(VK_SPACE) == 1)//按下了空格键
	{
		if (m_pMe != NULL && m_pMe->Fired())
		{
			CPoint pt = m_pMe->GetPoint();
			if (weaponlevel == 1)
			{
				m_ObjList[enBomb].AddTail(new CBomb(pt.x + 10, pt.y + 10));          //控制生成子弹的位置
				m_ObjList[enBomb].AddTail(new CBomb(pt.x + 48, pt.y + 10));
			}
			else if (weaponlevel == 2)
			{
				m_ObjList[enBomb].AddTail(new CBomb(pt.x + 5, pt.y + 10, -1));
				m_ObjList[enBomb].AddTail(new CBomb(pt.x + 30, pt.y, 0));     //控制生成子弹的位置
				m_ObjList[enBomb].AddTail(new CBomb(pt.x + 53, pt.y + 10, 1));
			}
			else if (weaponlevel == 3)
			{
				m_ObjList[enBomb].AddTail(new CBomb(pt.x - 5, pt.y + 10, -2));
				m_ObjList[enBomb].AddTail(new CBomb(pt.x, pt.y + 10, -1));

				m_ObjList[enBomb].AddTail(new CBomb(pt.x + 10, pt.y + 10));
				m_ObjList[enBomb].AddTail(new CBomb(pt.x + 30, pt.y, 0));     //控制生成子弹的位置
				m_ObjList[enBomb].AddTail(new CBomb(pt.x + 48, pt.y + 10));

				m_ObjList[enBomb].AddTail(new CBomb(pt.x + 58, pt.y + 10, 1));
				m_ObjList[enBomb].AddTail(new CBomb(pt.x + 65, pt.y + 10, 2));
			}

		}

		//僚机发射子弹
		// 僚机发射子弹

		if (m_pMe != NULL && m_pMe->Fired() && m_pWingmanLeft != NULL)
		{
			CPoint pt = m_pWingmanLeft->GetPoint();
			m_ObjList[enBomb1].AddTail(new CBomb1(pt.x + 8, pt.y, 0));

		}
		if (m_pMe != NULL && m_pMe->Fired() && m_pWingmanRight != NULL)
		{
			CPoint pt = m_pWingmanRight->GetPoint();
			m_ObjList[enBomb1].AddTail(new CBomb1(pt.x + 8, pt.y, 0));

		}
	

	}
	if (GetKey('X') && delay == 0)
	{
		if (Invincible == 0)
			Invincible = 1;
		else if (Invincible == 1)
			Invincible = 0;
		delay++;
	}
	if (GetKey('Z') && energy >= 300 && delay == 0)
	{
		energy -= 300;
		for (int i = 0; i < 8; i++)
		{
			m_ObjList[enSkill].AddTail(new CSkill(0 + 128 * i, GAME_HEIGHT + 100));
		}
		delay++;
	}

	/*以下是敌方发射子弹*/

	//敌机发射子弹

	CPoint PlanePt = m_pMe->GetPoint();
	for (POSITION ePos = m_ObjList[enEnemy].GetHeadPosition(); ePos != NULL;)
	{
		CEnemy* pEnemy = (CEnemy*)m_ObjList[enEnemy].GetNext(ePos);
		if (!pEnemy->Fired())
			continue;
		CPoint  ePt = pEnemy->GetPoint();

		BOOL by = FALSE;

		//敌机在战机前面
		if (pEnemy->GetMontion() == 1 && ePt.y < PlanePt.y)
			by = TRUE;
		//敌机在战机后面
		if (pEnemy->GetMontion() == -1 && ePt.y > PlanePt.y)
			by = TRUE;

		if (by && ePt.x >= PlanePt.x && ePt.x < PlanePt.x + CMyPlane::PLANE_WIDTH)
		{
			m_ObjList[enBall].AddTail(new CBall(ePt.x + 20, ePt.y + 10, pEnemy->GetMontion()));    //敌机子弹位置
		}
	}


	//boss发射子弹
	//PlanePt = m_pMe->GetPoint();
	for (POSITION ePos = m_ObjList[enBOSS].GetHeadPosition(); ePos != NULL;)
	{
		CBOSS* pBOSS = (CBOSS*)m_ObjList[enBOSS].GetNext(ePos);
		if (!pBOSS->Fired())
			continue;
		CPoint  ePt = pBOSS->GetPoint();

		m_ObjList[enBall1].AddTail(new CBall1(ePt.x + 30, ePt.y + 260, 1));   //敌机子弹位置
		m_ObjList[enBall1].AddTail(new CBall1(ePt.x + 150, ePt.y + 280, 1));    
		m_ObjList[enBall1].AddTail(new CBall1(ePt.x + 350, ePt.y + 280, 1));
		m_ObjList[enBall1].AddTail(new CBall1(ePt.x + 450, ePt.y + 260, 1));
	}

	//boss2发射子弹
	for (POSITION ePos = m_ObjList[enBOSS2].GetHeadPosition(); ePos != NULL;)
	{
		CBOSS2* pBOSS2 = (CBOSS2*)m_ObjList[enBOSS2].GetNext(ePos);
		if (!pBOSS2->Fired())
			continue;
		CPoint  ePt = pBOSS2->GetPoint();

		m_ObjList[enBall2].AddTail(new CBall2(ePt.x + 50, ePt.y + 260, -2));   //敌机子弹位置
		m_ObjList[enBall2].AddTail(new CBall2(ePt.x + 150, ePt.y + 280, -1));
		m_ObjList[enBall2].AddTail(new CBall2(ePt.x + 250, ePt.y + 300, 0));
		m_ObjList[enBall2].AddTail(new CBall2(ePt.x + 350, ePt.y + 280, 1));
		m_ObjList[enBall2].AddTail(new CBall2(ePt.x + 450, ePt.y + 260, 2));
		//}
	}

	//boss3发射子弹
	for (POSITION ePos = m_ObjList[enBOSS3].GetHeadPosition(); ePos != NULL;)
	{
		CBOSS3* pBOSS3 = (CBOSS3*)m_ObjList[enBOSS3].GetNext(ePos);
		if (!pBOSS3->Fired())
			continue;
		CPoint  ePt = pBOSS3->GetPoint();
		int balltype = rand();
		if (balltype %2==0)
		{
			m_ObjList[enBall1].AddTail(new CBall1(ePt.x + 30, ePt.y + 260, 1));
			m_ObjList[enBall1].AddTail(new CBall1(ePt.x + 150, ePt.y + 280, 1)); 
			m_ObjList[enBall1].AddTail(new CBall1(ePt.x + 350, ePt.y + 280, 1));
			m_ObjList[enBall1].AddTail(new CBall1(ePt.x + 450, ePt.y + 260, 1));
		}
		else
		{
			m_ObjList[enBall2].AddTail(new CBall2(ePt.x + 50, ePt.y + 260, -2));   
			m_ObjList[enBall2].AddTail(new CBall2(ePt.x + 150, ePt.y + 280, -1));
			m_ObjList[enBall2].AddTail(new CBall2(ePt.x + 250, ePt.y + 300, 0));
			m_ObjList[enBall2].AddTail(new CBall2(ePt.x + 350, ePt.y + 280, 1));
			m_ObjList[enBall2].AddTail(new CBall2(ePt.x + 450, ePt.y + 260, 2));
		}
		
	}



	/*以下是玩家收到攻击的判定*/


	//敌机子弹炸掉战机
	POSITION bPos1 = NULL, bPos2 = NULL;
	CRect mRect = m_pMe->GetRect();
	for (bPos1 = m_ObjList[enBall].GetHeadPosition(); (bPos2 = bPos1) != NULL;)
	{
		CBall* pBall = (CBall*)m_ObjList[enBall].GetNext(bPos1);
		CRect bRect = pBall->GetRect();
		CRect tmpRect;
		if (tmpRect.IntersectRect(&bRect, mRect))
		{
			//添加爆炸效果
			m_ObjList[enExplosion].AddTail(
				new CExplosion(mRect.left, mRect.top)
			);

			//删除子弹
			m_ObjList[enBall].RemoveAt(bPos2);
			delete pBall;

			//删除战机
			if (Invincible == 0 && shield == 0)
				mylife -= 2;
			if (mylife <= 0)
			{
				delete m_pMe;
				m_pMe = NULL;
				break;
			}

		}
	}
	//敌机子弹炸掉僚机左
	//POSITION bPos1 = NULL, bPos2 = NULL;
	//if (m_pWingmanLeft != NULL)
	//{
		//CRect wRect = m_pWingmanLeft->GetRect();
		for (bPos1 = m_ObjList[enBall].GetHeadPosition(); (bPos2 = bPos1) != NULL;)
		{
			CBall* pBall = (CBall*)m_ObjList[enBall].GetNext(bPos1);
			CRect bRect = pBall->GetRect();
			CRect tmpRect;
			if (m_pWingmanLeft != NULL)
			{
				CRect wRect = m_pWingmanLeft->GetRect();
				if (tmpRect.IntersectRect(&bRect, wRect))
				{
					//添加爆炸效果
					m_ObjList[enExplosion].AddTail(
						new CExplosion(wRect.left, wRect.top)
					);

					//删除子弹
					m_ObjList[enBall].RemoveAt(bPos2);
					delete pBall;

					//删除战机
					if (Invincible == 0 && shield == 0)
						m_pWingmanLeft->life -= 2;
					if (m_pWingmanLeft->life <= 0)
					{
						delete m_pWingmanLeft;
						m_pWingmanLeft = NULL;
						break;
					}

				}
			}
			if (m_pWingmanRight != NULL)
			{
				CRect wRect = m_pWingmanRight->GetRect();
				if (tmpRect.IntersectRect(&bRect, wRect))
				{
					//添加爆炸效果
					m_ObjList[enExplosion].AddTail(
						new CExplosion(wRect.left, wRect.top)
					);

					//删除子弹
					m_ObjList[enBall].RemoveAt(bPos2);
					delete pBall;

					//删除战机
					if (Invincible == 0 && shield == 0)
						m_pWingmanRight->life -= 2;
					if (m_pWingmanRight->life <= 0)
					{
						delete m_pWingmanRight;
						m_pWingmanRight = NULL;
						break;
					}

				}
			}
			
		}	
	

	//boss子弹1攻击战机
    //	mRect = m_pMe->GetRect();
	for (bPos1 = m_ObjList[enBall1].GetHeadPosition(); (bPos2 = bPos1) != NULL;)
	{
		CBall1* pBall1 = (CBall1*)m_ObjList[enBall1].GetNext(bPos1);
		CRect bRect = pBall1->GetRect();
		CRect tmpRect;
		if (tmpRect.IntersectRect(&bRect, mRect))
		{
			//添加爆炸效果
			m_ObjList[enExplosion].AddTail(
				new CExplosion(mRect.left, mRect.top)
			);

			//删除子弹
			m_ObjList[enBall1].RemoveAt(bPos2);
			delete pBall1;

			//删除战机
			if (Invincible == 0 && shield == 0)
				mylife -= 3;
			if (mylife <= 0)
			{
				delete m_pMe;
				m_pMe = NULL;
				break;
			}

		}
	}

	//boss子弹1炸掉僚机左
	//POSITION bPos1 = NULL, bPos2 = NULL;
		for (bPos1 = m_ObjList[enBall1].GetHeadPosition(); (bPos2 = bPos1) != NULL;)
		{
			CBall1* pBall1 = (CBall1*)m_ObjList[enBall1].GetNext(bPos1);
			CRect bRect = pBall1->GetRect();
			CRect tmpRect;
			if (m_pWingmanLeft != NULL)
			{
				CRect wRect = m_pWingmanLeft->GetRect();
				if (tmpRect.IntersectRect(&bRect, wRect))
				{
				//添加爆炸效果
				m_ObjList[enExplosion].AddTail(
					new CExplosion(wRect.left, wRect.top)
				);

				//删除子弹
				m_ObjList[enBall1].RemoveAt(bPos2);
				delete pBall1;

				//删除战机
				if (Invincible == 0 && shield == 0)
					m_pWingmanLeft->life -= 6;
				if (m_pWingmanLeft->life <= 0)
				{
					delete m_pWingmanLeft;
					m_pWingmanLeft = NULL;
					break;
				}

				}
				
			}
			if (m_pWingmanRight != NULL)
			{
				CRect wRect = m_pWingmanRight->GetRect();
				if (tmpRect.IntersectRect(&bRect, wRect))
				{
					//添加爆炸效果
					m_ObjList[enExplosion].AddTail(
						new CExplosion(wRect.left, wRect.top)
					);

					//删除子弹
					m_ObjList[enBall1].RemoveAt(bPos2);
					delete pBall1;

					//删除战机
					if (Invincible == 0 && shield == 0)
						m_pWingmanRight->life -= 6;
					if (m_pWingmanRight->life <= 0)
					{
						delete m_pWingmanRight;
						m_pWingmanRight = NULL;
						break;
					}

				}

			}
			
		}
	
	


	//boss子弹2攻击战机
	//mRect = m_pMe->GetRect();
	for (bPos1 = m_ObjList[enBall2].GetHeadPosition(); (bPos2 = bPos1) != NULL;)
	{
		CBall2* pBall2 = (CBall2*)m_ObjList[enBall2].GetNext(bPos1);
		CRect bRect = pBall2->GetRect();
		CRect tmpRect;
		if (tmpRect.IntersectRect(&bRect, mRect))
		{
			//添加爆炸效果
			m_ObjList[enExplosion].AddTail(
				new CExplosion(mRect.left, mRect.top)
			);

			//删除子弹
			m_ObjList[enBall2].RemoveAt(bPos2);
			delete pBall2;

			//删除战机
			if (Invincible == 0 && shield == 0)
				mylife -= 3;
			if (mylife <= 0)
			{
				delete m_pMe;
				m_pMe = NULL;
				break;
			}

		}
	}
	//boss子弹2炸掉僚机左
	//POSITION bPos1 = NULL, bPos2 = NULL;

	for (bPos1 = m_ObjList[enBall2].GetHeadPosition(); (bPos2 = bPos1) != NULL;)
	{
		CBall2* pBall2 = (CBall2*)m_ObjList[enBall2].GetNext(bPos1);
		CRect bRect = pBall2->GetRect();
		CRect tmpRect;
		if (m_pWingmanLeft != NULL)
		{
			CRect wRect = m_pWingmanLeft->GetRect();
			if (tmpRect.IntersectRect(&bRect, wRect))
			{
				//添加爆炸效果
				m_ObjList[enExplosion].AddTail(
					new CExplosion(wRect.left, wRect.top)
				);

				//删除子弹
				m_ObjList[enBall2].RemoveAt(bPos2);
				delete pBall2;

				//删除战机
				if (Invincible == 0 && shield == 0)
					m_pWingmanLeft->life -= 6;
				if (m_pWingmanLeft->life <= 0)
				{
					delete m_pWingmanLeft;
					m_pWingmanLeft = NULL;
					break;
				}

			}
		}
		if (m_pWingmanRight != NULL)
		{
			CRect wRect = m_pWingmanRight->GetRect();
			if (tmpRect.IntersectRect(&bRect, wRect))
			{
				//添加爆炸效果
				m_ObjList[enExplosion].AddTail(new CExplosion(wRect.left, wRect.top));

				//删除子弹
				m_ObjList[enBall2].RemoveAt(bPos2);
				delete pBall2;

				//删除战机
				if (Invincible == 0 && shield == 0)
					m_pWingmanRight->life -= 6;
				if (m_pWingmanRight->life <= 0)
				{
					delete m_pWingmanRight;
					m_pWingmanRight = NULL;
					break;
				}

			}
		}
		
	}
	
	




	/*以下是飞机间的碰撞*/

	//敌机碰到战机
	for (bPos1 = m_ObjList[enEnemy].GetHeadPosition(); (bPos2 = bPos1) != NULL;)
	{
		CEnemy* pEnecol = (CEnemy*)m_ObjList[enEnemy].GetNext(bPos1);
		CRect bRect = pEnecol->GetRect();
		CRect tmpRect;
		if (tmpRect.IntersectRect(&bRect, mRect))
		{
			m_ObjList[enEnemy].RemoveAt(bPos2);
			delete pEnecol;
			m_ObjList[enExplosion].AddTail(new CExplosion(bRect.left, bRect.top));

			if (Invincible == 0 && shield == 0)
				mylife -= 2;
			if (mylife <= 0)
			{
				delete m_pMe;
				m_pMe = NULL;
				break;
			}
		}
	}

	//敌机碰到僚机
	for (bPos1 = m_ObjList[enEnemy].GetHeadPosition(); (bPos2 = bPos1) != NULL;)
	{
		CEnemy* pEnecol = (CEnemy*)m_ObjList[enEnemy].GetNext(bPos1);
		CRect bRect = pEnecol->GetRect();
		CRect tmpRect;
		if (m_pWingmanLeft != NULL)
		{
			CRect wRect = m_pWingmanLeft->GetRect();
			if (tmpRect.IntersectRect(&bRect, wRect))
			{
				m_ObjList[enEnemy].RemoveAt(bPos2);
				delete pEnecol;
				m_ObjList[enExplosion].AddTail(new CExplosion(bRect.left, bRect.top));

				if (Invincible == 0 && shield == 0)
					m_pWingmanLeft->life -= 2;
				if (m_pWingmanLeft->life <= 0)
				{
					delete m_pWingmanLeft;
					m_pWingmanLeft = NULL;
					break;
				}
			}
		}
		if (m_pWingmanRight != NULL)
		{
			CRect wRect = m_pWingmanRight->GetRect();
			if (tmpRect.IntersectRect(&bRect, wRect))
			{
				m_ObjList[enEnemy].RemoveAt(bPos2);
				delete pEnecol;
				m_ObjList[enExplosion].AddTail(new CExplosion(bRect.left, bRect.top));

				if (Invincible == 0 && shield == 0)
					m_pWingmanRight->life -= 2;
				if (m_pWingmanRight->life <= 0)
				{
					delete m_pWingmanRight;
					m_pWingmanRight = NULL;
					break;
				}
			}
		}

		
	}


	//BOSS碰到战机
	for (bPos1 = m_ObjList[enBOSS].GetHeadPosition(); (bPos2 = bPos1) != NULL;)
	{
		CBOSS* pboscol = (CBOSS*)m_ObjList[enBOSS].GetNext(bPos1);
		CRect bRect = pboscol->GetRect();
		CRect tmpRect;
		if (tmpRect.IntersectRect(&bRect, mRect))
		{


			static int invtime = 0;  //与boss相撞后有短暂的无敌时间
			if (invtime == 0)
			{
				m_ObjList[enExplosion].AddTail(new CExplosion(mRect.left, mRect.top));
				pboscol->bosslife -= 5;
				if (Invincible == 0 && shield == 0)
				{
					mylife -= 10;
				}
				if (mylife <= 0)
				{
					delete m_pMe;
					m_pMe = NULL;
					break;
				}
				if (pboscol->bosslife <= 0)
				{
					m_ObjList[enBOSS].RemoveAt(bPos2);
					//bossnum = 0;
					m_ObjList[enExplosion].AddTail(new CExplosion(bRect.left, bRect.top));
					delete pboscol;
					SetStage(++stage);
					ncreatorboss = 400;
					break;
				}
			}
			invtime++;
			if (invtime == 20)
				invtime = 0;
		}
	}
	//boss碰到僚机
	for (bPos1 = m_ObjList[enBOSS].GetHeadPosition(); (bPos2 = bPos1) != NULL;)
	{
		CBOSS* pboscol = (CBOSS*)m_ObjList[enBOSS].GetNext(bPos1);
		CRect bRect = pboscol->GetRect();
		CRect tmpRect;
		if (m_pWingmanLeft != NULL)
		{
			CRect wRect = m_pWingmanLeft->GetRect();
			if (tmpRect.IntersectRect(&bRect, wRect))
			{
				m_ObjList[enExplosion].AddTail(new CExplosion(wRect.left, wRect.top));

				if (Invincible == 0 && shield == 0)
				{
					m_pWingmanLeft->life -= 10;
				}
				if (m_pWingmanLeft->life <= 0)
				{
					delete m_pWingmanLeft;
					m_pWingmanLeft = NULL;
					break;
				}
				//if (pboscol->bosslife <= 0)
				//{
				//	m_ObjList[enBOSS].RemoveAt(bPos2);
				//	//bossnum = 0;
				//	m_ObjList[enExplosion].AddTail(new CExplosion(bRect.left, bRect.top));
				//	delete pboscol;
				//	SetStage(++stage);
				//	ncreatorboss = 400;
				//	break;
				//}
			}
		}
		if (m_pWingmanRight != NULL)
		{
			CRect wRect = m_pWingmanRight->GetRect();
			if (tmpRect.IntersectRect(&bRect, wRect))
			{
				m_ObjList[enExplosion].AddTail(new CExplosion(wRect.left, wRect.top));

				if (Invincible == 0 && shield == 0)
				{
					m_pWingmanRight->life -= 10;
				}
				if (m_pWingmanRight->life <= 0)
				{
					delete m_pWingmanRight;
					m_pWingmanRight = NULL;
					break;
				}
				//if (pboscol->bosslife <= 0)
				//{
				//	m_ObjList[enBOSS].RemoveAt(bPos2);
				//	//bossnum = 0;
				//	m_ObjList[enExplosion].AddTail(new CExplosion(bRect.left, bRect.top));
				//	delete pboscol;
				//	SetStage(++stage);
				//	ncreatorboss = 400;
				//	break;
				//}
			}
		}
		
	}


	//BOSS2碰到战机
	for (bPos1 = m_ObjList[enBOSS2].GetHeadPosition(); (bPos2 = bPos1) != NULL;)
	{
		CBOSS2* pboscol2 = (CBOSS2*)m_ObjList[enBOSS2].GetNext(bPos1);
		CRect bRect = pboscol2->GetRect();
		CRect tmpRect;
		if (tmpRect.IntersectRect(&bRect, mRect))
		{
			static int invtime = 0;  //与boss相撞后有短暂的无敌时间
			if (invtime == 0)
			{
				m_ObjList[enExplosion].AddTail(new CExplosion(mRect.left, mRect.top));
				pboscol2->bosslife -= 5;
				if (Invincible == 0 && shield == 0)
				{
					mylife -= 10;
				}
				if (mylife <= 0)
				{
					delete m_pMe;
					m_pMe = NULL;
					break;
				}
				if (pboscol2->bosslife <= 0)
				{
					m_ObjList[enBOSS2].RemoveAt(bPos2);
					//bossnum = 0;
					m_ObjList[enExplosion].AddTail(new CExplosion(bRect.left, bRect.top));
					delete pboscol2;
					SetStage(++stage);
					ncreatorboss = 400;
					break;
				}
			}
			invtime++;
			if (invtime == 20)
				invtime = 0;
		}
	}
	//boss2碰到僚机
	for (bPos1 = m_ObjList[enBOSS2].GetHeadPosition(); (bPos2 = bPos1) != NULL;)
	{
		CBOSS2* pboscol = (CBOSS2*)m_ObjList[enBOSS2].GetNext(bPos1);
		CRect bRect = pboscol->GetRect();
		CRect tmpRect;
		if (m_pWingmanLeft != NULL)
		{
			CRect wRect = m_pWingmanLeft->GetRect();
			if (tmpRect.IntersectRect(&bRect, wRect))
			{
				m_ObjList[enExplosion].AddTail(new CExplosion(wRect.left, wRect.top));

				if (Invincible == 0 && shield == 0)
				{
					m_pWingmanLeft->life -= 10;
				}
				if (m_pWingmanLeft->life <= 0)
				{
					delete m_pWingmanLeft;
					m_pWingmanLeft = NULL;
					break;
				}
			}
		}
		if (m_pWingmanRight != NULL)
		{
			CRect wRect = m_pWingmanRight->GetRect();
			if (tmpRect.IntersectRect(&bRect, wRect))
			{
				m_ObjList[enExplosion].AddTail(new CExplosion(wRect.left, wRect.top));
				
				if (Invincible == 0 && shield == 0)
				{
					m_pWingmanRight->life -= 10;
				}
				if (m_pWingmanRight->life <= 0)
				{
					delete m_pWingmanRight;
					m_pWingmanRight = NULL;
					break;
				}
			}
		}

	}



	//BOSS3碰到战机
	for (bPos1 = m_ObjList[enBOSS3].GetHeadPosition(); (bPos2 = bPos1) != NULL;)
	{
		CBOSS3* pboscol3 = (CBOSS3*)m_ObjList[enBOSS3].GetNext(bPos1);
		CRect bRect = pboscol3->GetRect();
		CRect tmpRect;
		if (tmpRect.IntersectRect(&bRect, mRect))
		{
			static int invtime = 0;  //与boss相撞后有短暂的无敌时间
			if (invtime == 0)
			{
				m_ObjList[enExplosion].AddTail(new CExplosion(mRect.left, mRect.top));
				pboscol3->bosslife -= 5;
				if (Invincible == 0 && shield == 0)
				{
					mylife -= 10;
				}
				if (mylife <= 0)
				{
					delete m_pMe;
					m_pMe = NULL;
					break;
				}
				if (pboscol3->bosslife <= 0)
				{
					m_ObjList[enBOSS3].RemoveAt(bPos2);
					//bossnum = 0;
					m_ObjList[enExplosion].AddTail(new CExplosion(bRect.left, bRect.top));
					delete pboscol3;
					SetStage(++stage);
					ncreatorboss = 400;
					break;
				}
			}
			invtime++;
			if (invtime == 20)
				invtime = 0;
		}
	}
	//boss3碰到僚机
	for (bPos1 = m_ObjList[enBOSS3].GetHeadPosition(); (bPos2 = bPos1) != NULL;)
	{
		CBOSS3* pboscol = (CBOSS3*)m_ObjList[enBOSS3].GetNext(bPos1);
		CRect bRect = pboscol->GetRect();
		CRect tmpRect;
		if (m_pWingmanLeft != NULL)
		{
			CRect wRect = m_pWingmanLeft->GetRect();
			if (tmpRect.IntersectRect(&bRect, wRect))
			{
				m_ObjList[enExplosion].AddTail(new CExplosion(wRect.left, wRect.top));

				if (Invincible == 0 && shield == 0)
				{
					m_pWingmanLeft->life -= 10;
				}
				if (m_pWingmanLeft->life <= 0)
				{
					delete m_pWingmanLeft;
					m_pWingmanLeft = NULL;
					break;
				}
			}
		}
		if (m_pWingmanRight != NULL)
		{
			CRect wRect = m_pWingmanRight->GetRect();
			if (tmpRect.IntersectRect(&bRect, wRect))
			{
				m_ObjList[enExplosion].AddTail(new CExplosion(wRect.left, wRect.top));

				if (Invincible == 0 && shield == 0)
				{
					m_pWingmanRight->life -= 10;
				}
				if (m_pWingmanRight->life <= 0)
				{
					delete m_pWingmanRight;
					m_pWingmanRight = NULL;
					break;
				}
			}
		}

	}



		//玩家吃到武器升级
		for (bPos1 = m_ObjList[enGiftwp].GetHeadPosition(); (bPos2 = bPos1) != NULL;)
		{
			CGiftWeapon* gfwp = (CGiftWeapon*)m_ObjList[enGiftwp].GetNext(bPos1);
			CRect bRect = gfwp->GetRect();
			CRect tmpRect;
			if (tmpRect.IntersectRect(&bRect, mRect))
			{
				if (weaponlevel < 3)
					weaponlevel++;
				m_ObjList[enGiftwp].RemoveAt(bPos2);
				delete gfwp;


			}
		}

		//玩家吃到血包
		for (bPos1 = m_ObjList[enGiftlf].GetHeadPosition(); (bPos2 = bPos1) != NULL;)
		{
			CGiftLife* gflf = (CGiftLife*)m_ObjList[enGiftlf].GetNext(bPos1);
			CRect bRect = gflf->GetRect();
			CRect tmpRect;
			if (tmpRect.IntersectRect(&bRect, mRect))
			{
				mylife += 20;
				if (mylife > 50)
					mylife = 50;

				m_ObjList[enGiftlf].RemoveAt(bPos2);
				delete gflf;
			}
		}

		//玩家吃到护盾
		for (bPos1 = m_ObjList[enGiftShield].GetHeadPosition(); (bPos2 = bPos1) != NULL;)
		{
			CGiftShield* gfsd = (CGiftShield*)m_ObjList[enGiftShield].GetNext(bPos1);
			CRect bRect = gfsd->GetRect();
			CRect tmpRect;
			if (tmpRect.IntersectRect(&bRect, mRect))
			{
				m_ObjList[enGiftShield].RemoveAt(bPos2);
				delete gfsd;

				if (m_pShield == NULL)
					m_pShield = new CShield;
				shield = 1;
				shieldtime = 200;
			}
		}

		//玩家吃到僚机礼物
		for (bPos1 = m_ObjList[enGiftwm].GetHeadPosition(); (bPos2 = bPos1) != NULL;)
		{
			CGiftWM* gfwm = (CGiftWM*)m_ObjList[enGiftwm].GetNext(bPos1);
			CRect bRect = gfwm->GetRect();
			CRect tmpRect;
			if (tmpRect.IntersectRect(&bRect, mRect))
			{
				m_ObjList[enGiftwm].RemoveAt(bPos2);
				delete gfwm;

				if (m_pWingmanLeft == NULL)
				{
					m_pWingmanLeft = new CWingman();
				}
				else if (m_pWingmanRight == NULL)
				{
					m_pWingmanRight = new CWingman();
				}

				/*if (m_pShield == NULL)
					m_pShield = new CShield;
				shield = 1;*/
				//shieldtime = 200;
			}
		}




		/*以下是敌人收到攻击的判定*/



		//战机导弹炸掉敌机
		POSITION mPos1 = NULL, mPos2 = NULL;
		for (mPos1 = m_ObjList[enBomb].GetHeadPosition(); (mPos2 = mPos1) != NULL;)
		{
			CBomb* pBomb = (CBomb*)m_ObjList[enBomb].GetNext(mPos1);
			CRect bRect = pBomb->GetRect();

			POSITION ePos1 = NULL, ePos2 = NULL;
			for (ePos1 = m_ObjList[enEnemy].GetHeadPosition(); (ePos2 = ePos1) != NULL;)
			{
				CEnemy* pEnemy = (CEnemy*)m_ObjList[enEnemy].GetNext(ePos1);
				CRect mRect = pEnemy->GetRect();
				CRect tmpRect;
				if (tmpRect.IntersectRect(&bRect, mRect))
				{
					//添加爆炸效果
					m_ObjList[enExplosion].AddTail(
						new CExplosion(mRect.left, mRect.top)
					);
					//删除导弹
					m_ObjList[enBomb].RemoveAt(mPos2);
					delete pBomb;

					//删除敌机


					m_ObjList[enEnemy].RemoveAt(ePos2);
					score++;
					delete pEnemy;


					break;

				}
			}
		}

		//僚机导弹炸掉敌机
		//POSITION mPos1 = NULL, mPos2 = NULL;
		for (mPos1 = m_ObjList[enBomb1].GetHeadPosition(); (mPos2 = mPos1) != NULL;)
		{
			CBomb1* pBomb1 = (CBomb1*)m_ObjList[enBomb1].GetNext(mPos1);
			CRect bRect = pBomb1->GetRect();

			POSITION ePos1 = NULL, ePos2 = NULL;
			for (ePos1 = m_ObjList[enEnemy].GetHeadPosition(); (ePos2 = ePos1) != NULL;)
			{
				CEnemy* pEnemy = (CEnemy*)m_ObjList[enEnemy].GetNext(ePos1);
				CRect mRect = pEnemy->GetRect();
				CRect tmpRect;
				if (tmpRect.IntersectRect(&bRect, mRect))
				{
					//添加爆炸效果
					m_ObjList[enExplosion].AddTail(
						new CExplosion(mRect.left, mRect.top)
					);
					//删除导弹
					m_ObjList[enBomb1].RemoveAt(mPos2);
					delete pBomb1;
					//删除敌机
					m_ObjList[enEnemy].RemoveAt(ePos2);
					score++;
					delete pEnemy;
					break;
				}
			}
		}



		mPos1 = NULL, mPos2 = NULL;

		//战机导弹攻击BOSS
		for (mPos1 = m_ObjList[enBomb].GetHeadPosition(); (mPos2 = mPos1) != NULL;)
		{
			CBomb* pBomb = (CBomb*)m_ObjList[enBomb].GetNext(mPos1);
			CRect bRect = pBomb->GetRect();

			POSITION ePos1 = NULL, ePos2 = NULL;
			for (ePos1 = m_ObjList[enBOSS].GetHeadPosition(); (ePos2 = ePos1) != NULL;)
			{
				CBOSS* pBOSS = (CBOSS*)m_ObjList[enBOSS].GetNext(ePos1);
				CRect mRect = pBOSS->GetRect();
				CRect tmpRect;
				if (tmpRect.IntersectRect(&bRect, mRect))
				{
					//删除导弹
					m_ObjList[enBomb].RemoveAt(mPos2);
					delete pBomb;
					//添加爆炸效果
					m_ObjList[enExplosion].AddTail(new CExplosion(bRect.left, bRect.top));

					pBOSS->bosslife -= 1;

					if (pBOSS->bosslife <= 0)
					{
						m_ObjList[enExplosion].AddTail(new CExplosion(mRect.left, mRect.top));
						//删除boss
						m_ObjList[enBOSS].RemoveAt(ePos2);
						score += 10;
						delete pBOSS;
						//bossnum = 0;
						SetStage(++stage);
						ncreatorboss = 400;
						break;
					}

				}
			}
		}

		//战机导弹攻击BOSS2
		for (mPos1 = m_ObjList[enBomb].GetHeadPosition(); (mPos2 = mPos1) != NULL;)
		{
			CBomb* pBomb = (CBomb*)m_ObjList[enBomb].GetNext(mPos1);
			CRect bRect = pBomb->GetRect();

			POSITION ePos1 = NULL, ePos2 = NULL;
			for (ePos1 = m_ObjList[enBOSS2].GetHeadPosition(); (ePos2 = ePos1) != NULL;)
			{
				CBOSS2* pBOSS2 = (CBOSS2*)m_ObjList[enBOSS2].GetNext(ePos1);
				CRect mRect = pBOSS2->GetRect();
				CRect tmpRect;
				if (tmpRect.IntersectRect(&bRect, mRect))
				{
					//删除导弹
					m_ObjList[enBomb].RemoveAt(mPos2);
					delete pBomb;
					//添加爆炸效果
					m_ObjList[enExplosion].AddTail(new CExplosion(bRect.left, bRect.top));

					pBOSS2->bosslife -= 1;

					if (pBOSS2->bosslife <= 0)
					{
						m_ObjList[enExplosion].AddTail(new CExplosion(mRect.left, mRect.top));
						//删除boss
						m_ObjList[enBOSS2].RemoveAt(ePos2);
						score += 10;
						delete pBOSS2;
						//bossnum = 0;
						SetStage(++stage);
						ncreatorboss = 400;
						break;
					}

				}
			}
		}


		//战机导弹攻击BOSS3
		for (mPos1 = m_ObjList[enBomb].GetHeadPosition(); (mPos2 = mPos1) != NULL;)
		{
			CBomb* pBomb = (CBomb*)m_ObjList[enBomb].GetNext(mPos1);
			CRect bRect = pBomb->GetRect();

			POSITION ePos1 = NULL, ePos2 = NULL;
			for (ePos1 = m_ObjList[enBOSS3].GetHeadPosition(); (ePos2 = ePos1) != NULL;)
			{
				CBOSS3* pBOSS3 = (CBOSS3*)m_ObjList[enBOSS3].GetNext(ePos1);
				CRect mRect = pBOSS3->GetRect();
				CRect tmpRect;
				if (tmpRect.IntersectRect(&bRect, mRect))
				{
					//删除导弹
					m_ObjList[enBomb].RemoveAt(mPos2);
					delete pBomb;
					//添加爆炸效果
					m_ObjList[enExplosion].AddTail(new CExplosion(bRect.left, bRect.top));

					pBOSS3->bosslife -= 1;

					if (pBOSS3->bosslife <= 0)
					{
						m_ObjList[enExplosion].AddTail(new CExplosion(mRect.left, mRect.top));
						//删除boss
						m_ObjList[enBOSS3].RemoveAt(ePos2);
						score += 10;
						delete pBOSS3;
						//bossnum = 0;
						SetStage(++stage);
						ncreatorboss = 400;
						break;
					}

				}
			}
		}

		//僚机导弹攻击BOSS
		for (mPos1 = m_ObjList[enBomb1].GetHeadPosition(); (mPos2 = mPos1) != NULL;)
		{
			CBomb1* pBomb1 = (CBomb1*)m_ObjList[enBomb1].GetNext(mPos1);
			CRect bRect = pBomb1->GetRect();

			POSITION ePos1 = NULL, ePos2 = NULL;
			for (ePos1 = m_ObjList[enBOSS].GetHeadPosition(); (ePos2 = ePos1) != NULL;)
			{
				CBOSS* pBOSS = (CBOSS*)m_ObjList[enBOSS].GetNext(ePos1);
				CRect mRect = pBOSS->GetRect();
				CRect tmpRect;
				if (tmpRect.IntersectRect(&bRect, mRect))
				{
					//删除导弹
					m_ObjList[enBomb1].RemoveAt(mPos2);
					delete pBomb1;
					//添加爆炸效果
					m_ObjList[enExplosion].AddTail(new CExplosion(bRect.left, bRect.top));

					pBOSS->bosslife -= 1;

					if (pBOSS->bosslife <= 0)
					{
						m_ObjList[enExplosion].AddTail(new CExplosion(mRect.left, mRect.top));
						//删除boss
						m_ObjList[enBOSS].RemoveAt(ePos2);
						score += 10;
						delete pBOSS;
						//bossnum = 0;
						SetStage(++stage);
						ncreatorboss = 400;
						break;
					}

				}
			}
		}

		//僚机导弹攻击BOSS2
		for (mPos1 = m_ObjList[enBomb1].GetHeadPosition(); (mPos2 = mPos1) != NULL;)
		{
			CBomb1* pBomb1 = (CBomb1*)m_ObjList[enBomb1].GetNext(mPos1);
			CRect bRect = pBomb1->GetRect();

			POSITION ePos1 = NULL, ePos2 = NULL;
			for (ePos1 = m_ObjList[enBOSS2].GetHeadPosition(); (ePos2 = ePos1) != NULL;)
			{
				CBOSS2* pBOSS2 = (CBOSS2*)m_ObjList[enBOSS2].GetNext(ePos1);
				CRect mRect = pBOSS2->GetRect();
				CRect tmpRect;
				if (tmpRect.IntersectRect(&bRect, mRect))
				{
					//删除导弹
					m_ObjList[enBomb1].RemoveAt(mPos2);
					delete pBomb1;
					//添加爆炸效果
					m_ObjList[enExplosion].AddTail(new CExplosion(bRect.left, bRect.top));

					pBOSS2->bosslife -= 1;

					if (pBOSS2->bosslife <= 0)
					{
						m_ObjList[enExplosion].AddTail(new CExplosion(mRect.left, mRect.top));
						//删除boss
						m_ObjList[enBOSS2].RemoveAt(ePos2);
						score += 10;
						delete pBOSS2;
						//bossnum = 0;
						SetStage(++stage);
						ncreatorboss = 400;
						break;
					}

				}
			}
		}


		//僚机导弹攻击BOSS3
		for (mPos1 = m_ObjList[enBomb1].GetHeadPosition(); (mPos2 = mPos1) != NULL;)
		{
			CBomb1* pBomb1 = (CBomb1*)m_ObjList[enBomb1].GetNext(mPos1);
			CRect bRect = pBomb1->GetRect();

			POSITION ePos1 = NULL, ePos2 = NULL;
			for (ePos1 = m_ObjList[enBOSS3].GetHeadPosition(); (ePos2 = ePos1) != NULL;)
			{
				CBOSS3* pBOSS3 = (CBOSS3*)m_ObjList[enBOSS3].GetNext(ePos1);
				CRect mRect = pBOSS3->GetRect();
				CRect tmpRect;
				if (tmpRect.IntersectRect(&bRect, mRect))
				{
					//删除导弹
					m_ObjList[enBomb1].RemoveAt(mPos2);
					delete pBomb1;
					//添加爆炸效果
					m_ObjList[enExplosion].AddTail(new CExplosion(bRect.left, bRect.top));

					pBOSS3->bosslife -= 1;

					if (pBOSS3->bosslife <= 0)
					{
						m_ObjList[enExplosion].AddTail(new CExplosion(mRect.left, mRect.top));
						//删除boss
						m_ObjList[enBOSS3].RemoveAt(ePos2);
						score += 10;
						delete pBOSS3;
						//bossnum = 0;
						SetStage(++stage);
						ncreatorboss = 400;
						break;
					}

				}
			}
		}





		//玩家技能攻击敌机
		for (mPos1 = m_ObjList[enSkill].GetHeadPosition(); (mPos2 = mPos1) != NULL;)
		{
			CSkill* pSkill = (CSkill*)m_ObjList[enSkill].GetNext(mPos1);
			CRect bRect = pSkill->GetRect();

			POSITION ePos1 = NULL, ePos2 = NULL;
			for (ePos1 = m_ObjList[enEnemy].GetHeadPosition(); (ePos2 = ePos1) != NULL;)
			{
				CEnemy* pEnemy = (CEnemy*)m_ObjList[enEnemy].GetNext(ePos1);
				CRect mRect = pEnemy->GetRect();
				CRect tmpRect;
				if (tmpRect.IntersectRect(&bRect, mRect))
				{
					//添加爆炸效果
					m_ObjList[enExplosion].AddTail(
						new CExplosion(mRect.left, mRect.top)
					);

					//删除敌机
					m_ObjList[enEnemy].RemoveAt(ePos2);
					score++;
					delete pEnemy;
					break;
				}
			}
		}


		//玩家技能攻击boss
		for (mPos1 = m_ObjList[enSkill].GetHeadPosition(); (mPos2 = mPos1) != NULL;)
		{
			CSkill* pSkill = (CSkill*)m_ObjList[enSkill].GetNext(mPos1);
			CRect bRect = pSkill->GetRect();

			POSITION ePos1 = NULL, ePos2 = NULL;
			for (ePos1 = m_ObjList[enBOSS].GetHeadPosition(); (ePos2 = ePos1) != NULL;)
			{
				CBOSS* pBOSS = (CBOSS*)m_ObjList[enBOSS].GetNext(ePos1);
				CRect mRect = pBOSS->GetRect();
				CRect tmpRect;
				if (tmpRect.IntersectRect(&bRect, mRect))
				{
					static int i = 0;
					if (i == 0)
						pBOSS->bosslife -= 1;
					i++;
					if (i >= 8)
						i = 0;

					//添加爆炸效果
					m_ObjList[enExplosion].AddTail(new CExplosion(bRect.left, bRect.top));

					if (pBOSS->bosslife <= 0)
					{
						m_ObjList[enExplosion].AddTail(new CExplosion(mRect.left, mRect.top));
						//删除boss
						m_ObjList[enBOSS].RemoveAt(ePos2);
						score += 10;
						delete pBOSS;
						SetStage(++stage);
						ncreatorboss = 400;
						break;
					}
				}
			}
		}
		
		//玩家技能攻击boss2
		for (mPos1 = m_ObjList[enSkill].GetHeadPosition(); (mPos2 = mPos1) != NULL;)
		{
			CSkill* pSkill = (CSkill*)m_ObjList[enSkill].GetNext(mPos1);
			CRect bRect = pSkill->GetRect();

			POSITION ePos1 = NULL, ePos2 = NULL;
			for (ePos1 = m_ObjList[enBOSS2].GetHeadPosition(); (ePos2 = ePos1) != NULL;)
			{
				CBOSS2* pBOSS2 = (CBOSS2*)m_ObjList[enBOSS2].GetNext(ePos1);
				CRect mRect = pBOSS2->GetRect();
				CRect tmpRect;
				if (tmpRect.IntersectRect(&bRect, mRect))
				{

					static int i = 0;
					if (i == 0)
						pBOSS2->bosslife -= 1;
					i++;
					if (i >= 8)
						i = 0;

					//添加爆炸效果
					m_ObjList[enExplosion].AddTail(new CExplosion(bRect.left, bRect.top));

					if (pBOSS2->bosslife <= 0)
					{
						m_ObjList[enExplosion].AddTail(new CExplosion(mRect.left, mRect.top));
						//删除boss
						m_ObjList[enBOSS2].RemoveAt(ePos2);
						score += 10;
						delete pBOSS2;
						SetStage(++stage);
						ncreatorboss = 400;
						break;
					}
				}
			}
		}
		
		
		//玩家技能攻击boss3
		for (mPos1 = m_ObjList[enSkill].GetHeadPosition(); (mPos2 = mPos1) != NULL;)
		{
			CSkill* pSkill = (CSkill*)m_ObjList[enSkill].GetNext(mPos1);
			CRect bRect = pSkill->GetRect();

			POSITION ePos1 = NULL, ePos2 = NULL;
			for (ePos1 = m_ObjList[enBOSS3].GetHeadPosition(); (ePos2 = ePos1) != NULL;)
			{
				CBOSS3* pBOSS3 = (CBOSS3*)m_ObjList[enBOSS3].GetNext(ePos1);
				CRect mRect = pBOSS3->GetRect();
				CRect tmpRect;
				if (tmpRect.IntersectRect(&bRect, mRect))
				{

					static int i = 0;
					if (i == 0)
						pBOSS3->bosslife -= 1;
					i++;
					if (i >= 8)
						i = 0;

					//添加爆炸效果
					m_ObjList[enExplosion].AddTail(new CExplosion(bRect.left, bRect.top));

					if (pBOSS3->bosslife <= 0)
					{
						m_ObjList[enExplosion].AddTail(new CExplosion(mRect.left, mRect.top));
						//删除boss
						m_ObjList[enBOSS3].RemoveAt(ePos2);
						score += 10;
						delete pBOSS3;
						SetStage(++stage);
						ncreatorboss = 400;
						break;
					}
				}
			}
		}



}


void CPlaneGameView::OnTimer(UINT_PTR nIDEvent)
{
	if (GetKey('P') == 1)//按下了p键
	{
		if (ispause <= 0 && pausetrue == 1)
			pausetrue = 0;
		else
			if (ispause <= 0 && pausetrue == 0)
			{
				pausetrue = 1;
			}
		ispause = 3;
	}

	ispause--;

	if (pausetrue == 1)
	{
		//刷新游戏帧画面: 在内存DC上绘图
		UpdateFrame(m_pMemDC);

		if (delay != 0)
			delay++;
		if (delay > 10)
			delay = 0;

		if (shieldtime > 0)    //护盾时间
			shieldtime--;
		if (shieldtime == 0)
			shield = 0;

		AI();
	}
	CView::OnTimer(nIDEvent);
}


void CPlaneGameView::SetStage(int s)  //设置关卡
{
	stage = s;
	bossnum = 0;
	if (stage <= 3)
	{
		selectbackground = stage;
		selectboss = stage;
	}
		
	else
	{
		selectbackground = rand() % 5 + 1;
		selectboss = rand() % 3 + 1;
	}
		
}