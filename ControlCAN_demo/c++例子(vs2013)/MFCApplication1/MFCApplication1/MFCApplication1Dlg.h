
// MFCApplication1Dlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"

#include <vector>

// CMFCApplication1Dlg �Ի���
class CMFCApplication1Dlg : public CDialogEx
{
// ����
public:
	CMFCApplication1Dlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MFCAPPLICATION1_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
