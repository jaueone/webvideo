using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO;
using System.Runtime.InteropServices;
using AcsDemo.Public;

namespace AcsDemo.ProductAcs
{
    public partial class RemoteControlCapture : Form
    {
        public RemoteControlCapture()
        {
            InitializeComponent();
        }
        private DeviceLogList g_formList = DeviceLogList.Instance();
        public int m_lServerID = 0;
        public int m_dwGatawayNum = 0;
        public int index = 0;

        //图片质量的映射
        private short PicQualityMap(string PicQua)
        {
            short res = 0;
            switch(PicQua)
            {
                case "average":
                    res = 2;
                    break;
                case "better":
                    res=1;
                    break;
                case "best":
                    res = 0;
                    break;
                default:
                    res = 2;
                    break;
            }
            return res;
        }

        private void btnCapture_Click(object sender, EventArgs e)
        {
            CHCNetSDK.NET_DVR_JPEGPARA struJpegPara = new CHCNetSDK.NET_DVR_JPEGPARA();
            short temp = PicQualityMap( comboBoxPicQua.SelectedItem.ToString());
            struJpegPara.wPicSize = short.Parse(textBoxPicType.Text);
            int iChanShowNum = 0;

            string path = @"C:\\SdkLog";
            if(!Directory.Exists(path))
            {
                Directory.CreateDirectory(path);
            }


            string time = DateTime.Now.ToString();
            string PicPath = "C:\\SdkLog" +"\\"+"time"+".jpg";
            IntPtr PtrPicPath = Marshal.StringToHGlobalAnsi(PicPath);

            if(CHCNetSDK.NET_DVR_CaptureJPEGPicture(m_lServerID,iChanShowNum,ref struJpegPara,PtrPicPath))
            {
                string res = "Capture success save to C:\\SdkLog";
                g_formList.AddLog(m_lServerID, AcsDemoPublic.OPERATION_SUCC_T, res);
                MessageBox.Show(res);
            }
            else
            {
                uint x = CHCNetSDK.NET_DVR_GetLastError();
                g_formList.AddLog(m_lServerID, AcsDemoPublic.OPERATION_FAIL_T, "capture Fail");
                MessageBox.Show("capture fail");
            }
            Marshal.FreeHGlobal(PtrPicPath);

        }
    }
}
