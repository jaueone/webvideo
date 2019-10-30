namespace AcsDemo.ProductAcs.CardManage
{
    partial class UserAdd
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.textBoxMemberNo = new System.Windows.Forms.TextBox();
            this.labelMemberNo = new System.Windows.Forms.Label();
            this.textBoxMemberName = new System.Windows.Forms.TextBox();
            this.labelMemberName = new System.Windows.Forms.Label();
            this.tabControlUserAdd = new System.Windows.Forms.TabControl();
            this.tabPageCardInfo = new System.Windows.Forms.TabPage();
            this.buttonCardInfoDelete = new System.Windows.Forms.Button();
            this.buttonCardInfoModify = new System.Windows.Forms.Button();
            this.buttonConfirm = new System.Windows.Forms.Button();
            this.buttonCardInfoAdd = new System.Windows.Forms.Button();
            this.listViewUserAddCardInfo = new System.Windows.Forms.ListView();
            this.columnHeader1 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader2 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader3 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader4 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.tabPageModuleCfg = new System.Windows.Forms.TabPage();
            this.groupBox5 = new System.Windows.Forms.GroupBox();
            this.buttonModify = new System.Windows.Forms.Button();
            this.textBoxPlanNumber = new System.Windows.Forms.TextBox();
            this.labelPlanNumber = new System.Windows.Forms.Label();
            this.listViewCardAuthorityScheme = new System.Windows.Forms.ListView();
            this.columnHeader = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader5 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader6 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.checkedListBoxDoorAuthority = new System.Windows.Forms.CheckedListBox();
            this.tabPageInputFingerPrint = new System.Windows.Forms.TabPage();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.buttonBrowse = new System.Windows.Forms.Button();
            this.checkedListBoxReaderValid = new System.Windows.Forms.CheckedListBox();
            this.checkBoxNonBlock = new System.Windows.Forms.CheckBox();
            this.label7 = new System.Windows.Forms.Label();
            this.comboBoxFingerprintType = new System.Windows.Forms.ComboBox();
            this.label5 = new System.Windows.Forms.Label();
            this.textBoxDataLength = new System.Windows.Forms.TextBox();
            this.label4 = new System.Windows.Forms.Label();
            this.textBoxFingerprintDataPath = new System.Windows.Forms.TextBox();
            this.label3 = new System.Windows.Forms.Label();
            this.textBoxFingerPrintNumber = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.tabControlUserAdd.SuspendLayout();
            this.tabPageCardInfo.SuspendLayout();
            this.tabPageModuleCfg.SuspendLayout();
            this.groupBox5.SuspendLayout();
            this.groupBox1.SuspendLayout();
            this.tabPageInputFingerPrint.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.SuspendLayout();
            // 
            // textBoxMemberNo
            // 
            this.textBoxMemberNo.Location = new System.Drawing.Point(138, 38);
            this.textBoxMemberNo.Name = "textBoxMemberNo";
            this.textBoxMemberNo.Size = new System.Drawing.Size(104, 21);
            this.textBoxMemberNo.TabIndex = 37;
            this.textBoxMemberNo.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.textBoxMemberNo_KeyPress);
            // 
            // labelMemberNo
            // 
            this.labelMemberNo.AutoSize = true;
            this.labelMemberNo.ImeMode = System.Windows.Forms.ImeMode.NoControl;
            this.labelMemberNo.Location = new System.Drawing.Point(56, 41);
            this.labelMemberNo.Name = "labelMemberNo";
            this.labelMemberNo.Size = new System.Drawing.Size(59, 12);
            this.labelMemberNo.TabIndex = 36;
            this.labelMemberNo.Text = "Member No";
            // 
            // textBoxMemberName
            // 
            this.textBoxMemberName.Location = new System.Drawing.Point(366, 38);
            this.textBoxMemberName.Name = "textBoxMemberName";
            this.textBoxMemberName.Size = new System.Drawing.Size(104, 21);
            this.textBoxMemberName.TabIndex = 39;
            // 
            // labelMemberName
            // 
            this.labelMemberName.AutoSize = true;
            this.labelMemberName.ImeMode = System.Windows.Forms.ImeMode.NoControl;
            this.labelMemberName.Location = new System.Drawing.Point(279, 41);
            this.labelMemberName.Name = "labelMemberName";
            this.labelMemberName.Size = new System.Drawing.Size(71, 12);
            this.labelMemberName.TabIndex = 38;
            this.labelMemberName.Text = "Member Name";
            // 
            // tabControlUserAdd
            // 
            this.tabControlUserAdd.Controls.Add(this.tabPageCardInfo);
            this.tabControlUserAdd.Controls.Add(this.tabPageModuleCfg);
            this.tabControlUserAdd.Controls.Add(this.tabPageInputFingerPrint);
            this.tabControlUserAdd.Location = new System.Drawing.Point(1, 76);
            this.tabControlUserAdd.Name = "tabControlUserAdd";
            this.tabControlUserAdd.SelectedIndex = 0;
            this.tabControlUserAdd.Size = new System.Drawing.Size(574, 326);
            this.tabControlUserAdd.TabIndex = 40;
            // 
            // tabPageCardInfo
            // 
            this.tabPageCardInfo.Controls.Add(this.buttonCardInfoDelete);
            this.tabPageCardInfo.Controls.Add(this.buttonCardInfoModify);
            this.tabPageCardInfo.Controls.Add(this.buttonConfirm);
            this.tabPageCardInfo.Controls.Add(this.buttonCardInfoAdd);
            this.tabPageCardInfo.Controls.Add(this.listViewUserAddCardInfo);
            this.tabPageCardInfo.Location = new System.Drawing.Point(4, 22);
            this.tabPageCardInfo.Name = "tabPageCardInfo";
            this.tabPageCardInfo.Padding = new System.Windows.Forms.Padding(3);
            this.tabPageCardInfo.Size = new System.Drawing.Size(526, 300);
            this.tabPageCardInfo.TabIndex = 0;
            this.tabPageCardInfo.Text = "Card Info";
            this.tabPageCardInfo.UseVisualStyleBackColor = true;
            // 
            // buttonCardInfoDelete
            // 
            this.buttonCardInfoDelete.Enabled = false;
            this.buttonCardInfoDelete.ImeMode = System.Windows.Forms.ImeMode.NoControl;
            this.buttonCardInfoDelete.Location = new System.Drawing.Point(408, 261);
            this.buttonCardInfoDelete.Name = "buttonCardInfoDelete";
            this.buttonCardInfoDelete.Size = new System.Drawing.Size(107, 22);
            this.buttonCardInfoDelete.TabIndex = 27;
            this.buttonCardInfoDelete.Text = "Delete";
            this.buttonCardInfoDelete.UseVisualStyleBackColor = true;
            this.buttonCardInfoDelete.Click += new System.EventHandler(this.buttonCardInfoDelete_Click);
            // 
            // buttonCardInfoModify
            // 
            this.buttonCardInfoModify.Enabled = false;
            this.buttonCardInfoModify.ImeMode = System.Windows.Forms.ImeMode.NoControl;
            this.buttonCardInfoModify.Location = new System.Drawing.Point(283, 261);
            this.buttonCardInfoModify.Name = "buttonCardInfoModify";
            this.buttonCardInfoModify.Size = new System.Drawing.Size(107, 22);
            this.buttonCardInfoModify.TabIndex = 26;
            this.buttonCardInfoModify.Text = "Modify";
            this.buttonCardInfoModify.UseVisualStyleBackColor = true;
            this.buttonCardInfoModify.Click += new System.EventHandler(this.buttonCardInfoModify_Click);
            // 
            // buttonConfirm
            // 
            this.buttonConfirm.ImeMode = System.Windows.Forms.ImeMode.NoControl;
            this.buttonConfirm.Location = new System.Drawing.Point(158, 261);
            this.buttonConfirm.Name = "buttonConfirm";
            this.buttonConfirm.Size = new System.Drawing.Size(107, 22);
            this.buttonConfirm.TabIndex = 25;
            this.buttonConfirm.Text = "Confirm";
            this.buttonConfirm.UseVisualStyleBackColor = true;
            this.buttonConfirm.Click += new System.EventHandler(this.buttonConfirm_Click);
            // 
            // buttonCardInfoAdd
            // 
            this.buttonCardInfoAdd.ImeMode = System.Windows.Forms.ImeMode.NoControl;
            this.buttonCardInfoAdd.Location = new System.Drawing.Point(33, 261);
            this.buttonCardInfoAdd.Name = "buttonCardInfoAdd";
            this.buttonCardInfoAdd.Size = new System.Drawing.Size(107, 22);
            this.buttonCardInfoAdd.TabIndex = 25;
            this.buttonCardInfoAdd.Text = "Add";
            this.buttonCardInfoAdd.UseVisualStyleBackColor = true;
            this.buttonCardInfoAdd.Click += new System.EventHandler(this.buttonCardInfoAdd_Click);
            // 
            // listViewUserAddCardInfo
            // 
            this.listViewUserAddCardInfo.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader1,
            this.columnHeader2,
            this.columnHeader3,
            this.columnHeader4});
            this.listViewUserAddCardInfo.FullRowSelect = true;
            this.listViewUserAddCardInfo.GridLines = true;
            this.listViewUserAddCardInfo.HideSelection = false;
            this.listViewUserAddCardInfo.Location = new System.Drawing.Point(0, 0);
            this.listViewUserAddCardInfo.Margin = new System.Windows.Forms.Padding(0);
            this.listViewUserAddCardInfo.MultiSelect = false;
            this.listViewUserAddCardInfo.Name = "listViewUserAddCardInfo";
            this.listViewUserAddCardInfo.Size = new System.Drawing.Size(526, 242);
            this.listViewUserAddCardInfo.TabIndex = 3;
            this.listViewUserAddCardInfo.UseCompatibleStateImageBehavior = false;
            this.listViewUserAddCardInfo.View = System.Windows.Forms.View.Details;
            this.listViewUserAddCardInfo.SelectedIndexChanged += new System.EventHandler(this.listViewUserAddCardInfo_SelectedIndexChanged);
            this.listViewUserAddCardInfo.Click += new System.EventHandler(this.listViewUserAddCardInfo_Click);
            this.listViewUserAddCardInfo.Leave += new System.EventHandler(this.listViewUserAddCardInfo_Leave);
            this.listViewUserAddCardInfo.MouseDown += new System.Windows.Forms.MouseEventHandler(this.listViewUserAddCardInfo_MouseDown);
            // 
            // columnHeader1
            // 
            this.columnHeader1.Text = "Index";
            this.columnHeader1.Width = 100;
            // 
            // columnHeader2
            // 
            this.columnHeader2.Text = "Card No";
            this.columnHeader2.Width = 130;
            // 
            // columnHeader3
            // 
            this.columnHeader3.Text = "Card Type";
            this.columnHeader3.Width = 130;
            // 
            // columnHeader4
            // 
            this.columnHeader4.Text = "Card Validity";
            this.columnHeader4.Width = 250;
            // 
            // tabPageModuleCfg
            // 
            this.tabPageModuleCfg.Controls.Add(this.groupBox5);
            this.tabPageModuleCfg.Controls.Add(this.groupBox1);
            this.tabPageModuleCfg.Location = new System.Drawing.Point(4, 22);
            this.tabPageModuleCfg.Name = "tabPageModuleCfg";
            this.tabPageModuleCfg.Padding = new System.Windows.Forms.Padding(3);
            this.tabPageModuleCfg.Size = new System.Drawing.Size(566, 300);
            this.tabPageModuleCfg.TabIndex = 2;
            this.tabPageModuleCfg.Text = "Module Cfg";
            this.tabPageModuleCfg.UseVisualStyleBackColor = true;
            // 
            // groupBox5
            // 
            this.groupBox5.Controls.Add(this.buttonModify);
            this.groupBox5.Controls.Add(this.textBoxPlanNumber);
            this.groupBox5.Controls.Add(this.labelPlanNumber);
            this.groupBox5.Controls.Add(this.listViewCardAuthorityScheme);
            this.groupBox5.Location = new System.Drawing.Point(9, 106);
            this.groupBox5.Name = "groupBox5";
            this.groupBox5.Size = new System.Drawing.Size(549, 178);
            this.groupBox5.TabIndex = 43;
            this.groupBox5.TabStop = false;
            this.groupBox5.Text = "Card authority scheme";
            // 
            // buttonModify
            // 
            this.buttonModify.Location = new System.Drawing.Point(436, 132);
            this.buttonModify.Name = "buttonModify";
            this.buttonModify.Size = new System.Drawing.Size(65, 23);
            this.buttonModify.TabIndex = 3;
            this.buttonModify.Text = "Modify";
            this.buttonModify.UseVisualStyleBackColor = true;
            this.buttonModify.Click += new System.EventHandler(this.buttonModify_Click);
            // 
            // textBoxPlanNumber
            // 
            this.textBoxPlanNumber.Location = new System.Drawing.Point(436, 88);
            this.textBoxPlanNumber.Name = "textBoxPlanNumber";
            this.textBoxPlanNumber.Size = new System.Drawing.Size(62, 21);
            this.textBoxPlanNumber.TabIndex = 2;
            this.textBoxPlanNumber.Text = "0";
            // 
            // labelPlanNumber
            // 
            this.labelPlanNumber.AutoSize = true;
            this.labelPlanNumber.Location = new System.Drawing.Point(430, 73);
            this.labelPlanNumber.Name = "labelPlanNumber";
            this.labelPlanNumber.Size = new System.Drawing.Size(95, 12);
            this.labelPlanNumber.TabIndex = 1;
            this.labelPlanNumber.Text = "Template number";
            // 
            // listViewCardAuthorityScheme
            // 
            this.listViewCardAuthorityScheme.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader,
            this.columnHeader5,
            this.columnHeader6});
            this.listViewCardAuthorityScheme.FullRowSelect = true;
            this.listViewCardAuthorityScheme.GridLines = true;
            this.listViewCardAuthorityScheme.HideSelection = false;
            this.listViewCardAuthorityScheme.Location = new System.Drawing.Point(8, 20);
            this.listViewCardAuthorityScheme.Name = "listViewCardAuthorityScheme";
            this.listViewCardAuthorityScheme.Size = new System.Drawing.Size(419, 152);
            this.listViewCardAuthorityScheme.TabIndex = 0;
            this.listViewCardAuthorityScheme.TabStop = false;
            this.listViewCardAuthorityScheme.UseCompatibleStateImageBehavior = false;
            this.listViewCardAuthorityScheme.View = System.Windows.Forms.View.Details;
            // 
            // columnHeader
            // 
            this.columnHeader.Text = "Door number";
            this.columnHeader.Width = 80;
            // 
            // columnHeader5
            // 
            this.columnHeader5.Text = "Door permission number";
            this.columnHeader5.Width = 155;
            // 
            // columnHeader6
            // 
            this.columnHeader6.Text = "Template number";
            this.columnHeader6.Width = 105;
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.checkedListBoxDoorAuthority);
            this.groupBox1.Location = new System.Drawing.Point(7, 6);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(551, 94);
            this.groupBox1.TabIndex = 41;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Door authority";
            // 
            // checkedListBoxDoorAuthority
            // 
            this.checkedListBoxDoorAuthority.Items.AddRange(new object[] {
            "Door 1",
            "Door 2",
            "Door 3",
            "Door 4",
            "Door 5",
            "Door 6",
            "Door 7",
            "Door 8"});
            this.checkedListBoxDoorAuthority.Location = new System.Drawing.Point(9, 16);
            this.checkedListBoxDoorAuthority.MultiColumn = true;
            this.checkedListBoxDoorAuthority.Name = "checkedListBoxDoorAuthority";
            this.checkedListBoxDoorAuthority.Size = new System.Drawing.Size(536, 68);
            this.checkedListBoxDoorAuthority.TabIndex = 0;
            // 
            // tabPageInputFingerPrint
            // 
            this.tabPageInputFingerPrint.Controls.Add(this.groupBox2);
            this.tabPageInputFingerPrint.Location = new System.Drawing.Point(4, 22);
            this.tabPageInputFingerPrint.Name = "tabPageInputFingerPrint";
            this.tabPageInputFingerPrint.Padding = new System.Windows.Forms.Padding(3);
            this.tabPageInputFingerPrint.Size = new System.Drawing.Size(526, 300);
            this.tabPageInputFingerPrint.TabIndex = 3;
            this.tabPageInputFingerPrint.Text = "Input Finger Print";
            this.tabPageInputFingerPrint.UseVisualStyleBackColor = true;
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.buttonBrowse);
            this.groupBox2.Controls.Add(this.checkedListBoxReaderValid);
            this.groupBox2.Controls.Add(this.checkBoxNonBlock);
            this.groupBox2.Controls.Add(this.label7);
            this.groupBox2.Controls.Add(this.comboBoxFingerprintType);
            this.groupBox2.Controls.Add(this.label5);
            this.groupBox2.Controls.Add(this.textBoxDataLength);
            this.groupBox2.Controls.Add(this.label4);
            this.groupBox2.Controls.Add(this.textBoxFingerprintDataPath);
            this.groupBox2.Controls.Add(this.label3);
            this.groupBox2.Controls.Add(this.textBoxFingerPrintNumber);
            this.groupBox2.Controls.Add(this.label2);
            this.groupBox2.Location = new System.Drawing.Point(6, 6);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(510, 268);
            this.groupBox2.TabIndex = 10;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Fingerprint Information";
            // 
            // buttonBrowse
            // 
            this.buttonBrowse.Location = new System.Drawing.Point(418, 56);
            this.buttonBrowse.Name = "buttonBrowse";
            this.buttonBrowse.Size = new System.Drawing.Size(80, 23);
            this.buttonBrowse.TabIndex = 22;
            this.buttonBrowse.Text = "Browse";
            this.buttonBrowse.UseVisualStyleBackColor = true;
            this.buttonBrowse.Click += new System.EventHandler(this.buttonBrowse_Click);
            // 
            // checkedListBoxReaderValid
            // 
            this.checkedListBoxReaderValid.FormattingEnabled = true;
            this.checkedListBoxReaderValid.Items.AddRange(new object[] {
            "card reader 1",
            "card reader 2"});
            this.checkedListBoxReaderValid.Location = new System.Drawing.Point(19, 159);
            this.checkedListBoxReaderValid.Name = "checkedListBoxReaderValid";
            this.checkedListBoxReaderValid.Size = new System.Drawing.Size(120, 100);
            this.checkedListBoxReaderValid.TabIndex = 21;
            // 
            // checkBoxNonBlock
            // 
            this.checkBoxNonBlock.AutoSize = true;
            this.checkBoxNonBlock.Location = new System.Drawing.Point(285, 25);
            this.checkBoxNonBlock.Name = "checkBoxNonBlock";
            this.checkBoxNonBlock.Size = new System.Drawing.Size(174, 16);
            this.checkBoxNonBlock.TabIndex = 16;
            this.checkBoxNonBlock.Text = "EquipmentCallbackNonBlock";
            this.checkBoxNonBlock.UseVisualStyleBackColor = true;
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(17, 134);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(125, 12);
            this.label7.TabIndex = 12;
            this.label7.Text = "Card reader is valid";
            // 
            // comboBoxFingerprintType
            // 
            this.comboBoxFingerprintType.FormattingEnabled = true;
            this.comboBoxFingerprintType.Items.AddRange(new object[] {
            "Common fingerprint",
            "Stress fingerprint"});
            this.comboBoxFingerprintType.Location = new System.Drawing.Point(135, 93);
            this.comboBoxFingerprintType.Name = "comboBoxFingerprintType";
            this.comboBoxFingerprintType.Size = new System.Drawing.Size(100, 20);
            this.comboBoxFingerprintType.TabIndex = 9;
            this.comboBoxFingerprintType.Text = "Common fingerprint";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(17, 96);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(101, 12);
            this.label5.TabIndex = 8;
            this.label5.Text = "Fingerprint Type";
            // 
            // textBoxDataLength
            // 
            this.textBoxDataLength.Location = new System.Drawing.Point(365, 93);
            this.textBoxDataLength.Name = "textBoxDataLength";
            this.textBoxDataLength.Size = new System.Drawing.Size(100, 21);
            this.textBoxDataLength.TabIndex = 7;
            this.textBoxDataLength.Text = "384";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(276, 96);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(77, 12);
            this.label4.TabIndex = 6;
            this.label4.Text = " Data Length";
            // 
            // textBoxFingerprintDataPath
            // 
            this.textBoxFingerprintDataPath.Location = new System.Drawing.Point(135, 58);
            this.textBoxFingerprintDataPath.Name = "textBoxFingerprintDataPath";
            this.textBoxFingerprintDataPath.Size = new System.Drawing.Size(248, 21);
            this.textBoxFingerprintDataPath.TabIndex = 5;
            this.textBoxFingerprintDataPath.Text = "d:\\test.dat";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(17, 61);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(59, 12);
            this.label3.TabIndex = 4;
            this.label3.Text = "Data Path";
            // 
            // textBoxFingerPrintNumber
            // 
            this.textBoxFingerPrintNumber.Location = new System.Drawing.Point(133, 26);
            this.textBoxFingerPrintNumber.Name = "textBoxFingerPrintNumber";
            this.textBoxFingerPrintNumber.Size = new System.Drawing.Size(100, 21);
            this.textBoxFingerPrintNumber.TabIndex = 3;
            this.textBoxFingerPrintNumber.Text = "1";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(17, 29);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(113, 12);
            this.label2.TabIndex = 2;
            this.label2.Text = "Fingerprint Number";
            // 
            // UserAdd
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(575, 403);
            this.Controls.Add(this.tabControlUserAdd);
            this.Controls.Add(this.textBoxMemberName);
            this.Controls.Add(this.labelMemberName);
            this.Controls.Add(this.textBoxMemberNo);
            this.Controls.Add(this.labelMemberNo);
            this.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.SizableToolWindow;
            this.Name = "UserAdd";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "User Add";
            this.Load += new System.EventHandler(this.UserAdd_Load);
            this.tabControlUserAdd.ResumeLayout(false);
            this.tabPageCardInfo.ResumeLayout(false);
            this.tabPageModuleCfg.ResumeLayout(false);
            this.groupBox5.ResumeLayout(false);
            this.groupBox5.PerformLayout();
            this.groupBox1.ResumeLayout(false);
            this.tabPageInputFingerPrint.ResumeLayout(false);
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox textBoxMemberNo;
        private System.Windows.Forms.Label labelMemberNo;
        private System.Windows.Forms.TextBox textBoxMemberName;
        private System.Windows.Forms.Label labelMemberName;
        private System.Windows.Forms.TabControl tabControlUserAdd;
        private System.Windows.Forms.TabPage tabPageCardInfo;
        private System.Windows.Forms.TabPage tabPageModuleCfg;
        private System.Windows.Forms.TabPage tabPageInputFingerPrint;
        private System.Windows.Forms.ListView listViewUserAddCardInfo;
        private System.Windows.Forms.ColumnHeader columnHeader1;
        private System.Windows.Forms.ColumnHeader columnHeader2;
        private System.Windows.Forms.ColumnHeader columnHeader3;
        private System.Windows.Forms.Button buttonCardInfoAdd;
        private System.Windows.Forms.Button buttonCardInfoDelete;
        private System.Windows.Forms.Button buttonCardInfoModify;
        private System.Windows.Forms.ColumnHeader columnHeader4;
        private System.Windows.Forms.Button buttonConfirm;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.Button buttonBrowse;
        private System.Windows.Forms.CheckedListBox checkedListBoxReaderValid;
        private System.Windows.Forms.CheckBox checkBoxNonBlock;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.ComboBox comboBoxFingerprintType;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.TextBox textBoxDataLength;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.TextBox textBoxFingerprintDataPath;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.TextBox textBoxFingerPrintNumber;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.CheckedListBox checkedListBoxDoorAuthority;
        private System.Windows.Forms.GroupBox groupBox5;
        private System.Windows.Forms.Button buttonModify;
        private System.Windows.Forms.TextBox textBoxPlanNumber;
        private System.Windows.Forms.Label labelPlanNumber;
        private System.Windows.Forms.ListView listViewCardAuthorityScheme;
        private System.Windows.Forms.ColumnHeader columnHeader;
        private System.Windows.Forms.ColumnHeader columnHeader5;
        private System.Windows.Forms.ColumnHeader columnHeader6;
    }
}