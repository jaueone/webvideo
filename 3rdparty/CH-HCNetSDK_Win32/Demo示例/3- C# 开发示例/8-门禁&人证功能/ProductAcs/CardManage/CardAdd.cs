using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading;
using System.Windows.Forms;

using System.Runtime.InteropServices;
using AcsDemo.Public;

namespace AcsDemo.ProductAcs.CardManage
{
    public partial class CardAdd : Form
    {
        //this can change to list when iMaxCard > 1
        public CardInfo cardInformations = new CardInfo();

        //current is set to 1
        const int iMaxCard = 1;
        //private bool m_bSendOne = true;

        public int curCardNum = 0;
        public CardAdd()
        {
            InitializeComponent();
            //dateTimePickerStart.Format = DateTimePickerFormat.Custom;
            //dateTimePickerEnd.Format = DateTimePickerFormat.Custom;
        }

        public bool CheckUserInput()
        {
            if ((textBoxCardAddCardNo.Text.Length <= 0) || (textBoxCardAddPassword.Text.Length <= 0) || (comboBoxCardAddCardType.Text.Length <= 0))
            {
                MessageBox.Show(Properties.Resources.addCardTips);
                return false;
            }
            else
            {
                return true;
            }
        }

        private void buttonCardAddAdd_Click(object sender, EventArgs e)
        {
            if(curCardNum < iMaxCard)
            {
                if (!CheckUserInput())
                {
                    return;
                }

                this.listViewUserAddCard.BeginUpdate();
                ListViewItem lvi = new ListViewItem();
                lvi.Text = "1";
                lvi.SubItems.Add(textBoxCardAddCardNo.Text.ToString());
                lvi.SubItems.Add(comboBoxCardAddCardType.Text.ToString());
                lvi.SubItems.Add(dateTimePickerStart.Text.ToString() + "-" + dateTimePickerEnd.Text.ToString());
                curCardNum++;
                this.listViewUserAddCard.Items.Add(lvi);
                this.listViewUserAddCard.EndUpdate();
            }
        }

        private void SaveCardInfo()
        {
            cardInformations.cardSerialNumber = curCardNum.ToString();
            cardInformations.cardNumber = textBoxCardAddCardNo.Text.ToString();
            cardInformations.cardPassword = textBoxCardAddPassword.Text.ToString();
            cardInformations.cardType = comboBoxCardAddCardType.Text.ToString();
            cardInformations.cardValidityPeriod = dateTimePickerStart.Text.ToString() + "-" + dateTimePickerEnd.Text.ToString();
            cardInformations.fingerPrintDescriptions = "";
        }

        private void buttonConfirm_Click(object sender, EventArgs e)
        {
            //have set data
            if (curCardNum > 0)
            {
                SaveCardInfo();
            }
            this.DialogResult = DialogResult.OK;
        }

        public CHCNetSDK.NET_DVR_CARD_CFG_V50 GetCurCardInfo()
        {
            CHCNetSDK.NET_DVR_CARD_CFG_V50 struCardCfg = new CHCNetSDK.NET_DVR_CARD_CFG_V50();
            struCardCfg.Init();

            //see the NET_DVR_CARD_CFG_V50 definition
            struCardCfg.dwModifyParamType = 0x00000087;
            struCardCfg.byCardValid = 1;
            struCardCfg.byCardType = (byte)comboBoxCardAddCardType.SelectedIndex;

            struCardCfg.struValid.byEnable = 1;
            struCardCfg.struValid.struBeginTime.wYear = (ushort)dateTimePickerStart.Value.Year;
            struCardCfg.struValid.struBeginTime.byMonth = (byte)dateTimePickerStart.Value.Month;
            struCardCfg.struValid.struBeginTime.byDay = (byte)dateTimePickerStart.Value.Day;
            struCardCfg.struValid.struBeginTime.byHour = (byte)dateTimePickerStart.Value.Hour;
            struCardCfg.struValid.struBeginTime.bySecond = (byte)dateTimePickerStart.Value.Second;
            struCardCfg.struValid.struEndTime.wYear = (ushort)dateTimePickerEnd.Value.Year;
            struCardCfg.struValid.struEndTime.byMonth = (byte)dateTimePickerEnd.Value.Month;
            struCardCfg.struValid.struEndTime.byDay = (byte)dateTimePickerEnd.Value.Day;
            struCardCfg.struValid.struEndTime.byHour = (byte)dateTimePickerEnd.Value.Hour;
            struCardCfg.struValid.struEndTime.byMinute = (byte)dateTimePickerEnd.Value.Minute;
            struCardCfg.struValid.struEndTime.bySecond = (byte)dateTimePickerEnd.Value.Second;

            byte[] sCardNo = System.Text.Encoding.Default.GetBytes(textBoxCardAddCardNo.Text);
            sCardNo.CopyTo(struCardCfg.byCardNo, 0);

            byte[] sCardPassword = System.Text.Encoding.Default.GetBytes(textBoxCardAddPassword.Text);
            sCardPassword.CopyTo(struCardCfg.byCardPassword, 0);

            return struCardCfg;
        }

        private void buttonCancel_Click(object sender, EventArgs e)
        {
            this.DialogResult = DialogResult.Cancel;
        }

        private void textBoxCardAddCardNo_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar != '\b')//backspace 
            {
                if (!(((e.KeyChar > '0') && (e.KeyChar < '9')) || 
                    ((e.KeyChar > 'a') && (e.KeyChar > 'z')) ||
                    ((e.KeyChar > 'A') && (e.KeyChar > 'Z'))))//0-9,a-z,A-Z is permitted  
                {
                    e.Handled = true;
                }
            } 
        }
    }
}
