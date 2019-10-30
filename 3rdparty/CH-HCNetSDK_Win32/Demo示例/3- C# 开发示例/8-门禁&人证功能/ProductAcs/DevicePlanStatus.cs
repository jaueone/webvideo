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
    public partial class DevicePlanStatus : Form
    {
        public Int32 m_lUserID = -1;
        public Int32 m_iDeviceIndex = -1;
        public int m_iDeviceType = 0;
        public int m_iIndex = -1;
        public CHCNetSDK.NET_DVR_DOOR_STATUS_PLAN m_struStatusPlan = new CHCNetSDK.NET_DVR_DOOR_STATUS_PLAN();
        public CHCNetSDK.NET_DVR_CARD_READER_PLAN m_struCardReaderPlan = new CHCNetSDK.NET_DVR_CARD_READER_PLAN();

        private DeviceLogList g_formList = DeviceLogList.Instance();

        public DevicePlanStatus()
        {
            InitializeComponent();
            m_struStatusPlan.Init();
            m_struCardReaderPlan.Init();
        }

        public void CustomRelayout()
        {
            comboBoxDeviceType.Enabled = false;
            textBoxNumber.Enabled = false;
            textBoxNumber.Text = m_iIndex.ToString();
            if (m_iIndex <= 0)
            {
                textBoxNumber.Enabled = true;
                MessageBox.Show("please set a door/card reader");
            }
            comboBoxDeviceType.SelectedIndex = m_iDeviceType;
        }

        private void buttonGetCurrentPlan_Click(object sender, EventArgs e)
        {
            uint dwCommand = 0;
            uint dwReturned = 0;
            string strTemp = null;
            string[] strCommand = { "NET_DVR_GET_DOOR_STATUS_PLAN", "NET_DVR_GET_CARD_READER_PLAN" };

            switch (comboBoxDeviceType.SelectedIndex)
            {
                case 0:
                    dwCommand = (uint)CHCNetSDK.NET_DVR_GET_DOOR_STATUS_PLAN;
                    uint dwSize = (uint)Marshal.SizeOf(m_struStatusPlan);
                    IntPtr ptrPlanCfg = Marshal.AllocHGlobal((int)dwSize);
                    Marshal.StructureToPtr(m_struStatusPlan, ptrPlanCfg, false);

                    int doorIndex = 1;
                    int.TryParse(textBoxNumber.Text, out doorIndex);

                    if (!CHCNetSDK.NET_DVR_GetDVRConfig(m_lUserID, dwCommand, doorIndex, ptrPlanCfg, dwSize, ref dwReturned))
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

                    m_struStatusPlan = (CHCNetSDK.NET_DVR_DOOR_STATUS_PLAN)Marshal.PtrToStructure(ptrPlanCfg, typeof(CHCNetSDK.NET_DVR_DOOR_STATUS_PLAN));
                   
                    textBoxPlanTemplateNumber.Text = m_struStatusPlan.dwTemplateNo.ToString(); 

                    Marshal.FreeHGlobal(ptrPlanCfg);
                    break;
                case 1:
                    dwCommand = (uint)CHCNetSDK.NET_DVR_GET_CARD_READER_PLAN;
                    uint dwCardReadeSize = (uint)Marshal.SizeOf(m_struCardReaderPlan);
                    IntPtr ptrCardReadePlanCfg = Marshal.AllocHGlobal((int)dwCardReadeSize);
                    Marshal.StructureToPtr(m_struStatusPlan, ptrCardReadePlanCfg, false);

                    int cardReaderIndex = 1;
                    int.TryParse(textBoxNumber.Text, out cardReaderIndex);

                    if (!CHCNetSDK.NET_DVR_GetDVRConfig(m_lUserID, dwCommand, cardReaderIndex, ptrCardReadePlanCfg, dwCardReadeSize, ref dwReturned))
                    {
                        Marshal.FreeHGlobal(ptrCardReadePlanCfg);
                        strTemp = string.Format("{0} FAIL, ERROR CODE {1}", strCommand[comboBoxDeviceType.SelectedIndex], CHCNetSDK.NET_DVR_GetLastError());
                        MessageBox.Show(strTemp);
                        return;
                    }
                    else
                    {
                        strTemp = string.Format("{0}", strCommand[comboBoxDeviceType.SelectedIndex]);
                        g_formList.AddLog(m_lUserID, AcsDemoPublic.OPERATION_SUCC_T, strTemp);
                    }

                    m_struCardReaderPlan = (CHCNetSDK.NET_DVR_CARD_READER_PLAN)Marshal.PtrToStructure(ptrCardReadePlanCfg, typeof(CHCNetSDK.NET_DVR_CARD_READER_PLAN));
                   
                    textBoxPlanTemplateNumber.Text = m_struStatusPlan.dwTemplateNo.ToString();

                    Marshal.FreeHGlobal(ptrCardReadePlanCfg);
                    break;
                default:
                    MessageBox.Show("unknown command");
                    return;
            }
        }

        private void buttonSetCurrentPlan_Click(object sender, EventArgs e)
        {
            uint dwCommand = 0;
            string[] strCommand = { "NET_DVR_SET_DOOR_STATUS_PLAN", "NET_DVR_SET_CARD_READER_PLAN" };
            string strTemp = null;

           uint.TryParse(textBoxPlanTemplateNumber.Text, out m_struStatusPlan.dwTemplateNo);
           uint.TryParse(textBoxPlanTemplateNumber.Text, out m_struCardReaderPlan.dwTemplateNo);

            switch (comboBoxDeviceType.SelectedIndex)
            {
                case 0:
                    dwCommand = (uint)CHCNetSDK.NET_DVR_SET_DOOR_STATUS_PLAN;
                    uint dwSize = (uint)Marshal.SizeOf(m_struStatusPlan);
                    IntPtr ptrPlanCfg = Marshal.AllocHGlobal((int)dwSize);
                    m_struStatusPlan.dwSize = dwSize;
                    Marshal.StructureToPtr(m_struStatusPlan, ptrPlanCfg, false);

                    int doorIndex = 1;
                    int.TryParse(textBoxNumber.Text, out doorIndex);

                    if (!CHCNetSDK.NET_DVR_SetDVRConfig(m_lUserID, dwCommand, doorIndex, ptrPlanCfg, dwSize))
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

                    Marshal.FreeHGlobal(ptrPlanCfg);
                    break;
                case 1:
                    dwCommand = (uint)CHCNetSDK.NET_DVR_SET_CARD_READER_PLAN;
                    uint dwCardReadeSize = (uint)Marshal.SizeOf(m_struCardReaderPlan);
                    m_struCardReaderPlan.dwSize = dwCardReadeSize;
                    IntPtr ptrCardReadePlanCfg = Marshal.AllocHGlobal((int)dwCardReadeSize);
                    Marshal.StructureToPtr(m_struCardReaderPlan, ptrCardReadePlanCfg, false);

                    int cardReaderIndex = 1;
                    int.TryParse(textBoxNumber.Text, out cardReaderIndex);

                    if (!CHCNetSDK.NET_DVR_SetDVRConfig(m_lUserID, dwCommand, cardReaderIndex, ptrCardReadePlanCfg, dwCardReadeSize))
                    {
                        Marshal.FreeHGlobal(ptrCardReadePlanCfg);
                        strTemp = string.Format("{0} FAIL, ERROR CODE {1}", strCommand[comboBoxDeviceType.SelectedIndex], CHCNetSDK.NET_DVR_GetLastError());
                        MessageBox.Show(strTemp);
                        return;
                    }
                    else
                    {
                        strTemp = string.Format("{0}", strCommand[comboBoxDeviceType.SelectedIndex]);
                        g_formList.AddLog(m_lUserID, AcsDemoPublic.OPERATION_SUCC_T, strTemp);
                    }
                    Marshal.FreeHGlobal(ptrCardReadePlanCfg);
                    break;
                default:
                    MessageBox.Show("unknown command");
                    return;
            }
        }

        private void DevicePlanStatus_Load(object sender, EventArgs e)
        {
            MultiLanguage.LoadLanguage(this);
        }
    }
}
