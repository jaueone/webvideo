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
    public partial class ClearACSParameters : Form
    {
        private DeviceLogList g_formList = DeviceLogList.Instance();
        public int m_lUserID = -1;
        public int m_iDevIndex = -1;

        private uint m_bDoorStatusWeekPlan;
        private uint m_bVerifyWeekPaln;
        private uint m_bCardRightWeekPlan;
        private uint m_bDoorStatusHolidayPlan;
        private uint m_bVerifyHolidayPaln;
        private uint m_bCardRightHoliDayPlan;
        private uint m_bDoorStatusHolidayGroup;
        private uint m_bVerifyHolidayGroup;
        private uint m_bCardRightHolidayGroup;
        private uint m_bDoorStatusPlanTemplate;
        private uint m_bVerifyPlanTemplate;
        private uint m_bCardRightPlanTemplate;
        private uint m_bCard;
        private uint m_bGroup;
        private uint m_bAntiSneakCfg;
        private uint m_bDoorEventCardLinkge;
        private uint m_bDoorCardPasswdCfg;
        private uint m_bPersonStatistical;

        public ClearACSParameters()
        {
            InitializeComponent();
        }

        private void buttonClear_Click(object sender, EventArgs e)
        {
            CHCNetSDK.NET_DVR_ACS_PARAM_TYPE struAcsParam = new CHCNetSDK.NET_DVR_ACS_PARAM_TYPE();
            struAcsParam.byRes = new byte[30];

            uint dwSize = (uint)Marshal.SizeOf(struAcsParam);
            struAcsParam.dwSize = dwSize;
            IntPtr ptrAcsParam = Marshal.AllocHGlobal((int)dwSize);

            updateCheckStatus();
            struAcsParam.dwParamType |= (m_bDoorStatusWeekPlan << 0);
            struAcsParam.dwParamType |= (m_bVerifyWeekPaln << 1);
            struAcsParam.dwParamType |= (m_bCardRightWeekPlan << 2);
            struAcsParam.dwParamType |= (m_bDoorStatusHolidayPlan << 3);
            struAcsParam.dwParamType |= (m_bVerifyHolidayPaln << 4);
            struAcsParam.dwParamType |= (m_bCardRightHoliDayPlan << 5);
            struAcsParam.dwParamType |= (m_bDoorStatusHolidayGroup << 6);
            struAcsParam.dwParamType |= (m_bVerifyHolidayGroup << 7);
            struAcsParam.dwParamType |= (m_bCardRightHolidayGroup << 8);
            struAcsParam.dwParamType |= (m_bDoorStatusPlanTemplate << 9);
            struAcsParam.dwParamType |= (m_bVerifyPlanTemplate << 10);
            struAcsParam.dwParamType |= (m_bCardRightPlanTemplate << 11);
            struAcsParam.dwParamType |= (m_bCard << 12);
            struAcsParam.dwParamType |= (m_bGroup << 13);
            struAcsParam.dwParamType |= (m_bAntiSneakCfg << 14);
            struAcsParam.dwParamType |= (m_bDoorEventCardLinkge << 15);
            struAcsParam.dwParamType |= (m_bDoorCardPasswdCfg << 16);
            struAcsParam.dwParamType |= (m_bPersonStatistical << 17);

            if (!ushort.TryParse(textBoxId.Text, out struAcsParam.wLocalControllerID))
            {
                //set a default value
                MessageBox.Show("Open Duration value should be 0-64!, here has set default 0");
                struAcsParam.wLocalControllerID = 0;
            }

            Marshal.StructureToPtr(struAcsParam, ptrAcsParam, false);

            if (!CHCNetSDK.NET_DVR_RemoteControl(m_lUserID, CHCNetSDK.NET_DVR_CLEAR_ACS_PARAM, ptrAcsParam, dwSize))
            {
                g_formList.AddLog(m_iDevIndex, AcsDemoPublic.OPERATION_FAIL_T, "NET_DVR_CLEAR_ACS_PARAM FAIL");
                Marshal.FreeHGlobal(ptrAcsParam);
                return;
            }
            else
            {
                Marshal.FreeHGlobal(ptrAcsParam);
                g_formList.AddLog(m_iDevIndex, AcsDemoPublic.OPERATION_SUCC_T, "NET_DVR_CLEAR_ACS_PARAM SUCC");
            }
        }

        private void updateCheckStatus()
        {
            m_bDoorStatusWeekPlan =(uint)((checkBoxDoorWp.Checked == true) ? 1 : 0);
            m_bVerifyWeekPaln = (uint)((checkBoxCardWp.Checked == true) ? 1 : 0);
            m_bCardRightWeekPlan = (uint)((checkBoxRightWp.Checked == true) ? 1 : 0);
            m_bDoorStatusHolidayPlan = (uint)((checkBoxDoorHp.Checked == true) ? 1 : 0);
            m_bVerifyHolidayPaln = (uint)((checkBoxCardHp.Checked == true) ? 1 : 0);
            m_bCardRightHoliDayPlan = (uint)((checkBoxRightHp.Checked == true) ? 1 : 0);
            m_bDoorStatusHolidayGroup = (uint)((checkBoxDoorGp.Checked == true) ? 1 : 0);
            m_bVerifyHolidayGroup = (uint)((checkBoxCardGp.Checked == true) ? 1 : 0);
            m_bCardRightHolidayGroup = (uint)((checkBoxRightGp.Checked == true) ? 1 : 0);
            m_bDoorStatusPlanTemplate = (uint)((checkBoxDoorTp.Checked == true) ? 1 : 0);
            m_bVerifyPlanTemplate = (uint)((checkBoxCardTp.Checked == true) ? 1 : 0);
            m_bCardRightPlanTemplate = (uint)((checkBoxRightTp.Checked == true) ? 1 : 0);
            m_bCard = (uint)((checkBoxCardCfg.Checked == true) ? 1 : 0);
            m_bGroup = (uint)((checkBoxGroupCFG.Checked == true) ? 1 : 0);
            m_bAntiSneakCfg = (uint)((checkBoxAntiSneakCfg.Checked == true) ? 1 : 0);
            m_bDoorEventCardLinkge = (uint)((checkBoxEventLinkageCard.Checked == true) ? 1 : 0);
            m_bDoorCardPasswdCfg = (uint)((checkBoxOpenPsw.Checked == true) ? 1 : 0);
            m_bPersonStatistical = (uint)((checkBoxStatistic.Checked == true) ? 1 : 0);
        }

        private void ClearACSParameters_Load(object sender, EventArgs e)
        {
            MultiLanguage.LoadLanguage(this);
        }
    }
}
