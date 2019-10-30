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
using System.Diagnostics;
using System.IO;
using System.Threading;
using Newtonsoft.Json;
using Newtonsoft.Json.Linq;
using AcsDemo.Language;
namespace AcsDemo.ProductAcs
{
    public partial class CardManagementByStaffCenter : Form
    {
        public Int32 m_lUserID = -1;
        public int m_iCardCount = 0;
        public int m_iCardInfoRecordIndex = 0;
        public Int32 m_lCardInfoRecordHandle = -1;
        public Int32 m_lCardInfoSearchHandle = -1;
        public int m_iSearchType = 0;   //0-全部，1-人员ID，2-卡号
        public int m_iDeleteType = 0;   //0-全部，1-人员ID，2-卡号
        public int m_iSearchPosition = 0;

        private delegate void AddListCallBack(ListView cListView, ListViewItem cListViewItem);
        private CHCNetSDK.RemoteConfigCallback g_fCardInfoRecordCallback = null;
        private CHCNetSDK.RemoteConfigCallback g_fCardInfoModifyCallback = null;
        private CHCNetSDK.RemoteConfigCallback g_fCardInfoSearchCallback = null;

        CCardInfoSearch m_JsonCardInfoSearch;

        public class CResponseStatus
        {
            public string requestURL { get; set; }
            public int statusCode { get; set; }
            public string statusString { get; set; }
            public string subStatusCode { get; set; }
            public int errorCode { get; set; }
            public string errorMsg { get; set; }
        }

        public class CCardInfoRecord
        {
            public CCardInfoContent CardInfo { get; set; }
        }

        public class CCardInfoContent
        {
            public string employeeNo { get; set; }
            public string cardNo { get; set; }
            public string cardType { get; set; }
            public string leaderCard { get; set; }
        }

        public class CCardInfoSearch
        {
            public CCardInfoSearchContent CardInfoSearch { get; set; }
        }

        public class CCardInfoSearchContent
        {
            public string searchID { get; set; }
            public string responseStatusStrg { get; set; }
            public int numOfMatches { get; set; }
            public int totalMatches { get; set; }
            public List<CCardInfoContent> CardInfo { get; set; }
        }

        public class CCardInfoSearchCond
        {
            public CCardInfoSearchCondContent CardInfoSearchCond { get; set; }
        }

        public class CCardInfoSearchCondContent
        {
            public string searchID { get; set; }
            public int searchResultPosition { get; set; }
            public int maxResults { get; set; }
            public List<CEmployeeNoList> EmployeeNoList { get; set; }
            public List<CCardNoList> CardNoList { get; set; }
        }

        public class CEmployeeNoList
        {
            public string employeeNo { get; set; }
        }

        public class CCardNoList
        {
            public string cardNo { get; set; }
        }

        public class CCardInfoDelCond
        {
            public CCardInfoDelCondContent CardInfoDelCond { get; set; }
        }

        public class CCardInfoDelCondContent
        {
            public List<CEmployeeNoList> EmployeeNoList { get; set; }
            public List<CCardNoList> CardNoList { get; set; }
        }

        public class CCardInfoCount
        {
            public CCardInfoCountContent CardInfoCount { get; set; }
        }

        public class CCardInfoCountContent
        {
            public int cardNumber { get; set; }
        }

        public CardManagementByStaffCenter()
        {
            InitializeComponent();
            comboBoxCardType.SelectedIndex = 0;
            UpdateListEmployeeNo();
            UpdateListCardNo();
            this.SuspendLayout();
            this.columnHeader1.Name = "columnHeader1";
            this.columnHeader2.Name = "columnHeader2";
            this.columnHeader3.Name = "columnHeader3";
            this.columnHeader4.Name = "columnHeader4";
            this.columnHeader5.Name = "columnHeader5";
            this.columnHeader16.Name = "columnHeader16";
            this.columnHeader17.Name = "columnHeader17";
            this.columnHeader6.Name = "columnHeader6";
            this.columnHeader7.Name = "columnHeader7";
            this.columnHeader21.Name = "columnHeader21";
            this.columnHeader22.Name = "columnHeader22";
            this.ResumeLayout();
        }

        private void ProcessCardInfoRecordCallback(uint dwType, IntPtr lpBuffer, uint dwBufLen, IntPtr pUserData)
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
                CResponseStatus JsonResponseStatus = new CResponseStatus();
                JsonResponseStatus = JsonConvert.DeserializeObject<CResponseStatus>(strResponseStatus);

                CHCNetSDK.PostMessage(pUserData, 1003, JsonResponseStatus.statusCode, 0);

                if (m_iCardInfoRecordIndex >= m_iCardCount)
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

        private void ProcessCardInfoSearchCallback(uint dwType, IntPtr lpBuffer, uint dwBufLen, IntPtr pUserData)
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
                    CHCNetSDK.PostMessage(pUserData, 1004, 1, 0);
                    //可以结束了，异常结束
                }
                else if (dwStatus == (uint)CHCNetSDK.NET_SDK_CALLBACK_STATUS_NORMAL.NET_SDK_CALLBACK_STATUS_EXCEPTION)
                {
                    CHCNetSDK.PostMessage(pUserData, 1004, 2, 0);
                    //可以结束了，异常结束
                }
                else if (dwStatus == (uint)CHCNetSDK.NET_SDK_CALLBACK_STATUS_NORMAL.NET_SDK_CALLBACK_STATUS_PROCESSING)
                {
                    ;
                }
                else
                {
                    CHCNetSDK.PostMessage(pUserData, 1004, 3, 0);
                    //可以结束了，异常结束
                }
            }
            else if (dwType == (uint)CHCNetSDK.NET_SDK_CALLBACK_TYPE.NET_SDK_CALLBACK_TYPE_DATA)
            {
                //输出上一包解析信息, 1006
                string strCardInfoSearch = Marshal.PtrToStringAnsi(lpBuffer);
                m_JsonCardInfoSearch = new CCardInfoSearch();
                m_JsonCardInfoSearch = JsonConvert.DeserializeObject<CCardInfoSearch>(strCardInfoSearch);
                if (m_JsonCardInfoSearch.CardInfoSearch == null)
                {
                    //此处说明返回了Response报文了
                    CHCNetSDK.PostMessage(pUserData, 1004, 3, 0);
                    return;
                }

                if (m_JsonCardInfoSearch.CardInfoSearch.responseStatusStrg != "NO MATCH")
                {
                    CHCNetSDK.PostMessage(pUserData, 1006, 0, 0);
                }
                //根据上一包responseStatusStrg的结果，判定是结束还是继续查询

                if (m_JsonCardInfoSearch.CardInfoSearch.responseStatusStrg != "MORE")
                {
                    CHCNetSDK.PostMessage(pUserData, 1004, 4, 0);
                    //可以结束了，正常（异常）结束
                    return;
                }

                m_iSearchPosition += m_JsonCardInfoSearch.CardInfoSearch.numOfMatches;
                CHCNetSDK.PostMessage(pUserData, 1005, 0, 0);
                //在主线程中发送下一个用户
            }
            return;
        }

        private void UpdateListEmployeeNo()
        {
            listViewEmployeeNo.BeginUpdate();
            listViewEmployeeNo.Items.Clear();
            for (int i = 0; i < 32; i++)
            {
                ListViewItem listItem = new ListViewItem();
                listItem.Text = (i + 1).ToString();
                if (i <= 0)
                {
                    listItem.SubItems.Add("1");
                }
                else
                {
                    listItem.SubItems.Add("");
                }
                listViewEmployeeNo.Items.Add(listItem);
            }
            listViewEmployeeNo.EndUpdate();
        }

        private void UpdateListCardNo()
        {
            listViewCardNo.BeginUpdate();
            listViewCardNo.Items.Clear();
            for (int i = 0; i < 32; i++)
            {
                ListViewItem listItem = new ListViewItem();
                listItem.Text = (i + 1).ToString();
                if (i <= 0)
                {
                    listItem.SubItems.Add("111");
                }
                else
                {
                    listItem.SubItems.Add("");
                }
                listViewCardNo.Items.Add(listItem);
            }
            listViewCardNo.EndUpdate();
        }

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

        private bool SendCardInfo()
        {
            if (-1 == m_lCardInfoRecordHandle)
            {
                return false;
            }

            if (m_iCardInfoRecordIndex >= m_iCardCount)
            {
                return false;
            }

            CCardInfoRecord JsonCardInfoRecord = new CCardInfoRecord();
            JsonCardInfoRecord.CardInfo = new CCardInfoContent();

            JsonCardInfoRecord.CardInfo.employeeNo = listViewCardManage.Items[m_iCardInfoRecordIndex].SubItems[1].Text;
            JsonCardInfoRecord.CardInfo.cardNo = listViewCardManage.Items[m_iCardInfoRecordIndex].SubItems[2].Text;
            JsonCardInfoRecord.CardInfo.cardType = listViewCardManage.Items[m_iCardInfoRecordIndex].SubItems[3].Text;
            JsonCardInfoRecord.CardInfo.leaderCard = listViewCardManage.Items[m_iCardInfoRecordIndex].SubItems[4].Text;

            string strCardInfoRecord = JsonConvert.SerializeObject(JsonCardInfoRecord);

            IntPtr ptrCardInfoRecord = Marshal.StringToHGlobalAnsi(strCardInfoRecord);

            if (!CHCNetSDK.NET_DVR_SendRemoteConfig(m_lCardInfoRecordHandle, (int)CHCNetSDK.LONG_CFG_SEND_DATA_TYPE_ENUM.ENUM_SEND_JSON_DATA, ptrCardInfoRecord, (uint)strCardInfoRecord.Length))
            {
                ListViewItem listItem = new ListViewItem();
                listItem.Text = "Fail";
                string strTemp = null;
                strTemp = string.Format("Send Fail,CardNo:{0}", listViewCardManage.Items[m_iCardInfoRecordIndex].SubItems[2].Text);
                listItem.SubItems.Add(strTemp);
                this.AddList(listViewMessage, listItem);
                Marshal.FreeHGlobal(ptrCardInfoRecord);
                return false;
            }
            ListViewItem listItemSucc = new ListViewItem();
            listItemSucc.Text = "Succ";
            string strTempSucc = null;
            strTempSucc = string.Format("Send Processing,CardNo:{0}", listViewCardManage.Items[m_iCardInfoRecordIndex].SubItems[2].Text);
            listItemSucc.SubItems.Add(strTempSucc);
            this.AddList(listViewMessage, listItemSucc);
            Marshal.FreeHGlobal(ptrCardInfoRecord);
            m_iCardInfoRecordIndex++;
            return true;
        }

        private bool SearchCardInfo()
        {
            if (-1 == m_lCardInfoSearchHandle)
            {
                return false;
            }

            CCardInfoSearchCond JsonCardInfoSearchCond = new CCardInfoSearchCond();
            JsonCardInfoSearchCond.CardInfoSearchCond = new CCardInfoSearchCondContent();
            JsonCardInfoSearchCond.CardInfoSearchCond.EmployeeNoList = new List<CEmployeeNoList>();
            JsonCardInfoSearchCond.CardInfoSearchCond.CardNoList = new List<CCardNoList>();

            JsonCardInfoSearchCond.CardInfoSearchCond.searchID = "1";
            JsonCardInfoSearchCond.CardInfoSearchCond.searchResultPosition = m_iSearchPosition;
            JsonCardInfoSearchCond.CardInfoSearchCond.maxResults = 10;

            if (m_iSearchType == 1)
            {
                foreach (ListViewItem item in this.listViewEmployeeNo.Items)
                {
                    if (item.SubItems[1].Text != "")
                    {
                        CEmployeeNoList singleEmployeeNoList = new CEmployeeNoList();
                        singleEmployeeNoList.employeeNo = item.SubItems[1].Text;
                        JsonCardInfoSearchCond.CardInfoSearchCond.EmployeeNoList.Add(singleEmployeeNoList);
                    }
                }
            }
            else if (m_iSearchType == 2)
            {
                foreach (ListViewItem item in this.listViewCardNo.Items)
                {
                    if (item.SubItems[1].Text != "")
                    {
                        CCardNoList singleCardNoList = new CCardNoList();
                        singleCardNoList.cardNo = item.SubItems[1].Text;
                        JsonCardInfoSearchCond.CardInfoSearchCond.CardNoList.Add(singleCardNoList);
                    }
                }
            }

            string strCardInfoSearchCond = JsonConvert.SerializeObject(JsonCardInfoSearchCond);

            IntPtr ptrCardInfoSearchCond = Marshal.StringToHGlobalAnsi(strCardInfoSearchCond);

            if (!CHCNetSDK.NET_DVR_SendRemoteConfig(m_lCardInfoSearchHandle, (int)CHCNetSDK.LONG_CFG_SEND_DATA_TYPE_ENUM.ENUM_SEND_JSON_DATA, ptrCardInfoSearchCond, (uint)strCardInfoSearchCond.Length))
            {
                ListViewItem listItem = new ListViewItem();
                listItem.Text = "Fail";
                string strTemp = null;
                strTemp = string.Format("Search Fail");
                listItem.SubItems.Add(strTemp);
                this.AddList(listViewMessage, listItem);
                Marshal.FreeHGlobal(ptrCardInfoSearchCond);
                return false;
            }
            ListViewItem listItemSucc = new ListViewItem();
            listItemSucc.Text = "Succ";
            string strTempSucc = null;
            strTempSucc = string.Format("Search Processing");
            listItemSucc.SubItems.Add(strTempSucc);
            this.AddList(listViewMessage, listItemSucc);
            Marshal.FreeHGlobal(ptrCardInfoSearchCond);
            return true;
        }

        private void DeleteCardInfo()
        {
            IntPtr ptrOutBuf = Marshal.AllocHGlobal(1024);
            IntPtr ptrStatusBuffer = Marshal.AllocHGlobal(1024);

            for (int i = 0; i < 1024; i++)
            {
                Marshal.WriteByte(ptrOutBuf, i, 0);
                Marshal.WriteByte(ptrStatusBuffer, i, 0);
            }

            CHCNetSDK.NET_DVR_XML_CONFIG_INPUT struInput = new CHCNetSDK.NET_DVR_XML_CONFIG_INPUT();
            CHCNetSDK.NET_DVR_XML_CONFIG_OUTPUT struOuput = new CHCNetSDK.NET_DVR_XML_CONFIG_OUTPUT();

            string sUrl = "PUT /ISAPI/AccessControl/CardInfo/Delete?format=json";
            IntPtr ptrURL = Marshal.StringToHGlobalAnsi(sUrl);
            struInput.dwSize = (uint)Marshal.SizeOf(struInput);
            struInput.lpRequestUrl = ptrURL;
            struInput.dwRequestUrlLen = (uint)sUrl.Length;
            struInput.dwRecvTimeOut = 60000;

            CCardInfoDelCond JsonCardInfoDelCond = new CCardInfoDelCond();
            JsonCardInfoDelCond.CardInfoDelCond = new CCardInfoDelCondContent();
            JsonCardInfoDelCond.CardInfoDelCond.EmployeeNoList = new List<CEmployeeNoList>();
            JsonCardInfoDelCond.CardInfoDelCond.CardNoList = new List<CCardNoList>();

            if (m_iDeleteType == 1)
            {
                foreach (ListViewItem item in this.listViewEmployeeNo.Items)
                {
                    if (item.SubItems[1].Text != "")
                    {
                        CEmployeeNoList singleEmployeeNoList = new CEmployeeNoList();
                        singleEmployeeNoList.employeeNo = item.SubItems[1].Text;
                        JsonCardInfoDelCond.CardInfoDelCond.EmployeeNoList.Add(singleEmployeeNoList);
                    }
                }
            }
            else if (m_iDeleteType == 2)
            {
                foreach (ListViewItem item in this.listViewCardNo.Items)
                {
                    if (item.SubItems[1].Text != "")
                    {
                        CCardNoList singleCardNoList = new CCardNoList();
                        singleCardNoList.cardNo = item.SubItems[1].Text;
                        JsonCardInfoDelCond.CardInfoDelCond.CardNoList.Add(singleCardNoList);
                    }
                }
            }

            string strCardInfoDelCond = JsonConvert.SerializeObject(JsonCardInfoDelCond);

            IntPtr ptrInBuffer = Marshal.StringToHGlobalAnsi(strCardInfoDelCond);

            struInput.lpInBuffer = ptrInBuffer;
            struInput.dwInBufferSize = (uint)strCardInfoDelCond.Length;

            struOuput.dwSize = (uint)Marshal.SizeOf(struOuput);
            struOuput.lpOutBuffer = ptrOutBuf;
            struOuput.dwOutBufferSize = 1024;
            struOuput.lpStatusBuffer = ptrStatusBuffer;
            struOuput.dwStatusSize = 1024;

            IntPtr ptrInput = Marshal.AllocHGlobal(Marshal.SizeOf(struInput));
            Marshal.StructureToPtr(struInput, ptrInput, false);
            IntPtr ptrOuput = Marshal.AllocHGlobal(Marshal.SizeOf(struOuput));
            Marshal.StructureToPtr(struOuput, ptrOuput, false);
            if (!CHCNetSDK.NET_DVR_STDXMLConfig(m_lUserID, ptrInput, ptrOuput))
            {
                ListViewItem listItem = new ListViewItem();
                listItem.Text = "Fail";
                string strTemp = string.Format("Delete Card Info Fail,Error Code={0}", CHCNetSDK.NET_DVR_GetLastError());
                listItem.SubItems.Add(strTemp);
                this.AddList(listViewMessage, listItem);
            }
            else
            {
                ListViewItem listItem = new ListViewItem();
                listItem.Text = "Succ";
                string strTemp = string.Format("Delete Card Info Success");
                listItem.SubItems.Add(strTemp);
                this.AddList(listViewMessage, listItem);

                string strResponseStatus = Marshal.PtrToStringAnsi(struOuput.lpOutBuffer);
                CResponseStatus JsonResponseStatus = new CResponseStatus();
                JsonResponseStatus = JsonConvert.DeserializeObject<CResponseStatus>(strResponseStatus);
            }

            Marshal.FreeHGlobal(ptrOutBuf);
            Marshal.FreeHGlobal(ptrStatusBuffer);
            Marshal.FreeHGlobal(ptrInBuffer);
            Marshal.FreeHGlobal(ptrInput);
            Marshal.FreeHGlobal(ptrOuput);
            Marshal.FreeHGlobal(ptrURL);
        }

        private void buttonAdd_Click(object sender, EventArgs e)
        {
            int iAddCardNumber = 0;

            if (!int.TryParse(textBoxAddCardNumber.Text, out iAddCardNumber))
            {
                MessageBox.Show("add card number should be the number!");
                return;
            }

            for (int i = 0; i < iAddCardNumber; i++)
            {
                listViewCardManage.BeginUpdate();

                ListViewItem listItem = new ListViewItem();
                listItem.Text = (m_iCardCount + 1).ToString();
                if (iAddCardNumber > 1)
                {
                    listItem.SubItems.Add(textBoxEmployeeNo.Text + i);
                    listItem.SubItems.Add(textBoxCardNo.Text + i);
                }
                else
                {
                    listItem.SubItems.Add(textBoxEmployeeNo.Text);
                    listItem.SubItems.Add(textBoxCardNo.Text);
                }
                listItem.SubItems.Add(comboBoxCardType.Text);
                listItem.SubItems.Add(textBoxLeaderCard.Text);

                listViewCardManage.Items.Add(listItem);
                listViewCardManage.EndUpdate();

                m_iCardCount += 1;
            }
        }

        //add card
        private void buttonSend_Click(object sender, EventArgs e)
        {
            m_iCardInfoRecordIndex = 0;
            if (-1 != m_lCardInfoRecordHandle)
            {
                if (CHCNetSDK.NET_DVR_StopRemoteConfig(m_lCardInfoRecordHandle))
                {
                    m_lCardInfoRecordHandle = -1;
                }
            }

            if (m_iCardCount < 1)
            {
                MessageBox.Show("Please add some cards!");
                return;
            }

            g_fCardInfoRecordCallback = new CHCNetSDK.RemoteConfigCallback(ProcessCardInfoRecordCallback);
            string sURL = "POST /ISAPI/AccessControl/CardInfo/Record?format=json";
            IntPtr ptrURL = Marshal.StringToHGlobalAnsi(sURL);

            m_lCardInfoRecordHandle = CHCNetSDK.NET_DVR_StartRemoteConfig(m_lUserID, CHCNetSDK.NET_DVR_JSON_CONFIG, ptrURL, sURL.Length, g_fCardInfoRecordCallback, this.Handle);
            if (-1 == m_lCardInfoRecordHandle)
            {
                ListViewItem listItem = new ListViewItem();
                listItem.Text = "Fail";
                string strTemp = string.Format("Card Info Record Fail,Error Code={0}", CHCNetSDK.NET_DVR_GetLastError());
                listItem.SubItems.Add(strTemp);
                this.AddList(listViewMessage, listItem);
                Marshal.FreeHGlobal(ptrURL);
                return;
            }
            else
            {
                ListViewItem listItem = new ListViewItem();
                listItem.Text = "Succ";
                listItem.SubItems.Add("Card Info Record Success");
                this.AddList(listViewMessage, listItem);
            }
            Marshal.FreeHGlobal(ptrURL);
            if (!SendCardInfo())
            {
                CHCNetSDK.NET_DVR_StopRemoteConfig(m_lCardInfoRecordHandle);
                m_lCardInfoRecordHandle = -1;
            }
        }

        //modify card
        private void buttonModify_Click(object sender, EventArgs e)
        {
            m_iCardInfoRecordIndex = 0;
            if (-1 != m_lCardInfoRecordHandle)
            {
                if (CHCNetSDK.NET_DVR_StopRemoteConfig(m_lCardInfoRecordHandle))
                {
                    m_lCardInfoRecordHandle = -1;
                }
            }

            if (m_iCardCount < 1)
            {
                MessageBox.Show("Please add some cards!");
                return;
            }

            g_fCardInfoModifyCallback = new CHCNetSDK.RemoteConfigCallback(ProcessCardInfoRecordCallback);
            string sURL = "PUT /ISAPI/AccessControl/CardInfo/Modify?format=json";
            IntPtr ptrURL = Marshal.StringToHGlobalAnsi(sURL);

            m_lCardInfoRecordHandle = CHCNetSDK.NET_DVR_StartRemoteConfig(m_lUserID, CHCNetSDK.NET_DVR_JSON_CONFIG, ptrURL, sURL.Length, g_fCardInfoModifyCallback, this.Handle);
            if (-1 == m_lCardInfoRecordHandle)
            {
                ListViewItem listItem = new ListViewItem();
                listItem.Text = "Fail";
                string strTemp = string.Format("Card Info Modify Fail,Error Code={0}", CHCNetSDK.NET_DVR_GetLastError());
                listItem.SubItems.Add(strTemp);
                this.AddList(listViewMessage, listItem);
                Marshal.FreeHGlobal(ptrURL);
                return;
            }
            else
            {
                ListViewItem listItem = new ListViewItem();
                listItem.Text = "Succ";
                listItem.SubItems.Add("Card Info Modify Success");
                this.AddList(listViewMessage, listItem);
            }
            Marshal.FreeHGlobal(ptrURL);
            if (!SendCardInfo())
            {
                CHCNetSDK.NET_DVR_StopRemoteConfig(m_lCardInfoRecordHandle);
                m_lCardInfoRecordHandle = -1;
            }
        }

        //search card
        private void buttonSearchAll_Click(object sender, EventArgs e)
        {
            m_iSearchType = 0;

            m_iCardCount = 0;
            m_iSearchPosition = 0;
            listViewCardManage.Items.Clear();

            if (-1 != m_lCardInfoSearchHandle)
            {
                if (CHCNetSDK.NET_DVR_StopRemoteConfig(m_lCardInfoSearchHandle))
                {
                    m_lCardInfoSearchHandle = -1;
                }
            }

            g_fCardInfoSearchCallback = new CHCNetSDK.RemoteConfigCallback(ProcessCardInfoSearchCallback);
            string sURL = "POST /ISAPI/AccessControl/CardInfo/Search?format=json";
            IntPtr ptrURL = Marshal.StringToHGlobalAnsi(sURL);

            m_lCardInfoSearchHandle = CHCNetSDK.NET_DVR_StartRemoteConfig(m_lUserID, CHCNetSDK.NET_DVR_JSON_CONFIG, ptrURL, sURL.Length, g_fCardInfoSearchCallback, this.Handle);
            if (-1 == m_lCardInfoSearchHandle)
            {
                ListViewItem listItem = new ListViewItem();
                listItem.Text = "Fail";
                string strTemp = string.Format("Card Info Search Fail,Error Code={0}", CHCNetSDK.NET_DVR_GetLastError());
                listItem.SubItems.Add(strTemp);
                this.AddList(listViewMessage, listItem);
                Marshal.FreeHGlobal(ptrURL);
                return;
            }
            else
            {
                ListViewItem listItem = new ListViewItem();
                listItem.Text = "Succ";
                listItem.SubItems.Add("Card Info Search Success");
                this.AddList(listViewMessage, listItem);
            }
            Marshal.FreeHGlobal(ptrURL);
            if (!SearchCardInfo())
            {
                CHCNetSDK.NET_DVR_StopRemoteConfig(m_lCardInfoSearchHandle);
                m_lCardInfoSearchHandle = -1;
            }
        }

        //delete card
        private void buttonDeleteAll_Click(object sender, EventArgs e)
        {
            m_iDeleteType = 0;

            DeleteCardInfo();
        }

        private void buttonUpdateEmployeeNo_Click(object sender, EventArgs e)
        {
            if (listViewEmployeeNo.SelectedItems.Count > 0)
            {
                listViewEmployeeNo.SelectedItems[0].SubItems[1].Text = textBoxEmployeeNo2.Text;
            }
        }

        //search card by employee no
        private void buttonSearchByEmployeeNo_Click(object sender, EventArgs e)
        {
            m_iSearchType = 1;

            m_iCardCount = 0;
            m_iSearchPosition = 0;
            listViewCardManage.Items.Clear();

            if (-1 != m_lCardInfoSearchHandle)
            {
                if (CHCNetSDK.NET_DVR_StopRemoteConfig(m_lCardInfoSearchHandle))
                {
                    m_lCardInfoSearchHandle = -1;
                }
            }

            g_fCardInfoSearchCallback = new CHCNetSDK.RemoteConfigCallback(ProcessCardInfoSearchCallback);
            string sURL = "POST /ISAPI/AccessControl/CardInfo/Search?format=json";
            IntPtr ptrURL = Marshal.StringToHGlobalAnsi(sURL);

            m_lCardInfoSearchHandle = CHCNetSDK.NET_DVR_StartRemoteConfig(m_lUserID, CHCNetSDK.NET_DVR_JSON_CONFIG, ptrURL, sURL.Length, g_fCardInfoSearchCallback, this.Handle);
            if (-1 == m_lCardInfoSearchHandle)
            {
                ListViewItem listItem = new ListViewItem();
                listItem.Text = "Fail";
                string strTemp = string.Format("Card Info Search Fail,Error Code={0}", CHCNetSDK.NET_DVR_GetLastError());
                listItem.SubItems.Add(strTemp);
                this.AddList(listViewMessage, listItem);
                Marshal.FreeHGlobal(ptrURL);
                return;
            }
            else
            {
                ListViewItem listItem = new ListViewItem();
                listItem.Text = "Succ";
                listItem.SubItems.Add("Card Info Search Success");
                this.AddList(listViewMessage, listItem);
            }
            Marshal.FreeHGlobal(ptrURL);
            if (!SearchCardInfo())
            {
                CHCNetSDK.NET_DVR_StopRemoteConfig(m_lCardInfoSearchHandle);
                m_lCardInfoSearchHandle = -1;
            }
        }

        //delete card by employee no
        private void buttonDeleteByEmployeeNo_Click(object sender, EventArgs e)
        {
            m_iDeleteType = 1;

            DeleteCardInfo();
        }

        private void buttonUpdateCardNo_Click(object sender, EventArgs e)
        {
            if (listViewCardNo.SelectedItems.Count > 0)
            {
                listViewCardNo.SelectedItems[0].SubItems[1].Text = textBoxCardNo2.Text;
            }
        }

        //search card by card no
        private void buttonSearchByCardNo_Click(object sender, EventArgs e)
        {
            m_iSearchType = 2;

            m_iCardCount = 0;
            m_iSearchPosition = 0;
            listViewCardManage.Items.Clear();

            if (-1 != m_lCardInfoSearchHandle)
            {
                if (CHCNetSDK.NET_DVR_StopRemoteConfig(m_lCardInfoSearchHandle))
                {
                    m_lCardInfoSearchHandle = -1;
                }
            }

            g_fCardInfoSearchCallback = new CHCNetSDK.RemoteConfigCallback(ProcessCardInfoSearchCallback);
            string sURL = "POST /ISAPI/AccessControl/CardInfo/Search?format=json";
            IntPtr ptrURL = Marshal.StringToHGlobalAnsi(sURL);

            m_lCardInfoSearchHandle = CHCNetSDK.NET_DVR_StartRemoteConfig(m_lUserID, CHCNetSDK.NET_DVR_JSON_CONFIG, ptrURL, sURL.Length, g_fCardInfoSearchCallback, this.Handle);
            if (-1 == m_lCardInfoSearchHandle)
            {
                ListViewItem listItem = new ListViewItem();
                listItem.Text = "Fail";
                string strTemp = string.Format("Card Info Search Fail,Error Code={0}", CHCNetSDK.NET_DVR_GetLastError());
                listItem.SubItems.Add(strTemp);
                this.AddList(listViewMessage, listItem);
                Marshal.FreeHGlobal(ptrURL);
                return;
            }
            else
            {
                ListViewItem listItem = new ListViewItem();
                listItem.Text = "Succ";
                listItem.SubItems.Add("Card Info Search Success");
                this.AddList(listViewMessage, listItem);
            }
            Marshal.FreeHGlobal(ptrURL);
            if (!SearchCardInfo())
            {
                CHCNetSDK.NET_DVR_StopRemoteConfig(m_lCardInfoSearchHandle);
                m_lCardInfoSearchHandle = -1;
            }
        }

        //delete card by card no
        private void buttonDeleteByCardNo_Click(object sender, EventArgs e)
        {
            m_iDeleteType = 2;

            DeleteCardInfo();
        }

        private void buttonUpdateList_Click(object sender, EventArgs e)
        {
            if (listViewCardManage.SelectedItems.Count > 0)
            {
                listViewCardManage.SelectedItems[0].SubItems[1].Text = textBoxEmployeeNo.Text;
                listViewCardManage.SelectedItems[0].SubItems[2].Text = textBoxCardNo.Text;
                listViewCardManage.SelectedItems[0].SubItems[3].Text = comboBoxCardType.Text;
                listViewCardManage.SelectedItems[0].SubItems[4].Text = textBoxLeaderCard.Text;
            }
        }

        private void buttonClearList_Click(object sender, EventArgs e)
        {
            m_iCardCount = 0;
            listViewCardManage.Items.Clear();
        }

        //get card count
        private void buttonGetAllCount_Click(object sender, EventArgs e)
        {
            IntPtr ptrOutBuf = Marshal.AllocHGlobal(1024);
            IntPtr ptrStatusBuffer = Marshal.AllocHGlobal(1024);

            for (int i = 0; i < 1024; i++)
            {
                Marshal.WriteByte(ptrOutBuf, i, 0);
                Marshal.WriteByte(ptrStatusBuffer, i, 0);
            }

            CHCNetSDK.NET_DVR_XML_CONFIG_INPUT struInput = new CHCNetSDK.NET_DVR_XML_CONFIG_INPUT();
            CHCNetSDK.NET_DVR_XML_CONFIG_OUTPUT struOuput = new CHCNetSDK.NET_DVR_XML_CONFIG_OUTPUT();

            string sUrl = "GET /ISAPI/AccessControl/CardInfo/Count?format=json";
            IntPtr ptrURL = Marshal.StringToHGlobalAnsi(sUrl);
            struInput.dwSize = (uint)Marshal.SizeOf(struInput);
            struInput.lpRequestUrl = ptrURL;
            struInput.dwRequestUrlLen = (uint)sUrl.Length;

            struOuput.dwSize = (uint)Marshal.SizeOf(struOuput);
            struOuput.lpOutBuffer = ptrOutBuf;
            struOuput.dwOutBufferSize = 1024;
            struOuput.lpStatusBuffer = ptrStatusBuffer;
            struOuput.dwStatusSize = 1024;

            IntPtr ptrInput = Marshal.AllocHGlobal(Marshal.SizeOf(struInput));
            Marshal.StructureToPtr(struInput, ptrInput, false);
            IntPtr ptrOuput = Marshal.AllocHGlobal(Marshal.SizeOf(struOuput));
            Marshal.StructureToPtr(struOuput, ptrOuput, false);
            if (!CHCNetSDK.NET_DVR_STDXMLConfig(m_lUserID, ptrInput, ptrOuput))
            {
                ListViewItem listItem = new ListViewItem();
                listItem.Text = "Fail";
                string strTemp = string.Format("Get Card Info Count Fail,Error Code={0}", CHCNetSDK.NET_DVR_GetLastError());
                listItem.SubItems.Add(strTemp);
                this.AddList(listViewMessage, listItem);
            }
            else
            {
                ListViewItem listItem = new ListViewItem();
                listItem.Text = "Succ";
                string strTemp = string.Format("Get Card Info Count Success");
                listItem.SubItems.Add(strTemp);
                this.AddList(listViewMessage, listItem);

                string strCardInfoCount = Marshal.PtrToStringAnsi(struOuput.lpOutBuffer);
                CCardInfoCount JsonCardInfoCount = new CCardInfoCount();
                JsonCardInfoCount = JsonConvert.DeserializeObject<CCardInfoCount>(strCardInfoCount);
                textBoxCardInfoCount.Text = JsonCardInfoCount.CardInfoCount.cardNumber.ToString();
            }

            Marshal.FreeHGlobal(ptrOutBuf);
            Marshal.FreeHGlobal(ptrStatusBuffer);
            Marshal.FreeHGlobal(ptrInput);
            Marshal.FreeHGlobal(ptrOuput);
            Marshal.FreeHGlobal(ptrURL);
        }

        //get card count by employee no
        private void buttonGetByEmployeeNo_Click(object sender, EventArgs e)
        {
            IntPtr ptrOutBuf = Marshal.AllocHGlobal(1024);
            IntPtr ptrStatusBuffer = Marshal.AllocHGlobal(1024);

            for (int i = 0; i < 1024; i++)
            {
                Marshal.WriteByte(ptrOutBuf, i, 0);
                Marshal.WriteByte(ptrStatusBuffer, i, 0);
            }

            CHCNetSDK.NET_DVR_XML_CONFIG_INPUT struInput = new CHCNetSDK.NET_DVR_XML_CONFIG_INPUT();
            CHCNetSDK.NET_DVR_XML_CONFIG_OUTPUT struOuput = new CHCNetSDK.NET_DVR_XML_CONFIG_OUTPUT();

            string sUrl = "GET /ISAPI/AccessControl/CardInfo/Count?format=json&employeeNo=" + textBoxEmployeeNo3.Text;
            IntPtr ptrURL = Marshal.StringToHGlobalAnsi(sUrl);
            struInput.dwSize = (uint)Marshal.SizeOf(struInput);
            struInput.lpRequestUrl = ptrURL;
            struInput.dwRequestUrlLen = (uint)sUrl.Length;

            struOuput.dwSize = (uint)Marshal.SizeOf(struOuput);
            struOuput.lpOutBuffer = ptrOutBuf;
            struOuput.dwOutBufferSize = 1024;
            struOuput.lpStatusBuffer = ptrStatusBuffer;
            struOuput.dwStatusSize = 1024;

            IntPtr ptrInput = Marshal.AllocHGlobal(Marshal.SizeOf(struInput));
            Marshal.StructureToPtr(struInput, ptrInput, false);
            IntPtr ptrOuput = Marshal.AllocHGlobal(Marshal.SizeOf(struOuput));
            Marshal.StructureToPtr(struOuput, ptrOuput, false);
            if (!CHCNetSDK.NET_DVR_STDXMLConfig(m_lUserID, ptrInput, ptrOuput))
            {
                ListViewItem listItem = new ListViewItem();
                listItem.Text = "Fail";
                string strTemp = string.Format("Get Card Info Count Fail,Error Code={0}", CHCNetSDK.NET_DVR_GetLastError());
                listItem.SubItems.Add(strTemp);
                this.AddList(listViewMessage, listItem);
            }
            else
            {
                ListViewItem listItem = new ListViewItem();
                listItem.Text = "Succ";
                string strTemp = string.Format("Get Card Info Count Success");
                listItem.SubItems.Add(strTemp);
                this.AddList(listViewMessage, listItem);

                string strCardInfoCount = Marshal.PtrToStringAnsi(struOuput.lpOutBuffer);
                CCardInfoCount JsonCardInfoCount = new CCardInfoCount();
                JsonCardInfoCount = JsonConvert.DeserializeObject<CCardInfoCount>(strCardInfoCount);
                textBoxCardInfoCount.Text = JsonCardInfoCount.CardInfoCount.cardNumber.ToString();
            }

            Marshal.FreeHGlobal(ptrOutBuf);
            Marshal.FreeHGlobal(ptrStatusBuffer);
            Marshal.FreeHGlobal(ptrInput);
            Marshal.FreeHGlobal(ptrOuput);
            Marshal.FreeHGlobal(ptrURL);
        }

        private void listViewCardManage_ItemSelectionChanged(object sender, ListViewItemSelectionChangedEventArgs e)
        {
            if (listViewCardManage.SelectedItems.Count > 0)
            {
                textBoxEmployeeNo.Text = e.Item.SubItems[1].Text;
                textBoxCardNo.Text = e.Item.SubItems[2].Text;
                comboBoxCardType.Text = e.Item.SubItems[3].Text;
                textBoxLeaderCard.Text = e.Item.SubItems[4].Text;
            }
        }

        private void listViewEmployeeNo_ItemSelectionChanged(object sender, ListViewItemSelectionChangedEventArgs e)
        {
            if (listViewEmployeeNo.SelectedItems.Count > 0)
            {
                textBoxEmployeeNo2.Text = e.Item.SubItems[1].Text;
            }
        }

        private void listViewCardNo_ItemSelectionChanged(object sender, ListViewItemSelectionChangedEventArgs e)
        {
            if (listViewCardNo.SelectedItems.Count > 0)
            {
                textBoxCardNo2.Text = e.Item.SubItems[1].Text;
            }
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
                        this.AddList(listViewMessage, listItem);
                    }
                    else if (iErrorMsg == 2)
                    {
                        ListViewItem listItem = new ListViewItem();
                        listItem.Text = "Fail";
                        string strTemp = string.Format("NET_SDK_CALLBACK_STATUS_EXCEPTION");
                        listItem.SubItems.Add(strTemp);
                        this.AddList(listViewMessage, listItem);
                    }
                    else if (iErrorMsg == 3)
                    {
                        ListViewItem listItem = new ListViewItem();
                        listItem.Text = "Fail";
                        string strTemp = string.Format("NET_SDK_CALLBACK_OTHER_ERROR");
                        listItem.SubItems.Add(strTemp);
                        this.AddList(listViewMessage, listItem);
                    }
                    else if (iErrorMsg == 4)
                    {
                        ListViewItem listItem = new ListViewItem();
                        listItem.Text = "Succ";
                        string strTemp = string.Format("NET_SDK_CALLBACK_FINISH");
                        listItem.SubItems.Add(strTemp);
                        this.AddList(listViewMessage, listItem);
                    }
                    if (-1 != m_lCardInfoRecordHandle)
                    {
                        if (CHCNetSDK.NET_DVR_StopRemoteConfig(m_lCardInfoRecordHandle))
                        {
                            m_lCardInfoRecordHandle = -1;
                        }
                    }
                    break;
                case 1002:
                    SendCardInfo();
                    break;
                case 1003:
                    int iError = m.WParam.ToInt32();
                    if (iError == 1)
                    {
                        ListViewItem listItem = new ListViewItem();
                        listItem.Text = "Succ";
                        string strTemp = string.Format("Send Success");
                        listItem.SubItems.Add(strTemp);
                        this.AddList(listViewMessage, listItem);
                    }
                    else
                    {
                        ListViewItem listItem = new ListViewItem();
                        listItem.Text = "Fail";
                        string strTemp = string.Format("Send Failed");
                        listItem.SubItems.Add(strTemp);
                        this.AddList(listViewMessage, listItem);
                    }
                    break;
                case 1004:
                    int iErrorSer = m.WParam.ToInt32();
                    if (iErrorSer == 1)
                    {
                        ListViewItem listItem = new ListViewItem();
                        listItem.Text = "Fail";
                        string strTemp = string.Format("NET_SDK_CALLBACK_STATUS_FAILED");
                        listItem.SubItems.Add(strTemp);
                        this.AddList(listViewMessage, listItem);
                    }
                    else if (iErrorSer == 2)
                    {
                        ListViewItem listItem = new ListViewItem();
                        listItem.Text = "Fail";
                        string strTemp = string.Format("NET_SDK_CALLBACK_STATUS_EXCEPTION");
                        listItem.SubItems.Add(strTemp);
                        this.AddList(listViewMessage, listItem);
                    }
                    else if (iErrorSer == 3)
                    {
                        ListViewItem listItem = new ListViewItem();
                        listItem.Text = "Fail";
                        string strTemp = string.Format("NET_SDK_CALLBACK_OTHER_ERROR");
                        listItem.SubItems.Add(strTemp);
                        this.AddList(listViewMessage, listItem);
                    }
                    else if (iErrorSer == 4)
                    {
                        ListViewItem listItem = new ListViewItem();
                        listItem.Text = "Succ";
                        string strTemp = string.Format("NET_SDK_CALLBACK_FINISH");
                        listItem.SubItems.Add(strTemp);
                        this.AddList(listViewMessage, listItem);
                    }
                    if (-1 != m_lCardInfoSearchHandle)
                    {
                        if (CHCNetSDK.NET_DVR_StopRemoteConfig(m_lCardInfoSearchHandle))
                        {
                            m_lCardInfoSearchHandle = -1;
                        }
                    }
                    break;
                case 1005:
                    SearchCardInfo();
                    break;
                case 1006:
                    for (int i = 0; i < m_JsonCardInfoSearch.CardInfoSearch.numOfMatches; i++)
                    {
                        listViewCardManage.BeginUpdate();

                        ListViewItem listItem = new ListViewItem();
                        listItem.Text = (m_iCardCount + 1).ToString();
                        listItem.SubItems.Add(m_JsonCardInfoSearch.CardInfoSearch.CardInfo[i].employeeNo.ToString());
                        if (m_JsonCardInfoSearch.CardInfoSearch.CardInfo[i].cardNo != null)
                        {
                            listItem.SubItems.Add(m_JsonCardInfoSearch.CardInfoSearch.CardInfo[i].cardNo);
                        }
                        else
                        {
                            listItem.SubItems.Add("");
                        }
                        if (m_JsonCardInfoSearch.CardInfoSearch.CardInfo[i].cardType != null)
                        {
                            listItem.SubItems.Add(m_JsonCardInfoSearch.CardInfoSearch.CardInfo[i].cardType);
                        }
                        else
                        {
                            listItem.SubItems.Add("");
                        }
                        if (m_JsonCardInfoSearch.CardInfoSearch.CardInfo[i].leaderCard != null)
                        {
                            listItem.SubItems.Add(m_JsonCardInfoSearch.CardInfoSearch.CardInfo[i].leaderCard);
                        }
                        else
                        {
                            listItem.SubItems.Add("");
                        }

                        listViewCardManage.Items.Add(listItem);
                        listViewCardManage.EndUpdate();

                        m_iCardCount += 1;
                    }
                    break;
                default:
                    base.DefWndProc(ref m);
                    break;
            }
        }

        private void buttonSyn_Click(object sender, EventArgs e)
        {
            SynInfo dlg = new SynInfo();
            dlg.m_command = Public.SynInfoCommand.CARD_INFO_SYN;
            dlg.m_lUserID = m_lUserID;
            dlg.ShowDialog();
        }

        private void CardManagementByStaffCenter_Load(object sender, EventArgs e)
        {
            MultiLanguage.LoadLanguage(this);
        }
    }
}
