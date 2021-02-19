
// MFCApplication1Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MFCApplication1.h"
#include "MFCApplication1Dlg.h"
#include "afxdialogex.h"

#include <string>

#include "ControlCAN.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#pragma comment(lib, "ControlCAN.lib")

namespace {
    const TCHAR* cfgFile = TEXT("candtu.ini");
    const UINT clock = 24000000;

    struct IM28X_TSEG2_TSEG1_SJW
    {
        BYTE tSeg2;
        BYTE tSeg1Min;
        BYTE tSeg1Max;
        BYTE rSjwMin;
        BYTE rSjwMax;
        IM28X_TSEG2_TSEG1_SJW(BYTE seg2, BYTE seg1min, BYTE seg1max, BYTE sjwmin, BYTE sjwmax)
        {
            tSeg2 = seg2;
            tSeg1Min = seg1min;
            tSeg1Max = seg1max;
            rSjwMin = sjwmin;
            rSjwMax = sjwmax;
        }
    };

    IM28X_TSEG2_TSEG1_SJW im28xTseg2Tseg1SjwTable[] =
    {
        IM28X_TSEG2_TSEG1_SJW(2, 5, 10, 1, 2),
        IM28X_TSEG2_TSEG1_SJW(3, 4, 11, 1, 3),
        IM28X_TSEG2_TSEG1_SJW(4, 5, 12, 1, 4),
        IM28X_TSEG2_TSEG1_SJW(5, 6, 13, 1, 4),
        IM28X_TSEG2_TSEG1_SJW(6, 7, 14, 1, 4),
        IM28X_TSEG2_TSEG1_SJW(7, 8, 15, 1, 4),
        IM28X_TSEG2_TSEG1_SJW(8, 9, 16, 1, 4)
    };

    bool IsTseg2Tseg1SjwMatchIM28X(byte tSeg2, byte tSeg1, byte rSjw)
    {
        for (int i = 0; i < sizeof(im28xTseg2Tseg1SjwTable) / sizeof(im28xTseg2Tseg1SjwTable[0]); ++i)
        {
            if (tSeg2 == im28xTseg2Tseg1SjwTable[i].tSeg2)
            {
                if (tSeg1 >= im28xTseg2Tseg1SjwTable[i].tSeg1Min &&
                    tSeg1 <= im28xTseg2Tseg1SjwTable[i].tSeg1Max &&
                    rSjw >= im28xTseg2Tseg1SjwTable[i].rSjwMin &&
                    rSjw <= im28xTseg2Tseg1SjwTable[i].rSjwMax)
                {
                    return true;
                }
            }
        }
        return false;
    }

    CString hex2dec(const CString& hex) {
#ifdef _UNICODE
        const std::wstring str(hex.GetString());
        unsigned long val = std::stoul(str, 0, 16);
        return std::to_wstring(val).c_str();
#else
        const std::string str(hex.GetString());
        unsigned long val = std::stoul(str, 0, 16);
        return std::to_string(val).c_str();
#endif
    }

    INT32 str2int(const CString& str, int base = 10) {
#ifdef _UNICODE
        std::wstring s(str.GetString());
#else
        std::string s(str.GetString());
#endif
        return std::stoi(s, 0, base);
    }

    UINT32 str2uint(const CString& str, int base = 10) {
#ifdef _UNICODE
        std::wstring s(str.GetString());
#else
        std::string s(str.GetString());
#endif
        return std::stoul(s, 0, base);
    }


    CString int2str(int val, int base = 10) {
        CString ret;
        ret.Format(10 == base ? TEXT("%d") : TEXT("%X"), val);
        return ret;
    }

    struct CANDTUBPS {
        BYTE tseg1;
        BYTE tseg2;
        USHORT brp;
        UINT sjw;
        BOOL trismp;
    };

    struct DtuComConfig {
        USHORT len;
        CString data;
    };

    BOOL getCfg(UINT desiredBaud, CANDTUBPS& cfg)
    {
        byte SJW = 0;
        for (byte TSEG2 = (byte)(SJW + 1); TSEG2 <= 8; TSEG2++)
        {
            for (byte TSEG1 = (byte)(TSEG2 + 1); TSEG1 <= 16; TSEG1++)
            {
                for (USHORT BRP = 1; BRP <= 256; ++BRP)
                {
                    UINT baud = clock / ((UINT)(1 + TSEG1 + TSEG2) * BRP);
                    if ((baud > desiredBaud * 995 / 1000) && (baud < desiredBaud * 1005 / 1000))
                    {
                        if (IsTseg2Tseg1SjwMatchIM28X(TSEG2, TSEG1, (byte)(SJW + 1)))
                        {
                            cfg.tseg1 = (byte)(TSEG1 - 2);
                            cfg.tseg2 = (byte)(TSEG2 - 1);
                            cfg.sjw = SJW;
                            cfg.brp = (byte)(BRP - 1);
                            cfg.trismp = FALSE;
                            return TRUE;
                        }
                    }
                }
            }
        }
        cfg.tseg1 = 0;
        cfg.tseg2 = 0;
        cfg.sjw = 0;
        cfg.brp = 0;
        cfg.trismp = FALSE;
        return FALSE;
    }

    std::vector<BYTE> IntToBytes(UINT64 num, int bits) {
        std::vector<BYTE> b(bits);
        for (int i = 0; i < bits; ++i) {
            b[i] = ((num >> (i * 8)) & 0xFF);
        }
        return b;
    }
}

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
    CAboutDlg();

    // 对话框数据
    enum { IDD = IDD_ABOUTBOX };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

    // 实现
protected:
    DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFCApplication1Dlg 对话框



CMFCApplication1Dlg::CMFCApplication1Dlg(CWnd* pParent /*=NULL*/)
: CDialogEx(CMFCApplication1Dlg::IDD, pParent)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCApplication1Dlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_COMBO_INDEX, m_index);
    DDX_Control(pDX, IDC_COMBO_CHANNEL, m_channel);
    DDX_Control(pDX, IDC_COMBO_BAUDRATE, m_baudrate);
    DDX_Control(pDX, IDC_COMBO_MODE, m_mode);
    DDX_Control(pDX, IDC_COMBO_FILTER, m_filter);
    DDX_Control(pDX, IDC_COMBO_WORKMODE, m_workmode);
    DDX_Control(pDX, IDC_COMBO_SENDFORMAT, m_sendformat);
    DDX_Control(pDX, IDC_COMBO_FRAMETYPE, m_frametype);
    DDX_Control(pDX, IDC_COMBO_FRAMEFORMAT, m_frameformat);
    DDX_Control(pDX, IDC_COMBO_TYPE, m_type);
    DDX_Control(pDX, IDC_LIST_INFO, m_listInfo);
}

BEGIN_MESSAGE_MAP(CMFCApplication1Dlg, CDialogEx)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BUTTON_CONNECT, &CMFCApplication1Dlg::OnBnClickedButtonConnect)
    ON_BN_CLICKED(IDC_BUTTON_STARTCAN, &CMFCApplication1Dlg::OnBnClickedButtonStartcan)
    ON_BN_CLICKED(IDC_BUTTON_SEND, &CMFCApplication1Dlg::OnBnClickedButtonSend)
    ON_CBN_SELCHANGE(IDC_COMBO_TYPE, &CMFCApplication1Dlg::OnCbnSelchangeComboType)
    ON_WM_DESTROY()
    ON_CBN_SELCHANGE(IDC_COMBO_WORKMODE, &CMFCApplication1Dlg::OnCbnSelchangeComboWorkmode)
    ON_WM_TIMER()
    ON_CBN_SELCHANGE(IDC_COMBO_BAUDRATE, &CMFCApplication1Dlg::OnCbnSelchangeComboBaudrate)
END_MESSAGE_MAP()


// CMFCApplication1Dlg 消息处理程序

BOOL CMFCApplication1Dlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // 将“关于...”菜单项添加到系统菜单中。

    // IDM_ABOUTBOX 必须在系统命令范围内。
    ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
    ASSERT(IDM_ABOUTBOX < 0xF000);

    CMenu* pSysMenu = GetSystemMenu(FALSE);
    if (pSysMenu != NULL)
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

    // TODO:  在此添加额外的初始化代码
    for (int i = 0; i < 8; ++i) {
        CString str;
        str.Format(TEXT("%d"), i);
        m_index.AddString(str);
    }
    m_index.SetCurSel(0);
    for (int i = 0; i < 4; ++i) {
        CString str;
        str.Format(TEXT("%d"), i);
        m_channel.AddString(str);
    }
    m_channel.SetCurSel(0);
    SetDlgItemText(IDC_EDIT_TIMER0, TEXT("00"));
    SetDlgItemText(IDC_EDIT_TIMER1, TEXT("1C"));

    LPCTSTR baudrates[] = {
        TEXT("5Kbps"),
        TEXT("10Kbps"),
        TEXT("20Kbps"),
        TEXT("40Kbps"),
        TEXT("50Kbps"),
        TEXT("80Kbps"),
        TEXT("100Kbps"),
        TEXT("125Kbps"),
        TEXT("200Kbps"),
        TEXT("250Kbps"),
        TEXT("400Kbps"),
        TEXT("500Kbps"),
        TEXT("666Kbps"),
        TEXT("800Kbps"),
        TEXT("1000Kbps")
    };
    for (int i = 0; i < sizeof(baudrates) / sizeof(baudrates[0]); ++i) {
        m_baudrate.AddString(baudrates[i]);
    }
    m_baudrate.SetCurSel(m_baudrate.FindString(0, TEXT("500Kbps")));
    m_mode.AddString(TEXT("正常"));
    m_mode.AddString(TEXT("只听"));
    m_mode.SetCurSel(0);
    SetDlgItemText(IDC_EDIT_ACCCODE, TEXT("00000000"));
    SetDlgItemText(IDC_EDIT_MASKCODE, TEXT("FFFFFFFF"));
    m_filter.AddString(TEXT("双滤波"));
    m_filter.AddString(TEXT("单滤波"));
    m_filter.SetCurSel(1);
    m_workmode.AddString(TEXT("服务器"));
    m_workmode.AddString(TEXT("客户端"));
    m_workmode.SetCurSel(0);
    SetDlgItemText(IDC_EDIT_REMOTEADDR, TEXT("192.168.28.224"));
    SetDlgItemText(IDC_EDIT_LOCALPORT, TEXT("1008"));
    SetDlgItemText(IDC_EDIT_REMOTEPORT, TEXT("4001"));
    m_sendformat.AddString(TEXT("正常发送"));
    m_sendformat.AddString(TEXT("单次正常发送"));
    m_sendformat.AddString(TEXT("自发自收"));
    m_sendformat.AddString(TEXT("单次自发自收"));
    m_sendformat.SetCurSel(0);
    m_frametype.AddString(TEXT("标准帧"));
    m_frametype.AddString(TEXT("扩展帧"));
    m_frametype.SetCurSel(0);
    m_frameformat.AddString(TEXT("数据帧"));
    m_frameformat.AddString(TEXT("远程帧"));
    m_frameformat.SetCurSel(0);
    SetDlgItemText(IDC_EDIT_FRAMEID, TEXT("00000123"));
    SetDlgItemText(IDC_EDIT_DATA, TEXT("00 01 02 03 04 05 06 07"));
    SetDlgItemText(IDC_EDIT_FRAMENUM, TEXT("2"));

    m_type.AddString(TEXT("VCI_PCI5121"));
    m_devicetypes.push_back(VCI_PCI5121);
    m_type.AddString(TEXT("VCI_PCI9810"));
    m_devicetypes.push_back(VCI_PCI9810);
    m_type.AddString(TEXT("VCI_USBCAN1(I+)"));
    m_devicetypes.push_back(VCI_USBCAN1);
    m_type.AddString(TEXT("VCI_USBCAN2(II+)"));
    m_devicetypes.push_back(VCI_USBCAN2);
    m_type.AddString(TEXT("VCI_USBCAN2A"));
    m_devicetypes.push_back(VCI_USBCAN2A);
    m_type.AddString(TEXT("VCI_PCI9820"));
    m_devicetypes.push_back(VCI_PCI9820);
    m_type.AddString(TEXT("VCI_PCI5110"));
    m_devicetypes.push_back(VCI_PCI5110);
    m_type.AddString(TEXT("VCI_CANLITE"));
    m_devicetypes.push_back(VCI_CANLITE);
    m_type.AddString(TEXT("VCI_ISA9620"));
    m_devicetypes.push_back(VCI_ISA9620);
    m_type.AddString(TEXT("VCI_ISA5420"));
    m_devicetypes.push_back(VCI_ISA5420);
    m_type.AddString(TEXT("VCI_PC104CAN"));
    m_devicetypes.push_back(VCI_PC104CAN);
    m_type.AddString(TEXT("VCI_DNP9810"));
    m_devicetypes.push_back(VCI_DNP9810);
    m_type.AddString(TEXT("VCI_PCI9840"));
    m_devicetypes.push_back(VCI_PCI9840);
    m_type.AddString(TEXT("VCI_PC104CAN2"));
    m_devicetypes.push_back(VCI_PC104CAN2);
    m_type.AddString(TEXT("VCI_PCI9820I"));
    m_devicetypes.push_back(VCI_PCI9820I);
    m_type.AddString(TEXT("VCI_PEC9920"));
    m_devicetypes.push_back(VCI_PEC9920);
    m_type.AddString(TEXT("VCI_PCIE9221"));
    m_devicetypes.push_back(VCI_PCIE9221);
    m_type.AddString(TEXT("VCI_CANDTU200"));
    m_devicetypes.push_back(VCI_CANDTU_200UR);
    m_type.AddString(TEXT("VCI_CANETTCP"));
    m_devicetypes.push_back(VCI_CANETTCP);
    m_type.AddString(TEXT("VCI_CANETUDP"));
    m_devicetypes.push_back(VCI_CANETUDP);
    m_type.AddString(TEXT("VCI_USBCAN_E_U"));
    m_devicetypes.push_back(VCI_USBCAN_E_U);
    m_type.AddString(TEXT("VCI_USBCAN_2E_U"));
    m_devicetypes.push_back(VCI_USBCAN_2E_U);
    m_type.SetCurSel(3);

    EnableSet();

    SetDlgItemText(IDC_BUTTON_CONNECT, m_open ? TEXT("断开") : TEXT("连接"));
    GetDlgItem(IDC_BUTTON_STARTCAN)->EnableWindow(m_open);

    return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMFCApplication1Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMFCApplication1Dlg::OnPaint()
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
        CDialogEx::OnPaint();
    }
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMFCApplication1Dlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}



void CMFCApplication1Dlg::OnBnClickedButtonConnect()
{
    // TODO:  在此添加控件通知处理程序代码
    const int index = m_type.GetCurSel();
    const UINT type = m_devicetypes.at(index);
    const bool isNetDevice = (type == VCI_CANETTCP || type == VCI_CANETUDP);
    if (m_open) {
        VCI_CloseDevice(m_devtype, m_devindex);
        m_open = false;
    }
    else {
        m_devtype = type;
        m_devindex = m_index.GetCurSel();
        m_canindex = m_channel.GetCurSel();
        CString text;
        GetDlgItemText(IDC_EDIT_LOCALPORT, text);
        if (0 == VCI_OpenDevice(m_devtype, m_devindex, str2uint(text))) {
            MessageBox(TEXT("打开设备失败，请检查设备类型和设备索引号是否正确"), TEXT("错误"), MB_OK | MB_ICONEXCLAMATION);
            return;
        }

        VCI_INIT_CONFIG config;
        if (isNetDevice) {
            if (type == VCI_CANETTCP) // TCP
            {
                if (m_workmode.GetCurSel() == 0) // Server
                {
                    UINT port = str2uint(text);
                    std::vector<BYTE> ports = IntToBytes(port, 4);
                    UINT32 cmdtype = CMD_SRCPORT;
                    if (STATUS_OK != VCI_SetReference(m_devtype, m_devindex, m_canindex, cmdtype, ports.data()))
                    {
                        MessageBox(TEXT("设置本地端口失败"));
                        return;
                    }
                    cmdtype = CMD_TCP_TYPE;
                    UINT tcpType = TCP_SERVER;
                    std::vector<BYTE> types = IntToBytes(tcpType, 4);
                    if (STATUS_OK != VCI_SetReference(m_devtype, m_devindex, m_canindex, cmdtype, types.data()))
                    {
                        MessageBox(TEXT("设置TCP工作方式失败"));
                        return;
                    }
                }
                else // Client
                {
                    CString ip;
                    GetDlgItemText(IDC_EDIT_REMOTEADDR, ip);
                    UINT cmdtype = CMD_DESIP;
                    if (STATUS_OK != VCI_SetReference(m_devtype, m_devindex, m_canindex, cmdtype, (PVOID)(ip.GetString())))
                    {
                        MessageBox(TEXT("设置远程地址失败"));
                        return;
                    }
                    GetDlgItemText(IDC_EDIT_REMOTEPORT, text);
                    UINT port = str2uint(text);
                    std::vector<BYTE> ports = IntToBytes(port, 4);
                    cmdtype = CMD_DESPORT;
                    if (STATUS_OK != VCI_SetReference(m_devtype, m_devindex, m_canindex, cmdtype, ports.data()))
                    {
                        MessageBox(TEXT("设置远程端口失败"));
                        return;
                    }
                    cmdtype = CMD_TCP_TYPE;
                    UINT tcpType = TCP_CLIENT;
                    std::vector<BYTE> types = IntToBytes(tcpType, 4);
                    if (STATUS_OK != VCI_SetReference(m_devtype, m_devindex, m_canindex, cmdtype, types.data()))
                    {
                        MessageBox(TEXT("设置TCP工作方式失败"));
                        return;
                    }
                }
            }
            else // UDP
            {
                CString ip;
                GetDlgItemText(IDC_EDIT_REMOTEADDR, ip);
                UINT cmdtype = CMD_DESIP;
                if (STATUS_OK != VCI_SetReference(m_devtype, m_devindex, m_canindex, cmdtype, (PVOID)(ip.GetString())))
                {
                    MessageBox(TEXT("设置远程地址失败"));
                    return;
                }
                GetDlgItemText(IDC_EDIT_REMOTEPORT, text);
                UINT port = str2uint(text);
                std::vector<BYTE> ports = IntToBytes(port, 4);
                cmdtype = CMD_DESPORT;
                if (STATUS_OK != VCI_SetReference(m_devtype, m_devindex, m_canindex, cmdtype, ports.data()))
                {
                    MessageBox(TEXT("设置远程端口失败"));
                    return;
                }
                GetDlgItemText(IDC_EDIT_LOCALPORT, text);
                port = str2uint(text);
                ports = IntToBytes(port, 4);
                cmdtype = CMD_SRCPORT;
                if (STATUS_OK != VCI_SetReference(m_devtype, m_devindex, m_canindex, cmdtype, ports.data()))
                {
                    MessageBox(TEXT("设置本地端口失败"));
                    return;
                }
            }
        }
        else // non-netdevice
        {
            GetDlgItemText(IDC_EDIT_ACCCODE, text);
            config.AccCode = str2uint(text, 16);
            GetDlgItemText(IDC_EDIT_MASKCODE, text);
            config.AccMask = str2uint(text, 16);
            GetDlgItemText(IDC_EDIT_TIMER0, text);
            config.Timing0 = str2uint(text, 16);
            GetDlgItemText(IDC_EDIT_TIMER1, text);
            config.Timing1 = str2uint(text, 16);
            config.Filter = m_filter.GetCurSel();
            config.Mode = m_mode.GetCurSel();
        }

        if (m_devtype == VCI_USBCAN_E_U || m_devtype == VCI_USBCAN_2E_U)
        {
            connectUSBCANEU();
        }
        UINT32 ret = VCI_InitCAN(m_devtype, m_devindex, m_canindex, &config);
        if (ret != 1)
        {
            MessageBox(TEXT("Init失败"), TEXT("错误"), MB_OK | MB_ICONEXCLAMATION);
            return;
        }
        m_open = true;
        if (m_devtype == VCI_CANDTU_200UR)
        {
            connectCANDTU();
        }
    }

    SetDlgItemText(IDC_BUTTON_CONNECT, m_open ? TEXT("断开") : TEXT("连接"));
    SetDlgItemText(IDC_BUTTON_STARTCAN, TEXT("启动CAN"));
    GetDlgItem(IDC_BUTTON_STARTCAN)->EnableWindow(m_open);
    if (m_open)
        m_timerId = SetTimer(m_timerId, 100, NULL);
    else
        KillTimer(m_timerId);
}


void CMFCApplication1Dlg::OnBnClickedButtonStartcan()
{
    // TODO:  在此添加控件通知处理程序代码
    if (!m_open) return;
    if (m_started)
    {
        VCI_ResetCAN(m_devtype, m_devindex, m_canindex);
    }
    else
    {
        UINT32 ret = VCI_StartCAN(m_devtype, m_devindex, m_canindex);
        if (1 != ret)
        {
            MessageBox(TEXT("StartCAN失败"), TEXT("错误"), MB_OK | MB_ICONEXCLAMATION);
            return;
        }
    }
    m_started = !m_started;
    SetDlgItemText(IDC_BUTTON_STARTCAN, m_started ? TEXT("复位CAN") : TEXT("启动CAN"));
}


void CMFCApplication1Dlg::OnBnClickedButtonSend()
{
    // TODO:  在此添加控件通知处理程序代码
    if (!m_open) return;
    CString text;
    int num = GetDlgItemInt(IDC_EDIT_FRAMENUM);
    std::vector<VCI_CAN_OBJ> sendobj(num);
    for (std::size_t j = 0; j < sendobj.size(); ++j)
    {
        sendobj[j].SendType = m_sendformat.GetCurSel();
        sendobj[j].RemoteFlag = m_frameformat.GetCurSel();
        sendobj[j].ExternFlag = m_frametype.GetCurSel();
        GetDlgItemText(IDC_EDIT_FRAMEID, text);
        sendobj[j].ID = str2uint(text, 16);
        GetDlgItemText(IDC_EDIT_DATA, text);
        int len = (text.GetLength() + 1) / 3;
        sendobj[j].DataLen = len;
        for (int i = 0; i < len; ++i)
        {
            sendobj[j].Data[i] = str2uint(text.Mid(i * 3, 2), 16);
        }
    }
    UINT res = VCI_Transmit(m_devtype, m_devindex, m_canindex, sendobj.data(), num);
    if (0 == res)
    {
        VCI_ERR_INFO err;
        VCI_ReadErrInfo(m_devtype, m_devindex, m_canindex, &err);
        MessageBox(TEXT("发送失败"), TEXT("错误"), MB_OK | MB_ICONEXCLAMATION);
    }
}


void CMFCApplication1Dlg::OnCbnSelchangeComboType()
{
    // TODO:  在此添加控件通知处理程序代码
    EnableSet();
}

void CMFCApplication1Dlg::EnableSet()
{
    int index = m_type.GetCurSel();
    int type = m_devicetypes.at(index);
    BOOL isNetDevice = (type == VCI_CANETTCP || type == VCI_CANETUDP);
    BOOL isCANDTU200 = type == VCI_CANDTU_200UR;

    GetDlgItem(IDC_EDIT_TIMER0)->EnableWindow(!isNetDevice && !isCANDTU200 && !m_open);
    GetDlgItem(IDC_EDIT_TIMER1)->EnableWindow(!isNetDevice && !isCANDTU200 && !m_open);
    m_baudrate.EnableWindow(!isNetDevice && !m_open);
    GetDlgItem(IDC_EDIT_ACCCODE)->EnableWindow(!isNetDevice && !m_open);
    GetDlgItem(IDC_EDIT_MASKCODE)->EnableWindow(!isNetDevice && !m_open);
    m_filter.EnableWindow(!isNetDevice && !isCANDTU200 && !m_open);
    m_mode.EnableWindow(!isNetDevice && !m_open);

    if (type == VCI_CANETUDP) {
        m_workmode.EnableWindow(FALSE);
        GetDlgItem(IDC_EDIT_LOCALPORT)->EnableWindow(!m_open);
        GetDlgItem(IDC_EDIT_REMOTEPORT)->EnableWindow(!m_open);
        GetDlgItem(IDC_EDIT_REMOTEADDR)->EnableWindow(!m_open);
    }
    else if (type == VCI_CANETTCP) {
        m_workmode.EnableWindow(TRUE);
        BOOL server = m_workmode.GetCurSel() == 0;
        GetDlgItem(IDC_EDIT_LOCALPORT)->EnableWindow(server && !m_open);
        GetDlgItem(IDC_EDIT_REMOTEPORT)->EnableWindow(!server && !m_open);
        GetDlgItem(IDC_EDIT_REMOTEADDR)->EnableWindow(!server && !m_open);
    }
    else {
        m_workmode.EnableWindow(FALSE);
        GetDlgItem(IDC_EDIT_LOCALPORT)->EnableWindow(FALSE);
        GetDlgItem(IDC_EDIT_REMOTEPORT)->EnableWindow(FALSE);
        GetDlgItem(IDC_EDIT_REMOTEADDR)->EnableWindow(FALSE);
    }

    GetDlgItem(IDC_STATIC_TIMER0)->EnableWindow(!isNetDevice);
    GetDlgItem(IDC_STATIC_TIMER1)->EnableWindow(!isNetDevice);
    GetDlgItem(IDC_STATIC_BAUDRATE)->EnableWindow(!isNetDevice);
    GetDlgItem(IDC_STATIC_MODE)->EnableWindow(!isNetDevice);
    GetDlgItem(IDC_STATIC_ACCCODE)->EnableWindow(!isNetDevice);
    GetDlgItem(IDC_STATIC_MASKCODE)->EnableWindow(!isNetDevice);
    GetDlgItem(IDC_STATIC_FILTER)->EnableWindow(!isNetDevice);
    GetDlgItem(IDC_STATIC_WORKMODE)->EnableWindow(isNetDevice);
    GetDlgItem(IDC_STATIC_LOCALPORT)->EnableWindow(isNetDevice);
    GetDlgItem(IDC_STATIC_REMOTEADDR)->EnableWindow(isNetDevice);
    GetDlgItem(IDC_STATIC_REMOTEPORT)->EnableWindow(isNetDevice);
}


void CMFCApplication1Dlg::WriteCfgFile()
{
    TCHAR buff[256];
    GetModuleFileName(NULL, buff, 255);
    CString path(buff);
    path = path.Left(path.ReverseFind(TEXT('\\')) + 1);
    path.Append(cfgFile);

    WritePrivateProfileString(TEXT("GEN"), TEXT("CfgType"), TEXT("2"), path);
    WritePrivateProfileString(TEXT("GEN"), TEXT("NChan"), TEXT("2"), path);
    CString section;
    m_index.GetLBText(m_index.GetCurSel(), section);
    section = TEXT("CAN") + section;
    WritePrivateProfileString(section, TEXT("Mode"), m_mode.GetCurSel() == 0 ? TEXT("128") : TEXT("0"), path);
    WritePrivateProfileString(section, TEXT("UseRes"), TEXT("1"), path);
    WritePrivateProfileString(section, TEXT("FltCNT"), TEXT("1"), path);
    WritePrivateProfileString(section, TEXT("FltFmat"), TEXT("0"), path);
    CString text;
    GetDlgItemText(IDC_EDIT_ACCCODE, text);
    WritePrivateProfileString(section, TEXT("FltM0"), hex2dec(text), path);
    GetDlgItemText(IDC_EDIT_MASKCODE, text);
    WritePrivateProfileString(section, TEXT("FltT0"), hex2dec(text), path);
    CANDTUBPS cfg;
    m_baudrate.GetLBText(m_baudrate.GetCurSel(), text);
    text = text.Left(text.GetLength() - 4);
    getCfg(str2uint(text) * 1000, cfg);
    WritePrivateProfileString(section, TEXT("BpsBRP"), int2str(cfg.brp), path);
    WritePrivateProfileString(section, TEXT("BpsSWJ"), int2str(cfg.sjw), path);
    WritePrivateProfileString(section, TEXT("BpsSeg1"), int2str(cfg.tseg1), path);
    WritePrivateProfileString(section, TEXT("BpsSeg2"), int2str(cfg.tseg2), path);
    WritePrivateProfileString(section, TEXT("BpsSmp"), cfg.trismp ? TEXT("1") : TEXT("0"), path);
}

UINT CMFCApplication1Dlg::connectUSBCANEU()
{
    BYTE buffer[4];
    CString text;
    m_baudrate.GetLBText(m_baudrate.GetCurSel(), text);
    text = text.Left(text.GetLength() - 4);
    UINT baudRate = str2uint(text);
    switch (baudRate)
    {
    case 5:
        buffer[0] = 0xC1;
        buffer[1] = 0x01;
        buffer[2] = 0x1C;
        buffer[3] = 0x00;
        break;
    case 10:
        buffer[0] = 0xE0;
        buffer[1] = 0x00;
        buffer[2] = 0x1C;
        buffer[3] = 0x00;
        break;
    case 20:
        buffer[0] = 0xB3;
        buffer[1] = 0x00;
        buffer[2] = 0x16;
        buffer[3] = 0x00;
        break;
    case 50:
        buffer[0] = 0x2C;
        buffer[1] = 0x00;
        buffer[2] = 0x1C;
        buffer[3] = 0x00;
        break;
    case 100:
        buffer[0] = 0x23;
        buffer[1] = 0x00;
        buffer[2] = 0x16;
        buffer[3] = 0x00;
        break;
    case 125:
        buffer[0] = 0x11;
        buffer[1] = 0x00;
        buffer[2] = 0x1C;
        buffer[3] = 0x00;
        break;
    case 250:
        buffer[0] = 0x08;
        buffer[1] = 0x00;
        buffer[2] = 0x1C;
        buffer[3] = 0x00;
        break;
    case 500:
        buffer[0] = 0x07;
        buffer[1] = 0x00;
        buffer[2] = 0x06;
        buffer[3] = 0x00;
        break;
    case 800:
        buffer[0] = 0x04;
        buffer[1] = 0x00;
        buffer[2] = 0x06;
        buffer[3] = 0x00;
        break;
    default:
        buffer[0] = 0x03;
        buffer[1] = 0x00;
        buffer[2] = 0x06;
        buffer[3] = 0x00;
        break;
    }
    UINT ret = VCI_SetReference(m_devtype, m_devindex, 0, 0, buffer);
    if (ret != 1)
    {
        MessageBox(TEXT("SetReference失败"), TEXT("错误"), MB_OK | MB_ICONEXCLAMATION);
    }
    return ret;
}

UINT CMFCApplication1Dlg::connectCANDTU()
{
    WriteCfgFile();
    SYSTEMTIME time;
    GetLocalTime(&time);
    UINT ret = VCI_SetReference(m_devtype, m_devindex, 0, 1, &time);
    if (ret != 1) {
        MessageBox(TEXT("SetReference失败"), TEXT("错误"), MB_OK | MB_ICONEXCLAMATION);
        return ret;
    }
    DtuComConfig cfg;
    CFile file;
    file.Open(cfgFile, CFile::modeRead);
    std::vector<char> buff(static_cast<std::size_t>(file.GetLength()));
    file.Read(buff.data(), buff.size());
    file.Close();
    ret = VCI_SetReference(m_devtype, m_devindex, m_canindex, 3, buff.data());
    if (ret != 1) {
        MessageBox(TEXT("SetReference失败"), TEXT("错误"), MB_OK | MB_ICONEXCLAMATION);
    }
    return ret;
}

void CMFCApplication1Dlg::OnDestroy()
{
    CDialogEx::OnDestroy();

    // TODO:  在此处添加消息处理程序代码
    if (m_open) VCI_CloseDevice(m_devtype, m_devindex);
}


void CMFCApplication1Dlg::OnCbnSelchangeComboWorkmode()
{
    // TODO:  在此添加控件通知处理程序代码
    EnableSet();
}


void CMFCApplication1Dlg::OnTimer(UINT_PTR nIDEvent)
{
    // TODO:  在此添加消息处理程序代码和/或调用默认值
    if (nIDEvent == m_timerId) {
        UINT32 res = VCI_GetReceiveNum(m_devtype, m_devindex, m_canindex);
        if (0 == res) return;
        const UINT32 con_maxlen = 50;
        std::vector<VCI_CAN_OBJ> pt(50);
        res = VCI_Receive(m_devtype, m_devindex, m_canindex, pt.data(), con_maxlen, 100);
        CString str;
        for (UINT32 i = 0; i < res; ++i)
        {
            VCI_CAN_OBJ& obj = pt.at(i);
            str = TEXT("接收到数据:   帧ID:0x");
            str.Append(int2str(obj.ID, 16));
            str.Append(TEXT("  帧格式:"));
            str.Append(0 == obj.RemoteFlag ? TEXT("数据帧 ") : TEXT("远程帧 "));
            str.Append(0 == obj.ExternFlag ? TEXT("标准帧") : TEXT("扩展帧"));

            if (0 == obj.RemoteFlag)
            {
                str.Append(TEXT("  数据:"));
                BYTE len = obj.DataLen % 9;
                for (BYTE j = 0; j < len; ++j)
                    str.AppendFormat(TEXT(" %02X"), obj.Data[j]);
            }

            m_listInfo.AddString(str);
            m_listInfo.SetCurSel(m_listInfo.GetCount() - 1);
        }
    }


    CDialogEx::OnTimer(nIDEvent);
}


void CMFCApplication1Dlg::OnCbnSelchangeComboBaudrate()
{
    // TODO:  在此添加控件通知处理程序代码

    int i = m_baudrate.GetCurSel();
    switch (i)
    {
    case 0:
    {
              SetDlgItemText(IDC_EDIT_TIMER0, TEXT("bf"));
              SetDlgItemText(IDC_EDIT_TIMER1, TEXT("ff"));
    }
        break;
    case 1:
    {
              SetDlgItemText(IDC_EDIT_TIMER0, TEXT("31"));
              SetDlgItemText(IDC_EDIT_TIMER1, TEXT("1c"));
    }
        break;
    case 2:
    {
              SetDlgItemText(IDC_EDIT_TIMER0, TEXT("18"));
              SetDlgItemText(IDC_EDIT_TIMER1, TEXT("1c"));
    }
        break;
    case 3:
    {
              SetDlgItemText(IDC_EDIT_TIMER0, TEXT("87"));
              SetDlgItemText(IDC_EDIT_TIMER1, TEXT("ff"));
    }
        break;
    case 4:
    {
              SetDlgItemText(IDC_EDIT_TIMER0, TEXT("09"));
              SetDlgItemText(IDC_EDIT_TIMER1, TEXT("1c"));
    }
        break;
    case 5:
    {
              SetDlgItemText(IDC_EDIT_TIMER0, TEXT("83"));
              SetDlgItemText(IDC_EDIT_TIMER1, TEXT("ff"));
    }
        break;
    case 6:
    {
              SetDlgItemText(IDC_EDIT_TIMER0, TEXT("04"));
              SetDlgItemText(IDC_EDIT_TIMER1, TEXT("1c"));
    }
        break;
    case 7:
    {
              SetDlgItemText(IDC_EDIT_TIMER0, TEXT("03"));
              SetDlgItemText(IDC_EDIT_TIMER1, TEXT("1c"));
    }
        break;
    case 8:
    {
              SetDlgItemText(IDC_EDIT_TIMER0, TEXT("81"));
              SetDlgItemText(IDC_EDIT_TIMER1, TEXT("fa"));
    }
        break;
    case 9:
    {
              SetDlgItemText(IDC_EDIT_TIMER0, TEXT("01"));
              SetDlgItemText(IDC_EDIT_TIMER1, TEXT("1c"));
    }
        break;
    case 10:
    {
               SetDlgItemText(IDC_EDIT_TIMER0, TEXT("80"));
               SetDlgItemText(IDC_EDIT_TIMER1, TEXT("fa"));
    }
        break;
    case 11:
    {
               SetDlgItemText(IDC_EDIT_TIMER0, TEXT("00"));
               SetDlgItemText(IDC_EDIT_TIMER1, TEXT("1c"));
    }
        break;
    case 12:
    {
               SetDlgItemText(IDC_EDIT_TIMER0, TEXT("80"));
               SetDlgItemText(IDC_EDIT_TIMER1, TEXT("b6"));
    }
        break;
    case 13:
    {
               SetDlgItemText(IDC_EDIT_TIMER0, TEXT("00"));
               SetDlgItemText(IDC_EDIT_TIMER1, TEXT("16"));
    }
        break;
    case 14:
    {
               SetDlgItemText(IDC_EDIT_TIMER0, TEXT("00"));
               SetDlgItemText(IDC_EDIT_TIMER1, TEXT("14"));
    }
        break;
    }
}
