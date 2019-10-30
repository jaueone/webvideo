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
    public partial class StaffManagement : Form
    {
        public Int32 m_lUserID = -1;
        public Int32 m_lUserInfoRecordHandle = -1;
        public Int32 m_lUserInfoDeleteHandle = -1;
        public int m_iUserCount = 0;
        public int m_iUserInfoRecordIndex = 0;
        public Int32 m_lUserInfoSearchHandle = -1;
        public bool m_bSearchAll = true;
        public bool m_bDeleteAll = true;
        public int m_iSearchPosition = 0;

        private delegate void AddListCallBack(ListView cListView, ListViewItem cListViewItem);
        private CHCNetSDK.RemoteConfigCallback g_fUserInfoRecordCallback = null;
        private CHCNetSDK.RemoteConfigCallback g_fUserInfoModifyCallback = null;
        private CHCNetSDK.RemoteConfigCallback g_fUserInfoSearchCallback = null;
        private CHCNetSDK.RemoteConfigCallback g_fUserInfoDeleteCallback = null;

        CUserInfoSearch m_JsonUserInfoSearch;

        public class CUserInfoRecord
        {
            public CUserInfoContent UserInfo { get; set; }
        }

        public class CUserInfoContent
        {
            public string employeeNo { get; set; }
            public string name { get; set; }
            public string userType { get; set; }
            public bool closeDelayEnabled { get; set; }
            public CVaild Valid { get; set; }
            public string belongGroup { get; set; }
            public string password { get; set; }
            public string doorRight { get; set; }
            public List<CRightPlan> RightPlan { get; set; }
            public int maxOpenDoorTime { get; set; }
            public int openDoorTime { get; set; }
            public int roomNumber { get; set; }
            public int floorNumber { get; set; }
            public bool doubleLockRight { get; set; }
            public bool alwaysOpenRight { get; set; }
            public bool localUIRight { get; set; }
            public string userVerifyMode { get; set; }
        }

        public class CVaild
        {
            public bool enable { get; set; }
            public string beginTime { get; set; }
            public string endTime { get; set; }
            public string timeType { get; set; }
        }

        public class CRightPlan
        {
            public int doorNo { get; set; }
            public string planTemplateNo { get; set; }
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

        public class CUserInfoSearchCond
        {
            public CUserInfoSearchCondContent UserInfoSearchCond { get; set; }
        }

        public class CUserInfoSearchCondContent
        {
            public string searchID { get; set; }
            public int searchResultPosition { get; set; }
            public int maxResults { get; set; }
            public List<CEmployeeNoList> EmployeeNoList { get; set; }
        }

        public class CEmployeeNoList
        {
            public string employeeNo { get; set; }
        }

        public class CUserInfoSearch
        {
            public CUserInfoSearchContent UserInfoSearch { get; set; }
        }

        public class CUserInfoSearchContent
        {
            public string searchID { get; set; }
            public string responseStatusStrg { get; set; }
            public int numOfMatches { get; set; }
            public int totalMatches { get; set; }
            public List<CUserInfoContent> UserInfo { get; set; }
        }

        public class CUserInfoCount
        {
            public CUserInfoCountContent UserInfoCount { get; set; }
        }

        public class CUserInfoCountContent
        {
            public int userNumber { get; set; }
        }

        public class CUserInfoDelCond
        {
            public CUserInfoDelCondContent UserInfoDelCond { get; set; }
        }

        public class CUserInfoDelCondContent
        {
            public List<CEmployeeNoList> EmployeeNoList { get; set; }
        }

        public class CUserInfoDetail
        {
            public CUserInfoDelContent UserInfoDetail { get; set; }
        }

        public class CUserInfoDelContent
        {
            public string mode { get; set; }
            public List<CEmployeeNoList> EmployeeNoList { get; set; }
        }

        public class CUserInfoDetailDeleteProcess
        {
            public CUserInfoDetailDeleteProcessContent UserInfoDetailDeleteProcess { get; set; }
        }

        public class CUserInfoDetailDeleteProcessContent
        {
            public string status { get; set; }
        }

        public StaffManagement()
        {
            InitializeComponent();
            comboBoxUserType.SelectedIndex = 0;
            comboBoxUserVerifyMode.SelectedIndex = 0;
            comboBoxTimeType.SelectedIndex = 0;
            checkBoxTimeEnable.Checked = true;
            UpdateListRightPlan();
            UpdateListEmployeeNo();
            this.SuspendLayout();
            this.columnHeader1.Name = "columnHeader1";
            this.columnHeader2.Name = "columnHeader2";
            this.columnHeader3.Name = "columnHeader3";
            this.columnHeader4.Name = "columnHeader4";
            this.columnHeader5.Name = "columnHeader5";
            this.columnHeader6.Name = "columnHeader6";
            this.columnHeader7.Name = "columnHeader7";
            this.columnHeader8.Name = "columnHeader8";
            this.columnHeader9.Name = "columnHeader9";
            this.columnHeader10.Name = "columnHeader10";
            this.columnHeader11.Name = "columnHeader11";
            this.columnHeader12.Name = "columnHeader12";
            this.columnHeader13.Name = "columnHeader13";
            this.columnHeader18.Name = "columnHeader18";
            this.columnHeader19.Name = "columnHeader19";
            this.columnHeader20.Name = "columnHeader20";
            this.columnHeader23.Name = "columnHeader23";
            this.columnHeader24.Name = "columnHeader24";
            this.columnHeader25.Name = "columnHeader25";
            this.columnHeader26.Name = "columnHeader26";
            this.columnHeader14.Name = "columnHeader14";
            this.columnHeader15.Name = "columnHeader15";
            this.columnHeader16.Name = "columnHeader16";
            this.columnHeader17.Name = "columnHeader17";
            this.columnHeader21.Name = "columnHeader21";
            this.columnHeader22.Name = "columnHeader22";
            this.ResumeLayout();
        }

        private void UpdateListRightPlan()
        {
            listViewRightPlan.BeginUpdate();
            listViewRightPlan.Items.Clear();
            for (int i = 0; i < 256; i++)
            {
                ListViewItem listItem = new ListViewItem();
                listItem.Text = (i + 1).ToString();
                if (i <= 1)
                {
                    listItem.SubItems.Add("1,2");
                }
                else
                {
                    listItem.SubItems.Add("");
                }
                listViewRightPlan.Items.Add(listItem);
            }
            listViewRightPlan.EndUpdate();
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

        private void ProcessUserInfoRecordCallback(uint dwType, IntPtr lpBuffer, uint dwBufLen, IntPtr pUserData)
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
                string strResponseStatus = Marshal.PtrToStringAnsi(lpBuffer, (int)dwBufLen);
                CResponseStatus JsonResponseStatus = new CResponseStatus();
                JsonResponseStatus = JsonConvert.DeserializeObject<CResponseStatus>(strResponseStatus);

                CHCNetSDK.PostMessage(pUserData, 1003, JsonResponseStatus.statusCode, 0);

                if (m_iUserInfoRecordIndex >= m_iUserCount)
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

        private void ProcessUserInfoDeleteCallback(uint dwType, IntPtr lpBuffer, uint dwBufLen, IntPtr pUserData)
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
                    CHCNetSDK.PostMessage(pUserData, 1007, 1, 0);
                    //可以结束了，异常结束
                }
                else if (dwStatus == (uint)CHCNetSDK.NET_SDK_CALLBACK_STATUS_NORMAL.NET_SDK_CALLBACK_STATUS_EXCEPTION)
                {
                    CHCNetSDK.PostMessage(pUserData, 1007, 2, 0);
                    //可以结束了，异常结束
                }
                else if (dwStatus == (uint)CHCNetSDK.NET_SDK_CALLBACK_STATUS_NORMAL.NET_SDK_CALLBACK_STATUS_PROCESSING)
                {
                    ;
                }
                else
                {
                    CHCNetSDK.PostMessage(pUserData, 1007, 3, 0);
                    //可以结束了，异常结束
                }
            }
            else if (dwType == (uint)CHCNetSDK.NET_SDK_CALLBACK_TYPE.NET_SDK_CALLBACK_TYPE_DATA)
            {
                string strUserInfoDetailDeleteProcess = Marshal.PtrToStringAnsi(lpBuffer);
                CUserInfoDetailDeleteProcess JsonUserInfoDetailDeleteProcess = new CUserInfoDetailDeleteProcess();
                JsonUserInfoDetailDeleteProcess = JsonConvert.DeserializeObject<CUserInfoDetailDeleteProcess>(strUserInfoDetailDeleteProcess);
                if (JsonUserInfoDetailDeleteProcess.UserInfoDetailDeleteProcess.status == "processing")
                {
                    CHCNetSDK.PostMessage(pUserData, 1007, 4, 0);
                    //打印设备处于处理中
                }
                else if (JsonUserInfoDetailDeleteProcess.UserInfoDetailDeleteProcess.status == "success")
                {
                    CHCNetSDK.PostMessage(pUserData, 1007, 5, 0);
                    //打印设备删除成功，可以结束了
                }
                else if (JsonUserInfoDetailDeleteProcess.UserInfoDetailDeleteProcess.status == "failed")
                {
                    CHCNetSDK.PostMessage(pUserData, 1007, 6, 0);
                    //打印设备删除失败，可以结束了
                }
            }
            return;
        }

        private void ProcessUserInfoSearchCallback(uint dwType, IntPtr lpBuffer, uint dwBufLen, IntPtr pUserData)
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
                byte[] bUserInfoSearch = new byte[1024 * 10];
                Marshal.Copy(lpBuffer, bUserInfoSearch, 0, bUserInfoSearch.Length);
                string strUserInfoSearch = System.Text.Encoding.UTF8.GetString(bUserInfoSearch);

                m_JsonUserInfoSearch = new CUserInfoSearch();
                m_JsonUserInfoSearch = JsonConvert.DeserializeObject<CUserInfoSearch>(strUserInfoSearch);
                if (m_JsonUserInfoSearch.UserInfoSearch == null)
                {
                    //此处说明返回了Response报文了
                    CHCNetSDK.PostMessage(pUserData, 1004, 3, 0);
                    return;
                }

                if (m_JsonUserInfoSearch.UserInfoSearch.responseStatusStrg != "NO MATCH")
                {
                    CHCNetSDK.PostMessage(pUserData, 1006, 0, 0);
                }
                //根据上一包responseStatusStrg的结果，判定是结束还是继续查询

                if (m_JsonUserInfoSearch.UserInfoSearch.responseStatusStrg != "MORE")
                {
                    CHCNetSDK.PostMessage(pUserData, 1004, 4, 0);
                    //可以结束了，正常（异常）结束
                    return;
                }

                m_iSearchPosition += m_JsonUserInfoSearch.UserInfoSearch.numOfMatches;
                CHCNetSDK.PostMessage(pUserData, 1005, 0, 0);
                //在主线程中发送下一个用户
            }
            return;
        }

        private bool SendUserInfo()
        {
            if (-1 == m_lUserInfoRecordHandle)
            {
                return false;
            }

            if (m_iUserInfoRecordIndex >= m_iUserCount)
            {
                return false;
            }

            CUserInfoRecord JsonUserInfoRecord = new CUserInfoRecord();
            JsonUserInfoRecord.UserInfo = new CUserInfoContent();
            JsonUserInfoRecord.UserInfo.Valid = new CVaild();
            JsonUserInfoRecord.UserInfo.RightPlan = new List<CRightPlan>();

            JsonUserInfoRecord.UserInfo.employeeNo = listViewStaffManage.Items[m_iUserInfoRecordIndex].SubItems[1].Text;
            JsonUserInfoRecord.UserInfo.name = listViewStaffManage.Items[m_iUserInfoRecordIndex].SubItems[2].Text;
            JsonUserInfoRecord.UserInfo.userType = listViewStaffManage.Items[m_iUserInfoRecordIndex].SubItems[3].Text;
            if (listViewStaffManage.Items[m_iUserInfoRecordIndex].SubItems[4].Text == "True")
            {
                JsonUserInfoRecord.UserInfo.closeDelayEnabled = true;
            }
            else if (listViewStaffManage.Items[m_iUserInfoRecordIndex].SubItems[4].Text == "False")
            {
                JsonUserInfoRecord.UserInfo.closeDelayEnabled = false;
            }

            if ((listViewStaffManage.Items[m_iUserInfoRecordIndex].SubItems[5].Text == "") && (listViewStaffManage.Items[m_iUserInfoRecordIndex].SubItems[6].Text == ""))
            {
                JsonUserInfoRecord.UserInfo.Valid.enable = false;
            }
            else
            {
                JsonUserInfoRecord.UserInfo.Valid.enable = true;
            }
            JsonUserInfoRecord.UserInfo.Valid.beginTime = listViewStaffManage.Items[m_iUserInfoRecordIndex].SubItems[5].Text;
            JsonUserInfoRecord.UserInfo.Valid.endTime = listViewStaffManage.Items[m_iUserInfoRecordIndex].SubItems[6].Text;

            JsonUserInfoRecord.UserInfo.belongGroup = listViewStaffManage.Items[m_iUserInfoRecordIndex].SubItems[7].Text;
            JsonUserInfoRecord.UserInfo.password = listViewStaffManage.Items[m_iUserInfoRecordIndex].SubItems[8].Text;
            JsonUserInfoRecord.UserInfo.doorRight = listViewStaffManage.Items[m_iUserInfoRecordIndex].SubItems[9].Text;

            string sRightPlan = listViewStaffManage.Items[m_iUserInfoRecordIndex].SubItems[10].Text;
            int iDoorNoBegin = 0;
            int iDoorNoEnd = 0;
            int iPlanTemplateBegin = 0;
            int iPlanTemplateEnd = 0;
            for (int iCurrentLocate = 0; iCurrentLocate < sRightPlan.Length - 1; )
            {
                if (sRightPlan.IndexOf("Door", iCurrentLocate) != -1)
                {
                    iDoorNoBegin = sRightPlan.IndexOf("Door", iCurrentLocate) + 4;
                    iDoorNoEnd = sRightPlan.IndexOf(":", iCurrentLocate) - 1;
                    iPlanTemplateBegin = sRightPlan.IndexOf(":", iCurrentLocate) + 1;
                    iPlanTemplateEnd = sRightPlan.IndexOf("-", iCurrentLocate) - 1;
                    iCurrentLocate = sRightPlan.IndexOf("-", iCurrentLocate) + 1;
                    int iDoorNo = 0;
                    int.TryParse(sRightPlan.Substring(iDoorNoBegin, (iDoorNoEnd - iDoorNoBegin + 1)), out iDoorNo);
                    CRightPlan singleRightPlan = new CRightPlan();
                    singleRightPlan.doorNo = iDoorNo;
                    singleRightPlan.planTemplateNo = sRightPlan.Substring(iPlanTemplateBegin, (iPlanTemplateEnd - iPlanTemplateBegin + 1));
                    JsonUserInfoRecord.UserInfo.RightPlan.Add(singleRightPlan);
                }
                else
                {
                    break;
                }
            }

            int iMaxOpenDoorTime = 0;
            int.TryParse(listViewStaffManage.Items[m_iUserInfoRecordIndex].SubItems[11].Text, out iMaxOpenDoorTime);
            JsonUserInfoRecord.UserInfo.maxOpenDoorTime = iMaxOpenDoorTime;
            int iRoomNumber = 0;
            int.TryParse(listViewStaffManage.Items[m_iUserInfoRecordIndex].SubItems[12].Text, out iRoomNumber);
            JsonUserInfoRecord.UserInfo.roomNumber = iRoomNumber;
            int iFloorNumber = 0;
            int.TryParse(listViewStaffManage.Items[m_iUserInfoRecordIndex].SubItems[13].Text, out iFloorNumber);
            JsonUserInfoRecord.UserInfo.floorNumber = iFloorNumber;

            if (listViewStaffManage.Items[m_iUserInfoRecordIndex].SubItems[14].Text == "True")
            {
                JsonUserInfoRecord.UserInfo.doubleLockRight = true;
            }
            else if (listViewStaffManage.Items[m_iUserInfoRecordIndex].SubItems[14].Text == "False")
            {
                JsonUserInfoRecord.UserInfo.doubleLockRight = false;
            }

            if (listViewStaffManage.Items[m_iUserInfoRecordIndex].SubItems[15].Text == "True")
            {
                JsonUserInfoRecord.UserInfo.alwaysOpenRight = true;
            }
            else if (listViewStaffManage.Items[m_iUserInfoRecordIndex].SubItems[15].Text == "False")
            {
                JsonUserInfoRecord.UserInfo.alwaysOpenRight = false;
            }

            if (listViewStaffManage.Items[m_iUserInfoRecordIndex].SubItems[16].Text == "True")
            {
                JsonUserInfoRecord.UserInfo.localUIRight = true;
            }
            else if (listViewStaffManage.Items[m_iUserInfoRecordIndex].SubItems[16].Text == "False")
            {
                JsonUserInfoRecord.UserInfo.localUIRight = false;
            }

            JsonUserInfoRecord.UserInfo.userVerifyMode = listViewStaffManage.Items[m_iUserInfoRecordIndex].SubItems[17].Text;
            JsonUserInfoRecord.UserInfo.Valid.timeType = listViewStaffManage.Items[m_iUserInfoRecordIndex].SubItems[18].Text;

            int iOpenDoorTime = 0;
            int.TryParse(listViewStaffManage.Items[m_iUserInfoRecordIndex].SubItems[19].Text, out iOpenDoorTime);
            JsonUserInfoRecord.UserInfo.openDoorTime = iOpenDoorTime;

            string strUserInfoRecord = JsonConvert.SerializeObject(JsonUserInfoRecord);

            byte[] byUserInfoRecord = System.Text.Encoding.UTF8.GetBytes(strUserInfoRecord);
            IntPtr ptrUserInfoRecord = Marshal.AllocHGlobal(2048);
            Marshal.Copy(byUserInfoRecord, 0, ptrUserInfoRecord, byUserInfoRecord.Length);

            if (!CHCNetSDK.NET_DVR_SendRemoteConfig(m_lUserInfoRecordHandle, (int)CHCNetSDK.LONG_CFG_SEND_DATA_TYPE_ENUM.ENUM_SEND_JSON_DATA, ptrUserInfoRecord, (uint)byUserInfoRecord.Length))
            {

                ListViewItem listItem = new ListViewItem();
                listItem.Text = "Fail";
                string strTemp = null;
                strTemp = string.Format("Send Fail,EmployeeNo:{0}", listViewStaffManage.Items[m_iUserInfoRecordIndex].SubItems[1].Text);
                listItem.SubItems.Add(strTemp);
                this.AddList(listViewMessage, listItem);
                Marshal.FreeHGlobal(ptrUserInfoRecord);
                return false;
            }
            ListViewItem listItemSucc = new ListViewItem();
            listItemSucc.Text = "Succ";
            string strTempSucc = null;
            strTempSucc = string.Format("Send Processing,EmployeeNo:{0}", listViewStaffManage.Items[m_iUserInfoRecordIndex].SubItems[1].Text);
            listItemSucc.SubItems.Add(strTempSucc);
            this.AddList(listViewMessage, listItemSucc);
            Marshal.FreeHGlobal(ptrUserInfoRecord);
            m_iUserInfoRecordIndex++;
            return true;
        }

        private bool SearchUserInfo()
        {
            if (-1 == m_lUserInfoSearchHandle)
            {
                return false;
            }

            CUserInfoSearchCond JsonUserInfoSearchCond = new CUserInfoSearchCond();
            JsonUserInfoSearchCond.UserInfoSearchCond = new CUserInfoSearchCondContent();
            JsonUserInfoSearchCond.UserInfoSearchCond.EmployeeNoList = new List<CEmployeeNoList>();

            JsonUserInfoSearchCond.UserInfoSearchCond.searchID = "1";
            JsonUserInfoSearchCond.UserInfoSearchCond.searchResultPosition = m_iSearchPosition;
            JsonUserInfoSearchCond.UserInfoSearchCond.maxResults = 10;

            if (!m_bSearchAll)
            {
                foreach (ListViewItem item in this.listViewEmployeeNo.Items)
                {
                    if (item.SubItems[1].Text != "")
                    {
                        CEmployeeNoList singleEmployeeNoList = new CEmployeeNoList();
                        singleEmployeeNoList.employeeNo = item.SubItems[1].Text;
                        JsonUserInfoSearchCond.UserInfoSearchCond.EmployeeNoList.Add(singleEmployeeNoList);
                    }
                }
            }

            string strUserInfoSearchCond = JsonConvert.SerializeObject(JsonUserInfoSearchCond);

            IntPtr ptrUserInfoSearchCond = Marshal.StringToHGlobalAnsi(strUserInfoSearchCond);

            if (!CHCNetSDK.NET_DVR_SendRemoteConfig(m_lUserInfoSearchHandle, (int)CHCNetSDK.LONG_CFG_SEND_DATA_TYPE_ENUM.ENUM_SEND_JSON_DATA, ptrUserInfoSearchCond, (uint)strUserInfoSearchCond.Length))
            {
                ListViewItem listItem = new ListViewItem();
                listItem.Text = "Fail";
                string strTemp = null;
                strTemp = string.Format("Search Fail");
                listItem.SubItems.Add(strTemp);
                this.AddList(listViewMessage, listItem);
                Marshal.FreeHGlobal(ptrUserInfoSearchCond);
                return false;
            }
            ListViewItem listItemSucc = new ListViewItem();
            listItemSucc.Text = "Succ";
            string strTempSucc = null;
            strTempSucc = string.Format("Search Processing");
            listItemSucc.SubItems.Add(strTempSucc);
            this.AddList(listViewMessage, listItemSucc);
            Marshal.FreeHGlobal(ptrUserInfoSearchCond);
            return true;
        }

        private void DeleteUserInfo()
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

            string sUrl = "PUT /ISAPI/AccessControl/UserInfo/Delete?format=json";
            IntPtr ptrURL = Marshal.StringToHGlobalAnsi(sUrl);
            struInput.dwSize = (uint)Marshal.SizeOf(struInput);
            struInput.lpRequestUrl = ptrURL;
            struInput.dwRequestUrlLen = (uint)sUrl.Length;
            //delete timeout set 60s
            struInput.dwRecvTimeOut = 60000;

            CUserInfoDelCond JsonUserInfoDelCond = new CUserInfoDelCond();
            JsonUserInfoDelCond.UserInfoDelCond = new CUserInfoDelCondContent();
            JsonUserInfoDelCond.UserInfoDelCond.EmployeeNoList = new List<CEmployeeNoList>();

            if (!m_bDeleteAll)
            {
                foreach (ListViewItem item in this.listViewEmployeeNo.Items)
                {
                    if (item.SubItems[1].Text != "")
                    {
                        CEmployeeNoList singleEmployeeNoList = new CEmployeeNoList();
                        singleEmployeeNoList.employeeNo = item.SubItems[1].Text;
                        JsonUserInfoDelCond.UserInfoDelCond.EmployeeNoList.Add(singleEmployeeNoList);
                    }
                }
            }

            string strUserInfoDelCond = JsonConvert.SerializeObject(JsonUserInfoDelCond);

            IntPtr ptrInBuffer = Marshal.StringToHGlobalAnsi(strUserInfoDelCond);

            struInput.lpInBuffer = ptrInBuffer;
            struInput.dwInBufferSize = (uint)strUserInfoDelCond.Length;

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
                string strTemp = string.Format("Delete User Info Fail,Error Code={0}", CHCNetSDK.NET_DVR_GetLastError());
                listItem.SubItems.Add(strTemp);
                this.AddList(listViewMessage, listItem);
            }
            else
            {
                ListViewItem listItem = new ListViewItem();
                listItem.Text = "Succ";
                string strTemp = string.Format("Delete User Info Success");
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

        private void buttonBatchAdding_Click(object sender, EventArgs e)
        {
            int iAddUserNumber = 0;

            if(!int.TryParse(textBoxAddUserNumber.Text, out iAddUserNumber))
            {
                MessageBox.Show("add user number should be the number!");
                return;
            }

           
            for (int i = 0; i < iAddUserNumber; i++)
            {
                listViewStaffManage.BeginUpdate();

                ListViewItem listItem = new ListViewItem();
                listItem.Text = (m_iUserCount + 1).ToString();
                if (iAddUserNumber > 1)
                {
                    listItem.SubItems.Add(textBoxEmployeeNo.Text + i);
                    listItem.SubItems.Add(textBoxName.Text + i);
                }
                else
                {
                    listItem.SubItems.Add(textBoxEmployeeNo.Text);
                    listItem.SubItems.Add(textBoxName.Text);
                }
                listItem.SubItems.Add(comboBoxUserType.Text);
                listItem.SubItems.Add(checkBoxCloseDelayEnabled.Checked.ToString());
                if (checkBoxTimeEnable.Checked)
                {
                    listItem.SubItems.Add(textBoxBeginTime.Text);
                    listItem.SubItems.Add(textBoxEndTime.Text);
                }
                else
                {
                    listItem.SubItems.Add("");
                    listItem.SubItems.Add("");
                }
                listItem.SubItems.Add(textBoxbelongGroup.Text);
                listItem.SubItems.Add(textBoxPassword.Text);
                listItem.SubItems.Add(textBoxDoorRight.Text);
                string sRightPlan = "";
                foreach (ListViewItem item in this.listViewRightPlan.Items)
                {
                    if (item.SubItems[1].Text != "")
                    {
                        sRightPlan = sRightPlan + "Door" + item.SubItems[0].Text + ":" + item.SubItems[1].Text + "-";
                    }
                }
                listItem.SubItems.Add(sRightPlan);
                listItem.SubItems.Add(textBoxMaxOpenDoorTime.Text);
                listItem.SubItems.Add(textBoxRoomNumber.Text);
                listItem.SubItems.Add(textBoxFloorNumber.Text);

                listItem.SubItems.Add(checkBoxDoubleLockRight.Checked.ToString());
                listItem.SubItems.Add(checkBoxAlwaysOpenRight.Checked.ToString());
                listItem.SubItems.Add(checkBoxLocalUIRight.Checked.ToString());

                listItem.SubItems.Add(comboBoxUserVerifyMode.Text);
                if (checkBoxTimeEnable.Checked)
                {
                    listItem.SubItems.Add(comboBoxTimeType.Text);
                }
                else
                {
                    listItem.SubItems.Add("");
                }
                listItem.SubItems.Add(textBoxOpenDoorTime.Text);

                listViewStaffManage.Items.Add(listItem);
                listViewStaffManage.EndUpdate();

                m_iUserCount += 1;
            }
            
            //foreach (ListViewItem item in this.listViewRightPlan.Items)
            //{
            //    for (int i = 0; i < item.SubItems.Count; i++)
            //    {
            //        if (item.SubItems[i].Text != "")
            //        {
            //            MessageBox.Show(item.SubItems[i].Text);
            //        }
            //    }
            //}
        }

        //add user info
        private void buttonSend_Click(object sender, EventArgs e)
        {
            m_iUserInfoRecordIndex = 0;
            if (-1 != m_lUserInfoRecordHandle)
            {
                if (CHCNetSDK.NET_DVR_StopRemoteConfig(m_lUserInfoRecordHandle))
                {
                    m_lUserInfoRecordHandle = -1;
                }
            }

            if (m_iUserCount < 1)
            {
                MessageBox.Show("Please add some users!");
                return;
            }

            g_fUserInfoRecordCallback = new CHCNetSDK.RemoteConfigCallback(ProcessUserInfoRecordCallback);
            string sURL = "POST /ISAPI/AccessControl/UserInfo/Record?format=json";
            IntPtr ptrURL = Marshal.StringToHGlobalAnsi(sURL);

            m_lUserInfoRecordHandle = CHCNetSDK.NET_DVR_StartRemoteConfig(m_lUserID, CHCNetSDK.NET_DVR_JSON_CONFIG, ptrURL, sURL.Length, g_fUserInfoRecordCallback, this.Handle);
            if (-1 == m_lUserInfoRecordHandle)
            {
                ListViewItem listItem = new ListViewItem();
                listItem.Text = "Fail";
                string strTemp = string.Format("User Info Record Fail,Error Code={0}", CHCNetSDK.NET_DVR_GetLastError());
                listItem.SubItems.Add(strTemp);
                this.AddList(listViewMessage, listItem);
                Marshal.FreeHGlobal(ptrURL);
                return;
            }
            else
            {
                ListViewItem listItem = new ListViewItem();
                listItem.Text = "Succ";
                listItem.SubItems.Add("User Info Record Success");
                this.AddList(listViewMessage, listItem);
            }
            Marshal.FreeHGlobal(ptrURL);
            if (!SendUserInfo())
            {
                CHCNetSDK.NET_DVR_StopRemoteConfig(m_lUserInfoRecordHandle);
                m_lUserInfoRecordHandle = -1;
            }
        }

        private void listViewRightPlan_ItemSelectionChanged(object sender, ListViewItemSelectionChangedEventArgs e)
        {
            if (listViewRightPlan.SelectedItems.Count > 0)
            {
                textBoxPlanTemplateNo.Text = e.Item.SubItems[1].Text;
            }
        }

        private void buttonUpdate_Click(object sender, EventArgs e)
        {
            if (listViewRightPlan.SelectedItems.Count > 0)
            {
                listViewRightPlan.SelectedItems[0].SubItems[1].Text = textBoxPlanTemplateNo.Text;
            }
        }

        //modify user info
        private void buttonModify_Click(object sender, EventArgs e)
        {
            m_iUserInfoRecordIndex = 0;
            if (-1 != m_lUserInfoRecordHandle)
            {
                if (CHCNetSDK.NET_DVR_StopRemoteConfig(m_lUserInfoRecordHandle))
                {
                    m_lUserInfoRecordHandle = -1;
                }
            }

            if (m_iUserCount < 1)
            {
                MessageBox.Show("Please add some users!");
                return;
            }

            g_fUserInfoModifyCallback = new CHCNetSDK.RemoteConfigCallback(ProcessUserInfoRecordCallback);
            string sURL = "PUT /ISAPI/AccessControl/UserInfo/Modify?format=json";
            IntPtr ptrURL = Marshal.StringToHGlobalAnsi(sURL);

            m_lUserInfoRecordHandle = CHCNetSDK.NET_DVR_StartRemoteConfig(m_lUserID, CHCNetSDK.NET_DVR_JSON_CONFIG, ptrURL, sURL.Length, g_fUserInfoModifyCallback, this.Handle);
            if (-1 == m_lUserInfoRecordHandle)
            {
                ListViewItem listItem = new ListViewItem();
                listItem.Text = "Fail";
                string strTemp = string.Format("User Info Modify Fail,Error Code={0}", CHCNetSDK.NET_DVR_GetLastError());
                listItem.SubItems.Add(strTemp);
                this.AddList(listViewMessage, listItem);
                Marshal.FreeHGlobal(ptrURL);
                return;
            }
            else
            {
                ListViewItem listItem = new ListViewItem();
                listItem.Text = "Succ";
                listItem.SubItems.Add("User Info Modify Success");
                this.AddList(listViewMessage, listItem);
            }
            Marshal.FreeHGlobal(ptrURL);
            if (!SendUserInfo())
            {
                CHCNetSDK.NET_DVR_StopRemoteConfig(m_lUserInfoRecordHandle);
                m_lUserInfoRecordHandle = -1;
            }
        }

        //search user info
        private void buttonSearchAll_Click(object sender, EventArgs e)
        {
            m_iUserCount = 0;
            m_iSearchPosition = 0;
            listViewStaffManage.Items.Clear();

            m_bSearchAll = true;

            if (-1 != m_lUserInfoSearchHandle)
            {
                if (CHCNetSDK.NET_DVR_StopRemoteConfig(m_lUserInfoSearchHandle))
                {
                    m_lUserInfoSearchHandle = -1;
                }
            }

            g_fUserInfoSearchCallback = new CHCNetSDK.RemoteConfigCallback(ProcessUserInfoSearchCallback);
            string sURL = "POST /ISAPI/AccessControl/UserInfo/Search?format=json";
            IntPtr ptrURL = Marshal.StringToHGlobalAnsi(sURL);

            m_lUserInfoSearchHandle = CHCNetSDK.NET_DVR_StartRemoteConfig(m_lUserID, CHCNetSDK.NET_DVR_JSON_CONFIG, ptrURL, sURL.Length, g_fUserInfoSearchCallback, this.Handle);
            if (-1 == m_lUserInfoSearchHandle)
            {
                ListViewItem listItem = new ListViewItem();
                listItem.Text = "Fail";
                string strTemp = string.Format("User Info Search Fail,Error Code={0}", CHCNetSDK.NET_DVR_GetLastError());
                listItem.SubItems.Add(strTemp);
                this.AddList(listViewMessage, listItem);
                Marshal.FreeHGlobal(ptrURL);
                return;
            }
            else
            {
                ListViewItem listItem = new ListViewItem();
                listItem.Text = "Succ";
                listItem.SubItems.Add("User Info Search Success");
                this.AddList(listViewMessage, listItem);
            }
            Marshal.FreeHGlobal(ptrURL);
            if (!SearchUserInfo())
            {
                CHCNetSDK.NET_DVR_StopRemoteConfig(m_lUserInfoSearchHandle);
                m_lUserInfoSearchHandle = -1;
            }
        }

        //search user info by employee no
        private void buttonSearchByEmployeeNo_Click(object sender, EventArgs e)
        {
            m_iUserCount = 0;
            m_iSearchPosition = 0;
            listViewStaffManage.Items.Clear();

            m_bSearchAll = false;

            if (-1 != m_lUserInfoSearchHandle)
            {
                if (CHCNetSDK.NET_DVR_StopRemoteConfig(m_lUserInfoSearchHandle))
                {
                    m_lUserInfoSearchHandle = -1;
                }
            }

            g_fUserInfoSearchCallback = new CHCNetSDK.RemoteConfigCallback(ProcessUserInfoSearchCallback);
            string sURL = "POST /ISAPI/AccessControl/UserInfo/Search?format=json";
            IntPtr ptrURL = Marshal.StringToHGlobalAnsi(sURL);

            m_lUserInfoSearchHandle = CHCNetSDK.NET_DVR_StartRemoteConfig(m_lUserID, CHCNetSDK.NET_DVR_JSON_CONFIG, ptrURL, sURL.Length, g_fUserInfoSearchCallback, this.Handle);
            if (-1 == m_lUserInfoSearchHandle)
            {
                ListViewItem listItem = new ListViewItem();
                listItem.Text = "Fail";
                string strTemp = string.Format("User Info Search Fail,Error Code={0}", CHCNetSDK.NET_DVR_GetLastError());
                listItem.SubItems.Add(strTemp);
                this.AddList(listViewMessage, listItem);
                Marshal.FreeHGlobal(ptrURL);
                return;
            }
            else
            {
                ListViewItem listItem = new ListViewItem();
                listItem.Text = "Succ";
                listItem.SubItems.Add("User Info Search Success");
                this.AddList(listViewMessage, listItem);
            }
            Marshal.FreeHGlobal(ptrURL);
            if (!SearchUserInfo())
            {
                CHCNetSDK.NET_DVR_StopRemoteConfig(m_lUserInfoSearchHandle);
                m_lUserInfoSearchHandle = -1;
            }
        }

        private void buttonUpdateEmployeeNo_Click(object sender, EventArgs e)
        {
            if (listViewEmployeeNo.SelectedItems.Count > 0)
            {
                listViewEmployeeNo.SelectedItems[0].SubItems[1].Text = textBoxEmployeeNo2.Text;
            }
        }

        private void listViewEmployeeNo_ItemSelectionChanged(object sender, ListViewItemSelectionChangedEventArgs e)
        {
            if (listViewEmployeeNo.SelectedItems.Count > 0)
            {
                textBoxEmployeeNo2.Text = e.Item.SubItems[1].Text;
            }
        }

        private void buttonClearList_Click(object sender, EventArgs e)
        {
            m_iUserCount = 0;
            listViewStaffManage.Items.Clear();
        }

        //delete user info
        private void buttonDeleteAll_Click(object sender, EventArgs e)
        {
            m_bDeleteAll = true;

            DeleteUserInfo();
        }

        //delete user info by employee no
        private void buttonDeleteByEmployeeNo_Click(object sender, EventArgs e)
        {
            m_bDeleteAll = false;

            DeleteUserInfo();
        }

        //get user info count
        private void buttonGetCount_Click(object sender, EventArgs e)
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

            string sUrl = "GET /ISAPI/AccessControl/UserInfo/Count?format=json";
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
                string strTemp = string.Format("Get User Info Count Fail,Error Code={0}", CHCNetSDK.NET_DVR_GetLastError());
                listItem.SubItems.Add(strTemp);
                this.AddList(listViewMessage, listItem);
            }
            else
            {
                ListViewItem listItem = new ListViewItem();
                listItem.Text = "Succ";
                string strTemp = string.Format("Get User Info Count Success");
                listItem.SubItems.Add(strTemp);
                this.AddList(listViewMessage, listItem);

                string strUserInfoCount = Marshal.PtrToStringAnsi(struOuput.lpOutBuffer);
                CUserInfoCount JsonUserInfoCount = new CUserInfoCount();
                JsonUserInfoCount = JsonConvert.DeserializeObject<CUserInfoCount>(strUserInfoCount);
                int iUserNumber = JsonUserInfoCount.UserInfoCount.userNumber;
                textBoxUserInfoCount.Text = iUserNumber.ToString();
            }

            Marshal.FreeHGlobal(ptrOutBuf);
            Marshal.FreeHGlobal(ptrStatusBuffer);
            Marshal.FreeHGlobal(ptrInput);
            Marshal.FreeHGlobal(ptrOuput);
            Marshal.FreeHGlobal(ptrURL);
        }

        private void buttonUpdateList_Click(object sender, EventArgs e)
        {
            if (listViewStaffManage.SelectedItems.Count > 0)
            {
                listViewStaffManage.SelectedItems[0].SubItems[1].Text = textBoxEmployeeNo.Text;
                listViewStaffManage.SelectedItems[0].SubItems[2].Text = textBoxName.Text;
                listViewStaffManage.SelectedItems[0].SubItems[3].Text = comboBoxUserType.Text;
                listViewStaffManage.SelectedItems[0].SubItems[4].Text = checkBoxCloseDelayEnabled.Checked.ToString();
                if (checkBoxTimeEnable.Checked)
                {
                    listViewStaffManage.SelectedItems[0].SubItems[5].Text = textBoxBeginTime.Text;
                    listViewStaffManage.SelectedItems[0].SubItems[6].Text = textBoxEndTime.Text;
                }
                else
                {
                    listViewStaffManage.SelectedItems[0].SubItems[5].Text = "";
                    listViewStaffManage.SelectedItems[0].SubItems[6].Text = "";
                }
                listViewStaffManage.SelectedItems[0].SubItems[7].Text = textBoxbelongGroup.Text;
                listViewStaffManage.SelectedItems[0].SubItems[8].Text = textBoxPassword.Text;
                listViewStaffManage.SelectedItems[0].SubItems[9].Text = textBoxDoorRight.Text;
                string sRightPlan = "";
                foreach (ListViewItem item in this.listViewRightPlan.Items)
                {
                    if (item.SubItems[1].Text != "")
                    {
                        sRightPlan = sRightPlan + "Door" + item.SubItems[0].Text + ":" + item.SubItems[1].Text + "-";
                    }
                }
                listViewStaffManage.SelectedItems[0].SubItems[10].Text = sRightPlan;
                listViewStaffManage.SelectedItems[0].SubItems[11].Text = textBoxMaxOpenDoorTime.Text;
                listViewStaffManage.SelectedItems[0].SubItems[12].Text = textBoxRoomNumber.Text;
                listViewStaffManage.SelectedItems[0].SubItems[13].Text = textBoxFloorNumber.Text;
                listViewStaffManage.SelectedItems[0].SubItems[14].Text = checkBoxDoubleLockRight.Checked.ToString();
                listViewStaffManage.SelectedItems[0].SubItems[15].Text = checkBoxAlwaysOpenRight.Checked.ToString();
                listViewStaffManage.SelectedItems[0].SubItems[16].Text = checkBoxLocalUIRight.Checked.ToString();
                listViewStaffManage.SelectedItems[0].SubItems[17].Text = comboBoxUserVerifyMode.Text;
                if (checkBoxTimeEnable.Checked)
                {
                    listViewStaffManage.SelectedItems[0].SubItems[18].Text = comboBoxTimeType.Text;
                }
                else
                {
                    listViewStaffManage.SelectedItems[0].SubItems[18].Text = "";
                }
                listViewStaffManage.SelectedItems[0].SubItems[19].Text = textBoxOpenDoorTime.Text;
            }
        }

        private void listViewStaffManage_ItemSelectionChanged(object sender, ListViewItemSelectionChangedEventArgs e)
        {
            if (listViewStaffManage.SelectedItems.Count > 0)
            {
                textBoxEmployeeNo.Text = e.Item.SubItems[1].Text;
                textBoxName.Text = e.Item.SubItems[2].Text;
                comboBoxUserType.Text = e.Item.SubItems[3].Text;
                if (e.Item.SubItems[4].Text == "True")
                {
                    checkBoxCloseDelayEnabled.Checked = true;
                }
                else if (e.Item.SubItems[4].Text == "False")
                {
                    checkBoxCloseDelayEnabled.Checked = false;
                }
                if ((e.Item.SubItems[5].Text == "") && (e.Item.SubItems[6].Text == ""))
                {
                    checkBoxTimeEnable.Checked = false;
                }
                else
                {
                    checkBoxTimeEnable.Checked = true;
                }
                textBoxBeginTime.Text = e.Item.SubItems[5].Text;
                textBoxEndTime.Text = e.Item.SubItems[6].Text;
                textBoxbelongGroup.Text = e.Item.SubItems[7].Text;
                textBoxPassword.Text = e.Item.SubItems[8].Text;
                textBoxDoorRight.Text = e.Item.SubItems[9].Text;

                foreach (ListViewItem item in this.listViewRightPlan.Items)
                {
                    item.SubItems[1].Text = "";
                }
                string sRightPlan = e.Item.SubItems[10].Text;
                int iDoorNoBegin = 0;
                int iDoorNoEnd = 0;
                int iPlanTemplateBegin = 0;
                int iPlanTemplateEnd = 0;
                for (int iCurrentLocate = 0; iCurrentLocate < sRightPlan.Length - 1;)
                {
                    if (sRightPlan.IndexOf("Door",iCurrentLocate) != -1)
                    {
                        iDoorNoBegin = sRightPlan.IndexOf("Door",iCurrentLocate) + 4;
                        iDoorNoEnd = sRightPlan.IndexOf(":",iCurrentLocate) - 1;
                        iPlanTemplateBegin = sRightPlan.IndexOf(":",iCurrentLocate) + 1;
                        iPlanTemplateEnd = sRightPlan.IndexOf("-",iCurrentLocate) - 1;
                        iCurrentLocate = sRightPlan.IndexOf("-",iCurrentLocate) + 1;
                        int iDoorNo = 0;
                        int.TryParse(sRightPlan.Substring(iDoorNoBegin, (iDoorNoEnd - iDoorNoBegin + 1)), out iDoorNo);
                        string sPlanTemplate = sRightPlan.Substring(iPlanTemplateBegin, (iPlanTemplateEnd - iPlanTemplateBegin + 1));
                        listViewRightPlan.Items[iDoorNo - 1].SubItems[1].Text = sPlanTemplate;
                    }
                    else
                    {
                        break;
                    }
                }

                textBoxMaxOpenDoorTime.Text = e.Item.SubItems[11].Text;
                textBoxRoomNumber.Text = e.Item.SubItems[12].Text;
                textBoxFloorNumber.Text = e.Item.SubItems[13].Text;
                if (e.Item.SubItems[14].Text == "True")
                {
                    checkBoxDoubleLockRight.Checked = true;
                }
                else if (e.Item.SubItems[14].Text == "False")
                {
                    checkBoxDoubleLockRight.Checked = false;
                }


                if (e.Item.SubItems[15].Text == "True")
                {
                    checkBoxAlwaysOpenRight.Checked = true;
                }
                else if (e.Item.SubItems[15].Text == "False")
                {
                    checkBoxAlwaysOpenRight.Checked = false;
                }

                if (e.Item.SubItems[16].Text == "True")
                {
                    checkBoxLocalUIRight.Checked = true;
                }
                else if (e.Item.SubItems[16].Text == "False")
                {
                    checkBoxLocalUIRight.Checked = false;
                }

                comboBoxUserVerifyMode.Text = e.Item.SubItems[17].Text;
                comboBoxTimeType.Text = e.Item.SubItems[18].Text;
                textBoxOpenDoorTime.Text = e.Item.SubItems[19].Text;
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
                    if (-1 != m_lUserInfoRecordHandle)
                    {
                        if (CHCNetSDK.NET_DVR_StopRemoteConfig(m_lUserInfoRecordHandle))
                        {
                            m_lUserInfoRecordHandle = -1;
                        }
                    }
                    break;
                case 1002:
                    SendUserInfo();
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
                    if (-1 != m_lUserInfoSearchHandle)
                    {
                        if (CHCNetSDK.NET_DVR_StopRemoteConfig(m_lUserInfoSearchHandle))
                        {
                            m_lUserInfoSearchHandle = -1;
                        }
                    }
                    break;
                case 1005:
                    SearchUserInfo();
                    break;
                case 1006:
                    for (int i = 0; i < m_JsonUserInfoSearch.UserInfoSearch.numOfMatches; i++)
                    {
                        listViewStaffManage.BeginUpdate();

                        ListViewItem listItem = new ListViewItem();
                        listItem.Text = (m_iUserCount + 1).ToString();
                        listItem.SubItems.Add(m_JsonUserInfoSearch.UserInfoSearch.UserInfo[i].employeeNo.ToString());
                        if (m_JsonUserInfoSearch.UserInfoSearch.UserInfo[i].name != null)
                        {
                            listItem.SubItems.Add(m_JsonUserInfoSearch.UserInfoSearch.UserInfo[i].name);
                        }
                        else
                        {
                            listItem.SubItems.Add("");
                        }
                        if (m_JsonUserInfoSearch.UserInfoSearch.UserInfo[i].userType != null)
                        {
                            listItem.SubItems.Add(m_JsonUserInfoSearch.UserInfoSearch.UserInfo[i].userType);
                        }
                        else
                        {
                            listItem.SubItems.Add("");
                        }
                        if (m_JsonUserInfoSearch.UserInfoSearch.UserInfo[i].closeDelayEnabled == true)
                        {
                            listItem.SubItems.Add("True");
                        }
                        else
                        {
                            listItem.SubItems.Add("False");
                        }
                        if ((m_JsonUserInfoSearch.UserInfoSearch.UserInfo[i].Valid == null))
                        {
                            listItem.SubItems.Add("");
                            listItem.SubItems.Add("");
                        }
                        else if (m_JsonUserInfoSearch.UserInfoSearch.UserInfo[i].Valid.enable == true)
                        {
                            if (m_JsonUserInfoSearch.UserInfoSearch.UserInfo[i].Valid.beginTime != null)
                            {
                                listItem.SubItems.Add(m_JsonUserInfoSearch.UserInfoSearch.UserInfo[i].Valid.beginTime);
                            }
                            else
                            {
                                listItem.SubItems.Add("");
                            }
                            if (m_JsonUserInfoSearch.UserInfoSearch.UserInfo[i].Valid.endTime != null)
                            {
                                listItem.SubItems.Add(m_JsonUserInfoSearch.UserInfoSearch.UserInfo[i].Valid.endTime);
                            }
                            else
                            {
                                listItem.SubItems.Add("");
                            }
                        }
                        else
                        {
                            listItem.SubItems.Add("");
                            listItem.SubItems.Add("");
                        }
                        if (m_JsonUserInfoSearch.UserInfoSearch.UserInfo[i].belongGroup != null)
                        {
                            listItem.SubItems.Add(m_JsonUserInfoSearch.UserInfoSearch.UserInfo[i].belongGroup);
                        }
                        else
                        {
                            listItem.SubItems.Add("");
                        }
                        if (m_JsonUserInfoSearch.UserInfoSearch.UserInfo[i].password != null)
                        {
                            listItem.SubItems.Add(m_JsonUserInfoSearch.UserInfoSearch.UserInfo[i].password);
                        }
                        else
                        {
                            listItem.SubItems.Add("");
                        }
                        if (m_JsonUserInfoSearch.UserInfoSearch.UserInfo[i].doorRight != null)
                        {
                            listItem.SubItems.Add(m_JsonUserInfoSearch.UserInfoSearch.UserInfo[i].doorRight);
                        }
                        else
                        {
                            listItem.SubItems.Add("");
                        }
                        if (m_JsonUserInfoSearch.UserInfoSearch.UserInfo[i].RightPlan != null)
                        {
                            string sRightPlan = "";
                            for (int j = 0; j < m_JsonUserInfoSearch.UserInfoSearch.UserInfo[i].RightPlan.Count; j++)
                            {
                                sRightPlan = sRightPlan + "Door" + m_JsonUserInfoSearch.UserInfoSearch.UserInfo[i].RightPlan[j].doorNo + ":" + m_JsonUserInfoSearch.UserInfoSearch.UserInfo[i].RightPlan[j].planTemplateNo + "-";
                            }
                            listItem.SubItems.Add(sRightPlan);
                        }
                        else
                        {
                            listItem.SubItems.Add("");
                        }
                        listItem.SubItems.Add(m_JsonUserInfoSearch.UserInfoSearch.UserInfo[i].maxOpenDoorTime.ToString());
                        listItem.SubItems.Add(m_JsonUserInfoSearch.UserInfoSearch.UserInfo[i].roomNumber.ToString());
                        listItem.SubItems.Add(m_JsonUserInfoSearch.UserInfoSearch.UserInfo[i].floorNumber.ToString());
                        if (m_JsonUserInfoSearch.UserInfoSearch.UserInfo[i].doubleLockRight == true)
                        {
                            listItem.SubItems.Add("True");
                        }
                        else
                        {
                            listItem.SubItems.Add("False");
                        }
                        if (m_JsonUserInfoSearch.UserInfoSearch.UserInfo[i].alwaysOpenRight == true)
                        {
                            listItem.SubItems.Add("True");
                        }
                        else
                        {
                            listItem.SubItems.Add("False");
                        }
                        if (m_JsonUserInfoSearch.UserInfoSearch.UserInfo[i].localUIRight == true)
                        {
                            listItem.SubItems.Add("True");
                        }
                        else
                        {
                            listItem.SubItems.Add("False");
                        }
                        if (m_JsonUserInfoSearch.UserInfoSearch.UserInfo[i].userVerifyMode != null)
                        {
                            listItem.SubItems.Add(m_JsonUserInfoSearch.UserInfoSearch.UserInfo[i].userVerifyMode);
                        }
                        else
                        {
                            listItem.SubItems.Add("");
                        }
                        if ((m_JsonUserInfoSearch.UserInfoSearch.UserInfo[i].Valid == null))
                        {
                            listItem.SubItems.Add("");
                        }
                        else if (m_JsonUserInfoSearch.UserInfoSearch.UserInfo[i].Valid.enable == true)
                        {
                            if (m_JsonUserInfoSearch.UserInfoSearch.UserInfo[i].Valid.timeType != null)
                            {
                                listItem.SubItems.Add(m_JsonUserInfoSearch.UserInfoSearch.UserInfo[i].Valid.timeType);
                            }
                            else
                            {
                                listItem.SubItems.Add("");
                            }
                        }
                        else
                        {
                            listItem.SubItems.Add("");
                        }
                        listItem.SubItems.Add(m_JsonUserInfoSearch.UserInfoSearch.UserInfo[i].openDoorTime.ToString());

                        listViewStaffManage.Items.Add(listItem);
                        listViewStaffManage.EndUpdate();

                        m_iUserCount += 1;
                    }
                    break;
                case 1007:
                    int iErrorMsg2 = m.WParam.ToInt32();
                    if (iErrorMsg2 == 1)
                    {
                        ListViewItem listItem = new ListViewItem();
                        listItem.Text = "Fail";
                        string strTemp = string.Format("NET_SDK_CALLBACK_STATUS_FAILED");
                        listItem.SubItems.Add(strTemp);
                        this.AddList(listViewMessage, listItem);

                        if (-1 != m_lUserInfoDeleteHandle)
                        {
                            if (CHCNetSDK.NET_DVR_StopRemoteConfig(m_lUserInfoDeleteHandle))
                            {
                                m_lUserInfoDeleteHandle = -1;
                            }
                        }
                    }
                    else if (iErrorMsg2 == 2)
                    {
                        ListViewItem listItem = new ListViewItem();
                        listItem.Text = "Fail";
                        string strTemp = string.Format("NET_SDK_CALLBACK_STATUS_EXCEPTION");
                        listItem.SubItems.Add(strTemp);
                        this.AddList(listViewMessage, listItem);

                        if (-1 != m_lUserInfoDeleteHandle)
                        {
                            if (CHCNetSDK.NET_DVR_StopRemoteConfig(m_lUserInfoDeleteHandle))
                            {
                                m_lUserInfoDeleteHandle = -1;
                            }
                        }
                    }
                    else if (iErrorMsg2 == 3)
                    {
                        ListViewItem listItem = new ListViewItem();
                        listItem.Text = "Fail";
                        string strTemp = string.Format("NET_SDK_CALLBACK_OTHER_ERROR");
                        listItem.SubItems.Add(strTemp);
                        this.AddList(listViewMessage, listItem);

                        if (-1 != m_lUserInfoDeleteHandle)
                        {
                            if (CHCNetSDK.NET_DVR_StopRemoteConfig(m_lUserInfoDeleteHandle))
                            {
                                m_lUserInfoDeleteHandle = -1;
                            }
                        }
                    }
                    else if (iErrorMsg2 == 4)
                    {
                        ListViewItem listItem = new ListViewItem();
                        listItem.Text = "Succ";
                        string strTemp = string.Format("Delete Processing...");
                        listItem.SubItems.Add(strTemp);
                        this.AddList(listViewMessage, listItem);
                    }
                    else if (iErrorMsg2 == 5)
                    {
                        ListViewItem listItem = new ListViewItem();
                        listItem.Text = "Succ";
                        string strTemp = string.Format("Delete Success...");
                        listItem.SubItems.Add(strTemp);
                        this.AddList(listViewMessage, listItem);

                        if (-1 != m_lUserInfoDeleteHandle)
                        {
                            if (CHCNetSDK.NET_DVR_StopRemoteConfig(m_lUserInfoDeleteHandle))
                            {
                                m_lUserInfoDeleteHandle = -1;
                            }
                        }
                    }
                    else if (iErrorMsg2 == 6)
                    {
                        ListViewItem listItem = new ListViewItem();
                        listItem.Text = "Fail";
                        string strTemp = string.Format("Delete Failed...");
                        listItem.SubItems.Add(strTemp);
                        this.AddList(listViewMessage, listItem);

                        if (-1 != m_lUserInfoDeleteHandle)
                        {
                            if (CHCNetSDK.NET_DVR_StopRemoteConfig(m_lUserInfoDeleteHandle))
                            {
                                m_lUserInfoDeleteHandle = -1;
                            }
                        }
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
            dlg.m_command = Public.SynInfoCommand.USER_INFO_SYN;
            dlg.m_lUserID = m_lUserID;
            dlg.ShowDialog();
        }

        private void buttonDeleteAll2_Click(object sender, EventArgs e)
        {
            m_bDeleteAll = true;

            DeleteUserInfo2();
        }

        private void buttonDeleteByEmployeeNo2_Click(object sender, EventArgs e)
        {
            m_bDeleteAll = false;

            DeleteUserInfo2();
        }

        private void DeleteUserInfo2()
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

            string sUrl = "PUT /ISAPI/AccessControl/UserInfoDetail/Delete?format=json";
            IntPtr ptrURL = Marshal.StringToHGlobalAnsi(sUrl);
            struInput.dwSize = (uint)Marshal.SizeOf(struInput);
            struInput.lpRequestUrl = ptrURL;
            struInput.dwRequestUrlLen = (uint)sUrl.Length;

            CUserInfoDetail JsonUserInfoDel = new CUserInfoDetail();
            JsonUserInfoDel.UserInfoDetail = new CUserInfoDelContent();
            JsonUserInfoDel.UserInfoDetail.EmployeeNoList = new List<CEmployeeNoList>();

            if (!m_bDeleteAll)
            {
                JsonUserInfoDel.UserInfoDetail.mode = "byEmployeeNo";
                foreach (ListViewItem item in this.listViewEmployeeNo.Items)
                {
                    if (item.SubItems[1].Text != "")
                    {
                        CEmployeeNoList singleEmployeeNoList = new CEmployeeNoList();
                        singleEmployeeNoList.employeeNo = item.SubItems[1].Text;
                        JsonUserInfoDel.UserInfoDetail.EmployeeNoList.Add(singleEmployeeNoList);
                    }
                }
            }
            else
            {
                JsonUserInfoDel.UserInfoDetail.mode = "all";
            }

            string strUserInfoDelCond = JsonConvert.SerializeObject(JsonUserInfoDel);

            IntPtr ptrInBuffer = Marshal.StringToHGlobalAnsi(strUserInfoDelCond);

            struInput.lpInBuffer = ptrInBuffer;
            struInput.dwInBufferSize = (uint)strUserInfoDelCond.Length;

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
                string strTemp = string.Format("Delete User Info Fail,Error Code={0}", CHCNetSDK.NET_DVR_GetLastError());
                listItem.SubItems.Add(strTemp);
                this.AddList(listViewMessage, listItem);

                Marshal.FreeHGlobal(ptrOutBuf);
                Marshal.FreeHGlobal(ptrStatusBuffer);
                Marshal.FreeHGlobal(ptrInBuffer);
                Marshal.FreeHGlobal(ptrInput);
                Marshal.FreeHGlobal(ptrOuput);
                Marshal.FreeHGlobal(ptrURL);

                return;
            }
            else
            {
                ListViewItem listItem = new ListViewItem();
                listItem.Text = "Succ";
                string strTemp = string.Format("Delete User Info Start");
                listItem.SubItems.Add(strTemp);
                this.AddList(listViewMessage, listItem);

                string strResponseStatus = Marshal.PtrToStringAnsi(struOuput.lpOutBuffer);
                CResponseStatus JsonResponseStatus = new CResponseStatus();
                JsonResponseStatus = JsonConvert.DeserializeObject<CResponseStatus>(strResponseStatus);

                Marshal.FreeHGlobal(ptrOutBuf);
                Marshal.FreeHGlobal(ptrStatusBuffer);
                Marshal.FreeHGlobal(ptrInBuffer);
                Marshal.FreeHGlobal(ptrInput);
                Marshal.FreeHGlobal(ptrOuput);
                Marshal.FreeHGlobal(ptrURL);
            }

            if (-1 != m_lUserInfoDeleteHandle)
            {
                if (CHCNetSDK.NET_DVR_StopRemoteConfig(m_lUserInfoDeleteHandle))
                {
                    m_lUserInfoDeleteHandle = -1;
                }
            }

            g_fUserInfoDeleteCallback = new CHCNetSDK.RemoteConfigCallback(ProcessUserInfoDeleteCallback);
            string sURL2 = "GET /ISAPI/AccessControl/UserInfoDetail/DeleteProcess?format=json";
            IntPtr ptrURL2 = Marshal.StringToHGlobalAnsi(sURL2);

            m_lUserInfoDeleteHandle = CHCNetSDK.NET_DVR_StartRemoteConfig(m_lUserID, CHCNetSDK.NET_DVR_JSON_CONFIG, ptrURL2, sURL2.Length, g_fUserInfoDeleteCallback, this.Handle);
            if (-1 == m_lUserInfoDeleteHandle)
            {
                ListViewItem listItem = new ListViewItem();
                listItem.Text = "Fail";
                string strTemp = string.Format("User Info Delete Fail,Error Code={0}", CHCNetSDK.NET_DVR_GetLastError());
                listItem.SubItems.Add(strTemp);
                this.AddList(listViewMessage, listItem);
                Marshal.FreeHGlobal(ptrURL2);
                return;
            }
            else
            {
                ListViewItem listItem = new ListViewItem();
                listItem.Text = "Succ";
                listItem.SubItems.Add("User Info Delete Get Process Start");
                this.AddList(listViewMessage, listItem);
            }
            Marshal.FreeHGlobal(ptrURL2);
        }

        private void StaffManagement_Load(object sender, EventArgs e)
        {
            MultiLanguage.LoadLanguage(this);
        }
    }
}
