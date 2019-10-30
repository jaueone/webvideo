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
using System.Globalization;
using System.Reflection;

using AcsDemo.Language;
using AcsDemo.Public;
using AcsDemo.ProductAcs.CardManage;
using AcsDemo.ProductAcs;

namespace AcsDemo
{
    public partial class AcsDemo : Form
    {
        const uint XML_ABILITY_OUT_LEN = 3 * 1024 * 1024;

        private DeviceLogList g_formList = DeviceLogList.Instance();
        private DeviceTree g_deviceTree = DeviceTree.Instance();

        private CHCNetSDK.NET_DVR_ACS_WORK_STATUS_V50 m_struAcsWorkStatus = new CHCNetSDK.NET_DVR_ACS_WORK_STATUS_V50();
        private CHCNetSDK.NET_DVR_CARD_READER_CFG_V50 m_struCardReaderCfg = new CHCNetSDK.NET_DVR_CARD_READER_CFG_V50();
        private bool m_bInitSDK = false;
        private bool m_bValidate = false;

        //built in current demo, one user with one card and one fingerprint;
        public const int iMaxCardNum = 100;
        public const int iMaxFingerPrint = 100;
        public CHCNetSDK.NET_DVR_CARD_CFG_V50[] m_struCardInfo = new CHCNetSDK.NET_DVR_CARD_CFG_V50[iMaxCardNum];
        public UserInfo[] m_userInfo = new UserInfo[iMaxCardNum];
        private CHCNetSDK.NET_DVR_FINGER_PRINT_CFG[] m_struFingerPrintInfo = new CHCNetSDK.NET_DVR_FINGER_PRINT_CFG[iMaxFingerPrint];
        private int m_curUserNum = 0;

        private CHCNetSDK.RemoteConfigCallback g_fGetLogsCallback = null;
        public int lRemoteHandle = -1;
        public int iTotalAcsEvent = 0;

        //public event DeleteClick DeleteBtnClick;

        protected void mouseClickDeviceTreeNode(object send, TreeNodeMouseClickEventArgs e)
        {
            int iDeviceIndex = g_deviceTree.GetCurDeviceIndex();
            tabControlAcs.SuspendLayout();
            if ((e.Node.Level) == 0 || (e.Node.Level == 1))
            {
                //HideAcsControlAllTabPages();
                tabPageDoorInfo.Parent = null;
                tabPageDoorStatusPlan.Parent = null;
                //tabPageRemoteControlDoor.Parent = null;
                tabPageCardReaderInfo.Parent = null;
                tabPageCardReaderPlan.Parent = null;
                tabPageDeviceStatus.Parent = tabControlAcs;
                tabPageUserManagement.Parent = tabControlAcs;
                tabPageRemoteControl.Parent = tabControlAcs;
                tabPageParamCfg.Parent = tabControlAcs;
                tabPageBasicParam.Parent = tabControlAcs;
                tabPageAcsXml.Parent = tabControlAcs;

                //device login and select one device
                if (iDeviceIndex >= 0 && g_deviceTree.g_struDeviceInfo[iDeviceIndex].lLoginID >= 0)
                {
                    GetAcsWorkStatus(g_deviceTree.g_struDeviceInfo[iDeviceIndex].lLoginID, ref m_struAcsWorkStatus);
                    UpdateWorkStatus();
                    g_deviceTree.UpdateTreeNode();
                }
            }
            else if (e.Node.Level == 2)
            {
                tabPageDoorInfo.Parent = tabControlAcs;
                tabPageDoorStatusPlan.Parent = tabControlAcs;
                //tabPageRemoteControlDoor.Parent = tabControlAcs;
                tabPageCardReaderInfo.Parent = null;
                tabPageCardReaderPlan.Parent = null;
                tabPageDeviceStatus.Parent = null;
                tabPageRemoteControl.Parent = null;
                tabPageUserManagement.Parent = null;
                tabPageParamCfg.Parent = null;
                tabPageBasicParam.Parent = null;
                tabPageAcsXml.Parent = null;

                //device login and select one door
                if (iDeviceIndex >= 0 && g_deviceTree.g_struDeviceInfo[iDeviceIndex].lLoginID >= 0)
                {
                    ShowDoorConfiguration();
                    g_deviceTree.UpdateTreeNode();
                }
            }
            else
            {
                tabPageDoorInfo.Parent = null;
                tabPageDoorStatusPlan.Parent = null;
                //tabPageRemoteControlDoor.Parent = null;
                tabPageCardReaderInfo.Parent = tabControlAcs;
                tabPageCardReaderPlan.Parent = tabControlAcs;
                tabPageDeviceStatus.Parent = null;
                tabPageRemoteControl.Parent = null;
                tabPageUserManagement.Parent = null;
                tabPageParamCfg.Parent = tabControlAcs;
                tabPageBasicParam.Parent = null;
                tabPageAcsXml.Parent = null;

                //device login and select one card reader
                if (iDeviceIndex >= 0 && g_deviceTree.g_struDeviceInfo[iDeviceIndex].lLoginID >= 0)
                {
                    ShowCardReaderConfiguration();
                    g_deviceTree.UpdateTreeNode();
                }
            }

            if (comboBoxLanguage.Text != null)
            {
                MultiLanguage.SetDefaultLanguage(comboBoxLanguage.Text);
                foreach (Form form in Application.OpenForms)
                {
                    MultiLanguage.LoadLanguage(form);
                }
            }

            m_bValidate = true;

            tabControlAcs.ResumeLayout();
        }

        public AcsDemo()
        {
            InitializeComponent();
            radioButtonLocalLog.Checked = true;
            this.m_panelDeviceLog.Controls.Add(g_formList);
            this.m_panelDeviceTree.Controls.Add(g_deviceTree);
            g_deviceTree.Dock = DockStyle.Fill;
            g_formList.Dock = DockStyle.Fill;

            m_bInitSDK = CHCNetSDK.NET_DVR_Init();
            if (m_bInitSDK == false)
            {
                MessageBox.Show("NET_DVR_Init error!");
                return;
            }
            else
            {
                CHCNetSDK.NET_DVR_SetLogToFile(3, "C:\\SdkLog\\", true);
            }
        }

        private void HideAcsControlAllTabPages()
        {
            tabPageDoorInfo.Parent = null;
            tabPageDoorStatusPlan.Parent = null;
            //tabPageRemoteControlDoor.Parent = null;
            tabPageCardReaderInfo.Parent = null;
            tabPageCardReaderPlan.Parent = null;
            tabPageDeviceStatus.Parent = null;
            tabPageRemoteControl.Parent = null;
            tabPageUserManagement.Parent = null;
            tabPageParamCfg.Parent = null;
            tabPageBasicParam.Parent = null;
            tabPageAcsXml.Parent = null;
        }

        private void MainFrame_Load(object sender, EventArgs e)
        {
            //MultiLanguage.GetDefaultLanguage();
            //MultiLanguage.SetDefaultLanguage("Chinese");

            string language = MultiLanguage.GetDefaultLanguage();
            if (language == "English")
            {
                Thread.CurrentThread.CurrentUICulture = new CultureInfo("en-US");
                comboBoxLanguage.SelectedIndex = 0;
            }
            else if (language == "Chinese")
            {
                Thread.CurrentThread.CurrentUICulture = new CultureInfo("zh-CN");
                comboBoxLanguage.SelectedIndex = 1;
            }
            MultiLanguage.LoadLanguage(this);

            g_deviceTree.TreeClick += new mouseClickDeviceTreeNodeEventHander(mouseClickDeviceTreeNode);

            g_deviceTree.ClearAllNodes();

            tabPageDoorInfo.Parent = null;
            tabPageDoorStatusPlan.Parent = null;
            //tabPageRemoteControlDoor.Parent = null;
            tabPageCardReaderInfo.Parent = null;
            tabPageCardReaderPlan.Parent = null;
            //tabPageUserManagement.Parent = tabControlAcs;
            //tabPageDeviceStatus.Parent = tabControlAcs;
            //tabPageRemoteControl.Parent = tabControlAcs;
            //tabPageParamCfg.Parent = tabControlAcs;
            tabPageBasicParam.Parent = null;
            //tabPageAcsXml.Parent = tabControlAcs;

            //initialize
            m_struAcsWorkStatus.Init();
            m_struCardReaderCfg.Init();

            for (int i = 0; i < iMaxCardNum; i++)
            {
                m_userInfo[i] = new UserInfo();
                m_userInfo[i].cards = new CardInfo[1];
                m_userInfo[i].cards[0] = new CardInfo();
            }
        }

        private void comboBoxLanguage_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (comboBoxLanguage.Text != null)
            {
                MultiLanguage.SetDefaultLanguage(comboBoxLanguage.Text);
                foreach (Form form in Application.OpenForms)
                {
                    MultiLanguage.LoadLanguage(form);
                }


                if (comboBoxLanguage.Text == "English")
                {
                    Thread.CurrentThread.CurrentUICulture = new CultureInfo("en-US");
                }
                else if (comboBoxLanguage.Text == "Chinese")
                {
                    Thread.CurrentThread.CurrentUICulture = new CultureInfo("zh-CN");
                }
            }
        }

        private void buttonClear_Click(object sender, EventArgs e)
        {
            if (radioButtonLocalLog.Checked)
            {
                g_formList.ClearList(0);
            }
            else if (radioButtonAlarmInformation.Checked)
            {
                g_formList.ClearList(1);
            }
        }

        private void buttonSave_Click(object sender, EventArgs e)
        {

        }

        private void radioButtonLocalLog_CheckedChanged(object sender, EventArgs e)
        {
            if (radioButtonLocalLog.Checked)
            {
                g_formList.showList(0);
            }
            else if (radioButtonAlarmInformation.Checked)
            {
                g_formList.showList(1);
            }
        }

        private void buttonCardManagement_Click(object sender, EventArgs e)
        {
            //MessageBox.Show("see user management!");
            int index = g_deviceTree.GetCurDeviceIndex();
            if (index < 0) return;
            CardManagement struCardManagement = new CardManagement();
            struCardManagement.m_lUserID = g_deviceTree.g_struDeviceInfo[index].lLoginID;
            struCardManagement.ShowDialog();
            struCardManagement.Dispose();
        }

        private void buttonUserAdd_Click(object sender, EventArgs e)
        {
            int index = g_deviceTree.GetCurDeviceIndex();
            if (index < 0) return;
            UserAdd userAdd = new UserAdd();
            userAdd.m_lDeviceIndex = index;
            userAdd.m_lUserID = g_deviceTree.g_struDeviceInfo[index].lLoginID;
            userAdd.m_doorNum = g_deviceTree.g_struDeviceInfo[index].iDoorNum;
            userAdd.m_deviceTree = g_deviceTree;
            if (userAdd.ShowDialog() == DialogResult.OK)
            {
                //config set to device
                if (userAdd.g_fConfigCardCallBack(false))
                {
                    //demo built in with one user ,one card and one fingerprint;
                    m_struCardInfo[m_curUserNum].Init();
                    m_struFingerPrintInfo[m_curUserNum].Init();

                    m_userInfo[m_curUserNum] = userAdd.GetUserInfo();
                    m_struCardInfo[m_curUserNum] = userAdd.GetCurCardInfo();
                    m_struFingerPrintInfo[m_curUserNum] = userAdd.GetFingerprintInfo();
                    m_curUserNum++;
                    UpdateUserList();
                }
            }
            userAdd.Dispose();
        }

        private void UpdateUserList()
        {
            this.listViewUserManagement.BeginUpdate();

            ListViewItem lvi = new ListViewItem();
            lvi.Text = m_userInfo[m_curUserNum - 1].memberNo;
            lvi.SubItems.Add(m_userInfo[m_curUserNum - 1].memberName);
            lvi.SubItems.Add(m_userInfo[m_curUserNum - 1].cardNum);
            lvi.SubItems.Add(m_userInfo[m_curUserNum - 1].fingerPrintNum);

            this.listViewUserManagement.Items.Add(lvi);
            this.listViewUserManagement.EndUpdate();
        }

        private void buttonUserModify_Click(object sender, EventArgs e)
        {
            if (listViewUserManagement.SelectedItems.Count <= 0)
            {
                MessageBox.Show("Please select a item");
                return;
            }
            int index = g_deviceTree.GetCurDeviceIndex();
            if (index < 0) return;
            UserAdd userAdd = new UserAdd();
            userAdd.m_lDeviceIndex = index;
            userAdd.m_lUserID = g_deviceTree.g_struDeviceInfo[index].lLoginID;
            userAdd.m_doorNum = g_deviceTree.g_struDeviceInfo[index].iDoorNum;
            userAdd.m_deviceTree = g_deviceTree;

            int selectedCardIndex = listViewUserManagement.SelectedItems[0].Index;
            userAdd.SetCurCardInfo(ref m_struCardInfo[selectedCardIndex], ref m_struFingerPrintInfo[selectedCardIndex], ref m_userInfo[selectedCardIndex]);

            if (userAdd.ShowDialog() == DialogResult.OK)
            {
                //config set to device
                if (userAdd.g_fConfigCardCallBack(false))
                {
                    m_userInfo[selectedCardIndex] = userAdd.GetUserInfo();
                    m_struCardInfo[selectedCardIndex] = userAdd.GetCurCardInfo();
                    m_struFingerPrintInfo[selectedCardIndex] = userAdd.GetFingerprintInfo();
                    ModifyUserList(selectedCardIndex);
                }
            }
            userAdd.Dispose();
        }
        private void ModifyUserList(int index)
        {
            this.listViewUserManagement.BeginUpdate();
            listViewUserManagement.Items[index].Text = m_userInfo[m_curUserNum - 1].memberNo;
            listViewUserManagement.Items[index].SubItems[1].Text = m_userInfo[m_curUserNum - 1].memberName;
            listViewUserManagement.Items[index].SubItems[2].Text = m_userInfo[m_curUserNum - 1].cardNum;
            listViewUserManagement.Items[index].SubItems[3].Text = m_userInfo[m_curUserNum - 1].fingerPrintNum;
            this.listViewUserManagement.EndUpdate();
        }

        private void buttonUserDelete_Click(object sender, EventArgs e)
        {
            if (listViewUserManagement.SelectedItems.Count == 0)
            {
                MessageBox.Show("please select a item");
            }

            int index = g_deviceTree.GetCurDeviceIndex();
            if (index < 0) return;
            UserAdd userAdd = new UserAdd();
            userAdd.m_lDeviceIndex = index;
            userAdd.m_lUserID = g_deviceTree.g_struDeviceInfo[index].lLoginID;
            userAdd.m_doorNum = g_deviceTree.g_struDeviceInfo[index].iDoorNum;
            userAdd.g_fDeleteInfoCallBack();
            userAdd.Dispose();
        }

        private void tabControlAcs_SelectedIndexChanged(object sender, EventArgs e)
        {
            int iDeviceIndex = g_deviceTree.GetCurDeviceIndex();
            if (tabControlAcs.SelectedTab != null && m_bValidate && iDeviceIndex >= 0)
            {
                switch (tabControlAcs.SelectedTab.Name)
                {
                    case "tabPageDeviceStatus":
                        GetAcsWorkStatus(g_deviceTree.g_struDeviceInfo[iDeviceIndex].lLoginID, ref m_struAcsWorkStatus);
                        UpdateDoorStatus();
                        g_deviceTree.UpdateTreeNode();
                        break;
                    case "tabPageDoorInfo":
                        ShowDoorConfiguration();
                        g_deviceTree.UpdateTreeNode();
                        break;
                    case "tabPageCardReaderInfo":
                        ShowCardReaderConfiguration();
                        g_deviceTree.UpdateTreeNode();
                        break;
                    default:
                        break;
                }
            }
        }

        // update door status
        private void UpdateDoorStatus()
        {
            // clear items for update
            listViewDoorStatus.Items.Clear();

            int iDeviceIndex = g_deviceTree.GetCurDeviceIndex();
            //need to select device
            if (iDeviceIndex < 0) return;
            //device has been login out
            if (g_deviceTree.g_struDeviceInfo[iDeviceIndex].lLoginID < 0) return;

            int doorNum = g_deviceTree.g_struDeviceInfo[iDeviceIndex].iDoorNum;

            ListViewItem[] lvis = new ListViewItem[doorNum];

            for (int i = 0; i < doorNum; i++)
            {
                //initialize
                lvis[i] = new ListViewItem((i + 1).ToString());

                string[] items = new string[3];
                // door lock status(relay status)
                switch (m_struAcsWorkStatus.byDoorLockStatus[i])
                {
                    case 0:
                        items[0] = "Close";
                        g_deviceTree.g_struDeviceInfo[iDeviceIndex].iDoorStatus[3 * i] = AcsDemoPublic.DOOR_COLSED;
                        break;
                    case 1:
                        items[0] = "Open";
                        g_deviceTree.g_struDeviceInfo[iDeviceIndex].iDoorStatus[3 * i] = AcsDemoPublic.DOOR_OPEN;
                        break;
                    case 2:
                        items[0] = "Damage short-circuit alarm";
                        break;
                    case 3:
                        items[0] = "Damage breaking alarm";
                        break;
                    case 4:
                        items[0] = "Abnormal alarm";
                        break;
                    default:
                        items[0] = "Unknown";
                        break;
                }

                // door magnetic status
                switch (m_struAcsWorkStatus.byMagneticStatus[i])
                {
                    case 0:
                        items[1] = "Close";
                        break;
                    case 1:
                        items[1] = "Open";
                        break;
                    case 2:
                        items[1] = "Damage short-circuit alarm";
                        break;
                    case 3:
                        items[1] = "Damage breaking alarm";
                        break;
                    case 4:
                        items[1] = "Abnormal alarm";
                        break;
                    default:
                        items[1] = "Unknown";
                        break;
                }

                // door status(floor status)
                switch (m_struAcsWorkStatus.byDoorStatus[i])
                {
                    case 1:
                        items[2] = "dormant";
                        break;
                    case 2:
                        items[2] = "Always Open(free)";
                        break;
                    case 3:
                        items[2] = "Always Close(forbidden)";
                        break;
                    case 4:
                        items[2] = "Common(Under control)";
                        break;
                    default:
                        items[2] = "Unknown";
                        break;
                }

                lvis[i].SubItems.AddRange(items);
            }

            listViewDoorStatus.Items.AddRange(lvis);
        }

        // update reader card status
        private void UpdateCardReaderStatus()
        {
            // clear items for update
            listViewCardReaderStatus.Items.Clear();

            int iDeviceIndex = g_deviceTree.GetCurDeviceIndex();
            //need to select device
            if (iDeviceIndex < 0) return;
            //device has been login out
            if (g_deviceTree.g_struDeviceInfo[iDeviceIndex].lLoginID < 0) return;

            //reader card number is two times of door number
            int readerCardNum = 2 * g_deviceTree.g_struDeviceInfo[iDeviceIndex].iDoorNum;

            ListViewItem[] lvis = new ListViewItem[readerCardNum];
            for (int i = 0; i < readerCardNum; i++)
            {
                //initialize
                lvis[i] = new ListViewItem((i + 1).ToString());

                string[] items = new string[3];
                // card reader status
                switch (m_struAcsWorkStatus.byCardReaderOnlineStatus[i])
                {
                    case 0:
                        items[0] = "Offline";
                        g_deviceTree.g_struDeviceInfo[iDeviceIndex].iDoorStatus[3 * (i / 2) + (i % 2) + 1] = AcsDemoPublic.CARD_OFF_LINE;
                        //g_deviceTree.g_struDeviceInfo[iDeviceIndex].iDoorStatus[3 * i + 2] = AcsDemoPublic.CARD_OFF_LINE;
                        break;
                    case 1:
                        items[0] = "Online";
                        g_deviceTree.g_struDeviceInfo[iDeviceIndex].iDoorStatus[3 * (i / 2) + (i % 2) + 1] = AcsDemoPublic.CARD_ON_LINE;
                        //g_deviceTree.g_struDeviceInfo[iDeviceIndex].iDoorStatus[3 * i + 2] = AcsDemoPublic.CARD_ON_LINE;
                        break;
                    default:
                        items[0] = "Unknown";
                        break;
                }

                // card reader anti dismantle status
                switch (m_struAcsWorkStatus.byCardReaderAntiDismantleStatus[i])
                {
                    case 0:
                        items[1] = "Disabled";
                        break;
                    case 1:
                        items[1] = "Enabled";
                        break;
                    default:
                        items[1] = "Unknown";
                        break;
                }

                // card reader verify mode
                switch (m_struAcsWorkStatus.byCardReaderVerifyMode[i])
                {
                    case 1:
                        items[2] = "Dormant";
                        break;
                    case 2:
                        items[2] = "Card and password";
                        break;
                    case 3:
                        items[2] = "Card";
                        break;
                    case 4:
                        items[2] = "Card or password";
                        break;
                    case 5:
                        items[2] = "Fingerprint";
                        break;
                    case 6:
                        items[2] = "Fingerprint and password";
                        break;
                    case 7:
                        items[2] = "Fingerprint or Card";
                        break;
                    case 8:
                        items[2] = "Fingerprint and Card";
                        break;
                    case 9:
                        items[2] = "Fingerprint and Card and password";
                        break;
                    case 10:
                        items[2] = "Face or fingerprint or card or password";
                        break;
                    case 11:
                        items[2] = "Face and fingerprint";
                        break;
                    case 12:
                        items[2] = "Face and password";
                        break;
                    case 13:
                        items[2] = "Face and card";
                        break;
                    case 14:
                        items[2] = "Face";
                        break;
                    case 15:
                        items[2] = "Employee no and password";
                        break;
                    case 16:
                        items[2] = "Fingerprint or password";
                        break;
                    case 17:
                        items[2] = "Employee no and fingerprint";
                        break;
                    case 18:
                        items[2] = "Employee no and fingerprint and password";
                        break;
                    case 19:
                        items[2] = "Face and fingerprint and card";
                        break;
                    case 20:
                        items[2] = "Face and password and fingerprint";
                        break;
                    case 21:
                        items[2] = "Employee no and face";
                        break;
                    default:
                        items[2] = "Unknown";
                        break;
                }

                lvis[i].SubItems.AddRange(items);
            }
            listViewCardReaderStatus.Items.AddRange(lvis);
        }

        // update case sensor status
        private void UpdateCaseSensorStatus()
        {
            // clear items for update
            listViewCaseStatus.Items.Clear();

            int iDeviceIndex = g_deviceTree.GetCurDeviceIndex();
            //need to select device
            if (iDeviceIndex < 0) return;
            //device has been login out
            if (g_deviceTree.g_struDeviceInfo[iDeviceIndex].lLoginID < 0) return;

            ListViewItem[] lvis = new ListViewItem[CHCNetSDK.MAX_CASE_SENSOR_NUM];

            for (int i = 0; i < CHCNetSDK.MAX_CASE_SENSOR_NUM; i++)
            {
                //initialize
                lvis[i] = new ListViewItem((i + 1).ToString());

                string item;
                // Case Status
                switch (m_struAcsWorkStatus.byCaseStatus[i])
                {
                    case 0:
                        item = "No input";
                        break;
                    case 1:
                        item = "Input";
                        break;
                    default:
                        item = "Unknown";
                        break;
                }
                lvis[i].SubItems.Add(item);
            }

            listViewCaseStatus.Items.AddRange(lvis);
        }

        private void UpdateWorkStatus()
        {
            UpdateDoorStatus();
            UpdateCardReaderStatus();
            UpdateCaseSensorStatus();
            UpdateAlarmInStatus();
            UpdateAlarmOutStatus();
            UpdateHostStatus();
        }

        // update alarm in status
        private void UpdateAlarmInStatus()
        {
            // clear items for update
            listViewAlarmInStatus.Items.Clear();

            int iDeviceIndex = g_deviceTree.GetCurDeviceIndex();
            //need to select device
            if (iDeviceIndex < 0) return;
            //device has been login out
            if (g_deviceTree.g_struDeviceInfo[iDeviceIndex].lLoginID < 0) return;

            ListViewItem[] lvis = new ListViewItem[CHCNetSDK.MAX_ALARMHOST_ALARMIN_NUM];

            for (int i = 0; i < CHCNetSDK.MAX_ALARMHOST_ALARMIN_NUM; i++)
            {
                //initialize
                lvis[i] = new ListViewItem((i + 1).ToString());

                string[] items = new string[2];

                // alarm in setup Status
                switch (m_struAcsWorkStatus.bySetupAlarmStatus[i])
                {
                    case 0:
                        items[0] = "Unguard";
                        break;
                    case 1:
                        items[0] = "Guard";
                        break;
                    default:
                        items[0] = "Unknown";
                        break;
                }

                // alarm in Status
                switch (m_struAcsWorkStatus.byAlarmInStatus[i])
                {
                    case 0:
                        items[1] = "No input";
                        break;
                    case 1:
                        items[1] = "Input";
                        break;
                    default:
                        items[1] = "Unknown";
                        break;
                }

                lvis[i].SubItems.AddRange(items);
            }

            listViewAlarmInStatus.Items.AddRange(lvis);
        }

        // update alarm out status
        private void UpdateAlarmOutStatus()
        {
            // clear items for update
            listViewAlarmOutStatus.Items.Clear();

            int iDeviceIndex = g_deviceTree.GetCurDeviceIndex();
            //need to select device
            if (iDeviceIndex < 0) return;
            //device has been login out
            if (g_deviceTree.g_struDeviceInfo[iDeviceIndex].lLoginID < 0) return;

            ListViewItem[] lvis = new ListViewItem[CHCNetSDK.MAX_ALARMHOST_ALARMOUT_NUM];

            for (int i = 0; i < CHCNetSDK.MAX_ALARMHOST_ALARMOUT_NUM; i++)
            {
                //initialize
                lvis[i] = new ListViewItem((i + 1).ToString());

                string item;

                // alarm out Status
                switch (m_struAcsWorkStatus.byAlarmOutStatus[i])
                {
                    case 0:
                        item = "No output";
                        break;
                    case 1:
                        item = "Output";
                        break;
                    default:
                        item = "Unknown";
                        break;
                }

                lvis[i].SubItems.Add(item);
            }

            listViewAlarmOutStatus.Items.AddRange(lvis);
        }

        // update host status
        private void UpdateHostStatus()
        {
            int iDeviceIndex = g_deviceTree.GetCurDeviceIndex();
            // initialize
            comboBoxPowerSupplyMode.SelectedIndex = 0;
            comboBoxPowerSupplyMode.SelectedIndex = 0;
            comboBoxIndicatorLightStatus.SelectedIndex = 0;
            comboBoxFireAlarmStatus.SelectedIndex = 0;

            textBoxBatteryVoltage.Text = "";
            textBoxCardNum.Text = "";

            checkBoxLowBattery.Checked = false;
            checkBoxAntiSneak.Checked = false;
            checkBoxHostAntiDismantle.Checked = false;
            checkBoxMultidoorInterlock.Checked = false;

            //need to select device
            if (iDeviceIndex < 0) return;
            //device has been login out
            if (g_deviceTree.g_struDeviceInfo[iDeviceIndex].lLoginID < 0) return;

            comboBoxPowerSupplyMode.SelectedIndex = m_struAcsWorkStatus.byPowerSupplyStatus;
            comboBoxIndicatorLightStatus.SelectedIndex = m_struAcsWorkStatus.byIndicatorLightStatus;
            comboBoxFireAlarmStatus.SelectedIndex = m_struAcsWorkStatus.byFireAlarmStatus;

            textBoxBatteryVoltage.Text = ((float)m_struAcsWorkStatus.wBatteryVoltage / 10).ToString();
            textBoxCardNum.Text = m_struAcsWorkStatus.dwCardNum.ToString();

            checkBoxLowBattery.Checked = (m_struAcsWorkStatus.byBatteryLowVoltage == 1) ? true : false;
            checkBoxAntiSneak.Checked = (m_struAcsWorkStatus.byAntiSneakStatus == 1) ? true : false;
            checkBoxHostAntiDismantle.Checked = (m_struAcsWorkStatus.byHostAntiDismantleStatus == 1) ? true : false;
            checkBoxMultidoorInterlock.Checked = (m_struAcsWorkStatus.byMultiDoorInterlockStatus == 1) ? true : false;
        }

        private void AcsDemo_FormClosing(object sender, FormClosingEventArgs e)
        {
            for (int i = 0; i < AcsDemoPublic.MAX_DEVICES; i++)
            {
                if (g_deviceTree.g_struDeviceInfo[i].lLoginID >= 0)
                {
                    if (g_deviceTree.g_struDeviceInfo[i].lFortifyHandle >= 0)
                    {
                        if (CHCNetSDK.NET_DVR_CloseAlarmChan(g_deviceTree.g_struDeviceInfo[i].lFortifyHandle))
                        {
                            g_deviceTree.g_struDeviceInfo[i].lFortifyHandle = -1;
                        }
                    }

                    CHCNetSDK.NET_DVR_Logout_V30(g_deviceTree.g_struDeviceInfo[i].lLoginID);

                    g_deviceTree.g_struDeviceInfo[i].lLoginID = -1;
                }
            }
            CHCNetSDK.NET_DVR_Cleanup();
        }

        private void tabControlDeviceStatus_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (tabControlDeviceStatus.SelectedTab != null && m_bValidate)
            {
                switch (tabControlDeviceStatus.SelectedTab.Name)
                {
                    case "tabPageDoorStatus":
                        UpdateDoorStatus();
                        g_deviceTree.UpdateTreeNode();
                        break;
                    case "tabPageCardReaderStatus":
                        UpdateCardReaderStatus();
                        g_deviceTree.UpdateTreeNode();
                        break;
                    case "tabPageHostStatus":
                        UpdateHostStatus();
                        break;
                    case "tabPageAlarmInStatus":
                        UpdateAlarmInStatus();
                        break;
                    case "tabPageAlarmOutStatus":
                        UpdateAlarmOutStatus();
                        break;
                    case "tabPageCaseStatus":
                        UpdateCaseSensorStatus();
                        break;
                    default:
                        break;
                }
            }
        }

        private void buttonUserManagement_Click(object sender, EventArgs e)
        {
            int index = g_deviceTree.GetCurDeviceIndex();
            if (index < 0) return;

            UserManagement userManage = new UserManagement();
            userManage.m_lUserID = g_deviceTree.g_struDeviceInfo[index].lLoginID;
            userManage.ShowDialog();
        }

        private void buttonDoorWeekPlanManagement_Click(object sender, EventArgs e)
        {
            int index = g_deviceTree.GetCurDeviceIndex();
            if (index < 0) return;

            WeekPlan doorWeekPlan = new WeekPlan();
            doorWeekPlan.m_iDeviceType = 0;
            doorWeekPlan.m_iDeviceIndex = index;
            doorWeekPlan.m_lUserID = g_deviceTree.g_struDeviceInfo[index].lLoginID;

            doorWeekPlan.CustomRelayout();
            doorWeekPlan.ShowDialog();
            doorWeekPlan.Dispose();
        }

        private void buttonDoorHolidayPlanManagement_Click(object sender, EventArgs e)
        {
            int index = g_deviceTree.GetCurDeviceIndex();
            if (index < 0) return;

            HolidayPlan doorHolidayPlan = new HolidayPlan();
            doorHolidayPlan.m_iDeviceType = 0;
            doorHolidayPlan.m_iDeviceIndex = index;
            doorHolidayPlan.m_lUserID = g_deviceTree.g_struDeviceInfo[index].lLoginID;

            doorHolidayPlan.CustomRelayout();
            doorHolidayPlan.ShowDialog();
            doorHolidayPlan.Dispose();

        }

        private void buttonDoorHolidayGroupManagement_Click(object sender, EventArgs e)
        {
            int index = g_deviceTree.GetCurDeviceIndex();
            if (index < 0) return;

            HolidayGroupPlan doorHolidayGroupPlan = new HolidayGroupPlan();
            doorHolidayGroupPlan.m_iDeviceType = 0;
            doorHolidayGroupPlan.m_iDeviceIndex = index;
            doorHolidayGroupPlan.m_lUserID = g_deviceTree.g_struDeviceInfo[index].lLoginID;

            doorHolidayGroupPlan.CustomRelayout();
            doorHolidayGroupPlan.ShowDialog();
            doorHolidayGroupPlan.Dispose();
        }

        private void buttonDoorPlanTemplateManagement_Click(object sender, EventArgs e)
        {
            int index = g_deviceTree.GetCurDeviceIndex();
            if (index < 0) return;
            PlanTemplate doorTemplate = new PlanTemplate();
            doorTemplate.m_iDeviceType = 0;
            doorTemplate.m_iDeviceIndex = index;
            doorTemplate.m_lUserID = g_deviceTree.g_struDeviceInfo[index].lLoginID;
            doorTemplate.CustomRelayout();
            doorTemplate.ShowDialog();
            doorTemplate.Dispose();
        }

        private void buttonDoorStatusPlanManagement_Click(object sender, EventArgs e)
        {
            int iDeviceIndex = g_deviceTree.GetCurDeviceIndex();
            int iDoorIndex = g_deviceTree.GetCurDoorIndex();

            // selected a door
            if (iDeviceIndex < 0 || iDoorIndex < 0) return;

            DevicePlanStatus doorPlanStatus = new DevicePlanStatus();
            doorPlanStatus.m_iDeviceType = 0;
            doorPlanStatus.m_iDeviceIndex = iDeviceIndex;

            //iDoorIndex begin with 0, but following case begin with 1,thus plus 1;
            doorPlanStatus.m_iIndex = iDoorIndex + 1;
            doorPlanStatus.m_lUserID = g_deviceTree.g_struDeviceInfo[iDeviceIndex].lLoginID;
            doorPlanStatus.CustomRelayout();
            doorPlanStatus.ShowDialog();
            doorPlanStatus.Dispose();
        }

        private void buttonDoorCfgSave_Click(object sender, EventArgs e)
        {
            int iDeviceIndex = g_deviceTree.GetCurDeviceIndex();

            //GetCurDoorIndex() begin with 0, but following case begin with 1,thus plus 1;
            int iDoorIndex = g_deviceTree.GetCurDoorIndex() + 1;

            // selected a door
            if (iDeviceIndex < 0 || iDoorIndex < 0) return;
            int lUserID = g_deviceTree.g_struDeviceInfo[iDeviceIndex].lLoginID;

            CHCNetSDK.NET_DVR_DOOR_CFG struDoorCfg = new CHCNetSDK.NET_DVR_DOOR_CFG();
            struDoorCfg.Init();

            //get door name string from correspond control
            byte[] byTemp = System.Text.Encoding.UTF8.GetBytes(textBoxDoorName.Text);
            for (int i = 0; i < byTemp.Length && i < struDoorCfg.byDoorName.Length; i++)
            {
                struDoorCfg.byDoorName[i] = byTemp[i];
            }

            struDoorCfg.byMagneticType = (byte)comboBoxMagneticType.SelectedIndex;
            struDoorCfg.byOpenButtonType = (byte)comboBoxOpenButtonType.SelectedIndex;

            if (!byte.TryParse(textBoxOpenDuration.Text, out struDoorCfg.byOpenDuration))
            {
                //set a default value
                MessageBox.Show("Open Duration value should be 1-255!, here has set default 5");
                struDoorCfg.byOpenDuration = 5;
            }
            if (!byte.TryParse(textBoxDisabledOpenDuration.Text, out struDoorCfg.byDisabledOpenDuration))
            {
                //set a default value
                MessageBox.Show("Disabled Card Open Duration value should be 1-255!, here has set default 5");
                struDoorCfg.byDisabledOpenDuration = 5;
            }
            if (!byte.TryParse(textBoxMagneticAlarmTimeout.Text, out struDoorCfg.byMagneticAlarmTimeout))
            {
                //set a default value
                MessageBox.Show("Magnetic Alarm Timeout value should be 0-255!, here has set default 0(means no alarm)");
                struDoorCfg.byMagneticAlarmTimeout = 0;
            }
            if (!uint.TryParse(textBoxLeaderCardOpenDuration.Text, out struDoorCfg.dwLeaderCardOpenDuration))
            {
                //set a default value
                MessageBox.Show("Leader Card Open Duration value should be 1-1440!, here has set default 1");
                struDoorCfg.dwLeaderCardOpenDuration = 1;
            }

            //get stress password string from correspond control
            byTemp = System.Text.Encoding.UTF8.GetBytes(textBoxStressPassword.Text);
            for (int i = 0; i < byTemp.Length && i < struDoorCfg.byStressPassword.Length; i++)
            {
                struDoorCfg.byStressPassword[i] = byTemp[i];
            }

            //get super password string from correspond control
            byTemp = System.Text.Encoding.UTF8.GetBytes(textBoxSuperPassword.Text);
            for (int i = 0; i < byTemp.Length && i < struDoorCfg.bySuperPassword.Length; i++)
            {
                struDoorCfg.bySuperPassword[i] = byTemp[i];
            }

            //get unlock password string from correspond control
            byTemp = System.Text.Encoding.UTF8.GetBytes(textBoxUnlockPassword.Text);
            for (int i = 0; i < byTemp.Length && i < struDoorCfg.byUnlockPassword.Length; i++)
            {
                struDoorCfg.byUnlockPassword[i] = byTemp[i];
            }

            struDoorCfg.byEnableDoorLock = (checkBoxLockTheDoor.Checked == true) ? (byte)1 : (byte)0;
            struDoorCfg.byEnableLeaderCard = (checkBoxFirstCardOpen.Checked == true) ? (byte)1 : (byte)0;
            struDoorCfg.byLockInputCheck = (checkBoxDoorLockInputDetection.Checked == true) ? (byte)1 : (byte)0;
            struDoorCfg.byOpenButton = (checkBoxEnableOpenButton.Checked == true) ? (byte)1 : (byte)0;

            struDoorCfg.byDoorTerminalMode = (byte)comboBoxDoorTerminalMode.SelectedIndex;
            struDoorCfg.byLockInputType = (byte)comboBoxLockInputType.SelectedIndex;

            SetDoorConfiguration(lUserID, iDoorIndex, ref struDoorCfg);

            //save the name to the global variant and update tree node
            g_deviceTree.g_struDeviceInfo[iDeviceIndex].sDoorName[iDoorIndex - 1] = textBoxDoorName.Text;
            g_deviceTree.UpdateTreeNode();
        }

        // display the door configuration on the controls
        private void ShowDoorConfiguration()
        {
            int iDeviceIndex = g_deviceTree.GetCurDeviceIndex();

            //GetCurDoorIndex() begin with 0, but following case begin with 1,thus plus 1;
            int iDoorIndex = g_deviceTree.GetCurDoorIndex() + 1;

            // selected a door
            if (iDeviceIndex < 0 || iDoorIndex <= 0) return;
            int lUserID = g_deviceTree.g_struDeviceInfo[iDeviceIndex].lLoginID;

            CHCNetSDK.NET_DVR_DOOR_CFG struDoorCfg = new CHCNetSDK.NET_DVR_DOOR_CFG();
            struDoorCfg.Init();
            GetDoorConfiguration(lUserID, iDoorIndex, ref struDoorCfg);

            //show the parameters on controls
            textBoxDoorName.Text = System.Text.Encoding.UTF8.GetString(struDoorCfg.byDoorName);

            //save the name to the global variant and update tree node
            g_deviceTree.g_struDeviceInfo[iDeviceIndex].sDoorName[iDoorIndex - 1] = textBoxDoorName.Text;
            g_deviceTree.UpdateTreeNode();

            textBoxStressPassword.Text = System.Text.Encoding.UTF8.GetString(struDoorCfg.byStressPassword);
            textBoxSuperPassword.Text = System.Text.Encoding.UTF8.GetString(struDoorCfg.bySuperPassword);
            textBoxUnlockPassword.Text = System.Text.Encoding.UTF8.GetString(struDoorCfg.byUnlockPassword);

            checkBoxLockTheDoor.Checked = (0 == struDoorCfg.byEnableDoorLock) ? false : true;
            checkBoxFirstCardOpen.Checked = (0 == struDoorCfg.byEnableLeaderCard) ? false : true;
            checkBoxEnableOpenButton.Checked = (0 == struDoorCfg.byOpenButton) ? false : true;
            checkBoxDoorLockInputDetection.Checked = (0 == struDoorCfg.byLockInputCheck) ? false : true;
            checkBoxIsUseLocalController.Checked = (0 == struDoorCfg.byUseLocalController) ? false : true;

            comboBoxMagneticType.SelectedIndex = struDoorCfg.byMagneticType;
            comboBoxOpenButtonType.SelectedIndex = struDoorCfg.byOpenButtonType;
            comboBoxLeaderCardMode.SelectedIndex = struDoorCfg.byLeaderCardMode;
            comboBoxLockInputType.SelectedIndex = struDoorCfg.byLockInputType;
            comboBoxDoorTerminalMode.SelectedIndex = struDoorCfg.byDoorTerminalMode;

            textBoxOpenDuration.Text = struDoorCfg.byOpenDuration.ToString();
            textBoxDisabledOpenDuration.Text = struDoorCfg.byDisabledOpenDuration.ToString();
            textBoxMagneticAlarmTimeout.Text = struDoorCfg.byMagneticAlarmTimeout.ToString();
            textBoxLeaderCardOpenDuration.Text = struDoorCfg.dwLeaderCardOpenDuration.ToString();

            textBoxLocalControllerID.Text = (struDoorCfg.wLocalControllerID == 0) ? "Unregister" : struDoorCfg.wLocalControllerID.ToString();
            textBoxLocalControllerNumber.Text = (struDoorCfg.wLocalControllerDoorNumber == 0) ? "Unregister" : struDoorCfg.wLocalControllerDoorNumber.ToString();
            comboBoxLocalControllerStatus.SelectedIndex = struDoorCfg.wLocalControllerStatus;

            return;
        }

        private bool GetDoorConfiguration(int userID, int doorNo, ref CHCNetSDK.NET_DVR_DOOR_CFG struDoorCfg)
        {
            uint dwReturned = 0;
            uint dwSize = (uint)Marshal.SizeOf(struDoorCfg);
            struDoorCfg.dwSize = dwSize;
            IntPtr ptrDoorCfg = Marshal.AllocHGlobal((Int32)dwSize);
            Marshal.StructureToPtr(struDoorCfg, ptrDoorCfg, false);

            if (!CHCNetSDK.NET_DVR_GetDVRConfig(userID, CHCNetSDK.NET_DVR_GET_DOOR_CFG, doorNo, ptrDoorCfg, dwSize, ref dwReturned))
            {
                g_formList.AddLog(userID, AcsDemoPublic.OPERATION_FAIL_T, "NET_DVR_GET_DOOR_CFG");
                MessageBox.Show("Get door configuration fail error code :" + CHCNetSDK.NET_DVR_GetLastError().ToString());
                Marshal.FreeHGlobal(ptrDoorCfg);
                ptrDoorCfg = IntPtr.Zero;
                return false;
            }
            else
            {
                g_formList.AddLog(userID, AcsDemoPublic.OPERATION_SUCC_T, "NET_DVR_GET_DOOR_CFG");
                struDoorCfg = (CHCNetSDK.NET_DVR_DOOR_CFG)Marshal.PtrToStructure(ptrDoorCfg, typeof(CHCNetSDK.NET_DVR_DOOR_CFG));
                Marshal.FreeHGlobal(ptrDoorCfg);
                ptrDoorCfg = IntPtr.Zero;
                return true;
            }
        }

        private bool SetDoorConfiguration(int userID, int doorNo, ref CHCNetSDK.NET_DVR_DOOR_CFG struDoorCfg)
        {
            uint dwSize = (uint)Marshal.SizeOf(struDoorCfg);
            struDoorCfg.dwSize = dwSize;
            IntPtr ptrDoorCfg = Marshal.AllocHGlobal((Int32)dwSize);
            Marshal.StructureToPtr(struDoorCfg, ptrDoorCfg, false);

            if (!CHCNetSDK.NET_DVR_SetDVRConfig(userID, CHCNetSDK.NET_DVR_SET_DOOR_CFG, doorNo, ptrDoorCfg, dwSize))
            {
                g_formList.AddLog(userID, AcsDemoPublic.OPERATION_FAIL_T, "NET_DVR_SET_DOOR_CFG");
                MessageBox.Show("Set door configuration fail error code :" + CHCNetSDK.NET_DVR_GetLastError().ToString());
                Marshal.FreeHGlobal(ptrDoorCfg);
                ptrDoorCfg = IntPtr.Zero;
                return false;
            }
            else
            {
                g_formList.AddLog(userID, AcsDemoPublic.OPERATION_SUCC_T, "NET_DVR_SET_DOOR_CFG");
                Marshal.FreeHGlobal(ptrDoorCfg);
                ptrDoorCfg = IntPtr.Zero;
                return true;
            }
        }


        // return:1-success 0-failed
        private bool GetAcsWorkStatus(int userID, ref CHCNetSDK.NET_DVR_ACS_WORK_STATUS_V50 struAcsWorkStatus)
        {
            uint dwReturned = 0;
            uint dwSize = (uint)Marshal.SizeOf(struAcsWorkStatus);
            struAcsWorkStatus.dwSize = dwSize;
            IntPtr ptrAcsWorkStatus = Marshal.AllocHGlobal((Int32)dwSize);
            Marshal.StructureToPtr(struAcsWorkStatus, ptrAcsWorkStatus, false);

            if (!CHCNetSDK.NET_DVR_GetDVRConfig(userID, CHCNetSDK.NET_DVR_GET_ACS_WORK_STATUS_V50, 0, ptrAcsWorkStatus, dwSize, ref dwReturned))
            {
                g_formList.AddLog(userID, AcsDemoPublic.OPERATION_FAIL_T, "NET_DVR_GET_ACS_WORK_STATUS_V50");
                MessageBox.Show("Get ACS work status fail error code :" + CHCNetSDK.NET_DVR_GetLastError().ToString());
                Marshal.FreeHGlobal(ptrAcsWorkStatus);
                ptrAcsWorkStatus = IntPtr.Zero;
                return false;
            }
            else
            {
                g_formList.AddLog(userID, AcsDemoPublic.OPERATION_SUCC_T, "NET_DVR_GET_ACS_WORK_STATUS_V50");
                struAcsWorkStatus = (CHCNetSDK.NET_DVR_ACS_WORK_STATUS_V50)Marshal.PtrToStructure(ptrAcsWorkStatus, typeof(CHCNetSDK.NET_DVR_ACS_WORK_STATUS_V50));
                Marshal.FreeHGlobal(ptrAcsWorkStatus);
                return true;
            }
        }

        private void comboBoxLeaderCardMode_SelectedIndexChanged(object sender, EventArgs e)
        {
            //2 - the first card authorization mode (using this field, the byEnableLeaderCard is invalid ) 
            if (comboBoxLeaderCardMode.SelectedIndex != 1)
            {
                checkBoxFirstCardOpen.Checked = false;
                checkBoxFirstCardOpen.Enabled = false;
            }
            else
            {
                checkBoxFirstCardOpen.Enabled = true;
            }
        }

        private void checkBoxIsUseLocalController_CheckedChanged(object sender, EventArgs e)
        {
            if (checkBoxIsUseLocalController.Checked == false)
            {
                textBoxLocalControllerID.Hide();
                labelLocalControllerID.Hide();
                labelLocalControllerNumber.Hide();
                textBoxLocalControllerNumber.Hide();
                labelLocalControllerStatus.Hide();
                comboBoxLocalControllerStatus.Hide();
            }
            else
            {
                textBoxLocalControllerID.Show();
                labelLocalControllerID.Show();
                labelLocalControllerNumber.Show();
                textBoxLocalControllerNumber.Show();
                labelLocalControllerStatus.Show();
                comboBoxLocalControllerStatus.Show();
            }
        }

        private void buttonCardReaderWeekPlanManagement_Click(object sender, EventArgs e)
        {
            int index = g_deviceTree.GetCurDeviceIndex();
            if (index < 0) return;

            WeekPlan cardReaderWeekPlan = new WeekPlan();
            cardReaderWeekPlan.m_iDeviceType = 1;
            cardReaderWeekPlan.m_iDeviceIndex = index;
            cardReaderWeekPlan.m_lUserID = g_deviceTree.g_struDeviceInfo[index].lLoginID;

            cardReaderWeekPlan.CustomRelayout();
            cardReaderWeekPlan.ShowDialog();
            cardReaderWeekPlan.Dispose();
        }

        private void buttonCardReaderHolidayPlanManagement_Click(object sender, EventArgs e)
        {
            int index = g_deviceTree.GetCurDeviceIndex();
            if (index < 0) return;

            HolidayPlan cardReaderHolidayPlan = new HolidayPlan();
            cardReaderHolidayPlan.m_iDeviceType = 1;
            cardReaderHolidayPlan.m_iDeviceIndex = index;
            cardReaderHolidayPlan.m_lUserID = g_deviceTree.g_struDeviceInfo[index].lLoginID;

            cardReaderHolidayPlan.CustomRelayout();
            cardReaderHolidayPlan.ShowDialog();
            cardReaderHolidayPlan.Dispose();
        }

        private void buttonCardReaderHolidayGroupManagement_Click(object sender, EventArgs e)
        {
            int index = g_deviceTree.GetCurDeviceIndex();
            if (index < 0) return;

            HolidayGroupPlan cardReaderHolidayGroupPlan = new HolidayGroupPlan();
            cardReaderHolidayGroupPlan.m_iDeviceType = 1;
            cardReaderHolidayGroupPlan.m_iDeviceIndex = index;
            cardReaderHolidayGroupPlan.m_lUserID = g_deviceTree.g_struDeviceInfo[index].lLoginID;

            cardReaderHolidayGroupPlan.CustomRelayout();
            cardReaderHolidayGroupPlan.ShowDialog();
            cardReaderHolidayGroupPlan.Dispose();
        }

        private void buttonCardReaderPlanTemplateManagement_Click(object sender, EventArgs e)
        {
            int index = g_deviceTree.GetCurDeviceIndex();
            if (index < 0) return;

            PlanTemplate cardReaderTemplate = new PlanTemplate();
            cardReaderTemplate.m_iDeviceType = 1;
            cardReaderTemplate.m_iDeviceIndex = index;
            cardReaderTemplate.m_lUserID = g_deviceTree.g_struDeviceInfo[index].lLoginID;
            cardReaderTemplate.CustomRelayout();
            cardReaderTemplate.ShowDialog();
            cardReaderTemplate.Dispose();
        }
        private void buttonCardReaderPlanManagement_Click(object sender, EventArgs e)
        {
            int iDeviceIndex = g_deviceTree.GetCurDeviceIndex();
            int iDoorIndex = g_deviceTree.GetCurDoorIndex();

            // selected a door
            if (iDeviceIndex < 0 || iDoorIndex < 0) return;

            DevicePlanStatus cardReaderPlanStatus = new DevicePlanStatus();
            cardReaderPlanStatus.m_iDeviceType = 1;
            cardReaderPlanStatus.m_iDeviceIndex = iDeviceIndex;

            //iDoorIndex begin with 0, but following case begin with 1,thus plus 1;
            cardReaderPlanStatus.m_iIndex = iDoorIndex + 1;
            cardReaderPlanStatus.m_lUserID = g_deviceTree.g_struDeviceInfo[iDeviceIndex].lLoginID;
            cardReaderPlanStatus.CustomRelayout();
            cardReaderPlanStatus.ShowDialog();
            cardReaderPlanStatus.Dispose();
        }

        private void buttoncardReaderStatusPlanManagement_Click(object sender, EventArgs e)
        {
            int iDeviceIndex = g_deviceTree.GetCurDeviceIndex();
            int iCardReaderIndex = g_deviceTree.GetCurCardReaderIndex();

            // selected a cardReader
            if (iDeviceIndex < 0 || iCardReaderIndex < 0) return;

            DevicePlanStatus cardReaderPlanStatus = new DevicePlanStatus();
            cardReaderPlanStatus.m_iDeviceType = 0;
            cardReaderPlanStatus.m_iDeviceIndex = iDeviceIndex;

            //icardReaderIndex begin with 0, but following case begin with 1,thus plus 1;
            cardReaderPlanStatus.m_iIndex = iCardReaderIndex + 1;
            cardReaderPlanStatus.m_lUserID = g_deviceTree.g_struDeviceInfo[iDeviceIndex].lLoginID;
            cardReaderPlanStatus.CustomRelayout();
            cardReaderPlanStatus.ShowDialog();
            cardReaderPlanStatus.Dispose();
        }

        private void buttonClearAcsHost_Click(object sender, EventArgs e)
        {
            ClearACSParameters clearAcs = new ClearACSParameters();
            int index = g_deviceTree.GetCurDeviceIndex();
            if (index < 0) return;
            clearAcs.m_lUserID = g_deviceTree.g_struDeviceInfo[index].lLoginID;
            clearAcs.m_iDevIndex = index;
            clearAcs.ShowDialog();
            clearAcs.Dispose();
            return;
        }

        private void buttonCardWeekPlanManagement_Click(object sender, EventArgs e)
        {
            int index = g_deviceTree.GetCurDeviceIndex();
            if (index < 0) return;

            WeekPlan cardWeekPlan = new WeekPlan();
            cardWeekPlan.m_iDeviceType = 2;
            cardWeekPlan.m_iDeviceIndex = index;
            cardWeekPlan.m_lUserID = g_deviceTree.g_struDeviceInfo[index].lLoginID;

            cardWeekPlan.CustomRelayout();
            cardWeekPlan.ShowDialog();
            cardWeekPlan.Dispose();
        }

        private void buttonCardHolidayPlanManagement_Click(object sender, EventArgs e)
        {
            int index = g_deviceTree.GetCurDeviceIndex();
            if (index < 0) return;

            HolidayPlan cardHolidayPlan = new HolidayPlan();
            cardHolidayPlan.m_iDeviceType = 2;
            cardHolidayPlan.m_iDeviceIndex = index;
            cardHolidayPlan.m_lUserID = g_deviceTree.g_struDeviceInfo[index].lLoginID;

            cardHolidayPlan.CustomRelayout();
            cardHolidayPlan.ShowDialog();
            cardHolidayPlan.Dispose();
        }

        private void buttonCardHolidayGroupManagement_Click(object sender, EventArgs e)
        {
            int index = g_deviceTree.GetCurDeviceIndex();
            if (index < 0) return;

            HolidayGroupPlan cardHolidayGroupPlan = new HolidayGroupPlan();
            cardHolidayGroupPlan.m_iDeviceType = 2;
            cardHolidayGroupPlan.m_iDeviceIndex = index;
            cardHolidayGroupPlan.m_lUserID = g_deviceTree.g_struDeviceInfo[index].lLoginID;

            cardHolidayGroupPlan.CustomRelayout();
            cardHolidayGroupPlan.ShowDialog();
            cardHolidayGroupPlan.Dispose();
        }

        private void buttonCardPlanTemplateManagement_Click(object sender, EventArgs e)
        {
            int index = g_deviceTree.GetCurDeviceIndex();
            if (index < 0) return;

            PlanTemplate cardTemplate = new PlanTemplate();
            cardTemplate.m_iDeviceType = 2;
            cardTemplate.m_iDeviceIndex = index;
            cardTemplate.m_lUserID = g_deviceTree.g_struDeviceInfo[index].lLoginID;
            cardTemplate.CustomRelayout();
            cardTemplate.ShowDialog();
            cardTemplate.Dispose();
        }

        private void buttonFingerPrintManagement_Click(object sender, EventArgs e)
        {
            FingerPrintMgr fingerPrintManage = new FingerPrintMgr();
            int index = g_deviceTree.GetCurDeviceIndex();
            if (index < 0) return;
            fingerPrintManage.m_lUserID = g_deviceTree.g_struDeviceInfo[index].lLoginID;
            fingerPrintManage.ShowDialog();
            fingerPrintManage.Dispose();
            return;
        }

        private void buttonCardNumAndUserLinkage_Click(object sender, EventArgs e)
        {
            UserCardMgr userCardManage = new UserCardMgr();
            int index = g_deviceTree.GetCurDeviceIndex();
            if (index < 0) return;
            userCardManage.m_lUserID = g_deviceTree.g_struDeviceInfo[index].lLoginID;
            userCardManage.ShowDialog();
            userCardManage.Dispose();
            return;
        }

        private void buttonFaceManagement_Click(object sender, EventArgs e)
        {
            FaceConfigure faceConfigure = new FaceConfigure();
            int index = g_deviceTree.GetCurDeviceIndex();
            if (index < 0) return;
            faceConfigure.m_lUserID = g_deviceTree.g_struDeviceInfo[index].lLoginID;
            faceConfigure.ShowDialog();
            faceConfigure.Dispose();
            return;
        }

        private void buttonEventAndCardLinkageManagement_Click(object sender, EventArgs e)
        {
            EventCardLinkage eventCardMgr = new EventCardLinkage();
            int index = g_deviceTree.GetCurDeviceIndex();
            if (index < 0) return;
            eventCardMgr.m_lUserID = g_deviceTree.g_struDeviceInfo[index].lLoginID;
            eventCardMgr.m_iDeviceIndex = index;
            eventCardMgr.ShowDialog();
            eventCardMgr.Dispose();
            return;
        }

        private void buttonGetAcsHostEvent_Click(object sender, EventArgs e)
        {
            GetAcsEvent AcsEvent = new GetAcsEvent();
            int index = g_deviceTree.GetCurDeviceIndex();
            if (index < 0) return;
            AcsEvent.m_lUserID = g_deviceTree.g_struDeviceInfo[index].lLoginID;
            AcsEvent.DeviceIP = g_deviceTree.g_struDeviceInfo[index].chDeviceIP;
            AcsEvent.ShowDialog();
            AcsEvent.Dispose();
            return;
        }

        private void buttonCardReaderCfgSave_Click(object sender, EventArgs e)
        {
            int index = g_deviceTree.GetCurDeviceIndex();
            if (index < 0) return;
            int lUserId = g_deviceTree.g_struDeviceInfo[index].lLoginID;
            if (lUserId < 0)
            {
                MessageBox.Show("Please Login!!!");
                return;
            }

            CHCNetSDK.NET_DVR_CARD_READER_CFG_V50 struReaderCfg = m_struCardReaderCfg;
            struReaderCfg.dwSize = (uint)Marshal.SizeOf(struReaderCfg);

            //GetCurCardReaderIndex()begin with 0,here plus offset 1.
            int cardReaderIndex = g_deviceTree.GetCurCardReaderIndex() + 1;
            if (cardReaderIndex < 0)
            {
                MessageBox.Show("Please select a card reader");
                return;
            }

            if (checkBoxEnable.Checked)
            {
                struReaderCfg.byEnable = 1;
            }
            else
            {
                struReaderCfg.byEnable = 0;
            }
            if (checkBoxEnableFailedAlarm.Checked)
            {
                struReaderCfg.byEnableFailAlarm = 1;
            }
            else
            {
                struReaderCfg.byEnableFailAlarm = 0;
            }
            if (checkBoxEnableTamperDetec.Checked)
            {
                struReaderCfg.byEnableTamperCheck = 1;
            }
            else
            {
                struReaderCfg.byEnableTamperCheck = 0;
            }
            struReaderCfg.byCardReaderType = (byte)comboBoxCardReader.SelectedIndex;
            struReaderCfg.byOkLedPolarity = (byte)comboBoxOkLedPolarity.SelectedIndex;
            struReaderCfg.byErrorLedPolarity = (byte)comboBoxErrorLedPolarity.SelectedIndex;
            struReaderCfg.byBuzzerPolarity = (byte)comboBoxBuzzerPolarity.SelectedIndex;

            if (null == textBoxSwipeInterval.Text || null == textBoxPressTimeout.Text || null == textBoxMaxReadCardFailNum.Text
                || null == textBoxOfflineCheckTime.Text)
            {
                MessageBox.Show("Please input all the message!");
                return;
            }

            byte.TryParse(textBoxSwipeInterval.Text, out struReaderCfg.bySwipeInterval);
            byte.TryParse(textBoxPressTimeout.Text, out struReaderCfg.byPressTimeout);
            byte.TryParse(textBoxMaxReadCardFailNum.Text, out struReaderCfg.byMaxReadCardFailNum);
            byte.TryParse(textBoxOfflineCheckTime.Text, out struReaderCfg.byOfflineCheckTime);

            //fingerprint params
            struReaderCfg.byFingerPrintCheckLevel = (byte)comboBoxFingerprintLevel.SelectedIndex;
            struReaderCfg.byFingerPrintImageQuality = (byte)comboBoxImageQuality.SelectedIndex;
            struReaderCfg.byFingerPrintContrastTimeOut = (byte)comboBoxCompareTimeout.SelectedIndex;
            struReaderCfg.byFingerPrintRecogizeInterval = (byte)comboBoxRecongizeInterval.SelectedIndex;
            struReaderCfg.byFingerPrintMatchFastMode = (byte)comboBoxFastMode.SelectedIndex;
            struReaderCfg.byFingerPrintModuleSensitive = (byte)comboBoxSensitive.SelectedIndex;
            struReaderCfg.byFingerPrintModuleLightCondition = (byte)comboBoxLightConditions.SelectedIndex;

            //face recognize params
            struReaderCfg.byFaceRecogizeTimeOut = (byte)comboBoxFaceCompareTimeout.SelectedIndex;
            if (comboBoxFaceRecongizeInterval.SelectedIndex != 11)
            {
                struReaderCfg.byFaceRecogizeInterval = (byte)comboBoxFaceRecongizeInterval.SelectedIndex;
            }
            else
            {
                struReaderCfg.byFaceRecogizeInterval = 255;
            }
            byte.TryParse(textBoxOne2NThreshold.Text, out struReaderCfg.byFaceMatchThresholdN);
            byte.TryParse(textBoxFaceQuality.Text, out struReaderCfg.byFaceQuality);
            ushort.TryParse(textBoxImageExposure.Text, out struReaderCfg.wFaceImageSensitometry);
            byte.TryParse(textBoxOne2OneThreshold.Text, out struReaderCfg.byFaceMatchThreshold1);

            struReaderCfg.byLivingBodyDetect = (byte)comboBoxLivingDetection.SelectedIndex;
            ushort.TryParse(textBoxBuzzerTime.Text, out struReaderCfg.wBuzzerTime);

            // set card reader configuration
            uint dwSize = (uint)Marshal.SizeOf(struReaderCfg);
            IntPtr ptrReaderCfg = Marshal.AllocHGlobal((int)dwSize);
            Marshal.StructureToPtr(struReaderCfg, ptrReaderCfg, false);
            if (!CHCNetSDK.NET_DVR_SetDVRConfig(lUserId, CHCNetSDK.NET_DVR_SET_CARD_READER_CFG_V50, cardReaderIndex, ptrReaderCfg, dwSize))
            {
                Marshal.FreeHGlobal(ptrReaderCfg);
                g_formList.AddLog(lUserId, AcsDemoPublic.OPERATION_FAIL_T, "NET_DVR_GET_CARD_READER_CFG_V50");
                string strTemp = null;
                strTemp = string.Format("NET_DVR_SET_CARD_READER_CFG_V50 Fail, ERROR STATUS[{0}]", CHCNetSDK.NET_DVR_GetLastError());
                MessageBox.Show(strTemp);
                return;
            }
            else
            {
                g_formList.AddLog(lUserId, AcsDemoPublic.OPERATION_SUCC_T, "NET_DVR_GET_CARD_READER_CFG_V50");
            }

            Marshal.FreeHGlobal(ptrReaderCfg);
        }

        private void ShowCardReaderConfiguration()
        {
            int index = g_deviceTree.GetCurDeviceIndex();
            if (index < 0) return;
            int lUserId = g_deviceTree.g_struDeviceInfo[index].lLoginID;
            if (lUserId < 0)
            {
                MessageBox.Show("Please Login!!!");
                return;
            }

            //GetCurCardReaderIndex()begin with 0,here plus offset 1.
            int cardReaderIndex = g_deviceTree.GetCurCardReaderIndex() + 1;
            if (cardReaderIndex < 0)
            {
                MessageBox.Show("Please select a card reader");
                return;
            }

            CHCNetSDK.NET_DVR_CARD_READER_CFG_V50 struReaderCfg = m_struCardReaderCfg;
            GetCardReaderConfiguration(lUserId, cardReaderIndex, ref struReaderCfg);

            if (1 == struReaderCfg.byEnable)
            {
                checkBoxEnable.Checked = true;
            }
            else
            {
                checkBoxEnable.Checked = false;
            }
            if (1 == struReaderCfg.byEnableFailAlarm)
            {
                checkBoxEnableFailedAlarm.Checked = true;
            }
            else
            {
                checkBoxEnableFailedAlarm.Checked = false;
            }
            if (1 == struReaderCfg.byEnableTamperCheck)
            {
                checkBoxEnableTamperDetec.Checked = true;
            }
            else
            {
                checkBoxEnableTamperDetec.Checked = false;
            }
            comboBoxCardReader.SelectedIndex = (int)struReaderCfg.byCardReaderType;
            comboBoxOkLedPolarity.SelectedIndex = (int)struReaderCfg.byOkLedPolarity;
            comboBoxErrorLedPolarity.SelectedIndex = (int)struReaderCfg.byErrorLedPolarity;
            comboBoxBuzzerPolarity.SelectedIndex = (int)struReaderCfg.byBuzzerPolarity;
            textBoxSwipeInterval.Text = struReaderCfg.bySwipeInterval.ToString();
            textBoxPressTimeout.Text = struReaderCfg.byPressTimeout.ToString();
            textBoxMaxReadCardFailNum.Text = struReaderCfg.byMaxReadCardFailNum.ToString();
            textBoxOfflineCheckTime.Text = struReaderCfg.byOfflineCheckTime.ToString();

            //fingerprint params
            comboBoxFingerprintLevel.SelectedIndex = (int)struReaderCfg.byFingerPrintCheckLevel;
            comboBoxImageQuality.SelectedIndex = (int)struReaderCfg.byFingerPrintImageQuality;
            comboBoxCompareTimeout.SelectedIndex = (int)struReaderCfg.byFingerPrintContrastTimeOut;
            comboBoxRecongizeInterval.SelectedIndex = (int)struReaderCfg.byFingerPrintRecogizeInterval;
            comboBoxFastMode.SelectedIndex = (int)struReaderCfg.byFingerPrintMatchFastMode;
            comboBoxSensitive.SelectedIndex = (int)struReaderCfg.byFingerPrintModuleSensitive;
            comboBoxLightConditions.SelectedIndex = (int)struReaderCfg.byFingerPrintModuleLightCondition;

            //face recognize params
            comboBoxFaceCompareTimeout.SelectedIndex = (int)struReaderCfg.byFaceRecogizeTimeOut;
            if (struReaderCfg.byFaceRecogizeInterval != 255)
            {
                comboBoxFaceRecongizeInterval.SelectedIndex = (int)struReaderCfg.byFaceRecogizeInterval;
            }
            else
            {
                comboBoxFaceRecongizeInterval.SelectedIndex = 11;
            }
            textBoxOne2NThreshold.Text = struReaderCfg.byFaceMatchThresholdN.ToString();
            textBoxFaceQuality.Text = struReaderCfg.byFaceQuality.ToString();
            textBoxImageExposure.Text = struReaderCfg.wFaceImageSensitometry.ToString();
            textBoxOne2OneThreshold.Text = struReaderCfg.byFaceMatchThreshold1.ToString();

            comboBoxLivingDetection.SelectedIndex = (int)struReaderCfg.byLivingBodyDetect;
            textBoxBuzzerTime.Text = struReaderCfg.wBuzzerTime.ToString();

            if (struReaderCfg.byUseLocalController == 1)
            {
                checkBoxLocalCon.Checked = true;
                textBoxLocalControllerNo.Text = struReaderCfg.wLocalControllerID.ToString();
                textBoxLocalControllerReaderID.Text = struReaderCfg.wLocalControllerReaderID.ToString();
                comboBoxCardReaderChannel.SelectedIndex = (int)struReaderCfg.wCardReaderChannel;
            }
            textBoxDiscription.Text = System.Text.Encoding.UTF8.GetString(struReaderCfg.byCardReaderDescription);
            comboBoxCardeReaderType.SelectedIndex = (int)struReaderCfg.byCardReaderType;


            if (struReaderCfg.byCardReaderType != 0)
            {

            }

        }

        private void GetCardReaderConfiguration(int userID, int cardReaderNo, ref CHCNetSDK.NET_DVR_CARD_READER_CFG_V50 struCardReaderCfg)
        {
            uint dwRet = 0; ;
            uint dwSize = (uint)Marshal.SizeOf(struCardReaderCfg);
            IntPtr ptrReaderCfg = Marshal.AllocHGlobal((int)dwSize);
            Marshal.StructureToPtr(struCardReaderCfg, ptrReaderCfg, false);

            if (!CHCNetSDK.NET_DVR_GetDVRConfig(userID, CHCNetSDK.NET_DVR_GET_CARD_READER_CFG_V50, cardReaderNo, ptrReaderCfg, dwSize, ref dwRet))
            {
                Marshal.FreeHGlobal(ptrReaderCfg);
                g_formList.AddLog(userID, AcsDemoPublic.OPERATION_FAIL_T, "NET_DVR_GET_CARD_READER_CFG_V50");
                string strTemp = null;
                strTemp = string.Format("NET_DVR_GET_CARD_READER_CFG Fail, ERROR STATUS[{0}]", CHCNetSDK.NET_DVR_GetLastError());
                MessageBox.Show(strTemp);
                return;
            }
            else
            {
                g_formList.AddLog(userID, AcsDemoPublic.OPERATION_SUCC_T, "NET_DVR_GET_CARD_READER_CFG_V50");
            }
            struCardReaderCfg = (CHCNetSDK.NET_DVR_CARD_READER_CFG_V50)Marshal.PtrToStructure(ptrReaderCfg, typeof(CHCNetSDK.NET_DVR_CARD_READER_CFG_V50));
            Marshal.FreeHGlobal(ptrReaderCfg);
        }

        private void buttonOperate_Click(object sender, EventArgs e)
        {
            int index = g_deviceTree.GetCurDeviceIndex();
            if (index < 0) return;
            int lUserId = g_deviceTree.g_struDeviceInfo[index].lLoginID;
            if (lUserId < 0)
            {
                MessageBox.Show("Please Login!!!");
                return;
            }

            string inUrl = textBoxUrl.Text.ToString();

            Encoding utf8 = System.Text.Encoding.GetEncoding("utf-8");//转换编码
            string inPutBuffer = utf8.GetString(utf8.GetBytes(richTextBoxInputXml.Text));
            int inputlen = utf8.GetByteCount(inPutBuffer);

            IntPtr ptrInUrl;
            IntPtr m_pOutBuf;
            IntPtr m_lpStatusBuffer;
            IntPtr ptrInBuffer;

            ptrInUrl = Marshal.StringToCoTaskMemAnsi(inUrl);
            m_pOutBuf = Marshal.AllocHGlobal((Int32)XML_ABILITY_OUT_LEN);
            m_lpStatusBuffer = Marshal.AllocHGlobal((Int32)XML_ABILITY_OUT_LEN);
            ptrInBuffer = Marshal.AllocHGlobal(inputlen);

            CHCNetSDK.NET_DVR_XML_CONFIG_INPUT struInput = new CHCNetSDK.NET_DVR_XML_CONFIG_INPUT();
            CHCNetSDK.NET_DVR_XML_CONFIG_OUTPUT struOuput = new CHCNetSDK.NET_DVR_XML_CONFIG_OUTPUT();

            struInput.dwSize = (uint)Marshal.SizeOf(struInput);
            struInput.lpRequestUrl = ptrInUrl;
            struInput.dwRequestUrlLen = (uint)inUrl.Length;

            Marshal.Copy(utf8.GetBytes(inPutBuffer), 0, ptrInBuffer, inputlen);
            struInput.lpInBuffer = ptrInBuffer;
            struInput.dwInBufferSize = (uint)inputlen;

            struOuput.dwSize = (uint)Marshal.SizeOf(struOuput);
            struOuput.lpOutBuffer = m_pOutBuf;
            struOuput.dwOutBufferSize = XML_ABILITY_OUT_LEN;

            struOuput.lpStatusBuffer = m_lpStatusBuffer;
            struOuput.dwStatusSize = XML_ABILITY_OUT_LEN;

            IntPtr ptrInput = Marshal.AllocHGlobal(Marshal.SizeOf(struInput));
            Marshal.StructureToPtr(struInput, ptrInput, false);
            IntPtr ptrOuput = Marshal.AllocHGlobal(Marshal.SizeOf(struOuput));
            Marshal.StructureToPtr(struOuput, ptrOuput, false);

            if (!CHCNetSDK.NET_DVR_STDXMLConfig(lUserId, ptrInput, ptrOuput))
            {
                g_formList.AddLog(lUserId, AcsDemoPublic.OPERATION_FAIL_T, "NET_DVR_STDXMLConfig");
                MessageBox.Show("Std XML config error code :" + CHCNetSDK.NET_DVR_GetLastError().ToString());
            }
            else
            {
                g_formList.AddLog(lUserId, AcsDemoPublic.OPERATION_SUCC_T, "NET_DVR_STDXMLConfig");
            }

            if (inUrl.IndexOf("GET") != -1)
            {
                byte[] bResponse = new byte[XML_ABILITY_OUT_LEN];
                Marshal.Copy(m_pOutBuf, bResponse, 0, bResponse.Length);
                string pOutBuf = System.Text.Encoding.UTF8.GetString(bResponse);
                richTextBoxOutputXml.Text = pOutBuf;
            }
            else if (inUrl.IndexOf("PUT") != -1)
            {
                byte[] bResponse = new byte[XML_ABILITY_OUT_LEN];
                Marshal.Copy(m_lpStatusBuffer, bResponse, 0, bResponse.Length);
                string pOutBuf = System.Text.Encoding.UTF8.GetString(bResponse);
                richTextBoxOutputXml.Text = pOutBuf;
            }
            if (inUrl.IndexOf("POST") != -1)
            {
                byte[] bResponse = new byte[XML_ABILITY_OUT_LEN];
                Marshal.Copy(m_pOutBuf, bResponse, 0, bResponse.Length);
                string pOutBuf = System.Text.Encoding.UTF8.GetString(bResponse);
                richTextBoxOutputXml.Text = pOutBuf;
            }
            if (inUrl.IndexOf("DELETE") != -1)
            {
                byte[] bResponse = new byte[XML_ABILITY_OUT_LEN];
                Marshal.Copy(m_pOutBuf, bResponse, 0, bResponse.Length);
                string pOutBuf = System.Text.Encoding.UTF8.GetString(bResponse);
                richTextBoxOutputXml.Text = pOutBuf;
            }

            Marshal.FreeHGlobal(ptrInput);
            Marshal.FreeHGlobal(ptrOuput);
            Marshal.FreeHGlobal(m_pOutBuf);
            Marshal.FreeHGlobal(m_lpStatusBuffer);
            Marshal.FreeHGlobal(ptrInBuffer);
            Marshal.FreeCoTaskMem(ptrInUrl);

            return;
        }


        private void buttonVideoCall_Click(object sender, EventArgs e)
        {
            int index = g_deviceTree.GetCurDeviceIndex();
            if (index < 0) return;

            VideoCall videoCall = new VideoCall(g_deviceTree.g_struDeviceInfo[index].lLoginID, index);
            videoCall.ShowDialog();
        }

        private void buttonTimeCfg_Click(object sender, EventArgs e)
        {
            int index = g_deviceTree.GetCurDeviceIndex();
            if (index < 0) return;
            int userID = g_deviceTree.g_struDeviceInfo[index].lLoginID;
            CHCNetSDK.NET_DVR_TIME struTimeCfg = new CHCNetSDK.NET_DVR_TIME();

            uint dwReturned = 0;
            uint dwSize = (uint)Marshal.SizeOf(struTimeCfg);
            //struTimeCfg.dwSize = dwSize;
            IntPtr ptrTimeCfg = Marshal.AllocHGlobal((Int32)dwSize);
            Marshal.StructureToPtr(struTimeCfg, ptrTimeCfg, false);

            if (!CHCNetSDK.NET_DVR_GetDVRConfig(userID, CHCNetSDK.NET_DVR_GET_TIMECFG, 1, ptrTimeCfg, dwSize, ref dwReturned))
            {
                g_formList.AddLog(userID, AcsDemoPublic.OPERATION_FAIL_T, "NET_DVR_GET_TIMECFG");
                MessageBox.Show("Get device time error code :" + CHCNetSDK.NET_DVR_GetLastError().ToString());
                Marshal.FreeHGlobal(ptrTimeCfg);
                ptrTimeCfg = IntPtr.Zero;
                return;
            }
            else
            {
                g_formList.AddLog(userID, AcsDemoPublic.OPERATION_SUCC_T, "NET_DVR_GET_TIMECFG");
                struTimeCfg = (CHCNetSDK.NET_DVR_TIME)Marshal.PtrToStructure(ptrTimeCfg, typeof(CHCNetSDK.NET_DVR_TIME));
            }

            if (!CHCNetSDK.NET_DVR_SetDVRConfig(userID, CHCNetSDK.NET_DVR_SET_TIMECFG, 1, ptrTimeCfg, dwSize))
            {
                g_formList.AddLog(userID, AcsDemoPublic.OPERATION_FAIL_T, "NET_DVR_SET_TIMECFG");
                MessageBox.Show("Set device time error code :" + CHCNetSDK.NET_DVR_GetLastError().ToString());
                Marshal.FreeHGlobal(ptrTimeCfg);
                ptrTimeCfg = IntPtr.Zero;
                return;
            }
            else
            {
                g_formList.AddLog(userID, AcsDemoPublic.OPERATION_SUCC_T, "NET_DVR_SET_TIMECFG");
                Marshal.FreeHGlobal(ptrTimeCfg);
                ptrTimeCfg = IntPtr.Zero;
            }
        }

        private void buttonAudioInVolume_Click(object sender, EventArgs e)
        {
            int index = g_deviceTree.GetCurDeviceIndex();
            if (index < 0) return;
            int userID = g_deviceTree.g_struDeviceInfo[index].lLoginID;
            CHCNetSDK.NET_DVR_VOLUME_CFG struVolumeCfg = new CHCNetSDK.NET_DVR_VOLUME_CFG();

            uint dwReturned = 0;
            uint dwSize = (uint)Marshal.SizeOf(struVolumeCfg);
            struVolumeCfg.dwSize = dwSize;
            IntPtr ptrVolumeCfg = Marshal.AllocHGlobal((Int32)dwSize);
            Marshal.StructureToPtr(struVolumeCfg, ptrVolumeCfg, false);

            if (!CHCNetSDK.NET_DVR_GetDVRConfig(userID, CHCNetSDK.NET_DVR_GET_AUDIOIN_VOLUME_CFG, 1, ptrVolumeCfg, dwSize, ref dwReturned))
            {
                g_formList.AddLog(userID, AcsDemoPublic.OPERATION_FAIL_T, "NET_DVR_GET_AUDIOIN_VOLUME_CFG");
                MessageBox.Show("Get audio in volume error code :" + CHCNetSDK.NET_DVR_GetLastError().ToString());
                Marshal.FreeHGlobal(ptrVolumeCfg);
                ptrVolumeCfg = IntPtr.Zero;
                return;
            }
            else
            {
                g_formList.AddLog(userID, AcsDemoPublic.OPERATION_SUCC_T, "NET_DVR_GET_AUDIOIN_VOLUME_CFG");
                struVolumeCfg = (CHCNetSDK.NET_DVR_VOLUME_CFG)Marshal.PtrToStructure(ptrVolumeCfg, typeof(CHCNetSDK.NET_DVR_VOLUME_CFG));
            }

            if (!CHCNetSDK.NET_DVR_SetDVRConfig(userID, CHCNetSDK.NET_DVR_SET_AUDIOIN_VOLUME_CFG, 1, ptrVolumeCfg, dwSize))
            {
                g_formList.AddLog(userID, AcsDemoPublic.OPERATION_FAIL_T, "NET_DVR_SET_AUDIOIN_VOLUME_CFG");
                MessageBox.Show("Set audio in volume error code :" + CHCNetSDK.NET_DVR_GetLastError().ToString());
                Marshal.FreeHGlobal(ptrVolumeCfg);
                ptrVolumeCfg = IntPtr.Zero;
                return;
            }
            else
            {
                g_formList.AddLog(userID, AcsDemoPublic.OPERATION_SUCC_T, "NET_DVR_SET_AUDIOIN_VOLUME_CFG");
                Marshal.FreeHGlobal(ptrVolumeCfg);
                ptrVolumeCfg = IntPtr.Zero;
            }
        }

        private void buttonAudioOutVolume_Click(object sender, EventArgs e)
        {
            int index = g_deviceTree.GetCurDeviceIndex();
            if (index < 0) return;
            int userID = g_deviceTree.g_struDeviceInfo[index].lLoginID;
            CHCNetSDK.NET_DVR_VOLUME_CFG struVolumeCfg = new CHCNetSDK.NET_DVR_VOLUME_CFG();

            uint dwReturned = 0;
            uint dwSize = (uint)Marshal.SizeOf(struVolumeCfg);
            struVolumeCfg.dwSize = dwSize;
            IntPtr ptrVolumeCfg = Marshal.AllocHGlobal((Int32)dwSize);
            Marshal.StructureToPtr(struVolumeCfg, ptrVolumeCfg, false);

            if (!CHCNetSDK.NET_DVR_GetDVRConfig(userID, CHCNetSDK.NET_DVR_GET_AUDIOOUT_VOLUME_CFG, 1, ptrVolumeCfg, dwSize, ref dwReturned))
            {
                g_formList.AddLog(userID, AcsDemoPublic.OPERATION_FAIL_T, "NET_DVR_GET_AUDIOOUT_VOLUME_CFG");
                MessageBox.Show("Get audio out volume error code :" + CHCNetSDK.NET_DVR_GetLastError().ToString());
                Marshal.FreeHGlobal(ptrVolumeCfg);
                ptrVolumeCfg = IntPtr.Zero;
                return;
            }
            else
            {
                g_formList.AddLog(userID, AcsDemoPublic.OPERATION_SUCC_T, "NET_DVR_GET_AUDIOOUT_VOLUME_CFG");
                struVolumeCfg = (CHCNetSDK.NET_DVR_VOLUME_CFG)Marshal.PtrToStructure(ptrVolumeCfg, typeof(CHCNetSDK.NET_DVR_VOLUME_CFG));
            }

            if (!CHCNetSDK.NET_DVR_SetDVRConfig(userID, CHCNetSDK.NET_DVR_SET_AUDIOOUT_VOLUME_CFG, 1, ptrVolumeCfg, dwSize))
            {
                g_formList.AddLog(userID, AcsDemoPublic.OPERATION_FAIL_T, "NET_DVR_SET_AUDIOOUT_VOLUME_CFG");
                MessageBox.Show("Set audio out volume error code :" + CHCNetSDK.NET_DVR_GetLastError().ToString());
                Marshal.FreeHGlobal(ptrVolumeCfg);
                ptrVolumeCfg = IntPtr.Zero;
                return;
            }
            else
            {
                g_formList.AddLog(userID, AcsDemoPublic.OPERATION_SUCC_T, "NET_DVR_SET_AUDIOOUT_VOLUME_CFG");
                Marshal.FreeHGlobal(ptrVolumeCfg);
                ptrVolumeCfg = IntPtr.Zero;
            }
        }

        private void buttonUpgrade_Click(object sender, EventArgs e)
        {
            int index = g_deviceTree.GetCurDeviceIndex();
            if (index < 0) return;

            Upgrade upgrade = new Upgrade();
            upgrade.m_lUserID = g_deviceTree.g_struDeviceInfo[index].lLoginID;
            upgrade.ShowDialog();
        }

        private void ProcessGetGatewayCardCallback1(uint dwType, IntPtr lpBuffer, uint dwBufLen, IntPtr pUserData)
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
                    g_formList.AddLog(-1, AcsDemoPublic.OPERATION_SUCC_T, "GetAcsEvent Processing");
                }
                else if (dwStatus == (uint)CHCNetSDK.NET_SDK_CALLBACK_STATUS_NORMAL.NET_SDK_CALLBACK_STATUS_FAILED)
                {
                    g_formList.AddLog(-1, AcsDemoPublic.OPERATION_FAIL_T, "GetAcsEvent Failed");
                }
                else if (dwStatus == (uint)CHCNetSDK.NET_SDK_CALLBACK_STATUS_NORMAL.NET_SDK_CALLBACK_STATUS_SUCCESS)
                {
                    int iToital = iTotalAcsEvent;
                    g_formList.AddLog(-1, AcsDemoPublic.OPERATION_SUCC_T, "GetAcsEvent Success");
                }
                else if (dwStatus == (uint)CHCNetSDK.NET_SDK_CALLBACK_STATUS_NORMAL.NET_SDK_CALLBACK_STATUS_EXCEPTION)
                {
                    g_formList.AddLog(-1, AcsDemoPublic.OPERATION_FAIL_T, "GetAcsEvent Exception");
                }
                else
                {
                    g_formList.AddLog(-1, AcsDemoPublic.OPERATION_FAIL_T, "Unknown Status");
                }
            }
            else if (dwType == (uint)CHCNetSDK.NET_SDK_CALLBACK_TYPE.NET_SDK_CALLBACK_TYPE_DATA)
            {
                var result = Marshal.PtrToStructure(lpBuffer, typeof(CHCNetSDK.NET_DVR_ACS_EVENT_CFG));
                var struAcsEventCfg = (CHCNetSDK.NET_DVR_ACS_EVENT_CFG)result;
                iTotalAcsEvent += 1;
            }
            return;
        }

        private void buttonGetEvent_Click(object sender, EventArgs e)
        {
            iTotalAcsEvent = 0;
            if (lRemoteHandle != -1)
            {
                if (CHCNetSDK.NET_DVR_StopRemoteConfig(lRemoteHandle))
                {
                    lRemoteHandle = -1;
                }
            }
            CHCNetSDK.NET_DVR_ACS_EVENT_COND struCond1 = new CHCNetSDK.NET_DVR_ACS_EVENT_COND();
            struCond1.dwSize = (uint)Marshal.SizeOf(struCond1);
            struCond1.struStartTime.dwYear = 2017;
            struCond1.struStartTime.dwMonth = 12;
            struCond1.struStartTime.dwDay = 4;
            struCond1.struEndTime.dwYear = 2017;
            struCond1.struEndTime.dwMonth = 12;
            struCond1.struEndTime.dwDay = 6;

            int dwSize = Marshal.SizeOf(struCond1);
            IntPtr ptrStruCond = Marshal.AllocHGlobal(dwSize);
            Marshal.StructureToPtr(struCond1, ptrStruCond, false);
            g_fGetLogsCallback = new CHCNetSDK.RemoteConfigCallback(ProcessGetGatewayCardCallback1);

            int index = g_deviceTree.GetCurDeviceIndex();
            int lUserID = g_deviceTree.g_struDeviceInfo[index].lLoginID;
            lRemoteHandle = CHCNetSDK.NET_DVR_StartRemoteConfig(lUserID, CHCNetSDK.NET_DVR_GET_ACS_EVENT, ptrStruCond, dwSize, g_fGetLogsCallback, this.Handle);
            if (-1 == lRemoteHandle)
            {
                uint iErr = CHCNetSDK.NET_DVR_GetLastError();
            }


        }

        private void buttonStaffManagementStaffCenter_Click(object sender, EventArgs e)
        {
            int index = g_deviceTree.GetCurDeviceIndex();
            if (index < 0) return;
            StaffManagement struStaffManagement = new StaffManagement();
            struStaffManagement.m_lUserID = g_deviceTree.g_struDeviceInfo[index].lLoginID;
            struStaffManagement.ShowDialog();
            struStaffManagement.Dispose();
        }

        private void buttonCardManagementStaffCenter_Click(object sender, EventArgs e)
        {
            int index = g_deviceTree.GetCurDeviceIndex();
            if (index < 0) return;
            CardManagementByStaffCenter struCardManagementByStaffCenter = new CardManagementByStaffCenter();
            struCardManagementByStaffCenter.m_lUserID = g_deviceTree.g_struDeviceInfo[index].lLoginID;
            struCardManagementByStaffCenter.ShowDialog();
            struCardManagementByStaffCenter.Dispose();
        }

        private void buttonFaceRecordManagement_Click(object sender, EventArgs e)
        {
            int index = g_deviceTree.GetCurDeviceIndex();
            if (index < 0) return;
            FaceRecordManagement struFaceRecordManagement = new FaceRecordManagement();
            struFaceRecordManagement.m_lUserID = g_deviceTree.g_struDeviceInfo[index].lLoginID;
            struFaceRecordManagement.ShowDialog();
            struFaceRecordManagement.Dispose();
        }

        private void btnCapture_Click(object sender, EventArgs e)
        {
            int index = g_deviceTree.GetCurDeviceIndex();
            if (index < 0) return;
            RemoteControlCapture struRemoteControlCapture = new RemoteControlCapture();
            struRemoteControlCapture.index = index;
            struRemoteControlCapture.m_lServerID = g_deviceTree.g_struDeviceInfo[index].lLoginID;
            struRemoteControlCapture.m_dwGatawayNum = 128;
            struRemoteControlCapture.ShowDialog();
            struRemoteControlCapture.Dispose();
        }

        private void btnHumanFaceManage_Click(object sender, EventArgs e)
        {
            int index = g_deviceTree.GetCurDeviceIndex();
            if (index < 0) return;
            ACS_FACE_PARAM_CFG struAcs_Face_Param = new ACS_FACE_PARAM_CFG();
            struAcs_Face_Param.m_lServerID = g_deviceTree.g_struDeviceInfo[index].lLoginID;
            struAcs_Face_Param.m_iDevIndex = index;
            struAcs_Face_Param.ShowDialog();
            struAcs_Face_Param.Dispose();
        }

        private void menuStrip1_ItemClicked(object sender, ToolStripItemClickedEventArgs e)
        {

        }

        private void btnTimeCorrection_Click(object sender, EventArgs e)
        {
            int index = g_deviceTree.GetCurDeviceIndex();
            if (index < 0) return;
            DeviceTimeCorrection DeviceTimeCfg = new DeviceTimeCorrection();
            DeviceTimeCfg.iDevIndex = index;
            DeviceTimeCfg.m_loginID = g_deviceTree.g_struDeviceInfo[index].lLoginID;
            DeviceTimeCfg.ShowDialog();
            DeviceTimeCfg.Dispose();
        }

    }
}
