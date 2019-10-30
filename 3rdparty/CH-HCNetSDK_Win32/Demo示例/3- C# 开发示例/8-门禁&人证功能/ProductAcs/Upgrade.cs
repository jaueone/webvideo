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
using System.Diagnostics;
using System.Threading;
using AcsDemo.Language;
namespace AcsDemo.ProductAcs
{
    public partial class Upgrade : Form
    {
        public Int32 m_lUserID = -1;
        private Thread m_hGetProgress = null;
        public Int32 m_lUpgradeHandle = -1;
        private bool m_bExit = false;
        private delegate void UpgradeProgress(int nProgress);
        private UpgradeProgress m_fnUpdateProgress;

        private void SetProgress(int nProgress)
        {
            progressBarUpgrade.Value = nProgress;
            if (nProgress == 100)
            {
                MessageBox.Show("Upgrade Succ");
                m_lblRestart.Visible = true;
            }
        }

        public Upgrade()
        {
            InitializeComponent();
            m_fnUpdateProgress = new UpgradeProgress(SetProgress);
        }

        private void GetProgressThread()
        {
            int nProgress = 0;
            while (!m_bExit)
            {
                nProgress = CHCNetSDK.NET_DVR_GetUpgradeProgress(m_lUpgradeHandle);
                if (nProgress == -1)
                {
                    break;
                }
                else
                {
                    this.Invoke(m_fnUpdateProgress, nProgress);
                    if (nProgress == 100)
                    {
                        CHCNetSDK.NET_DVR_CloseUpgradeHandle(m_lUpgradeHandle);
                        break;
                    }
                    else
                    {
                        Thread.Sleep(1000);
                    }
                }
            }
        }

        private void m_btnUpdate_Click(object sender, EventArgs e)
        {
            int dwDevNo = 0;
            int dwSize = Marshal.SizeOf(dwDevNo);
            IntPtr pDevNo = Marshal.AllocHGlobal((Int32)dwSize);
            Marshal.StructureToPtr(dwDevNo, pDevNo, false);
            m_lUpgradeHandle = CHCNetSDK.NET_DVR_Upgrade_V40(m_lUserID, 4, m_txtBrowsePath.Text, pDevNo, dwSize);

            if (m_lUpgradeHandle < 0)
            {
                uint iError = CHCNetSDK.NET_DVR_GetLastError();
                MessageBox.Show("Upgrade failed");
            }
            else
            {
                progressBarUpgrade.Maximum = 100;
                progressBarUpgrade.Value = 0;
                if (m_hGetProgress == null)
                {
                    m_hGetProgress = new Thread(GetProgressThread);
                    m_hGetProgress.Start();
                }
            }
        }

        private void m_btnBrowse_Click(object sender, EventArgs e)
        {
            OpenFileDialog ofd = new OpenFileDialog();
            if (ofd.ShowDialog() == DialogResult.OK)
            {
                StreamReader sr = new StreamReader(ofd.FileName, Encoding.GetEncoding("GB2312"));
                m_txtBrowsePath.Clear();
                m_txtBrowsePath.Text = ofd.FileName;
                sr.Close();
            }
        }

        private void Upgrade_Load(object sender, EventArgs e)
        {
            MultiLanguage.LoadLanguage(this);
        }
    }
}
