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
using AcsDemo.Language;

namespace AcsDemo.ProductAcs
{
    public partial class UserCardMgr : Form
    {
        public CHCNetSDK.NET_DVR_CARD_CFG_SEND_DATA m_struCond = new CHCNetSDK.NET_DVR_CARD_CFG_SEND_DATA();
        public CHCNetSDK.NET_DVR_CARD_USER_INFO_CFG m_struUserInfo = new CHCNetSDK.NET_DVR_CARD_USER_INFO_CFG();
        public int m_lUserID = -1;

        public UserCardMgr()
        {
            InitializeComponent();
            m_struCond.byCardNo = new byte[CHCNetSDK.ACS_CARD_NO_LEN];
            m_struUserInfo.byUsername = new byte[CHCNetSDK.NAME_LEN];
        }

        private void buttonGet_Click(object sender, EventArgs e)
        {
            if ("" == textBoxCardNumber.Text)
            {
                MessageBox.Show("Please input the card number!");
                return;
            }
            byte[] byTempCardNo = System.Text.Encoding.UTF8.GetBytes(textBoxCardNumber.Text);
            for (int i = 0; i < byTempCardNo.Length; i++)
            {
                if (i > m_struCond.byCardNo.Length)
                {
                    MessageBox.Show("card number length too long!");
                    return;
                }
                m_struCond.byCardNo[i] = byTempCardNo[i];
            }

            m_struCond.dwSize = (uint)Marshal.SizeOf(m_struCond);
            uint dwSizeCard = (uint)Marshal.SizeOf(m_struCond);
            IntPtr ptrStruCard = Marshal.AllocHGlobal((int)dwSizeCard);
            Marshal.StructureToPtr(m_struCond, ptrStruCard, false);

            uint dwStatus = 0;
            IntPtr ptrdwStatus = Marshal.AllocHGlobal(Marshal.SizeOf(dwStatus));
            Marshal.WriteInt32(ptrdwStatus, (int)dwStatus);

            uint dwSizeUse = (uint)Marshal.SizeOf(m_struUserInfo);
            IntPtr ptrStruUser = Marshal.AllocHGlobal((int)dwSizeUse);
            Marshal.StructureToPtr(m_struUserInfo, ptrStruUser, false);

            if (!CHCNetSDK.NET_DVR_GetDeviceConfig(m_lUserID, CHCNetSDK.NET_DVR_GET_CARD_USERINFO_CFG,
                1, ptrStruCard, dwSizeCard, ptrdwStatus, ptrStruUser, dwSizeUse))
            {
                Marshal.FreeHGlobal(ptrStruCard);
                Marshal.FreeHGlobal(ptrStruUser);
                string strTemp = null;
                strTemp = string.Format("NET_DVR_GET_CARD_USERINFO_CFG FAIL ERROR CODE {0}", CHCNetSDK.NET_DVR_GetLastError());
                MessageBox.Show(strTemp);
                return;
            }
            else
            {
                dwStatus = (uint)Marshal.ReadInt32(ptrdwStatus);
                if (dwStatus != CHCNetSDK.NET_DVR_NOERROR)
                {
                    string strTemp = null;
                    strTemp = string.Format("NET_DVR_GET_CARD_USERINFO_CFG Error Status{0}", dwStatus);
                    MessageBox.Show(strTemp);
                    Marshal.FreeHGlobal(ptrStruCard);
                    Marshal.FreeHGlobal(ptrStruUser);
                    return;
                }
                m_struUserInfo = (CHCNetSDK.NET_DVR_CARD_USER_INFO_CFG)Marshal.PtrToStructure(ptrStruUser, typeof(CHCNetSDK.NET_DVR_CARD_USER_INFO_CFG));
                textBoxUserName.Text = System.Text.Encoding.UTF8.GetString(m_struUserInfo.byUsername);
                textBoxUserNumber.Text = m_struUserInfo.byRes2[0].ToString();
                Marshal.FreeHGlobal(ptrStruCard);
                Marshal.FreeHGlobal(ptrStruUser);
            }
        }

        private void buttonSet_Click(object sender, EventArgs e)
        {
            if ("" == textBoxCardNumber.Text || "" == textBoxUserName.Text)
            {
                MessageBox.Show("Please input all the parameter!");
                return;
            }
            byte[] byTempCardNo = System.Text.Encoding.UTF8.GetBytes(textBoxCardNumber.Text);
            for (int i = 0; i < byTempCardNo.Length; i++)
            {
                if (i > m_struCond.byCardNo.Length)
                {
                    return;
                }
                m_struCond.byCardNo[i] = byTempCardNo[i];
            }
            byte[] byTempUserName = System.Text.Encoding.UTF8.GetBytes(textBoxUserName.Text);
            for (int i = 0; i < byTempUserName.Length; i++)
            {
                if (i > m_struUserInfo.byUsername.Length)
                {
                    return;
                }
                m_struUserInfo.byUsername[i] = byTempUserName[i];
            }

            byte.TryParse(textBoxUserNumber.Text, out m_struUserInfo.byRes2[0]);

            m_struCond.dwSize = (uint)Marshal.SizeOf(m_struCond);
            uint dwSizeCard = (uint)Marshal.SizeOf(m_struCond);
            IntPtr ptrStruCard = Marshal.AllocHGlobal((int)dwSizeCard);
            Marshal.StructureToPtr(m_struCond, ptrStruCard, false);

            uint dwStatus = 0;
            IntPtr ptrdwStatus = Marshal.AllocHGlobal(Marshal.SizeOf(dwStatus));
            Marshal.WriteInt32(ptrdwStatus, (int)dwStatus);

            m_struUserInfo.dwSize = (uint)Marshal.SizeOf(m_struUserInfo);
            uint dwSizeUse = (uint)Marshal.SizeOf(m_struUserInfo);
            IntPtr ptrStruUser = Marshal.AllocHGlobal((int)dwSizeUse);
            Marshal.StructureToPtr(m_struUserInfo, ptrStruUser, false);

            if (!CHCNetSDK.NET_DVR_SetDeviceConfig(m_lUserID, CHCNetSDK.NET_DVR_SET_CARD_USERINFO_CFG,
                1, ptrStruCard, dwSizeCard, ptrdwStatus, ptrStruUser, dwSizeUse))
            {
                Marshal.FreeHGlobal(ptrStruCard);
                Marshal.FreeHGlobal(ptrStruUser);
                string strTemp = null;
                strTemp = string.Format("NET_DVR_SET_CARD_USERINFO_CFG FAIL, ERROR CODE {0}", CHCNetSDK.NET_DVR_GetLastError());
                MessageBox.Show(strTemp);
                return;
            }
            else
            {
                dwStatus = (uint)Marshal.ReadInt32(ptrdwStatus);
                if (dwStatus != CHCNetSDK.NET_DVR_NOERROR)
                {
                    string strTemp = null;
                    strTemp = string.Format("NET_DVR_SET_CARD_USERINFO_CFG Error Status{0}", dwStatus);
                    MessageBox.Show(strTemp);
                    Marshal.FreeHGlobal(ptrStruCard);
                    Marshal.FreeHGlobal(ptrStruUser);
                    return;
                }
                Marshal.FreeHGlobal(ptrStruCard);
                Marshal.FreeHGlobal(ptrStruUser);
                MessageBox.Show("SET SUCC");
            }
        }

        private void UserCardMgr_Load(object sender, EventArgs e)
        {
            MultiLanguage.LoadLanguage(this);
        }
    }
}
