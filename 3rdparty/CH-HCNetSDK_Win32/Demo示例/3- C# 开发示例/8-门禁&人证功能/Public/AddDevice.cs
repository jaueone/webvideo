using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading;
using System.Windows.Forms;
using System.Runtime.InteropServices;

using AcsDemo.Language;

namespace AcsDemo.Public
{
    public partial class AddDevice : Form
    {
        private DeviceTree g_deviceTree = DeviceTree.Instance();
        private DeviceLogList g_formList = DeviceLogList.Instance();
        public int m_iDeviceIndex = -1;
        CHCNetSDK.NET_DVR_DEVICEINFO_V30 m_struDeviceInfo;
        private int m_iUserID = -1;
        private uint m_AysnLoginResult = 0;
        private bool LoginCallBackFlag = false;
        private bool AysnLoginFlag = false;

        public AddDevice()
        {
            InitializeComponent();
        }

        private void buttonAdd_Click(object sender, EventArgs e)
        {
            if (textBoxDeviceAddress.Text.Length <= 0 || textBoxDeviceAddress.Text.Length > 128)
            {
                MessageBox.Show(Properties.Resources.deviceAddressTips);
                return;
            }

            int port;
            int.TryParse(textBoxPort.Text, out port);

            if (textBoxPort.Text.Length > 5 || port <= 0)
            {
                MessageBox.Show(Properties.Resources.portTips);
                return;
            }
            if (textBoxUserName.Text.Length > 32 || textBoxPassword.Text.Length > 16)
            {
                MessageBox.Show(Properties.Resources.usernameAndPasswordTips);
                return;
            }
            if (textBoxLocalNode.Text.Length == 0)
            {
                textBoxLocalNode.Text = textBoxDeviceAddress.Text;
            }

            if (!Login(true))
            {
                //textBoxLocalNode.Text = null;
                return;
            }
            this.DialogResult = DialogResult.OK;

        }

        // Asynchronous callback function
        public void AsynLoginMsgCallback(Int32 lUserID, UInt32 dwResult, ref CHCNetSDK.NET_DVR_DEVICEINFO_V30 lpDeviceInfo, IntPtr pUser)
        {

            if (dwResult == 1)
            {

                m_struDeviceInfo = lpDeviceInfo;

            }

            m_AysnLoginResult = dwResult;
            m_iUserID = lUserID;
            LoginCallBackFlag = true;
        }

        public bool Login(bool bStatus)//true said add node login, false for the existing node to log in 
        {
            LoginCallBackFlag = false;
            m_struDeviceInfo = new CHCNetSDK.NET_DVR_DEVICEINFO_V30();

            CHCNetSDK.NET_DVR_DEVICEINFO_V30 struDeviceInfo = new CHCNetSDK.NET_DVR_DEVICEINFO_V30();
            struDeviceInfo.sSerialNumber = new byte[CHCNetSDK.SERIALNO_LEN];

            CHCNetSDK.NET_DVR_NETCFG_V50 struNetCfg = new CHCNetSDK.NET_DVR_NETCFG_V50();
            struNetCfg.Init();
            CHCNetSDK.NET_DVR_DEVICECFG_V40 struDevCfg = new CHCNetSDK.NET_DVR_DEVICECFG_V40();
            struDevCfg.sDVRName = new byte[CHCNetSDK.NAME_LEN];
            struDevCfg.sSerialNumber = new byte[CHCNetSDK.SERIALNO_LEN];
            struDevCfg.byDevTypeName = new byte[CHCNetSDK.DEV_TYPE_NAME_LEN];
            CHCNetSDK.NET_DVR_USER_LOGIN_INFO struLoginInfo = new CHCNetSDK.NET_DVR_USER_LOGIN_INFO();
            CHCNetSDK.NET_DVR_DEVICEINFO_V40 struDeviceInfoV40 = new CHCNetSDK.NET_DVR_DEVICEINFO_V40();
            struDeviceInfoV40.struDeviceV30.sSerialNumber = new byte[CHCNetSDK.SERIALNO_LEN];
            uint dwReturned = 0;
            int lUserID = -1;

            struLoginInfo.bUseAsynLogin = AysnLoginFlag;
            struLoginInfo.cbLoginResult = new CHCNetSDK.LoginResultCallBack(AsynLoginMsgCallback);
            struLoginInfo.byLoginMode = 2;

            if (bStatus)
            {
                struLoginInfo.sDeviceAddress = textBoxDeviceAddress.Text;
                struLoginInfo.sUserName = textBoxUserName.Text;
                struLoginInfo.sPassword = textBoxPassword.Text;
                ushort.TryParse(textBoxPort.Text, out struLoginInfo.wPort);
            }
            else
            {
                struLoginInfo.sDeviceAddress = g_deviceTree.g_struDeviceInfo[m_iDeviceIndex].chDeviceIP;
                struLoginInfo.sUserName = g_deviceTree.g_struDeviceInfo[m_iDeviceIndex].chLoginUserName;
                struLoginInfo.sPassword = g_deviceTree.g_struDeviceInfo[m_iDeviceIndex].chLoginPwd;
                struLoginInfo.wPort = (ushort)g_deviceTree.g_struDeviceInfo[m_iDeviceIndex].lDevicePort;
            }

            lUserID = CHCNetSDK.NET_DVR_Login_V40(ref struLoginInfo, ref struDeviceInfoV40);
            if (struLoginInfo.bUseAsynLogin)
            {
                for (int i = 0; i < 1000; i++)
                {
                    if (!LoginCallBackFlag)
                    {
                        Thread.Sleep(5);
                    }
                    else
                    {
                        break;
                    }
                }
                if (!LoginCallBackFlag)
                {
                    MessageBox.Show(Properties.Resources.asynloginTips, Properties.Resources.messageBoxTitle, MessageBoxButtons.OK, MessageBoxIcon.Information);
                }
                if (m_AysnLoginResult == 1)
                {
                    lUserID = m_iUserID;
                    struDeviceInfoV40.struDeviceV30 = m_struDeviceInfo;
                }
                else
                {
                    MessageBox.Show(Properties.Resources.asynloginFailedTips, Properties.Resources.messageBoxTitle, MessageBoxButtons.OK, MessageBoxIcon.Information);
                    return false;
                }

            }

            if (lUserID < 0)
            {
                uint nErr = CHCNetSDK.NET_DVR_GetLastError();
                string strTemp = string.Format("NET_DVR_Login_V40 [{0}]", textBoxDeviceAddress.Text);
                g_formList.AddLog(-1, AcsDemoPublic.OPERATION_FAIL_T, strTemp);
                if (nErr == CHCNetSDK.NET_DVR_PASSWORD_ERROR)
                {
                    MessageBox.Show("user name or password error!");
                    if (1 == struDeviceInfoV40.bySupportLock)
                    {
                        string strTemp1 = string.Format("Left {0} try opportunity", struDeviceInfoV40.byRetryLoginTime);
                        MessageBox.Show(strTemp1);
                    }
                }
                else if (nErr == CHCNetSDK.NET_DVR_USER_LOCKED)
                {
                    if (1 == struDeviceInfoV40.bySupportLock)
                    {
                        string strTemp1 = string.Format("user is locked, the remaining lock time is {0}", struDeviceInfoV40.dwSurplusLockTime);
                        MessageBox.Show(strTemp1);
                    }
                }
                else
                {
                    MessageBox.Show("net error or dvr is busy!");
                }
                return false;
            }
            else
            {
                if (1 == struDeviceInfoV40.byPasswordLevel)
                {
                    MessageBox.Show("default password, please change the password");
                }
                else if (3 == struDeviceInfoV40.byPasswordLevel)
                {
                    MessageBox.Show("risk password, please change the password");
                }
                struDeviceInfo = struDeviceInfoV40.struDeviceV30;
            }

            if (bStatus)
            {
                g_deviceTree.g_struDeviceInfo[m_iDeviceIndex].chLocalNodeName = textBoxLocalNode.Text;
                g_deviceTree.g_struDeviceInfo[m_iDeviceIndex].chLoginPwd = textBoxPassword.Text;
                g_deviceTree.g_struDeviceInfo[m_iDeviceIndex].chDeviceIP = textBoxDeviceAddress.Text;
                g_deviceTree.g_struDeviceInfo[m_iDeviceIndex].chLoginUserName = textBoxUserName.Text;
                int.TryParse(textBoxPort.Text, out g_deviceTree.g_struDeviceInfo[m_iDeviceIndex].lDevicePort);
            }

            g_formList.AddLog(m_iDeviceIndex, AcsDemoPublic.OPERATION_SUCC_T, "NET_DVR_Login_V40");

            g_deviceTree.g_struDeviceInfo[m_iDeviceIndex].byCharaterEncodeType = struDeviceInfoV40.byCharEncodeType;
            g_deviceTree.g_struDeviceInfo[m_iDeviceIndex].lLoginID = lUserID;
            g_deviceTree.g_struDeviceInfo[m_iDeviceIndex].chSerialNumber = System.Text.Encoding.UTF8.GetString(struDeviceInfo.sSerialNumber).TrimEnd('\0');
            g_deviceTree.g_struDeviceInfo[m_iDeviceIndex].iDeviceIndex = m_iDeviceIndex;
            g_deviceTree.g_struDeviceInfo[m_iDeviceIndex].iDeviceType = (int)struDeviceInfo.wDevType;
            g_deviceTree.g_struDeviceInfo[m_iDeviceIndex].iDeviceChanNum = (int)(struDeviceInfo.byChanNum + struDeviceInfo.byIPChanNum + struDeviceInfo.byHighDChanNum * 256);
            g_deviceTree.g_struDeviceInfo[m_iDeviceIndex].iStartChan = (int)struDeviceInfo.byStartChan;
            g_deviceTree.g_struDeviceInfo[m_iDeviceIndex].iDiskNum = (int)struDeviceInfo.byDiskNum;
            g_deviceTree.g_struDeviceInfo[m_iDeviceIndex].iAlarmInNum = (int)struDeviceInfo.byAlarmInPortNum;
            g_deviceTree.g_struDeviceInfo[m_iDeviceIndex].iAlarmOutNum = (int)struDeviceInfo.byAlarmOutPortNum;
            g_deviceTree.g_struDeviceInfo[m_iDeviceIndex].iAudioNum = (int)struDeviceInfo.byAlarmOutPortNum;
            g_deviceTree.g_struDeviceInfo[m_iDeviceIndex].iAnalogChanNum = (int)struDeviceInfo.byChanNum;
            g_deviceTree.g_struDeviceInfo[m_iDeviceIndex].iIPChanNum = (int)(struDeviceInfo.byIPChanNum + struDeviceInfo.byHighDChanNum * 256);
            g_deviceTree.g_struDeviceInfo[m_iDeviceIndex].byZeroChanNum = struDeviceInfo.byZeroChanNum;
            g_deviceTree.g_struDeviceInfo[m_iDeviceIndex].byStartDTalkChan = struDeviceInfo.byStartDTalkChan;
            g_deviceTree.g_struDeviceInfo[m_iDeviceIndex].byLanguageType = struDeviceInfo.byLanguageType;
            g_deviceTree.g_struDeviceInfo[m_iDeviceIndex].byMirrorChanNum = struDeviceInfo.byMirrorChanNum;
            g_deviceTree.g_struDeviceInfo[m_iDeviceIndex].wStartMirrorChanNo = struDeviceInfo.wStartMirrorChanNo;
            g_deviceTree.g_struDeviceInfo[m_iDeviceIndex].byAudioInputChanNum = struDeviceInfo.byVoiceInChanNum;
            g_deviceTree.g_struDeviceInfo[m_iDeviceIndex].byStartAudioInputChanNo = struDeviceInfo.byStartVoiceInChanNo;

            if (1 == (struDeviceInfo.bySupport & 0x80))
            {
                g_deviceTree.g_struDeviceInfo[m_iDeviceIndex].byMainProto = (byte)(struDeviceInfo.byMainProto + 2);
                g_deviceTree.g_struDeviceInfo[m_iDeviceIndex].bySubProto = (byte)(struDeviceInfo.bySubProto + 2);
            }
            else
            {
                g_deviceTree.g_struDeviceInfo[m_iDeviceIndex].byMainProto = struDeviceInfo.byMainProto;
                g_deviceTree.g_struDeviceInfo[m_iDeviceIndex].bySubProto = struDeviceInfo.bySubProto;
            }

            g_deviceTree.g_struDeviceInfo[m_iDeviceIndex].bySupport1 = struDeviceInfo.bySupport1;
            g_deviceTree.g_struDeviceInfo[m_iDeviceIndex].bySupport2 = struDeviceInfo.bySupport2;
            g_deviceTree.g_struDeviceInfo[m_iDeviceIndex].bySupport7 = struDeviceInfo.bySupport7;
            g_deviceTree.g_struDeviceInfo[m_iDeviceIndex].byLanguageType = struDeviceInfo.byLanguageType;

            uint dwSize = (uint)Marshal.SizeOf(struNetCfg);
            IntPtr ptrNetCfg = Marshal.AllocHGlobal((int)dwSize);
            Marshal.StructureToPtr(struNetCfg, ptrNetCfg, false);

            if (!CHCNetSDK.NET_DVR_GetDVRConfig(lUserID, CHCNetSDK.NET_DVR_GET_NETCFG_V50, 0, ptrNetCfg, dwSize, ref dwReturned))
            {
                g_formList.AddLog(m_iDeviceIndex, AcsDemoPublic.OPERATION_FAIL_T, "NET_DVR_GET_NETCFG_V50");
            }
            else
            {
                //IPv6 temporary unrealized 
                struNetCfg = (CHCNetSDK.NET_DVR_NETCFG_V50)Marshal.PtrToStructure(ptrNetCfg, typeof(CHCNetSDK.NET_DVR_NETCFG_V50));
                g_deviceTree.g_struDeviceInfo[m_iDeviceIndex].chDeviceMultiIP = struNetCfg.struMulticastIpAddr.sIpV4;
                string strTemp = string.Format("multi-cast ipv4 {0}", struNetCfg.struMulticastIpAddr.sIpV4);
                g_formList.AddLog(m_iDeviceIndex, AcsDemoPublic.OPERATION_SUCC_T, strTemp);
            }
            Marshal.FreeHGlobal(ptrNetCfg);

            dwReturned = 0;
            uint dwSize2 = (uint)Marshal.SizeOf(struDevCfg);
            IntPtr ptrDevCfg = Marshal.AllocHGlobal((int)dwSize2);
            Marshal.StructureToPtr(struDevCfg, ptrDevCfg, false);

            if (!CHCNetSDK.NET_DVR_GetDVRConfig(lUserID, CHCNetSDK.NET_DVR_GET_DEVICECFG_V40, 0, ptrDevCfg, dwSize2, ref dwReturned))
            {
                g_formList.AddLog(lUserID, AcsDemoPublic.OPERATION_FAIL_T, "NET_DVR_GET_DEVICECFG_V40");
            }
            else
            {
                struDevCfg = (CHCNetSDK.NET_DVR_DEVICECFG_V40)Marshal.PtrToStructure(ptrDevCfg, typeof(CHCNetSDK.NET_DVR_DEVICECFG_V40));
                if (g_deviceTree.g_struDeviceInfo[m_iDeviceIndex].iDeviceType != (int)struDevCfg.wDevType)
                {
                   // string strTemp = null;
                    string strShow = null;
                   // g_formList.g_StringLanType(ref strTemp, "登陆返回设备类型值与获取设备参数返回设备类型值不同", "returned device type is different between login and get device config");
                    strShow = "returned device type is different between login and get device config" + g_deviceTree.g_struDeviceInfo[m_iDeviceIndex].iDeviceType.ToString() + " " + struDevCfg.wDevType.ToString();
                    MessageBox.Show(strShow);
                }
                g_deviceTree.g_struDeviceInfo[m_iDeviceIndex].chDeviceName = System.Text.Encoding.UTF8.GetString(struDevCfg.byDevTypeName).Trim('\0');
                g_deviceTree.g_struDeviceInfo[m_iDeviceIndex].dwDevSoftVer = struDevCfg.dwSoftwareVersion;
            }
            Marshal.FreeHGlobal(ptrDevCfg);

            if (g_deviceTree.g_struDeviceInfo[m_iDeviceIndex].iIPChanNum >= 0)
            {
                if (g_deviceTree.g_struDeviceInfo[m_iDeviceIndex].iIPChanNum == 0)
                {
                    g_deviceTree.g_struDeviceInfo[m_iDeviceIndex].pStruIPParaCfgV40 = new CHCNetSDK.NET_DVR_IPPARACFG_V40[1];
                }
                else
                {
                    if (g_deviceTree.g_struDeviceInfo[m_iDeviceIndex].iIPChanNum % CHCNetSDK.MAX_CHANNUM_V30 == 0)
                    {
                        g_deviceTree.g_struDeviceInfo[m_iDeviceIndex].pStruIPParaCfgV40 =
                            new CHCNetSDK.NET_DVR_IPPARACFG_V40[g_deviceTree.g_struDeviceInfo[m_iDeviceIndex].iIPChanNum / CHCNetSDK.MAX_CHANNUM_V30];
                    }
                    else
                    {
                        g_deviceTree.g_struDeviceInfo[m_iDeviceIndex].pStruIPParaCfgV40 =
                            new CHCNetSDK.NET_DVR_IPPARACFG_V40[g_deviceTree.g_struDeviceInfo[m_iDeviceIndex].iIPChanNum / CHCNetSDK.MAX_CHANNUM_V30 + 1];
                    }
                }
            }

            //if (DoGetDeviceResoureCfg(m_iDeviceIndex))
            //{

            //}

            return true;
        }
        public bool DoGetDeviceResoureCfg(int iDeviceIndex, int iGroupNO = 0)
        {
            int i = 0, j = 0;
            uint dwReturned = 0;
            int dwSize = 0;
            int iLoopTime = 0;

            if (g_deviceTree.g_struDeviceInfo[iDeviceIndex].iIPChanNum > 0)
            {
                if (g_deviceTree.g_struDeviceInfo[iDeviceIndex].iIPChanNum % (int)(CHCNetSDK.MAX_CHANNUM_V30) == 0)
                {
                    iLoopTime = g_deviceTree.g_struDeviceInfo[iDeviceIndex].iIPChanNum / (int)(CHCNetSDK.MAX_CHANNUM_V30);
                }
                else
                {
                    iLoopTime = g_deviceTree.g_struDeviceInfo[iDeviceIndex].iIPChanNum / (int)(CHCNetSDK.MAX_CHANNUM_V30) + 1;
                }
            }

            for (j = 0; j < iLoopTime; j++)
            {
                CHCNetSDK.NET_DVR_IPPARACFG_V40 struIPAccessCfgV40 = new CHCNetSDK.NET_DVR_IPPARACFG_V40();
                iGroupNO = j;
                struIPAccessCfgV40 = g_deviceTree.g_struDeviceInfo[iDeviceIndex].pStruIPParaCfgV40[iGroupNO];
                dwSize = Marshal.SizeOf(struIPAccessCfgV40);
                IntPtr ptrIPAccessCfgV40 = Marshal.AllocHGlobal(dwSize);
                Marshal.StructureToPtr(struIPAccessCfgV40, ptrIPAccessCfgV40, false);

                //2008-9-15 13:44 ip input configuration
                g_deviceTree.g_struDeviceInfo[iDeviceIndex].bIPRet =
                    CHCNetSDK.NET_DVR_GetDVRConfig(g_deviceTree.g_struDeviceInfo[iDeviceIndex].lLoginID, CHCNetSDK.NET_DVR_GET_IPPARACFG_V40, iGroupNO, ptrIPAccessCfgV40, (uint)dwSize, ref dwReturned);
                if (!g_deviceTree.g_struDeviceInfo[iDeviceIndex].bIPRet)
                {	///device no support ip access
                    g_deviceTree.g_struDeviceInfo[iDeviceIndex].lFirstEnableChanIndex = 0;
                    g_formList.AddLog(iDeviceIndex, AcsDemoPublic.OPERATION_FAIL_T, "NET_DVR_GET_IPPARACFG_V40");
                    i = j + iGroupNO * CHCNetSDK.MAX_CHANNUM_V30;
                    if (i < g_deviceTree.g_struDeviceInfo[iDeviceIndex].iAnalogChanNum)
                    {
                        g_deviceTree.g_struDeviceInfo[iDeviceIndex].pStruChanInfo[i].iDeviceIndex = iDeviceIndex;
                        g_deviceTree.g_struDeviceInfo[iDeviceIndex].pStruChanInfo[i].iChanIndex = i;
                        g_deviceTree.g_struDeviceInfo[iDeviceIndex].pStruChanInfo[i].iChannelNO = i + g_deviceTree.g_struDeviceInfo[iDeviceIndex].iStartChan;
                        g_deviceTree.g_struDeviceInfo[iDeviceIndex].pStruChanInfo[i].bEnable = true;
                        g_deviceTree.g_struDeviceInfo[iDeviceIndex].pStruChanInfo[i].iChanType = AcsDemoPublic.DEMO_CHANNEL_TYPE.DEMO_CHANNEL_TYPE_ANALOG;
                        g_deviceTree.g_struDeviceInfo[m_iDeviceIndex].pStruChanInfo[i].chChanName = string.Format("Camera{0}", i + g_deviceTree.g_struDeviceInfo[m_iDeviceIndex].iStartChan); ;
                    }
                    else//clear the state of other channel
                    {
                        g_deviceTree.g_struDeviceInfo[iDeviceIndex].pStruChanInfo[i].iDeviceIndex = -1;
                        g_deviceTree.g_struDeviceInfo[iDeviceIndex].pStruChanInfo[i].iChanIndex = -1;
                        g_deviceTree.g_struDeviceInfo[iDeviceIndex].pStruChanInfo[i].bEnable = false;
                        g_deviceTree.g_struDeviceInfo[m_iDeviceIndex].pStruChanInfo[i].chChanName = "";
                    }


                    g_deviceTree.g_struDeviceInfo[iDeviceIndex].iGroupNO = -1;
                }
                else
                {
                    struIPAccessCfgV40 = (CHCNetSDK.NET_DVR_IPPARACFG_V40)Marshal.PtrToStructure(ptrIPAccessCfgV40, typeof(CHCNetSDK.NET_DVR_IPPARACFG_V40));
                    g_deviceTree.g_struDeviceInfo[iDeviceIndex].pStruIPParaCfgV40[iGroupNO] = struIPAccessCfgV40;
                    g_deviceTree.g_struDeviceInfo[iDeviceIndex].iGroupNO = iGroupNO;
                    g_formList.AddLog(iDeviceIndex, AcsDemoPublic.OPERATION_SUCC_T, "NET_DVR_GET_IPPARACFG_V40");
                    RefreshIPDevLocalCfg(iDeviceIndex);
                }
                Marshal.FreeHGlobal(ptrIPAccessCfgV40);
            }

            for (i = 0; i < g_deviceTree.g_struDeviceInfo[m_iDeviceIndex].iAnalogChanNum; i++)
            {
                g_deviceTree.g_struDeviceInfo[m_iDeviceIndex].pStruChanInfo[i].iDeviceIndex = m_iDeviceIndex;
                g_deviceTree.g_struDeviceInfo[m_iDeviceIndex].pStruChanInfo[i].iChanIndex = i;
                g_deviceTree.g_struDeviceInfo[m_iDeviceIndex].pStruChanInfo[i].iChannelNO = i + g_deviceTree.g_struDeviceInfo[m_iDeviceIndex].iStartChan;
                g_deviceTree.g_struDeviceInfo[m_iDeviceIndex].pStruChanInfo[i].chChanName = string.Format("Camera{0}", i + g_deviceTree.g_struDeviceInfo[m_iDeviceIndex].iStartChan);

                g_deviceTree.g_struDeviceInfo[m_iDeviceIndex].pStruChanInfo[i].bEnable = true;
                g_deviceTree.g_struDeviceInfo[iDeviceIndex].pStruChanInfo[i].iChanType = AcsDemoPublic.DEMO_CHANNEL_TYPE.DEMO_CHANNEL_TYPE_ANALOG;
                g_deviceTree.g_struDeviceInfo[m_iDeviceIndex].pStruChanInfo[i].dwImageType = AcsDemoPublic.CHAN_ORIGINAL;

            }

            if ((g_deviceTree.g_struDeviceInfo[m_iDeviceIndex].byMirrorChanNum > 0) &&
                (g_deviceTree.g_struDeviceInfo[m_iDeviceIndex].wStartMirrorChanNo > (g_deviceTree.g_struDeviceInfo[m_iDeviceIndex].iDeviceChanNum - 1)))
            {
                for (i = 0; i < g_deviceTree.g_struDeviceInfo[m_iDeviceIndex].byMirrorChanNum && i < 16; i++)
                {
                    g_deviceTree.g_struDeviceInfo[m_iDeviceIndex].struMirrorChan[i].iDeviceIndex = m_iDeviceIndex;
                    g_deviceTree.g_struDeviceInfo[m_iDeviceIndex].struMirrorChan[i].iChanIndex = i + AcsDemoPublic.MIRROR_CHAN_INDEX;
                    g_deviceTree.g_struDeviceInfo[m_iDeviceIndex].struMirrorChan[i].iChannelNO = i + g_deviceTree.g_struDeviceInfo[m_iDeviceIndex].wStartMirrorChanNo;
                    g_deviceTree.g_struDeviceInfo[m_iDeviceIndex].struMirrorChan[i].chChanName = string.Format("MirrorChan{0}", i + 1);

                    //analog devices
                    g_deviceTree.g_struDeviceInfo[m_iDeviceIndex].struMirrorChan[i].bEnable = true;
                    g_deviceTree.g_struDeviceInfo[iDeviceIndex].pStruChanInfo[i].iChanType = AcsDemoPublic.DEMO_CHANNEL_TYPE.DEMO_CHANNEL_TYPE_IP;
                    g_deviceTree.g_struDeviceInfo[m_iDeviceIndex].struMirrorChan[i].dwImageType = AcsDemoPublic.CHAN_ORIGINAL;
                }
            }
            if (g_deviceTree.g_struDeviceInfo[m_iDeviceIndex].byZeroChanNum > 0)
            {
                for (i = 0; i < g_deviceTree.g_struDeviceInfo[m_iDeviceIndex].byZeroChanNum; i++)
                {
                    g_deviceTree.g_struDeviceInfo[m_iDeviceIndex].struZeroChan[i].iDeviceIndex = m_iDeviceIndex;
                    g_deviceTree.g_struDeviceInfo[m_iDeviceIndex].struZeroChan[i].iChanIndex = i + AcsDemoPublic.ZERO_CHAN_INDEX;
                    g_deviceTree.g_struDeviceInfo[m_iDeviceIndex].struZeroChan[i].chChanName = string.Format("ZeroChan {0}", i);

                    g_deviceTree.g_struDeviceInfo[m_iDeviceIndex].struZeroChan[i].bEnable = true;
                    g_deviceTree.g_struDeviceInfo[iDeviceIndex].pStruChanInfo[i].iChanType = AcsDemoPublic.DEMO_CHANNEL_TYPE.DEMO_CHANNEL_TYPE_MIRROR;
                    g_deviceTree.g_struDeviceInfo[m_iDeviceIndex].struZeroChan[i].dwImageType = AcsDemoPublic.CHAN_ORIGINAL;

                }
            }
            return g_deviceTree.g_struDeviceInfo[iDeviceIndex].bIPRet;
        }

        public void RefreshIPDevLocalCfg(int iDeviceIndex)
        {
            CHCNetSDK.NET_DVR_IPPARACFG_V40 struIPAccessCfgV40 = g_deviceTree.g_struDeviceInfo[iDeviceIndex].pStruIPParaCfgV40[g_deviceTree.g_struDeviceInfo[iDeviceIndex].iGroupNO];
            uint dwChanShow = 0;
            int iIPChanIndex = 0;
            int i = 0;
            g_deviceTree.g_struDeviceInfo[iDeviceIndex].iIPChanNum = (int)struIPAccessCfgV40.dwDChanNum;

            int iAnalogChanCount = 0;
            int iIPChanCount = 0;
            int iGroupNO = g_deviceTree.g_struDeviceInfo[iDeviceIndex].iGroupNO;    //Group NO.
            int iGroupNum = (int)struIPAccessCfgV40.dwGroupNum;
            int iIPChanNum = g_deviceTree.g_struDeviceInfo[iDeviceIndex].iIPChanNum;

            for (i = 0; i < CHCNetSDK.MAX_CHANNUM_V30; i++)
            {

                //analog channel
                if (iAnalogChanCount < g_deviceTree.g_struDeviceInfo[iDeviceIndex].iAnalogChanNum)
                {
                    dwChanShow = (uint)(iAnalogChanCount + g_deviceTree.g_struDeviceInfo[iDeviceIndex].iStartChan + g_deviceTree.g_struDeviceInfo[iDeviceIndex].iGroupNO * 64);

                    g_deviceTree.g_struDeviceInfo[iDeviceIndex].pStruChanInfo[i + g_deviceTree.g_struDeviceInfo[iDeviceIndex].iGroupNO * 64].iDeviceIndex = iDeviceIndex;
                    g_deviceTree.g_struDeviceInfo[iDeviceIndex].pStruChanInfo[i + g_deviceTree.g_struDeviceInfo[iDeviceIndex].iGroupNO * 64].iChanIndex = i;
                    g_deviceTree.g_struDeviceInfo[iDeviceIndex].pStruChanInfo[i + g_deviceTree.g_struDeviceInfo[iDeviceIndex].iGroupNO * 64].iChanType = AcsDemoPublic.DEMO_CHANNEL_TYPE.DEMO_CHANNEL_TYPE_ANALOG;
                    g_deviceTree.g_struDeviceInfo[iDeviceIndex].pStruChanInfo[i + g_deviceTree.g_struDeviceInfo[iDeviceIndex].iGroupNO * 64].iChannelNO = (int)dwChanShow;

                    g_deviceTree.g_struDeviceInfo[m_iDeviceIndex].pStruChanInfo[i].chChanName =
                        string.Format("Camera{0}", i + g_deviceTree.g_struDeviceInfo[m_iDeviceIndex].iStartChan - g_deviceTree.g_struDeviceInfo[m_iDeviceIndex].iAnalogChanNum);
                    //analog devices
                    if (struIPAccessCfgV40.byAnalogChanEnable[i] > 0)
                    {
                        g_deviceTree.g_struDeviceInfo[iDeviceIndex].pStruChanInfo[i + g_deviceTree.g_struDeviceInfo[iDeviceIndex].iGroupNO * 64].bEnable = true;
                        g_deviceTree.g_struDeviceInfo[iDeviceIndex].pStruChanInfo[i + g_deviceTree.g_struDeviceInfo[iDeviceIndex].iGroupNO * 64].dwImageType = AcsDemoPublic.CHAN_ORIGINAL;
                        //g_struDeviceInfo[iDeviceIndex].iEnableChanNum ++;
                    }
                    else
                    {
                        g_deviceTree.g_struDeviceInfo[iDeviceIndex].pStruChanInfo[i + g_deviceTree.g_struDeviceInfo[iDeviceIndex].iGroupNO * 64].bEnable = false;
                        g_deviceTree.g_struDeviceInfo[iDeviceIndex].pStruChanInfo[i + g_deviceTree.g_struDeviceInfo[iDeviceIndex].iGroupNO * 64].dwImageType = AcsDemoPublic.CHAN_OFF_LINE;
                    }

                    iAnalogChanCount++;
                }
                else if (iGroupNO >= 0 && ((iIPChanCount + iGroupNO * CHCNetSDK.MAX_CHANNUM_V30) < iIPChanNum) && (iIPChanCount < iIPChanNum))
                {
                    dwChanShow = (uint)(iIPChanCount + iGroupNO * CHCNetSDK.MAX_CHANNUM_V30 + struIPAccessCfgV40.dwStartDChan);

                    g_deviceTree.g_struDeviceInfo[iDeviceIndex].pStruChanInfo[i + g_deviceTree.g_struDeviceInfo[iDeviceIndex].iGroupNO * 64].iChanType = AcsDemoPublic.DEMO_CHANNEL_TYPE.DEMO_CHANNEL_TYPE_IP;
                    g_deviceTree.g_struDeviceInfo[iDeviceIndex].pStruChanInfo[i + g_deviceTree.g_struDeviceInfo[iDeviceIndex].iGroupNO * 64].iChannelNO = (int)dwChanShow;

                    iIPChanIndex = iIPChanCount;
                    g_deviceTree.g_struDeviceInfo[iDeviceIndex].pStruChanInfo[i + g_deviceTree.g_struDeviceInfo[iDeviceIndex].iGroupNO * 64].iDeviceIndex = iDeviceIndex;
                    g_deviceTree.g_struDeviceInfo[iDeviceIndex].pStruChanInfo[i + g_deviceTree.g_struDeviceInfo[iDeviceIndex].iGroupNO * 64].iChanIndex = i + g_deviceTree.g_struDeviceInfo[iDeviceIndex].iGroupNO * 64;
                    g_deviceTree.g_struDeviceInfo[m_iDeviceIndex].pStruChanInfo[i].chChanName =
                        string.Format("IPCamera{0}", iIPChanCount + iGroupNO * CHCNetSDK.MAX_CHANNUM_V30 + 1);

                    if (struIPAccessCfgV40.struStreamMode[iIPChanIndex].uGetStream.struChanInfo.byIPID != 0 && struIPAccessCfgV40.struStreamMode[iIPChanIndex].uGetStream.struChanInfo.byEnable != 0)
                    {
                        g_deviceTree.g_struDeviceInfo[iDeviceIndex].pStruChanInfo[i + g_deviceTree.g_struDeviceInfo[iDeviceIndex].iGroupNO * 64].bEnable = true;//
                        if (struIPAccessCfgV40.struStreamMode[iIPChanIndex].uGetStream.struChanInfo.byEnable > 0)
                        {
                            g_deviceTree.g_struDeviceInfo[iDeviceIndex].pStruChanInfo[i + g_deviceTree.g_struDeviceInfo[iDeviceIndex].iGroupNO * 64].dwImageType = AcsDemoPublic.CHAN_ORIGINAL;
                        }
                        else
                        {
                            g_deviceTree.g_struDeviceInfo[iDeviceIndex].pStruChanInfo[i + g_deviceTree.g_struDeviceInfo[iDeviceIndex].iGroupNO * 64].dwImageType = AcsDemoPublic.CHAN_OFF_LINE;
                        }

                        //				g_struDeviceInfo[iDeviceIndex].iEnableChanNum ++;
                    }
                    else
                    {
                        g_deviceTree.g_struDeviceInfo[iDeviceIndex].pStruChanInfo[i + g_deviceTree.g_struDeviceInfo[iDeviceIndex].iGroupNO * 64].dwImageType = AcsDemoPublic.CHAN_OFF_LINE;
                        g_deviceTree.g_struDeviceInfo[iDeviceIndex].pStruChanInfo[i + g_deviceTree.g_struDeviceInfo[iDeviceIndex].iGroupNO * 64].bEnable = false;
                        //g_struDeviceInfo[iDeviceIndex].struChanInfo[i].bAlarm = FALSE;
                    }

                    iIPChanCount++;
                }
                else
                {
                    g_deviceTree.g_struDeviceInfo[iDeviceIndex].pStruChanInfo[i + g_deviceTree.g_struDeviceInfo[iDeviceIndex].iGroupNO * 64].iDeviceIndex = -1;
                    g_deviceTree.g_struDeviceInfo[iDeviceIndex].pStruChanInfo[i + g_deviceTree.g_struDeviceInfo[iDeviceIndex].iGroupNO * 64].iChanIndex = -1;
                    g_deviceTree.g_struDeviceInfo[iDeviceIndex].pStruChanInfo[i + g_deviceTree.g_struDeviceInfo[iDeviceIndex].iGroupNO * 64].iChanType = AcsDemoPublic.DEMO_CHANNEL_TYPE.DEMO_CHANNEL_TYPE_INVALID;
                    g_deviceTree.g_struDeviceInfo[iDeviceIndex].pStruChanInfo[i + g_deviceTree.g_struDeviceInfo[iDeviceIndex].iGroupNO * 64].iChannelNO = -1;
                    g_deviceTree.g_struDeviceInfo[iDeviceIndex].pStruChanInfo[i + g_deviceTree.g_struDeviceInfo[iDeviceIndex].iGroupNO * 64].bEnable = false;
                    g_deviceTree.g_struDeviceInfo[iDeviceIndex].pStruChanInfo[i + g_deviceTree.g_struDeviceInfo[iDeviceIndex].iGroupNO * 64].bAlarm = false;
                    g_deviceTree.g_struDeviceInfo[iDeviceIndex].pStruChanInfo[i + g_deviceTree.g_struDeviceInfo[iDeviceIndex].iGroupNO * 64].bLocalManualRec = false;
                    g_deviceTree.g_struDeviceInfo[iDeviceIndex].pStruChanInfo[i + g_deviceTree.g_struDeviceInfo[iDeviceIndex].iGroupNO * 64].lRealHandle = -1;
                    g_deviceTree.g_struDeviceInfo[m_iDeviceIndex].pStruChanInfo[i].chChanName = "";
                }
            }

            for (i = 0; i < CHCNetSDK.MAX_CHANNUM_V40; i++)
            {
                if (g_deviceTree.g_struDeviceInfo[iDeviceIndex].pStruChanInfo[i + g_deviceTree.g_struDeviceInfo[iDeviceIndex].iGroupNO * 64].bEnable)
                {
                    g_deviceTree.g_struDeviceInfo[iDeviceIndex].lFirstEnableChanIndex = i;
                    break;
                }
            }
        }
        private void buttonCancel_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        private void AddDevice_Load(object sender, EventArgs e)
        {
            MultiLanguage.LoadLanguage(this);
        }

        private void AysnLogincheckBox_CheckedChanged(object sender, EventArgs e)
        {
            if (AysnLogincheckBox.Checked)
            {
                AysnLoginFlag = true;
            }
            else
            {
                AysnLoginFlag = false;
            }
        }
    }
}
