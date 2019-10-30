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

using AcsDemo.Public;
using AcsDemo.Language;

namespace AcsDemo.ProductAcs
{
    public partial class CardManagement : Form
    {
        public const int iMaxCardNum = 100100;
        public const int MAX_RESEND_CARD_TIME = 5;
        public Int32 m_lUserID = -1;
        public Int32 m_lGetCardCfgHandle = -1;
        public Int32 m_lSetCardCfgHandle = -1;
        public Int32 m_iSelectIndex = -1;
        public uint m_dwCardNum = 0;
        public bool m_BSendSel = false;
        public uint m_dwSendIndex = 0;
        public Int32 m_iLastCardSendTime = 0;
        public bool m_bSendOne = false;
        CHCNetSDK.NET_DVR_CARD_CFG_V50 m_struNowSendCard;
        CHCNetSDK.NET_DVR_CARD_CFG_V50 m_struSelSendCardCfg;
        CHCNetSDK.NET_DVR_CARD_CFG_V50[] m_struCardInfo = new CHCNetSDK.NET_DVR_CARD_CFG_V50[iMaxCardNum];
        Thread m_pDisplayListThread = null;

        private CHCNetSDK.RemoteConfigCallback g_fGetGatewayCardCallback = null;
        private CHCNetSDK.RemoteConfigCallback g_fSetGatewayCardCallback = null;
        private delegate void AddListCallBack(ListView cListView, ListViewItem cListViewItem, bool bReverse);
        private delegate string GetListTextCallBack(ListView cListView, int iItemTag, int iSubitemTag);

        public CardManagement()
        {
            InitializeComponent();
            this.SuspendLayout();//临时挂起控件，
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
            this.columnHeader23.Name = "columnHeader23";
            this.columnHeader14.Name = "columnHeader14";
            this.columnHeader15.Name = "columnHeader15";
            this.columnHeader16.Name = "columnHeader16";
            this.columnHeader17.Name = "columnHeader17";
            this.columnHeader17.Name = "columnHeader17";
            this.columnHeader18.Name = "columnHeader18";
            this.columnHeader19.Name = "columnHeader19";
            this.columnHeader20.Name = "columnHeader20";
            this.columnHeader21.Name = "columnHeader21";
            this.columnHeader22.Name = "columnHeader22";
            this.ResumeLayout();
        }
        private int GetExistItem(CHCNetSDK.NET_DVR_CARD_CFG_V50 struCardCfg)
        {
            int iItemCount = listViewGataManage.Items.Count;
            CHCNetSDK.NET_DVR_CARD_CFG_V50? struTemp = struCardCfg;
            for (int i = 0; i < iItemCount; i++ )
            {
                if (null == struTemp)
                {
                    continue;
                }
                if (System.Text.Encoding.UTF8.GetString(struCardCfg.byCardNo) == this.GetListText(listViewGataManage, i, 1))
                {
                    return i;
                }
            }
            return -1;
        }

        private string GetListText(ListView cListView, int iItemTag, int iSubitemTag)
        {
            if (this.InvokeRequired)
            {
                GetListTextCallBack add = new GetListTextCallBack(GetListText);
                this.Invoke(add, new object[] { cListView, iItemTag, iSubitemTag });
            }
            else
            {
                return cListView.Items[iItemTag].SubItems[iSubitemTag].Text;
            }
            return null;
        }

        private void AddToCardList(CHCNetSDK.NET_DVR_CARD_CFG_V50 struCardInfo, string strCardNo)
        {
            int iItemIndex = GetExistItem(struCardInfo);
            if (-1 == iItemIndex)
            {
                iItemIndex = listViewGataManage.Items.Count;
            }
            UpdateList(iItemIndex, strCardNo, struCardInfo);
            m_struCardInfo[iItemIndex] = struCardInfo;
        }

        private void SendCardData(CHCNetSDK.NET_DVR_CARD_CFG_V50 struCardCfg, uint dwDiffTime = 0)
        {
            if (-1 == m_lSetCardCfgHandle)
            {
                return;
            }
            uint dwSize = (uint)Marshal.SizeOf(struCardCfg);
            IntPtr ptrStruCard = Marshal.AllocHGlobal((int)dwSize);
            Marshal.StructureToPtr(struCardCfg, ptrStruCard, false);
            if (!CHCNetSDK.NET_DVR_SendRemoteConfig(m_lSetCardCfgHandle, 3, ptrStruCard, dwSize))
            {
                Marshal.FreeHGlobal(ptrStruCard);
                return;
            }
            Marshal.FreeHGlobal(ptrStruCard);
            return;
        }

        private bool SendFirstCard()
        {
            if (-1 == m_lSetCardCfgHandle)
            {
                return false;
            }
            if (m_BSendSel)
            {
                SendCardData(m_struSelSendCardCfg);
                m_BSendSel = false;
                m_struSelSendCardCfg = new CHCNetSDK.NET_DVR_CARD_CFG_V50();
                return true;
            }
            m_dwSendIndex = 0;
            if (m_dwCardNum < 1)
            {
                return false;
            }
            m_struNowSendCard = m_struCardInfo[m_dwSendIndex];
            uint dwSize = (uint)Marshal.SizeOf(m_struNowSendCard);
            IntPtr ptrSendCard = Marshal.AllocHGlobal((int)dwSize);
            Marshal.StructureToPtr(m_struNowSendCard, ptrSendCard, false);
            if (!CHCNetSDK.NET_DVR_SendRemoteConfig(m_lSetCardCfgHandle, 3, ptrSendCard, dwSize))
            {
                Marshal.FreeHGlobal(ptrSendCard);
                ListViewItem listItem = new ListViewItem();
                listItem.Text = "FAIL";
                string strTemp = null;
                strTemp = string.Format("Send Fail,CardNO:{0}", System.Text.Encoding.UTF8.GetString(m_struNowSendCard.byCardNo));
                listItem.SubItems.Add(strTemp);
                this.AddList(listViewMessage, listItem, true);
                return false;
            }
            Marshal.FreeHGlobal(ptrSendCard);
            return true;
        }

        private void btnDelSingleCard_Click(object sender, EventArgs e)
        {
            CHCNetSDK.NET_DVR_CARD_CFG_V50 DelSingleCard = new CHCNetSDK.NET_DVR_CARD_CFG_V50();
            DelSingleCard = GetSelItem();
            DelSingleCard.byCardValid = 0;
            DelSingleCard.dwSize = (uint)Marshal.SizeOf(DelSingleCard);
            
            int dwSize=(int)DelSingleCard.dwSize;
            IntPtr PtrDelSingleCard = Marshal.AllocHGlobal(dwSize);
            Marshal.StructureToPtr(DelSingleCard,PtrDelSingleCard,true);

        }
        private bool ReSendLastCard()
        {
            if (-1 == m_lSetCardCfgHandle)
            {
                return false;
            }
            m_iLastCardSendTime++;
            if (m_iLastCardSendTime >= MAX_RESEND_CARD_TIME)
            {
                return false;
            }
            uint dwSize = (uint)Marshal.SizeOf(m_struNowSendCard);
            IntPtr ptrSendCard = Marshal.AllocHGlobal((int)dwSize);
            Marshal.StructureToPtr(m_struNowSendCard, ptrSendCard, false);
            if (!CHCNetSDK.NET_DVR_SendRemoteConfig(m_lSetCardCfgHandle, 3, ptrSendCard, dwSize))
            {
                Marshal.FreeHGlobal(ptrSendCard);
                ListViewItem listItem = new ListViewItem();
                listItem.Text = "FAIL";
                string strTemp = null;
                strTemp = string.Format("Send Fail,CardNO:{0}", System.Text.Encoding.UTF8.GetString(m_struNowSendCard.byCardNo).TrimEnd('\0'));
                listItem.SubItems.Add(strTemp);
                this.AddList(listViewMessage, listItem, true);
                return false;
            }
            Marshal.FreeHGlobal(ptrSendCard);
            return true;
        }

        private bool SendNextCard()
        {
            if (-1 == m_lSetCardCfgHandle)
            {
                return false;
            }
            m_dwSendIndex++;
            if (m_dwSendIndex >= m_dwCardNum)
            {
                //CHCNetSDK.NET_DVR_StopRemoteConfig(m_lSetCardCfgHandle);
                //this.StopRemoteCfg(ref m_lSetCardCfgHandle);
                //m_lSetCardCfgHandle = -1;
                ListViewItem listItem2 = new ListViewItem();
                listItem2.Text = "SUCC";
                string strTemp2 = null;
                strTemp2 = string.Format("Send {0} card(s) over", m_dwCardNum);
                listItem2.SubItems.Add(strTemp2);
                this.AddList(listViewMessage, listItem2, true);
                return true;
            }
            m_struNowSendCard = m_struCardInfo[m_dwSendIndex];
            uint dwSize = (uint)Marshal.SizeOf(m_struNowSendCard);
            IntPtr ptrSendCard = Marshal.AllocHGlobal((int)dwSize);
            Marshal.StructureToPtr(m_struNowSendCard, ptrSendCard, false);
            if (!CHCNetSDK.NET_DVR_SendRemoteConfig(m_lSetCardCfgHandle, 3, ptrSendCard, dwSize))
            {
                Marshal.FreeHGlobal(ptrSendCard);
                ListViewItem listItem3 = new ListViewItem();
                listItem3.Text = "FAIL";
                string strTemp3 = null;
                strTemp3 = string.Format("Send Fail,CardNO:{0}", System.Text.Encoding.UTF8.GetString(m_struNowSendCard.byCardNo).TrimEnd('\0'));
                listItem3.SubItems.Add(strTemp3);
                this.AddList(listViewMessage, listItem3, true);
                //CHCNetSDK.NET_DVR_StopRemoteConfig(m_lSetCardCfgHandle);
                //this.StopRemoteCfg(ref m_lSetCardCfgHandle);
                //m_lSetCardCfgHandle = -1;
                return false;
            }
            Marshal.FreeHGlobal(ptrSendCard);
            return true;
        }

        private CHCNetSDK.NET_DVR_CARD_CFG_V50 GetSelItem()
        {
            int iPos = -1;
            if (m_iSelectIndex >= 0)
            {
                iPos = m_iSelectIndex;
            }
            CHCNetSDK.NET_DVR_CARD_CFG_V50 struCardInfo = new CHCNetSDK.NET_DVR_CARD_CFG_V50();
            if (iPos < 0)
            {
                return struCardInfo;
            }
            return m_struCardInfo[iPos];
        }

        private CHCNetSDK.NET_DVR_CARD_CFG_V50 GetSelItem(ref string strCardNo)
        {
            int iPos = m_iSelectIndex;
            CHCNetSDK.NET_DVR_CARD_CFG_V50 struCardInfo = new CHCNetSDK.NET_DVR_CARD_CFG_V50();
            if (iPos < 0)
            {
                return struCardInfo;
            }
            if (null != strCardNo)
            {
                strCardNo = listViewGataManage.Items[iPos].SubItems[1].Text;
            }
            return m_struCardInfo[iPos];
        }


        private void ProcessGetGatewayCardCallback(uint dwType, IntPtr lpBuffer, uint dwBufLen, IntPtr pUserData)
        {
            if (pUserData == null)
            {
                return;
            }

            if (dwType == (uint)CHCNetSDK.NET_SDK_CALLBACK_TYPE.NET_SDK_CALLBACK_TYPE_DATA)
            {
                CHCNetSDK.NET_DVR_CARD_CFG_V50 struCardCfg = new CHCNetSDK.NET_DVR_CARD_CFG_V50();
                struCardCfg = (CHCNetSDK.NET_DVR_CARD_CFG_V50)Marshal.PtrToStructure(lpBuffer, typeof(CHCNetSDK.NET_DVR_CARD_CFG_V50));
                string strCardNo = System.Text.Encoding.UTF8.GetString(struCardCfg.byCardNo);
                IntPtr pCardInfo =  Marshal.AllocHGlobal(Marshal.SizeOf(struCardCfg));
                Marshal.StructureToPtr(struCardCfg, pCardInfo, true);
                CHCNetSDK.PostMessage(pUserData, 1003, (int)pCardInfo, 0);

                //AddToCardList(struCardCfg, strCardNo);
            }
            else if (dwType == (uint)CHCNetSDK.NET_SDK_CALLBACK_TYPE.NET_SDK_CALLBACK_TYPE_STATUS)
            {
                uint dwStatus = (uint)Marshal.ReadInt32(lpBuffer);
                if (dwStatus == (uint)CHCNetSDK.NET_SDK_CALLBACK_STATUS_NORMAL.NET_SDK_CALLBACK_STATUS_SUCCESS)
                {
                    ListViewItem listItem = new ListViewItem();
                    listItem.Text = "SUCC";
                    listItem.SubItems.Add("NET_DVR_GET_CARD_CFG_V50 Get finish");
                    this.AddList(listViewMessage, listItem, true);
                    CHCNetSDK.PostMessage(pUserData, 1002, 0, 0);
                }
                else if (dwStatus == (uint)CHCNetSDK.NET_SDK_CALLBACK_STATUS_NORMAL.NET_SDK_CALLBACK_STATUS_FAILED)
                {
                    uint dwErrorCode = (uint)Marshal.ReadInt32(lpBuffer + 1);
                    string cardNumber = Marshal.PtrToStringAnsi(lpBuffer + 2);
                    ListViewItem listItem = new ListViewItem();
                    listItem.Text = "FAIL";
                    listItem.SubItems.Add(string.Format("NET_DVR_GET_CARD_CFG_V50 Get Failed,ErrorCode:{0},CardNo:{1}", dwErrorCode, cardNumber));
                    this.AddList(listViewMessage, listItem, true);
                    CHCNetSDK.PostMessage(pUserData, 1002, 0, 0);
                }
            }
            return;
        }

        private void ProcessSetGatewayCardCallback(uint dwType, IntPtr lpBuffer, uint dwBufLen, IntPtr pUserData)
        {
            if (pUserData == null)
            {
                return;
            }
            uint dwStatus = (uint)Marshal.ReadInt32(lpBuffer);
            
            if (dwStatus == (uint)CHCNetSDK.NET_SDK_CALLBACK_STATUS_NORMAL.NET_SDK_CALLBACK_STATUS_PROCESSING)
            {
                // just example need refinement
                ListViewItem listItem = new ListViewItem();
                listItem.Text = "SUCC";
                string strTemp = null; 
                if (m_bSendOne)
                {
                    strTemp = string.Format("Send SUCC,CardNO:{0}", System.Text.Encoding.UTF8.GetString(m_struCardInfo[m_iSelectIndex].byCardNo).TrimEnd('\0'));
                    m_bSendOne = false;
                }
                else
                {
                    strTemp = string.Format("Send SUCC,CardNO:{0}", System.Text.Encoding.UTF8.GetString(m_struCardInfo[m_dwSendIndex].byCardNo).TrimEnd('\0'));
                }
                listItem.SubItems.Add(strTemp);
                this.AddList(listViewMessage, listItem, true);
                //next
                CHCNetSDK.PostMessage(pUserData, 1002, 0, 0);
            }
            else if (dwStatus == (uint)CHCNetSDK.NET_SDK_CALLBACK_STATUS_NORMAL.NET_SDK_CALLBACK_STATUS_FAILED)
            {
                ListViewItem listItem = new ListViewItem();
                listItem.Text = "FAIL";
                string strTemp = null;
                uint errorCode = (uint)Marshal.ReadInt32(lpBuffer + 4);
                uint errorUserID = (uint)Marshal.ReadInt32(lpBuffer + 40);
                if (m_iSelectIndex!=-1)
                {
                    strTemp = string.Format("Send FAILED,CardNO:{0},Error code{1},USER ID{2}", System.Text.Encoding.UTF8.GetString(m_struCardInfo[m_iSelectIndex].byCardNo).TrimEnd('\0'), errorCode, errorUserID);
                    listItem.SubItems.Add(strTemp);
                    this.AddList(listViewMessage, listItem, true);
                }
                //next
                CHCNetSDK.PostMessage(pUserData, 1002, 0, 0);
            }
            else if (dwStatus == (uint)CHCNetSDK.NET_SDK_CALLBACK_STATUS_NORMAL.NET_SDK_CALLBACK_STATUS_SUCCESS)
            {
                ListViewItem listItem = new ListViewItem();
                listItem.Text = "SUCC";
                listItem.SubItems.Add("NET_DVR_SET_CARD_CFG_V50 Set finish");
                this.AddList(listViewMessage, listItem, true);
                CHCNetSDK.PostMessage(pUserData, 1001, 0, 0);
            }
            else if (dwStatus == (uint)CHCNetSDK.NET_SDK_CALLBACK_STATUS_NORMAL.NET_SDK_CALLBACK_STATUS_EXCEPTION)
            {
                ListViewItem listItem = new ListViewItem();
                listItem.Text = "FAIL";
                listItem.SubItems.Add("NET_DVR_SET_CARD_CFG_V50 Set FAIL");
                this.AddList(listViewMessage, listItem, true);
                CHCNetSDK.PostMessage(pUserData, 1001, 0, 0);
            }
            return;
        }

        private void AddList(ListView cListView, ListViewItem cListViewItem, bool bReverse)
        {
            if (this.InvokeRequired)
            {
                AddListCallBack add = new AddListCallBack(AddList);
                this.Invoke(add, new object[] { cListView, cListViewItem, bReverse});
            }
            else
            {
                if (bReverse)
                {
                    cListView.Items.Insert(0, cListViewItem);
                }
                else
                {
                    cListView.Items.Add(cListViewItem);
                }
                ////cListView.Items.Insert(0, cListViewItem);
                //cListView.Items.Add(cListViewItem);
            }
        }

        private void UpdateSubList(CHCNetSDK.NET_DVR_CARD_CFG_V50 struCardCfg)
        {
            listViewDoorAuthority.Items.Clear();
            listViewGroup.Items.Clear();
            listViewPlaTemplateNumber.Items.Clear();
            string strTemp1 = null;
            string strTemp2 = null;
            string strTemp3 = null;

            for (int i = 0; i < CHCNetSDK.MAX_DOOR_NUM_256; i++)
            {
                strTemp1 = string.Format("{0}", i + 1);
                ListViewItem listItem = new ListViewItem();
                listItem.Text = strTemp1;
                if (struCardCfg.byDoorRight[i] > 0)
                {
                    listItem.SubItems.Add("Yes");
                }
                else
                {
                    listItem.SubItems.Add("No");
                }
                listViewDoorAuthority.Items.Add(listItem);
            }
            for (int i = 0; i < CHCNetSDK.MAX_GROUP_NUM_128; i++)
            {
                strTemp1 = string.Format("{0}", i + 1);
                ListViewItem listItem = new ListViewItem();
                listItem.Text = strTemp1;
                if (struCardCfg.byBelongGroup[i] > 0)
                {
                    listItem.SubItems.Add("Yes");
                }
                else
                {
                    listItem.SubItems.Add("No");
                }
                listViewGroup.Items.Add(listItem);
            }
            for (int i = 0; i < CHCNetSDK.MAX_DOOR_NUM_256; i++)
            {
                for (int j = 0; j < CHCNetSDK.MAX_CARD_RIGHT_PLAN_NUM; j++)
                {
                    strTemp1 = string.Format("{0}", i + 1);
                    strTemp2 = string.Format("{0}", j + 1);
                    strTemp3 = string.Format("{0}", struCardCfg.wCardRightPlan[i * CHCNetSDK.MAX_CARD_RIGHT_PLAN_NUM + j]);
                    ListViewItem listItem = new ListViewItem();
                    listItem.Text = strTemp1;
                    listItem.SubItems.Add(strTemp2);
                    listItem.SubItems.Add(strTemp3);
                    listViewPlaTemplateNumber.Items.Add(listItem);
                }
            }
        }

        private void CreateSameCfgCard(uint dwNum, CHCNetSDK.NET_DVR_CARD_CFG_V50 struCardCfg)
        {
            int iListCount = listViewGataManage.Items.Count;
            for (int i = iListCount; i < iListCount + dwNum; i++ )
            {
                m_struCardInfo[i] = struCardCfg;
                m_struCardInfo[i].byCardNo = new byte[CHCNetSDK.ACS_CARD_NO_LEN];
                string strTemp = (i - iListCount).ToString();
                byte[] byTempNo = System.Text.Encoding.UTF8.GetBytes(strTemp);
                int iIndex = 0;
                for (int k = 0; k < struCardCfg.byCardNo.Length; k++ )
                {
                    if (0 == struCardCfg.byCardNo[k])
                    {
                        break;
                    }
                    else
                    {
                        m_struCardInfo[i].byCardNo[iIndex] = struCardCfg.byCardNo[k];
                        iIndex++;
                    }
                    if (iIndex >= m_struCardInfo[i].byCardNo.Length)
                    {
                        return;
                    }
                }
                if (dwNum != 1)
                {
                    for (int k = 0; k < byTempNo.Length; k++)
                    {
                        m_struCardInfo[i].byCardNo[iIndex] = byTempNo[k];
                        iIndex++;
                        if (iIndex >= m_struCardInfo[i].byCardNo.Length)
                        {
                            return;
                        }
                    }
                }
            }
        }

        private void buttonExit_Click(object sender, EventArgs e)
        {
            this.Close();
            return ;
        }

        private void UpdateList(int iInsertIndex, string strCardNo, CHCNetSDK.NET_DVR_CARD_CFG_V50 struCardInfo)
        {
            ListViewItem lvItem;
            lvItem = new ListViewItem();
            string strTemp1 = null; 
            string strTemp2 = null; 
            string strItem = null;
            strItem = (iInsertIndex + 1).ToString();
            lvItem.Text = strItem;
            lvItem.SubItems.Add(System.Text.Encoding.UTF8.GetString(struCardInfo.byCardNo));
            if (1 == struCardInfo.byCardValid)
            {
                strItem = "Yes";
            }
            else
            {
                strItem = "No";
            }
            lvItem.SubItems.Add(strItem);
            lvItem.SubItems.Add(System.Text.Encoding.UTF8.GetString(struCardInfo.byCardPassword));
            if (struCardInfo.byCardType == 0 || struCardInfo.byCardType > 7)
            {
                lvItem.SubItems.Add(AcsDemoPublic.strCardType[0]);
            }
            else
            {
                lvItem.SubItems.Add(AcsDemoPublic.strCardType[struCardInfo.byCardType]);
            }
            if (1 == struCardInfo.byLeaderCard)
            {
                strItem = "Yes";
            }
            else
            {
                strItem = "No";
            }
            lvItem.SubItems.Add(strItem);
            lvItem.SubItems.Add(struCardInfo.dwMaxSwipeTime.ToString());
            lvItem.SubItems.Add(struCardInfo.dwSwipeTime.ToString());
            if (1 == struCardInfo.struValid.byEnable)
            {
                strItem = "Yes";
                CHCNetSDK.NET_DVR_TIME_EX strTimeEx1 = struCardInfo.struValid.struBeginTime;
                strTemp1 = string.Format("{0,4}-{1,0:D2}-{2,0:D2}, {3,0:D2}:{4,0:D2}:{5,0:D2}", strTimeEx1.wYear, strTimeEx1.byMonth,
                    strTimeEx1.byDay, strTimeEx1.byHour, strTimeEx1.byMinute, strTimeEx1.bySecond);
                CHCNetSDK.NET_DVR_TIME_EX strTimeEx2 = struCardInfo.struValid.struEndTime;
                strTemp2 = string.Format("{0,4}-{1,0:D2}-{2,0:D2}, {3,0:D2}:{4,0:D2}:{5,0:D2}", strTimeEx2.wYear, strTimeEx2.byMonth,
                    strTimeEx2.byDay, strTimeEx2.byHour, strTimeEx2.byMinute, strTimeEx2.bySecond);
            }
            else
            {
                strItem = "No";
            }
            lvItem.SubItems.Add(strItem);
            lvItem.SubItems.Add(strTemp1);
            lvItem.SubItems.Add(strTemp2);
            lvItem.SubItems.Add("");
            lvItem.SubItems.Add(struCardInfo.wRoomNumber.ToString());
            lvItem.SubItems.Add(struCardInfo.wFloorNumber.ToString());




            this.AddList(listViewGataManage, lvItem, false);
        }

        private void BatchAddCardToList()
        {
            int iItemCount = listViewGataManage.Items.Count;
            for (int i = iItemCount; i < iItemCount + m_dwCardNum; i++)
            {
                UpdateList(i, null, m_struCardInfo[i]);
            }
        }

        private void g_fShowCardListThread()
        {
            this.BatchAddCardToList();
            return;
        }

        private void buttonBatchAdd_Click(object sender, EventArgs e)
        {
            string strCardNo = null;
            CardConfigure batchAdd = new CardConfigure();
            CHCNetSDK.NET_DVR_CARD_CFG_V50 struCardCfg = new CHCNetSDK.NET_DVR_CARD_CFG_V50();
            batchAdd.SetCardCfg(ref struCardCfg);

            if (batchAdd.ShowDialog() == DialogResult.OK)
            {
                struCardCfg = batchAdd.GetCardCfg(strCardNo);
                m_dwCardNum = (uint)batchAdd.GetAddNum();
                CreateSameCfgCard(m_dwCardNum, struCardCfg);
                m_pDisplayListThread = new Thread(new ThreadStart(g_fShowCardListThread));
                m_pDisplayListThread.Start();
            }

        }

        private void buttonGetAll_Click(object sender, EventArgs e)
        {
            if (-1 != m_lGetCardCfgHandle)
            {
                if (CHCNetSDK.NET_DVR_StopRemoteConfig(m_lGetCardCfgHandle))
                {
                    m_lGetCardCfgHandle = -1;
                }
            }
            listViewGataManage.Items.Clear();
            CHCNetSDK.NET_DVR_CARD_CFG_COND struCond = new CHCNetSDK.NET_DVR_CARD_CFG_COND();
            struCond.dwSize = (uint)Marshal.SizeOf(struCond);
            ushort.TryParse(textBoxControllerNo.Text, out struCond.wLocalControllerID);
            struCond.dwCardNum = 0xffffffff;
            if (checkBoxCheckNumber.Checked)
            {
                struCond.byCheckCardNo = 1;
            }
            else
            {
                struCond.byCheckCardNo = 0;
            }

            int dwSize = Marshal.SizeOf(struCond);
            IntPtr ptrStruCond = Marshal.AllocHGlobal(dwSize);
            Marshal.StructureToPtr(struCond, ptrStruCond, false);
            g_fGetGatewayCardCallback = new CHCNetSDK.RemoteConfigCallback(ProcessGetGatewayCardCallback);

            m_lGetCardCfgHandle = CHCNetSDK.NET_DVR_StartRemoteConfig(m_lUserID, CHCNetSDK.NET_DVR_GET_CARD_CFG_V50, ptrStruCond, dwSize, g_fGetGatewayCardCallback, this.Handle);
            if (m_lGetCardCfgHandle == -1)
            {
                ListViewItem listItem = new ListViewItem();
                listItem.Text = "FAIL";
                string strTemp = string.Format("NET_DVR_GET_CARD_CFG_V50 FAIL, ERROR CODE {0}", CHCNetSDK.NET_DVR_GetLastError());
                listItem.SubItems.Add(strTemp);
                this.AddList(listViewMessage, listItem, true);
                Marshal.FreeHGlobal(ptrStruCond);
                return;
            }
            else
            {
                ListViewItem listItem = new ListViewItem();
                listItem.Text = "SUCC";
                listItem.SubItems.Add("NET_DVR_GET_CARD_CFG_V50");
                this.AddList(listViewMessage, listItem, true);
            }
            Marshal.FreeHGlobal(ptrStruCond);

        }

        private void buttonEdit_Click(object sender, EventArgs e)
        {
            if (m_iSelectIndex < 0)
            {
                MessageBox.Show("Select list item first");
                return;
            }
            string strTemp1 = null;
            string strTemp2 = null; 
            string strItem = null;
            int iSelIndex = m_iSelectIndex;
            CHCNetSDK.NET_DVR_CARD_CFG_V50 struCardInfo = m_struCardInfo[iSelIndex];
            string strCardNo = listViewGataManage.Items[iSelIndex].SubItems[1].Text;
            CardConfigure batchAdd = new CardConfigure();
            batchAdd.SetCardCfg(ref struCardInfo, strCardNo);
            if (batchAdd.ShowDialog() == DialogResult.OK)
            {
                listViewGataManage.BeginUpdate();
                struCardInfo = batchAdd.GetCardCfg(strCardNo);
                m_struCardInfo[iSelIndex] = struCardInfo;
                listViewGataManage.Items[iSelIndex].SubItems[1].Text = System.Text.Encoding.UTF8.GetString(struCardInfo.byCardNo);
                if (1 == struCardInfo.byCardValid)
                {
                    strItem = "Yes";
                }
                else
                {
                    strItem = "No";
                }
                listViewGataManage.Items[iSelIndex].SubItems[2].Text = strItem;
                listViewGataManage.Items[iSelIndex].SubItems[3].Text = System.Text.Encoding.UTF8.GetString(struCardInfo.byCardPassword);
                if (struCardInfo.byCardType == 0 || struCardInfo.byCardType > 7)
                {
                    listViewGataManage.Items[iSelIndex].SubItems[4].Text = AcsDemoPublic.strCardType[0];
                }
                else
                {
                    listViewGataManage.Items[iSelIndex].SubItems[4].Text = AcsDemoPublic.strCardType[struCardInfo.byCardType];
                }
                if (1 == struCardInfo.byLeaderCard)
                {
                    strItem = "Yes";
                }
                else
                {
                    strItem = "No";
                }
                listViewGataManage.Items[iSelIndex].SubItems[5].Text = strItem;
                listViewGataManage.Items[iSelIndex].SubItems[6].Text = struCardInfo.dwMaxSwipeTime.ToString();
                listViewGataManage.Items[iSelIndex].SubItems[7].Text = struCardInfo.dwSwipeTime.ToString();
                if (1 == struCardInfo.struValid.byEnable)
                {
                    strItem = "Yes";
                    CHCNetSDK.NET_DVR_TIME_EX strTimeEx1 = struCardInfo.struValid.struBeginTime;
                    strTemp1 = string.Format("{0,4}-{1,2}-{2,2}, {3,2}:{4,2}:{5,2}", strTimeEx1.wYear, strTimeEx1.byMonth,
                        strTimeEx1.byDay, strTimeEx1.byHour, strTimeEx1.byMinute, strTimeEx1.bySecond);
                    CHCNetSDK.NET_DVR_TIME_EX strTimeEx2 = struCardInfo.struValid.struEndTime;
                    strTemp2 = string.Format("{0,4}-{1,2}-{2,2}, {3,2}:{4,2}:{5,2}", strTimeEx2.wYear, strTimeEx2.byMonth,
                        strTimeEx2.byDay, strTimeEx2.byHour, strTimeEx2.byMinute, strTimeEx2.bySecond);
                }
                else
                {
                    strItem = "No";
                }
                listViewGataManage.Items[iSelIndex].SubItems[8].Text = strItem;
                listViewGataManage.Items[iSelIndex].SubItems[9].Text = strTemp1;
                listViewGataManage.Items[iSelIndex].SubItems[10].Text = strTemp2;
                listViewGataManage.Items[iSelIndex].SubItems[11].Text = struCardInfo.wRoomNumber.ToString();
                listViewGataManage.EndUpdate();
            }
        }

        private void listViewGataManage_ItemSelectionChanged(object sender, ListViewItemSelectionChangedEventArgs e)
        {
            m_iSelectIndex = e.ItemIndex;
            if (e.ItemIndex < 0)
            {
                return;
            }
            CHCNetSDK.NET_DVR_CARD_CFG_V50 strCardCfg = m_struCardInfo[e.ItemIndex];
            UpdateSubList(strCardCfg);
        }

        private void buttonSendAll_Click(object sender, EventArgs e)
        {
            if (-1 != m_lSetCardCfgHandle)
            {
                if (CHCNetSDK.NET_DVR_StopRemoteConfig(m_lSetCardCfgHandle))
                {
                    m_lSetCardCfgHandle = -1;
                }
            }
            m_dwCardNum = (uint)listViewGataManage.Items.Count;
            if (listViewGataManage.Items.Count < 1)
            {
                MessageBox.Show("Please add some cards!");
                return;
            }
            CHCNetSDK.NET_DVR_CARD_CFG_COND struCond = new CHCNetSDK.NET_DVR_CARD_CFG_COND();
            struCond.dwSize = (uint)Marshal.SizeOf(struCond);
            struCond.dwCardNum = m_dwCardNum;
            ushort.TryParse(textBoxControllerNo.Text, out struCond.wLocalControllerID);
            if (checkBoxCheckNumber.Checked)
            {
                struCond.byCheckCardNo = 1;
            }
            else
            {
                struCond.byCheckCardNo = 0;
            }

            int dwSize = Marshal.SizeOf(struCond);
            IntPtr ptrStruCond = Marshal.AllocHGlobal(dwSize);
            Marshal.StructureToPtr(struCond, ptrStruCond, false);
            g_fSetGatewayCardCallback = new CHCNetSDK.RemoteConfigCallback(ProcessSetGatewayCardCallback);
            m_lSetCardCfgHandle = CHCNetSDK.NET_DVR_StartRemoteConfig(m_lUserID, CHCNetSDK.NET_DVR_SET_CARD_CFG_V50, ptrStruCond, dwSize, g_fSetGatewayCardCallback, this.Handle);
            if (-1 == m_lSetCardCfgHandle)
            {
                ListViewItem listItem = new ListViewItem();
                listItem.Text = "FAIL";
                string strTemp = string.Format("NET_DVR_SET_CARD_CFG_V50 FAIL, ERROR CODE {0}", CHCNetSDK.NET_DVR_GetLastError());
                listItem.SubItems.Add(strTemp);
                this.AddList(listViewMessage, listItem, true);
                Marshal.FreeHGlobal(ptrStruCond);
                return;
            }
            else
            {
                ListViewItem listItem = new ListViewItem();
                listItem.Text = "SUCC";
                listItem.SubItems.Add("NET_DVR_SET_CARD_CFG_V50");
                this.AddList(listViewMessage, listItem, true);
            }
            Marshal.FreeHGlobal(ptrStruCond);
            if (!SendFirstCard())
            {
                CHCNetSDK.NET_DVR_StopRemoteConfig(m_lSetCardCfgHandle);
                m_lSetCardCfgHandle = -1;
            }
        }

        private void buttonObtainIndividual_Click(object sender, EventArgs e)
        {
            //just get only one,not one by one, so close link first
            if (-1 != m_lGetCardCfgHandle)
            {
                if (CHCNetSDK.NET_DVR_StopRemoteConfig(m_lGetCardCfgHandle))
                {
                    m_lGetCardCfgHandle = -1;
                }
            }

            CHCNetSDK.NET_DVR_CARD_CFG_SEND_DATA struSendData = new CHCNetSDK.NET_DVR_CARD_CFG_SEND_DATA();
            struSendData.dwSize = (uint)Marshal.SizeOf(struSendData);
            if (m_iSelectIndex < 0)
            {
                byte[] byCardNo= System.Text.Encoding.Default.GetBytes(textBoxCardNo.Text);
                struSendData.byCardNo = new byte[32];
                byCardNo.CopyTo(struSendData.byCardNo, 0);
            }
            else 
            {
                if (null == struSendData.byCardNo)
                {
                    struSendData.byCardNo = GetSelItem().byCardNo;
                }
                else
                {
                    string strTemp = System.Text.Encoding.UTF8.GetString(struSendData.byCardNo);
                    GetSelItem(ref strTemp);
                    struSendData.byCardNo = System.Text.Encoding.UTF8.GetBytes(strTemp);
                }
            }
           
            CHCNetSDK.NET_DVR_CARD_CFG_COND struCond = new CHCNetSDK.NET_DVR_CARD_CFG_COND();
            struCond.dwSize = (uint)Marshal.SizeOf(struCond);
            struCond.dwCardNum = 1;
            ushort.TryParse(textBoxControllerNo.Text, out struCond.wLocalControllerID);

            if (checkBoxCheckNumber.Checked)
            {
                struCond.byCheckCardNo = 1;
            }
            else
            {
                struCond.byCheckCardNo = 0;
            }

            int dwSize = Marshal.SizeOf(struCond);
            IntPtr ptrStruCond = Marshal.AllocHGlobal(dwSize);
            Marshal.StructureToPtr(struCond, ptrStruCond, false);
            g_fGetGatewayCardCallback = new CHCNetSDK.RemoteConfigCallback(ProcessGetGatewayCardCallback);
            m_lGetCardCfgHandle = CHCNetSDK.NET_DVR_StartRemoteConfig(m_lUserID, CHCNetSDK.NET_DVR_GET_CARD_CFG_V50, ptrStruCond, dwSize, g_fGetGatewayCardCallback, this.Handle);
            if (-1 == m_lGetCardCfgHandle)
            {
                ListViewItem listItem = new ListViewItem();
                listItem.Text = "FAIL";
                string strTemp = string.Format("NET_DVR_GET_CARD_CFG_V50 FAIL, ERROR CODE {0}", CHCNetSDK.NET_DVR_GetLastError());
                listItem.SubItems.Add(strTemp);
                this.AddList(listViewMessage, listItem, true);
                Marshal.FreeHGlobal(ptrStruCond);
                return;
            }
            else
            {
                ListViewItem listItem = new ListViewItem();
                listItem.Text = "SUCC";
                listItem.SubItems.Add("NET_DVR_GET_CARD_CFG_V50");
                this.AddList(listViewMessage, listItem, true);
                uint dwSizeData = (uint)Marshal.SizeOf(struSendData);
                IntPtr ptrStruSendData = Marshal.AllocHGlobal((int)dwSizeData);
                Marshal.StructureToPtr(struSendData, ptrStruSendData, false);
                if (!CHCNetSDK.NET_DVR_SendRemoteConfig(m_lGetCardCfgHandle, 3, ptrStruSendData, dwSizeData))
                {
                    ListViewItem listItem2 = new ListViewItem();
                    listItem2.Text = "FAIL";
                    string strTemp = string.Format("NET_DVR_SendRemoteConfig ENUM_ACS_SEND_DATA FAIL, ERROR CODE {0}", CHCNetSDK.NET_DVR_GetLastError());
                    listItem2.SubItems.Add(strTemp);
                    this.AddList(listViewMessage, listItem2, true);
                }
                else
                {
                    ListViewItem listItem2 = new ListViewItem();
                    listItem2.Text = "SUCC";
                    listItem2.SubItems.Add("NET_DVR_SendRemoteConfig ENUM_ACS_SEND_DATA");
                    this.AddList(listViewMessage, listItem2, true);
                }
                Marshal.FreeHGlobal(ptrStruSendData);
            }
            Marshal.FreeHGlobal(ptrStruCond);
        }

        private void buttonSendIndividual_Click(object sender, EventArgs e)
        {
            if (-1 != m_lSetCardCfgHandle)
            {
                if (CHCNetSDK.NET_DVR_StopRemoteConfig(m_lSetCardCfgHandle))
                {
                    m_lSetCardCfgHandle = -1;
                }
            }
            m_bSendOne = true;
            m_struSelSendCardCfg = GetSelItem();
            if (0 == listViewGataManage.SelectedItems.Count)
            {
                MessageBox.Show("Select list item first");
                return;
            }
            CHCNetSDK.NET_DVR_CARD_CFG_COND struCond = new CHCNetSDK.NET_DVR_CARD_CFG_COND();
            struCond.dwSize = (uint)Marshal.SizeOf(struCond);
            struCond.dwCardNum = 1;
            ushort.TryParse(textBoxControllerNo.Text, out struCond.wLocalControllerID);
            if (checkBoxCheckNumber.Checked)
            {
                struCond.byCheckCardNo = 1;
            }
            else
            {
                struCond.byCheckCardNo = 0;
            }
            m_BSendSel = true;
            int dwSize = Marshal.SizeOf(struCond);
            IntPtr ptrStruCond = Marshal.AllocHGlobal(dwSize);
            Marshal.StructureToPtr(struCond, ptrStruCond, false);
            g_fSetGatewayCardCallback = new CHCNetSDK.RemoteConfigCallback(ProcessSetGatewayCardCallback);
            m_lSetCardCfgHandle = CHCNetSDK.NET_DVR_StartRemoteConfig(m_lUserID, CHCNetSDK.NET_DVR_SET_CARD_CFG_V50, ptrStruCond, dwSize, g_fSetGatewayCardCallback, this.Handle);
            if (-1 == m_lSetCardCfgHandle)
            {
                ListViewItem listItem = new ListViewItem();
                listItem.Text = "FAIL";
                string strTemp = string.Format("NET_DVR_SET_CARD_CFG_V50 FAIL, ERROR CODE {0}", CHCNetSDK.NET_DVR_GetLastError());
                listItem.SubItems.Add(strTemp);
                this.AddList(listViewMessage, listItem, true);
                Marshal.FreeHGlobal(ptrStruCond);
                return;
            }
            else
            {
                ListViewItem listItem = new ListViewItem();
                listItem.Text = "SUCC";
                listItem.SubItems.Add("NET_DVR_SET_CARD_CFG_V50");
                this.AddList(listViewMessage, listItem, true);
            }
            Marshal.FreeHGlobal(ptrStruCond);
            if (m_BSendSel)
            {
                m_dwCardNum = 1;
                m_dwSendIndex = 0;
                SendCardData(m_struSelSendCardCfg);
                m_BSendSel = false;
                m_struSelSendCardCfg = new CHCNetSDK.NET_DVR_CARD_CFG_V50();
                return;
            }
        }

        private void buttonDeleteCard_Click(object sender, EventArgs e)
        {
            if (-1 != m_lSetCardCfgHandle)
            {
                if (CHCNetSDK.NET_DVR_StopRemoteConfig(m_lSetCardCfgHandle))
                {
                    m_lSetCardCfgHandle = -1;
                }
            }
            m_bSendOne = true;
            m_struSelSendCardCfg = GetSelItem();
            if (0 == listViewGataManage.SelectedItems.Count)
            {
                MessageBox.Show("Select list item first");
                return;
            }
            CHCNetSDK.NET_DVR_CARD_CFG_COND struCond = new CHCNetSDK.NET_DVR_CARD_CFG_COND();
            struCond.dwSize = (uint)Marshal.SizeOf(struCond);
            struCond.dwCardNum = 1;
            if (checkBoxCheckNumber.Checked)
            {
                struCond.byCheckCardNo = 1;
            }
            else
            {
                struCond.byCheckCardNo = 0;
            }
            m_BSendSel = true;
            int dwSize = Marshal.SizeOf(struCond);
            IntPtr ptrStruCond = Marshal.AllocHGlobal(dwSize);
            Marshal.StructureToPtr(struCond, ptrStruCond, false);
            g_fSetGatewayCardCallback = new CHCNetSDK.RemoteConfigCallback(ProcessSetGatewayCardCallback);
            m_lSetCardCfgHandle = CHCNetSDK.NET_DVR_StartRemoteConfig(m_lUserID, CHCNetSDK.NET_DVR_SET_CARD_CFG_V50, ptrStruCond, dwSize, g_fSetGatewayCardCallback, this.Handle);
            if (-1 == m_lSetCardCfgHandle)
            {
                ListViewItem listItem = new ListViewItem();
                listItem.Text = "FAIL";
                string strTemp = string.Format("NET_DVR_SET_CARD_CFG_V50 FAIL, ERROR CODE {0}", CHCNetSDK.NET_DVR_GetLastError());
                listItem.SubItems.Add(strTemp);
                this.AddList(listViewMessage, listItem, true);
                Marshal.FreeHGlobal(ptrStruCond);
                return;
            }
            else
            {
                ListViewItem listItem = new ListViewItem();
                listItem.Text = "SUCC";
                listItem.SubItems.Add("NET_DVR_SET_CARD_CFG_V50");
                this.AddList(listViewMessage, listItem, true);
            }
            Marshal.FreeHGlobal(ptrStruCond);
            if (m_BSendSel)
            {
                m_dwCardNum = 1;
                m_dwSendIndex = 0;
                m_struSelSendCardCfg.byCardValid = 0;
                SendCardData(m_struSelSendCardCfg);
                m_BSendSel = false;
                m_struSelSendCardCfg = new CHCNetSDK.NET_DVR_CARD_CFG_V50();
                return;
            }
        }

        private void buttonDeleteAllCard_Click(object sender, EventArgs e)
        {
            CHCNetSDK.NET_DVR_ACS_PARAM_TYPE struAcsParam = new CHCNetSDK.NET_DVR_ACS_PARAM_TYPE();
            struAcsParam.dwSize = (uint)Marshal.SizeOf(struAcsParam);
            struAcsParam.dwParamType |= 1 << 12;

            uint dwSize = (uint)Marshal.SizeOf(struAcsParam);
            IntPtr ptrAcsParam = Marshal.AllocHGlobal((int)dwSize);
            Marshal.StructureToPtr(struAcsParam, ptrAcsParam, false);
            if (!CHCNetSDK.NET_DVR_RemoteControl(m_lUserID, CHCNetSDK.NET_DVR_CLEAR_ACS_PARAM, ptrAcsParam, dwSize))
            {
                ListViewItem listItem = new ListViewItem();
                listItem.Text = "FAIL";
                string strTemp = string.Format("NET_DVR_CLEAR_ACS_PARAM FAIL, ERROR CODE {0}", CHCNetSDK.NET_DVR_GetLastError());
                listItem.SubItems.Add(strTemp);
                this.AddList(listViewMessage, listItem, true);
            }
            else
            {
                ListViewItem listItem = new ListViewItem();
                listItem.Text = "SUCC";
                listItem.SubItems.Add("NET_DVR_CLEAR_ACS_PARAM SUCC");
                this.AddList(listViewMessage, listItem, true);
            }
            Marshal.FreeHGlobal(ptrAcsParam);
        }

        private void CardManageCfg_FormClosed(object sender, FormClosedEventArgs e)
        {
            if (-1 != m_lGetCardCfgHandle)
            {
                for (int i = 0; i <= m_lGetCardCfgHandle; i++ )
                {
                    CHCNetSDK.NET_DVR_StopRemoteConfig(i);
                }
                
            }
            if (-1 != m_lSetCardCfgHandle)
            {
                for (int i = 0; i <= m_lSetCardCfgHandle; i++ )
                {
                    CHCNetSDK.NET_DVR_StopRemoteConfig(i);
                }
            }
        }


        protected override void DefWndProc(ref Message m)
        {
            switch (m.Msg)
            {
                case 1001:
                    int iErrorMsg = m.WParam.ToInt32();
                    if (-1 != m_lSetCardCfgHandle)
                    {
                        if (CHCNetSDK.NET_DVR_StopRemoteConfig(m_lSetCardCfgHandle))
                        {
                            m_lSetCardCfgHandle = -1;
                        }
                    }

                    if (-1 != m_lGetCardCfgHandle)
                    {
                        if (CHCNetSDK.NET_DVR_StopRemoteConfig(m_lGetCardCfgHandle))
                        {
                            m_lGetCardCfgHandle = -1;
                        }
                    }
                    break;
                case 1002:
                    SendNextCard();
                    break;
                case 1003:
                    IntPtr pCardInfo = (IntPtr)m.WParam.ToInt32();
                    CHCNetSDK.NET_DVR_CARD_CFG_V50 struCardCfg = new CHCNetSDK.NET_DVR_CARD_CFG_V50();
                    struCardCfg = (CHCNetSDK.NET_DVR_CARD_CFG_V50)Marshal.PtrToStructure(pCardInfo, typeof(CHCNetSDK.NET_DVR_CARD_CFG_V50));
                    string strCardNo = System.Text.Encoding.UTF8.GetString(struCardCfg.byCardNo);
                    AddToCardList(struCardCfg, strCardNo);
                    break;
                default:
                    base.DefWndProc(ref m);
                    break;
            }
        }

        private void CardManagement_Load(object sender, EventArgs e)
        {
            MultiLanguage.LoadLanguage(this);
        }
    }
}
