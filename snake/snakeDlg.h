
// snakeDlg.h: 头文件
//

#pragma once
#include<list>
using namespace std;

#define GAME_ROW 22
#define  GAME_COL 32
#define  GAMRBLOCK 20
#define  GAME_CORDER  1
#define  SNAKE_MOVE 100
#define GAME_FOOD 3
typedef  struct SNAKE
{
	int nRow;     //蛇身在界面的哪行
	int nCOl;     //蛇身在界面的哪列
}SNAKE;           //蛇的身体结构


enum MOVEENUM
{
	LEFT,
	RIGHT,
	UP,
	DOWN,
};

// CsnakeDlg 对话框
class CsnakeDlg : public CDialogEx
{
// 构造
public:
	CsnakeDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SNAKE_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	void InitGame();                                           //初始化游戏
	void DrawGameWnd(CDC &dc);                 //画游戏背景
	void DrawSnake(CDC &dc);                        //画蛇
	void SnakeMove();                                     //蛇移动的逻辑
	void CreatFood();                                      //食物产生的逻辑
	void Inputname();                                     //用户信息的输入和保存
	int  score;
private:
	byte m_bGameArr[GAME_ROW][GAME_COL];      //游戏背景的二维数组
	list<SNAKE> m_listSnake;                            //蛇的身体
	MOVEENUM m_snakeMove;                      //蛇的移动
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);          //用于静态文本的背景透明化和文字的颜色的改变
	virtual BOOL PreTranslateMessage(MSG* pMsg);                               //重载虚函数以解决键盘失灵的问题
};
