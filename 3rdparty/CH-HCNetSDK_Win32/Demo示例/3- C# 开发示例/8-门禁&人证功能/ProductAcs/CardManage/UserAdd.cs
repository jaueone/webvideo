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

using AcsDemo.Public;


namespace AcsDemo.ProductAcs.CardManage
{
    public delegate bool DeleteClick(object sender, EventArgs e);
    public partial class UserAdd : Form
    {
        private CHCNetSDK.NET_DVR_CARD_CFG_V50 m_struCardInfo = new CHCNetSDK.NET_DVR_CARD_CFG_V50();
        private CHCNetSDK.NET_DVR_FINGER_PRINT_CFG m_struFingerPrintOne = new CHCNetSDK.NET_DVR_FINGER_PRINT_CFG();
        //private CHCNetSDK.NET_DVR_FINGER_PRINT_INFO_CTRL_BYCARD m_struDelFingerPrint = new CHCNetSDK.NET_DVR_FINGER_PRINT_INFO_CTRL_BYCARD();
        private UserInfo m_userInfo = new UserInfo();

        public DeviceTree m_deviceTree;
        public int m_lUserID = -1;
        public int m_lDeviceIndex = -1;
        public int m_doorNum = 1;

        private DeviceTree g_deviceTree = DeviceTree.Instance();
        private DeviceLogList g_formList = DeviceLogList.Instance();

        private bool m_bIsEmpty = false;
        private const int iMaxCardNum = 1;
        private const int iMaxFingprintNum = 1;
        private int m_curFingprintNum = iMaxFingprintNum;

        private int m_lGetCardCfgHandle = -1;
        private int m_lSetCardCfgHandle = -1;
        private int m_lSetFingerPrintCfgHandle = -1;
        private int m_lGetFingerPrintCfgHandle = -1;

        private CHCNetSDK.RemoteConfigCallback g_fSetFingerPrintCallback = null;
        private CHCNetSDK.RemoteConfigCallback g_fGetFingerPrintCallback = null;
        private CHCNetSDK.RemoteConfigCallback g_fGetGatewayCardCallback = null;
        private CHCNetSDK.RemoteConfigCallback g_fSetGatewayCardCallback = null;
        private delegate void AddListCallBack(ListView cListView, ListViewItem cListViewItem);
        private delegate string GetListTextCallBack(ListView cListView, int iItemTag, int iSubitemTag);

        public delegate bool ConfigCardCallBack(bool bGet);
        public delegate void DeleteInfoCallBack();
        public ConfigCardCallBack g_fConfigCardCallBack = null;
        public DeleteInfoCallBack g_fDeleteInfoCallBack = null;

        private bool bSetDataByInvoker = false;
        public UserAdd()
        {
            InitializeComponent();
            m_struCardInfo.Init();
            m_userInfo.cards = new CardInfo[iMaxCardNum];
            for (int i = 0; i < iMaxCardNum; i++)
            {
                m_userInfo.cards[0] = new CardInfo();
            }
            m_struFingerPrintOne.Init();
            g_fDeleteInfoCallBack += new DeleteInfoCallBack(deleteUserInfo);

        }

        private void buttonCardInfoAdd_Click(object sender, EventArgs e)
        {
            CardAdd cardAdd = new CardAdd();
            if (cardAdd.ShowDialog() == DialogResult.OK)
            {
                this.listViewUserAddCardInfo.BeginUpdate();

                ListViewItem lvi = new ListViewItem();
                lvi.Text = cardAdd.cardInformations.cardSerialNumber;
                lvi.SubItems.Add(cardAdd.cardInformations.cardNumber);
                lvi.SubItems.Add(cardAdd.cardInformations.cardType);
                lvi.SubItems.Add(cardAdd.cardInformations.cardValidityPeriod);

                this.listViewUserAddCardInfo.Items.Add(lvi);
                this.listViewUserAddCardInfo.EndUpdate();
                m_struCardInfo = cardAdd.GetCurCardInfo();

                for (int i = 0; i < iMaxCardNum; i++)
                {
                    m_userInfo.cards[i].cardNumber = cardAdd.cardInformations.cardNumber;
                    m_userInfo.cards[i].cardPassword = cardAdd.cardInformations.cardPassword;
                    m_userInfo.cards[i].cardSerialNumber = cardAdd.cardInformations.cardSerialNumber;
                    m_userInfo.cards[i].cardType = cardAdd.cardInformations.cardType;
                    m_userInfo.cards[i].cardValidityPeriod = cardAdd.cardInformations.cardValidityPeriod;
                }

                m_bIsEmpty = false;
            }
        }

        private void textBoxMemberNo_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar != '\b')//backspace 
            {
                if ((e.KeyChar < '0') || (e.KeyChar > '9'))//0-9 is permitted  
                {
                    e.Handled = true;
                }
            } 
        }

        private void listViewUserAddCardInfo_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (listViewUserAddCardInfo.SelectedItems.Count == 0) return;
            switch (listViewUserAddCardInfo.SelectedItems[0].Text.ToString())
            {
                case "1":
                    //MessageBox.Show("hah");
                    break;
            }
        }

        private void listViewUserAddCardInfo_Leave(object sender, EventArgs e)
        {
            if (listViewUserAddCardInfo.SelectedItems != null)
            {
                foreach (ListViewItem lvi in listViewUserAddCardInfo.SelectedItems)  
                {
                    //let selected back view appear gray 
                    lvi.BackColor = Color.LightGray;
                    //the foreground color to white 
                    lvi.ForeColor = Color.White;
                }
            }
        }

        //press down the mouse and up at the same item position 
        private void listViewUserAddCardInfo_Click(object sender, EventArgs e)
        {
            buttonCardInfoModify.Enabled = true;
            buttonCardInfoDelete.Enabled = true;
        }

        private void listViewUserAddCardInfo_MouseDown(object sender, MouseEventArgs e)
        {
            if (listViewUserAddCardInfo.SelectedItems.Count > 0)
            {
                buttonCardInfoModify.Enabled = false;
                buttonCardInfoDelete.Enabled = false;
                foreach (ListViewItem lvi in listViewUserAddCardInfo.SelectedItems)
                {
                    //let selected back view appear gray 
                    lvi.BackColor = Color.Empty;
                    //the foreground color to white 
                    lvi.ForeColor = Color.Black;
                }
            }
        }

        public CHCNetSDK.NET_DVR_CARD_CFG_V50 GetCurCardInfo()
        {
            return m_struCardInfo;
        }

        public CHCNetSDK.NET_DVR_FINGER_PRINT_CFG GetFingerprintInfo()
        {
            return m_struFingerPrintOne;
        }

        public UserInfo GetUserInfo()
        {
            return m_userInfo;
        }

        public void SetCurCardInfo(ref CHCNetSDK.NET_DVR_CARD_CFG_V50 cardInfo, ref CHCNetSDK.NET_DVR_FINGER_PRINT_CFG fingerprintInfo, ref UserInfo userInfo)
        {
            m_struCardInfo = cardInfo;
            m_struFingerPrintOne = fingerprintInfo;
            m_userInfo = userInfo;
            bSetDataByInvoker = true;
        }

        private void buttonConfirm_Click(object sender, EventArgs e)
        {
            if (UpdateDataFromUI())
            {
                g_fConfigCardCallBack = new ConfigCardCallBack(ConfigUserAddAll);
                this.DialogResult = DialogResult.OK;
            }
        }

        private void SetDataToUI()
        {
            textBoxMemberNo.Text = m_userInfo.memberNo;
            textBoxMemberName.Text = m_userInfo.memberName;

            this.listViewUserAddCardInfo.BeginUpdate();

            for (int i = 0; i < iMaxCardNum; i++)
            {
                ListViewItem lvi = new ListViewItem();
                lvi.Text = m_userInfo.cards[i].cardSerialNumber;
                lvi.SubItems.Add(System.Text.Encoding.UTF8.GetString(m_struCardInfo.byCardNo));
                lvi.SubItems.Add(m_userInfo.cards[i].cardType);

                string tmpStartTime = "";
                string tmpEndTime = "";
                if (CheckDate(m_struCardInfo.struValid.struBeginTime))
                {
                    tmpStartTime = string.Format("{0,0:D2}/{1,0:D2}/{2,4} {3,0:D2}:{4,0:D2}:{5,0:D2}", m_struCardInfo.struValid.struBeginTime.byMonth,
                        m_struCardInfo.struValid.struBeginTime.byDay, m_struCardInfo.struValid.struBeginTime.wYear,
                        m_struCardInfo.struValid.struBeginTime.byHour, m_struCardInfo.struValid.struBeginTime.byMinute,
                        m_struCardInfo.struValid.struBeginTime.bySecond);
                }
                if (CheckDate(m_struCardInfo.struValid.struEndTime))
                {
                    tmpEndTime = string.Format("{0,0:D2}/{1,0:D2}/{2,4} {3,0:D2}:{4,0:D2}:{5,0:D2}", m_struCardInfo.struValid.struEndTime.byMonth,
                        m_struCardInfo.struValid.struEndTime.byDay, m_struCardInfo.struValid.struEndTime.wYear,
                        m_struCardInfo.struValid.struEndTime.byHour, m_struCardInfo.struValid.struEndTime.byMinute,
                        m_struCardInfo.struValid.struEndTime.bySecond);
                }
                string tmpTime = string.Format("{0}-{1}", tmpStartTime, tmpEndTime);
                lvi.SubItems.Add(tmpTime);
                this.listViewUserAddCardInfo.Items.Add(lvi);
            }
            this.listViewUserAddCardInfo.EndUpdate();

            bool bCheck = false;
            for (int i = 0; i < m_doorNum; i++)
            {
                if (0 != (m_struCardInfo.byDoorRight[i]))
                {
                    bCheck = true;
                }
                else
                {
                    bCheck = false;
                }
                checkedListBoxDoorAuthority.SetItemChecked(i, bCheck);
            }

            UpdatePlanList(m_struCardInfo.wCardRightPlan);

            textBoxFingerPrintNumber.Text = m_struFingerPrintOne.byFingerPrintID.ToString();
            textBoxFingerprintDataPath.Text = m_userInfo.cards[0].fingerPrintDescriptions;
            textBoxDataLength.Text = m_struFingerPrintOne.dwFingerPrintLen.ToString();
            comboBoxFingerprintType.SelectedIndex = (int)m_struFingerPrintOne.byFingerType;
            checkBoxNonBlock.Checked = false;

            for (int i = 0; i < 2 * m_doorNum; i++)
            {
                if (0 != (m_struFingerPrintOne.byEnableCardReader[i]))
                {
                    bCheck = true;
                }
                else
                {
                    bCheck = false;
                }
                checkedListBoxReaderValid.SetItemChecked(i, bCheck);
            }
        }

        private bool UpdateFingerPrintCfg(ref CHCNetSDK.NET_DVR_FINGER_PRINT_CFG struFingerPrintCfg)
        {
            struFingerPrintCfg.dwSize = (uint)Marshal.SizeOf(struFingerPrintCfg);
            byte[] byTempCard = System.Text.Encoding.UTF8.GetBytes(listViewUserAddCardInfo.SelectedItems[0].SubItems[1].Text);
            for (int i = 0; i < byTempCard.Length; i++)
            {
                if (i > struFingerPrintCfg.byCardNo.Length)
                {
                    return false;
                }
                struFingerPrintCfg.byCardNo[i] = byTempCard[i];
            }
            uint.TryParse(textBoxDataLength.Text, out struFingerPrintCfg.dwFingerPrintLen);

            for (int i = 0; i < 2 * m_doorNum; i++)
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
            struFingerPrintCfg.byEnableCardReader = m_struFingerPrintOne.byEnableCardReader;
            byte.TryParse(textBoxFingerPrintNumber.Text, out struFingerPrintCfg.byFingerPrintID);
            struFingerPrintCfg.byFingerType = (byte)comboBoxFingerprintType.SelectedIndex;
            if (!File.Exists(textBoxFingerprintDataPath.Text))
            {
                MessageBox.Show("The fingerprint file does not exist !");
                return false;
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

        private bool UpdateDataFromUI()
        {
            if (listViewUserAddCardInfo.SelectedItems.Count == 0)
            {
                MessageBox.Show("Please select a card");
                return false;
            }
            UpdateUserInfo();
            //UpdateFingerPrintCfg(ref m_struFingerPrintOne);
            UpdateMoudleCfg();

            return true;
        }

        private void UpdateUserInfo()
        {
            m_userInfo.memberNo = textBoxMemberNo.Text;
            m_userInfo.memberName = textBoxMemberName.Text;
            m_userInfo.cardNum = listViewUserAddCardInfo.Items.Count.ToString();
            m_userInfo.fingerPrintNum = m_curFingprintNum.ToString();
            m_userInfo.cards[0].fingerPrintDescriptions = textBoxFingerprintDataPath.Text;
            m_userInfo.cards[0].fingerSerialNumber = "1";
        }

        private void UpdateMoudleCfg()
        {
            int i;
            int j;
            m_struCardInfo.dwModifyParamType |= 0x8;
            System.Array.Clear(m_struCardInfo.byDoorRight, 0, m_struCardInfo.byDoorRight.Length);
            for (i = 0; i < checkedListBoxDoorAuthority.Items.Count; i++)
            {
                if (checkedListBoxDoorAuthority.GetItemChecked(i))
                {
                    m_struCardInfo.byDoorRight[i] = 1;
                }
            }
            m_struCardInfo.dwModifyParamType |= 0x100;
            for (i = 0; i < m_doorNum; i++)
            {
                for (j = 0; j < CHCNetSDK.MAX_CARD_RIGHT_PLAN_NUM; j++)
                {
                    ushort.TryParse(listViewCardAuthorityScheme.Items[i * CHCNetSDK.MAX_CARD_RIGHT_PLAN_NUM + j].SubItems[2].Text,
                        out m_struCardInfo.wCardRightPlan[i * CHCNetSDK.MAX_CARD_RIGHT_PLAN_NUM + j]);
                }
            }
        }

        private bool ConfigUserAddAll(bool bGet)
        {
            if (bGet)
            {
                if (GetCardInfoFromDevice())
                {
                    if (!GetFingerPrint())
                    {
                        m_curFingprintNum = 0;
                        UpdateUserInfo();
                    }
                    SetDataToUI();
                    return true;
                }
                else
                {
                    return false;
                }
            }
            else
            {
                if (SendCardInfoToDevice())
                {
                    if (!SetFingerPrint())
                    {
                        m_curFingprintNum = 0;
                        UpdateUserInfo();
                    }
                    return true;
                }
                return false;

            }
        }

        private bool SetFingerPrint()
        {
            if (UpdateFingerPrintCfg(ref m_struFingerPrintOne))
            {
                if (m_lSetFingerPrintCfgHandle != -1)
                {
                    CHCNetSDK.NET_DVR_StopRemoteConfig(m_lSetFingerPrintCfgHandle);
                }
                CHCNetSDK.NET_DVR_FINGER_PRINT_INFO_COND struCond = new CHCNetSDK.NET_DVR_FINGER_PRINT_INFO_COND();
                struCond.byCardNo = new byte[32];
                struCond.byEnableCardReader = new byte[512];
                struCond.dwSize = (uint)Marshal.SizeOf(struCond);
                uint.TryParse(m_userInfo.fingerPrintNum, out struCond.dwFingerPrintNum);
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
                Marshal.StructureToPtr(struCond, ptrStruCond, false);
                g_fSetFingerPrintCallback = new CHCNetSDK.RemoteConfigCallback(ProcessSetFingerPrintCfgCallbackData);
                m_lSetFingerPrintCfgHandle = CHCNetSDK.NET_DVR_StartRemoteConfig(m_lUserID,
                    CHCNetSDK.NET_DVR_SET_FINGERPRINT_CFG, ptrStruCond, dwSize, g_fSetFingerPrintCallback, this.Handle);
                if (-1 == m_lSetFingerPrintCfgHandle)
                {
                    g_formList.AddLog(m_lDeviceIndex, AcsDemoPublic.OPERATION_FAIL_T, "NET_DVR_SET_FINGERPRINT_CFG");
                    Marshal.FreeHGlobal(ptrStruCond);
                    return false;
                }
                else
                {
                    g_formList.AddLog(m_lDeviceIndex, AcsDemoPublic.OPERATION_SUCC_T, "NET_DVR_SET_FINGERPRINT_CFG");
                }

                if (!SendFirstCard())
                {
                    CHCNetSDK.NET_DVR_StopRemoteConfig(m_lSetFingerPrintCfgHandle);
                    m_lSetFingerPrintCfgHandle = -1;
                    Marshal.FreeHGlobal(ptrStruCond);
                    return false;
                }
                Marshal.FreeHGlobal(ptrStruCond);
                return true;
            }
            else 
            {
                return false;
            }
        }

        private bool GetFingerPrint()
        {
            if (m_lGetFingerPrintCfgHandle != -1)
            {
                CHCNetSDK.NET_DVR_StopRemoteConfig((int)m_lGetFingerPrintCfgHandle);
            }

            CHCNetSDK.NET_DVR_FINGER_PRINT_INFO_COND struCond = new CHCNetSDK.NET_DVR_FINGER_PRINT_INFO_COND();
            struCond.byCardNo = new byte[32];
            struCond.dwSize = (uint)Marshal.SizeOf(struCond);
            uint.TryParse(m_userInfo.fingerPrintNum, out struCond.dwFingerPrintNum);
            byte.TryParse(textBoxFingerPrintNumber.Text, out struCond.byFingerPrintID);
            byte[] byTemp = System.Text.Encoding.UTF8.GetBytes(m_userInfo.cards[0].cardNumber);
            for (int i = 0; i < byTemp.Length; i++)
            {
                struCond.byCardNo[i] = byTemp[i];
            }
            struCond.byEnableCardReader = m_struFingerPrintOne.byEnableCardReader;

            int dwSize = Marshal.SizeOf(struCond);
            IntPtr ptrStruCond = Marshal.AllocHGlobal(dwSize);
            Marshal.StructureToPtr(struCond, ptrStruCond, false);
            g_fGetFingerPrintCallback = new CHCNetSDK.RemoteConfigCallback(ProcessGetFingerPrintCfgCallbackData);
            m_lGetFingerPrintCfgHandle = CHCNetSDK.NET_DVR_StartRemoteConfig(m_lUserID, CHCNetSDK.NET_DVR_GET_FINGERPRINT_CFG, ptrStruCond, dwSize, g_fGetFingerPrintCallback, this.Handle);
            if (-1 == m_lGetFingerPrintCfgHandle)
            {
                g_formList.AddLog(m_lDeviceIndex, AcsDemoPublic.OPERATION_FAIL_T, "NET_DVR_GET_FINGERPRINT_CFG");
                Marshal.FreeHGlobal(ptrStruCond);
                return false;
            }
            else
            {
                g_formList.AddLog(m_lDeviceIndex, AcsDemoPublic.OPERATION_SUCC_T, "NET_DVR_GET_FINGERPRINT_CFG");
                Marshal.FreeHGlobal(ptrStruCond);
                return true;
            }
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
                    g_formList.AddLog(m_lDeviceIndex, AcsDemoPublic.OPERATION_SUCC_T, "SetFingerPrint PROCESSING");
                }
                else if (dwStatus == (uint)CHCNetSDK.NET_SDK_CALLBACK_STATUS_NORMAL.NET_SDK_CALLBACK_STATUS_FAILED)
                {
                    g_formList.AddLog(m_lDeviceIndex, AcsDemoPublic.OPERATION_FAIL_T, "SetFingerPrint Err:NET_SDK_CALLBACK_STATUS_FAILED");
                }
                else if (dwStatus == (uint)CHCNetSDK.NET_SDK_CALLBACK_STATUS_NORMAL.NET_SDK_CALLBACK_STATUS_SUCCESS)
                {
                    g_formList.AddLog(m_lDeviceIndex, AcsDemoPublic.OPERATION_SUCC_T, "NET_DVR_SET_FINGERPRINT_CFG Set finish");
                }
                else if (dwStatus == (uint)CHCNetSDK.NET_SDK_CALLBACK_STATUS_NORMAL.NET_SDK_CALLBACK_STATUS_EXCEPTION)
                {
                    g_formList.AddLog(m_lDeviceIndex, AcsDemoPublic.OPERATION_FAIL_T, "NET_DVR_SET_FINGERPRINT_CFG Set Exception");
                }
                else
                {
                    g_formList.AddLog(m_lDeviceIndex, AcsDemoPublic.OPERATION_SUCC_T, "NET_DVR_SET_FINGERPRINT_CFG Set finish");
                }
            }
            else if (dwType == (uint)CHCNetSDK.NET_SDK_CALLBACK_TYPE.NET_SDK_CALLBACK_TYPE_DATA)
            {
                CHCNetSDK.NET_DVR_FINGER_PRINT_STATUS struCfg = new CHCNetSDK.NET_DVR_FINGER_PRINT_STATUS();
                struCfg = (CHCNetSDK.NET_DVR_FINGER_PRINT_STATUS)Marshal.PtrToStructure(lpBuffer, typeof(CHCNetSDK.NET_DVR_FINGER_PRINT_STATUS));
                bool bSendOK = false;
                for (int i = 0; i < struCfg.byCardReaderRecvStatus.Length; i++)
                {
                    if (1 == struCfg.byCardReaderRecvStatus[i])
                    {
                        bSendOK = true;
                        string strTemp = null;
                        strTemp = string.Format("SetFingerPrint PROCESSING {0},CardReader {1}", System.Text.Encoding.UTF8.GetString(m_struFingerPrintOne.byCardNo).TrimEnd('\0'), i + 1);
                        g_formList.AddLog(m_lDeviceIndex, AcsDemoPublic.OPERATION_SUCC_T, strTemp);
                    }
                }
                if (!bSendOK)
                {
                    string strTemp = null;
                    strTemp = string.Format("SetFingerPrint Failed,CardNo:{0}", System.Text.Encoding.UTF8.GetString(m_struFingerPrintOne.byCardNo).TrimEnd('\0'));
                    g_formList.AddLog(m_lDeviceIndex, AcsDemoPublic.OPERATION_FAIL_T, strTemp);
                }
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
                CHCNetSDK.NET_DVR_FINGER_PRINT_CFG strFingerPrintCfg = new CHCNetSDK.NET_DVR_FINGER_PRINT_CFG();
                strFingerPrintCfg = (CHCNetSDK.NET_DVR_FINGER_PRINT_CFG)Marshal.PtrToStructure(lpBuffer, typeof(CHCNetSDK.NET_DVR_FINGER_PRINT_CFG));
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
                    g_formList.AddLog(m_lDeviceIndex, AcsDemoPublic.OPERATION_FAIL_T, "NET_DVR_GET_FINGERPRINT_CFG");
                }
                else 
                {
                    g_formList.AddLog(m_lDeviceIndex, AcsDemoPublic.OPERATION_SUCC_T, "NET_DVR_GET_FINGERPRINT_CFG");
                }
                m_struFingerPrintOne = strFingerPrintCfg;
                SetDataToUI();
                if (0 == strFingerPrintCfg.dwSize)
                {
                    return;
                }
            }
            else if (dwType == (uint)CHCNetSDK.NET_SDK_CALLBACK_TYPE.NET_SDK_CALLBACK_TYPE_STATUS)
            {
                uint dwStatus = 0;
                dwStatus = (uint)Marshal.ReadInt32(lpBuffer);
                if (dwStatus == (uint)CHCNetSDK.NET_SDK_CALLBACK_STATUS_NORMAL.NET_SDK_CALLBACK_STATUS_SUCCESS)
                {
                    g_formList.AddLog(m_lDeviceIndex, AcsDemoPublic.OPERATION_SUCC_T, "NET_DVR_GET_FINGERPRINT_CFG Get finish");
                }
                else if (dwStatus == (uint)CHCNetSDK.NET_SDK_CALLBACK_STATUS_NORMAL.NET_SDK_CALLBACK_STATUS_FAILED)
                {
                    g_formList.AddLog(m_lDeviceIndex, AcsDemoPublic.OPERATION_FAIL_T, "NET_DVR_GET_FINGERPRINT_CFG Get Failed");
                }
            }
        }

        private bool SendFirstCard()
        {
            if (-1 == m_lSetFingerPrintCfgHandle)
            {
                return false;
            }

            int fingerPrintNumTmp;
            int.TryParse(m_userInfo.fingerPrintNum, out fingerPrintNumTmp);

            if (fingerPrintNumTmp < 1)
            {
                return false;
            }

            uint dwSize = (uint)Marshal.SizeOf(m_struFingerPrintOne);
            IntPtr ptrNowSendCard = Marshal.AllocHGlobal((int)dwSize);
            Marshal.StructureToPtr(m_struFingerPrintOne, ptrNowSendCard, false);
            if (!CHCNetSDK.NET_DVR_SendRemoteConfig(m_lSetFingerPrintCfgHandle, 3, ptrNowSendCard, dwSize))
            {
                string strTemp = string.Format("Send Fail,CardNO: {0}", System.Text.Encoding.UTF8.GetString(m_struFingerPrintOne.byCardNo));
                g_formList.AddLog(m_lDeviceIndex, AcsDemoPublic.OPERATION_FAIL_T, strTemp);
                Marshal.FreeHGlobal(ptrNowSendCard);
                return false;
            }
            Marshal.FreeHGlobal(ptrNowSendCard);
            return true;
        }

        private bool GetCardInfoFromDevice()
        {
            if (-1 != m_lGetCardCfgHandle)
            {
                if (CHCNetSDK.NET_DVR_StopRemoteConfig(m_lGetCardCfgHandle))
                {
                    m_lGetCardCfgHandle = -1;
                }
            }

            CHCNetSDK.NET_DVR_CARD_CFG_SEND_DATA struSendData = new CHCNetSDK.NET_DVR_CARD_CFG_SEND_DATA();
            struSendData.dwSize = (uint)Marshal.SizeOf(struSendData);
            string strTemp = System.Text.Encoding.UTF8.GetString(m_struCardInfo.byCardNo);
            struSendData.byCardNo = System.Text.Encoding.UTF8.GetBytes(strTemp);

            CHCNetSDK.NET_DVR_CARD_CFG_COND struCond = new CHCNetSDK.NET_DVR_CARD_CFG_COND();
            struCond.dwSize = (uint)Marshal.SizeOf(struCond);
            struCond.dwCardNum = 1;
            struCond.byCheckCardNo = 1;

            int dwSize = Marshal.SizeOf(struCond);
            IntPtr ptrStruCond = Marshal.AllocHGlobal(dwSize);
            Marshal.StructureToPtr(struCond, ptrStruCond, false);
            g_fGetGatewayCardCallback = new CHCNetSDK.RemoteConfigCallback(ProcessGetGatewayCardCallback);
            m_lGetCardCfgHandle = CHCNetSDK.NET_DVR_StartRemoteConfig(m_lUserID, CHCNetSDK.NET_DVR_GET_CARD_CFG_V50, ptrStruCond, dwSize, g_fGetGatewayCardCallback, this.Handle);
            if (-1 == m_lGetCardCfgHandle)
            {
                g_formList.AddLog(m_lDeviceIndex, AcsDemoPublic.OPERATION_FAIL_T, "NET_DVR_GET_CARD_CFG_V50");
                Marshal.FreeHGlobal(ptrStruCond);
                return false;
            }
            else
            {
                g_formList.AddLog(m_lDeviceIndex, AcsDemoPublic.OPERATION_SUCC_T, "NET_DVR_GET_CARD_CFG_V50");
                uint dwSizeData = (uint)Marshal.SizeOf(struSendData);
                IntPtr ptrStruSendData = Marshal.AllocHGlobal((int)dwSizeData);
                Marshal.StructureToPtr(struSendData, ptrStruSendData, false);
                if (!CHCNetSDK.NET_DVR_SendRemoteConfig(m_lGetCardCfgHandle, 3, ptrStruSendData, dwSizeData))
                {
                    g_formList.AddLog(m_lDeviceIndex, AcsDemoPublic.OPERATION_FAIL_T, "NET_DVR_SendRemoteConfig ENUM_ACS_SEND_DATA FAIL");
                    return false;
                }
                else
                {
                    g_formList.AddLog(m_lDeviceIndex, AcsDemoPublic.OPERATION_SUCC_T, "NET_DVR_SendRemoteConfig ENUM_ACS_SEND_DATA SUCC");
                }
                Marshal.FreeHGlobal(ptrStruSendData);
            }
            Marshal.FreeHGlobal(ptrStruCond);

            return true;
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
                m_struCardInfo = struCardCfg;
                SetDataToUI();
            }
            else if (dwType == (uint)CHCNetSDK.NET_SDK_CALLBACK_TYPE.NET_SDK_CALLBACK_TYPE_STATUS)
            {
                uint dwStatus = (uint)Marshal.ReadInt32(lpBuffer);
                if (dwStatus == (uint)CHCNetSDK.NET_SDK_CALLBACK_STATUS_NORMAL.NET_SDK_CALLBACK_STATUS_SUCCESS)
                {
                    g_formList.AddLog(m_lDeviceIndex, AcsDemoPublic.OPERATION_SUCC_T, "NET_DVR_GET_CARD_CFG_V50 Get finish");
                }
                else if (dwStatus == (uint)CHCNetSDK.NET_SDK_CALLBACK_STATUS_NORMAL.NET_SDK_CALLBACK_STATUS_FAILED)
                {
                    g_formList.AddLog(m_lDeviceIndex, AcsDemoPublic.OPERATION_FAIL_T, "NET_DVR_GET_CARD_CFG_V50 Get FAILED");
                }
            }
        }

        private void ProcessSetGatewayCardCallback(uint dwType, IntPtr lpBuffer, uint dwBufLen, IntPtr pUserData)
        {
            if (pUserData == null)
            {
                return;
            }

            if (dwType != (uint)CHCNetSDK.NET_SDK_CALLBACK_TYPE.NET_SDK_CALLBACK_TYPE_STATUS)
            {
                return;
            }
            uint dwStatus = (uint)Marshal.ReadInt32(lpBuffer);

            if (dwStatus == (uint)CHCNetSDK.NET_SDK_CALLBACK_STATUS_NORMAL.NET_SDK_CALLBACK_STATUS_PROCESSING)
            {
                string strTemp = null;
                strTemp = string.Format("Send SUCC,CardNO:{0}", System.Text.Encoding.UTF8.GetString(m_struCardInfo.byCardNo).TrimEnd('\0'));
                g_formList.AddLog(m_lDeviceIndex, AcsDemoPublic.OPERATION_SUCC_T, strTemp);
            }
            else if (dwStatus == (uint)CHCNetSDK.NET_SDK_CALLBACK_STATUS_NORMAL.NET_SDK_CALLBACK_STATUS_FAILED)
            {
                g_formList.AddLog(m_lDeviceIndex, AcsDemoPublic.OPERATION_FAIL_T, "NET_DVR_SET_CARD_CFG_V50 Set Failed");
            }
            else if (dwStatus == (uint)CHCNetSDK.NET_SDK_CALLBACK_STATUS_NORMAL.NET_SDK_CALLBACK_STATUS_SUCCESS)
            {
                g_formList.AddLog(m_lDeviceIndex, AcsDemoPublic.OPERATION_SUCC_T, "NET_DVR_SET_CARD_CFG_V50 Set finish");
            }
            else if (dwStatus == (uint)CHCNetSDK.NET_SDK_CALLBACK_STATUS_NORMAL.NET_SDK_CALLBACK_STATUS_EXCEPTION)
            {
                g_formList.AddLog(m_lDeviceIndex, AcsDemoPublic.OPERATION_FAIL_T, "NET_DVR_SET_CARD_CFG_V50 Set Exception");
            }
        }

        private bool SendCardInfoToDevice()
        {
            if (-1 != m_lSetCardCfgHandle)
            {
                if (CHCNetSDK.NET_DVR_StopRemoteConfig(m_lSetCardCfgHandle))
                {
                    m_lSetCardCfgHandle = -1;
                }
            }
            CHCNetSDK.NET_DVR_CARD_CFG_COND struCond = new CHCNetSDK.NET_DVR_CARD_CFG_COND();
            struCond.dwSize = (uint)Marshal.SizeOf(struCond);
            struCond.dwCardNum = 1;

            int dwSize = Marshal.SizeOf(struCond);
            IntPtr ptrStruCond = Marshal.AllocHGlobal(dwSize);
            Marshal.StructureToPtr(struCond, ptrStruCond, false);
            g_fSetGatewayCardCallback = new CHCNetSDK.RemoteConfigCallback(ProcessSetGatewayCardCallback);
            m_lSetCardCfgHandle = CHCNetSDK.NET_DVR_StartRemoteConfig(m_lUserID, CHCNetSDK.NET_DVR_SET_CARD_CFG_V50, ptrStruCond, dwSize, g_fSetGatewayCardCallback, this.Handle);
            if (-1 == m_lSetCardCfgHandle)
            {
                g_formList.AddLog(m_lDeviceIndex, AcsDemoPublic.OPERATION_FAIL_T, "NET_DVR_SET_CARD_CFG_V50");
                Marshal.FreeHGlobal(ptrStruCond);
                return false;
            }
            else
            {
                g_formList.AddLog(m_lDeviceIndex, AcsDemoPublic.OPERATION_SUCC_T, "NET_DVR_SET_CARD_CFG_V50");
            }

            Marshal.FreeHGlobal(ptrStruCond);

            return SendCardData(m_struCardInfo);
        }

        private bool SendCardData(CHCNetSDK.NET_DVR_CARD_CFG_V50 struCardCfg, uint dwDiffTime = 0)
        {
            if (-1 == m_lSetCardCfgHandle)
            {
                return false;
            }
            uint dwSize = (uint)Marshal.SizeOf(struCardCfg);
            struCardCfg.dwSize = dwSize;
            IntPtr ptrStruCard = Marshal.AllocHGlobal((int)dwSize);
            Marshal.StructureToPtr(struCardCfg, ptrStruCard, false);
            if (!CHCNetSDK.NET_DVR_SendRemoteConfig(m_lSetCardCfgHandle, (int)CHCNetSDK.LONG_CFG_SEND_DATA_TYPE_ENUM.ENUM_ACS_SEND_DATA, ptrStruCard, dwSize))
            {
                Marshal.FreeHGlobal(ptrStruCard);
                string strTemp = null;
                strTemp = string.Format("Send Failed,CardNO:{0}", System.Text.Encoding.UTF8.GetString(m_struCardInfo.byCardNo).TrimEnd('\0'));
                g_formList.AddLog(m_lDeviceIndex, AcsDemoPublic.OPERATION_FAIL_T, strTemp);
                return false;
            }
            Marshal.FreeHGlobal(ptrStruCard);
            return true;
        }

        private void buttonCardInfoModify_Click(object sender, EventArgs e)
        {
            if (listViewUserAddCardInfo.SelectedItems.Count == 0) return;
            
            CardModify cardModify = new CardModify();
            cardModify.SetCardCfg(ref m_struCardInfo);


            if (cardModify.ShowDialog() == DialogResult.OK)
            {
                this.listViewUserAddCardInfo.BeginUpdate();

                listViewUserAddCardInfo.SelectedItems[0].SubItems[1].Text = cardModify.cardInformations.cardNumber;
                listViewUserAddCardInfo.SelectedItems[0].SubItems[2].Text = cardModify.cardInformations.cardType;
                listViewUserAddCardInfo.SelectedItems[0].SubItems[3].Text = cardModify.cardInformations.cardValidityPeriod;

                this.listViewUserAddCardInfo.EndUpdate();

                m_struCardInfo = cardModify.GetCardCfg();
            }
        }


        private void deleteUserInfo()
        {
            deleteSingleCard();
            deleteFingerprintByCard();
        }

        private void deleteSingleCard()
        {
            //delete card 
            if (-1 != m_lSetCardCfgHandle)
            {
                if (CHCNetSDK.NET_DVR_StopRemoteConfig(m_lSetCardCfgHandle))
                {
                    m_lSetCardCfgHandle = -1;
                }
            }

            CHCNetSDK.NET_DVR_CARD_CFG_COND struCond = new CHCNetSDK.NET_DVR_CARD_CFG_COND();
            struCond.dwSize = (uint)Marshal.SizeOf(struCond);
            struCond.dwCardNum = 1;
            int dwSize = Marshal.SizeOf(struCond);
            IntPtr ptrStruCond = Marshal.AllocHGlobal(dwSize);
            Marshal.StructureToPtr(struCond, ptrStruCond, false);
            g_fSetGatewayCardCallback = new CHCNetSDK.RemoteConfigCallback(ProcessSetGatewayCardCallback);
            m_lSetCardCfgHandle = CHCNetSDK.NET_DVR_StartRemoteConfig(m_lUserID, CHCNetSDK.NET_DVR_SET_CARD_CFG_V50, ptrStruCond, dwSize, g_fSetGatewayCardCallback, this.Handle);
            if (-1 == m_lSetCardCfgHandle)
            {
                g_formList.AddLog(m_lDeviceIndex, AcsDemoPublic.OPERATION_FAIL_T, "NET_DVR_SET_CARD_CFG_V50");
                Marshal.FreeHGlobal(ptrStruCond);
                return;
            }
            else
            {
                g_formList.AddLog(m_lDeviceIndex, AcsDemoPublic.OPERATION_SUCC_T, "NET_DVR_SET_CARD_CFG_V50");
            }
            Marshal.FreeHGlobal(ptrStruCond);

            //delete card should set this field
            m_struCardInfo.byCardValid = 0;
            SendCardData(m_struCardInfo);
        }

        private void deleteFingerprintByCard()
        {
            if (int.Parse(m_userInfo.fingerPrintNum) > 0)
            {
                CHCNetSDK.NET_DVR_FINGER_PRINT_INFO_CTRL_BYCARD struDelCfg = new CHCNetSDK.NET_DVR_FINGER_PRINT_INFO_CTRL_BYCARD();
                struDelCfg.byMode = 0;
                struDelCfg.struByCard.byCardNo = new byte[32];

                for (int i = 0; i < m_struCardInfo.byCardNo.Length; i++)
                {
                    if (i >= struDelCfg.struByCard.byCardNo.Length)
                    {
                        break;
                    }
                    struDelCfg.struByCard.byCardNo[i] = m_struCardInfo.byCardNo[i];
                }
                struDelCfg.struByCard.byFingerPrintID = new byte[CHCNetSDK.MAX_FINGER_PRINT_NUM];
                struDelCfg.struByCard.byEnableCardReader = m_struFingerPrintOne.byEnableCardReader;
                struDelCfg.struByCard.byFingerPrintID[m_struFingerPrintOne.byFingerPrintID] = 1;

                struDelCfg.dwSize = (uint)Marshal.SizeOf(struDelCfg);
                int dwSize = Marshal.SizeOf(struDelCfg);
                IntPtr ptrDelCfg = Marshal.AllocHGlobal(dwSize);
                Marshal.StructureToPtr(struDelCfg, ptrDelCfg, false);
                if (!CHCNetSDK.NET_DVR_RemoteControl(m_lUserID, CHCNetSDK.NET_DVR_DEL_FINGERPRINT_CFG, ptrDelCfg, (uint)dwSize))
                {
                    g_formList.AddLog(m_lDeviceIndex, AcsDemoPublic.OPERATION_FAIL_T, "NET_DVR_DEL_FINGERPRINT_CFG");
                    Marshal.FreeHGlobal(ptrDelCfg);
                    return;
                }
                else
                {
                    g_formList.AddLog(m_lDeviceIndex, AcsDemoPublic.OPERATION_SUCC_T, "NET_DVR_DEL_FINGERPRINT_CFG");
                    Marshal.FreeHGlobal(ptrDelCfg);
                }
            }
        }

        private void buttonCardInfoDelete_Click(object sender, EventArgs e)
        {
            if (m_bIsEmpty) return;

            //deleteSingleCard();

            //delete fingerprint
            listViewUserAddCardInfo.Items.RemoveAt(listViewUserAddCardInfo.SelectedItems[0].Index);
            m_struCardInfo = new CHCNetSDK.NET_DVR_CARD_CFG_V50();
            m_struCardInfo.Init();
            m_bIsEmpty = true;

            return;
        }

        private bool IsCardInfoEmpty()
        {
            return m_bIsEmpty;
        }

        private void buttonModify_Click(object sender, EventArgs e)
        {
            foreach (ListViewItem lvi in listViewCardAuthorityScheme.SelectedItems)
            {
                lvi.SubItems[2].Text = textBoxPlanNumber.Text;
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

        private void InitUI()
        {
            //clear 
            checkedListBoxDoorAuthority.Items.Clear();
            checkedListBoxReaderValid.Items.Clear();

            //create
            checkedListBoxDoorAuthority.BeginUpdate();
            string doorName = null;
            for (int i = 0; i < m_doorNum; i++)
            {
                doorName = m_deviceTree.GetCurDoorName(i);
                if (doorName != null)
                {
                    checkedListBoxDoorAuthority.Items.Add(doorName);
                }
                else
                {
                    checkedListBoxDoorAuthority.Items.Add("Door " + (i + 1));
                }
            }
            checkedListBoxDoorAuthority.EndUpdate();

            //number of card reader is 2 times of door
            checkedListBoxReaderValid.BeginUpdate();
            for (int i = 0; i < 2 * m_doorNum; i++)
            {
                checkedListBoxReaderValid.Items.Add("CardReader " + (i + 1));
            }
            checkedListBoxReaderValid.EndUpdate();
            UpdatePlanList(m_struCardInfo.wCardRightPlan);
        }

        private void UpdatePlanList(ushort[] wCardPlan)
        {
            listViewCardAuthorityScheme.BeginUpdate();
            listViewCardAuthorityScheme.Items.Clear();
            ListViewItem lvItem;
            ListViewItem.ListViewSubItem lvSubItem;
            int i, j;
            String csTmp;
            for (i = 0; i < m_doorNum; i++)
            {
                for (j = 0; j < CHCNetSDK.MAX_CARD_RIGHT_PLAN_NUM; j++)
                {
                    lvItem = new ListViewItem();
                    lvItem.Text = (i + 1).ToString();
                    lvSubItem = new System.Windows.Forms.ListViewItem.ListViewSubItem();
                    csTmp = (j + 1).ToString();

                    lvSubItem.Text = csTmp;
                    lvItem.SubItems.Add(lvSubItem);

                    lvSubItem = new System.Windows.Forms.ListViewItem.ListViewSubItem();
                    csTmp = wCardPlan[i * CHCNetSDK.MAX_CARD_RIGHT_PLAN_NUM + j].ToString();

                    lvSubItem.Text = csTmp;
                    lvItem.SubItems.Add(lvSubItem);
                    listViewCardAuthorityScheme.Items.Add(lvItem);
                }
            }
            listViewCardAuthorityScheme.EndUpdate();
            return;
        }

        private bool CheckDate(CHCNetSDK.NET_DVR_TIME_EX struItem)
        {
            if (struItem.wYear < 1970 || struItem.byMonth > 12 || struItem.byDay > 31)
            {
                return false;
            }

            if (struItem.byHour > 23 || struItem.byMinute > 59 || struItem.bySecond > 59)
            {
                return false;
            }
            return true;
        }

        private void UserAdd_Load(object sender, EventArgs e)
        {
            if (m_doorNum < 0)
            {
                m_doorNum = 4; //default 4
            }
            InitUI();
            // have set data by invoker
            if (bSetDataByInvoker)
            {
                SetDataToUI();
            }
        }
    }
}
