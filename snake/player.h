#pragma once


// player 对话框

class player : public CDialogEx
{
	DECLARE_DYNAMIC(player)

public:
	player(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~player();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	// 用来记录用户名
	CString playername;
	afx_msg void OnBnClickedOk();
};
