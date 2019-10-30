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

namespace AcsDemo.ProductAcs
{
    public partial class DeviceTimeCorrection : Form
    {
        public DeviceTimeCorrection()
        {
            InitializeComponent();
        }

        public int m_loginID = 0;
        public int iDevIndex = 0;
        private DeviceLogList g_formList = DeviceLogList.Instance();
        private void btnTimeCorr_Click(object sender, EventArgs e)
        {
            CHCNetSDK.NET_DVR_TIME CurTime = new CHCNetSDK.NET_DVR_TIME();
            CurTime.dwYear = dateTimePickerCorrection.Value.Year;
            CurTime.dwMonth = dateTimePickerCorrection.Value.Month;
            CurTime.dwDay = dateTimePickerCorrection.Value.Day;
            CurTime.dwHour = dateTimePickerCorrection.Value.Hour;
            CurTime.dwMinute = dateTimePickerCorrection.Value.Minute;
            CurTime.dwSecond = dateTimePickerCorrection.Value.Second;

            int dwSize = (int)Marshal.SizeOf(CurTime);
            IntPtr ptrCurTime = Marshal.AllocHGlobal(dwSize);
            Marshal.StructureToPtr(CurTime, ptrCurTime, true);
            Boolean res = CHCNetSDK.NET_DVR_SetDVRConfig(m_loginID, CHCNetSDK.NET_DVR_SET_TIMECFG, 0, ptrCurTime, (uint)dwSize);
            if(true==res)
            {
                g_formList.AddLog(iDevIndex, AcsDemoPublic.OPERATION_SUCC_T, "NET_DVR_SET_TIMECFG");
            }
            else
            {
                g_formList.AddLog(iDevIndex, AcsDemoPublic.OPERATION_FAIL_T, "NET_DVR_SET_TIMECFG");
            }
            Marshal.FreeHGlobal(ptrCurTime);
        }

        private void button1_Click(object sender, EventArgs e)
        {
            CHCNetSDK.NET_DVR_TIME DevTime = new CHCNetSDK.NET_DVR_TIME();
            int dwSize = (int)Marshal.SizeOf(DevTime);
            IntPtr PtrDevTime = Marshal.AllocHGlobal(dwSize);
            Marshal.StructureToPtr(DevTime, PtrDevTime, true);
            Boolean res = CHCNetSDK.NET_DVR_SetDVRConfig(m_loginID, CHCNetSDK.NET_DVR_GET_TIMECFG, 0, PtrDevTime, (uint)dwSize);

            if(true==res)
            {
                g_formList.AddLog(iDevIndex, AcsDemoPublic.OPERATION_SUCC_T, "NET_DVR_GET_TIMECFG");
                textBoxDevTime.Text = DevTime.dwYear.ToString() + ":" + DevTime.dwMonth.ToString() + ":" + DevTime.dwDay.ToString() + ":" +
                    DevTime.dwHour.ToString() + ":" + DevTime.dwMinute.ToString() + ":" + DevTime.dwSecond.ToString();
            }
            else
            {
                g_formList.AddLog(iDevIndex, AcsDemoPublic.OPERATION_FAIL_T, "NET_DVR_GET_TIMECFG");
            }
            Marshal.FreeHGlobal(PtrDevTime);
        }
    }
}
