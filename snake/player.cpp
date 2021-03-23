// player.cpp: 实现文件
//

#include "stdafx.h"
#include "snake.h"
#include "player.h"
#include "afxdialogex.h"


// player 对话框

IMPLEMENT_DYNAMIC(player, CDialogEx)

player::player(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG2, pParent)
	, playername(_T(""))
{

}

player::~player()
{
}

void player::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, playername);
}


BEGIN_MESSAGE_MAP(player, CDialogEx)
	ON_BN_CLICKED(IDOK, &player::OnBnClickedOk)
END_MESSAGE_MAP()


// player 消息处理程序


void player::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	// 将各控件中的数据保存到相应的变量   
	UpdateData(TRUE);

	// 根据各变量的值更新相应的控件。和的编辑框会显示m_editSum的值   
	UpdateData(FALSE);

	CDialogEx::OnOK();
}


 
