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
using System.IO;

using AcsDemo.Public;
using AcsDemo.Language;

namespace AcsDemo.ProductAcs
{
    public partial class FingerPrintMgr : Form
    {
        public const int iMaxCardNum = 1000;
        public CHCNetSDK.NET_DVR_FINGER_PRINT_CFG_V50 m_struFingerPrintOne = new CHCNetSDK.NET_DVR_FINGER_PRINT_CFG_V50();
        public CHCNetSDK.NET_DVR_FINGER_PRINT_INFO_CTRL_BYCARD_V50 m_struDelFingerPrint = new CHCNetSDK.NET_DVR_FINGER_PRINT_INFO_CTRL_BYCARD_V50();
        public CHCNetSDK.NET_DVR_FINGER_PRINT_CFG_V50[] m_struRecordCardCfg = new CHCNetSDK.NET_DVR_FINGER_PRINT_CFG_V50[iMaxCardNum];
        public int m_lSetFingerPrintCfgHandle = -1;
        public int m_lGetFingerPrintCfgHandle = -1;
        public int m_lUserID = -1;
        public int m_iSendIndex = -1;
        public int m_lDelFingerPrintCfHandle = -1;

        public FingerPrintMgr()
        {
            InitializeComponent();
            CreateCheckListBox();
            m_struFingerPrintOne.Init();
            m_struDelFingerPrint.struByCard.byFingerPrintID = new byte[CHCNetSDK.MAX_FINGER_PRINT_NUM];
            this.SuspendLayout();
            this.columnHeader1.Name = "columnHeader1";
            this.columnHeader2.Name = "columnHeader2";
            this.columnHeader3.Name = "columnHeader3";
            this.columnHeader4.Name = "columnHeader4";
            this.columnHeader5.Name = "columnHeader5";
            this.columnHeader6.Name = "columnHeader6";
            this.columnHeader9.Name = "columnHeader9";
            this.columnHeader7.Name = "columnHeader7";
            this.columnHeader8.Name = "columnHeader8";
            this.ResumeLayout();
        }

        private CHCNetSDK.RemoteConfigCallback g_fSetFingerPrintCallback = null;
        private CHCNetSDK.RemoteConfigCallback g_fGetFingerPrintCallback = null;
        private CHCNetSDK.RemoteConfigCallback g_fDelFingerPrintCallback = null;
        private delegate void AddListCallBack(ListView cListView, ListViewItem cListViewItem);
        private delegate string GetListTextCallBack(ListView cListView, int iItemTag, int iSubitemTag, string strText = null);
        private delegate string GetTextBoxTextCallBack(TextBox cTextBox);

        private string GetTextBoxText(TextBox cTextBox)
        {
            if (this.InvokeRequired)
            {
                GetTextBoxTextCallBack add = new GetTextBoxTextCallBack(GetTextBoxText);
                this.Invoke(add, new object[] { cTextBox });
            }
            else
            {
                return cTextBox.Text;
            }
            return cTextBox.Text;
        }

        private string GetListText(ListView cListView, int iItemTag, int iSubitemTag, string strText = null)
        {
            if (this.InvokeRequired)
            {
                GetListTextCallBack add = new GetListTextCallBack(GetListText);
                this.Invoke(add, new object[] { cListView, iItemTag, iSubitemTag, strText });
            }
            else
            {
                if (null == strText)
                {
                    return cListView.Items[iItemTag].SubItems[iSubitemTag].Text;
                }
                else
                {
                    cListView.Items[iItemTag].SubItems[iSubitemTag].Text = strText;
                    return strText;
                }
            }
            return null;
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
                cListView.Items.Add(cListViewItem);
            }
        }

        private bool SendFirstCard()
        {
            if (-1 == m_lSetFingerPrintCfgHandle)
            {
                return false;
            }

            m_iSendIndex = 0;
            int indexTmp = 0;
            int.TryParse(textBoxNumber.Text, out indexTmp);
            if (indexTmp < 1)
            {
                return false;
            }

            uint dwSize = (uint)Marshal.SizeOf(m_struRecordCardCfg[m_iSendIndex]);
            IntPtr ptrNowSendCard = Marshal.AllocHGlobal((int)dwSize);
            Marshal.StructureToPtr(m_struRecordCardCfg[m_iSendIndex], ptrNowSendCard, false);
            if (!CHCNetSDK.NET_DVR_SendRemoteConfig(m_lSetFingerPrintCfgHandle, 3, ptrNowSendCard, dwSize))
            {
                ListViewItem listItem = new ListViewItem();
                listItem.Text = "FAIL";
                string strTemp = string.Format("Send Fail,CardNo:{0},EmployeeNo:{1}", System.Text.Encoding.UTF8.GetString(m_struRecordCardCfg[m_iSendIndex].byCardNo).TrimEnd('\0'), System.Text.Encoding.UTF8.GetString(m_struRecordCardCfg[m_iSendIndex].byEmployeeNo).TrimEnd('\0'));
                listItem.SubItems.Add(strTemp);
                this.AddList(listViewMessage, listItem);
                Marshal.FreeHGlobal(ptrNowSendCard);
                return false;
            }
            Marshal.FreeHGlobal(ptrNowSendCard);
            return true;
        }


        private bool SendNextFingerPrint()
        {
            //IntPtr ihWnd = this.Handle;
            if (-1 == m_lSetFingerPrintCfgHandle)
            {
                return false;
            }

            m_iSendIndex++;
            string strTempText = GetTextBoxText(textBoxNumber);

            int indexTmp = 0;
            int.TryParse(strTempText, out indexTmp);

            if (m_iSendIndex >= indexTmp)
            {
                return true;
            }
            uint dwSize = (uint)Marshal.SizeOf(m_struRecordCardCfg[m_iSendIndex]);
            IntPtr ptrNowSendCard = Marshal.AllocHGlobal((int)dwSize);
            Marshal.StructureToPtr(m_struRecordCardCfg[m_iSendIndex], ptrNowSendCard, false);
            if (!CHCNetSDK.NET_DVR_SendRemoteConfig(m_lSetFingerPrintCfgHandle, 3,  ptrNowSendCard, dwSize))
            {
                ListViewItem listItem = new ListViewItem();
                listItem.Text = "FAIL";
                string strTemp = string.Format("Send Fail,CardNo:{0},EmployeeNo:{1}", System.Text.Encoding.UTF8.GetString(m_struRecordCardCfg[m_iSendIndex].byCardNo).TrimEnd('\0'), System.Text.Encoding.UTF8.GetString(m_struRecordCardCfg[m_iSendIndex].byEmployeeNo).TrimEnd('\0'));
                listItem.SubItems.Add(strTemp);
                this.AddList(listViewMessage, listItem);
                Marshal.FreeHGlobal(ptrNowSendCard);
                return false;
            }
            Marshal.FreeHGlobal(ptrNowSendCard);
            return true;
        }

        private void ProcessSetFingerPrintCfgCallbackData(uint dwType, IntPtr lpBuffer, uint dwBufLen, IntPtr pUserData)
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
                    //some problem
                    ListViewItem listItem = new ListViewItem();
                    listItem.Text = "SUCC";
                    string strTemp = null;
                    strTemp = string.Format("SetFingerPrint PROCESSING,CardNo:{0},EmployeeNo:{1}", System.Text.Encoding.UTF8.GetString(m_struRecordCardCfg[m_iSendIndex].byCardNo).TrimEnd('\0'), System.Text.Encoding.UTF8.GetString(m_struRecordCardCfg[m_iSendIndex].byEmployeeNo).TrimEnd('\0'));
                    listItem.SubItems.Add(strTemp);
                    this.AddList(listViewMessage, listItem);
                    SendNextFingerPrint();
                }
                else if (dwStatus == (uint)CHCNetSDK.NET_SDK_CALLBACK_STATUS_NORMAL.NET_SDK_CALLBACK_STATUS_FAILED)
                {
                    ListViewItem listItem = new ListViewItem();
                    listItem.Text = "FAIL";
                    listItem.SubItems.Add("SetFingerPrint Err:NET_SDK_CALLBACK_STATUS_FAILED");
                    this.AddList(listViewMessage, listItem);
                    SendNextFingerPrint();
                }
                else if (dwStatus == (uint)CHCNetSDK.NET_SDK_CALLBACK_STATUS_NORMAL.NET_SDK_CALLBACK_STATUS_SUCCESS)
                {
                    ListViewItem listItem = new ListViewItem();
                    listItem.Text = "SUCC";
                    listItem.SubItems.Add("SetFingerPrint SUCCESS");
                    this.AddList(listViewMessage, listItem);

                    ListViewItem listItem2 = new ListViewItem();
                    listItem2.Text = "SUCC";
                    listItem2.SubItems.Add("NET_DVR_SET_FINGERPRINT_CFG_V50 Set finish");
                    this.AddList(listViewMessage, listItem2);
                    CHCNetSDK.PostMessage(pUserData, 1001, 0, 0);
                }
                else if (dwStatus == (uint)CHCNetSDK.NET_SDK_CALLBACK_STATUS_NORMAL.NET_SDK_CALLBACK_STATUS_EXCEPTION)
                {
                    ListViewItem listItem = new ListViewItem();
                    listItem.Text = "FAIL";
                    listItem.SubItems.Add("SetFingerPrint EXCEPTION");
                    this.AddList(listViewMessage, listItem);

                    ListViewItem listItem2 = new ListViewItem();
                    listItem2.Text = "SUCC";
                    listItem2.SubItems.Add("NET_DVR_SET_FINGERPRINT_CFG_V50 Set finish");
                    this.AddList(listViewMessage, listItem2);
                }
                else
                {
                    ListViewItem listItem = new ListViewItem();
                    listItem.Text = "SUCC";
                    listItem.SubItems.Add("SetFingerPrint SUCCESS");
                    this.AddList(listViewMessage, listItem);

                    ListViewItem listItem2 = new ListViewItem();
                    listItem2.Text = "SUCC";
                    listItem2.SubItems.Add("NET_DVR_SET_FINGERPRINT_CFG_V50 Set finish");
                    this.AddList(listViewMessage, listItem2);
                }
            }
            else if (dwType == (uint)CHCNetSDK.NET_SDK_CALLBACK_TYPE.NET_SDK_CALLBACK_TYPE_DATA)
            {
                CHCNetSDK.NET_DVR_FINGER_PRINT_STATUS_V50 struCfg = new CHCNetSDK.NET_DVR_FINGER_PRINT_STATUS_V50();
                struCfg = (CHCNetSDK.NET_DVR_FINGER_PRINT_STATUS_V50)Marshal.PtrToStructure(lpBuffer, typeof(CHCNetSDK.NET_DVR_FINGER_PRINT_STATUS_V50));
                bool bSendOK = false;
                if (struCfg.byRecvStatus == 0)  //Total status is OK!
                {
                    for (int i = 0; i < struCfg.byCardReaderRecvStatus.Length; i++)
                    {
                        if (1 == struCfg.byCardReaderRecvStatus[i])
                        {
                            bSendOK = true;
                            ListViewItem listItem = new ListViewItem();
                            listItem.Text = "SUCC";
                            string strTemp = null;
                            strTemp = string.Format("SetFingerPrint PROCESSING,CardNo:{0},EmployeeNo:{1},CardReader:{2}", System.Text.Encoding.UTF8.GetString(m_struRecordCardCfg[m_iSendIndex].byCardNo).TrimEnd('\0'), System.Text.Encoding.UTF8.GetString(m_struRecordCardCfg[m_iSendIndex].byEmployeeNo).TrimEnd('\0'), i + 1);
                            listItem.SubItems.Add(strTemp);
                            this.AddList(listViewMessage, listItem);
                        }
                        else if (8 == struCfg.byCardReaderRecvStatus[i])
                        {
                            //此处为8，代表无需配置，故无需打印任何信息
                            ;
                        }
                        else
                        {
                            bSendOK = false;
                            ListViewItem listItem = new ListViewItem();
                            listItem.Text = "FAIL";
                            string strTemp = null;
                            strTemp = string.Format("SetFingerPrint PROCESSING,CardNo:{0},EmployeeNo:{1},CardReader:{2},Error Code:{3}", System.Text.Encoding.UTF8.GetString(m_struRecordCardCfg[m_iSendIndex].byCardNo).TrimEnd('\0'), System.Text.Encoding.UTF8.GetString(m_struRecordCardCfg[m_iSendIndex].byEmployeeNo).TrimEnd('\0'), i + 1, struCfg.byCardReaderRecvStatus[i]);
                            listItem.SubItems.Add(strTemp);
                            this.AddList(listViewMessage, listItem);
                        }
                    }
                }
                else
                {
                    bSendOK = false;
                    ListViewItem listItem = new ListViewItem();
                    listItem.Text = "FAIL";
                    string strTemp = null;
                    if (struCfg.byRecvStatus == 1)
                    {
                        strTemp = string.Format("SetFingerPrint PROCESSING,CardNo:{0},EmployeeNo:{1},Error Finger ID", System.Text.Encoding.UTF8.GetString(m_struRecordCardCfg[m_iSendIndex].byCardNo).TrimEnd('\0'), System.Text.Encoding.UTF8.GetString(m_struRecordCardCfg[m_iSendIndex].byEmployeeNo).TrimEnd('\0'));
                    }
                    else if (struCfg.byRecvStatus == 2)
                    {
                        strTemp = string.Format("SetFingerPrint PROCESSING,CardNo:{0},EmployeeNo:{1},Error Finger Print Type", System.Text.Encoding.UTF8.GetString(m_struRecordCardCfg[m_iSendIndex].byCardNo).TrimEnd('\0'), System.Text.Encoding.UTF8.GetString(m_struRecordCardCfg[m_iSendIndex].byEmployeeNo).TrimEnd('\0'));
                    }
                    else if (struCfg.byRecvStatus == 3)
                    {
                        strTemp = string.Format("SetFingerPrint PROCESSING,CardNo:{0},EmployeeNo:{1},Illegal Card No", System.Text.Encoding.UTF8.GetString(m_struRecordCardCfg[m_iSendIndex].byCardNo).TrimEnd('\0'), System.Text.Encoding.UTF8.GetString(m_struRecordCardCfg[m_iSendIndex].byEmployeeNo).TrimEnd('\0'));
                    }
                    else if (struCfg.byRecvStatus == 4)
                    {
                        strTemp = string.Format("SetFingerPrint PROCESSING,CardNo:{0},EmployeeNo:{1},Finger Print Not Link Employee No Or Card No", System.Text.Encoding.UTF8.GetString(m_struRecordCardCfg[m_iSendIndex].byCardNo).TrimEnd('\0'), System.Text.Encoding.UTF8.GetString(m_struRecordCardCfg[m_iSendIndex].byEmployeeNo).TrimEnd('\0'));
                    }
                    else if (struCfg.byRecvStatus == 5)
                    {
                        strTemp = string.Format("SetFingerPrint PROCESSING,CardNo:{0},EmployeeNo:{1},Employee No Not Exist", System.Text.Encoding.UTF8.GetString(m_struRecordCardCfg[m_iSendIndex].byCardNo).TrimEnd('\0'), System.Text.Encoding.UTF8.GetString(m_struRecordCardCfg[m_iSendIndex].byEmployeeNo).TrimEnd('\0'));
                    }
                    else
                    {
                        strTemp = string.Format("SetFingerPrint PROCESSING,CardNo:{0},EmployeeNo:{1},Error Code:{2}", System.Text.Encoding.UTF8.GetString(m_struRecordCardCfg[m_iSendIndex].byCardNo).TrimEnd('\0'), System.Text.Encoding.UTF8.GetString(m_struRecordCardCfg[m_iSendIndex].byEmployeeNo).TrimEnd('\0'), struCfg.byRecvStatus);
                    }
                    listItem.SubItems.Add(strTemp);
                    this.AddList(listViewMessage, listItem);
                }

                if (!bSendOK)
                {
                    ListViewItem listItem = new ListViewItem();
                    listItem.Text = "FAIL";
                    string strTemp = null;
                    strTemp = string.Format("SetFingerPrint Failed,CardNo:{0},EmployeeNo:{1}", System.Text.Encoding.UTF8.GetString(m_struRecordCardCfg[m_iSendIndex].byCardNo).TrimEnd('\0'), System.Text.Encoding.UTF8.GetString(m_struRecordCardCfg[m_iSendIndex].byEmployeeNo).TrimEnd('\0'));
                    listItem.SubItems.Add(strTemp);
                    this.AddList(listViewMessage, listItem);
                }
                //if (0 == struCfg.byTotalStatus)
                //{

                //}
                SendNextFingerPrint();
            }
        }

        private void ProcessGetFingerPrintCfgCallbackData(uint dwType, IntPtr lpBuffer, uint dwBufLen, IntPtr pUserData)
        {
            if (pUserData == null)
            {
                return;
            }
            if (dwType == (uint)CHCNetSDK.NET_SDK_CALLBACK_TYPE.NET_SDK_CALLBACK_TYPE_DATA)
            {
                CHCNetSDK.NET_DVR_FINGER_PRINT_CFG_V50 strFingerPrintCfg = new CHCNetSDK.NET_DVR_FINGER_PRINT_CFG_V50();
                strFingerPrintCfg = (CHCNetSDK.NET_DVR_FINGER_PRINT_CFG_V50)Marshal.PtrToStructure(lpBuffer, typeof(CHCNetSDK.NET_DVR_FINGER_PRINT_CFG_V50));
                //Marshal.PtrToStructure(lpBuffer, strCardCfg);
                bool bSendOK = false;
                for (int i = 0; i < strFingerPrintCfg.byEnableCardReader.Length; i++)
                {
                    if (1 == strFingerPrintCfg.byEnableCardReader[i])
                    {
                        bSendOK = true;
                        
                    }
                }
                if (!bSendOK)
                {
                    
                }
                bSendOK = false;
                for (int i = 0; i < strFingerPrintCfg.byLeaderFP.Length; i++)
                {
                    if (1 == strFingerPrintCfg.byLeaderFP[i])
                    {
                        bSendOK = true;

                    }
                }
                if (!bSendOK)
                {

                }
                if (0 == strFingerPrintCfg.dwSize)
                {
                    return;
                }
                AddToFingerPrintList(strFingerPrintCfg, false);

            }
            else if (dwType == (uint)CHCNetSDK.NET_SDK_CALLBACK_TYPE.NET_SDK_CALLBACK_TYPE_STATUS)
            {
                uint dwStatus = 0;
                dwStatus = (uint)Marshal.ReadInt32(lpBuffer);
                if (dwStatus == (uint)CHCNetSDK.NET_SDK_CALLBACK_STATUS_NORMAL.NET_SDK_CALLBACK_STATUS_SUCCESS)
                {
                    ListViewItem listItem = new ListViewItem();
                    listItem.Text = "SUCC";
                    listItem.SubItems.Add("NET_DVR_GET_FINGERPRINT_CFG_V50 Get finish");
                    this.AddList(listViewMessage, listItem);
                }
                else if (dwStatus == (uint)CHCNetSDK.NET_SDK_CALLBACK_STATUS_NORMAL.NET_SDK_CALLBACK_STATUS_FAILED)
                {
                    ListViewItem listItem = new ListViewItem();
                    listItem.Text = "FAIL";
                    listItem.SubItems.Add("NET_DVR_GET_FINGERPRINT_CFG_V50 Get FAIL");
                    this.AddList(listViewMessage, listItem);
                }
            }
        }

        private void ProcessDelFingerPrintCallback(uint dwType, IntPtr lpBuffer, uint dwBufLen, IntPtr pUserData)
        {
            if (pUserData == null)
            {
                return;
            }
            if (dwType == (uint)CHCNetSDK.NET_SDK_CALLBACK_TYPE.NET_SDK_CALLBACK_TYPE_DATA)
            {
                CHCNetSDK.NET_DVR_FINGER_PRINT_INFO_STATUS_V50 strFingerPrintInfoStatusV50 = new CHCNetSDK.NET_DVR_FINGER_PRINT_INFO_STATUS_V50();
                strFingerPrintInfoStatusV50 = (CHCNetSDK.NET_DVR_FINGER_PRINT_INFO_STATUS_V50)Marshal.PtrToStructure(lpBuffer, typeof(CHCNetSDK.NET_DVR_FINGER_PRINT_INFO_STATUS_V50));
                if (strFingerPrintInfoStatusV50.byStatus == 1)
                {
                    ListViewItem listItem = new ListViewItem();
                    listItem.Text = "SUCC";
                    listItem.SubItems.Add("NET_DVR_FINGER_PRINT_INFO_STATUS_V50 Processing");
                    this.AddList(listViewMessage, listItem);
                }
                else if (strFingerPrintInfoStatusV50.byStatus == 2)
                {
                    ListViewItem listItem = new ListViewItem();
                    listItem.Text = "FAIL";
                    listItem.SubItems.Add("NET_DVR_FINGER_PRINT_INFO_STATUS_V50 Failed");
                    this.AddList(listViewMessage, listItem);
                }
                else if (strFingerPrintInfoStatusV50.byStatus == 3)
                {
                    ListViewItem listItem = new ListViewItem();
                    listItem.Text = "SUCC";
                    listItem.SubItems.Add("NET_DVR_FINGER_PRINT_INFO_STATUS_V50 Success");
                    this.AddList(listViewMessage, listItem);
                }
            }
            else if (dwType == (uint)CHCNetSDK.NET_SDK_CALLBACK_TYPE.NET_SDK_CALLBACK_TYPE_STATUS)
            {
                uint dwStatus = 0;
                dwStatus = (uint)Marshal.ReadInt32(lpBuffer);
                if (dwStatus == (uint)CHCNetSDK.NET_SDK_CALLBACK_STATUS_NORMAL.NET_SDK_CALLBACK_STATUS_SUCCESS)
                {
                    ListViewItem listItem = new ListViewItem();
                    listItem.Text = "SUCC";
                    listItem.SubItems.Add("NET_DVR_DEL_FINGERPRINT_CFG_V50 finish");
                    this.AddList(listViewMessage, listItem);
                }
                else if (dwStatus == (uint)CHCNetSDK.NET_SDK_CALLBACK_STATUS_NORMAL.NET_SDK_CALLBACK_STATUS_FAILED)
                {
                    ListViewItem listItem = new ListViewItem();
                    listItem.Text = "FAIL";
                    listItem.SubItems.Add("NET_DVR_DEL_FINGERPRINT_CFG_V50 FAIL");
                    this.AddList(listViewMessage, listItem);
                }
            }
        }

        private void CreateCheckListBox()
        {
            string strTemp = null;
            for (int i = 0; i < 512; i++ )
            {
                strTemp = string.Format("CardReader {0}", i + 1);
                checkedListBoxReaderValid.Items.Add(strTemp);
            }
            for (int i = 0; i < 256; i++)
            {
                strTemp = string.Format("DoorNo {0}", i + 1);
                checkedListBoxLeaderFP.Items.Add(strTemp);
            }
            for (int i = 0; i < 10; i++ )
            {
                strTemp = string.Format("id {0}", i + 1);
                checkedListBoxFingerPrint.Items.Add(strTemp);
            }
        }

        private void GetTreeSel()
        {
            for (int i = 0; i < checkedListBoxReaderValid.Items.Count; i++ )
            {
                if (checkedListBoxReaderValid.GetItemChecked(i))
                {
                    m_struFingerPrintOne.byEnableCardReader[i] = 1;
                }
                else
                {
                    m_struFingerPrintOne.byEnableCardReader[i] = 0;
                }
            }

            for (int i = 0; i < checkedListBoxLeaderFP.Items.Count; i++)
            {
                if (checkedListBoxLeaderFP.GetItemChecked(i))
                {
                    m_struFingerPrintOne.byLeaderFP[i] = 1;
                }
                else
                {
                    m_struFingerPrintOne.byLeaderFP[i] = 0;
                }
            }

            for (int i = 0; i < checkedListBoxFingerPrint.Items.Count; i++ )
            {
                if (checkedListBoxFingerPrint.GetItemChecked(i))
                {
                    m_struDelFingerPrint.struByCard.byFingerPrintID[i] = 1;
                }
                else
                {
                    m_struDelFingerPrint.struByCard.byFingerPrintID[i] = 0;
                }
            }
        }

        private bool UpdateFingerPrintCfg(ref CHCNetSDK.NET_DVR_FINGER_PRINT_CFG_V50 struFingerPrintCfg)
        {
            struFingerPrintCfg.dwSize = (uint)Marshal.SizeOf(struFingerPrintCfg);
            byte[] byTempCard = System.Text.Encoding.UTF8.GetBytes(testBoxCard.Text);
            for (int i = 0; i < byTempCard.Length; i++ )
            {
                if (i > struFingerPrintCfg.byCardNo.Length)
                {
                    return false;
                }
                struFingerPrintCfg.byCardNo[i] = byTempCard[i];
            }
            uint.TryParse(textBoxDataLength.Text, out struFingerPrintCfg.dwFingerPrintLen);
            struFingerPrintCfg.byEnableCardReader = m_struFingerPrintOne.byEnableCardReader;
            struFingerPrintCfg.byLeaderFP = m_struFingerPrintOne.byLeaderFP;
            byte.TryParse(textBoxFingerPrintNumber.Text, out struFingerPrintCfg.byFingerPrintID);
            struFingerPrintCfg.byFingerType = (byte)comboBoxFingerprintType.SelectedIndex;
            byte[] byTempEmployeeNo = System.Text.Encoding.UTF8.GetBytes(textBoxEmployeeNo.Text);
            for (int i = 0; i < byTempEmployeeNo.Length; i++)
            {
                if (i > struFingerPrintCfg.byEmployeeNo.Length)
                {
                    return false;
                }
                struFingerPrintCfg.byEmployeeNo[i] = byTempEmployeeNo[i];
            }

            FileStream fs = new FileStream(textBoxFingerprintDataPath.Text, FileMode.OpenOrCreate);
            if (0 == fs.Length)
            {
                struFingerPrintCfg.byFingerData[0] = 0;
                fs.Close();
                return true;
            }
            BinaryReader objBinaryReader = new BinaryReader(fs);
            if (struFingerPrintCfg.dwFingerPrintLen > CHCNetSDK.MAX_FINGER_PRINT_LEN)
            {
                return false;
            }
            for (int i = 0; i < struFingerPrintCfg.dwFingerPrintLen; i++)
            {
                if (i >= fs.Length)
                {
                    break;
                }
                struFingerPrintCfg.byFingerData[i] = objBinaryReader.ReadByte();
            }
            fs.Close();
            return true;
        }

        private int GetExistItem(CHCNetSDK.NET_DVR_FINGER_PRINT_CFG_V50 struCardCfg)
        {
            int iItemCount = listViewFingerPrint.Items.Count;
            for (int i = 0; i < iItemCount; i++ )
            {
                string strCardNo = this.GetListText(listViewFingerPrint, i, 1);
                string strFingerPrintID = this.GetListText(listViewFingerPrint, i, 2);

                byte byFingerPrintIDTmp;
                byte.TryParse(strFingerPrintID, out byFingerPrintIDTmp);

                //由于此处增加了工号的字段，故不进行重复判断，上层保证不重复
                //if (System.Text.Encoding.UTF8.GetString(struCardCfg.byCardNo) == strCardNo &&
                //    struCardCfg.byFingerPrintID == byFingerPrintIDTmp)
                //{
                //    return i;
                //}
            }
            return -1;
        }

        private void UpdateList(int iInsertIndex, CHCNetSDK.NET_DVR_FINGER_PRINT_CFG_V50 struFingerPrintCfg)
        {
            string strPath = null;
            strPath = string.Format("{0} fingerprint.dat", iInsertIndex);
            FileStream fs = new FileStream(strPath, FileMode.OpenOrCreate);
            if (!File.Exists(strPath))
            {
                MessageBox.Show("Fingerprint storage file creat failed！");
            }
            BinaryWriter objBinaryWrite = new BinaryWriter(fs);
            fs.Write(struFingerPrintCfg.byFingerData, 0, (int)struFingerPrintCfg.dwFingerPrintLen);
            fs.Close();
        }

        private void AddToFingerPrintList(CHCNetSDK.NET_DVR_FINGER_PRINT_CFG_V50 struCardInfo, bool bAdd = true)
        {
            CHCNetSDK.NET_DVR_FINGER_PRINT_CFG_V50 stCardInfo = new CHCNetSDK.NET_DVR_FINGER_PRINT_CFG_V50();
            int iItemIndex = GetExistItem(struCardInfo);
            int iItemCount = listViewFingerPrint.Items.Count;
            if (-1 == iItemIndex)
            {
                stCardInfo = struCardInfo;
                string strItem = string.Format("{0}", iItemCount + 1);
                ListViewItem listItem = new ListViewItem();
                listItem.Text = strItem;
                listItem.SubItems.Add(System.Text.Encoding.UTF8.GetString(stCardInfo.byCardNo));
                listItem.SubItems.Add(stCardInfo.byFingerPrintID.ToString());
                if (0 == struCardInfo.byFingerType)
                {
                    listItem.SubItems.Add(AcsDemoPublic.strFingerType[0]);
                }
                else if (1 == struCardInfo.byFingerType)
                {
                    listItem.SubItems.Add(AcsDemoPublic.strFingerType[1]);
                }
                else if (2 == struCardInfo.byFingerType)
                {
                    listItem.SubItems.Add(AcsDemoPublic.strFingerType[2]);
                }
                else if (3 == struCardInfo.byFingerType)
                {
                    listItem.SubItems.Add(AcsDemoPublic.strFingerType[3]);
                }
                else if (4 == struCardInfo.byFingerType)
                {
                    listItem.SubItems.Add(AcsDemoPublic.strFingerType[4]);
                }
                listItem.SubItems.Add(stCardInfo.dwFingerPrintLen.ToString());
                if (bAdd)
                {
                    listItem.SubItems.Add(textBoxFingerprintDataPath.Text);
                }
                else
                {
                    string strPath = null;
                    strPath = string.Format("{0}\\{1} fingerprint.dat", Environment.CurrentDirectory, iItemCount);
                    listItem.SubItems.Add(strPath);
                }
                listItem.SubItems.Add(System.Text.Encoding.UTF8.GetString(stCardInfo.byEmployeeNo));
                //listItem.SubItems.Add(System.Text.Encoding.UTF8.GetString(stCardInfo.byFingerData));
                this.AddList(listViewFingerPrint, listItem);
                m_struRecordCardCfg[iItemCount] = struCardInfo;
            }
            else
            {
                m_struRecordCardCfg[iItemIndex] = struCardInfo;
                if (0 == struCardInfo.byFingerType)
                {
                    this.GetListText(listViewFingerPrint, iItemIndex, 3, AcsDemoPublic.strFingerType[0]);
                }
                else if (1 == struCardInfo.byFingerType)
                {
                    this.GetListText(listViewFingerPrint, iItemIndex, 3, AcsDemoPublic.strFingerType[1]);
                }
                else if (2 == struCardInfo.byFingerType)
                {
                    this.GetListText(listViewFingerPrint, iItemIndex, 3, AcsDemoPublic.strFingerType[2]);
                }
                else if (3 == struCardInfo.byFingerType)
                {
                    this.GetListText(listViewFingerPrint, iItemIndex, 3, AcsDemoPublic.strFingerType[3]);
                }
                else if (4 == struCardInfo.byFingerType)
                {
                    this.GetListText(listViewFingerPrint, iItemIndex, 3, AcsDemoPublic.strFingerType[4]);
                }
                this.GetListText(listViewFingerPrint, iItemIndex, 4, struCardInfo.dwFingerPrintLen.ToString());
                this.GetListText(listViewFingerPrint, iItemIndex, 5, System.Text.Encoding.UTF8.GetString(struCardInfo.byFingerData));
                this.GetListText(listViewFingerPrint, iItemIndex, 6, struCardInfo.byEmployeeNo.ToString());
            }
            UpdateList(iItemCount, struCardInfo);
        }

        private void buttonAdd_Click(object sender, EventArgs e)
        {
            CHCNetSDK.NET_DVR_FINGER_PRINT_CFG_V50 struFingerPrintCfg = new CHCNetSDK.NET_DVR_FINGER_PRINT_CFG_V50();
            struFingerPrintCfg.byCardNo = new byte[32];
            struFingerPrintCfg.byEmployeeNo = new byte[32];
            struFingerPrintCfg.byEnableCardReader = new byte[512];
            struFingerPrintCfg.byLeaderFP = new byte[256];
            struFingerPrintCfg.byFingerData = new byte[CHCNetSDK.MAX_FINGER_PRINT_LEN];
            UpdateFingerPrintCfg(ref struFingerPrintCfg);
            AddToFingerPrintList(struFingerPrintCfg);
        }

        private void buttonSet_Click(object sender, EventArgs e)
        {
            if (m_lSetFingerPrintCfgHandle != -1)
            {
                CHCNetSDK.NET_DVR_StopRemoteConfig(m_lSetFingerPrintCfgHandle);
            }
            CHCNetSDK.NET_DVR_FINGER_PRINT_INFO_COND_V50 struCond = new CHCNetSDK.NET_DVR_FINGER_PRINT_INFO_COND_V50();
            struCond.byCardNo = new byte[32];
            struCond.byEmployeeNo = new byte[32];
            struCond.byEnableCardReader = new byte[512];
            struCond.dwSize = (uint)Marshal.SizeOf(struCond);
            uint.TryParse(textBoxNumber.Text, out struCond.dwFingerPrintNum);
            if(struCond.dwFingerPrintNum > listViewFingerPrint.Items.Count)
            {
                struCond.dwFingerPrintNum = (uint)listViewFingerPrint.Items.Count;
                textBoxNumber.Text = struCond.dwFingerPrintNum.ToString();
            }
            if (checkBoxNonBlock.Checked)
            {
                struCond.byCallbackMode = 1;
            }
            else
            {
                struCond.byCallbackMode = 0;
            }
            int dwSize = Marshal.SizeOf(struCond);
            IntPtr ptrStruCond = Marshal.AllocHGlobal(dwSize);
            uint.TryParse(textBoxNumber.Text, out struCond.dwFingerPrintNum);
            byte[] byTempEmployeeNo = System.Text.Encoding.UTF8.GetBytes(textBoxEmployeeNo.Text);
            for (int i = 0; i < byTempEmployeeNo.Length; i++)
            {
                struCond.byEmployeeNo[i] = byTempEmployeeNo[i];
            }
            byte.TryParse(textBoxFingerPrintNumber.Text, out struCond.byFingerPrintID);
            byte[] byTemp = System.Text.Encoding.UTF8.GetBytes(testBoxCard.Text);
            for (int i = 0; i < byTemp.Length; i++)
            {
                struCond.byCardNo[i] = byTemp[i];
            }
            GetTreeSel();
            struCond.byEnableCardReader = m_struFingerPrintOne.byEnableCardReader;
            Marshal.StructureToPtr(struCond, ptrStruCond, false);
            g_fSetFingerPrintCallback = new CHCNetSDK.RemoteConfigCallback(ProcessSetFingerPrintCfgCallbackData);
            m_lSetFingerPrintCfgHandle = CHCNetSDK.NET_DVR_StartRemoteConfig(m_lUserID,
                CHCNetSDK.NET_DVR_SET_FINGERPRINT_CFG_V50, ptrStruCond, dwSize, g_fSetFingerPrintCallback, this.Handle);
            if (-1 == m_lSetFingerPrintCfgHandle)
            {
                ListViewItem listItem = new ListViewItem();
                listItem.Text = "FAIL";
                string strTemp = string.Format("NET_DVR_SET_FINGERPRINT_CFG_V50 FAIL, ERROR CODE {0}", CHCNetSDK.NET_DVR_GetLastError());
                listItem.SubItems.Add(strTemp);
                this.AddList(listViewMessage, listItem);
                Marshal.FreeHGlobal(ptrStruCond);
                return;
            }
            else
            {
                ListViewItem listItem = new ListViewItem();
                listItem.Text = "SUCC";
                listItem.SubItems.Add("NET_DVR_SET_FINGERPRINT_CFG_V50");
                this.AddList(listViewMessage, listItem);
            }
            m_iSendIndex = 0;
            if (!SendFirstCard())
            {
                CHCNetSDK.NET_DVR_StopRemoteConfig(m_lSetFingerPrintCfgHandle);
                m_lSetFingerPrintCfgHandle = -1;
            }
            Marshal.FreeHGlobal(ptrStruCond);
        }

        private void buttonGet_Click(object sender, EventArgs e)
        {
            if (m_lGetFingerPrintCfgHandle != -1)
            {
                CHCNetSDK.NET_DVR_StopRemoteConfig((int)m_lGetFingerPrintCfgHandle);
            }
            listViewFingerPrint.Items.Clear();
            CHCNetSDK.NET_DVR_FINGER_PRINT_INFO_COND_V50 struCond = new CHCNetSDK.NET_DVR_FINGER_PRINT_INFO_COND_V50();
            struCond.byCardNo = new byte[32];
            struCond.byEmployeeNo = new byte[32];
            struCond.dwSize = (uint)Marshal.SizeOf(struCond);
            uint.TryParse(textBoxNumber.Text, out struCond.dwFingerPrintNum);
            byte[] byTempEmployeeNo = System.Text.Encoding.UTF8.GetBytes(textBoxEmployeeNo.Text);
            for (int i = 0; i < byTempEmployeeNo.Length; i++)
            {
                struCond.byEmployeeNo[i] = byTempEmployeeNo[i];
            }
            byte.TryParse(textBoxFingerPrintNumber.Text, out struCond.byFingerPrintID);
            byte[] byTemp = System.Text.Encoding.UTF8.GetBytes(testBoxCard.Text);
            for (int i = 0; i < byTemp.Length; i++)
            {
                struCond.byCardNo[i] = byTemp[i];
            }
            GetTreeSel();
            struCond.byEnableCardReader = m_struFingerPrintOne.byEnableCardReader;
            int dwSize = Marshal.SizeOf(struCond);
            IntPtr ptrStruCond = Marshal.AllocHGlobal(dwSize);
            Marshal.StructureToPtr(struCond, ptrStruCond, false);
            g_fGetFingerPrintCallback = new CHCNetSDK.RemoteConfigCallback(ProcessGetFingerPrintCfgCallbackData);
            m_lGetFingerPrintCfgHandle = CHCNetSDK.NET_DVR_StartRemoteConfig(m_lUserID, CHCNetSDK.NET_DVR_GET_FINGERPRINT_CFG_V50, ptrStruCond, dwSize, g_fGetFingerPrintCallback, this.Handle);
            if (-1 == m_lGetFingerPrintCfgHandle)
            {
                ListViewItem listItem = new ListViewItem();
                listItem.Text = "FAIL";
                string strTemp = string.Format("NET_DVR_GET_FINGERPRINT_CFG_V50 FAIL, ERROR CODE {0}", CHCNetSDK.NET_DVR_GetLastError());
                listItem.SubItems.Add(strTemp);
                this.AddList(listViewMessage, listItem);
                Marshal.FreeHGlobal(ptrStruCond);
                return;
            }
            else
            {
                ListViewItem listItem = new ListViewItem();
                listItem.Text = "SUCC";
                listItem.SubItems.Add("NET_DVR_GET_FINGERPRINT_CFG_V50");
                this.AddList(listViewMessage, listItem);
                Marshal.FreeHGlobal(ptrStruCond);
            }
        }

        private void buttonDelete_Click(object sender, EventArgs e)
        {
            if (-1 != m_lDelFingerPrintCfHandle)
            {
                if (CHCNetSDK.NET_DVR_StopRemoteConfig(m_lDelFingerPrintCfHandle))
                {
                    m_lDelFingerPrintCfHandle = -1;
                }
            }
            g_fDelFingerPrintCallback = new CHCNetSDK.RemoteConfigCallback(ProcessDelFingerPrintCallback);

            if (0 == comboBoxDeleteType.SelectedIndex)
            {
                CHCNetSDK.NET_DVR_FINGER_PRINT_INFO_CTRL_BYCARD_V50 struDelCfg = new CHCNetSDK.NET_DVR_FINGER_PRINT_INFO_CTRL_BYCARD_V50();
                struDelCfg.byMode = (byte)comboBoxDeleteType.SelectedIndex;
                struDelCfg.struByCard.byCardNo = new byte[32];
                struDelCfg.struByCard.byEmployeeNo = new byte[32];
                byte[] byTempCardNo = System.Text.Encoding.UTF8.GetBytes(testBoxCard.Text);
                for (int i = 0; i < byTempCardNo.Length; i++)
                {
                    if (i >= struDelCfg.struByCard.byCardNo.Length)
                    {
                        break;
                    }
                    struDelCfg.struByCard.byCardNo[i] = byTempCardNo[i];
                }
                GetTreeSel();
                struDelCfg.struByCard.byEnableCardReader = m_struFingerPrintOne.byEnableCardReader;
                struDelCfg.struByCard.byFingerPrintID = m_struDelFingerPrint.struByCard.byFingerPrintID;
                byte[] byTempEmployeeNo = System.Text.Encoding.UTF8.GetBytes(textBoxEmployeeNo.Text);
                for (int i = 0; i < byTempEmployeeNo.Length; i++)
                {
                    if (i >= struDelCfg.struByCard.byEmployeeNo.Length)
                    {
                        break;
                    }
                    struDelCfg.struByCard.byEmployeeNo[i] = byTempEmployeeNo[i];
                }

                struDelCfg.dwSize = (uint)Marshal.SizeOf(struDelCfg);
                int dwSize = Marshal.SizeOf(struDelCfg);
                IntPtr ptrDelCfg = Marshal.AllocHGlobal(dwSize);
                Marshal.StructureToPtr(struDelCfg, ptrDelCfg, false);

                m_lDelFingerPrintCfHandle = CHCNetSDK.NET_DVR_StartRemoteConfig(m_lUserID, CHCNetSDK.NET_DVR_DEL_FINGERPRINT_CFG_V50, ptrDelCfg, dwSize, g_fDelFingerPrintCallback, this.Handle);
                if (m_lDelFingerPrintCfHandle == -1)
                {
                    ListViewItem listItem = new ListViewItem();
                    listItem.Text = "FAIL";
                    string strTemp = string.Format("NET_DVR_DEL_FINGERPRINT_CFG FAIL, ERROR CODE {0}", CHCNetSDK.NET_DVR_GetLastError());
                    listItem.SubItems.Add(strTemp);
                    this.AddList(listViewMessage, listItem);
                    Marshal.FreeHGlobal(ptrDelCfg);
                    return;
                }
                else
                {
                    ListViewItem listItem = new ListViewItem();
                    listItem.Text = "SUCC";
                    listItem.SubItems.Add("NET_DVR_DEL_FINGERPRINT_CFG");
                    this.AddList(listViewMessage, listItem);
                    Marshal.FreeHGlobal(ptrDelCfg);
                }
            }
            else
            {
                CHCNetSDK.NET_DVR_FINGER_PRINT_INFO_CTRL_BYREADER_V50 struDelCfg = new CHCNetSDK.NET_DVR_FINGER_PRINT_INFO_CTRL_BYREADER_V50();
                struDelCfg.byMode = (byte)comboBoxDeleteType.SelectedIndex;
                struDelCfg.struByReader.byCardNo = new byte[32];
                struDelCfg.struByReader.byEmployeeNo = new byte[32];
                uint.TryParse(textBoxReaderNo.Text, out struDelCfg.struByReader.dwCardReaderNo);
                if (checkBoxDeleteAll.Checked)
                {
                    struDelCfg.struByReader.byClearAllCard = 1;
                }
                else
                {
                    struDelCfg.struByReader.byClearAllCard = 0;
                }
                byte[] byTempCardNo = System.Text.Encoding.UTF8.GetBytes(testBoxCard.Text);
                for (int i = 0; i < byTempCardNo.Length; i++ )
                {
                    if (i >= struDelCfg.struByReader.byCardNo.Length)
                    {
                        break;
                    }
                    struDelCfg.struByReader.byCardNo[i] = byTempCardNo[i];
                }
                byte[] byTempEmployeeNo = System.Text.Encoding.UTF8.GetBytes(textBoxEmployeeNo.Text);
                for (int i = 0; i < byTempEmployeeNo.Length; i++)
                {
                    if (i >= struDelCfg.struByReader.byEmployeeNo.Length)
                    {
                        break;
                    }
                    struDelCfg.struByReader.byEmployeeNo[i] = byTempEmployeeNo[i];
                }

                struDelCfg.dwSize = (uint)Marshal.SizeOf(struDelCfg);
                int dwSize = Marshal.SizeOf(struDelCfg);
                IntPtr ptrDelCfg = Marshal.AllocHGlobal(dwSize);
                Marshal.StructureToPtr(struDelCfg, ptrDelCfg, false);
                m_lDelFingerPrintCfHandle = CHCNetSDK.NET_DVR_StartRemoteConfig(m_lUserID, CHCNetSDK.NET_DVR_DEL_FINGERPRINT_CFG_V50, ptrDelCfg, dwSize, g_fDelFingerPrintCallback, this.Handle);
                if (m_lDelFingerPrintCfHandle == -1)
                {
                    ListViewItem listItem = new ListViewItem();
                    listItem.Text = "FAIL";
                    string strTemp = string.Format("NET_DVR_DEL_FINGERPRINT_CFG FAIL, ERROR CODE {0}", CHCNetSDK.NET_DVR_GetLastError());
                    listItem.SubItems.Add(strTemp);
                    this.AddList(listViewMessage, listItem);
                    Marshal.FreeHGlobal(ptrDelCfg);
                    return;
                }
                else
                {
                    if (checkBoxDeleteAll.Checked)
                    {
                        listViewFingerPrint.Items.Clear();
                    }
                    ListViewItem listItem = new ListViewItem();
                    listItem.Text = "SUCC";
                    listItem.SubItems.Add("NET_DVR_DEL_FINGERPRINT_CFG");
                    this.AddList(listViewMessage, listItem);
                    Marshal.FreeHGlobal(ptrDelCfg);
                }
            }
        }

        private void FingerprintManage_FormClosed(object sender, FormClosedEventArgs e)
        {
            if (-1 != m_lGetFingerPrintCfgHandle)
            {
                for (int i = 0; i <= m_lGetFingerPrintCfgHandle; i++ )
                {
                    CHCNetSDK.NET_DVR_StopRemoteConfig(m_lGetFingerPrintCfgHandle);
                }
            }
            if (-1 != m_lSetFingerPrintCfgHandle)
            {
                for (int i = 0; i <= m_lSetFingerPrintCfgHandle; i++ )
                {
                    CHCNetSDK.NET_DVR_StopRemoteConfig(m_lSetFingerPrintCfgHandle);
                }
            }
        }

        private void buttonBrowse_Click(object sender, EventArgs e)
        {
            OpenFileDialog openFileDialog = new OpenFileDialog();
            openFileDialog.InitialDirectory = "c:\\";//should be attention to the path is "c:\\" not "c:\"
            openFileDialog.Filter = "Fingerprint file|*.dat|All documents|*.*";
            openFileDialog.RestoreDirectory = true;
            openFileDialog.FilterIndex = 1;
            if (openFileDialog.ShowDialog() == DialogResult.OK)
            {
                textBoxFingerprintDataPath.Text = openFileDialog.FileName;
            }
        }

        protected override void DefWndProc(ref Message m)
        {
            switch (m.Msg)
            {
                case 1001:
                    if (m_lSetFingerPrintCfgHandle != -1)
                    {
                        CHCNetSDK.NET_DVR_StopRemoteConfig(m_lSetFingerPrintCfgHandle);
                        m_lSetFingerPrintCfgHandle = -1;
                    }
                    break;
                default:
                    base.DefWndProc(ref m);
                    break;
            }
        }

        private void FingerPrintMgr_Load(object sender, EventArgs e)
        {
            MultiLanguage.LoadLanguage(this);
        }
    }
}
