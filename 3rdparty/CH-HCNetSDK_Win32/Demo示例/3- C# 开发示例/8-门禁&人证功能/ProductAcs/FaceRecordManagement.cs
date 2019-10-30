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
namespace AcsDemo
{
    public partial class FaceRecordManagement : Form
    {
        public Int32 m_lUserID = -1;
        public int m_iFaceDataRecordCount = 0;
        public bool m_bDeleteAll = true;
        public Int32 m_lFaceDataRecordHandle = -1;
        public int m_iFaceDataRecordIndex = 0;
        public Int32 m_lFaceDataSearchHandle = -1;

        private delegate void AddListCallBack(ListView cListView, ListViewItem cListViewItem);
        private CHCNetSDK.RemoteConfigCallback g_fFaceDataRecordCallback = null;
        private CHCNetSDK.RemoteConfigCallback g_fFaceDataModifyCallback = null;
        private CHCNetSDK.RemoteConfigCallback g_fFaceDataSearchCallback = null;

        CHCNetSDK.NET_DVR_JSON_DATA_CFG m_struJsonDataCfg;
        CFaceDataSearchReturn m_JsonFaceDataSearchReturn;
        String m_strFPID;

        public class CFaceRecordDelete
        {
            public List<CFPID> FPID { get; set; }
        }

        public class CFPID
        {
            public string value { get; set; }
        }

        public class CResponseStatus
        {
            public string requestURL { get; set; }
            public int statusCode { get; set; }
            public string statusString { get; set; }
            public string subStatusCode { get; set; }
            public int errorCode { get; set; }
            public string errorMsg { get; set; }
            public string FPID { get; set; }
        }

        public class CFaceRecordCount
        {
            public string requestURL { get; set; }
            public int statusCode { get; set; }
            public string statusString { get; set; }
            public string subStatusCode { get; set; }
            public int errorCode { get; set; }
            public string errorMsg { get; set; }
            public string FDID { get; set; }
            public string faceLibType { get; set; }
            public int recordDataNumber { get; set; }
        }

        public class CFaceRecordData
        {
            public string faceLibType { get; set; }
            public string FDID { get; set; }
            public string FPID { get; set; }
        }

        public class CFaceRecordModify
        {
            public string faceLibType { get; set; }
            public string FDID { get; set; }
            public string FPID { get; set; }
        }

        public class CFaceDataSearchCond
        {
            public int searchResultPosition { get; set; }
            public int maxResults { get; set; }
            public string faceLibType { get; set; }
            public string FDID { get; set; }
            public string FPID { get; set; }
        }

        public class CFaceDataSearchReturn
        {
            public string requestURL { get; set; }
            public int statusCode { get; set; }
            public string statusString { get; set; }
            public string subStatusCode { get; set; }
            public int errorCode { get; set; }
            public string errorMsg { get; set; }
            public string responseStatusStrg { get; set; }
            public int numOfMatches { get; set; }
            public int totalMatches { get; set; }
            public List<CMatchList> MatchList { get; set; }
        }

        public class CMatchList
        {
            public string FPID { get; set; }
            public int employeeNo { get; set; }
        }

        public FaceRecordManagement()
        {
            InitializeComponent();
            UpdateListFPID();
            this.SuspendLayout();//挂起
            this.columnHeader1.Name = "columnHeader1";
            this.columnHeader2.Name = "columnHeader2";
            this.columnHeader3.Name = "columnHeader3";
            this.columnHeader4.Name = "columnHeader4";
            this.columnHeader5.Name = "columnHeader5";
            this.columnHeader16.Name = "columnHeader16";
            this.columnHeader21.Name = "columnHeader21";
            this.columnHeader22.Name = "columnHeader22";
            this.ResumeLayout();
        }

        private void UpdateListFPID()
        {
            listViewFPID.BeginUpdate();
            listViewFPID.Items.Clear();
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
                listViewFPID.Items.Add(listItem);
            }
            listViewFPID.EndUpdate();
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

        private void ProcessFaceDataRecordCallback(uint dwType, IntPtr lpBuffer, uint dwBufLen, IntPtr pUserData)
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
                string strResponseStatus = Marshal.PtrToStringAnsi(lpBuffer);
                CResponseStatus JsonResponseStatus = new CResponseStatus();
                JsonResponseStatus = JsonConvert.DeserializeObject<CResponseStatus>(strResponseStatus);
                m_strFPID = JsonResponseStatus.FPID;

                CHCNetSDK.PostMessage(pUserData, 1003, JsonResponseStatus.statusCode, 0);

                if ((m_iFaceDataRecordIndex >= m_iFaceDataRecordCount) || (JsonResponseStatus.statusCode != 1))
                {
                    CHCNetSDK.PostMessage(pUserData, 1001, 4, 0);
                    //可以结束了，正常（异常）结束
                    return;
                }
                CHCNetSDK.PostMessage(pUserData, 1002, 0, 0);
                //在主线程中发送下一个用户
            }
            return;
        }

        private void ProcessFaceDataModifyCallback(uint dwType, IntPtr lpBuffer, uint dwBufLen, IntPtr pUserData)
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
                string strResponseStatus = Marshal.PtrToStringAnsi(lpBuffer);
                CResponseStatus JsonResponseStatus = new CResponseStatus();
                JsonResponseStatus = JsonConvert.DeserializeObject<CResponseStatus>(strResponseStatus);

                CHCNetSDK.PostMessage(pUserData, 1006, JsonResponseStatus.statusCode, 0);

                if ((m_iFaceDataRecordIndex >= m_iFaceDataRecordCount) || (JsonResponseStatus.statusCode != 1))
                {
                    CHCNetSDK.PostMessage(pUserData, 1004, 4, 0);
                    //可以结束了，正常(异常)结束
                    return;
                }
                CHCNetSDK.PostMessage(pUserData, 1005, 0, 0);
                //在主线程中发送下一个用户
            }
            return;
        }

        private void ProcessFaceDataSearchCallback(uint dwType, IntPtr lpBuffer, uint dwBufLen, IntPtr pUserData)
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
                //输出上一包解析信息, 1009
                m_struJsonDataCfg = (CHCNetSDK.NET_DVR_JSON_DATA_CFG)Marshal.PtrToStructure(lpBuffer, typeof(CHCNetSDK.NET_DVR_JSON_DATA_CFG));

                string strFaceDataSearchReturn = Marshal.PtrToStringAnsi((IntPtr)m_struJsonDataCfg.lpJsonData, (int)m_struJsonDataCfg.dwJsonDataSize);

                m_JsonFaceDataSearchReturn = JsonConvert.DeserializeObject<CFaceDataSearchReturn>(strFaceDataSearchReturn);

                if ((m_JsonFaceDataSearchReturn.responseStatusStrg != "NO MATCH") && (m_JsonFaceDataSearchReturn.statusCode == 1))
                {
                    CHCNetSDK.PostMessage(pUserData, 1009, 0, 0);
                }

                if (m_iFaceDataRecordIndex >= m_iFaceDataRecordCount)
                {
                    CHCNetSDK.PostMessage(pUserData, 1007, 4, 0);
                    //可以结束了，正常（异常）结束
                    return;
                }
                CHCNetSDK.PostMessage(pUserData, 1008, 0, 0);
                //在主线程中发送下一个用户
            }
            return;
        }

        private bool SendFaceRecord()
        {
            if (-1 == m_lFaceDataRecordHandle)
            {
                return false;
            }

            if (m_iFaceDataRecordIndex >= m_iFaceDataRecordCount)
            {
                return false;
            }

            CFaceRecordData JsonFaceRecordData = new CFaceRecordData();

            JsonFaceRecordData.faceLibType = listViewFaceRecordManage.Items[m_iFaceDataRecordIndex].SubItems[1].Text;
            JsonFaceRecordData.FDID = listViewFaceRecordManage.Items[m_iFaceDataRecordIndex].SubItems[2].Text;
            JsonFaceRecordData.FPID = listViewFaceRecordManage.Items[m_iFaceDataRecordIndex].SubItems[3].Text;

            string strFaceRecordData = JsonConvert.SerializeObject(JsonFaceRecordData);

            IntPtr ptrFaceRecordData = Marshal.StringToHGlobalAnsi(strFaceRecordData);

            CHCNetSDK.NET_DVR_JSON_DATA_CFG struJsonDataCfg = new CHCNetSDK.NET_DVR_JSON_DATA_CFG();
            struJsonDataCfg.dwSize = (uint)Marshal.SizeOf(struJsonDataCfg);
            struJsonDataCfg.lpJsonData = ptrFaceRecordData;
            struJsonDataCfg.dwJsonDataSize = (uint)strFaceRecordData.Length;

            if (!File.Exists(listViewFaceRecordManage.Items[m_iFaceDataRecordIndex].SubItems[4].Text))
            {
                MessageBox.Show("The picture does not exist!");
                Marshal.FreeHGlobal(ptrFaceRecordData);
                return false;
            }
            FileStream fs = new FileStream(listViewFaceRecordManage.Items[m_iFaceDataRecordIndex].SubItems[4].Text, FileMode.OpenOrCreate);
            if (0 == fs.Length)
            {
                MessageBox.Show("The picture is 0k,please input another picture!");
                Marshal.FreeHGlobal(ptrFaceRecordData);
                fs.Close();
                return false;
            }
            if (200 * 1024 < fs.Length)
            {
                MessageBox.Show("The picture is larger than 200k,please input another picture!");
                Marshal.FreeHGlobal(ptrFaceRecordData);
                fs.Close();
                return false;
            }

            struJsonDataCfg.dwPicDataSize = (uint)fs.Length;
            int iLen = (int)struJsonDataCfg.dwPicDataSize;
            byte[] by = new byte[iLen];
            struJsonDataCfg.lpPicData = Marshal.AllocHGlobal(iLen);
            fs.Read(by, 0, iLen);
            Marshal.Copy(by, 0, struJsonDataCfg.lpPicData, iLen);
            fs.Close();

            IntPtr ptrJsonDataCfg = Marshal.AllocHGlobal(Marshal.SizeOf(struJsonDataCfg));
            Marshal.StructureToPtr(struJsonDataCfg, ptrJsonDataCfg, false);

            if (!CHCNetSDK.NET_DVR_SendRemoteConfig(m_lFaceDataRecordHandle, (int)CHCNetSDK.LONG_CFG_SEND_DATA_TYPE_ENUM.ENUM_SEND_JSON_DATA, ptrJsonDataCfg, (uint)Marshal.SizeOf(struJsonDataCfg)))
            {
                ListViewItem listItem = new ListViewItem();
                listItem.Text = "Fail";
                string strTemp = null;
                strTemp = string.Format("Send Fail,FPID:{0}", listViewFaceRecordManage.Items[m_iFaceDataRecordIndex].SubItems[3].Text);
                listItem.SubItems.Add(strTemp);
                this.AddList(listViewMessage, listItem);
                Marshal.FreeHGlobal(ptrFaceRecordData);
                Marshal.FreeHGlobal(struJsonDataCfg.lpPicData);
                Marshal.FreeHGlobal(ptrJsonDataCfg);
                return false;
            }
            ListViewItem listItemSucc = new ListViewItem();
            listItemSucc.Text = "Succ";
            string strTempSucc = null;
            strTempSucc = string.Format("Send Processing,FPID:{0}", listViewFaceRecordManage.Items[m_iFaceDataRecordIndex].SubItems[3].Text);
            listItemSucc.SubItems.Add(strTempSucc);
            this.AddList(listViewMessage, listItemSucc);
            Marshal.FreeHGlobal(ptrFaceRecordData);
            Marshal.FreeHGlobal(struJsonDataCfg.lpPicData);
            Marshal.FreeHGlobal(ptrJsonDataCfg);
            m_iFaceDataRecordIndex++;
            return true;
        }

        private bool SendFaceModify()
        {
            if (-1 == m_lFaceDataRecordHandle)
            {
                return false;
            }

            if (m_iFaceDataRecordIndex >= m_iFaceDataRecordCount)
            {
                return false;
            }

            CFaceRecordModify JsonFaceRecordModify = new CFaceRecordModify();

            JsonFaceRecordModify.faceLibType = listViewFaceRecordManage.Items[m_iFaceDataRecordIndex].SubItems[1].Text;
            JsonFaceRecordModify.FDID = listViewFaceRecordManage.Items[m_iFaceDataRecordIndex].SubItems[2].Text;
            JsonFaceRecordModify.FPID = listViewFaceRecordManage.Items[m_iFaceDataRecordIndex].SubItems[3].Text;

            string strFaceRecordModify = JsonConvert.SerializeObject(JsonFaceRecordModify);

            IntPtr ptrFaceRecordModify = Marshal.StringToHGlobalAnsi(strFaceRecordModify);

            CHCNetSDK.NET_DVR_JSON_DATA_CFG struJsonDataCfg = new CHCNetSDK.NET_DVR_JSON_DATA_CFG();
            struJsonDataCfg.dwSize = (uint)Marshal.SizeOf(struJsonDataCfg);
            struJsonDataCfg.lpJsonData = ptrFaceRecordModify;
            struJsonDataCfg.dwJsonDataSize = (uint)strFaceRecordModify.Length;

            if (!File.Exists(listViewFaceRecordManage.Items[m_iFaceDataRecordIndex].SubItems[4].Text))
            {
                MessageBox.Show("The picture does not exist!");
                Marshal.FreeHGlobal(ptrFaceRecordModify);
                return false;
            }
            FileStream fs = new FileStream(listViewFaceRecordManage.Items[m_iFaceDataRecordIndex].SubItems[4].Text, FileMode.OpenOrCreate);
            if (0 == fs.Length)
            {
                MessageBox.Show("The picture is 0k,please input another picture!");
                Marshal.FreeHGlobal(ptrFaceRecordModify);
                fs.Close();
                return false;
            }
            if (200 * 1024 < fs.Length)
            {
                MessageBox.Show("The picture is larger than 200k,please input another picture!");
                Marshal.FreeHGlobal(ptrFaceRecordModify);
                fs.Close();
                return false;
            }

            struJsonDataCfg.dwPicDataSize = (uint)fs.Length;
            int iLen = (int)struJsonDataCfg.dwPicDataSize;
            byte[] by = new byte[iLen];
            struJsonDataCfg.lpPicData = Marshal.AllocHGlobal(iLen);
            fs.Read(by, 0, iLen);
            Marshal.Copy(by, 0, struJsonDataCfg.lpPicData, iLen);
            fs.Close();

            IntPtr ptrJsonDataCfg = Marshal.AllocHGlobal(Marshal.SizeOf(struJsonDataCfg));
            Marshal.StructureToPtr(struJsonDataCfg, ptrJsonDataCfg, false);

            if (!CHCNetSDK.NET_DVR_SendRemoteConfig(m_lFaceDataRecordHandle, (int)CHCNetSDK.LONG_CFG_SEND_DATA_TYPE_ENUM.ENUM_SEND_JSON_DATA, ptrJsonDataCfg, (uint)Marshal.SizeOf(struJsonDataCfg)))
            {
                ListViewItem listItem = new ListViewItem();
                listItem.Text = "Fail";
                string strTemp = null;
                strTemp = string.Format("Send Fail,FPID:{0}", listViewFaceRecordManage.Items[m_iFaceDataRecordIndex].SubItems[3].Text);
                listItem.SubItems.Add(strTemp);
                this.AddList(listViewMessage, listItem);
                Marshal.FreeHGlobal(ptrFaceRecordModify);
                Marshal.FreeHGlobal(struJsonDataCfg.lpPicData);
                Marshal.FreeHGlobal(ptrJsonDataCfg);
                return false;
            }
            ListViewItem listItemSucc = new ListViewItem();
            listItemSucc.Text = "Succ";
            string strTempSucc = null;
            strTempSucc = string.Format("Send Processing,FPID:{0}", listViewFaceRecordManage.Items[m_iFaceDataRecordIndex].SubItems[3].Text);
            listItemSucc.SubItems.Add(strTempSucc);
            this.AddList(listViewMessage, listItemSucc);
            Marshal.FreeHGlobal(ptrFaceRecordModify);
            Marshal.FreeHGlobal(struJsonDataCfg.lpPicData);
            Marshal.FreeHGlobal(ptrJsonDataCfg);
            m_iFaceDataRecordIndex++;
            return true;
        }

        private bool SearchFaceData()
        {
            if (-1 == m_lFaceDataSearchHandle)
            {
                return false;
            }

            CFaceDataSearchCond JsonFaceDataSearchCond = new CFaceDataSearchCond();

            JsonFaceDataSearchCond.searchResultPosition = 0;
            JsonFaceDataSearchCond.maxResults = 1;
            JsonFaceDataSearchCond.faceLibType = listViewFaceRecordManage.Items[m_iFaceDataRecordIndex].SubItems[1].Text;
            JsonFaceDataSearchCond.FDID = listViewFaceRecordManage.Items[m_iFaceDataRecordIndex].SubItems[2].Text;
            JsonFaceDataSearchCond.FPID = listViewFaceRecordManage.Items[m_iFaceDataRecordIndex].SubItems[3].Text;

            string strFaceDataSearchCond = JsonConvert.SerializeObject(JsonFaceDataSearchCond);

            IntPtr ptrFaceDataSearchCond = Marshal.StringToHGlobalAnsi(strFaceDataSearchCond);

            if (!CHCNetSDK.NET_DVR_SendRemoteConfig(m_lFaceDataSearchHandle, (int)CHCNetSDK.LONG_CFG_SEND_DATA_TYPE_ENUM.ENUM_SEND_JSON_DATA, ptrFaceDataSearchCond, (uint)strFaceDataSearchCond.Length))
            {
                ListViewItem listItem = new ListViewItem();
                listItem.Text = "Fail";
                string strTemp = null;
                strTemp = string.Format("Search Fail");
                listItem.SubItems.Add(strTemp);
                this.AddList(listViewMessage, listItem);
                Marshal.FreeHGlobal(ptrFaceDataSearchCond);
                return false;
            }
            ListViewItem listItemSucc = new ListViewItem();
            listItemSucc.Text = "Succ";
            string strTempSucc = null;
            strTempSucc = string.Format("Search Success");
            listItemSucc.SubItems.Add(strTempSucc);
            this.AddList(listViewMessage, listItemSucc);
            Marshal.FreeHGlobal(ptrFaceDataSearchCond);
            m_iFaceDataRecordIndex++;
            return true;
        }

        private void DeleteFaceRecord()
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

            string sUrl = "PUT /ISAPI/Intelligent/FDLib/FDSearch/Delete?format=json&FDID=" + textBoxFDID2.Text + "&faceLibType=blackFD";
            IntPtr ptrURL = Marshal.StringToHGlobalAnsi(sUrl);
            struInput.dwSize = (uint)Marshal.SizeOf(struInput);
            struInput.lpRequestUrl = ptrURL;
            struInput.dwRequestUrlLen = (uint)sUrl.Length;

            CFaceRecordDelete JsonFaceRecordDelete = new CFaceRecordDelete();
            JsonFaceRecordDelete.FPID = new List<CFPID>();

            if (!m_bDeleteAll)
            {
                foreach (ListViewItem item in this.listViewFPID.Items)
                {
                    if (item.SubItems[1].Text != "")
                    {
                        CFPID singleFPID = new CFPID();
                        singleFPID.value = item.SubItems[1].Text;
                        JsonFaceRecordDelete.FPID.Add(singleFPID);
                    }
                }
            }

            string strFaceRecordDelete = JsonConvert.SerializeObject(JsonFaceRecordDelete);

            IntPtr ptrInBuffer = Marshal.StringToHGlobalAnsi(strFaceRecordDelete);

            struInput.lpInBuffer = ptrInBuffer;
            struInput.dwInBufferSize = (uint)strFaceRecordDelete.Length;

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
                string strTemp = string.Format("Delete Face Record Fail,Error Code={0}", CHCNetSDK.NET_DVR_GetLastError());
                listItem.SubItems.Add(strTemp);
                this.AddList(listViewMessage, listItem);
            }
            else
            {
                ListViewItem listItem = new ListViewItem();
                listItem.Text = "Succ";
                string strTemp = string.Format("Delete Face Record Success");
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
            //if (!File.Exists(textBoxPictureDataPath.Text))
            //{
            //    MessageBox.Show("The picture does not exist!");
            //    return;
            //}
            //FileStream fs = new FileStream(textBoxPictureDataPath.Text, FileMode.OpenOrCreate);
            //if (0 == fs.Length)
            //{
            //    MessageBox.Show("The picture is 0k,please input another picture!");
            //    fs.Close();
            //    return;
            //}
            //if (200 * 1024 < fs.Length)
            //{
            //    MessageBox.Show("The picture is larger than 200k,please input another picture!");
            //    fs.Close();
            //    return;
            //}
            //fs.Close();

            listViewFaceRecordManage.BeginUpdate();

            ListViewItem listItem = new ListViewItem();
            listItem.Text = (m_iFaceDataRecordCount + 1).ToString();
            listItem.SubItems.Add("blackFD");
            listItem.SubItems.Add(textBoxFDID.Text);
            listItem.SubItems.Add(textBoxFPID.Text);
            listItem.SubItems.Add(textBoxPictureDataPath.Text);

            m_iFaceDataRecordCount++;

            listViewFaceRecordManage.Items.Add(listItem);
            listViewFaceRecordManage.EndUpdate();
        }

        private void buttonUpdateList_Click(object sender, EventArgs e)
        {
            if (listViewFaceRecordManage.SelectedItems.Count > 0)
            {
                //if (!File.Exists(textBoxPictureDataPath.Text))
                //{
                //    MessageBox.Show("The picture does not exist!");
                //    return;
                //}
                //FileStream fs = new FileStream(textBoxPictureDataPath.Text, FileMode.OpenOrCreate);
                //if (0 == fs.Length)
                //{
                //    MessageBox.Show("The picture is 0k,please input another picture!");
                //    fs.Close();
                //    return;
                //}
                //if (200 * 1024 < fs.Length)
                //{
                //    MessageBox.Show("The picture is larger than 200k,please input another picture!");
                //    fs.Close();
                //    return;
                //}
                //fs.Close();

                listViewFaceRecordManage.SelectedItems[0].SubItems[2].Text = textBoxFDID.Text;
                listViewFaceRecordManage.SelectedItems[0].SubItems[3].Text = textBoxFPID.Text;
                listViewFaceRecordManage.SelectedItems[0].SubItems[4].Text = textBoxPictureDataPath.Text;
            }
        }

        private void buttonSend_Click(object sender, EventArgs e)
        {
            m_iFaceDataRecordIndex = 0;
            if (-1 != m_lFaceDataRecordHandle)
            {
                if (CHCNetSDK.NET_DVR_StopRemoteConfig(m_lFaceDataRecordHandle))
                {
                    m_lFaceDataRecordHandle = -1;
                }
            }

            if (m_iFaceDataRecordCount < 1)
            {
                MessageBox.Show("Please add some face records!");
                return;
            }

            g_fFaceDataRecordCallback = new CHCNetSDK.RemoteConfigCallback(ProcessFaceDataRecordCallback);
            string sURL = "POST /ISAPI/Intelligent/FDLib/FaceDataRecord?format=json";
            IntPtr ptrURL = Marshal.StringToHGlobalAnsi(sURL);

            m_lFaceDataRecordHandle = CHCNetSDK.NET_DVR_StartRemoteConfig(m_lUserID, CHCNetSDK.NET_DVR_FACE_DATA_RECORD, ptrURL, sURL.Length, g_fFaceDataRecordCallback, this.Handle);
            if (-1 == m_lFaceDataRecordHandle)
            {
                ListViewItem listItem = new ListViewItem();
                listItem.Text = "Fail";
                string strTemp = string.Format("Face Data Record Fail,Error Code={0}", CHCNetSDK.NET_DVR_GetLastError());
                listItem.SubItems.Add(strTemp);
                this.AddList(listViewMessage, listItem);
                Marshal.FreeHGlobal(ptrURL);
                return;
            }
            else
            {
                ListViewItem listItem = new ListViewItem();
                listItem.Text = "Succ";
                listItem.SubItems.Add("Face Data Record Success");
                this.AddList(listViewMessage, listItem);
            }
            Marshal.FreeHGlobal(ptrURL);
            if (!SendFaceRecord())
            {
                CHCNetSDK.NET_DVR_StopRemoteConfig(m_lFaceDataRecordHandle);
                m_lFaceDataRecordHandle = -1;
            }
        }

        private void buttonModify_Click(object sender, EventArgs e)
        {
            m_iFaceDataRecordIndex = 0;
            if (-1 != m_lFaceDataRecordHandle)
            {
                if (CHCNetSDK.NET_DVR_StopRemoteConfig(m_lFaceDataRecordHandle))
                {
                    m_lFaceDataRecordHandle = -1;
                }
            }

            if (m_iFaceDataRecordCount < 1)
            {
                MessageBox.Show("Please add some face records!");
                return;
            }

            g_fFaceDataModifyCallback = new CHCNetSDK.RemoteConfigCallback(ProcessFaceDataModifyCallback);
            string sURL = "PUT /ISAPI/Intelligent/FDLib/FDModify?format=json";
            IntPtr ptrURL = Marshal.StringToHGlobalAnsi(sURL);

            m_lFaceDataRecordHandle = CHCNetSDK.NET_DVR_StartRemoteConfig(m_lUserID, CHCNetSDK.NET_DVR_FACE_DATA_MODIFY, ptrURL, sURL.Length, g_fFaceDataModifyCallback, this.Handle);
            if (-1 == m_lFaceDataRecordHandle)
            {
                ListViewItem listItem = new ListViewItem();
                listItem.Text = "Fail";
                string strTemp = string.Format("Face Data Modify Fail,Error Code={0}", CHCNetSDK.NET_DVR_GetLastError());
                listItem.SubItems.Add(strTemp);
                this.AddList(listViewMessage, listItem);
                Marshal.FreeHGlobal(ptrURL);
                return;
            }
            else
            {
                ListViewItem listItem = new ListViewItem();
                listItem.Text = "Succ";
                listItem.SubItems.Add("Face Data Modify Success");
                this.AddList(listViewMessage, listItem);
            }
            Marshal.FreeHGlobal(ptrURL);
            if (!SendFaceModify())
            {
                CHCNetSDK.NET_DVR_StopRemoteConfig(m_lFaceDataRecordHandle);
                m_lFaceDataRecordHandle = -1;
            }
        }

        private void buttonSearch_Click(object sender, EventArgs e)
        {
            m_iFaceDataRecordIndex = 0;
            if (-1 != m_lFaceDataSearchHandle)
            {
                if (CHCNetSDK.NET_DVR_StopRemoteConfig(m_lFaceDataSearchHandle))
                {
                    m_lFaceDataSearchHandle = -1;
                }
            }

            g_fFaceDataSearchCallback = new CHCNetSDK.RemoteConfigCallback(ProcessFaceDataSearchCallback);
            string sURL = "POST /ISAPI/Intelligent/FDLib/FDSearch?format=json";
            IntPtr ptrURL = Marshal.StringToHGlobalAnsi(sURL);

            m_lFaceDataSearchHandle = CHCNetSDK.NET_DVR_StartRemoteConfig(m_lUserID, CHCNetSDK.NET_DVR_FACE_DATA_SEARCH, ptrURL, sURL.Length, g_fFaceDataSearchCallback, this.Handle);
            if (-1 == m_lFaceDataSearchHandle)
            {
                ListViewItem listItem = new ListViewItem();
                listItem.Text = "Fail";
                string strTemp = string.Format("Face Data Search Fail,Error Code={0}", CHCNetSDK.NET_DVR_GetLastError());
                listItem.SubItems.Add(strTemp);
                this.AddList(listViewMessage, listItem);
                Marshal.FreeHGlobal(ptrURL);
                return;
            }
            else
            {
                ListViewItem listItem = new ListViewItem();
                listItem.Text = "Succ";
                listItem.SubItems.Add("Face Data Search Success");
                this.AddList(listViewMessage, listItem);
            }
            Marshal.FreeHGlobal(ptrURL);
            if (!SearchFaceData())
            {
                CHCNetSDK.NET_DVR_StopRemoteConfig(m_lFaceDataSearchHandle);
                m_lFaceDataSearchHandle = -1;
            }
        }

        private void buttonUpdatFPID_Click(object sender, EventArgs e)
        {
            if (listViewFPID.SelectedItems.Count > 0)
            {
                listViewFPID.SelectedItems[0].SubItems[1].Text = textBoxFPID2.Text;
            }
        }

        private void buttonDeleteAll_Click(object sender, EventArgs e)
        {
            m_bDeleteAll = true;

            DeleteFaceRecord();
        }

        private void buttonDeleteByFPID_Click(object sender, EventArgs e)
        {
            m_bDeleteAll = false;

            DeleteFaceRecord();
        }

        private void buttonClearList_Click(object sender, EventArgs e)
        {
            m_iFaceDataRecordCount = 0;
            listViewFaceRecordManage.Items.Clear();
        }

        private void buttonGetAllCount_Click(object sender, EventArgs e)
        {

        }

        private void buttonGetByFDID_Click(object sender, EventArgs e)
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

            string sUrl = "GET /ISAPI/Intelligent/FDLib/Count?format=json&FDID=" + textBoxFDID3.Text + "&faceLibType=blackFD";
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
                string strTemp = string.Format("Get Face Record Count Fail,Error Code={0}", CHCNetSDK.NET_DVR_GetLastError());
                listItem.SubItems.Add(strTemp);
                this.AddList(listViewMessage, listItem);
            }
            else
            {
                ListViewItem listItem = new ListViewItem();
                listItem.Text = "Succ";
                string strTemp = string.Format("Get Face Record Count Success");
                listItem.SubItems.Add(strTemp);
                this.AddList(listViewMessage, listItem);

                string strFaceRecordCount = Marshal.PtrToStringAnsi(struOuput.lpOutBuffer);
                CFaceRecordCount JsonFaceRecordCount = new CFaceRecordCount();
                JsonFaceRecordCount = JsonConvert.DeserializeObject<CFaceRecordCount>(strFaceRecordCount);
                int iRecordDataNumber = JsonFaceRecordCount.recordDataNumber;
                textBoxRecordDataNumber.Text = iRecordDataNumber.ToString();
            }

            Marshal.FreeHGlobal(ptrOutBuf);
            Marshal.FreeHGlobal(ptrStatusBuffer);
            Marshal.FreeHGlobal(ptrInput);
            Marshal.FreeHGlobal(ptrOuput);
            Marshal.FreeHGlobal(ptrURL);
        }

        private void listViewFaceRecordManage_ItemSelectionChanged(object sender, ListViewItemSelectionChangedEventArgs e)
        {
            if (listViewFaceRecordManage.SelectedItems.Count > 0)
            {
                textBoxFDID.Text = e.Item.SubItems[2].Text;
                textBoxFPID.Text = e.Item.SubItems[3].Text;
                textBoxPictureDataPath.Text = e.Item.SubItems[4].Text;
            }
        }

        private void listViewFPID_ItemSelectionChanged(object sender, ListViewItemSelectionChangedEventArgs e)
        {
            if (listViewFPID.SelectedItems.Count > 0)
            {
                textBoxFPID2.Text = e.Item.SubItems[1].Text;
            }
        }

        protected override void DefWndProc(ref Message m)
        {
            switch (m.Msg)
            {
                case 1001:
                case 1004:
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
                    if (-1 != m_lFaceDataRecordHandle)
                    {
                        if (CHCNetSDK.NET_DVR_StopRemoteConfig(m_lFaceDataRecordHandle))
                        {
                            m_lFaceDataRecordHandle = -1;
                        }
                    }
                    break;
                case 1002:
                    SendFaceRecord();
                    break;
                case 1003:
                    listViewFaceRecordManage.Items[m_iFaceDataRecordIndex - 1].SubItems[3].Text = m_strFPID;
                    int iErrorRecord = m.WParam.ToInt32();
                    if (iErrorRecord == 1)
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
                case 1006:
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
                case 1005:
                    SendFaceModify();
                    break;
                case 1007:
                    int iErrorMsgSearch = m.WParam.ToInt32();
                    if (iErrorMsgSearch == 1)
                    {
                        ListViewItem listItem = new ListViewItem();
                        listItem.Text = "Fail";
                        string strTemp = string.Format("NET_SDK_CALLBACK_STATUS_FAILED");
                        listItem.SubItems.Add(strTemp);
                        this.AddList(listViewMessage, listItem);
                    }
                    else if (iErrorMsgSearch == 2)
                    {
                        ListViewItem listItem = new ListViewItem();
                        listItem.Text = "Fail";
                        string strTemp = string.Format("NET_SDK_CALLBACK_STATUS_EXCEPTION");
                        listItem.SubItems.Add(strTemp);
                        this.AddList(listViewMessage, listItem);
                    }
                    else if (iErrorMsgSearch == 3)
                    {
                        ListViewItem listItem = new ListViewItem();
                        listItem.Text = "Fail";
                        string strTemp = string.Format("NET_SDK_CALLBACK_OTHER_ERROR");
                        listItem.SubItems.Add(strTemp);
                        this.AddList(listViewMessage, listItem);
                    }
                    else if (iErrorMsgSearch == 4)
                    {
                        ListViewItem listItem = new ListViewItem();
                        listItem.Text = "Succ";
                        string strTemp = string.Format("NET_SDK_CALLBACK_FINISH");
                        listItem.SubItems.Add(strTemp);
                        this.AddList(listViewMessage, listItem);
                    }
                    if (-1 != m_lFaceDataSearchHandle)
                    {
                        if (CHCNetSDK.NET_DVR_StopRemoteConfig(m_lFaceDataSearchHandle))
                        {
                            m_lFaceDataSearchHandle = -1;
                        }
                    }
                    break;
                case 1008:
                    SearchFaceData();
                    break;
                case 1009:
                    if (m_JsonFaceDataSearchReturn.MatchList[0].FPID != null)
                    {
                        listViewFaceRecordManage.Items[m_iFaceDataRecordIndex-1].SubItems[3].Text = m_JsonFaceDataSearchReturn.MatchList[0].FPID;
                    }
                    else
                    {
                        listViewFaceRecordManage.Items[m_iFaceDataRecordIndex-1].SubItems[3].Text = "";
                    }
                    listViewFaceRecordManage.Items[m_iFaceDataRecordIndex-1].SubItems[4].Text = m_JsonFaceDataSearchReturn.MatchList[0].employeeNo.ToString();

                    if (m_struJsonDataCfg.dwPicDataSize > 0)
                    {
                        string strPicName = "FaceRecord_FPID_" + m_JsonFaceDataSearchReturn.MatchList[0].FPID + "_employeeNo_" + m_JsonFaceDataSearchReturn.MatchList[0].employeeNo.ToString() + ".jpg";
                        FileStream fs = new FileStream(strPicName, FileMode.Create);
                        int iLen = (int)m_struJsonDataCfg.dwPicDataSize;
                        byte[] by = new byte[iLen];
                        Marshal.Copy(m_struJsonDataCfg.lpPicData, by, 0, iLen);
                        fs.Write(by, 0, iLen);
                        fs.Close();
                        listViewFaceRecordManage.Items[m_iFaceDataRecordIndex-1].SubItems[5].Text = strPicName;
                    }
                    else
                    {
                        listViewFaceRecordManage.Items[m_iFaceDataRecordIndex-1].SubItems[5].Text = "";
                    }
                    break;
                default:
                    base.DefWndProc(ref m);
                    break;
            }
        }

        private void FaceRecordManagement_Load(object sender, EventArgs e)
        {
            MultiLanguage.LoadLanguage(this);
        }
    }
}
