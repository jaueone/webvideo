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
    public partial class CardConfigure : Form
    {
        private bool m_bSelectAll = false;
        private string m_strCardNo = null;
        private string m_strSaveCardNo = null;
        private int m_iAddNum = -1;
        private CHCNetSDK.NET_DVR_CARD_CFG_V50 m_struCardCfg = new CHCNetSDK.NET_DVR_CARD_CFG_V50();

        public CardConfigure()
        {
            InitializeComponent();
            this.SuspendLayout();
            this.columnHeader.Name = "columnHeader";
            this.columnHeader1.Name = "columnHeader1";
            this.columnHeader2.Name = "columnHeader2";
            this.ResumeLayout();
            m_struCardCfg.Init();
            InitUI();
        }

        public void SetCardCfg(ref CHCNetSDK.NET_DVR_CARD_CFG_V50 struItem, string strCardNo = null)
        {
            if (null == struItem.byCardNo)
            {
                struItem.byCardNo = new byte[CHCNetSDK.ACS_CARD_NO_LEN];
            }
            if (null == struItem.byCardPassword)
            {
                struItem.byCardPassword = new byte[CHCNetSDK.CARD_PASSWORD_LEN];
            }
            if (null == struItem.wCardRightPlan)
            {
                struItem.wCardRightPlan = new ushort[CHCNetSDK.MAX_DOOR_NUM_256 * CHCNetSDK.MAX_CARD_RIGHT_PLAN_NUM];
            }
            if (null == struItem.byDoorRight)
            {
                struItem.byDoorRight = new byte[CHCNetSDK.MAX_DOOR_NUM_256];
            }
            if (null == struItem.byBelongGroup)
            {
                struItem.byBelongGroup = new byte[CHCNetSDK.MAX_GROUP_NUM_128];
            }
            if (null == struItem.byLockCode)
            {
                struItem.byLockCode = new byte[CHCNetSDK.MAX_LOCK_CODE_LEN];
            }
            if (null == struItem.byRoomCode)
            {
                struItem.byRoomCode = new byte[CHCNetSDK.MAX_DOOR_CODE_LEN];
            }
            if (null == struItem.byName)
            {
                struItem.byName = new byte[CHCNetSDK.NAME_LEN];
            }

            m_struCardCfg = struItem;
            m_strCardNo = strCardNo;
            m_strSaveCardNo = m_strCardNo;
            UpdateInterface();
        }

        public CHCNetSDK.NET_DVR_CARD_CFG_V50 GetCardCfg(string strCardNo)
        {
            m_strSaveCardNo = strCardNo;
            UpdateInterface(true);
            return m_struCardCfg;
        }

        public int GetAddNum()
        {
            return m_iAddNum;
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

        private void buttonExit_Click(object sender, EventArgs e)
        {
            this.Close();
            return;
        }

        private void buttonTestParameters_Click(object sender, EventArgs e)
        {
            m_bSelectAll = true;
            textBoxCardNumber.Text = "111";
            textBoxCardPassword.Text = "222";
            textBoxWorkNo.Text = "12345";

            checkBoxCardIsValid.Checked = true;
            checkBoxFirstCard.Checked = false;
            comboBoxCardType.SelectedIndex = 0;

            textBoxMaximumCreditCard.Text = "32";
            textBoxPlanNumber.Text = "1";
            checkBoxEnable.Checked = true;
            checkBoxAll.Checked = true;

            checkBoxAll_CheckedChanged(sender, e);
            checkedListBoxDoorAuthority.SetItemChecked(0, true);
            checkedListBoxGroup.SetItemChecked(0, false);

            m_struCardCfg.wCardRightPlan[0] = 1;
            m_struCardCfg.wCardRightPlan[1] = 2;
            UpdatePlanList(m_struCardCfg.wCardRightPlan);
        }

        private void checkBoxAll_CheckedChanged(object sender, EventArgs e)
        {
            if (checkBoxAll.Checked == true)
            {
                m_bSelectAll = true;
            }
            if (m_bSelectAll)
            {
                checkBoxEffectiveParameters.Checked = true;
                checkBoxTheValidityOfParameter.Checked = true;
                checkBoxDoorLimitParameters.Checked = true;
                checkBoxTheFirstCardParameters.Checked = true;
                checkBoxCardPasswordParameters.Checked = true;
                checkBoxBelongsToGroupParameters.Checked = true;
                checkBoxHaveCreditCardNumber.Checked = true;
                checkBoxCardTypeParameter.Checked = true;
                checkBoxTheBiggestCreditCardNumber.Checked = true;
                checkBoxCardAccessPlan.Checked = true;
                checkBoxWorkNo.Checked = true;
                m_bSelectAll = false;
            }
            else
            {
                checkBoxEffectiveParameters.Checked = false;
                checkBoxTheValidityOfParameter.Checked = false;
                checkBoxDoorLimitParameters.Checked = false;
                checkBoxTheFirstCardParameters.Checked = false;
                checkBoxCardPasswordParameters.Checked = false;
                checkBoxBelongsToGroupParameters.Checked = false;
                checkBoxHaveCreditCardNumber.Checked = false;
                checkBoxCardTypeParameter.Checked = false;
                checkBoxTheBiggestCreditCardNumber.Checked = false;
                checkBoxCardAccessPlan.Checked = false;
                checkBoxWorkNo.Checked = false;
            }
        }

        private void UpdatePlanList(ushort[] wCardPlan)
        {
            listViewCardAuthorityScheme.BeginUpdate();
            listViewCardAuthorityScheme.Items.Clear();
            ListViewItem lvItem;
            ListViewItem.ListViewSubItem lvSubItem;
            int i, j;
            String csTmp;
            for (i = 0; i < CHCNetSDK.MAX_DOOR_NUM; i++)
            {
                for (j = 0; j < CHCNetSDK.MAX_CARD_RIGHT_PLAN_NUM; j++)
                {
                    lvItem = new ListViewItem();
                    lvItem.Text = (i+1).ToString();
                    lvSubItem = new System.Windows.Forms.ListViewItem.ListViewSubItem();
                    csTmp = (j+1).ToString();

                    lvSubItem.Text = csTmp;
                    lvItem.SubItems.Add(lvSubItem);

                    lvSubItem = new System.Windows.Forms.ListViewItem.ListViewSubItem();
                    csTmp = wCardPlan[i * CHCNetSDK.MAX_CARD_RIGHT_PLAN_NUM + j].ToString();

                    lvSubItem.Text = csTmp;
                    lvItem.SubItems.Add(lvSubItem);
                    listViewCardAuthorityScheme.Items.Add(lvItem);
                }
            }
            listViewCardAuthorityScheme.EndUpdate();
            return ;
        }

        private void UpdateInterface()
        {
            m_strCardNo = m_strSaveCardNo;
            CHCNetSDK.NET_DVR_CARD_CFG_V50? struCardInfo = m_struCardCfg;
            if (null == struCardInfo)
            {
                return;
            }
            bool bCheck = false;
            textBoxCardNumber.Text = m_strCardNo;
            if ((m_struCardCfg.dwModifyParamType & 0x1) > 0)
            {
                checkBoxEffectiveParameters.Checked = true;
            }
            if (1 == m_struCardCfg.byCardValid)
            {
                checkBoxCardIsValid.Checked = true;
            }
            if ((m_struCardCfg.dwModifyParamType & 0x2) > 0)
            {
                checkBoxTheValidityOfParameter.Checked = true;
            }
            if (1 == m_struCardCfg.struValid.byEnable)
            {
                checkBoxEnable.Checked = true;
                if (CheckDate(m_struCardCfg.struValid.struBeginTime))
                {
                    dateTimePickerStartDate.Value = new System.DateTime(m_struCardCfg.struValid.struBeginTime.wYear,
                        m_struCardCfg.struValid.struBeginTime.byMonth, m_struCardCfg.struValid.struBeginTime.byDay);
                    dateTimePickerStartTime.Value = new System.DateTime(dateTimePickerStartTime.Value.Year, 
                        dateTimePickerStartTime.Value.Month, dateTimePickerStartTime.Value.Day,
                        m_struCardCfg.struValid.struBeginTime.byHour, m_struCardCfg.struValid.struBeginTime.byMinute,
                        m_struCardCfg.struValid.struBeginTime.bySecond);
                }
                if (CheckDate(m_struCardCfg.struValid.struEndTime))
                {
                    dateTimePickerEndDate.Value = new System.DateTime(m_struCardCfg.struValid.struEndTime.wYear,
                        m_struCardCfg.struValid.struEndTime.byMonth, m_struCardCfg.struValid.struEndTime.byDay);
                    dateTimePickerEndTime.Value = new System.DateTime(dateTimePickerEndTime.Value.Year,
                        dateTimePickerEndTime.Value.Month, dateTimePickerEndTime.Value.Day,
                        m_struCardCfg.struValid.struEndTime.byHour, m_struCardCfg.struValid.struEndTime.byMinute,
                        m_struCardCfg.struValid.struEndTime.bySecond);
                }
            }
            if ((m_struCardCfg.dwModifyParamType & 0x4) > 0)
            {
                checkBoxCardTypeParameter.Checked = true;
            }
            comboBoxCardType.SelectedIndex = (int)m_struCardCfg.byCardType - 1;
            if ((m_struCardCfg.dwModifyParamType & 0x8) > 0)
            {
                checkBoxDoorLimitParameters.Checked = true;
            }
            for (int i = 0; i < CHCNetSDK.MAX_DOOR_NUM_256; i++)
            {
                if (0 != (m_struCardCfg.byDoorRight[i]))
                {
                    bCheck = true;
                }
                else
                {
                    bCheck = false;
                }
                checkedListBoxDoorAuthority.SetItemChecked(i, bCheck);
            }
            if ((m_struCardCfg.dwModifyParamType & 0x10) > 0)
            {
                checkBoxTheFirstCardParameters.Checked = true;
            }
            if (1 == m_struCardCfg.byLeaderCard)
            {
                checkBoxFirstCard.Checked = true;
            }
            if ((m_struCardCfg.dwModifyParamType & 0x20) > 0)
            {
                checkBoxTheBiggestCreditCardNumber.Checked = true;
            }
            textBoxMaximumCreditCard.Text = m_struCardCfg.dwMaxSwipeTime.ToString();
            textBoxRoomNO.Text = m_struCardCfg.wRoomNumber.ToString();
            if ((m_struCardCfg.dwModifyParamType & 0x20) > 0)
            {
                checkBoxBelongsToGroupParameters.Checked = true;
            }
            for (int i = 0; i < CHCNetSDK.MAX_GROUP_NUM_128; i++ )
            {
                if (0 != (m_struCardCfg.byBelongGroup[i]))
                {
                    bCheck = true;
                }
                else
                {
                    bCheck = false;
                }
                checkedListBoxGroup.SetItemChecked(i, bCheck);
            }
            if ((m_struCardCfg.dwModifyParamType & 0x80) > 0)
            {
                checkBoxCardPasswordParameters.Checked = true;
            }
            textBoxCardPassword.Text = System.Text.Encoding.UTF8.GetString(m_struCardCfg.byCardPassword);
            if ((m_struCardCfg.dwModifyParamType & 0x100) > 0)
            {
                checkBoxCardAccessPlan.Checked = true;
            }
            
            if ((m_struCardCfg.dwModifyParamType & 0x200) > 0)
            {
                checkBoxHaveCreditCardNumber.Checked = true;
            } 
            textBoxreditCard.Text = m_struCardCfg.dwSwipeTime.ToString();
            UpdatePlanList(m_struCardCfg.wCardRightPlan);


            if ((m_struCardCfg.dwModifyParamType & 0x400) > 0)
            {
                checkBoxWorkNo.Checked = true;
            }

            textBoxFloorNo.Text = m_struCardCfg.wFloorNumber.ToString();

            for (int i = 0; i < checkedListBoxCardAuthority.Items.Count; i++)
            {
                if (0 != (m_struCardCfg.dwCardRight & (1 << i)))
                {
                    bCheck = true;
                }
                else
                {
                    bCheck = false;
                }
                checkedListBoxCardAuthority.SetItemChecked(i, bCheck);
            }

            textBoxWorkNo.Text = m_struCardCfg.dwEmployeeNo.ToString();
            textBoxName.Text = System.Text.Encoding.UTF8.GetString(m_struCardCfg.byName);
            textBoxDepartmentNo.Text = m_struCardCfg.wDepartmentNo.ToString();
            textBoxScheduleNo.Text = m_struCardCfg.wSchedulePlanNo.ToString();
            comboBoxScheduleType.SelectedIndex = (int)m_struCardCfg.bySchedulePlanType;


            textBoxLockID.Text = m_struCardCfg.dwLockID.ToString();
            textBoxCardOwnerID.Text = m_struCardCfg.dwCardUserId.ToString();
            comboBoxModelType.SelectedIndex = (int)m_struCardCfg.byCardModelType;
            textBoxLockCode.Text = System.Text.Encoding.UTF8.GetString(m_struCardCfg.byLockCode);
            textBoxRoomCode.Text = System.Text.Encoding.UTF8.GetString(m_struCardCfg.byRoomCode);
        }

        private void UpdateInterface(bool bRefresh)
        {
            m_strCardNo = textBoxCardNumber.Text;
            m_strSaveCardNo = m_strCardNo;
            m_struCardCfg = new CHCNetSDK.NET_DVR_CARD_CFG_V50();
            m_struCardCfg.Init();

            m_struCardCfg.dwSize = (uint)Marshal.SizeOf(m_struCardCfg);
            byte[] byTempCardNo = new byte[CHCNetSDK.ACS_CARD_NO_LEN];
            byTempCardNo = System.Text.Encoding.UTF8.GetBytes(m_strCardNo);
            for (int i = 0; i < byTempCardNo.Length; i++ )
            {
                m_struCardCfg.byCardNo[i] = byTempCardNo[i];
            }

            if (checkBoxEffectiveParameters.Checked)
            {
                m_struCardCfg.dwModifyParamType |= 0x1;
            }

            if (checkBoxCardIsValid.Checked)
            {
                m_struCardCfg.byCardValid = 1;
            }
            else
            {
                m_struCardCfg.byCardValid = 0;
            }

            if (checkBoxTheValidityOfParameter.Checked)
            {
                m_struCardCfg.dwModifyParamType |= 0x2;
                m_struCardCfg.struValid.byEnable = 1;
                if (checkBoxEnable.Checked)
                {
                    m_struCardCfg.struValid.struBeginTime.wYear = (ushort)dateTimePickerStartDate.Value.Year;
                    m_struCardCfg.struValid.struBeginTime.byMonth = (byte)dateTimePickerStartDate.Value.Month;
                    m_struCardCfg.struValid.struBeginTime.byDay = (byte)dateTimePickerStartDate.Value.Day;
                    m_struCardCfg.struValid.struBeginTime.byHour = (byte)dateTimePickerStartTime.Value.Hour;
                    m_struCardCfg.struValid.struBeginTime.byMinute = (byte)dateTimePickerStartTime.Value.Minute;
                    m_struCardCfg.struValid.struBeginTime.bySecond = (byte)dateTimePickerStartTime.Value.Second;
                    m_struCardCfg.struValid.struEndTime.wYear = (ushort)dateTimePickerEndDate.Value.Year;
                    m_struCardCfg.struValid.struEndTime.byMonth = (byte)dateTimePickerEndDate.Value.Month;
                    m_struCardCfg.struValid.struEndTime.byDay = (byte)dateTimePickerEndDate.Value.Day;
                    m_struCardCfg.struValid.struEndTime.byHour = (byte)dateTimePickerEndTime.Value.Hour;
                    m_struCardCfg.struValid.struEndTime.byMinute = (byte)dateTimePickerEndTime.Value.Minute;
                    m_struCardCfg.struValid.struEndTime.bySecond = (byte)dateTimePickerEndTime.Value.Second;
                }
            }
            if (checkBoxCardTypeParameter.Checked)
            {
                m_struCardCfg.dwModifyParamType |= 0x4;
                m_struCardCfg.byCardType = (byte)(comboBoxCardType.SelectedIndex + 1);
            }
            if (checkBoxDoorLimitParameters.Checked)
            {
                m_struCardCfg.dwModifyParamType |= 0x8;
                System.Array.Clear(m_struCardCfg.byDoorRight, 0, m_struCardCfg.byDoorRight.Length);
                for (int i = 0; i < checkedListBoxDoorAuthority.Items.Count; i++)
                {
                    if (checkedListBoxDoorAuthority.GetItemChecked(i))
                    {
                        m_struCardCfg.byDoorRight[i] = 1;
                    }
                }
            }
            if (checkBoxTheFirstCardParameters.Checked)
            {
                m_struCardCfg.dwModifyParamType |= 0x10;
                m_struCardCfg.byLeaderCard = 1;
            }
            if (checkBoxTheBiggestCreditCardNumber.Checked)
            {
                m_struCardCfg.dwModifyParamType |= 0x20;
                uint.TryParse(textBoxMaximumCreditCard.Text, out m_struCardCfg.dwMaxSwipeTime);
            }
            ushort.TryParse(textBoxRoomNO.Text, out m_struCardCfg.wRoomNumber);
            if (checkBoxBelongsToGroupParameters.Checked)
            {
                m_struCardCfg.dwModifyParamType |= 0x40;
                System.Array.Clear(m_struCardCfg.byBelongGroup, 0, m_struCardCfg.byBelongGroup.Length);
                for (int i = 0; i < checkedListBoxGroup.Items.Count; i++)
                {
                    if (checkedListBoxGroup.GetItemChecked(i))
                    {
                        m_struCardCfg.byBelongGroup[i] = 1;
                    }
                }
            }
            if (checkBoxCardPasswordParameters.Checked)
            {
                m_struCardCfg.dwModifyParamType |= 0x80;
                byte[] byTempCardPassword = new byte[CHCNetSDK.CARD_PASSWORD_LEN];
                byTempCardPassword = System.Text.Encoding.UTF8.GetBytes(textBoxCardPassword.Text);
                for (int i = 0; i < byTempCardPassword.Length; i++)
                {
                    m_struCardCfg.byCardPassword[i] = byTempCardPassword[i];
                }
            }
            if (checkBoxCardAccessPlan.Checked)
            {
                m_struCardCfg.dwModifyParamType |= 0x100;
                int i;
                int j;
                for (i = 0; i < CHCNetSDK.MAX_DOOR_NUM; i++)
                {
                    for (j = 0; j < CHCNetSDK.MAX_CARD_RIGHT_PLAN_NUM; j++)
                    {
                        ushort.TryParse(listViewCardAuthorityScheme.Items[i * CHCNetSDK.MAX_CARD_RIGHT_PLAN_NUM + j].SubItems[2].Text,
                            out m_struCardCfg.wCardRightPlan[i * CHCNetSDK.MAX_CARD_RIGHT_PLAN_NUM + j]);
                    }
                }
            }
            if (checkBoxHaveCreditCardNumber.Checked)
            {
                m_struCardCfg.dwModifyParamType |= 0x200;
                uint.TryParse(textBoxreditCard.Text, out m_struCardCfg.dwSwipeTime);
            }

            if (checkBoxWorkNo.Checked)
            {
                m_struCardCfg.dwModifyParamType |= 0x400;
                uint.TryParse(textBoxreditCard.Text, out m_struCardCfg.dwEmployeeNo);
            }

            short.TryParse(textBoxRoomNO.Text, out m_struCardCfg.wFloorNumber);

            m_struCardCfg.dwCardRight = 0;
            for (int i = 0; i < checkedListBoxCardAuthority.Items.Count; i++)
            {
                if (checkedListBoxCardAuthority.GetItemChecked(i))
                {
                    m_struCardCfg.dwCardRight |= (uint)(1 << i);
                }
            }
            uint.TryParse(textBoxWorkNo.Text, out m_struCardCfg.dwEmployeeNo);

            m_struCardCfg.dwModifyParamType |= 0xf800;
            byte[] byTempName = new byte[CHCNetSDK.NAME_LEN];
            byTempName = System.Text.Encoding.Default.GetBytes(textBoxName.Text);
            for (int i = 0; i < byTempName.Length; i++)
            {
                m_struCardCfg.byName[i] = byTempName[i];
            }

            ushort.TryParse(textBoxDepartmentNo.Text, out m_struCardCfg.wDepartmentNo);
            ushort.TryParse(textBoxScheduleNo.Text, out m_struCardCfg.wSchedulePlanNo);
            m_struCardCfg.bySchedulePlanType = (byte)comboBoxScheduleType.SelectedIndex;

            m_struCardCfg.byCardModelType = (byte)comboBoxModelType.SelectedIndex;
            uint.TryParse(textBoxLockID.Text, out m_struCardCfg.dwLockID);
            uint.TryParse(textBoxCardOwnerID.Text, out m_struCardCfg.dwCardUserId);

            byte[] byTmpLockCode = new byte[CHCNetSDK.MAX_LOCK_CODE_LEN];
            byTmpLockCode = System.Text.Encoding.UTF8.GetBytes(textBoxLockCode.Text);
            for (int i = 0; i < byTmpLockCode.Length; i++)
            {
                m_struCardCfg.byLockCode[i] = byTmpLockCode[i];
            }

            byte[] byTempRoomCode = new byte[CHCNetSDK.MAX_DOOR_CODE_LEN];
            byTempRoomCode = System.Text.Encoding.UTF8.GetBytes(textBoxRoomCode.Text);
            for (int i = 0; i < byTempRoomCode.Length; i++)
            {
                m_struCardCfg.byRoomCode[i] = byTempRoomCode[i];
            }

            //input not int type ,make default to 1;
            if (!int.TryParse(textBoxAddCardNumber.Text, out m_iAddNum))
            {
                m_iAddNum = 1;
            }
        }
        private void buttonConfirm_Click(object sender, EventArgs e)
        {
            UpdateInterface(false);
            this.DialogResult = DialogResult.OK;
        }

        private void InitUI()
        {
            //clear 
            checkedListBoxDoorAuthority.Items.Clear();
            checkedListBoxGroup.Items.Clear();

            //create
            checkedListBoxDoorAuthority.BeginUpdate();
            for (int i = 0; i < CHCNetSDK.MAX_DOOR_NUM_256; i++)
            {
                checkedListBoxDoorAuthority.Items.Add("Door " + (i + 1));
            }
            checkedListBoxDoorAuthority.EndUpdate();
            checkedListBoxGroup.BeginUpdate();
            for (int i = 0; i < CHCNetSDK.MAX_GROUP_NUM_128; i++)
            {
                checkedListBoxGroup.Items.Add("Group " + (i + 1));
            }
            checkedListBoxGroup.EndUpdate();
        }

        private void buttonModify_Click(object sender, EventArgs e)
        {
            foreach (ListViewItem lvi in listViewCardAuthorityScheme.SelectedItems)
            {
                lvi.SubItems[2].Text = textBoxPlanNumber.Text;
            }
        }

        private void CardConfigure_Load(object sender, EventArgs e)
        {
            MultiLanguage.LoadLanguage(this);
        }
    }
}
