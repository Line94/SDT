
// MFCApplication1Dlg.h : 头文件
//

#pragma once
#include "afxwin.h"

#include <vector>

// CMFCApplication1Dlg 对话框
class CMFCApplication1Dlg : public CDialogEx
{
// 构造
public:
	CMFCApplication1Dlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_MFCAPPLICATION1_DIALOG };

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
    afx_msg void OnBnClickedButtonConnect();
    afx_msg void OnBnClickedButtonStartcan();
    afx_msg void OnBnClickedButtonSend();
    afx_msg void OnCbnSelchangeComboType();
    afx_msg void OnCbnSelchangeComboWorkmode();
    afx_msg void OnCbnSelchangeComboBaudrate();
    afx_msg void OnTimer(UINT_PTR nIDEvent);
    afx_msg void OnDestroy();

private:
    void EnableSet();
    void WriteCfgFile();
    UINT connectUSBCANEU();
    UINT connectCANDTU();

private:
    CComboBox m_index;
    CComboBox m_channel;
    CComboBox m_baudrate;
    CComboBox m_mode;
    CComboBox m_filter;
    CComboBox m_workmode;
    CComboBox m_sendformat;
    CComboBox m_frametype;
    CComboBox m_frameformat;
    CComboBox m_type;
    CListBox m_listInfo;

    std::vector<int> m_devicetypes;
    bool m_open{ false };
    bool m_started{ false };
    int m_devtype{ 0 };
    int m_devindex{ 0 };
    int m_canindex{ 0 };
    UINT_PTR m_timerId{ 12345 };
};
