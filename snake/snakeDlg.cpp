
// snakeDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "snake.h"
#include "snakeDlg.h"
#include "afxdialogex.h"
#include"player.h"               //用户的对话框
#include<fstream>       //将用户和分数写入文件中
#include<windows.h>    //
#include"mmsystem.h"   //
using namespace std;
#pragma comment(lib,"winmm.lib")   //
 

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CsnakeDlg 对话框



CsnakeDlg::CsnakeDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SNAKE_DIALOG, pParent)
	,m_snakeMove(LEFT)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);   //将图标改为自己找到的图标
	
}

void CsnakeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CsnakeDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_WM_KEYDOWN()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CsnakeDlg 消息处理程序

BOOL CsnakeDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	InitGame();                                 //初始化游戏的界面

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CsnakeDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CsnakeDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		//双缓冲，防止闪烁
		CRect   rt;
		GetClientRect(rt);

		CDC dcMem;  //定义一个显示设备对象
		dcMem.CreateCompatibleDC(&dc);
		CBitmap bmp , *oldbmp;          //定义位图
		bmp.CreateCompatibleBitmap(&dc, rt.Width(), rt.Height());
		oldbmp=dcMem.SelectObject(&bmp);    //将位图选入到内存显示设备中
		dcMem.FillSolidRect(rt,dc.GetBkColor());    //用背景色将屏幕清除干净

		DrawGameWnd(dcMem);
		DrawSnake(dcMem);

		dc.BitBlt(0, 0, rt.Width(), rt.Height(),&dcMem,0,0,SRCCOPY);

		//释放dc资源
		dcMem.SelectObject(oldbmp);
		dcMem.DeleteDC();
		bmp.DeleteObject();

		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CsnakeDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CsnakeDlg::InitGame()
{
	ZeroMemory(m_bGameArr, sizeof(m_bGameArr));
	score = 0;
	for (int i = 0; i < GAME_ROW; i++)
	{
		for (int j = 0; j < GAME_COL; j++)
		{
			if (i == 0 ||  i == GAME_ROW - 1  ||  j == 0  ||  j == GAME_COL-1)
			{
				m_bGameArr[i][j] = GAME_CORDER;
			}
		}
	}
	//游戏初始化要将蛇清空
	m_listSnake.clear();

	//定义开始游戏时，蛇的位置
	SNAKE sTmp;
	sTmp.nCOl = 8;
	sTmp.nRow = 8;
	for (int i = 0; i < 5; i++)
	{
		m_listSnake.push_back(sTmp);
		sTmp.nCOl++;
	}

	//设定种子，产生食物的行和列的随机数
	srand(time(0));

	CreatFood();
	//背景音乐，重复播放
	PlaySound(TEXT("res/Easy Breeze.wav"), NULL, SND_FILENAME | SND_ASYNC|SND_LOOP);
	SetTimer(SNAKE_MOVE, 500, NULL);
}

void CsnakeDlg::DrawGameWnd(CDC & dc)
{
	CBrush brushGame(RGB(245,245,220));    //游戏区域颜色
	CBrush brushBorder(RGB(102,205,170));   //边框颜色
	CBrush brushFood(RGB(238, 130, 238));   //食物颜色
	for (int i = 0; i < GAME_ROW; i++)
	{
		for (int j = 0; j < GAME_COL; j++)
		{
			CRect rt;
			rt.left = j * GAMRBLOCK;
			rt.top = i * GAMRBLOCK;
			rt.right = rt.left + GAMRBLOCK;
			rt.bottom = rt.top + GAMRBLOCK;
			if (m_bGameArr[i][j] == GAME_CORDER)
			{
				dc.FillRect(rt, &brushBorder);
			}
			else if (m_bGameArr[i][j] == GAME_FOOD)
			{
				dc.FillRect(rt, &brushFood);
			}
			else
			{
				dc.FillRect(rt, &brushGame);
			}
		}
	 }
}

void CsnakeDlg::DrawSnake(CDC &dc)
{
	CBrush brushSnake(RGB(176, 224, 230));       //蛇的颜色
	for (list<SNAKE>::iterator it = m_listSnake.begin(); 
		it != m_listSnake.end(); it++)
	{ 
		CRect rt;
		rt.left =  it->nCOl * GAMRBLOCK;
		rt.top = it->nRow * GAMRBLOCK;
		rt.right = rt.left + GAMRBLOCK;
		rt.bottom = rt.top + GAMRBLOCK;
 
		if (it == m_listSnake.begin())
		{
			dc.SelectObject(&brushSnake);
			dc.Rectangle(rt);
		}
		else
		{
			dc.FillRect(rt, &brushSnake);
		}
	}
	 
}

void CsnakeDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == SNAKE_MOVE)
	{
		 SnakeMove();
	 }
	CDialogEx::OnTimer(nIDEvent);
}

void CsnakeDlg::SnakeMove()
{
	list<SNAKE>::iterator itHead = m_listSnake.end();
	list<SNAKE>::iterator itBody = m_listSnake.end();
	itBody--;
	itHead--;
	
	//蛇头的时候要特殊设置，以区别蛇的前后
	while (itHead != m_listSnake.begin())
	{
		itHead--;
		itBody->nCOl = itHead->nCOl;
		itBody->nRow = itHead->nRow;
		itBody--;
	}

	itHead = m_listSnake.begin();

	//蛇吃食物的逻辑
	if (m_bGameArr[itHead->nRow][itHead->nCOl] == GAME_FOOD)
	{
		score += 10;
		m_bGameArr[itHead->nRow][itHead->nCOl] = 0;
		m_listSnake.push_back(*itHead);
		CreatFood();
	}

	//蛇撞墙，游戏结束的逻辑
	if (m_bGameArr[itHead->nRow][itHead->nCOl] == GAME_CORDER)
	{
		KillTimer(SNAKE_MOVE);
		PlaySound(NULL, NULL, SND_FILENAME);
		Inputname();            //链接输入用户名的对话框
		CString   string;
		string.Format(_T("你的得分是：%d\n"), score);
		MessageBox(string);
		if (MessageBox(_T("Game Over!\r\n是否重新开始"), _T(" 提示"), MB_YESNO) == IDYES)
		{
			InitGame();
		}
		return;
	} 

	//蛇的移动
	switch (m_snakeMove)
	{
	case LEFT:
		itHead->nCOl -= 1;
		break;
	case RIGHT:
		itHead->nCOl += 1;
		break;
	case UP:
		itHead->nRow -= 1;
		break;
	case DOWN:
		itHead->nRow += 1;
		break;
	}
	Invalidate();
}

//通过虚函数重载来是键盘消息得到响应，使得键盘可以控制蛇的移动
BOOL CsnakeDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
		case  'A':
			m_snakeMove = LEFT;
			break;
		case 'D':
			m_snakeMove = RIGHT;
			break;
		case'W':
			m_snakeMove = UP;
			break;
		case 'S':
			m_snakeMove = DOWN;
			break;
		}
		Invalidate();
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}
 

void CsnakeDlg::CreatFood()
{
	int x = rand() % (GAME_ROW - 2);    //产生0--GAME_ROW-2的数
	x = x + 1;          //产生0--GAME_ROW-1的数
	
	int y = rand() % (GAME_COL - 2);
	y = y + 1;

	for (list<SNAKE>::iterator it = m_listSnake.begin();
		it != m_listSnake.end(); it++)
	{
		if ( x == it->nRow && y == it->nCOl)
			continue;
		m_bGameArr[x][y] = GAME_FOOD;
	}
}

//改变静态文字的颜色和背景透明化
HBRUSH CsnakeDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	if ((pWnd->GetDlgCtrlID() == IDC_STATIC))         //在该对话框中要进行透明处理的静态文本地址
	{
		pDC->SetTextColor(RGB(0, 0, 139));//设置静态文本框字体颜色
		pDC->SetBkMode(TRANSPARENT);//设置为透明
		return HBRUSH(GetStockObject(HOLLOW_BRUSH));//设置透明这句话不能掉
	}
	 
	return hbr;
}

//将输入用户名的对话框弹出去
void CsnakeDlg::Inputname()
{
	INT_PTR nRes;             // 用于保存DoModal函数的返回值   

	player tipDlg;           // 构造对话框类CTipDlg的实例   
	nRes = tipDlg.DoModal();  // 弹出对话框   
	if (IDCANCEL == nRes)     // 判断对话框退出后返回值是否为IDCANCEL，如果是则return，否则继续向下执行   
		return;

	// 将各控件中的数据保存到相应的变量   
	UpdateData(TRUE);

	USES_CONVERSION;
	//函数T2A和W2A均支持ATL和MFC中的字符
	char * pFileName = T2A(tipDlg.playername);
	int length = tipDlg.playername.GetLength();

	ofstream outfile("snake.txt",ios::app);
	outfile << "玩家：\t得分：" << endl;
	for (int i = 0; i < length; i++)
	{
		outfile<< pFileName[i];
	}
	outfile <<"\t"<<score<<endl;
	outfile.close();

	// 根据各变量的值更新相应的控件。和的编辑框会显示m_editSum的值   
	UpdateData(FALSE);
}
