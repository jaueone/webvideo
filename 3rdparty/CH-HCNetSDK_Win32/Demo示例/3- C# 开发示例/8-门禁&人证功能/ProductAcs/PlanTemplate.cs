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
    public partial class PlanTemplate : Form
    {
        public Int32 m_lUserID = -1;
        public Int32 m_iDeviceIndex = -1;
        public int m_iDeviceType = 0;
        public CHCNetSDK.NET_DVR_PLAN_TEMPLATE m_struTemplateCfg = new CHCNetSDK.NET_DVR_PLAN_TEMPLATE();
        public CHCNetSDK.NET_DVR_PLAN_TEMPLATE_COND m_struTemplateCond = new CHCNetSDK.NET_DVR_PLAN_TEMPLATE_COND();

        private DeviceLogList g_formList = DeviceLogList.Instance();
        private int iItemIndex = -1;

        public PlanTemplate()
        {
            InitializeComponent();
            m_struTemplateCfg.Init();
            m_struTemplateCond.Init();
            UpdateLstGroupNoP();
            this.SuspendLayout();
            this.columnHeader1.Name = "columnHeader1";
            this.columnHeader2.Name = "columnHeader2";
            this.ResumeLayout();
        }
        public void CustomRelayout()
        {
            if (m_iDeviceType != 2)
            {
                // only card right plan there are LocalControllerID parameters
                textBoxLocalControllerID.Hide();
                label1.Hide();
            }

            if (m_iDeviceType >= 0 && m_iDeviceType <= 2)
            {
                comboBoxDeviceTypeHG.SelectedIndex = m_iDeviceType;
            }
            comboBoxDeviceTypeHG.Enabled = false;
        }

        private void UpdateLstGroupNoP()
        {
            listViewTemplate.BeginUpdate();
            listViewTemplate.Items.Clear();
            int iItemNum = m_struTemplateCfg.dwHolidayGroupNo.Length;
            for (int i = 0; i < iItemNum; i++)
            {
                ListViewItem listItem = new ListViewItem();
                listItem.Text = (i + 1).ToString();
                listItem.SubItems.Add(m_struTemplateCfg.dwHolidayGroupNo[i].ToString());
                //listItem.SubItems.Add(m_struTemplateCfg.dwWeekPlanNo.ToString());
                listViewTemplate.Items.Add(listItem);
            }
            listViewTemplate.EndUpdate();
        }

        private void textBoxTemplateNumber_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar != '\b')//backspace 
            {
                if ((e.KeyChar < '0') || (e.KeyChar > '9'))//0-9 is permitted  
                {
                    e.Handled = true;
                }
            }
        }

        private void textBoxTemplateName_KeyPress(object sender, KeyPressEventArgs e)
        {
            //input template name is to long
            if (System.Text.Encoding.UTF8.GetBytes(textBoxTemplateName.Text).Length > CHCNetSDK.TEMPLATE_NAME_LEN)
            {
                // disable input
                if (e.KeyChar != '\b')//backspace 
                {
                    e.Handled = true;
                }
            }
        }

        private void textBoxHolidayGroupNo_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar != '\b')//backspace 
            {
                if ((e.KeyChar < '0') || (e.KeyChar > '9'))//0-9 is permitted  
                {
                    e.Handled = true;
                }
            }
        }

        private void textBoxWeekPlanNo_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar != '\b')//backspace 
            {
                if ((e.KeyChar < '0') || (e.KeyChar > '9'))//0-9 is permitted  
                {
                    e.Handled = true;
                }
            }
        }

        private void buttonUpdate_Click(object sender, EventArgs e)
        {
            if (-1 == iItemIndex)
            {
                MessageBox.Show("Please select the list!!!");
                return;
            }
            // limited input data guarantee parse success
            uint.TryParse(textBoxHolidayGroupNo.Text, out m_struTemplateCfg.dwHolidayGroupNo[iItemIndex]);

            UpdateLstGroupNoP();
        }

        private void buttonGetTemplate_Click(object sender, EventArgs e)
        {
            uint dwCommand = 0;
            string[] strCommand = { "NET_DVR_GET_DOOR_STATUS_PLAN_TEMPLATE", "NET_DVR_GET_VERIFY_PLAN_TEMPLATE", "NET_DVR_GET_CARD_RIGHT_PLAN_TEMPLATE_V50" };

            uint dwReturned = 0;
            string strTemp = null;
            uint dwSize = (uint)Marshal.SizeOf(m_struTemplateCfg);
            IntPtr ptrPlanCfg = Marshal.AllocHGlobal((int)dwSize);
            Marshal.StructureToPtr(m_struTemplateCfg, ptrPlanCfg, false);

            switch (comboBoxDeviceTypeHG.SelectedIndex)
            {
                case 0:
                case 1:
                    if (comboBoxDeviceTypeHG.SelectedIndex == 0)
                    {
                        dwCommand = (uint)CHCNetSDK.NET_DVR_GET_DOOR_STATUS_PLAN_TEMPLATE;
                    }
                    else
                    {
                        dwCommand = (uint)CHCNetSDK.NET_DVR_GET_VERIFY_PLAN_TEMPLATE;
                    }

                    int templateNumberIndex;
                    int.TryParse(textBoxTemplateNumber.Text, out templateNumberIndex);

                    if (!CHCNetSDK.NET_DVR_GetDVRConfig(m_lUserID, dwCommand, templateNumberIndex, ptrPlanCfg, dwSize, ref dwReturned))
                    {
                        Marshal.FreeHGlobal(ptrPlanCfg);
                        strTemp = string.Format("{0} FAIL, ERROR CODE {1}", strCommand[comboBoxDeviceTypeHG.SelectedIndex], CHCNetSDK.NET_DVR_GetLastError());
                        MessageBox.Show(strTemp);
                        return;
                    }
                    else
                    {
                        strTemp = string.Format("{0}", strCommand[comboBoxDeviceTypeHG.SelectedIndex]);
                        g_formList.AddLog(m_lUserID, AcsDemoPublic.OPERATION_SUCC_T, strTemp);
                    }
                    break;
                case 2:
                    dwCommand = (uint)CHCNetSDK.NET_DVR_GET_CARD_RIGHT_PLAN_TEMPLATE_V50;
                    uint dwConSize = (uint)Marshal.SizeOf(m_struTemplateCond);
                    m_struTemplateCond.dwSize = dwConSize;

                    // limited input data guarantee parse success
                    uint.TryParse(textBoxTemplateNumber.Text, out m_struTemplateCond.dwPlanTemplateNumber);
                    ushort.TryParse(textBoxLocalControllerID.Text, out m_struTemplateCond.wLocalControllerID);

                    IntPtr ptrPlanCon = Marshal.AllocHGlobal((int)dwConSize);
                    Marshal.StructureToPtr(m_struTemplateCond, ptrPlanCon, false);
                    IntPtr ptrDwReturned = Marshal.AllocHGlobal(4);

                    if (!CHCNetSDK.NET_DVR_GetDeviceConfig(m_lUserID, dwCommand, 1, ptrPlanCon, dwConSize, ptrDwReturned, ptrPlanCfg, dwSize))
                    {
                        Marshal.FreeHGlobal(ptrPlanCfg);
                        Marshal.FreeHGlobal(ptrPlanCon);
                        Marshal.FreeHGlobal(ptrDwReturned);
                        strTemp = string.Format("{0} FAIL, ERROR CODE {1}", strCommand[comboBoxDeviceTypeHG.SelectedIndex], CHCNetSDK.NET_DVR_GetLastError());
                        MessageBox.Show(strTemp);
                        return;
                    }
                    else
                    {
                        dwReturned = (uint)Marshal.ReadInt32(ptrDwReturned);
                        Marshal.FreeHGlobal(ptrDwReturned);
                        Marshal.FreeHGlobal(ptrPlanCon);
                        strTemp = string.Format("{0}", strCommand[comboBoxDeviceTypeHG.SelectedIndex]);
                        g_formList.AddLog(m_lUserID, AcsDemoPublic.OPERATION_SUCC_T, strTemp);
                    }

                    break;
                default:
                    Marshal.FreeHGlobal(ptrPlanCfg);
                    MessageBox.Show("unknown command");
                    return;
            }

            m_struTemplateCfg = (CHCNetSDK.NET_DVR_PLAN_TEMPLATE)Marshal.PtrToStructure(ptrPlanCfg, typeof(CHCNetSDK.NET_DVR_PLAN_TEMPLATE));

            if (1 == m_struTemplateCfg.byEnable)
            {
                checkBoxEnableHG.Checked = true;
            }
            else
            {
                checkBoxEnableHG.Checked = false;
            }

            Encoding ec = System.Text.Encoding.GetEncoding("gb2312");
            textBoxTemplateName.Text = ec.GetString(m_struTemplateCfg.byTemplateName);
            textBoxWeekPlanNo.Text = m_struTemplateCfg.dwWeekPlanNo.ToString();
            UpdateLstGroupNoP();

            Marshal.FreeHGlobal(ptrPlanCfg);

        }

        private void buttonSetTemplate_Click(object sender, EventArgs e)
        {
            uint dwCommand = 0;
            string[] strCommand = { "NET_DVR_SET_DOOR_STATUS_PLAN_TEMPLATE", "NET_DVR_SET_VERIFY_PLAN_TEMPLATE", "NET_DVR_SET_CARD_RIGHT_PLAN_TEMPLATE_V50" };

            uint dwReturned = 0;
            string strTemp = null;

            if (checkBoxEnableHG.Checked)
            {
                m_struTemplateCfg.byEnable = 1;
            }
            else
            {
                m_struTemplateCfg.byEnable = 0;
            }

            for (int i = 0; i < CHCNetSDK.TEMPLATE_NAME_LEN; i++)
            {
                m_struTemplateCfg.byTemplateName[i] = 0;
            }

            //m_struGroupCfg.byGroupName = System.Text.Encoding.UTF8.GetBytes(textBoxHolidayGroupNameHG.Text);
            Encoding ec = System.Text.Encoding.GetEncoding("gb2312");
            byte[] byTempName = ec.GetBytes(textBoxTemplateName.Text);
            for (int i = 0; i < byTempName.Length; i++)
            {
                if (i >= m_struTemplateCfg.byTemplateName.Length)
                {
                    break;
                }
                m_struTemplateCfg.byTemplateName[i] = byTempName[i];
            }

            int iItemIndex = listViewTemplate.Items.Count;
            for (int i = 0; i < iItemIndex; i++)
            {
                uint.TryParse(listViewTemplate.Items[i].SubItems[1].Text, out m_struTemplateCfg.dwHolidayGroupNo[i]);
            }
            uint.TryParse(textBoxWeekPlanNo.Text, out m_struTemplateCfg.dwWeekPlanNo);

            uint dwSize = (uint)Marshal.SizeOf(m_struTemplateCfg);
            m_struTemplateCfg.dwSize = dwSize;
            IntPtr ptrPlanCfg = Marshal.AllocHGlobal((int)dwSize);
            Marshal.StructureToPtr(m_struTemplateCfg, ptrPlanCfg, false);

            switch (comboBoxDeviceTypeHG.SelectedIndex)
            {
                case 0:
                case 1:
                    if (comboBoxDeviceTypeHG.SelectedIndex == 0)
                    {
                        dwCommand = (uint)CHCNetSDK.NET_DVR_SET_DOOR_STATUS_PLAN_TEMPLATE;
                    }
                    else
                    {
                        dwCommand = (uint)CHCNetSDK.NET_DVR_SET_VERIFY_PLAN_TEMPLATE;
                    }

                    int templateNumberIndex;
                    int.TryParse(textBoxTemplateNumber.Text, out templateNumberIndex);

                    if (!CHCNetSDK.NET_DVR_SetDVRConfig(m_lUserID, dwCommand, templateNumberIndex, ptrPlanCfg, dwSize))
                    {
                        Marshal.FreeHGlobal(ptrPlanCfg);
                        strTemp = string.Format("{0} FAIL, ERROR CODE {1}", strCommand[comboBoxDeviceTypeHG.SelectedIndex], CHCNetSDK.NET_DVR_GetLastError());
                        MessageBox.Show(strTemp);
                        return;
                    }
                    else
                    {
                        strTemp = string.Format("{0}", strCommand[comboBoxDeviceTypeHG.SelectedIndex]);
                        g_formList.AddLog(m_lUserID, AcsDemoPublic.OPERATION_SUCC_T, strTemp);
                    }
                    break;
                case 2:
                    dwCommand = (uint)CHCNetSDK.NET_DVR_SET_CARD_RIGHT_PLAN_TEMPLATE_V50;
                    uint dwConSize = (uint)Marshal.SizeOf(m_struTemplateCond);
                    m_struTemplateCond.dwSize = dwConSize;

                    // limited input data guarantee parse success
                    uint.TryParse(textBoxTemplateNumber.Text, out m_struTemplateCond.dwPlanTemplateNumber);
                    ushort.TryParse(textBoxLocalControllerID.Text, out m_struTemplateCond.wLocalControllerID);

                    IntPtr ptrPlanCon = Marshal.AllocHGlobal((int)dwConSize);
                    Marshal.StructureToPtr(m_struTemplateCond, ptrPlanCon, false);
                    IntPtr ptrDwReturned = Marshal.AllocHGlobal(4);



                    if (!CHCNetSDK.NET_DVR_SetDeviceConfig(m_lUserID, dwCommand, 1, ptrPlanCon, dwConSize, ptrDwReturned, ptrPlanCfg, dwSize))
                    {
                        Marshal.FreeHGlobal(ptrPlanCfg);
                        Marshal.FreeHGlobal(ptrPlanCon);
                        Marshal.FreeHGlobal(ptrDwReturned);
                        strTemp = string.Format("{0} FAIL, ERROR CODE {1}", strCommand[comboBoxDeviceTypeHG.SelectedIndex], CHCNetSDK.NET_DVR_GetLastError());
                        MessageBox.Show(strTemp);
                        return;
                    }
                    else
                    {
                        dwReturned = (uint)Marshal.ReadInt32(ptrDwReturned);
                        Marshal.FreeHGlobal(ptrDwReturned);
                        Marshal.FreeHGlobal(ptrPlanCon);
                        strTemp = string.Format("{0}", strCommand[comboBoxDeviceTypeHG.SelectedIndex]);
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

        private void listViewTemplate_ItemSelectionChanged(object sender, ListViewItemSelectionChangedEventArgs e)
        {
            iItemIndex = e.ItemIndex;
            if (iItemIndex < 0)
            {
                return;
            }

            textBoxHolidayGroupNo.Text = m_struTemplateCfg.dwHolidayGroupNo[iItemIndex].ToString();
        }

        private void PlanTemplate_Load(object sender, EventArgs e)
        {
            MultiLanguage.LoadLanguage(this);
        }
    }
}
