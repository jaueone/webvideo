using AcsDemo.Public;
using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;
using AcsDemo.Language;
namespace AcsDemo.ProductAcs
{
    public partial class SynInfo : Form
    {
        public Int32 m_lInfoSynHandle = -1;
        public SynInfoCommand m_command = SynInfoCommand.NO_DEFINE;
        public Int32 m_lUserID = -1;
        private int m_iCurProgress = 0;
        private bool m_bStart = false;
        private bool m_bGetResult = false;
        private CHCNetSDK.RemoteConfigCallback g_fInfoSynCallback = null;
        private bool m_bFirst = true;
        private const uint XML_OUT_LEN = 3 * 1024 * 1024;
        private delegate void AddListCallBack(ListView cListView, ListViewItem cListViewItem);

        private void AddList(ListView cListView, ListViewItem cListViewItem)
        {
            if (this.InvokeRequired)
            {
                AddListCallBack add = new AddListCallBack(AddList);
                this.Invoke(add, new object[] { cListView, cListViewItem });
            }
            else
            {
                cListView.Items.Insert(0, cListViewItem);
            }
        }

        public class CResponseStatus
        {
            public string requestURL { get; set; }
            public int statusCode { get; set; }
            public string statusString { get; set; }
            public string subStatusCode { get; set; }
            public int errorCode { get; set; }
            public string errorMsg { get; set; }
        }

        public class CCardInfoSynCond
        {
            public CCardInfoSynCondContent CardInfoSyncCond { get; set; }
        }

        public class CCardInfoSynCondContent
        {
            public string command { get; set; }
            public int maxResults { get; set; }
        }

        public class CCardInfoSyn
        {
            public CCardInfoSynContent CardInfoSync { get; set; }
        }

        public class CCardInfoSynContent
        {
            public int progressValue { get; set; }
            public List<CCardInfoSynList> SyncInfo { get; set; }
        }
        public class CCardInfoSynList
        {
            public int doorNo { get; set; }
            public string cardNo { get; set; }
            public string errorCode { get; set; }
        }

        public class CUserInfoSynCond
        {
            public CUserInfoSynCondContent UserInfoSyncCond { get; set; }
        }

        public class CUserInfoSynCondContent
        {
            public string command { get; set; }
            public int maxResults { get; set; }
        }

        public class CUserInfoSyn
        {
            public CUserInfoSynContent UserInfoSync { get; set; }
        }

        public class CUserInfoSynContent
        {
            public int progressValue { get; set; }
            public List<CUserInfoSynList> SyncInfo { get; set; }
        }
        public class CUserInfoSynList
        {
            public int doorNo { get; set; }
            public int employeeNo { get; set; }
            public string errorCode { get; set; }
        }

        public SynInfo()
        {
            InitializeComponent();
            this.SuspendLayout();
            this.columnHeader1.Name = "columnHeader1";
            this.columnHeader2.Name = "columnHeader2";
            this.ResumeLayout();
        }

        private string GetRoute(SynInfoCommand command)
        {
            string tmp = null;
            switch (command)
            {
                case SynInfoCommand.NO_DEFINE:
                    break;
                case SynInfoCommand.USER_INFO_SYN:
                    tmp = "POST /ISAPI/SmartLock/UserInfo/Sync?format=json";
                    break;
                case SynInfoCommand.CARD_INFO_SYN:
                    tmp = "POST /ISAPI/SmartLock/CardInfo/Sync?format=json";
                    break;
                default:
                    break;
            }

            return tmp;
        }

        private void ProcessInfoSynCallback(uint dwType, IntPtr lpBuffer, uint dwBufLen, IntPtr pUserData)
        {
            if (pUserData == null)
            {
                return;
            }

            if (dwType == (uint)CHCNetSDK.NET_SDK_CALLBACK_TYPE.NET_SDK_CALLBACK_TYPE_STATUS)
            {
                uint dwStatus = (uint)Marshal.ReadInt32(lpBuffer);
                if (dwStatus == (uint)CHCNetSDK.NET_SDK_CALLBACK_STATUS_NORMAL.NET_SDK_CALLBACK_STATUS_FAILED)
                {
                    CHCNetSDK.PostMessage(pUserData, 1001, 1, 0);
                    //可以结束了，异常结束
                }
                else if (dwStatus == (uint)CHCNetSDK.NET_SDK_CALLBACK_STATUS_NORMAL.NET_SDK_CALLBACK_STATUS_EXCEPTION)
                {
                    CHCNetSDK.PostMessage(pUserData, 1001, 2, 0);
                    //可以结束了，异常结束
                }
                else if (dwStatus == (uint)CHCNetSDK.NET_SDK_CALLBACK_STATUS_NORMAL.NET_SDK_CALLBACK_STATUS_PROCESSING)
                {
                    ;
                }
                else
                {
                    CHCNetSDK.PostMessage(pUserData, 1001, 3, 0);
                    //可以结束了，异常结束
                }
            }
            else if (dwType == (uint)CHCNetSDK.NET_SDK_CALLBACK_TYPE.NET_SDK_CALLBACK_TYPE_DATA)
            {
                //输出上一包解析信息, 1003
                string strResponseStatus = Marshal.PtrToStringAnsi(lpBuffer);
                IntPtr response = IntPtr.Zero;
                
                if (m_bFirst)
                {
                    m_bFirst = false;
                    CResponseStatus JsonResponseStatus = new CResponseStatus();
                    JsonResponseStatus = JsonConvert.DeserializeObject<CResponseStatus>(strResponseStatus);
                    response = Marshal.StringToHGlobalAnsi(strResponseStatus);
                }
                else
                {
                    if (m_command == SynInfoCommand.USER_INFO_SYN)
                    {
                        CUserInfoSyn JsonResponseStatus = new CUserInfoSyn();
                        JsonResponseStatus = JsonConvert.DeserializeObject<CUserInfoSyn>(strResponseStatus);
                        m_iCurProgress = JsonResponseStatus.UserInfoSync.progressValue;
                        response = Marshal.StringToHGlobalAnsi(strResponseStatus);
                    }
                    else
                    {
                        CCardInfoSyn JsonResponseStatus = new CCardInfoSyn();
                        JsonResponseStatus = JsonConvert.DeserializeObject<CCardInfoSyn>(strResponseStatus);
                        m_iCurProgress = JsonResponseStatus.CardInfoSync.progressValue;
                        response = Marshal.StringToHGlobalAnsi(strResponseStatus);
                    }
                }

                CHCNetSDK.PostMessage(pUserData, 1003, (int)response, 0);

                if (m_iCurProgress >= 100)
                {
                    CHCNetSDK.PostMessage(pUserData, 1001, 4, 0);
                    //可以结束了，正常(异常)结束
                    return;
                }
                CHCNetSDK.PostMessage(pUserData, 1002, 0, 0);
                //在主线程中发送下一个用户
            }
            return;
        }

        private delegate void ProgressBarShow(int i);
        private void ShowPro(int value)
        {
            if (this.InvokeRequired)
            {
                this.Invoke(new ProgressBarShow(ShowPro), value);
            }
            else
            {
                if (value < 100)
                {
                    this.progressBarSynProgress.Value = value;
                    this.labelSynProgress.Text = value + "% Processing...";
                }
                else
                {
                    this.progressBarSynProgress.Value = value;
                    this.labelSynProgress.Text = value + "% Succeed";
                }

            }
        }

        private void GetSynResult()
        {
            int i = 0;
            while (i < 100)
            {
                //获取进度
                if (!PostSynProgress_ISAPI())
                {
                    break;
                }

                i = (int)m_iCurProgress;

                //显示进度信息
                this.ShowPro(i);

                //0.2s
                Thread.Sleep(200);

                //等待获取结果
                while (!m_bGetResult)
                {
                    Thread.Sleep(10);
                }
            }

            m_bStart = false;
            Thread.CurrentThread.Abort();
        }

        private bool PostSynProgress_ISAPI()
        {
            m_bGetResult = false;
            string strInfoSyn = null;
            if (m_command == SynInfoCommand.USER_INFO_SYN)
            {
                CUserInfoSynCond JsonUserInfoSynCond = new CUserInfoSynCond();
                JsonUserInfoSynCond.UserInfoSyncCond = new CUserInfoSynCondContent();
                JsonUserInfoSynCond.UserInfoSyncCond.command = "getProgress";
                JsonUserInfoSynCond.UserInfoSyncCond.maxResults = 30;

                strInfoSyn = JsonConvert.SerializeObject(JsonUserInfoSynCond);
            }
            else
            {
                CCardInfoSynCond JsonCardInfoSynCond = new CCardInfoSynCond();
                JsonCardInfoSynCond.CardInfoSyncCond = new CCardInfoSynCondContent();
                JsonCardInfoSynCond.CardInfoSyncCond.command = "getProgress";
                JsonCardInfoSynCond.CardInfoSyncCond.maxResults = 30;

                strInfoSyn = JsonConvert.SerializeObject(JsonCardInfoSynCond);
            }

            IntPtr ptrInfoSyn = Marshal.StringToHGlobalAnsi(strInfoSyn);

            if (!CHCNetSDK.NET_DVR_SendRemoteConfig(m_lInfoSynHandle, (int)CHCNetSDK.LONG_CFG_SEND_DATA_TYPE_ENUM.ENUM_SEND_JSON_DATA, ptrInfoSyn, (uint)strInfoSyn.Length))
            {
                Marshal.FreeHGlobal(ptrInfoSyn);
                return false;
            }
            Marshal.FreeHGlobal(ptrInfoSyn);
            return true;
        }

        private void buttonStart_Click(object sender, EventArgs e)
        {
            if (-1 != m_lInfoSynHandle)
            {
                if (CHCNetSDK.NET_DVR_StopRemoteConfig(m_lInfoSynHandle))
                {
                    m_lInfoSynHandle = -1;
                }
            }

            if (m_bStart) return;

            m_bStart = true;
            g_fInfoSynCallback = new CHCNetSDK.RemoteConfigCallback(ProcessInfoSynCallback);
            string sURL = GetRoute(m_command);
            if (sURL == null)
            {
                return;
            }

            IntPtr ptrURL = Marshal.StringToHGlobalAnsi(sURL);

            m_lInfoSynHandle = CHCNetSDK.NET_DVR_StartRemoteConfig(m_lUserID, CHCNetSDK.NET_DVR_JSON_CONFIG, ptrURL, sURL.Length, g_fInfoSynCallback, this.Handle);

            string strInfoSyn = null;
            if (m_command == SynInfoCommand.USER_INFO_SYN)
            {
                CUserInfoSynCond JsonUserInfoSynCond = new CUserInfoSynCond();
                JsonUserInfoSynCond.UserInfoSyncCond = new CUserInfoSynCondContent();
                JsonUserInfoSynCond.UserInfoSyncCond.command = "startSync";
                JsonUserInfoSynCond.UserInfoSyncCond.maxResults = 30;

                strInfoSyn = JsonConvert.SerializeObject(JsonUserInfoSynCond);
            }
            else
            {
                CCardInfoSynCond JsonCardInfoSynCond = new CCardInfoSynCond();
                JsonCardInfoSynCond.CardInfoSyncCond = new CCardInfoSynCondContent();
                JsonCardInfoSynCond.CardInfoSyncCond.command = "startSync";
                JsonCardInfoSynCond.CardInfoSyncCond.maxResults = 30;

                strInfoSyn = JsonConvert.SerializeObject(JsonCardInfoSynCond);
            }

            IntPtr ptrInfoSyn = Marshal.StringToHGlobalAnsi(strInfoSyn);

            if (!CHCNetSDK.NET_DVR_SendRemoteConfig(m_lInfoSynHandle, (int)CHCNetSDK.LONG_CFG_SEND_DATA_TYPE_ENUM.ENUM_SEND_JSON_DATA, ptrInfoSyn, (uint)strInfoSyn.Length))
            {
                Marshal.FreeHGlobal(ptrInfoSyn);
                return;
            }
            Marshal.FreeHGlobal(ptrInfoSyn);

            //获取结果
            Thread thread = new Thread(new ThreadStart(GetSynResult)); 
            thread.IsBackground = true;
            thread.Start();
        }
        protected override void DefWndProc(ref Message m)
        {
            switch (m.Msg)
            {
                case 1001:
                    int iErrorMsg = m.WParam.ToInt32();
                    if (iErrorMsg == 1)
                    {
                        ListViewItem listItem = new ListViewItem();
                        listItem.Text = "Fail";
                        string strTemp = string.Format("NET_SDK_CALLBACK_STATUS_FAILED");
                        listItem.SubItems.Add(strTemp);
                        this.AddList(listViewResponse, listItem);
                    }
                    else if (iErrorMsg == 2)
                    {
                        ListViewItem listItem = new ListViewItem();
                        listItem.Text = "Fail";
                        string strTemp = string.Format("NET_SDK_CALLBACK_STATUS_EXCEPTION");
                        listItem.SubItems.Add(strTemp);
                        this.AddList(listViewResponse, listItem);
                    }
                    else if (iErrorMsg == 3)
                    {
                        ListViewItem listItem = new ListViewItem();
                        listItem.Text = "Fail";
                        string strTemp = string.Format("NET_SDK_CALLBACK_OTHER_ERROR");
                        listItem.SubItems.Add(strTemp);
                        this.AddList(listViewResponse, listItem);
                    }
                    else if (iErrorMsg == 4)
                    {
                        ListViewItem listItem = new ListViewItem();
                        listItem.Text = "Succ";
                        string strTemp = string.Format("NET_SDK_CALLBACK_FINISH");
                        listItem.SubItems.Add(strTemp);
                        this.AddList(listViewResponse, listItem);
                    }
                    m_bStart = false;
                    m_bFirst = true;
                    if (-1 != m_lInfoSynHandle)
                    {
                        if (CHCNetSDK.NET_DVR_StopRemoteConfig(m_lInfoSynHandle))
                        {
                            m_lInfoSynHandle = -1;
                        }
                    }
                    break;
                case 1002:
                    m_bGetResult = true;
                    break;
                case 1003:
                    IntPtr ptrResponse = (IntPtr)m.WParam.ToInt32();
                    string strResponseStatus = Marshal.PtrToStringAnsi(ptrResponse);

                    string strResponse = null;

                    if (m_bFirst)
                    {
                        m_bFirst = false;
                        CResponseStatus JsonResponseStatus = new CResponseStatus();
                        strResponse = strResponseStatus;
                        m_iCurProgress = 0;
                    }
                    else
                    {
                        if (m_command == SynInfoCommand.USER_INFO_SYN)
                        {
                            CUserInfoSyn JsonResponseStatus = new CUserInfoSyn();
                            JsonResponseStatus.UserInfoSync = new CUserInfoSynContent();
                            if (JsonResponseStatus.UserInfoSync != null)
                            {
                                strResponse = JsonConvert.SerializeObject(JsonResponseStatus.UserInfoSync.SyncInfo);
                                m_iCurProgress = JsonResponseStatus.UserInfoSync.progressValue;
                            }
                            else
                            {
                                strResponse = strResponseStatus;
                            }
                        }
                        else
                        {
                            CCardInfoSyn JsonResponseStatus = new CCardInfoSyn();
                            JsonResponseStatus = JsonConvert.DeserializeObject<CCardInfoSyn>(strResponseStatus);
                            if (JsonResponseStatus.CardInfoSync != null)
                            {
                                strResponse = JsonConvert.SerializeObject(JsonResponseStatus.CardInfoSync.SyncInfo);
                                m_iCurProgress = JsonResponseStatus.CardInfoSync.progressValue;
                            }
                            else 
                            {
                                strResponse = strResponseStatus;
                            }
                        }
                    }

                    ListViewItem lvItem = new ListViewItem();
                    string strTmp = null;
                    lvItem.Text = "Succ";
                    strTmp = string.Format("Percent:{0}-{1}", m_iCurProgress, strResponse);
                    lvItem.SubItems.Add(strTmp);
                    this.AddList(listViewResponse, lvItem);
                    Marshal.FreeHGlobal(ptrResponse);
                    break;
                default:
                    base.DefWndProc(ref m);
                    break;
            }
        }

        private void SynInfo_Load(object sender, EventArgs e)
        {
            MultiLanguage.LoadLanguage(this);
        }
    }
}
