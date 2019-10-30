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

using AcsDemo.Language;
using AcsDemo.Public;

namespace AcsDemo.ProductAcs
{
    public partial class HolidayPlan : Form
    {
        public Int32 m_lUserID = -1;
        public Int32 m_iDeviceIndex = -1;
        public int m_iDeviceType = 0;

        public CHCNetSDK.NET_DVR_HOLIDAY_PLAN_CFG m_struPlanCfgH = new CHCNetSDK.NET_DVR_HOLIDAY_PLAN_CFG();
        public CHCNetSDK.NET_DVR_HOLIDAY_PLAN_COND m_struPlanCond = new CHCNetSDK.NET_DVR_HOLIDAY_PLAN_COND();

        private DeviceLogList g_formList = DeviceLogList.Instance();
        private int iItemIndex = -1;

        public HolidayPlan()
        {
            InitializeComponent();
            m_struPlanCfgH.Init();
            m_struPlanCond.Init();
            UpdateList();
            this.SuspendLayout();
            this.columnHeader1.Name = "columnHeader1";
            this.columnHeader2.Name = "columnHeader2";
            this.columnHeader3.Name = "columnHeader3";
            this.columnHeader4.Name = "columnHeader4";
            this.columnHeader5.Name = "columnHeader5";
            this.columnHeader6.Name = "columnHeader6";
            this.ResumeLayout();
        }
        public void CustomRelayout()
        {
            switch (m_iDeviceType)
            {
                case 0:
                    // door plan without verify mode
                    comboBoxVerificationMode.Hide();
                    label6.Hide();
                    // hide LocalController control
                    textBoxLocalControllerID.Hide();
                    label8.Hide();
                    break;
                case 1:
                    //card reader plan without door mode
                    comboBoxDoorStateMode.Hide();
                    label7.Hide();
                    // hide LocalController control
                    textBoxLocalControllerID.Hide();
                    label8.Hide();
                    break;
                case 2:
                    // card right plan without door mode
                    comboBoxDoorStateMode.Hide();
                    label7.Hide();
                    // card right plan without verify mode
                    comboBoxVerificationMode.Hide();
                    label6.Hide();
                    break;
                default:
                    foreach (Control c in this.Controls)
                    {
                        c.Enabled = false;
                    }
                    MessageBox.Show("Wrong Device Type!");
                    break;
            }

            if (m_iDeviceType >= 0 && m_iDeviceType <= 2)
            { 
                comboBoxDeviceType.SelectedIndex = m_iDeviceType; 
            }
            
            comboBoxDeviceType.Enabled = false;
        }

        private void UpdateList()
        {
            CHCNetSDK.NET_DVR_SINGLE_PLAN_SEGMENT[] struTemp = new CHCNetSDK.NET_DVR_SINGLE_PLAN_SEGMENT[CHCNetSDK.MAX_TIMESEGMENT_V30];
            for (int i = 0; i < CHCNetSDK.MAX_TIMESEGMENT_V30; i++)
            {
                struTemp[i] = m_struPlanCfgH.struPlanCfg[i];
            }

            listViewTimeSegment.BeginUpdate();
            listViewTimeSegment.Items.Clear();
            string strTemp = null;
            for (int i = 0; i < CHCNetSDK.MAX_TIMESEGMENT_V30; i++)
            {
                ListViewItem listItem = new ListViewItem();
                strTemp = string.Format("{0}", i + 1);
                listItem.Text = strTemp;
                if (1 == struTemp[i].byEnable)
                {
                    strTemp = "yes";
                }
                else
                {
                    strTemp = "no";
                }
                listItem.SubItems.Add(strTemp);
                if (struTemp[i].byDoorStatus > 4)
                {
                    strTemp = string.Format("{0}", struTemp[i].byDoorStatus);
                }
                else
                {
                    int iDoorIndex = (int)struTemp[i].byDoorStatus;
                    strTemp = AcsDemoPublic.strDoorStatus[iDoorIndex];
                }
                listItem.SubItems.Add(strTemp);
                if (struTemp[i].byVerifyMode > 9)
                {
                    strTemp = string.Format("{0}", struTemp[i].byVerifyMode);
                }
                else
                {
                    int iDoorIndex = (int)struTemp[i].byVerifyMode;
                    strTemp = AcsDemoPublic.strVerify[iDoorIndex];
                }
                listItem.SubItems.Add(strTemp);
                CHCNetSDK.NET_DVR_SIMPLE_DAYTIME strTime = struTemp[i].struTimeSegment.struBeginTime;
                strTemp = string.Format("{0,2}:{1,2}:{2,2}", strTime.byHour, strTime.byMinute, strTime.bySecond);
                listItem.SubItems.Add(strTemp);
                strTime = struTemp[i].struTimeSegment.struEndTime;
                strTemp = string.Format("{0,2}:{1,2}:{2,2}", strTime.byHour, strTime.byMinute, strTime.bySecond);
                listItem.SubItems.Add(strTemp);
                listViewTimeSegment.Items.Add(listItem);
            }

            listViewTimeSegment.EndUpdate();
        }

        private void buttonGet_Click(object sender, EventArgs e)
        {
            uint dwCommand = 0;
            string[] strCommand = { "NET_DVR_GET_DOOR_STATUS_HOLIDAY_PLAN", "NET_DVR_GET_VERIFY_HOLIDAY_PLAN", "NET_DVR_GET_CARD_RIGHT_HOLIDAY_PLAN_V50" };

            uint dwReturned = 0;
            string strTemp = null;
            uint dwSize = (uint)Marshal.SizeOf(m_struPlanCfgH);
            m_struPlanCfgH.dwSize = dwSize;
            IntPtr ptrPlanCfgH = Marshal.AllocHGlobal((int)dwSize);
            Marshal.StructureToPtr(m_struPlanCfgH, ptrPlanCfgH, false);

            switch (comboBoxDeviceType.SelectedIndex)
            {
                case 0:
                case 1:
                    if (comboBoxDeviceType.SelectedIndex == 0)
                    {
                        dwCommand = (uint)CHCNetSDK.NET_DVR_GET_DOOR_STATUS_HOLIDAY_PLAN;
                    }
                    else
                    {
                        dwCommand = (uint)CHCNetSDK.NET_DVR_GET_VERIFY_HOLIDAY_PLAN;
                    }

                    int holidayPlanNumberIndex;
                    int.TryParse(textBoxHolidayPlanNumber.Text, out holidayPlanNumberIndex);

                    if (!CHCNetSDK.NET_DVR_GetDVRConfig(m_lUserID, dwCommand, holidayPlanNumberIndex, ptrPlanCfgH, dwSize, ref dwReturned))
                    {
                        Marshal.FreeHGlobal(ptrPlanCfgH);
                        strTemp = string.Format("{0} FAIL, ERROR CODE {1}", strCommand[comboBoxDeviceType.SelectedIndex], CHCNetSDK.NET_DVR_GetLastError());
                        MessageBox.Show(strTemp);
                        return;
                    }
                    else
                    {
                        strTemp = string.Format("{0}", strCommand[comboBoxDeviceType.SelectedIndex]);
                        g_formList.AddLog(m_lUserID, AcsDemoPublic.OPERATION_SUCC_T, strTemp);
                    }

                    break;
                case 2:
                    dwCommand = (uint)CHCNetSDK.NET_DVR_GET_CARD_RIGHT_HOLIDAY_PLAN_V50;
                    uint dwConSize = (uint)Marshal.SizeOf(m_struPlanCond);
                    m_struPlanCond.dwSize = dwConSize;
                    // limited input data guarantee parse success
                    uint.TryParse(textBoxHolidayPlanNumber.Text, out m_struPlanCond.dwHolidayPlanNumber);
                    ushort.TryParse(textBoxLocalControllerID.Text, out m_struPlanCond.wLocalControllerID);

                    IntPtr ptrPlanCon = Marshal.AllocHGlobal((int)dwConSize);
                    Marshal.StructureToPtr(m_struPlanCond, ptrPlanCon, false);
                    IntPtr ptrDwReturned = Marshal.AllocHGlobal(4);

                    if (!CHCNetSDK.NET_DVR_GetDeviceConfig(m_lUserID, dwCommand, 1, ptrPlanCon, dwConSize, ptrDwReturned, ptrPlanCfgH, dwSize))
                    {
                        Marshal.FreeHGlobal(ptrPlanCfgH);
                        Marshal.FreeHGlobal(ptrPlanCon);
                        Marshal.FreeHGlobal(ptrDwReturned);
                        strTemp = string.Format("{0} FAIL, ERROR CODE {1}", strCommand[comboBoxDeviceType.SelectedIndex], CHCNetSDK.NET_DVR_GetLastError());
                        MessageBox.Show(strTemp);
                        return;
                    }
                    else
                    {
                        dwReturned = (uint)Marshal.ReadInt32(ptrDwReturned);
                        Marshal.FreeHGlobal(ptrDwReturned);
                        Marshal.FreeHGlobal(ptrPlanCon);
                        strTemp = string.Format("{0}", strCommand[comboBoxDeviceType.SelectedIndex]);
                        g_formList.AddLog(m_lUserID, AcsDemoPublic.OPERATION_SUCC_T, strTemp);
                    }

                    break;
                default:
                    Marshal.FreeHGlobal(ptrPlanCfgH);
                    MessageBox.Show("unknown command");
                    return;
            }

            m_struPlanCfgH = (CHCNetSDK.NET_DVR_HOLIDAY_PLAN_CFG)Marshal.PtrToStructure(ptrPlanCfgH, typeof(CHCNetSDK.NET_DVR_HOLIDAY_PLAN_CFG));

            UpdateList();

            if (1 == m_struPlanCfgH.byEnable)
            {
                checkBoxEnablePlan.Checked = true;
            }
            else
            {
                checkBoxEnablePlan.Checked = false;
            }

            if (!AcsDemoPublic.CheckState(m_struPlanCfgH.struBeginDate) || !AcsDemoPublic.CheckState(m_struPlanCfgH.struEndDate))
            {
                Marshal.FreeHGlobal(ptrPlanCfgH);
                return;
            }

            // set the date
            dateTimePickerStartDateHP.Value = new System.DateTime(m_struPlanCfgH.struBeginDate.wYear, m_struPlanCfgH.struBeginDate.byMonth, m_struPlanCfgH.struBeginDate.byDay);
            dateTimePickerEndDateHP.Value = new System.DateTime(m_struPlanCfgH.struEndDate.wYear, m_struPlanCfgH.struEndDate.byMonth, m_struPlanCfgH.struEndDate.byDay);

            Marshal.FreeHGlobal(ptrPlanCfgH);
        }

        private void buttonSet_Click(object sender, EventArgs e)
        {
            AdjustTime();
            uint dwCommand = 0;
            string[] strCommand = { "NET_DVR_SET_DOOR_STATUS_HOLIDAY_PLAN", "NET_DVR_SET_VERIFY_HOLIDAY_PLAN", "NET_DVR_SET_CARD_RIGHT_HOLIDAY_PLAN_V50"};

            uint dwReturned = 0;
            string strTemp = null;

            if (checkBoxEnablePlan.Checked)
            {
                m_struPlanCfgH.byEnable = 1;
            }
            else
            {
                m_struPlanCfgH.byEnable = 0;
            }

            // set the date
            m_struPlanCfgH.struBeginDate.wYear = (ushort)dateTimePickerStartDateHP.Value.Year;
            m_struPlanCfgH.struBeginDate.byMonth = (byte)dateTimePickerStartDateHP.Value.Month;
            m_struPlanCfgH.struBeginDate.byDay = (byte)dateTimePickerStartDateHP.Value.Day;
            m_struPlanCfgH.struEndDate.wYear = (ushort)dateTimePickerEndDateHP.Value.Year;
            m_struPlanCfgH.struEndDate.byMonth = (byte)dateTimePickerEndDateHP.Value.Month;
            m_struPlanCfgH.struEndDate.byDay = (byte)dateTimePickerEndDateHP.Value.Day;

            uint dwSize = (uint)Marshal.SizeOf(m_struPlanCfgH);
            m_struPlanCfgH.dwSize = dwSize;
            IntPtr ptrPlanCfg = Marshal.AllocHGlobal((int)dwSize);
            Marshal.StructureToPtr(m_struPlanCfgH, ptrPlanCfg, false);


            switch (comboBoxDeviceType.SelectedIndex)
            {
                case 0:
                case 1:
                    if (comboBoxDeviceType.SelectedIndex == 0)
                    {
                        dwCommand = (uint)CHCNetSDK.NET_DVR_SET_DOOR_STATUS_HOLIDAY_PLAN;
                    }
                    else
                    {
                        dwCommand = (uint)CHCNetSDK.NET_DVR_SET_VERIFY_HOLIDAY_PLAN;
                    }

                    int holidayPlanNumberIndex;
                    int.TryParse(textBoxHolidayPlanNumber.Text, out holidayPlanNumberIndex);

                    if (!CHCNetSDK.NET_DVR_SetDVRConfig(m_lUserID, dwCommand, holidayPlanNumberIndex, ptrPlanCfg, dwSize))
                    {
                        Marshal.FreeHGlobal(ptrPlanCfg);
                        strTemp = string.Format("{0} FAIL, ERROR CODE {1}", strCommand[comboBoxDeviceType.SelectedIndex], CHCNetSDK.NET_DVR_GetLastError());
                        MessageBox.Show(strTemp);
                        return;
                    }
                    else
                    {
                        strTemp = string.Format("{0}", strCommand[comboBoxDeviceType.SelectedIndex]);
                        g_formList.AddLog(m_lUserID, AcsDemoPublic.OPERATION_SUCC_T, strTemp);
                    }

                    break;
                case 2:
                    dwCommand = (uint)CHCNetSDK.NET_DVR_SET_CARD_RIGHT_HOLIDAY_PLAN_V50;
                    uint dwConSize = (uint)Marshal.SizeOf(m_struPlanCond);
                    m_struPlanCond.dwSize = dwConSize;
                    IntPtr ptrPlanCon = Marshal.AllocHGlobal((int)dwConSize);
                    Marshal.StructureToPtr(m_struPlanCond, ptrPlanCon, false);
                    IntPtr ptrDwReturned = Marshal.AllocHGlobal(4);

                    // limited input data guarantee parse success
                    // limited input data guarantee parse success
                    uint.TryParse(textBoxHolidayPlanNumber.Text, out m_struPlanCond.dwHolidayPlanNumber);
                    ushort.TryParse(textBoxLocalControllerID.Text, out m_struPlanCond.wLocalControllerID);


                    if (!CHCNetSDK.NET_DVR_SetDeviceConfig(m_lUserID, dwCommand, 1, ptrPlanCon, dwConSize, ptrDwReturned, ptrPlanCfg, dwSize))
                    {
                        Marshal.FreeHGlobal(ptrPlanCfg);
                        Marshal.FreeHGlobal(ptrPlanCon);
                        Marshal.FreeHGlobal(ptrDwReturned);
                        strTemp = string.Format("{0} FAIL, ERROR CODE {1}", strCommand[comboBoxDeviceType.SelectedIndex], CHCNetSDK.NET_DVR_GetLastError());
                        MessageBox.Show(strTemp);
                        return;
                    }
                    else
                    {
                        dwReturned = (uint)Marshal.ReadInt32(ptrDwReturned);
                        Marshal.FreeHGlobal(ptrDwReturned);
                        Marshal.FreeHGlobal(ptrPlanCon);
                        strTemp = string.Format("{0}", strCommand[comboBoxDeviceType.SelectedIndex]);
                        g_formList.AddLog(m_lUserID, AcsDemoPublic.OPERATION_SUCC_T, strTemp);
                    }

                    break;
                default:
                    Marshal.FreeHGlobal(ptrPlanCfg);
                    MessageBox.Show("unknown command");
                    return;
            }
            Marshal.FreeHGlobal(ptrPlanCfg);
        }

        private void buttonConfirm_Click(object sender, EventArgs e)
        {
            this.DialogResult = DialogResult.OK;
            return;
        }

        private void buttonAdd_Click(object sender, EventArgs e)
        {
            if (-1 == iItemIndex)
            {
                MessageBox.Show("Please select the list!!!");
                return;
            }
            if (checkBoxEnableTimeSegment.Checked)
            {
                m_struPlanCfgH.struPlanCfg[iItemIndex].byEnable = 1;
            }
            else
            {
                m_struPlanCfgH.struPlanCfg[iItemIndex].byEnable = 0;
            }
            m_struPlanCfgH.struPlanCfg[iItemIndex].byVerifyMode = (byte)comboBoxVerificationMode.SelectedIndex;
            m_struPlanCfgH.struPlanCfg[iItemIndex].byDoorStatus = (byte)comboBoxDoorStateMode.SelectedIndex;
            m_struPlanCfgH.struPlanCfg[iItemIndex].struTimeSegment.struBeginTime.byHour = (byte)dateTimePickerStartTime.Value.Hour;
            m_struPlanCfgH.struPlanCfg[iItemIndex].struTimeSegment.struBeginTime.byMinute = (byte)dateTimePickerStartTime.Value.Minute;
            m_struPlanCfgH.struPlanCfg[iItemIndex].struTimeSegment.struBeginTime.bySecond = (byte)dateTimePickerStartTime.Value.Second;
            if (m_struPlanCfgH.struPlanCfg[iItemIndex].struTimeSegment.struBeginTime.byHour == 23
                && m_struPlanCfgH.struPlanCfg[iItemIndex].struTimeSegment.struBeginTime.byMinute == 59
                && m_struPlanCfgH.struPlanCfg[iItemIndex].struTimeSegment.struBeginTime.bySecond == 59)
            {
                m_struPlanCfgH.struPlanCfg[iItemIndex].struTimeSegment.struBeginTime.byHour = 24;
                m_struPlanCfgH.struPlanCfg[iItemIndex].struTimeSegment.struBeginTime.byMinute = 0;
                m_struPlanCfgH.struPlanCfg[iItemIndex].struTimeSegment.struBeginTime.bySecond = 0;
            }
            m_struPlanCfgH.struPlanCfg[iItemIndex].struTimeSegment.struEndTime.byHour = (byte)dateTimePickerEndTime.Value.Hour;
            m_struPlanCfgH.struPlanCfg[iItemIndex].struTimeSegment.struEndTime.byMinute = (byte)dateTimePickerEndTime.Value.Minute;
            m_struPlanCfgH.struPlanCfg[iItemIndex].struTimeSegment.struEndTime.bySecond = (byte)dateTimePickerEndTime.Value.Second;
            if (m_struPlanCfgH.struPlanCfg[iItemIndex].struTimeSegment.struBeginTime.byHour == 23
                && m_struPlanCfgH.struPlanCfg[iItemIndex].struTimeSegment.struBeginTime.byMinute == 59
                && m_struPlanCfgH.struPlanCfg[iItemIndex].struTimeSegment.struBeginTime.bySecond == 59)
            {
                m_struPlanCfgH.struPlanCfg[iItemIndex].struTimeSegment.struBeginTime.byHour = 24;
                m_struPlanCfgH.struPlanCfg[iItemIndex].struTimeSegment.struBeginTime.byMinute = 0;
                m_struPlanCfgH.struPlanCfg[iItemIndex].struTimeSegment.struBeginTime.bySecond = 0;
            }

            UpdateList();

        }

        private void listViewTimeSegment_ItemSelectionChanged(object sender, ListViewItemSelectionChangedEventArgs e)
        {
            iItemIndex = e.ItemIndex;
            if (iItemIndex < 0)
            {
                return;
            }

            int i = iItemIndex;

            if (1 == m_struPlanCfgH.struPlanCfg[i].byEnable)
            {
                checkBoxEnableTimeSegment.Checked = true;
            }
            else
            {
                checkBoxEnableTimeSegment.Checked = false;
            }
            comboBoxDoorStateMode.SelectedIndex = (int)m_struPlanCfgH.struPlanCfg[i].byDoorStatus;
            comboBoxVerificationMode.SelectedIndex = (int)m_struPlanCfgH.struPlanCfg[i].byVerifyMode;
            CHCNetSDK.NET_DVR_SIMPLE_DAYTIME struTime = new CHCNetSDK.NET_DVR_SIMPLE_DAYTIME();
            if (AcsDemoPublic.CheckDate(m_struPlanCfgH.struPlanCfg[i].struTimeSegment.struBeginTime))
            {
                struTime = m_struPlanCfgH.struPlanCfg[i].struTimeSegment.struBeginTime;
                if (struTime.byHour == 24 && struTime.byMinute == 0 && struTime.bySecond == 0)
                {
                    struTime.byHour = 23;
                    struTime.byMinute = 59;
                    struTime.bySecond = 59;
                }
                dateTimePickerStartTime.Value = new System.DateTime(dateTimePickerStartTime.Value.Year,
                    dateTimePickerStartTime.Value.Month, dateTimePickerStartTime.Value.Day, struTime.byHour,
                    struTime.byMinute, struTime.bySecond);
            }
            if (AcsDemoPublic.CheckDate(m_struPlanCfgH.struPlanCfg[i].struTimeSegment.struEndTime))
            {
                struTime = m_struPlanCfgH.struPlanCfg[i].struTimeSegment.struEndTime;
                if (struTime.byHour == 24 && struTime.byMinute == 0 && struTime.bySecond == 0)
                {
                    struTime.byHour = 23;
                    struTime.byMinute = 59;
                    struTime.bySecond = 59;
                }
                dateTimePickerEndTime.Value = new System.DateTime(dateTimePickerEndTime.Value.Year,
                    dateTimePickerEndTime.Value.Month, dateTimePickerEndTime.Value.Day, struTime.byHour,
                    struTime.byMinute, struTime.bySecond);
            }
        }

        private void textBoxHolidayPlanNumber_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar != '\b')//backspace 
            {
                if ((e.KeyChar < '0') || (e.KeyChar > '9'))//0-9 is permitted  
                {
                    e.Handled = true;
                }
            }
        }

        private void textBoxLocalControllerID_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar != '\b')//backspace 
            {
                if ((e.KeyChar < '0') || (e.KeyChar > '9'))//0-9 is permitted  
                {
                    e.Handled = true;
                }
            }
        }

        private void AdjustTime()
        {
            for (int i = 0; i < CHCNetSDK.MAX_TIMESEGMENT_V30; i++)
            {
                if (m_struPlanCfgH.struPlanCfg[i].struTimeSegment.struBeginTime.byHour == 23
                    && m_struPlanCfgH.struPlanCfg[i].struTimeSegment.struBeginTime.byMinute == 59
                    && m_struPlanCfgH.struPlanCfg[i].struTimeSegment.struBeginTime.bySecond == 59)
                {
                    m_struPlanCfgH.struPlanCfg[i].struTimeSegment.struBeginTime.byHour = 24;
                    m_struPlanCfgH.struPlanCfg[i].struTimeSegment.struBeginTime.byMinute = 0;
                    m_struPlanCfgH.struPlanCfg[i].struTimeSegment.struBeginTime.bySecond = 0;
                }

                if (m_struPlanCfgH.struPlanCfg[i].struTimeSegment.struEndTime.byHour == 23
                    && m_struPlanCfgH.struPlanCfg[i].struTimeSegment.struEndTime.byMinute == 59
                    && m_struPlanCfgH.struPlanCfg[i].struTimeSegment.struEndTime.bySecond == 59)
                {
                    m_struPlanCfgH.struPlanCfg[i].struTimeSegment.struEndTime.byHour = 24;
                    m_struPlanCfgH.struPlanCfg[i].struTimeSegment.struEndTime.byMinute = 0;
                    m_struPlanCfgH.struPlanCfg[i].struTimeSegment.struEndTime.bySecond = 0;
                }
            }
        }

        private void HolidayPlan_Load(object sender, EventArgs e)
        {
            MultiLanguage.LoadLanguage(this);
        }

    }
}
