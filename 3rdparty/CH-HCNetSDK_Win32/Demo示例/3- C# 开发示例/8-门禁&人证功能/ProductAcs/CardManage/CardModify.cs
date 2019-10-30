using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading;
using System.Windows.Forms;

using AcsDemo.Public;

namespace AcsDemo.ProductAcs.CardManage
{
    public partial class CardModify : Form
    {
        private CHCNetSDK.NET_DVR_CARD_CFG_V50 m_struCardCfg = new CHCNetSDK.NET_DVR_CARD_CFG_V50();
        public CardInfo cardInformations = new CardInfo();

        public CardModify()
        {
            InitializeComponent();
            m_struCardCfg.Init();
        }

        private void SaveCardInfo()
        {
            cardInformations.cardSerialNumber = "1";
            cardInformations.cardNumber = textBoxCardModifyCardNo.Text.ToString();
            cardInformations.cardPassword = textBoxCardModifyPassword.Text.ToString();
            cardInformations.cardType = comboBoxCardModifyCardType.Text.ToString();
            cardInformations.cardValidityPeriod = dateTimePickerStart.Text.ToString() + "-" + dateTimePickerEnd.Text.ToString();
            cardInformations.fingerPrintDescriptions = "";
        }

        private void buttonConfirm_Click(object sender, EventArgs e)
        {
            SaveCardInfo();
            this.DialogResult = DialogResult.OK;
        }

        private void buttonCancel_Click(object sender, EventArgs e)
        {
            this.DialogResult = DialogResult.Cancel;
        }

        private void UpdateInterface()
        {
            if (CheckDate(m_struCardCfg.struValid.struBeginTime))
            {
                dateTimePickerStart.Value = new System.DateTime(m_struCardCfg.struValid.struBeginTime.wYear,
                    m_struCardCfg.struValid.struBeginTime.byMonth,m_struCardCfg.struValid.struBeginTime.byDay,
                    m_struCardCfg.struValid.struBeginTime.byHour, m_struCardCfg.struValid.struBeginTime.byMinute,
                    m_struCardCfg.struValid.struBeginTime.bySecond);
            }
            if (CheckDate(m_struCardCfg.struValid.struEndTime))
            {
                dateTimePickerEnd.Value = new System.DateTime(m_struCardCfg.struValid.struEndTime.wYear,
                    m_struCardCfg.struValid.struEndTime.byMonth, m_struCardCfg.struValid.struEndTime.byDay,
                    m_struCardCfg.struValid.struEndTime.byHour, m_struCardCfg.struValid.struEndTime.byMinute,
                    m_struCardCfg.struValid.struEndTime.bySecond);
            }

            comboBoxCardModifyCardType.SelectedIndex = (int)m_struCardCfg.byCardType;
            textBoxCardModifyPassword.Text = System.Text.Encoding.UTF8.GetString(m_struCardCfg.byCardPassword);
            textBoxCardModifyCardNo.Text = System.Text.Encoding.UTF8.GetString(m_struCardCfg.byCardNo);
        }

        private bool CheckDate(CHCNetSDK.NET_DVR_TIME_EX struItem)
        {
            if (struItem.wYear < 1970 || struItem.byMonth > 12 || struItem.byDay > 31)
            {
                return false;
            }

            if (struItem.byHour > 23 || struItem.byMinute > 59 || struItem.bySecond > 59)
            {
                return false;
            }
            return true;
        }

        public void SetCardCfg(ref CHCNetSDK.NET_DVR_CARD_CFG_V50 struItem)
        {
            m_struCardCfg = struItem;
            UpdateInterface();
        }

        public CHCNetSDK.NET_DVR_CARD_CFG_V50 GetCardCfg()
        {
            return m_struCardCfg;
        }


    }
}
