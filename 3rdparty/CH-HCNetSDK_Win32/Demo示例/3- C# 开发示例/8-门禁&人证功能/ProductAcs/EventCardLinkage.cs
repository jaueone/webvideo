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
using AcsDemo.Public;
using AcsDemo.Language;

namespace AcsDemo.ProductAcs
{
    public partial class EventCardLinkage : Form
    {
        private DeviceLogList g_formList = DeviceLogList.Instance();
        public Int32 m_lUserID = -1;
        public Int32 m_iDeviceIndex = -1;
        private CHCNetSDK.NET_DVR_EVENT_CARD_LINKAGE_CFG_V51 m_struEventCardLinkageCfgV51 = new CHCNetSDK.NET_DVR_EVENT_CARD_LINKAGE_CFG_V51();
        private CHCNetSDK.NET_DVR_EVENT_CARD_LINKAGE_COND m_struEventCardLinkageCond = new CHCNetSDK.NET_DVR_EVENT_CARD_LINKAGE_COND();
        private bool m_byProMode = false;
        private bool m_byMacAddr = false;
        private bool m_byEmployeeNo = false;

        public EventCardLinkage()
        {
            InitializeComponent();
            m_struEventCardLinkageCfgV51.Init();
            m_struEventCardLinkageCond.byRes = new byte[106];
        }

        private void EventCardLinkage_Load(object sender, EventArgs e)
        {
            InitMainEventType();
            SubEventTypeDev();
            CreateChecklistBox();
            MultiLanguage.LoadLanguage(this);
        }

        private void CreateChecklistBox()
        {
            //clear 
            checkedListBoxAlarmOut.Items.Clear();
            checkedListBoxOpenDoor.Items.Clear();
            checkedListBoxAlwaysCloseDoor.Items.Clear();
            checkedListBoxAlarmOutClose.Items.Clear();
            checkedListBoxCloseDoor.Items.Clear();
            checkedListBoxArming.Items.Clear();
            checkedListBoxBuzzerReaderCard.Items.Clear();
            checkedListBoxAlwaysOpenDoor.Items.Clear();
            checkedListBoxDisarming.Items.Clear();

            //create
            checkedListBoxAlarmOut.BeginUpdate();
            for (int i = 0; i < CHCNetSDK.MAX_ALARMHOST_ALARMOUT_NUM; i++)
            {
                checkedListBoxAlarmOut.Items.Add("Alarm Out " + (i + 1));
                if(m_struEventCardLinkageCfgV51.byAlarmout[i] == 1)
                {
                    checkedListBoxAlarmOut.SetItemChecked(i, true);
                }
            }
            checkedListBoxAlarmOut.EndUpdate();

            checkedListBoxOpenDoor.BeginUpdate();
            for (int i = 0; i < CHCNetSDK.MAX_DOOR_NUM_256; i++)
            {
                checkedListBoxOpenDoor.Items.Add("Door " + (i + 1));
                if(m_struEventCardLinkageCfgV51.byOpenDoor[i] == 1)
                {
                    checkedListBoxOpenDoor.SetItemChecked(i, true);
                }
            }
            checkedListBoxOpenDoor.EndUpdate();

            checkedListBoxAlwaysOpenDoor.BeginUpdate();
            for (int i = 0; i < CHCNetSDK.MAX_DOOR_NUM_256; i++)
            {
                checkedListBoxAlwaysOpenDoor.Items.Add("Door " + (i + 1));
                if(m_struEventCardLinkageCfgV51.byNormalOpen[i] == 1)
                {
                    checkedListBoxAlwaysOpenDoor.SetItemChecked(i, true);
                }
            }
            checkedListBoxAlwaysOpenDoor.EndUpdate();

            checkedListBoxCloseDoor.BeginUpdate();
            for (int i = 0; i < CHCNetSDK.MAX_DOOR_NUM_256; i++)
            {
                checkedListBoxCloseDoor.Items.Add("Door " + (i + 1));
                if(m_struEventCardLinkageCfgV51.byCloseDoor[i] == 1)
                {
                    checkedListBoxCloseDoor.SetItemChecked(i, true);
                }
            }
            checkedListBoxCloseDoor.EndUpdate();

            checkedListBoxAlwaysCloseDoor.BeginUpdate();
            for (int i = 0; i < CHCNetSDK.MAX_DOOR_NUM_256; i++)
            {
                checkedListBoxAlwaysCloseDoor.Items.Add("Door " + (i + 1));
                if(m_struEventCardLinkageCfgV51.byNormalClose[i] == 1)
                {
                    checkedListBoxAlwaysCloseDoor.SetItemChecked(i, true);
                }
            }
            checkedListBoxAlwaysCloseDoor.EndUpdate();

            checkedListBoxAlarmOutClose.BeginUpdate();
            for (int i = 0; i < CHCNetSDK.MAX_ALARMHOST_ALARMOUT_NUM; i++)
            {
                checkedListBoxAlarmOutClose.Items.Add("Alarm Out " + (i + 1));
                if(m_struEventCardLinkageCfgV51.byAlarmOutClose[i] == 1)
                {
                    checkedListBoxAlarmOutClose.SetItemChecked(i, true);
                }
            }
            checkedListBoxAlarmOutClose.EndUpdate();

            checkedListBoxArming.BeginUpdate();
            for (int i = 0; i < CHCNetSDK.MAX_ALARMHOST_ALARMIN_NUM; i++)
            {
                checkedListBoxArming.Items.Add("Alarm In " + (i + 1));
                 if(m_struEventCardLinkageCfgV51.byAlarmInSetup[i] == 1)
                {
                    checkedListBoxArming.SetItemChecked(i, true);
                }
            }
            checkedListBoxArming.EndUpdate();

            checkedListBoxDisarming.BeginUpdate();
            for (int i = 0; i < CHCNetSDK.MAX_ALARMHOST_ALARMIN_NUM; i++)
            {
                checkedListBoxDisarming.Items.Add("Alarm In " + (i + 1));
                if(m_struEventCardLinkageCfgV51.byAlarmInClose[i] == 1)
                {
                    checkedListBoxDisarming.SetItemChecked(i, true);
                }
            }
            checkedListBoxDisarming.EndUpdate();

            checkedListBoxBuzzerReaderCard.BeginUpdate();
            for (int i = 0; i < CHCNetSDK.MAX_CARD_READER_NUM_512; i++)
            {
                checkedListBoxBuzzerReaderCard.Items.Add("Buzzer " + (i + 1));
                if(m_struEventCardLinkageCfgV51.byReaderBuzzer[i] == 1)
                {
                    checkedListBoxBuzzerReaderCard.SetItemChecked(i, true);
                }
            }
            checkedListBoxBuzzerReaderCard.EndUpdate();
        }

        private void InitMainEventType()
        {
            comboBoxMajorEventType.SuspendLayout();
            comboBoxMajorEventType.Items.Clear();
            comboBoxMajorEventType.Items.Add("Device Event");
            comboBoxMajorEventType.Items.Add("AlarmIn Event");
            comboBoxMajorEventType.Items.Add("Door Event");
            comboBoxMajorEventType.Items.Add("CardReader Event");
            comboBoxMajorEventType.SelectedIndex = 0;
            comboBoxMajorEventType.ResumeLayout();
        }
        private void SubEventTypeAlarm()
        {
            comboBoxMinorEventType.SuspendLayout();
            comboBoxMinorEventType.Items.Clear();
            comboBoxMinorEventType.Items.AddRange(new object[] {
                "Zone Short-Circuit Alarm",
                "Zone Circuit Alarm",
                "Abnormal Zone Alarm",
                "Zone Alarm Recover",
                "Event Input Alarm",
                "Event Recover Alarm"});
            comboBoxMinorEventType.SelectedIndex = 0;
            comboBoxMinorEventType.ResumeLayout();
        }
        private void SubEventTypeDoor()
        {
            comboBoxMinorEventType.SuspendLayout();
            comboBoxMinorEventType.Items.Clear();
            comboBoxMinorEventType.Items.AddRange(new object[] {
                "Opening door by the first card start",
                "Opening door by the first card end",
                "Enter normally open state",
                "Exit normally open state",
                "Enter normally close state",
                "Exit normally close state",
                "Door open",
                "door closed",
                "Push open button",
                "Release open button",
                "Normal open(door magnetometer)",
                "Normal close(door magnetometer)",
                "Abnormal door opening(door magnetometer)",
                "Door open timeout(door magnetometer)",
                "Remote open door",
                "Remote close door",
                "Remote always open",
                "Remote always close",
                "Card does not belong to multiple authentication group",
                "Card is not in the multiple certification time period",
                "Multiple authentication mode super password authentication failed",
                "Multiple authentication mode remote authentication failed ",
                "Multiple authentication success ",
                "Multiple authentication requires remote door opening",
                "Multiple authentication super password authentication successful event ",
                "Multiple authentication failed",
                "Multiple authentication timeout failed",
                "Remote capture",
                "The Door Bell Rang",
                "Anti theft alarm",
                "Call center event",
                "Start the first card authorization",
                "End of first card authorization",
                "Door lock input short-circuit alarm",
                "Door lock input broken-circuit alarm",
                "Door lock input exception alarm",
                "Door magnetometer input short-circuit alarm",
                "Door magnetometer input broken-circuit alarm",
                "Door magnetometer input exception alarm",
                "Open button input short-circuit alarm",
                "Open button input broken-circuit alarm",
                "Open button input exception alarm",
                "Door lock abnormal open ",
                "Door lock open timeout",
                "The first card is not authorized to open the door failed",
                "Calling relay disconnect ",
                "Relay closing",
                "Automatic key relay disconnect",
                "Automatic key relay",
                "Key ladder control relay off ",
                "Key ladder control relay ",
                "Visitor call",
                "Household call",
                "Legal SMS",
                "Illegal SMS"});
            comboBoxMinorEventType.SelectedIndex = 0;
            comboBoxMinorEventType.ResumeLayout();
        }
        private void SubEventTypeCardReader()
        {
            comboBoxMinorEventType.SuspendLayout();
            comboBoxMinorEventType.Items.Clear();
            comboBoxMinorEventType.Items.AddRange(new object[] {
                "stress alarm",
                "Card reader tamper alarm",
                "Legal card authentication through",
                "Swipe card and password authentication success",
                "Swipe card and password authentication failed",
                "Swipe card and password authentication timeout",
                "Card reader authentication failed times over limits",
                "Card unallocated rights",
                "Card current period invalid",
                "Card expired",
                "Without this card numbers",
                "Anti-submarine authentication failed",
                "Each door is not closed",
                "Fingerprint comparison success",
                "Fingerprint comparison failure",
                "Swipe card and fingerprint authentication through",
                "Swipe card and fingerprint authentication failed",
                "Swipe card and fingerprint authentication timeout",
                "Swipe card, fingerprint and password authentication success",
                "Swipe card, fingerprint and password authentication failed",
                "Swipe card, fingerprint and password authentication timeout",
                "Fingerprint and password authentication success",
                "Fingerprint and password authentication failed",
                "Fingerprint and password authentication timeout",
                "The fingerprint does not exist",
                "Face authentication success",
                "Face authentication failed",
                "Facial and fingerprint authentication success",
                "Facial and fingerprint authentication failed",
                "Facial and fingerprint authentication timeout",
                "Face and password authentication success",
                "Face and password authentication failed",
                "Face and password authentication timeout",
                "Face and swipe card authentication success",
                "Face and swipe card authentication failed",
                "Face and swipe card authentication timeout",
                "Face, password and fingerprint authentication success",
                "Face, password and fingerprint authentication failed",
                "Face, password and fingerprint authentication timeout",
                "Face, swipe card and fingerprint authentication success",
                "Face, swipe card and fingerprint authentication failed",
                "Face, swipe card and fingerprint authentication timeout",
                "Work number and fingerprint authentication success",
                "Work number and fingerprint authentication failed",
                "Work number and fingerprint authentication timeout",
                "Work number, fingerprint and password authentication success",
                "Work number, fingerprint and password authentication failed",
                "Work number, fingerprint and password authentication timeout",
                "Work number and face authentication success",
                "Work number and face authentication failed",
                "Work number and face authentication timeout",
                "Face recognition failure",
                "Work number and password authentication success",
                "Work number and password authentication failed",
                "Work number and password authentication timeout",
                "people detection failure"});
            comboBoxMinorEventType.SelectedIndex = 0;
            comboBoxMinorEventType.ResumeLayout();
        }
        private void SubEventTypeDev()
        {
            comboBoxMinorEventType.SuspendLayout();
            comboBoxMinorEventType.Items.Clear();
            comboBoxMinorEventType.Items.AddRange(new object[] {
            "Host Tamper Alarm",
            "Device Event",
            "Network Disconnect",
            "Network Recovery",
            "Low Battery Voltage",
            "Battery Voltage resume normal",
            "AC Power Off",
            "AC Power On",
            "SD Card Full Alarm",
            "Linkage Capture Event Alarm",
            "Image Quality Low",
            "Finger Print Quality Low",
            "Battery Electric Low",
            "Battery Electric Resume",
            "Fire import short circuit",
            "Fire import broken circuit",
            "Fire import resume",
            "Master RS485 loop node broken",
            "Master RS485 loop node resume",
            "Local control off line",
            "Local control resume",
            "Local downside RS485 loop node broken",
            "Local downside RS485 loop node resume",
            "Distract controller on line",
            "Distract controller off line",
            "Fire button trigger",
            "Fire button resume",
            "Maintenance button trigger",
            "Maintenance button resume",
            "Emergency button trigger",
            "Emergency button resume",
            "MAC detect"});
            comboBoxMinorEventType.SelectedIndex = 0;
            comboBoxMinorEventType.ResumeLayout();
        }

        //get current ChecklistBox selected status from controls set by user
        private void UpdateUserData()
        {
            //card no. is preferred
            if (checkBoxCardNoLinkage.Checked)
            {
                m_struEventCardLinkageCfgV51.byProMode = 1;
            }
            else if (checkBoxMacAddressLinkage.Checked)
            {
                m_struEventCardLinkageCfgV51.byProMode = 2;
            }
            else if(checkBoxEmployeeNoLinkage.Checked)
            {
                m_struEventCardLinkageCfgV51.byProMode = 3;
            }

            if (!checkBoxCardNoLinkage.Checked && !checkBoxMacAddressLinkage.Checked && !checkBoxEmployeeNoLinkage.Checked)
            {
                m_struEventCardLinkageCfgV51.byProMode = 0;
            }
            uint.TryParse(textBoxEventSourceID.Text, out m_struEventCardLinkageCfgV51.dwEventSourceID);
            m_struEventCardLinkageCfgV51.byMainDevBuzzer =(byte)(checkBoxBuzzer.Checked ? 1 : 0);

            if (m_struEventCardLinkageCfgV51.byProMode == 1)
            {
                byte [] byTempCardNo = System.Text.Encoding.UTF8.GetBytes(textBoxCardNo.Text);
                m_struEventCardLinkageCfgV51.uLinkageInfo.byCardNo = new byte[CHCNetSDK.ACS_CARD_NO_LEN];
                for (int i = 0; i < byTempCardNo.Length; i++)
                {
                    if (i > CHCNetSDK.ACS_CARD_NO_LEN)
                    {
                        MessageBox.Show("card number length too long!");
                        return;
                    }
                    m_struEventCardLinkageCfgV51.uLinkageInfo.byCardNo[i] = byTempCardNo[i];
                }
            }
            if (m_struEventCardLinkageCfgV51.byProMode == 2)
            {
                string sTempMACAddr = textBoxMacAddress.Text;
                if (sTempMACAddr.Length != "1A:2B:3C:4D:5E:6F".Length)
                {
                    MessageBox.Show("Mac address error!");
                    return;
                }
                string[] sTempMACAddrSec = new string[6];

                sTempMACAddrSec[0] = sTempMACAddr.Substring(0, 2);
                m_struEventCardLinkageCfgV51.uLinkageInfo.byMACAddr = new byte[CHCNetSDK.MACADDR_LEN];
                byte.TryParse(sTempMACAddrSec[0], out m_struEventCardLinkageCfgV51.uLinkageInfo.byMACAddr[0]);

                for (int i = 1; i < 6; i++)
                {
                    sTempMACAddrSec[i] = sTempMACAddr.Substring(2 * i + 1, 2);
                    byte.TryParse(sTempMACAddrSec[i], out m_struEventCardLinkageCfgV51.uLinkageInfo.byMACAddr[i]);
                }
            }
            if(m_struEventCardLinkageCfgV51.byProMode == 3)
            {
                 byte [] byTempEmployeeNo = System.Text.Encoding.UTF8.GetBytes(textBoxEmployeeNo.Text);
                 m_struEventCardLinkageCfgV51.uLinkageInfo.byEmployeeNo = new byte[CHCNetSDK.NET_SDK_EMPLOYEE_NO_LEN];
                 for (int i = 0; i < byTempEmployeeNo.Length; i++)
                {
                    if (i > CHCNetSDK.NET_SDK_EMPLOYEE_NO_LEN)
                    {
                        MessageBox.Show("employee number length too long!");
                        return;
                    }
                    m_struEventCardLinkageCfgV51.uLinkageInfo.byEmployeeNo[i] = byTempEmployeeNo[i];
                }
            }
            if (m_struEventCardLinkageCfgV51.byProMode == 0)
            {
                m_struEventCardLinkageCfgV51.uLinkageInfo.struEventLinkage.wMainEventType = (ushort)comboBoxMajorEventType.SelectedIndex;
                m_struEventCardLinkageCfgV51.uLinkageInfo.struEventLinkage.wSubEventType = (ushort)comboBoxMinorEventType.SelectedIndex;
            }

            int dwCountAlarmOut = checkedListBoxAlarmOut.Items.Count;
            for (int i = 0; i < dwCountAlarmOut; i++)
            {
                if (checkedListBoxAlarmOut.GetItemChecked(i))
                {
                    m_struEventCardLinkageCfgV51.byAlarmout[i] = 1;
                }
            }

            int dwCountOpenDoor = checkedListBoxOpenDoor.Items.Count;
            for (int i = 0; i < dwCountOpenDoor; i++)
            {
                if (checkedListBoxOpenDoor.GetItemChecked(i))
                {
                    m_struEventCardLinkageCfgV51.byOpenDoor[i] = 1;
                }
            }

            int dwCountNormalClose = checkedListBoxAlwaysCloseDoor.Items.Count;
            for (int i = 0; i < dwCountNormalClose; i++)
            {
                if (checkedListBoxAlwaysCloseDoor.GetItemChecked(i))
                {
                    m_struEventCardLinkageCfgV51.byNormalClose[i] = 1;
                }
            }

            int dwCountAlarmOutClose = checkedListBoxAlarmOutClose.Items.Count;
            for (int i = 0; i < dwCountAlarmOutClose; i++)
            {
                if (checkedListBoxAlarmOutClose.GetItemChecked(i))
                {
                    m_struEventCardLinkageCfgV51.byAlarmOutClose[i] = 1;
                }
            }

            int dwCountCloseDoor = checkedListBoxCloseDoor.Items.Count;
            for (int i = 0; i < dwCountCloseDoor; i++)
            {
                if (checkedListBoxCloseDoor.GetItemChecked(i))
                {
                    m_struEventCardLinkageCfgV51.byCloseDoor[i] = 1;
                }
            }

            int dwCountAlarmInSetup = checkedListBoxArming.Items.Count;
            for (int i = 0; i < dwCountAlarmInSetup; i++)
            {
                if (checkedListBoxArming.GetItemChecked(i))
                {
                    m_struEventCardLinkageCfgV51.byAlarmInSetup[i] = 1;
                }
            }

            int dwCountReaderBuzzer = checkedListBoxBuzzerReaderCard.Items.Count;
            for (int i = 0; i < dwCountReaderBuzzer; i++)
            {
                if (checkedListBoxBuzzerReaderCard.GetItemChecked(i))
                {
                    m_struEventCardLinkageCfgV51.byReaderBuzzer[i] = 1;
                }
            }

            int dwCountNormalOpen = checkedListBoxAlwaysOpenDoor.Items.Count;
            for (int i = 0; i < dwCountNormalOpen; i++)
            {
                if (checkedListBoxAlwaysOpenDoor.GetItemChecked(i))
                {
                    m_struEventCardLinkageCfgV51.byNormalOpen[i] = 1;
                }
            }

            int dwCountAlarmInClose = checkedListBoxDisarming.Items.Count;
            for (int i = 0; i < dwCountAlarmInClose; i++)
            {
                if (checkedListBoxDisarming.GetItemChecked(i))
                {
                    m_struEventCardLinkageCfgV51.byAlarmInClose[i] = 1;
                }
            }
        }

        private void buttonGet_Click(object sender, EventArgs e)
        {
            uint dwSize = (uint)Marshal.SizeOf(m_struEventCardLinkageCond);

            m_struEventCardLinkageCond.dwSize = dwSize;
            uint.TryParse(textBoxEventNo.Text, out m_struEventCardLinkageCond.dwEventID);
            ushort.TryParse(textBoxLocalConID.Text, out m_struEventCardLinkageCond.wLocalControllerID);

            IntPtr ptrStruCond = Marshal.AllocHGlobal((int)dwSize);
            Marshal.StructureToPtr(m_struEventCardLinkageCond, ptrStruCond, false);
            IntPtr ptrDwReturned = Marshal.AllocHGlobal(4);

            uint dwSizeCfg = (uint)Marshal.SizeOf(m_struEventCardLinkageCfgV51);
            m_struEventCardLinkageCfgV51.dwSize = dwSizeCfg;
            IntPtr ptrStruCfg = Marshal.AllocHGlobal((int)m_struEventCardLinkageCfgV51.dwSize);
            Marshal.StructureToPtr(m_struEventCardLinkageCfgV51, ptrStruCfg, false);

            if (!CHCNetSDK.NET_DVR_GetDeviceConfig(m_lUserID, CHCNetSDK.NET_DVR_GET_EVENT_CARD_LINKAGE_CFG_V51, 1,
                ptrStruCond, dwSize, ptrDwReturned, ptrStruCfg, dwSizeCfg))
            {
                g_formList.AddLog(m_iDeviceIndex, AcsDemoPublic.OPERATION_FAIL_T, "NET_DVR_GET_EVENT_CARD_LINKAGE_CFG_V51 Failed");
                uint error = CHCNetSDK.NET_DVR_GetLastError();
                MessageBox.Show("Get event card linkage cfg failed" + ":" + error);
            }
            else
            {
                uint dwStatus = (uint)Marshal.ReadInt32(ptrDwReturned);
                if (dwStatus != CHCNetSDK.NET_DVR_NOERROR)
                {
                    string strTemp = null;
                    strTemp = string.Format("NET_DVR_GET_EVENT_CARD_LINKAGE_CFG_V51 Error Status{0}", dwStatus);
                    MessageBox.Show(strTemp);
                    Marshal.FreeHGlobal(ptrStruCond);
                    Marshal.FreeHGlobal(ptrStruCfg);
                    Marshal.FreeHGlobal(ptrDwReturned);
                    return;
                } 
                //get data from unmanaged memory
                m_struEventCardLinkageCfgV51 = (CHCNetSDK.NET_DVR_EVENT_CARD_LINKAGE_CFG_V51)Marshal.PtrToStructure(ptrStruCfg, typeof(CHCNetSDK.NET_DVR_EVENT_CARD_LINKAGE_CFG_V51));

                g_formList.AddLog(m_iDeviceIndex, AcsDemoPublic.OPERATION_SUCC_T, "NET_DVR_GET_EVENT_CARD_LINKAGE_CFG_V51 SUCC");
                if (m_struEventCardLinkageCfgV51.byProMode == 1)
                {
                    m_byProMode = true;
                    checkBoxCardNoLinkage.Checked = true;
                    checkBoxMacAddressLinkage.Checked = false;
                    checkBoxEmployeeNoLinkage.Checked = false;
                }
                if (m_struEventCardLinkageCfgV51.byProMode == 2)
                {
                    m_byMacAddr = true;
                    checkBoxMacAddressLinkage.Checked = true;
                    checkBoxCardNoLinkage.Checked = false;
                    checkBoxEmployeeNoLinkage.Checked = false;
                }
                if(m_struEventCardLinkageCfgV51.byProMode == 3)
                {
                    m_byEmployeeNo = true;
                    checkBoxEmployeeNoLinkage.Checked = true;
                    checkBoxCardNoLinkage.Checked = false;
                    checkBoxMacAddressLinkage.Checked = false;
                }
                textBoxEventSourceID.Text = m_struEventCardLinkageCfgV51.dwEventSourceID.ToString();
                checkBoxBuzzer.Checked = (m_struEventCardLinkageCfgV51.byMainDevBuzzer == 1) ? true : false;
                checkBoxSnapshortLinkage.Checked = (m_struEventCardLinkageCfgV51.byCapturePic == 1) ? true : false;
                checkBoxRecordLinkage.Checked = (m_struEventCardLinkageCfgV51.byRecordVideo == 1) ? true : false;
                
                if (m_byProMode)
                {
                    textBoxCardNo.Text = System.Text.Encoding.Default.GetString(m_struEventCardLinkageCfgV51.uLinkageInfo.byCardNo);
                }
                if (m_byMacAddr)
                {
                    textBoxMacAddress.Text = string.Format("{0:x2}:{0:x2}:{0:x2}:{0:x2}:{0:x2}:{0:x2}", m_struEventCardLinkageCfgV51.uLinkageInfo.byMACAddr[0], m_struEventCardLinkageCfgV51.uLinkageInfo.byMACAddr[1],
                        m_struEventCardLinkageCfgV51.uLinkageInfo.byMACAddr[2], m_struEventCardLinkageCfgV51.uLinkageInfo.byMACAddr[3], m_struEventCardLinkageCfgV51.uLinkageInfo.byMACAddr[4],
                        m_struEventCardLinkageCfgV51.uLinkageInfo.byMACAddr[5]);
                }
                if(m_byEmployeeNo)
                {
                    textBoxEmployeeNo.Text = System.Text.Encoding.Default.GetString(m_struEventCardLinkageCfgV51.uLinkageInfo.byEmployeeNo);
                }

                if (m_byProMode == false && m_byMacAddr == false && m_byEmployeeNo == false)
                {
                    comboBoxMajorEventType.SelectedIndex = m_struEventCardLinkageCfgV51.uLinkageInfo.struEventLinkage.wMainEventType;

                    int dwMainEventType = comboBoxMajorEventType.SelectedIndex;
                    if (dwMainEventType == 0)
                    {
                        SubEventTypeDev();
                    }
                    else if (dwMainEventType == 1)
                    {
                        SubEventTypeAlarm();
                    }
                    else if (dwMainEventType == 2)
                    {
                        SubEventTypeDoor();
                    }
                    else if (dwMainEventType == 3)
                    {
                        SubEventTypeCardReader();
                    }

                    comboBoxMinorEventType.SelectedIndex = m_struEventCardLinkageCfgV51.uLinkageInfo.struEventLinkage.wSubEventType;
                }
                CreateChecklistBox();
            }
            m_byProMode = false;
            m_byMacAddr = false;
            m_byEmployeeNo = false;
            Marshal.FreeHGlobal(ptrStruCond);
            Marshal.FreeHGlobal(ptrStruCfg);
            Marshal.FreeHGlobal(ptrDwReturned);
        }

        private void buttonSet_Click(object sender, EventArgs e)
        {
            //init
            m_struEventCardLinkageCfgV51 = new CHCNetSDK.NET_DVR_EVENT_CARD_LINKAGE_CFG_V51();
            m_struEventCardLinkageCfgV51.Init();
            m_struEventCardLinkageCond = new CHCNetSDK.NET_DVR_EVENT_CARD_LINKAGE_COND();
            m_struEventCardLinkageCond.byRes = new byte[106];

            //update
            UpdateUserData();
            m_struEventCardLinkageCfgV51.byCapturePic = (byte)(checkBoxSnapshortLinkage.Checked ? 1 : 0);
            m_struEventCardLinkageCfgV51.byRecordVideo = (byte)(checkBoxRecordLinkage.Checked ? 1 : 0);
            uint.TryParse(textBoxEventNo.Text, out m_struEventCardLinkageCond.dwEventID);
            ushort.TryParse(textBoxLocalConID.Text, out m_struEventCardLinkageCond.wLocalControllerID);

            //allocate unmanaged resources
            uint dwSize = (uint)Marshal.SizeOf(m_struEventCardLinkageCond);
            m_struEventCardLinkageCond.dwSize = dwSize;

            IntPtr ptrStruCond = Marshal.AllocHGlobal((int)dwSize);
            Marshal.StructureToPtr(m_struEventCardLinkageCond, ptrStruCond, false);
            IntPtr ptrDwReturned = Marshal.AllocHGlobal(4);

            uint dwSizeCfg = (uint)Marshal.SizeOf(m_struEventCardLinkageCfgV51);
            m_struEventCardLinkageCfgV51.dwSize = dwSizeCfg;
            IntPtr ptrStruCfg = Marshal.AllocHGlobal((int)m_struEventCardLinkageCfgV51.dwSize);
            Marshal.StructureToPtr(m_struEventCardLinkageCfgV51, ptrStruCfg, false);

            //connect to device
            if (!CHCNetSDK.NET_DVR_SetDeviceConfig(m_lUserID, CHCNetSDK.NET_DVR_SET_EVENT_CARD_LINKAGE_CFG_V51, 1,
                ptrStruCond, dwSize, ptrDwReturned, ptrStruCfg, dwSizeCfg))
            {
                g_formList.AddLog(m_iDeviceIndex, AcsDemoPublic.OPERATION_FAIL_T, "NET_DVR_SET_EVENT_CARD_LINKAGE_CFG_V51 Failed");
                uint error = CHCNetSDK.NET_DVR_GetLastError();
                MessageBox.Show("Set event card linkage cfg Failed" + ":" + error);
            }
            else
            {
                g_formList.AddLog(m_iDeviceIndex, AcsDemoPublic.OPERATION_SUCC_T, "NET_DVR_SET_EVENT_CARD_LINKAGE_CFG_V51 SUCC");
            }
            m_byProMode = false;
            m_byMacAddr = false;
            m_byEmployeeNo = false;
            Marshal.FreeHGlobal(ptrStruCond);
            Marshal.FreeHGlobal(ptrStruCfg);
            Marshal.FreeHGlobal(ptrDwReturned);
        }
    }
}
