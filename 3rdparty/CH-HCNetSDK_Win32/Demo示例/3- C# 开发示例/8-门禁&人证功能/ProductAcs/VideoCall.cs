using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using AcsDemo.Language;
using AcsDemo.Public;

namespace AcsDemo.ProductAcs
{
    public partial class VideoCall : Form
    {
        private DeviceLogList g_formList = DeviceLogList.Instance();
        public Int32 m_lUserID = -1;
        public int m_iDevIndex = -1;
        private int m_lRemoteHandle = -1;
        bool m_bClientCall = true;
        private int m_lRealPlayHandle = -1;
        private int m_lVoiceTalkHandle = -1;

        //public IntPtr hwndTest;

        private CHCNetSDK.RemoteConfigCallback g_fProcessVideoCallDataCallBack = null;

        private void StartPlay()
        {
            if (m_lRealPlayHandle != -1)
            {
                return;
            }

            CHCNetSDK.NET_DVR_CLIENTINFO struClientInfo = new CHCNetSDK.NET_DVR_CLIENTINFO();
            struClientInfo.hPlayWnd = PreViewBox.Handle;
            struClientInfo.lChannel = 1;
            struClientInfo.lLinkMode = 0;
            IntPtr ptrUser = new IntPtr();

            m_lRealPlayHandle = CHCNetSDK.NET_DVR_RealPlay_V30(m_lUserID, ref struClientInfo, null, ptrUser, 1);
            if (m_lRealPlayHandle == -1)
            {
                uint iLastErr = CHCNetSDK.NET_DVR_GetLastError();
                string strTemp = "NET_DVR_RealPlay_V30 failed, error code= " + iLastErr;
                g_formList.AddLog(m_iDevIndex, AcsDemoPublic.OPERATION_FAIL_T, strTemp);  
            }
            Marshal.FreeHGlobal(ptrUser);
        }

        private void StopPlay()
        {
            if (m_lRealPlayHandle == -1)
            {
                return;
            }
            if (CHCNetSDK.NET_DVR_StopRealPlay(m_lRealPlayHandle))
            {
                g_formList.AddLog(m_iDevIndex, AcsDemoPublic.OPERATION_SUCC_T, "NET_DVR_StopRealPlay");  
                m_lRealPlayHandle = -1;
                PreViewBox.Refresh();
            }
            else
            {
                g_formList.AddLog(m_iDevIndex, AcsDemoPublic.OPERATION_FAIL_T, "NET_DVR_StopRealPlay");  
            }
        }

        void StartVoiceTalk()
        {
            if (m_lVoiceTalkHandle == -1)
            {
                return;
            }

            IntPtr ptrUser = new IntPtr();

            m_lVoiceTalkHandle = CHCNetSDK.NET_DVR_StartVoiceCom_V30(m_lUserID, 1, false, null, ptrUser);
            if (m_lVoiceTalkHandle == -1)
            {
                uint iLastErr = CHCNetSDK.NET_DVR_GetLastError();
                string strTemp = "NET_DVR_StartVoiceCom_V30 failed, error code= " + iLastErr;
                g_formList.AddLog(m_iDevIndex, AcsDemoPublic.OPERATION_FAIL_T, strTemp);
            }
            Marshal.FreeHGlobal(ptrUser);
        }

        void StopAudioIntercom()
        {
            if (m_lVoiceTalkHandle == -1)
            {
                return;
            }
            if (CHCNetSDK.NET_DVR_StopVoiceCom(m_lVoiceTalkHandle))
            {
                g_formList.AddLog(m_iDevIndex, AcsDemoPublic.OPERATION_SUCC_T, "NET_DVR_StopVoiceCom");
                m_lVoiceTalkHandle = -1;
            }
            else
            {
                g_formList.AddLog(m_iDevIndex, AcsDemoPublic.OPERATION_FAIL_T, "NET_DVR_StopVoiceCom");
            }
        }

        private void SendCallCmd()
        {
            if (m_lRemoteHandle == -1)
            {
                return;
            }

            CHCNetSDK.NET_DVR_VIDEO_CALL_PARAM struVideoCallParam = new CHCNetSDK.NET_DVR_VIDEO_CALL_PARAM();
            struVideoCallParam.dwSize = (uint)Marshal.SizeOf(struVideoCallParam);
            struVideoCallParam.dwCmdType = 0;
            uint dwSize = (uint)Marshal.SizeOf(struVideoCallParam);
            IntPtr ptrStruVideoCallParam = Marshal.AllocHGlobal((int)dwSize);
            Marshal.StructureToPtr(struVideoCallParam, ptrStruVideoCallParam, false);

            if (CHCNetSDK.NET_DVR_SendRemoteConfig(m_lRemoteHandle, 0, ptrStruVideoCallParam, dwSize))
            {
                g_formList.AddLog(m_iDevIndex, AcsDemoPublic.OPERATION_SUCC_T, "NET_DVR_SendRemoteConfig dwCmdType:" + struVideoCallParam.dwCmdType);
            }
            else
            {
                g_formList.AddLog(m_iDevIndex, AcsDemoPublic.OPERATION_FAIL_T, "NET_DVR_SendRemoteConfig dwCmdType:" + struVideoCallParam.dwCmdType);
            }
            Marshal.FreeHGlobal(ptrStruVideoCallParam);

            buttonCall.Hide();
            buttonAnswer.Hide();
            buttonCancel.Show();
            buttonHangUp.Show();
        }

        private void SendCancellCmd()
        {
            if (m_lRemoteHandle < 0)
            {
                return;
            }

            CHCNetSDK.NET_DVR_VIDEO_CALL_PARAM struVideoCallParam = new CHCNetSDK.NET_DVR_VIDEO_CALL_PARAM();
            struVideoCallParam.dwSize = (uint)Marshal.SizeOf(struVideoCallParam);
            struVideoCallParam.dwCmdType = 1;
            uint dwSize = (uint)Marshal.SizeOf(struVideoCallParam);
            IntPtr ptrStruVideoCallParam = Marshal.AllocHGlobal((int)dwSize);
            Marshal.StructureToPtr(struVideoCallParam, ptrStruVideoCallParam, false);

            if (CHCNetSDK.NET_DVR_SendRemoteConfig(m_lRemoteHandle, 0, ptrStruVideoCallParam, dwSize))
            {
                g_formList.AddLog(m_iDevIndex, AcsDemoPublic.OPERATION_SUCC_T, "NET_DVR_SendRemoteConfig dwCmdType:" + struVideoCallParam.dwCmdType);
            }
            else
            {
                g_formList.AddLog(m_iDevIndex, AcsDemoPublic.OPERATION_FAIL_T, "NET_DVR_SendRemoteConfig dwCmdType:" + struVideoCallParam.dwCmdType);
            }
            Marshal.FreeHGlobal(ptrStruVideoCallParam);

            buttonCall.Show();
            buttonAnswer.Hide();
            buttonCancel.Hide();
            buttonHangUp.Hide();
        }

        private void SendAcceptCmd()
        {
            if (m_lRemoteHandle < 0)
            {
                return;
            }

            CHCNetSDK.NET_DVR_VIDEO_CALL_PARAM struVideoCallParam = new CHCNetSDK.NET_DVR_VIDEO_CALL_PARAM();
            struVideoCallParam.dwSize = (uint)Marshal.SizeOf(struVideoCallParam);
            struVideoCallParam.dwCmdType = 2;
            uint dwSize = (uint)Marshal.SizeOf(struVideoCallParam);
            IntPtr ptrStruVideoCallParam = Marshal.AllocHGlobal((int)dwSize);
            Marshal.StructureToPtr(struVideoCallParam, ptrStruVideoCallParam, false);

            if (CHCNetSDK.NET_DVR_SendRemoteConfig(m_lRemoteHandle, 0, ptrStruVideoCallParam, dwSize))
            {
                g_formList.AddLog(m_iDevIndex, AcsDemoPublic.OPERATION_SUCC_T, "NET_DVR_SendRemoteConfig dwCmdType:" + struVideoCallParam.dwCmdType);
            }
            else
            {
                g_formList.AddLog(m_iDevIndex, AcsDemoPublic.OPERATION_FAIL_T, "NET_DVR_SendRemoteConfig dwCmdType:" + struVideoCallParam.dwCmdType);
            }
            Marshal.FreeHGlobal(ptrStruVideoCallParam);

            buttonCall.Hide();
            buttonAnswer.Hide();
            buttonCancel.Hide();
            buttonHangUp.Show();
        }

        private void SendByeCmd()
        {
            if (m_lRemoteHandle < 0)
            {
                return;
            }

            CHCNetSDK.NET_DVR_VIDEO_CALL_PARAM struVideoCallParam = new CHCNetSDK.NET_DVR_VIDEO_CALL_PARAM();
            struVideoCallParam.dwSize = (uint)Marshal.SizeOf(struVideoCallParam);
            struVideoCallParam.dwCmdType = 5;
            uint dwSize = (uint)Marshal.SizeOf(struVideoCallParam);
            IntPtr ptrStruVideoCallParam = Marshal.AllocHGlobal((int)dwSize);
            Marshal.StructureToPtr(struVideoCallParam, ptrStruVideoCallParam, false);

            if (CHCNetSDK.NET_DVR_SendRemoteConfig(m_lRemoteHandle, 0, ptrStruVideoCallParam, dwSize))
            {
                g_formList.AddLog(m_iDevIndex, AcsDemoPublic.OPERATION_SUCC_T, "NET_DVR_SendRemoteConfig dwCmdType:" + struVideoCallParam.dwCmdType);
            }
            else
            {
                g_formList.AddLog(m_iDevIndex, AcsDemoPublic.OPERATION_FAIL_T, "NET_DVR_SendRemoteConfig dwCmdType:" + struVideoCallParam.dwCmdType);
            }
            Marshal.FreeHGlobal(ptrStruVideoCallParam);

            buttonCall.Show();
            buttonAnswer.Hide();
            buttonCancel.Hide();
            buttonHangUp.Hide();
        }

        private void ProcessVideoCallDataCallBack(uint dwType, IntPtr lpBuffer, uint dwBufLen, IntPtr pUserData)
        {
            if (pUserData == null)
            {
                return;
            }

            if (dwType == (uint)CHCNetSDK.NET_SDK_CALLBACK_TYPE.NET_SDK_CALLBACK_TYPE_DATA)
            {
                CHCNetSDK.NET_DVR_VIDEO_CALL_PARAM struVideoCallParam = new CHCNetSDK.NET_DVR_VIDEO_CALL_PARAM();
                struVideoCallParam = (CHCNetSDK.NET_DVR_VIDEO_CALL_PARAM)Marshal.PtrToStructure(lpBuffer, typeof(CHCNetSDK.NET_DVR_VIDEO_CALL_PARAM));
                CHCNetSDK.PostMessage(pUserData, 1001, (int)struVideoCallParam.dwCmdType, 0);
            }
            else if (dwType == (uint)CHCNetSDK.NET_SDK_CALLBACK_TYPE.NET_SDK_CALLBACK_TYPE_STATUS)
            {
                uint dwStatus = (uint)Marshal.ReadInt32(lpBuffer);
                if (dwStatus == (uint)CHCNetSDK.NET_SDK_CALLBACK_STATUS_NORMAL.NET_SDK_CALLBACK_STATUS_SUCCESS)
                {
                    g_formList.AddLog(m_iDevIndex, AcsDemoPublic.OPERATION_SUCC_T, "Process Video Call Data Call Back Finish");
                }
                else if (dwStatus == (uint)CHCNetSDK.NET_SDK_CALLBACK_STATUS_NORMAL.NET_SDK_CALLBACK_STATUS_FAILED)
                {
                    g_formList.AddLog(m_iDevIndex, AcsDemoPublic.OPERATION_FAIL_T, "Process Video Call Data Call Back Failed");
                }
                else if (dwStatus == (uint)CHCNetSDK.NET_SDK_CALLBACK_STATUS_NORMAL.NET_SDK_CALLBACK_STATUS_EXCEPTION)
                {
                    g_formList.AddLog(m_iDevIndex, AcsDemoPublic.OPERATION_FAIL_T, "Process Video Call Data Call Back Exception");
                }
                else if (dwStatus == (uint)CHCNetSDK.NET_SDK_CALLBACK_STATUS_NORMAL.NET_SDK_CALLBACK_STATUS_PROCESSING)
                {
                    g_formList.AddLog(m_iDevIndex, AcsDemoPublic.OPERATION_SUCC_T, "Process Video Call Data Call Back Processing");
                }
            }
        }

        public VideoCall(Int32 lUserID, int iDevIndex)
        {
            m_lUserID = lUserID;
            m_iDevIndex = iDevIndex;
            InitializeComponent();

            buttonCall.Show();
            buttonAnswer.Hide();
            buttonCancel.Hide();
            buttonHangUp.Hide();

            StartRemoteCfg();
        }

        void StartRemoteCfg()
        {
            CHCNetSDK.NET_DVR_VIDEO_CALL_COND struCond = new CHCNetSDK.NET_DVR_VIDEO_CALL_COND();
            struCond.dwSize = (uint)Marshal.SizeOf(struCond);

            int dwSize = Marshal.SizeOf(struCond);
            IntPtr ptrStruCond = Marshal.AllocHGlobal(dwSize);
            Marshal.StructureToPtr(struCond, ptrStruCond, false);

            g_fProcessVideoCallDataCallBack = new CHCNetSDK.RemoteConfigCallback(ProcessVideoCallDataCallBack);
            m_lRemoteHandle = CHCNetSDK.NET_DVR_StartRemoteConfig(m_lUserID, CHCNetSDK.NET_DVR_VIDEO_CALL_SIGNAL_PROCESS, ptrStruCond, dwSize, g_fProcessVideoCallDataCallBack, this.Handle);
            if (m_lRemoteHandle == -1)
            {
                g_formList.AddLog(m_iDevIndex, AcsDemoPublic.OPERATION_FAIL_T, "NET_DVR_VIDEO_CALL_SIGNAL_PROCESS FAIL");    
            }
            else
            {
                g_formList.AddLog(m_iDevIndex, AcsDemoPublic.OPERATION_SUCC_T, "NET_DVR_VIDEO_CALL_SIGNAL_PROCESS SUCCESS");
            }
            Marshal.FreeHGlobal(ptrStruCond);
        }

        private void buttonCall_Click(object sender, EventArgs e)
        {
            SendCallCmd();
        }

        private void buttonAnswer_Click(object sender, EventArgs e)
        {
            if (m_lRemoteHandle == -1)
            {
                return;
            }
            StartPlay();
            SendAcceptCmd();
        }

        private void buttonCancel_Click(object sender, EventArgs e)
        {
            if (m_lRemoteHandle == -1)
            {
                return;
            }
            StopPlay();
            StopAudioIntercom();

            buttonCall.Show();
            buttonAnswer.Hide();
            buttonCancel.Hide();
            buttonHangUp.Hide();
        }

        private void buttonHangUp_Click(object sender, EventArgs e)
        {
            if (m_lRemoteHandle == -1)
            {
                return;
            }
            SendByeCmd();

            if (m_lRealPlayHandle >= 0)
            {
                StopPlay();
                StopAudioIntercom();
            }

            buttonCall.Show();
            buttonAnswer.Hide();
            buttonCancel.Hide();
            buttonHangUp.Hide();
        }


        protected override void DefWndProc(ref Message m)
        {
            switch (m.Msg)
            {
                case 1001:
                    int iCmdType = m.WParam.ToInt32();
                    if (iCmdType == 0)//Device Request Talk
                    {
                        m_bClientCall = false;

                        buttonCall.Hide();
                        buttonAnswer.Show();
                        buttonCancel.Hide();
                        buttonHangUp.Show();

                        labelShowStatus.Text = "Device Request Talk";
                    }
                    else if (iCmdType == 1)//Cancel Talk
                    {
                        StopPlay();

                        if (m_bClientCall)
                        {
                            SendCancellCmd();
                            labelShowStatus.Text = "Cancel Talk";
                        }
                        else
                        {
                            labelShowStatus.Text = "Device Cancel Talk";
                        }

                        buttonCall.Show();
                        buttonAnswer.Hide();
                        buttonCancel.Hide();
                        buttonHangUp.Hide();
                    }
                    else if (iCmdType == 2)//Accept This Talk
                    {
                        StartPlay();
                        StartVoiceTalk();

                        buttonCall.Hide();
                        buttonAnswer.Hide();
                        buttonCancel.Hide();
                        buttonHangUp.Show();

                        labelShowStatus.Text = "Calling...";
                    }
                    else if (iCmdType == 3)//Refuse Answer
                    {
                        buttonCall.Show();
                        buttonAnswer.Hide();
                        buttonCancel.Hide();
                        buttonHangUp.Hide();

                        if (m_bClientCall)
                        {
                            SendCancellCmd();
                            labelShowStatus.Text = "Refuse Answer...";
                        }
                        else
                        {
                            labelShowStatus.Text = "Other Party Refuse Answer...";
                        }
                    }
                    else if (iCmdType == 4)//No answer
                    {
                        StopPlay();

                        buttonCall.Show();
                        buttonAnswer.Hide();
                        buttonCancel.Hide();
                        buttonHangUp.Hide();

                        if (m_bClientCall)
                        {
                            labelShowStatus.Text = "No answer";
                        }
                        else
                        {
                            labelShowStatus.Text = "Bell Timeout";
                        }
                    }
                    else if (iCmdType == 5)//Finish This Call
                    {
                        StopPlay();
                        StopAudioIntercom();

                        buttonCall.Show();
                        buttonAnswer.Hide();
                        buttonCancel.Hide();
                        buttonHangUp.Hide();

                        if (m_bClientCall)
                        {
                            labelShowStatus.Text = "Closed...";
                        }
                        else
                        {
                            labelShowStatus.Text = "Other Party Closed...";
                        }
                    }
                    else if (iCmdType == 6)//Calling
                    {
                        StopPlay();
                        StopAudioIntercom();

                        buttonCall.Show();
                        buttonAnswer.Hide();
                        buttonCancel.Hide();
                        buttonHangUp.Hide();

                        labelShowStatus.Text = "Other Party In Calling...";
                    }
                    else
                    {
                        labelShowStatus.Text = "Unknown Command Type";
                    }
                    break;
                default:
                    base.DefWndProc(ref m);
                    break;      
            }
        }

        private void VideoCall_Load(object sender, EventArgs e)
        {
            MultiLanguage.LoadLanguage(this);
        }
    }
}
