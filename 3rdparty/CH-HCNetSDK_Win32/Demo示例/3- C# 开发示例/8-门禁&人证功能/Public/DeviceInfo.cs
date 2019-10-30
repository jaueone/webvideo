using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace AcsDemo.Public
{
    public partial class DeviceInfo : Form
    {
        private DeviceTree g_deviceTree = DeviceTree.Instance();
        private DeviceLogList g_formList = DeviceLogList.Instance();
        private int m_iDeviceIndex = -1;

        public DeviceInfo()
        {
            InitializeComponent();
        }
        public void GetDeviceInfo(int iDeviceIndex)
        {
            m_iDeviceIndex = iDeviceIndex;
            if (-1 == g_deviceTree.g_struDeviceInfo[m_iDeviceIndex].iDeviceChanNum)
            {
                textBoxDoorNum.Text = "16";
                textBoxPort.Text = "8000";
            }
            else
            {
                textBoxNodeName.Text = g_deviceTree.g_struDeviceInfo[m_iDeviceIndex].chLocalNodeName;
                textBoxUserName.Text = g_deviceTree.g_struDeviceInfo[m_iDeviceIndex].chLoginUserName;
                textBoxPassword.Text = g_deviceTree.g_struDeviceInfo[m_iDeviceIndex].chLoginPwd;
                textBoxDoorNum.Text = g_deviceTree.g_struDeviceInfo[m_iDeviceIndex].iDoorNum.ToString();
                textBoxIpAddress.Text = g_deviceTree.g_struDeviceInfo[m_iDeviceIndex].chDeviceIP;
                textBoxMulticastGroupAddress.Text = g_deviceTree.g_struDeviceInfo[m_iDeviceIndex].chDeviceMultiIP;
                textBoxPort.Text = g_deviceTree.g_struDeviceInfo[m_iDeviceIndex].lDevicePort.ToString();
                textBoxDeviceType.Text = g_deviceTree.g_struDeviceInfo[m_iDeviceIndex].chDeviceName;
                textBoxSerialNumber.Text = g_deviceTree.g_struDeviceInfo[m_iDeviceIndex].chSerialNumber;
            }
        }

        private void buttonConfirm_Click(object sender, EventArgs e)
        {
            g_deviceTree.g_struDeviceInfo[m_iDeviceIndex].chLocalNodeName = textBoxNodeName.Text;
            g_deviceTree.g_struDeviceInfo[m_iDeviceIndex].chLoginUserName = textBoxUserName.Text;
            g_deviceTree.g_struDeviceInfo[m_iDeviceIndex].chLoginPwd = textBoxPassword.Text;
            g_deviceTree.g_struDeviceInfo[m_iDeviceIndex].chDeviceIP = textBoxIpAddress.Text;
            int.TryParse(textBoxPort.Text, out g_deviceTree.g_struDeviceInfo[m_iDeviceIndex].lDevicePort);

            if (0 == g_deviceTree.g_struDeviceInfo[m_iDeviceIndex].chLocalNodeName.Length)
            {
                g_deviceTree.g_struDeviceInfo[m_iDeviceIndex].chLocalNodeName = g_deviceTree.g_struDeviceInfo[m_iDeviceIndex].chDeviceIP;
            }
            this.DialogResult = DialogResult.OK;
        }

        private void buttonCancel_Click(object sender, EventArgs e)
        {
            this.DialogResult = DialogResult.Cancel;
        }

    }
}
