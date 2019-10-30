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
namespace AcsDemo.ProductAcs
{
    public partial class UserManagement : Form
    {       
        public Int32 m_lUserID = -1;
        private uint iLastErr = 0;
        private string strErr;
        private CheckBox[] checkBox;
        public CHCNetSDK.NET_DVR_ALARM_DEVICE_USER m_strAlarmCfg;

        public UserManagement()
        {
            InitializeComponent();
            checkBox = new CheckBox[]
            {
                checkBoxLog,
                checkBoxStart,
                checkBoxSet,
                checkBoxGet,
                checkBoxResume,
                checkBoxAlarm,
                checkBoxPTZ,
                checkBoxUpgrade,
                checkBoxAlarmOut,
                checkBoxSerial,
                checkBoxKey,
                checkBoxVoice,
                checkBoxFarControl,
                checkBoxDisk,
                checkBoxFormat,
                checkBoxSimulate

            };
        }

        private void AlarmGet_Click(object sender, EventArgs e)
        {
            if (m_lUserID < 0)
            {
                MessageBox.Show("Please Login!");
                return;
            }
            Int32 iSize = Marshal.SizeOf(m_strAlarmCfg);
            IntPtr ptrAlarmCfg = Marshal.AllocHGlobal(iSize);
            Marshal.StructureToPtr(m_strAlarmCfg, ptrAlarmCfg, false);
            if (!CHCNetSDK.NET_DVR_GetAlarmDeviceUser(m_lUserID, 1, ref m_strAlarmCfg))
            {
                iLastErr = CHCNetSDK.NET_DVR_GetLastError();
                strErr = "NET_DVR_GetAlarmDeviceUser failed,error code=" + iLastErr;
                MessageBox.Show(strErr);
            }
            else
            {
                textBoxUserName.Text = System.Text.Encoding.UTF8.GetString(m_strAlarmCfg.sUserName);
                textBoxPassword.Text = System.Text.Encoding.UTF8.GetString(m_strAlarmCfg.sPassword);
                textBoxUserIP.Text = m_strAlarmCfg.struUserIP.sIpV4;
                if (m_strAlarmCfg.byAMCAddr == null)
                {
                    m_strAlarmCfg.byAMCAddr.CopyTo(System.Text.Encoding.Default.GetBytes("00:00:00:00:00:00"), 0);
                }
                textBoxMac.Text = System.Text.Encoding.UTF8.GetString(m_strAlarmCfg.byAMCAddr);

                if (m_strAlarmCfg.byUserType == 0)
                {
                    comboBoxUserType.SelectedIndex = 0;
                }
                else if (m_strAlarmCfg.byUserType == 1)
                {
                    comboBoxUserType.SelectedIndex = 1;
                }

                if (m_strAlarmCfg.byAlarmOnRight == 1)
                {
                    checkBoxAlarmOn.Checked = true;
                }
                else
                {
                    checkBoxAlarmOn.Checked = false;
                }

                if (m_strAlarmCfg.byAlarmOffRight == 1)
                {
                    checkBoxAlarmOff.Checked = true;
                }
                else
                {
                    checkBoxAlarmOff.Checked = false;
                }

                if (m_strAlarmCfg.byBypassRight == 1)
                {
                    checkBoxByPass.Checked = true;
                }
                else
                {
                    checkBoxByPass.Checked = false;
                }

                for (int i = 0; i < checkBox.Length; i++)
                {
                    if (m_strAlarmCfg.byOtherRight[i] == 1)
                    {
                        checkBox[i].Checked = true;
                    }
                    else
                    {
                        checkBox[i].Checked = false;
                    }
                }
            }
        }

        private void buttonSet_Click(object sender, EventArgs e)
        {
            if (m_lUserID < 0)
            {
                MessageBox.Show("Please login!");
                return;
            }
            byte[] sUserName = System.Text.Encoding.Default.GetBytes(textBoxUserName.Text);
            m_strAlarmCfg.sUserName = new byte[32];
            sUserName.CopyTo(m_strAlarmCfg.sUserName, 0);

            byte[] sOldPassword = System.Text.Encoding.Default.GetBytes(textBoxOldPassword.Text);
            m_strAlarmCfg.sOriginalPassword = new byte[16];
            sOldPassword.CopyTo(m_strAlarmCfg.sOriginalPassword, 0);

            byte[] sPassword = System.Text.Encoding.Default.GetBytes(textBoxPassword.Text);
            m_strAlarmCfg.sPassword = new byte[16];
            sPassword.CopyTo(m_strAlarmCfg.sPassword, 0);

            string sIPV4 = textBoxUserIP.Text.ToString();
            m_strAlarmCfg.struUserIP.sIpV4 = sIPV4;

            byte[] byMac = System.Text.Encoding.Default.GetBytes(textBoxMac.Text);
            m_strAlarmCfg.byAMCAddr = new byte[6];
            byMac.CopyTo(m_strAlarmCfg.byAMCAddr, 0);

            if (comboBoxUserType.SelectedIndex == 0)
            {
                m_strAlarmCfg.byUserType = 0;
            }
            else
            {
                m_strAlarmCfg.byUserType = 1;
            }
            if (checkBoxAlarmOn.Checked)
            {
                m_strAlarmCfg.byAlarmOnRight = 1;
            }
            else
            {
                m_strAlarmCfg.byAlarmOnRight = 0;
            }

            if (checkBoxAlarmOff.Checked)
            {
                m_strAlarmCfg.byAlarmOffRight = 1;
            }
            else
            {
                m_strAlarmCfg.byAlarmOffRight = 0;
            }

            if (checkBoxByPass.Checked)
            {
                m_strAlarmCfg.byBypassRight = 1;
            }
            else
            {
                m_strAlarmCfg.byBypassRight = 0;
            }

            for (int i = 0; i < checkBox.Length; i++)
            {
                if (checkBox[i].Checked)
                {
                    m_strAlarmCfg.byOtherRight[i] = 1;
                }
                else
                {
                    m_strAlarmCfg.byOtherRight[i] = 0;
                }
            }

            Int32 iSize = Marshal.SizeOf(m_strAlarmCfg);
            IntPtr ptrAlarmCfg = Marshal.AllocHGlobal(iSize);
            Marshal.StructureToPtr(m_strAlarmCfg, ptrAlarmCfg, false);
            if (!CHCNetSDK.NET_DVR_SetAlarmDeviceUser(m_lUserID, 1, ref m_strAlarmCfg))
            {
                iLastErr = CHCNetSDK.NET_DVR_GetLastError();
                strErr = "NET_DVR_GetAlarmDeviceUser failed,error code=" + iLastErr;
                MessageBox.Show(strErr);
            }
            else
            {
                MessageBox.Show("Set Param Sussess!");
            }
        }

        private void UserManagement_Load(object sender, EventArgs e)
        {
            MultiLanguage.LoadLanguage(this);
        }

    }
}
