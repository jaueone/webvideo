using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading;
using System.Windows.Forms;

using System.Runtime.InteropServices;

namespace AcsDemo.Public
{
    public partial class DeviceLogList : UserControl
    {
        private object locker = new object();
        private AcsDemoPublic.LOCAL_LOG_INFO m_struLogInfo = new AcsDemoPublic.LOCAL_LOG_INFO();
        private static DeviceLogList g_LogPanel = new DeviceLogList();

        public DeviceLogList()
        {
            InitializeComponent();
            listViewAllLog.Visible = true;
            listViewAlarmInfo.Visible = false;
        }

        public static DeviceLogList Instance()
        {
            return g_LogPanel;
        }

        //Change the ListView(Log List or Alarm List)
        public void showList(int iTag)
        {
            switch (iTag)
            {
                case 0:
                    listViewAllLog.Visible = true;
                    listViewAlarmInfo.Visible = false;
                    break;
                case 1:
                    listViewAllLog.Visible = false;
                    listViewAlarmInfo.Visible = true;
                    break;
                default:
                    listViewAllLog.Visible = true;
                    listViewAlarmInfo.Visible = false;
                    break;
            }
        }

        //Change the ListView(Log List or Alarm List)
        public void ClearList(int iTag)
        {
            switch (iTag)
            {
                case 0:
                    listViewAllLog.Items.Clear();
                    break;
                case 1:
                    listViewAlarmInfo.Items.Clear();
                    break;
                default:
                    listViewAllLog.Items.Clear();
                    listViewAlarmInfo.Items.Clear();
                    break;
            }
        }

        private delegate void AddListCallBack(AcsDemoPublic.LOCAL_LOG_INFO struLogInfo);

        //Output log to ListView
        private void AddList(AcsDemoPublic.LOCAL_LOG_INFO struLogInfo)
        {
            if (this.InvokeRequired)
            {
                AddListCallBack add = new AddListCallBack(AddList);
                this.BeginInvoke(add, new object[] { struLogInfo });
            }
            else
            {
                lock (locker)
                {
                    if (listViewAllLog.Items.Count >= 5000)
                    {
                        listViewAllLog.Items.Clear();
                    }
                    if (listViewAlarmInfo.Items.Count >= 5000)
                    {
                        listViewAlarmInfo.Items.Clear();
                    }
                    string strLogType = Properties.Resources.successLogType;
                    switch (struLogInfo.iLogType)
                    {
                        case AcsDemoPublic.ALARM_INFO_T:
                            strLogType = Properties.Resources.alarmLogType;
                            ListViewItem listItem1 = new ListViewItem();
                            listItem1.Text = struLogInfo.strTime;
                            listItem1.SubItems.Add(struLogInfo.strLogInfo);
                            listItem1.SubItems.Add(struLogInfo.strDevInfo);
                            listItem1.SubItems.Add(struLogInfo.strSavePath);
                            listViewAlarmInfo.Items.Insert(0, listItem1);
                            break;
                        case AcsDemoPublic.OPERATION_SUCC_T:
                            strLogType = Properties.Resources.successLogType;
                            ListViewItem listItem2 = new ListViewItem();
                            listItem2.Text = struLogInfo.strTime;
                            listItem2.SubItems.Add(strLogType);
                            listItem2.SubItems.Add(struLogInfo.strLogInfo);
                            listItem2.SubItems.Add(struLogInfo.strDevInfo);
                            listItem2.SubItems.Add(struLogInfo.strErrInfo);
                            listViewAllLog.Items.Insert(0, listItem2);
                            break;
                        case AcsDemoPublic.OPERATION_FAIL_T:
                            strLogType = Properties.Resources.failLogType;
                            ListViewItem listItem3 = new ListViewItem();
                            listItem3.Text = struLogInfo.strTime;
                            listItem3.SubItems.Add(strLogType);
                            listItem3.SubItems.Add(struLogInfo.strLogInfo);
                            listItem3.SubItems.Add(struLogInfo.strDevInfo);
                            listItem3.SubItems.Add(struLogInfo.strErrInfo);
                            listViewAllLog.Items.Insert(0, listItem3);
                            break;
                        case AcsDemoPublic.PLAY_SUCC_T:
                            strLogType = Properties.Resources.successLogType;
                            ListViewItem listItem4 = new ListViewItem();
                            listItem4.Text = struLogInfo.strTime;
                            listItem4.SubItems.Add(strLogType);
                            listItem4.SubItems.Add(struLogInfo.strLogInfo);
                            listItem4.SubItems.Add(struLogInfo.strDevInfo);
                            listItem4.SubItems.Add(struLogInfo.strErrInfo);
                            listViewAllLog.Items.Insert(0, listItem4);
                            break;
                        case AcsDemoPublic.PLAY_FAIL_T:
                            strLogType = Properties.Resources.failLogType;
                            ListViewItem listItem5 = new ListViewItem();
                            listItem5.Text = struLogInfo.strTime;
                            listItem5.SubItems.Add(strLogType);
                            listItem5.SubItems.Add(struLogInfo.strLogInfo);
                            listItem5.SubItems.Add(struLogInfo.strDevInfo);
                            listItem5.SubItems.Add(struLogInfo.strErrInfo);
                            listViewAllLog.Items.Insert(0, listItem5);
                            break;
                        default:
                            strLogType = Properties.Resources.failLogType;
                            ListViewItem listItem6 = new ListViewItem();
                            listItem6.Text = struLogInfo.strTime;
                            listItem6.SubItems.Add(strLogType);
                            listItem6.SubItems.Add(struLogInfo.strLogInfo);
                            listItem6.SubItems.Add(struLogInfo.strDevInfo);
                            listItem6.SubItems.Add(struLogInfo.strErrInfo);
                            listViewAllLog.Items.Insert(0, listItem6);
                            break;
                    }
                }
            }
        }

        public void DoubleEvent(object sender, EventArgs e)
        {
            string strText=null;
            string cloum1 = string.Format(listViewAlarmInfo.SelectedItems[0].SubItems[0].Text);
            string cloum2 = string.Format(listViewAlarmInfo.SelectedItems[0].SubItems[1].Text);
            string cloum3 = string.Format(listViewAlarmInfo.SelectedItems[0].SubItems[2].Text);
            string cloum4 = string.Format(listViewAlarmInfo.SelectedItems[0].SubItems[3].Text);

            strText = string.Format("Time:{0}\r\n\r\nAlarmInformation:{1}\r\n\r\nDeviceInformation:{2}\r\n\r\nSavePath:{3}", cloum1, cloum2, cloum3, cloum4);
            AlarmInfoShow alarmInfoShow = new AlarmInfoShow();
            alarmInfoShow.strInfo = strText;
            alarmInfoShow.showText();
            if (cloum4.Length > 0)
            {
                PicAlarmInfo pic = new PicAlarmInfo();
                pic.path= cloum4;
                pic.showPic();
                
            }
        }

        //Write the log message
        public void AddLog(int iDeviceIndex, int iLogType, string strFormat)
        {
            DeviceTree g_deviceTree = DeviceTree.Instance();
            DateTime curTime = DateTime.Now;
            string strTime = null;
            string strlogType = Properties.Resources.failLogType;
            string strLogInfo = null;
            string strDevInfo = null;
            string strErrInfo = null;
            string strSavePath = null;
            string strLog = null;
            int iErrorCode = 0;
            strTime = curTime.ToString();
            strLogInfo = strFormat;
            if (iDeviceIndex != -1 && iDeviceIndex < 512)
            {
                if (null == g_deviceTree.GetCurDeviceIp())
                {
                    strDevInfo = null;
                }
                else
                {
                    strDevInfo = string.Format("{0}-{1}", g_deviceTree.GetCurDeviceIp(), g_deviceTree.GetCurLocalNodeName());
                }
            }
            switch (iLogType)
            {
                case AcsDemoPublic.OPERATION_SUCC_T:
                    strErrInfo = "";
                    strlogType = Properties.Resources.successLogType;
                    break;
                case AcsDemoPublic.PLAY_SUCC_T:
                    strErrInfo = "";
                    strlogType = Properties.Resources.successLogType;
                    break;
                case AcsDemoPublic.PLAY_FAIL_T:
                    //strErrInfo = "PLAY_M4 Error!!!";
                    break;
                case AcsDemoPublic.OPERATION_FAIL_T:
                    IntPtr ptrFail = CHCNetSDK.NET_DVR_GetErrorMsg(ref iErrorCode);
                    string strFail = Marshal.PtrToStringAnsi(ptrFail);
                    strErrInfo = string.Format("err{0}:{1}", CHCNetSDK.NET_DVR_GetLastError(), strFail);
                    break;
                default:
                    IntPtr ptrTemp = CHCNetSDK.NET_DVR_GetErrorMsg(ref iErrorCode);
                    string strTemp = Marshal.PtrToStringAnsi(ptrTemp);
                    strErrInfo = string.Format("err{0}:{1}", CHCNetSDK.NET_DVR_GetLastError(), strTemp);
                    break;
            }

            if (iLogType == AcsDemoPublic.ALARM_INFO_T)
            {
                int index=strLogInfo.IndexOf("SavePath");
                if(index>-1)
                {
                    strSavePath = strLogInfo.Substring(index+9);
                    strLogInfo=strLogInfo.Substring(0,index);
                }
                strLog = string.Format("{0}{1}\n", strTime, strLogInfo);
                //log file path undetermined, temporarily not implementation 
            }
            else
            {
                strLog = string.Format("{0}{1}{2}{3}{4}", strTime, strlogType, strLogInfo, strDevInfo, strErrInfo);

            }

            m_struLogInfo.iLogType = iLogType;
            m_struLogInfo.strTime = strTime;
            m_struLogInfo.strLogInfo = strLogInfo;
            m_struLogInfo.strDevInfo = strDevInfo;
            m_struLogInfo.strErrInfo = strErrInfo;
            m_struLogInfo.strSavePath = strSavePath;
            AddList(m_struLogInfo);
        }
    }
}
