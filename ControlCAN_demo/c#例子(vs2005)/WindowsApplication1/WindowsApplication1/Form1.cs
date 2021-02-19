using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Text;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using ControlCAN;

namespace WindowsApplication1
{
    public partial class Form1 : Form
    {
        static UInt32 m_devtype = 4;//USBCAN2

        bool started_ = false;
        UInt32 m_bOpen = 0;
        UInt32 m_devind = 0;
        UInt32 m_canind = 0;
        const string startText = "启动CAN";
        const string stopText = "复位CAN";

        VCI_CAN_OBJ[] m_recobj = new VCI_CAN_OBJ[50];

        UInt32[] m_arrdevtype = new UInt32[20];

        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            comboBox1.SelectedIndex = 11;
            comboBox_DevIndex.SelectedIndex = 0;
            comboBox_CANIndex.SelectedIndex = 0;
            textBox_AccCode.Text = "00000000";
            textBox_AccMask.Text = "FFFFFFFF";
            textBox_Time0.Text = "00";
            textBox_Time1.Text = "1C";
            comboBox_Filter.SelectedIndex = 1;
            comboBox_Mode.SelectedIndex = 0;
            comboBox_SendType.SelectedIndex = 2;
            comboBox_FrameFormat.SelectedIndex = 0;
            comboBox_FrameType.SelectedIndex = 0;
            textBox_ID.Text = "00000123";
            textBox_Data.Text = "00 01 02 03 04 05 06 07 ";

            //
            Int32 curindex = 0;
            comboBox_devtype.Items.Clear();

            curindex = comboBox_devtype.Items.Add("VCI_PCI5121");
            m_arrdevtype[curindex] = Device.VCI_PCI5121;
            //comboBox_devtype.Items[0] = "VCI_PCI5121";
            //m_arrdevtype[0]=  VCI_PCI5121 ;

            curindex = comboBox_devtype.Items.Add("VCI_PCI9810");
            m_arrdevtype[curindex] = Device.VCI_PCI9810;
            //comboBox_devtype.Items[1] = "VCI_PCI9810";
            //m_arrdevtype[1]=  VCI_PCI9810 ;

            curindex = comboBox_devtype.Items.Add("VCI_USBCAN1(I+)");
            m_arrdevtype[curindex] = Device.VCI_USBCAN1;
            //comboBox_devtype.Items[2] = "VCI_USBCAN1";
            //m_arrdevtype[2]=  VCI_USBCAN1 ;

            curindex = comboBox_devtype.Items.Add("VCI_USBCAN2(II+)");
            m_arrdevtype[curindex] = Device.VCI_USBCAN2;
            //comboBox_devtype.Items[3] = "VCI_USBCAN2";
            //m_arrdevtype[3]=  VCI_USBCAN2 ;

            curindex = comboBox_devtype.Items.Add("VCI_USBCAN2A");
            m_arrdevtype[curindex] = Device.VCI_USBCAN2A;
            //comboBox_devtype.Items[4] = "VCI_USBCAN2A";
            //m_arrdevtype[4]=  VCI_USBCAN2A ;

            curindex = comboBox_devtype.Items.Add("VCI_PCI9820");
            m_arrdevtype[curindex] = Device.VCI_PCI9820;
            //comboBox_devtype.Items[5] = "VCI_PCI9820";
            //m_arrdevtype[5]=  VCI_PCI9820 ;

            curindex = comboBox_devtype.Items.Add("VCI_PCI5110");
            m_arrdevtype[curindex] = Device.VCI_PCI5110;
            //comboBox_devtype.Items[6] = "VCI_PCI5110";
            //m_arrdevtype[6]=  VCI_PCI5110 ;

            curindex = comboBox_devtype.Items.Add("VCI_CANLITE");
            m_arrdevtype[curindex] = Device.VCI_CANLITE;

            curindex = comboBox_devtype.Items.Add("VCI_ISA9620");
            m_arrdevtype[curindex] = Device.VCI_ISA9620;
            //comboBox_devtype.Items[7] = "VCI_ISA9620";
            //m_arrdevtype[7]=  VCI_ISA9620 ;

            curindex = comboBox_devtype.Items.Add("VCI_ISA5420");
            m_arrdevtype[curindex] = Device.VCI_ISA5420;
            //comboBox_devtype.Items[8] = "VCI_ISA5420";
            //m_arrdevtype[8]=  VCI_ISA5420 ;

            curindex = comboBox_devtype.Items.Add("VCI_PC104CAN");
            m_arrdevtype[curindex] = Device.VCI_PC104CAN;
            //comboBox_devtype.Items[9] = "VCI_PC104CAN";
            //m_arrdevtype[9]=  VCI_PC104CAN ;

            curindex = comboBox_devtype.Items.Add("VCI_DNP9810");
            m_arrdevtype[curindex] = Device.VCI_DNP9810;
            //comboBox_devtype.Items[10] = "VCI_DNP9810";
            //m_arrdevtype[10]=  VCI_DNP9810 ;

            curindex = comboBox_devtype.Items.Add("VCI_PCI9840");
            m_arrdevtype[curindex] = Device.VCI_PCI9840;
            //comboBox_devtype.Items[11] = "VCI_PCI9840";
            //m_arrdevtype[11]=   VCI_PCI9840;

            curindex = comboBox_devtype.Items.Add("VCI_PC104CAN2");
            m_arrdevtype[curindex] = Device.VCI_PC104CAN2;
            //comboBox_devtype.Items[12] = "VCI_PC104CAN2";
            //m_arrdevtype[12]=  VCI_PC104CAN2 ;

            curindex = comboBox_devtype.Items.Add("VCI_PCI9820I");
            m_arrdevtype[curindex] = Device.VCI_PCI9820I;
            //comboBox_devtype.Items[13] = "VCI_PCI9820I";
            //m_arrdevtype[13]=  VCI_PCI9820I ;

            curindex = comboBox_devtype.Items.Add("VCI_PEC9920");
            m_arrdevtype[curindex] = Device.VCI_PEC9920;

            curindex = comboBox_devtype.Items.Add("VCI_PCIE9221");
            m_arrdevtype[curindex] = Device.VCI_PCIE9221;
            //comboBox_devtype.Items[14] = "VCI_PEC9920";
            //m_arrdevtype[14]= VCI_PEC9920  ;

            curindex = comboBox_devtype.Items.Add("VCI_CANDTU200");
            m_arrdevtype[curindex] = Device.VCI_CANDTU200;

            curindex = comboBox_devtype.Items.Add("VCI_CANETTCP");
            m_arrdevtype[curindex] = Device.VCI_CANETTCP;

            curindex = comboBox_devtype.Items.Add("VCI_CANETUDP");
            m_arrdevtype[curindex] = Device.VCI_CANETUDP;

            comboBox_devtype.SelectedIndex = 3;
            comboBox_devtype.MaxDropDownItems = comboBox_devtype.Items.Count;

            netMode.Items.Add("服务器");
            netMode.Items.Add("客户端");
            netMode.SelectedIndex = 0;

            localPort.Text = "1008";
            remoteAddr.Text = "192.168.28.222";
            remotePort.Text = "4001";

            started_ = false;
            button_StartCAN.Enabled = false;
            button_StartCAN.Text = startText;

            EnableSet();
        }

        private void Form1_FormClosed(object sender, FormClosedEventArgs e)
        {
            if (m_bOpen == 1)
            {
                Device.VCI_CloseDevice(m_devtype, m_devind);
            }
        }

        private void EnableSet()
        {
            bool opened = m_bOpen == 1;
            int index = comboBox_devtype.SelectedIndex;
            uint type = m_arrdevtype[index];
            bool isNetDevice = (type == Device.VCI_CANETTCP || type == Device.VCI_CANETUDP);
            bool isCANDTU200 = type == Device.VCI_CANDTU200;

            textBox_Time0.Enabled = !isNetDevice && !isCANDTU200 && !opened;
            comboBox1.Enabled = !isNetDevice && !opened;
            textBox_AccCode.Enabled = !isNetDevice && !opened;
            comboBox_Filter.Enabled = !isNetDevice && !isCANDTU200 && !opened;
            textBox_Time1.Enabled = !isNetDevice && !isCANDTU200 && !opened;
            comboBox_Mode.Enabled = !isNetDevice && !opened;
            textBox_AccMask.Enabled = !isNetDevice && !opened;

            if (type == Device.VCI_CANETUDP)
            {
                netMode.Enabled = false;
                localPort.Enabled = !opened;
                remoteAddr.Enabled = !opened;
                remotePort.Enabled = !opened;
            }
            else if (type == Device.VCI_CANETTCP)
            {
                netMode.Enabled = true;
                bool server = netMode.SelectedIndex == 0;
                localPort.Enabled = server && !opened;
                remoteAddr.Enabled = !server && !opened;
                remotePort.Enabled = !server && !opened;
            }
            else
            {
                netMode.Enabled = false;
                localPort.Enabled = false;
                remoteAddr.Enabled = false;
                remotePort.Enabled = false;
            }

            label4.Enabled = !isNetDevice;
            label3.Enabled = !isNetDevice;
            label7.Enabled = !isNetDevice;
            label16.Enabled = !isNetDevice;
            label5.Enabled = !isNetDevice;
            label6.Enabled = !isNetDevice;
            label8.Enabled = !isNetDevice;
            label17.Enabled = isNetDevice;
            label18.Enabled = isNetDevice;
            label19.Enabled = isNetDevice;
            label20.Enabled = isNetDevice;
        }

        [StructLayout(LayoutKind.Sequential)]
        private struct SystemTime
        {
            public ushort year;
            public ushort month;
            public ushort dayOfWeek;
            public ushort day;
            public ushort hour;
            public ushort minute;
            public ushort second;
            public ushort milliseconds;
        };

        [DllImport("Kernel32.dll")]
        private static extern void GetLocalTime(ref SystemTime st);

        private static byte[] StructToBytes(object obj, int size)
        {
            IntPtr buffer = Marshal.AllocHGlobal(size);
            try
            {
                Marshal.StructureToPtr(obj, buffer, false);
                byte[] bytes = new byte[size];
                Marshal.Copy(buffer, bytes, 0, size);
                return bytes;
            }
            finally
            {
                Marshal.FreeHGlobal(buffer);
            }
        }

        [StructLayout(LayoutKind.Sequential)]
        private struct DtuComConfig
        {
            public ushort len;
            //public byte[] data;
            public string data;
        }

        [DllImport("Kernel32")]
        private static extern long WritePrivateProfileString(
            string section, string key, string val, string filepath);

        private struct CANDTUBPS
        {
            public byte tseg1;
            public byte tseg2;
            public ushort brp;
            public uint sjw;
            public bool trismp;
        }
        const string cfgFile = "candtu.ini";

        const uint clock = 24000000;
        private class IM28X_TSEG2_TSEG1_SJW
        {
            public byte tSeg2;
            public byte tSeg1Min;
            public byte tSeg1Max;
            public byte rSjwMin;
            public byte rSjwMax;
            public IM28X_TSEG2_TSEG1_SJW(byte seg2, byte seg1min, byte seg1max, byte sjwmin, byte sjwmax)
            {
                tSeg2 = seg2;
                tSeg1Min = seg1min;
                tSeg1Max = seg1max;
                rSjwMin = sjwmin;
                rSjwMax = sjwmax;
            }
        };

        IM28X_TSEG2_TSEG1_SJW[] im28xTseg2Tseg1SjwTable =
        {
            new IM28X_TSEG2_TSEG1_SJW(2, 5, 10, 1, 2),
            new IM28X_TSEG2_TSEG1_SJW(3, 4, 11, 1, 3),
            new IM28X_TSEG2_TSEG1_SJW(4, 5, 12, 1, 4),
            new IM28X_TSEG2_TSEG1_SJW(5, 6, 13, 1, 4),
            new IM28X_TSEG2_TSEG1_SJW(6, 7, 14, 1, 4),
            new IM28X_TSEG2_TSEG1_SJW(7, 8, 15, 1, 4),
            new IM28X_TSEG2_TSEG1_SJW(8, 9, 16, 1, 4)
        };

        bool IsTseg2Tseg1SjwMatchIM28X(byte tSeg2, byte tSeg1, byte rSjw)
        {
            for (int i = 0; i < im28xTseg2Tseg1SjwTable.Length; ++i)
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

        private bool getCfg(uint desiredBaud, out CANDTUBPS cfg)
        {
            byte SJW = 0;
            for (byte TSEG2 = (byte)(SJW + 1); TSEG2 <= 8; TSEG2++)
            {
                for (byte TSEG1 = (byte)(TSEG2 + 1); TSEG1 <= 16; TSEG1++)
                {
                    for (ushort BRP = 1; BRP <= 256; ++BRP)
                    {
                        uint baud = clock / ((uint)(1 + TSEG1 + TSEG2) * BRP);
                        if ((baud > desiredBaud * 995 / 1000) && (baud < desiredBaud * 1005 / 1000))
                        {
                            if (IsTseg2Tseg1SjwMatchIM28X(TSEG2, TSEG1, (byte)(SJW + 1)))
                            {
                                cfg.tseg1 = (byte)(TSEG1 - 2);
                                cfg.tseg2 = (byte)(TSEG2 - 1);
                                cfg.sjw = SJW;
                                cfg.brp = (byte)(BRP - 1);
                                cfg.trismp = false;
                                return true;
                            }
                        }
                    }
                }
            }
            cfg.tseg1 = 0;
            cfg.tseg2 = 0;
            cfg.sjw = 0;
            cfg.brp = 0;
            cfg.trismp = false;
            return false;
        }

        private void writeCfgFile()
        {
            string path = Application.StartupPath + "\\" + cfgFile;
            long ret = WritePrivateProfileString("GEN", "CfgType", "2", path);
            WritePrivateProfileString("GEN", "NChan", "2", path);
            string section = "CAN" + comboBox_CANIndex.Text;
            WritePrivateProfileString(section, "Mode",
                comboBox_Mode.SelectedIndex == 0 ? "128" : "0", path);
            WritePrivateProfileString(section, "UseRes", "1", path);
            WritePrivateProfileString(section, "FltCNT", "1", path);
            WritePrivateProfileString(section, "FltFmat", "0", path);
            WritePrivateProfileString(section, "FltM0",
                Convert.ToString(Convert.ToInt32(textBox_AccMask.Text, 16)), path);
            WritePrivateProfileString(section, "FltT0",
                Convert.ToString(Convert.ToInt32(textBox_AccCode.Text, 16)), path);
            CANDTUBPS cfg;
            string baud = comboBox1.Text.Remove(comboBox1.Text.Length - 4);
            getCfg((uint)Convert.ToInt32(baud) * 1000, out cfg);
            WritePrivateProfileString(section, "BpsBRP", Convert.ToString(cfg.brp), path);
            WritePrivateProfileString(section, "BpsSWJ", Convert.ToString(cfg.sjw), path);
            WritePrivateProfileString(section, "BpsSeg1", Convert.ToString(cfg.tseg1), path);
            WritePrivateProfileString(section, "BpsSeg2", Convert.ToString(cfg.tseg2), path);
            WritePrivateProfileString(section, "BpsSmp", cfg.trismp ? "1" : "0", path);
        }

        private uint connectCANDTU()
        {
            writeCfgFile();
            SystemTime st = new SystemTime();
            GetLocalTime(ref st);
            byte[] buffer = StructToBytes(st, Marshal.SizeOf(st));
            uint ret = Device.VCI_SetReference(m_devtype, m_devind, 0, 1, ref buffer[0]);
            if (ret != 1)
            {
                MessageBox.Show("SetReference失败", "错误",
                        MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                return ret;
            }
            DtuComConfig cfg = new DtuComConfig();
            FileStream file = new FileStream(cfgFile, FileMode.Open);
            byte[] data = new byte[file.Length];
            cfg.len = (ushort)file.Length;
            file.Read(data, 0, (int)file.Length);
            cfg.data = Encoding.Default.GetString(data);
            buffer = StructToBytes(cfg, Marshal.SizeOf(cfg));
            file.Close();
            ret = Device.VCI_SetReference(m_devtype, m_devind, m_canind, 3, ref buffer[0]);
            if (ret != 1)
            {
                MessageBox.Show("SetReference失败", "错误",
                        MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                return ret;
            }
            return 1;
        }

        private static byte[] IntToBytes(UInt64 num, int bits)
        {
            byte[] b = new byte[bits];
            for (int i = 0; i < bits; i++)
            {
                //b[i] = (byte)(num >> ((bits - 1) * 8 - i * 8));//高字节在索引低处
                b[i] = (byte)((num >> (i * 8)) & 0xff);

            }
            return b;
        }

        private void buttonConnect_Click(object sender, EventArgs e)
        {
            int index = comboBox_devtype.SelectedIndex;
            uint type = m_arrdevtype[index];
            bool isNetDevice = (type == Device.VCI_CANETTCP || type == Device.VCI_CANETUDP);
            if (m_bOpen == 1)
            {
                Device.VCI_CloseDevice(m_devtype, m_devind);
                m_bOpen = 0;
            }
            else
            {
                m_devtype = m_arrdevtype[comboBox_devtype.SelectedIndex];

                m_devind = (UInt32)comboBox_DevIndex.SelectedIndex;
                m_canind = (UInt32)comboBox_CANIndex.SelectedIndex;
                if (Device.VCI_OpenDevice(m_devtype, m_devind, Convert.ToUInt32(localPort.Text)) == 0)
                {
                    MessageBox.Show("打开设备失败,请检查设备类型和设备索引号是否正确", "错误",
                            MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                    return;
                }

                VCI_INIT_CONFIG config = new VCI_INIT_CONFIG();
                if (isNetDevice)
                {
                    if (type == Device.VCI_CANETTCP) // TCP
                    {
                        if (netMode.SelectedIndex == 0) // Server
                        {
                            uint port = Convert.ToUInt32(localPort.Text);
                            byte[] ports = IntToBytes(port, 4);
                            uint cmdtype = Convert.ToUInt32(CMD.CMD_SRCPORT);
                            if (Convert.ToUInt32(STATUS.STATUS_OK) != Device.VCI_SetReference(
                                m_devtype, m_devind, m_canind, cmdtype, ref ports[0]))
                            {
                                MessageBox.Show("设置本地端口失败");
                                return;
                            }
                            cmdtype = Convert.ToUInt32(CMD.CMD_TCP_TYPE);
                            uint tcpType = Convert.ToUInt32(TCPTYPE.TCP_SERVER);
                            byte[] types = IntToBytes(tcpType, 4);
                            if (Convert.ToUInt32(STATUS.STATUS_OK) != Device.VCI_SetReference(
                                m_devtype, m_devind, m_canind, cmdtype, ref types[0]))
                            {
                                MessageBox.Show("设置TCP工作方式失败");
                                return;
                            }
                        }
                        else // Client
                        {
                            string ip = remoteAddr.Text;
                            char[] chars = ip.ToCharArray();
                            byte[] bs = new byte[50];
                            for (int i = 0; i < chars.Length; ++i)
                            {
                                bs[i] = Convert.ToByte(chars[i]);
                            }
                            uint cmdtype = Convert.ToUInt32(CMD.CMD_DESIP);
                            if (Convert.ToUInt32(STATUS.STATUS_OK) != Device.VCI_SetReference(
                                m_devtype, m_devind, m_canind, cmdtype, ref bs[0]))
                            {
                                MessageBox.Show("设置远程地址失败");
                                return;
                            }
                            uint port = Convert.ToUInt32(remotePort.Text);
                            byte[] ports = IntToBytes(port, 4);
                            cmdtype = Convert.ToUInt32(CMD.CMD_DESPORT);
                            if (Convert.ToUInt32(STATUS.STATUS_OK) != Device.VCI_SetReference(
                                m_devtype, m_devind, m_canind, cmdtype, ref ports[0]))
                            {
                                MessageBox.Show("设置远程端口失败");
                                return;
                            }
                            cmdtype = Convert.ToUInt32(CMD.CMD_TCP_TYPE);
                            uint tcpType = Convert.ToUInt32(TCPTYPE.TCP_CLIENT);
                            byte[] types = IntToBytes(tcpType, 4);
                            if (Convert.ToUInt32(STATUS.STATUS_OK) != Device.VCI_SetReference(
                                m_devtype, m_devind, m_canind, cmdtype, ref types[0]))
                            {
                                MessageBox.Show("设置TCP工作方式失败");
                                return;
                            }
                        }
                    }
                    else // UDP
                    {
                        string ip = remoteAddr.Text;
                        char[] chars = ip.ToCharArray();
                        byte[] bs = new byte[50];
                        for (int i = 0; i < chars.Length; ++i)
                        {
                            bs[i] = Convert.ToByte(chars[i]);
                        }
                        uint cmdtype = Convert.ToUInt32(CMD.CMD_DESIP);
                        if (Convert.ToUInt32(STATUS.STATUS_OK) != Device.VCI_SetReference(
                            m_devtype, m_devind, m_canind, cmdtype, ref bs[0]))
                        {
                            MessageBox.Show("设置远程地址失败");
                            return;
                        }
                        uint port = Convert.ToUInt32(remotePort.Text);
                        byte[] ports = IntToBytes(port, 4);
                        cmdtype = Convert.ToUInt32(CMD.CMD_DESPORT);
                        if (Convert.ToUInt32(STATUS.STATUS_OK) != Device.VCI_SetReference(
                            m_devtype, m_devind, m_canind, cmdtype, ref ports[0]))
                        {
                            MessageBox.Show("设置远程端口失败");
                            return;
                        }
                        port = Convert.ToUInt32(localPort.Text);
                        ports = IntToBytes(port, 4);
                        cmdtype = Convert.ToUInt32(CMD.CMD_SRCPORT);
                        if (Convert.ToUInt32(STATUS.STATUS_OK) != Device.VCI_SetReference(
                            m_devtype, m_devind, m_canind, cmdtype, ref ports[0]))
                        {
                            MessageBox.Show("设置本地端口失败");
                            return;
                        }
                    }
                }
                else
                {
                    config.AccCode = System.Convert.ToUInt32("0x" + textBox_AccCode.Text, 16);
                    config.AccMask = System.Convert.ToUInt32("0x" + textBox_AccMask.Text, 16);
                    config.Timing0 = System.Convert.ToByte("0x" + textBox_Time0.Text, 16);
                    config.Timing1 = System.Convert.ToByte("0x" + textBox_Time1.Text, 16);
                    config.Filter = (Byte)comboBox_Filter.SelectedIndex;
                    config.Mode = (Byte)comboBox_Mode.SelectedIndex;
                }
                UInt32 ret = Device.VCI_InitCAN(m_devtype, m_devind, m_canind, ref config);
                if (ret != 1)
                {
                    MessageBox.Show("InitCAN失败", "错误",
                            MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                    return;
                }
                m_bOpen = 1;
                if (m_devtype == Device.VCI_CANDTU200)
                {
                    connectCANDTU();
                }
            }
            bool opened = m_bOpen == 1;
            buttonConnect.Text = opened ? "断开" : "连接";
            button_StartCAN.Text = startText;
            button_StartCAN.Enabled = opened;
            timer_rec.Enabled = opened;

            EnableSet();
        }

        private void timer_rec_Tick(object sender, EventArgs e)
        {
            UInt32 res = new UInt32();
            res = Device.VCI_GetReceiveNum(m_devtype, m_devind, m_canind);
            if (res == 0)
                return;
            //res = VCI_Receive(m_devtype, m_devind, m_canind, ref m_recobj[0],50, 100);

            /////////////////////////////////////
            UInt32 con_maxlen = 50;
            IntPtr pt = Marshal.AllocHGlobal(Marshal.SizeOf(typeof(VCI_CAN_OBJ)) * (Int32)con_maxlen);




            res = Device.VCI_Receive(m_devtype, m_devind, m_canind, pt, con_maxlen, 100);
            ////////////////////////////////////////////////////////

            String str = "";
            for (UInt32 i = 0; i < res; i++)
            {
                VCI_CAN_OBJ obj = (VCI_CAN_OBJ)Marshal.PtrToStructure((IntPtr)((UInt32)pt + i * Marshal.SizeOf(typeof(VCI_CAN_OBJ))), typeof(VCI_CAN_OBJ));

                str = "接收到数据: ";
                str += "  帧ID:0x" + System.Convert.ToString((Int32)obj.ID, 16);
                str += "  帧格式:";
                if (obj.RemoteFlag == 0)
                    str += "数据帧 ";
                else
                    str += "远程帧 ";
                if (obj.ExternFlag == 0)
                    str += "标准帧 ";
                else
                    str += "扩展帧 ";

                //////////////////////////////////////////
                if (obj.RemoteFlag == 0)
                {
                    str += "数据: ";
                    byte len = (byte)(obj.DataLen % 9);
                    byte j = 0;
                    if (j++ < len)
                        str += " " + System.Convert.ToString(obj.Data[0], 16);
                    if (j++ < len)
                        str += " " + System.Convert.ToString(obj.Data[1], 16);
                    if (j++ < len)
                        str += " " + System.Convert.ToString(obj.Data[2], 16);
                    if (j++ < len)
                        str += " " + System.Convert.ToString(obj.Data[3], 16);
                    if (j++ < len)
                        str += " " + System.Convert.ToString(obj.Data[4], 16);
                    if (j++ < len)
                        str += " " + System.Convert.ToString(obj.Data[5], 16);
                    if (j++ < len)
                        str += " " + System.Convert.ToString(obj.Data[6], 16);
                    if (j++ < len)
                        str += " " + System.Convert.ToString(obj.Data[7], 16);

                }

                listBox_Info.Items.Add(str);
                listBox_Info.SelectedIndex = listBox_Info.Items.Count - 1;
            }
            Marshal.FreeHGlobal(pt);
        }

        private void button_StartCAN_Click(object sender, EventArgs e)
        {
            if (m_bOpen == 0)
                return;
            if (started_)
            {
                Device.VCI_ResetCAN(m_devtype, m_devind, m_canind);
            }
            else
            {
                UInt32 ret = Device.VCI_StartCAN(m_devtype, m_devind, m_canind);
                if (ret != 1)
                {
                    MessageBox.Show("StartCAN失败", "错误",
                            MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                }
            }
            started_ = !started_;
            button_StartCAN.Text = started_ ? stopText : startText;
        }

        private void button_StopCAN_Click(object sender, EventArgs e)
        {
            if (m_bOpen == 0)
                return;
            Device.VCI_ResetCAN(m_devtype, m_devind, m_canind);
        }

        private void button_Send_Click(object sender, EventArgs e)
        {
            if (m_bOpen == 0)
                return;
            int num = int.Parse(textBox1.Text);
            VCI_CAN_OBJ[] sendobj = new VCI_CAN_OBJ[num];//sendobj.Init();
            for (int j = 0; j < sendobj.Length; j++)
            {
                sendobj[j].SendType = (byte)comboBox_SendType.SelectedIndex;
                sendobj[j].RemoteFlag = (byte)comboBox_FrameFormat.SelectedIndex;
                sendobj[j].ExternFlag = (byte)comboBox_FrameType.SelectedIndex;
                sendobj[j].ID = System.Convert.ToUInt32("0x" + textBox_ID.Text, 16);
                int len = (textBox_Data.Text.Length + 1) / 3;
                sendobj[j].DataLen = System.Convert.ToByte(len);
                String strdata = textBox_Data.Text;
                int i = -1;
                sendobj[j].Data = new byte[8];
                if (i++ < len - 1)
                    sendobj[j].Data[0] = System.Convert.ToByte("0x" + strdata.Substring(i * 3, 2), 16);

                if (i++ < len - 1)
                    sendobj[j].Data[1] = System.Convert.ToByte("0x" + strdata.Substring(i * 3, 2), 16);

                if (i++ < len - 1)
                    sendobj[j].Data[2] = System.Convert.ToByte("0x" + strdata.Substring(i * 3, 2), 16);

                if (i++ < len - 1)
                    sendobj[j].Data[3] = System.Convert.ToByte("0x" + strdata.Substring(i * 3, 2), 16);

                if (i++ < len - 1)
                    sendobj[j].Data[4] = System.Convert.ToByte("0x" + strdata.Substring(i * 3, 2), 16);

                if (i++ < len - 1)
                    sendobj[j].Data[5] = System.Convert.ToByte("0x" + strdata.Substring(i * 3, 2), 16);

                if (i++ < len - 1)
                    sendobj[j].Data[6] = System.Convert.ToByte("0x" + strdata.Substring(i * 3, 2), 16);

                if (i++ < len - 1)
                    sendobj[j].Data[7] = System.Convert.ToByte("0x" + strdata.Substring(i * 3, 2), 16);
            }

            uint res = Device.VCI_Transmit(m_devtype, m_devind, m_canind, sendobj, (uint)num);
            if (res == 0)
            {
                MessageBox.Show("发送失败", "错误", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
            }
        }

        private void textBox_AccCode_TextChanged(object sender, EventArgs e)
        {

        }

        private void comboBoxDevType_SelectedIndexChanged(object sender, EventArgs e)
        {
            EnableSet();
        }

        private void comboBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            int i = comboBox1.SelectedIndex;
            switch (i)
            {
                case 0:
                    {
                        textBox_Time0.Text = "bf";
                        textBox_Time1.Text = "ff";
                    }
                    break;
                case 1:
                    {
                        textBox_Time0.Text = "31";
                        textBox_Time1.Text = "1c";
                    }
                    break;
                case 2:
                    {
                        textBox_Time0.Text = "18";
                        textBox_Time1.Text = "1c";
                    }
                    break;
                case 3:
                    {
                        textBox_Time0.Text = "87";
                        textBox_Time1.Text = "ff";
                    }
                    break;
                case 4:
                    {
                        textBox_Time0.Text = "09";
                        textBox_Time1.Text = "1c";
                    }
                    break;
                case 5:
                    {
                        textBox_Time0.Text = "83";
                        textBox_Time1.Text = "ff";
                    }
                    break;
                case 6:
                    {
                        textBox_Time0.Text = "04";
                        textBox_Time1.Text = "1c";
                    }
                    break;
                case 7:
                    {
                        textBox_Time0.Text = "03";
                        textBox_Time1.Text = "1c";
                    }
                    break;
                case 8:
                    {
                        textBox_Time0.Text = "81";
                        textBox_Time1.Text = "fa";
                    }
                    break;
                case 9:
                    {
                        textBox_Time0.Text = "01";
                        textBox_Time1.Text = "1c";
                    }
                    break;
                case 10:
                    {
                        textBox_Time0.Text = "80";
                        textBox_Time1.Text = "fa";
                    }
                    break;
                case 11:
                    {
                        textBox_Time0.Text = "00";
                        textBox_Time1.Text = "1c";
                    }
                    break;
                case 12:
                    {
                        textBox_Time0.Text = "80";
                        textBox_Time1.Text = "b6";
                    }
                    break;
                case 13:
                    {
                        textBox_Time0.Text = "00";
                        textBox_Time1.Text = "16";
                    }
                    break;
                case 14:
                    {
                        textBox_Time0.Text = "00";
                        textBox_Time1.Text = "14";
                    }
                    break;
            }
        }

        private void netMode_SelectedIndexChanged(object sender, EventArgs e)
        {
            EnableSet();
        }
    }
}