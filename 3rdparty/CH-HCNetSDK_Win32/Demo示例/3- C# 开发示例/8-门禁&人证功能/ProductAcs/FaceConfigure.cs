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
using System.IO;
using AcsDemo.Language;
using AcsDemo.Public;

namespace AcsDemo.ProductAcs
{
    public partial class FaceConfigure : Form
    {
        public int m_lUserID = -1;
        private int m_lSetFaceCfgHandle = -1;
        private int m_lGetFaceCfgHandle = -1;
        private int m_lGetFacePicHandle = -1;
        private CHCNetSDK.RemoteConfigCallback g_fSetFaceParamCallback = null;
        private CHCNetSDK.RemoteConfigCallback g_fGetFaceParamCallback = null;
        private CHCNetSDK.RemoteConfigCallback g_fGetFacePicCallback = null;
        private DeviceLogList g_formList = DeviceLogList.Instance();
        public int m_lDeviceIndex = -1;

        public FaceConfigure()
        {
            InitializeComponent();
        }

        private void ProcessSetFaceParamCallback(uint dwType, IntPtr lpBuffer, uint dwBufLen, IntPtr pUserData)
        {
            if (pUserData == null)
            {
                return;
            }

            if (dwType == (uint)CHCNetSDK.NET_SDK_CALLBACK_TYPE.NET_SDK_CALLBACK_TYPE_STATUS)
            {
                uint dwStatus = (uint)Marshal.ReadInt32(lpBuffer);

                if (dwStatus == (uint)CHCNetSDK.NET_SDK_CALLBACK_STATUS_NORMAL.NET_SDK_CALLBACK_STATUS_PROCESSING)
                {
                    g_formList.AddLog(m_lDeviceIndex, AcsDemoPublic.OPERATION_SUCC_T, "SetFaceParam Processing");
                }
                else if (dwStatus == (uint)CHCNetSDK.NET_SDK_CALLBACK_STATUS_NORMAL.NET_SDK_CALLBACK_STATUS_FAILED)
                {
                    g_formList.AddLog(m_lDeviceIndex, AcsDemoPublic.OPERATION_FAIL_T, "SetFaceParam Failed");
                    CHCNetSDK.PostMessage(pUserData, 1001, 0, 0);
                }
                else if (dwStatus == (uint)CHCNetSDK.NET_SDK_CALLBACK_STATUS_NORMAL.NET_SDK_CALLBACK_STATUS_SUCCESS)
                {
                    g_formList.AddLog(m_lDeviceIndex, AcsDemoPublic.OPERATION_SUCC_T, "SetFaceParam Success");
                    CHCNetSDK.PostMessage(pUserData, 1001, 0, 0);
                }
                else if (dwStatus == (uint)CHCNetSDK.NET_SDK_CALLBACK_STATUS_NORMAL.NET_SDK_CALLBACK_STATUS_EXCEPTION)
                {
                    g_formList.AddLog(m_lDeviceIndex, AcsDemoPublic.OPERATION_FAIL_T, "SetFaceParam Exception");
                    CHCNetSDK.PostMessage(pUserData, 1001, 0, 0);
                }
                else
                {
                    g_formList.AddLog(m_lDeviceIndex, AcsDemoPublic.OPERATION_FAIL_T, "Unknown Status");
                    CHCNetSDK.PostMessage(pUserData, 1001, 0, 0);
                }
            }
            else if (dwType == (uint)CHCNetSDK.NET_SDK_CALLBACK_TYPE.NET_SDK_CALLBACK_TYPE_DATA)
            {
                var result = Marshal.PtrToStructure(lpBuffer, typeof(CHCNetSDK.NET_DVR_FACE_PARAM_STATUS));
                var struFaceParamStatus = (CHCNetSDK.NET_DVR_FACE_PARAM_STATUS)result;
                if (struFaceParamStatus.byCardReaderRecvStatus[0] != 1)
                {
                    g_formList.AddLog(m_lDeviceIndex, AcsDemoPublic.OPERATION_FAIL_T, "SetFaceParam Return Failed：byCardReaderRecvStatus " + struFaceParamStatus.byCardReaderRecvStatus[0]);
                }
            }
            return;
        }

        private void ProcessGetFaceParamCallback(uint dwType, IntPtr lpBuffer, uint dwBufLen, IntPtr pUserData)
        {
            if (pUserData == null)
            {
                return;
            }

            if (dwType == (uint)CHCNetSDK.NET_SDK_CALLBACK_TYPE.NET_SDK_CALLBACK_TYPE_STATUS)
            {
                uint dwStatus = (uint)Marshal.ReadInt32(lpBuffer);

                if (dwStatus == (uint)CHCNetSDK.NET_SDK_CALLBACK_STATUS_NORMAL.NET_SDK_CALLBACK_STATUS_PROCESSING)
                {
                    g_formList.AddLog(m_lDeviceIndex, AcsDemoPublic.OPERATION_SUCC_T, "GetFaceParam Processing");
                }
                else if (dwStatus == (uint)CHCNetSDK.NET_SDK_CALLBACK_STATUS_NORMAL.NET_SDK_CALLBACK_STATUS_FAILED)
                {
                    g_formList.AddLog(m_lDeviceIndex, AcsDemoPublic.OPERATION_FAIL_T, "GetFaceParam Failed");
                    CHCNetSDK.PostMessage(pUserData, 1002, 0, 0);
                }
                else if (dwStatus == (uint)CHCNetSDK.NET_SDK_CALLBACK_STATUS_NORMAL.NET_SDK_CALLBACK_STATUS_SUCCESS)
                {
                    g_formList.AddLog(m_lDeviceIndex, AcsDemoPublic.OPERATION_SUCC_T, "GetFaceParam Success");
                    CHCNetSDK.PostMessage(pUserData, 1002, 0, 0);
                }
                else if (dwStatus == (uint)CHCNetSDK.NET_SDK_CALLBACK_STATUS_NORMAL.NET_SDK_CALLBACK_STATUS_EXCEPTION)
                {
                    g_formList.AddLog(m_lDeviceIndex, AcsDemoPublic.OPERATION_FAIL_T, "GetFaceParam Exception");
                    CHCNetSDK.PostMessage(pUserData, 1002, 0, 0);
                }
                else
                {
                    g_formList.AddLog(m_lDeviceIndex, AcsDemoPublic.OPERATION_FAIL_T, "Unknown Status");
                    CHCNetSDK.PostMessage(pUserData, 1002, 0, 0);
                }
            }
            else if (dwType == (uint)CHCNetSDK.NET_SDK_CALLBACK_TYPE.NET_SDK_CALLBACK_TYPE_DATA)
            {
                var result = Marshal.PtrToStructure(lpBuffer, typeof(CHCNetSDK.NET_DVR_FACE_PARAM_CFG));
                var struFaceParamCfg = (CHCNetSDK.NET_DVR_FACE_PARAM_CFG)result;
                if (struFaceParamCfg.byEnableCardReader[0] != 1)
                {
                    g_formList.AddLog(m_lDeviceIndex, AcsDemoPublic.OPERATION_FAIL_T, "GetFaceParam Return Failed");
                }
                if (struFaceParamCfg.dwFaceLen != 0)
                {

                    FileStream fs = new FileStream("FacePicture.jpg", FileMode.Create);
                    int iLen = (int)struFaceParamCfg.dwFaceLen;
                    byte[] by = new byte[iLen];
                    Marshal.Copy(struFaceParamCfg.pFaceBuffer, by, 0, iLen);

                    fs.Write(by, 0, iLen);

                    fs.Close();
                }
            }
            return;
        }



        private void buttonBrowse_Click(object sender, EventArgs e)
        {
            OpenFileDialog openFileDialog = new OpenFileDialog();
            openFileDialog.InitialDirectory = "d:\\";//should be attention to the path is "d:\\" not "d:\"
            openFileDialog.Filter = "Face picture|*.jpg|All documents|*.*";
            openFileDialog.RestoreDirectory = true;
            openFileDialog.FilterIndex = 1;
            if (openFileDialog.ShowDialog() == DialogResult.OK)
            {
                textBoxFacePicturePath.Text = openFileDialog.FileName;
            }
            if (!File.Exists(textBoxFacePicturePath.Text))
            {
                MessageBox.Show("The face picture does not exist!");
                return;
            }
            FileStream fs = new FileStream(textBoxFacePicturePath.Text, FileMode.OpenOrCreate);
            if (0 == fs.Length)
            {
                MessageBox.Show("The face picture is 0k,please input another picture!");
            }
            if (200 * 1024 < fs.Length)
            {
                MessageBox.Show("The face picture is larger than 200k,please input another picture!");
            }
            fs.Close();
        }

        private void buttonGet_Click(object sender, EventArgs e)
        {
            if (-1 != m_lGetFaceCfgHandle)
            {
                if (CHCNetSDK.NET_DVR_StopRemoteConfig(m_lGetFaceCfgHandle))
                {
                    m_lGetFaceCfgHandle = -1;
                }
            }
            CHCNetSDK.NET_DVR_FACE_PARAM_COND struCond = new CHCNetSDK.NET_DVR_FACE_PARAM_COND();
            struCond.dwSize = (uint)Marshal.SizeOf(struCond);
            struCond.dwFaceNum = 1;
            struCond.byFaceID = 1;
            struCond.byEnableCardReader = new byte[CHCNetSDK.MAX_CARD_READER_NUM_512];
            struCond.byEnableCardReader[0] = 1;
            if ("" == textBoxCardNo.Text)
            {
                MessageBox.Show("Please input the card number!");
                return;
            }
            struCond.byCardNo = new byte[CHCNetSDK.ACS_CARD_NO_LEN];
            byte[] byTempCardNo = System.Text.Encoding.UTF8.GetBytes(textBoxCardNo.Text);
            for (int i = 0; i < byTempCardNo.Length; i++)
            {
                if (i > struCond.byCardNo.Length)
                {
                    MessageBox.Show("card number length too long!");
                    return;
                }
                struCond.byCardNo[i] = byTempCardNo[i];
            }

            int dwSize = Marshal.SizeOf(struCond);
            IntPtr ptrStruCond = Marshal.AllocHGlobal(dwSize);
            Marshal.StructureToPtr(struCond, ptrStruCond, false);
            g_fGetFaceParamCallback = new CHCNetSDK.RemoteConfigCallback(ProcessGetFaceParamCallback);
            m_lGetFaceCfgHandle = CHCNetSDK.NET_DVR_StartRemoteConfig(m_lUserID, CHCNetSDK.NET_DVR_GET_FACE_PARAM_CFG, ptrStruCond, dwSize, g_fGetFaceParamCallback, this.Handle);
            if (-1 == m_lGetFaceCfgHandle)
            {
                g_formList.AddLog(m_lDeviceIndex, AcsDemoPublic.OPERATION_FAIL_T, "NET_DVR_GET_FACE_PARAM_CFG");
            }
            else
            {
                g_formList.AddLog(m_lDeviceIndex, AcsDemoPublic.OPERATION_SUCC_T, "NET_DVR_GET_FACE_PARAM_CFG");
            }
            Marshal.FreeHGlobal(ptrStruCond);
        }

        private void buttonPut_Click(object sender, EventArgs e)
        {
            if (-1 != m_lSetFaceCfgHandle)
            {
                if (CHCNetSDK.NET_DVR_StopRemoteConfig(m_lSetFaceCfgHandle))
                {
                    m_lSetFaceCfgHandle = -1;
                }
            }
            CHCNetSDK.NET_DVR_FACE_PARAM_COND struCond = new CHCNetSDK.NET_DVR_FACE_PARAM_COND();
            struCond.dwSize = (uint)Marshal.SizeOf(struCond);
            struCond.dwFaceNum = 1;
            struCond.byFaceID = 1;
            struCond.byEnableCardReader = new byte[CHCNetSDK.MAX_CARD_READER_NUM_512];
            struCond.byEnableCardReader[0] = 1;
            if ("" == textBoxCardNo.Text)
            {
                MessageBox.Show("Please input the card number!");
                return;
            }
            struCond.byCardNo = new byte[CHCNetSDK.ACS_CARD_NO_LEN];
            byte[] byTempCardNo = System.Text.Encoding.UTF8.GetBytes(textBoxCardNo.Text);
            for (int i = 0; i < byTempCardNo.Length; i++)
            {
                if (i > struCond.byCardNo.Length)
                {
                    MessageBox.Show("card number length too long!");
                    return;
                }
                struCond.byCardNo[i] = byTempCardNo[i];
            }

            int dwSize = Marshal.SizeOf(struCond);
            IntPtr ptrStruCond = Marshal.AllocHGlobal(dwSize);
            Marshal.StructureToPtr(struCond, ptrStruCond, false);
            g_fSetFaceParamCallback = new CHCNetSDK.RemoteConfigCallback(ProcessSetFaceParamCallback);
            m_lSetFaceCfgHandle = CHCNetSDK.NET_DVR_StartRemoteConfig(m_lUserID, CHCNetSDK.NET_DVR_SET_FACE_PARAM_CFG, ptrStruCond, dwSize, g_fSetFaceParamCallback, this.Handle);
            if (-1 == m_lSetFaceCfgHandle)
            {
                g_formList.AddLog(m_lDeviceIndex, AcsDemoPublic.OPERATION_FAIL_T, "NET_DVR_SET_FACE_PARAM_CFG");
            }
            else
            {
                g_formList.AddLog(m_lDeviceIndex, AcsDemoPublic.OPERATION_SUCC_T, "NET_DVR_SET_FACE_PARAM_CFG");
            }
            Marshal.FreeHGlobal(ptrStruCond);

            CHCNetSDK.NET_DVR_FACE_PARAM_CFG struFaceParamCfg = new CHCNetSDK.NET_DVR_FACE_PARAM_CFG();
            struFaceParamCfg.dwSize = (uint)Marshal.SizeOf(struFaceParamCfg);
            struFaceParamCfg.byFaceID = 1;
            struFaceParamCfg.byFaceDataType = 1;
            struFaceParamCfg.byEnableCardReader = new byte[CHCNetSDK.MAX_CARD_READER_NUM_512];
            struFaceParamCfg.byEnableCardReader[0] = 1;
            struFaceParamCfg.byCardNo = new byte[CHCNetSDK.ACS_CARD_NO_LEN];
            byte[] byTempFaceCardNo = System.Text.Encoding.UTF8.GetBytes(textBoxCardNo.Text);
            for (int i = 0; i < byTempFaceCardNo.Length; i++)
            {
                if (i > struFaceParamCfg.byCardNo.Length)
                {
                    MessageBox.Show("card number length too long!");
                    return;
                }
                struFaceParamCfg.byCardNo[i] = byTempFaceCardNo[i];
            }

            if (!File.Exists(textBoxFacePicturePath.Text))
            {
                MessageBox.Show("The face picture does not exist!");
                return;
            }
            FileStream fs = new FileStream(textBoxFacePicturePath.Text, FileMode.OpenOrCreate);
            if (0 == fs.Length)
            {
                MessageBox.Show("The face picture is 0k,please input another picture!");
                return;
            }
            if (200 * 1024 < fs.Length)
            {
                MessageBox.Show("The face picture is larger than 200k,please input another picture!");
                return;
            }

            struFaceParamCfg.dwFaceLen = (uint)fs.Length;
            int iLen = (int)struFaceParamCfg.dwFaceLen;
            byte[] by = new byte[iLen];
            struFaceParamCfg.pFaceBuffer = Marshal.AllocHGlobal(iLen);
            fs.Read(by, 0, iLen);
            Marshal.Copy(by, 0, struFaceParamCfg.pFaceBuffer, iLen);
            fs.Close();

            uint dwParamSize = (uint)Marshal.SizeOf(struFaceParamCfg);
            IntPtr ptrStruFaceParamCfg = Marshal.AllocHGlobal((int)dwParamSize);
            Marshal.StructureToPtr(struFaceParamCfg, ptrStruFaceParamCfg, false);

            if (!CHCNetSDK.NET_DVR_SendRemoteConfig(m_lSetFaceCfgHandle, 9, ptrStruFaceParamCfg, dwParamSize))
            {
                g_formList.AddLog(m_lDeviceIndex, AcsDemoPublic.OPERATION_FAIL_T, "set face picture failed!");
            }

            Marshal.FreeHGlobal(struFaceParamCfg.pFaceBuffer);
            Marshal.FreeHGlobal(ptrStruFaceParamCfg);

            return;
        }

        protected override void DefWndProc(ref Message m)
        {
            switch (m.Msg)
            {
                case 1001:
                    if (-1 != m_lSetFaceCfgHandle)
                    {
                        if (CHCNetSDK.NET_DVR_StopRemoteConfig(m_lSetFaceCfgHandle))
                        {
                            m_lSetFaceCfgHandle = -1;
                        }
                    }
                    break;
                case 1002:
                    if (-1 != m_lGetFaceCfgHandle)
                    {
                        if (CHCNetSDK.NET_DVR_StopRemoteConfig(m_lGetFaceCfgHandle))
                        {
                            m_lGetFaceCfgHandle = -1;
                        }
                    }
                    break;
                case 1003:
                    if (-1 != m_lGetFacePicHandle)
                    {
                        if (CHCNetSDK.NET_DVR_StopRemoteConfig(m_lGetFacePicHandle))
                        {
                            m_lGetFacePicHandle = -1;
                        }
                    }
                    break;
                default:
                    base.DefWndProc(ref m);
                    break;      
            }
        }

        private void FaceConfigure_Load(object sender, EventArgs e)
        {
            MultiLanguage.LoadLanguage(this);
        }

        private void btnGetFacePic_Click(object sender, EventArgs e)
        {
            if (-1 != m_lGetFacePicHandle)
            {
                if (CHCNetSDK.NET_DVR_StopRemoteConfig(m_lGetFacePicHandle))
                {
                    m_lGetFacePicHandle = -1;
                }
            }
            CHCNetSDK.NET_DVR_CAPTURE_FACE_COND struCond = new CHCNetSDK.NET_DVR_CAPTURE_FACE_COND();
            struCond.dwSize = (uint)Marshal.SizeOf(struCond);

            int dwSize = Marshal.SizeOf(struCond);
            IntPtr ptrStruCond = Marshal.AllocHGlobal(dwSize);
            Marshal.StructureToPtr(struCond, ptrStruCond, false);
            g_fGetFacePicCallback = new CHCNetSDK.RemoteConfigCallback(ProcessGetFacePicCallback);
            m_lGetFacePicHandle = CHCNetSDK.NET_DVR_StartRemoteConfig(m_lUserID, CHCNetSDK.NET_DVR_CAPTURE_FACE_INFO, ptrStruCond, dwSize, g_fGetFacePicCallback, this.Handle);
            if (-1 == m_lGetFacePicHandle)
            {
                g_formList.AddLog(m_lDeviceIndex, AcsDemoPublic.OPERATION_FAIL_T, "NET_DVR_CAPTURE_FACE_INFO");
            }
            else
            {
                g_formList.AddLog(m_lDeviceIndex, AcsDemoPublic.OPERATION_SUCC_T, "NET_DVR_CAPTURE_FACE_INFO");
            }
            Marshal.FreeHGlobal(ptrStruCond);
        }

        private void ProcessGetFacePicCallback(uint dwType, IntPtr lpBuffer, uint dwBufLen, IntPtr pUserData)
        {
            if (pUserData == null)
            {
                return;
            }

            if (dwType == (uint)CHCNetSDK.NET_SDK_CALLBACK_TYPE.NET_SDK_CALLBACK_TYPE_STATUS)
            {
                uint dwStatus = (uint)Marshal.ReadInt32(lpBuffer);

                if (dwStatus == (uint)CHCNetSDK.NET_SDK_CALLBACK_STATUS_NORMAL.NET_SDK_CALLBACK_STATUS_PROCESSING)
                {
                    g_formList.AddLog(m_lDeviceIndex, AcsDemoPublic.OPERATION_SUCC_T, "GetFacePicture Processing");
                }
                else if (dwStatus == (uint)CHCNetSDK.NET_SDK_CALLBACK_STATUS_NORMAL.NET_SDK_CALLBACK_STATUS_FAILED)
                {
                    g_formList.AddLog(m_lDeviceIndex, AcsDemoPublic.OPERATION_FAIL_T, "GetFacePicture Failed");
                    CHCNetSDK.PostMessage(pUserData, 1003, 0, 0);
                }
                else if (dwStatus == (uint)CHCNetSDK.NET_SDK_CALLBACK_STATUS_NORMAL.NET_SDK_CALLBACK_STATUS_SUCCESS)
                {
                    g_formList.AddLog(m_lDeviceIndex, AcsDemoPublic.OPERATION_SUCC_T, "GetFacePicture Success");
                    CHCNetSDK.PostMessage(pUserData, 1003, 0, 0);
                }
                else if (dwStatus == (uint)CHCNetSDK.NET_SDK_CALLBACK_STATUS_NORMAL.NET_SDK_CALLBACK_STATUS_EXCEPTION)
                {
                    g_formList.AddLog(m_lDeviceIndex, AcsDemoPublic.OPERATION_FAIL_T, "GetFaceParam Exception");
                    CHCNetSDK.PostMessage(pUserData, 1003, 0, 0);
                }
                else
                {
                    g_formList.AddLog(m_lDeviceIndex, AcsDemoPublic.OPERATION_FAIL_T, "Unknown Status");
                    CHCNetSDK.PostMessage(pUserData, 1003, 0, 0);
                }
            }
            else if (dwType == (uint)CHCNetSDK.NET_SDK_CALLBACK_TYPE.NET_SDK_CALLBACK_TYPE_DATA)
            {
                var result = Marshal.PtrToStructure(lpBuffer, typeof(CHCNetSDK.NET_DVR_CAPTURE_FACE_CFG));
                var struFaceParamCfg = (CHCNetSDK.NET_DVR_CAPTURE_FACE_CFG)result;

                if (struFaceParamCfg.dwFacePicSize != 0)
                {

                    FileStream fs = new FileStream(@"d:\test_picture.jpg", FileMode.Create);
                    int iLen = (int)struFaceParamCfg.dwFacePicSize;
                    byte[] by = new byte[iLen];
                    Marshal.Copy(struFaceParamCfg.pFacePicBuffer, by, 0, iLen);

                    fs.Write(by, 0, iLen);

                    fs.Close();
                }
            }
            return;
        }

    }
}
