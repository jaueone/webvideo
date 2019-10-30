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
    public partial class WeekPlan : Form
    {
        public Int32 m_lUserID = -1;
        public Int32 m_iDeviceIndex = -1;
        public int m_iDeviceType = 0;

        public CHCNetSDK.NET_DVR_WEEK_PLAN_CFG m_struPlanCfg = new CHCNetSDK.NET_DVR_WEEK_PLAN_CFG();
        public CHCNetSDK.NET_DVR_WEEK_PLAN_COND m_struPlanCond = new CHCNetSDK.NET_DVR_WEEK_PLAN_COND();

        private DeviceLogList g_formList = DeviceLogList.Instance();
        private int iItemIndex = -1;

        public WeekPlan()
        {
            InitializeComponent();
            m_struPlanCfg.Init();
            m_struPlanCond.Init();
            this.SuspendLayout();
            this.columnHeader1.Name = "columnHeader1";
            this.columnHeader2.Name = "columnHeader2";
            this.columnHeader3.Name = "columnHeader3";
            this.columnHeader4.Name = "columnHeader4";
            this.columnHeader5.Name = "columnHeader5";
            this.columnHeader6.Name = "columnHeader6";
            this.ResumeLayout();
            UpdateList();
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
                    textBoxLocalControllerIDWP.Hide();
                    label8.Hide();
                    break;
                case 1:
                    //card reader plan without door mode
                    comboBoxDoorStateMode.Hide();
                    label7.Hide();
                    // hide LocalController control
                    textBoxLocalControllerIDWP.Hide();
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
                    foreach(Control c in this.Controls)
                    {
                        c.Enabled = false;
                    }
                    MessageBox.Show("Wrong Device Type!");
                    break;
            }

            if (m_iDeviceType >= 0 && m_iDeviceType <= 2)
            {
                comboBoxDeviceTypeWP.SelectedIndex = m_iDeviceType;
            }

            comboBoxDeviceTypeWP.Enabled = false;
        }

        private void buttonGetWP_Click(object sender, EventArgs e)
        {
            uint dwCommand = 0;
            string[] strCommand = { "NET_DVR_GET_WEEK_PLAN_CFG", "NET_DVR_GET_VERIFY_WEEK_PLAN", "NET_DVR_GET_CARD_RIGHT_WEEK_PLAN_V50" };

            uint dwReturned = 0;
            string strTemp = null;
            uint dwSize = (uint)Marshal.SizeOf(m_struPlanCfg);
            m_struPlanCfg.dwSize = dwSize;
            IntPtr ptrPlanCfg = Marshal.AllocHGlobal((int)dwSize);
            Marshal.StructureToPtr(m_struPlanCfg, ptrPlanCfg, false);

            switch (comboBoxDeviceTypeWP.SelectedIndex)
            {
                case 0:
                case 1:
                    if (comboBoxDeviceTypeWP.SelectedIndex == 0)
                    {
                        dwCommand = (uint)CHCNetSDK.NET_DVR_GET_WEEK_PLAN_CFG;
                    }
                    else
                    {
                        dwCommand = (uint)CHCNetSDK.NET_DVR_GET_VERIFY_WEEK_PLAN;
                    }

                    int weekPlanNumberWPIndex;
                    int.TryParse(textBoxWeekPlanNumberWP.Text, out weekPlanNumberWPIndex);

                    if (!CHCNetSDK.NET_DVR_GetDVRConfig(m_lUserID, dwCommand, weekPlanNumberWPIndex, ptrPlanCfg, dwSize, ref dwReturned))
                    {
                        Marshal.FreeHGlobal(ptrPlanCfg);
                        strTemp = string.Format("{0} FAIL, ERROR CODE {1}", strCommand[comboBoxDeviceTypeWP.SelectedIndex], CHCNetSDK.NET_DVR_GetLastError());
                        MessageBox.Show(strTemp);
                        return;
                    }
                    else
                    {
                        strTemp = string.Format("{0}",strCommand[comboBoxDeviceTypeWP.SelectedIndex]);
                        g_formList.AddLog(m_lUserID, AcsDemoPublic.OPERATION_SUCC_T, strTemp);
                    }

                    break;
                case 2:
                    dwCommand = (uint)CHCNetSDK.NET_DVR_GET_CARD_RIGHT_WEEK_PLAN_V50;
                    uint dwConSize = (uint)Marshal.SizeOf(m_struPlanCond);
                    m_struPlanCond.dwSize = dwConSize;
                    // limited input data guarantee parse success
                    uint.TryParse(textBoxWeekPlanNumberWP.Text, out m_struPlanCond.dwWeekPlanNumber);
                    ushort.TryParse(textBoxLocalControllerIDWP.Text, out m_struPlanCond.wLocalControllerID);

                    IntPtr ptrPlanCon = Marshal.AllocHGlobal((int)dwConSize);
                    Marshal.StructureToPtr(m_struPlanCond, ptrPlanCon, false);
                    IntPtr ptrDwReturned = Marshal.AllocHGlobal(4);

                    if (!CHCNetSDK.NET_DVR_GetDeviceConfig(m_lUserID, dwCommand, 1, ptrPlanCon, dwConSize, ptrDwReturned, ptrPlanCfg, dwSize))
                    {
                        Marshal.FreeHGlobal(ptrPlanCfg);
                        Marshal.FreeHGlobal(ptrPlanCon);
                        Marshal.FreeHGlobal(ptrDwReturned);
                        strTemp = string.Format("{0} FAIL, ERROR CODE {1}", strCommand[comboBoxDeviceTypeWP.SelectedIndex], CHCNetSDK.NET_DVR_GetLastError());
                        MessageBox.Show(strTemp);
                        return;
                    }
                    else
                    {
                        dwReturned = (uint)Marshal.ReadInt32(ptrDwReturned);
                        Marshal.FreeHGlobal(ptrDwReturned);
                        Marshal.FreeHGlobal(ptrPlanCon);
                        strTemp = string.Format("{0}",strCommand[comboBoxDeviceTypeWP.SelectedIndex]);
                        g_formList.AddLog(m_lUserID, AcsDemoPublic.OPERATION_SUCC_T, strTemp);
                    }

                    break;
                default:
                    Marshal.FreeHGlobal(ptrPlanCfg);
                    MessageBox.Show("unknown command");
                    return;
            }

            m_struPlanCfg = (CHCNetSDK.NET_DVR_WEEK_PLAN_CFG)Marshal.PtrToStructure(ptrPlanCfg, typeof(CHCNetSDK.NET_DVR_WEEK_PLAN_CFG));
            comboBoxDateWP.SelectedIndex = 0;
            UpdateList();

            if (1 == m_struPlanCfg.byEnable)
            {
                checkBoxEnableWP.Checked = true;
            }
            else
            {
                checkBoxEnableWP.Checked = false;
            }

            Marshal.FreeHGlobal(ptrPlanCfg);
        }

        private void UpdateList()
        {
            int iDate = comboBoxDateWP.SelectedIndex;

            CHCNetSDK.NET_DVR_SINGLE_PLAN_SEGMENT[] struTemp = new CHCNetSDK.NET_DVR_SINGLE_PLAN_SEGMENT[CHCNetSDK.MAX_TIMESEGMENT_V30];
            for (int i = 0; i < CHCNetSDK.MAX_TIMESEGMENT_V30; i++)
            {
                struTemp[i] = m_struPlanCfg.struPlanCfg[iDate * CHCNetSDK.MAX_TIMESEGMENT_V30 + i];
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

        private void comboBoxDateWP_SelectedIndexChanged(object sender, EventArgs e)
        {
            UpdateList();
        }

        private void textBoxWeekPlanNumberWP_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar != '\b')//backspace 
            {
                if ((e.KeyChar < '0') || (e.KeyChar > '9'))//1-9 is permitted  
                {
                    e.Handled = true;
                }
            }
        }

        private void textBoxLocalControllerIDWP_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar != '\b')//backspace 
            {
                if ((e.KeyChar < '0') || (e.KeyChar > '9'))//0-9 is permitted  
                {
                    e.Handled = true;
                }
            }
        }

        private void buttonSetWP_Click(object sender, EventArgs e)
        {
            AdjustTime();
            uint dwCommand = 0;
            string[] strCommand = { "NET_DVR_SET_WEEK_PLAN_CFG", "NET_DVR_SET_VERIFY_WEEK_PLAN", "NET_DVR_SET_CARD_RIGHT_WEEK_PLAN_V50" };

            uint dwReturned = 0;
            string strTemp = null;  
            
            if (checkBoxEnableWP.Checked)
            {
                m_struPlanCfg.byEnable = 1;
            }
            else
            {
                m_struPlanCfg.byEnable = 0;
            }

            uint dwSize = (uint)Marshal.SizeOf(m_struPlanCfg);
            m_struPlanCfg.dwSize = dwSize;
            IntPtr ptrPlanCfg = Marshal.AllocHGlobal((int)dwSize);
            Marshal.StructureToPtr(m_struPlanCfg, ptrPlanCfg, false);

          
            switch (comboBoxDeviceTypeWP.SelectedIndex)
            {
                case 0:
                case 1:
                    if (comboBoxDeviceTypeWP.SelectedIndex == 0)
                    {
                        dwCommand = (uint)CHCNetSDK.NET_DVR_SET_WEEK_PLAN_CFG;
                    }
                    else
                    {
                        dwCommand = (uint)CHCNetSDK.NET_DVR_SET_VERIFY_WEEK_PLAN;
                    }

                    int weekPlanNumberWPIndex;
                    int.TryParse(textBoxWeekPlanNumberWP.Text, out weekPlanNumberWPIndex);

                    if (!CHCNetSDK.NET_DVR_SetDVRConfig(m_lUserID, dwCommand, weekPlanNumberWPIndex, ptrPlanCfg, dwSize))
                    {
                        Marshal.FreeHGlobal(ptrPlanCfg);
                        strTemp = string.Format("{0} FAIL, ERROR CODE {1}", strCommand[comboBoxDeviceTypeWP.SelectedIndex], CHCNetSDK.NET_DVR_GetLastError());
                        MessageBox.Show(strTemp);
                        return;
                    }
                    else
                    {
                        strTemp = string.Format("{0}", strCommand[comboBoxDeviceTypeWP.SelectedIndex]);
                        g_formList.AddLog(m_lUserID, AcsDemoPublic.OPERATION_SUCC_T, strTemp);
                    }

                    break;
                case 2:
                    dwCommand = (uint)CHCNetSDK.NET_DVR_SET_CARD_RIGHT_WEEK_PLAN_V50;
                    uint dwConSize = (uint)Marshal.SizeOf(m_struPlanCond);
                    m_struPlanCond.dwSize = dwConSize;
                    IntPtr ptrPlanCon = Marshal.AllocHGlobal((int)dwConSize);
                    Marshal.StructureToPtr(m_struPlanCond, ptrPlanCon, false);
                    IntPtr ptrDwReturned = Marshal.AllocHGlobal(4);

                    // limited input data guarantee parse success
                    uint.TryParse(textBoxWeekPlanNumberWP.Text, out m_struPlanCond.dwWeekPlanNumber);
                    ushort.TryParse(textBoxLocalControllerIDWP.Text, out m_struPlanCond.wLocalControllerID);
                    

                    if (!CHCNetSDK.NET_DVR_SetDeviceConfig(m_lUserID, dwCommand, 1, ptrPlanCon, dwConSize, ptrDwReturned, ptrPlanCfg, dwSize))
                    {
                        Marshal.FreeHGlobal(ptrPlanCfg);
                        Marshal.FreeHGlobal(ptrPlanCon);
                        Marshal.FreeHGlobal(ptrDwReturned);
                        strTemp = string.Format("{0} FAIL, ERROR CODE {1}", strCommand[comboBoxDeviceTypeWP.SelectedIndex], CHCNetSDK.NET_DVR_GetLastError());
                        MessageBox.Show(strTemp);
                        return;
                    }
                    else
                    {
                        dwReturned = (uint)Marshal.ReadInt32(ptrDwReturned);
                        Marshal.FreeHGlobal(ptrDwReturned);
                        Marshal.FreeHGlobal(ptrPlanCon);
                        strTemp = string.Format("{0}",strCommand[comboBoxDeviceTypeWP.SelectedIndex]);
                        g_formList.AddLog(m_lUserID, AcsDemoPublic.OPERATION_SUCC_T, strTemp);
                    }

                    break;
                default:
                    Marshal.FreeHGlobal(ptrPlanCfg);
                    MessageBox.Show("unknow command");
                    return;
            }
            Marshal.FreeHGlobal(ptrPlanCfg);
        }

        private void AdjustTime()
        {
            for (int i = 0; i < CHCNetSDK.MAX_TIMESEGMENT_V30; i++)
            {
                if (m_struPlanCfg.struPlanCfg[i].struTimeSegment.struBeginTime.byHour == 23
                    && m_struPlanCfg.struPlanCfg[i].struTimeSegment.struBeginTime.byMinute == 59
                    && m_struPlanCfg.struPlanCfg[i].struTimeSegment.struBeginTime.bySecond == 59)
                {
                    m_struPlanCfg.struPlanCfg[i].struTimeSegment.struBeginTime.byHour = 24;
                    m_struPlanCfg.struPlanCfg[i].struTimeSegment.struBeginTime.byMinute = 0;
                    m_struPlanCfg.struPlanCfg[i].struTimeSegment.struBeginTime.bySecond = 0;
                }

                if (m_struPlanCfg.struPlanCfg[i].struTimeSegment.struEndTime.byHour == 23
                    && m_struPlanCfg.struPlanCfg[i].struTimeSegment.struEndTime.byMinute == 59
                    && m_struPlanCfg.struPlanCfg[i].struTimeSegment.struEndTime.bySecond == 59)
                {
                    m_struPlanCfg.struPlanCfg[i].struTimeSegment.struEndTime.byHour = 24;
                    m_struPlanCfg.struPlanCfg[i].struTimeSegment.struEndTime.byMinute = 0;
                    m_struPlanCfg.struPlanCfg[i].struTimeSegment.struEndTime.bySecond = 0;
                }
            }
        }

        private void listViewTimeSegment_ItemSelectionChanged(object sender, ListViewItemSelectionChangedEventArgs e)
        {
            iItemIndex = e.ItemIndex;
            if (iItemIndex < 0)
            {
                return;
            }
            int iDate = comboBoxDateWP.SelectedIndex;
            int i = iDate*CHCNetSDK.MAX_TIMESEGMENT_V30 + iItemIndex;

            if (1 == m_struPlanCfg.struPlanCfg[i].byEnable)
            {
                checkBoxEnable.Checked = true;
            }
            else
            {
                checkBoxEnable.Checked = false;
            }
            comboBoxDoorStateMode.SelectedIndex = (int)m_struPlanCfg.struPlanCfg[i].byDoorStatus;
            comboBoxVerificationMode.SelectedIndex = (int)m_struPlanCfg.struPlanCfg[i].byVerifyMode;
            CHCNetSDK.NET_DVR_SIMPLE_DAYTIME struTime = new CHCNetSDK.NET_DVR_SIMPLE_DAYTIME();
            if (AcsDemoPublic.CheckDate(m_struPlanCfg.struPlanCfg[i].struTimeSegment.struBeginTime))
            {
                struTime = m_struPlanCfg.struPlanCfg[i].struTimeSegment.struBeginTime;
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
            if (AcsDemoPublic.CheckDate(m_struPlanCfg.struPlanCfg[i].struTimeSegment.struEndTime))
            {
                struTime = m_struPlanCfg.struPlanCfg[i].struTimeSegment.struEndTime;
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

        private void buttonAdd_Click(object sender, EventArgs e)
        {
            int iDateIndex = comboBoxDateWP.SelectedIndex;
            int iDate = iDateIndex * CHCNetSDK.MAX_TIMESEGMENT_V30 + iItemIndex;
            if (-1 == iDate)
            {
                MessageBox.Show("Please select the list!!!");
                return;
            }
            if (checkBoxEnable.Checked)
            {
                m_struPlanCfg.struPlanCfg[iDate].byEnable = 1;
            }
            else
            {
                m_struPlanCfg.struPlanCfg[iDate].byEnable = 0;
            }
            m_struPlanCfg.struPlanCfg[iDate].byVerifyMode = (byte)comboBoxVerificationMode.SelectedIndex;
            m_struPlanCfg.struPlanCfg[iDate].byDoorStatus = (byte)comboBoxDoorStateMode.SelectedIndex;
            m_struPlanCfg.struPlanCfg[iDate].struTimeSegment.struBeginTime.byHour = (byte)dateTimePickerStartTime.Value.Hour;
            m_struPlanCfg.struPlanCfg[iDate].struTimeSegment.struBeginTime.byMinute = (byte)dateTimePickerStartTime.Value.Minute;
            m_struPlanCfg.struPlanCfg[iDate].struTimeSegment.struBeginTime.bySecond = (byte)dateTimePickerStartTime.Value.Second;
            if (m_struPlanCfg.struPlanCfg[iDate].struTimeSegment.struBeginTime.byHour == 23
                && m_struPlanCfg.struPlanCfg[iDate].struTimeSegment.struBeginTime.byMinute == 59
                && m_struPlanCfg.struPlanCfg[iDate].struTimeSegment.struBeginTime.bySecond == 59)
            {
                m_struPlanCfg.struPlanCfg[iDate].struTimeSegment.struBeginTime.byHour = 24;
                m_struPlanCfg.struPlanCfg[iDate].struTimeSegment.struBeginTime.byMinute = 0;
                m_struPlanCfg.struPlanCfg[iDate].struTimeSegment.struBeginTime.bySecond = 0;
            }
            m_struPlanCfg.struPlanCfg[iDate].struTimeSegment.struEndTime.byHour = (byte)dateTimePickerEndTime.Value.Hour;
            m_struPlanCfg.struPlanCfg[iDate].struTimeSegment.struEndTime.byMinute = (byte)dateTimePickerEndTime.Value.Minute;
            m_struPlanCfg.struPlanCfg[iDate].struTimeSegment.struEndTime.bySecond = (byte)dateTimePickerEndTime.Value.Second;
            if (m_struPlanCfg.struPlanCfg[iDate].struTimeSegment.struBeginTime.byHour == 23
                && m_struPlanCfg.struPlanCfg[iDate].struTimeSegment.struBeginTime.byMinute == 59
                && m_struPlanCfg.struPlanCfg[iDate].struTimeSegment.struBeginTime.bySecond == 59)
            {
                m_struPlanCfg.struPlanCfg[iDate].struTimeSegment.struBeginTime.byHour = 24;
                m_struPlanCfg.struPlanCfg[iDate].struTimeSegment.struBeginTime.byMinute = 0;
                m_struPlanCfg.struPlanCfg[iDate].struTimeSegment.struBeginTime.bySecond = 0;
            }

            UpdateList();
        }

        private void buttonConfirm_Click(object sender, EventArgs e)
        {
            this.DialogResult = DialogResult.OK;
            return;
        }

        private void WeekPlan_Load(object sender, EventArgs e)
        {
            MultiLanguage.LoadLanguage(this);
        }
    }
}
