namespace AcsDemo.ProductAcs
{
    partial class HolidayPlan
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
            this.buttonAdd = new System.Windows.Forms.Button();
            this.dateTimePickerStartTime = new System.Windows.Forms.DateTimePicker();
            this.columnHeader6 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader5 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader4 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader3 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader2 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader1 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.listViewTimeSegment = new System.Windows.Forms.ListView();
            this.comboBoxVerificationMode = new System.Windows.Forms.ComboBox();
            this.comboBoxDoorStateMode = new System.Windows.Forms.ComboBox();
            this.dateTimePickerEndTime = new System.Windows.Forms.DateTimePicker();
            this.checkBoxEnableTimeSegment = new System.Windows.Forms.CheckBox();
            this.label5 = new System.Windows.Forms.Label();
            this.label7 = new System.Windows.Forms.Label();
            this.buttonSet = new System.Windows.Forms.Button();
            this.textBoxLocalControllerID = new System.Windows.Forms.TextBox();
            this.label4 = new System.Windows.Forms.Label();
            this.buttonGet = new System.Windows.Forms.Button();
            this.checkBoxEnablePlan = new System.Windows.Forms.CheckBox();
            this.label8 = new System.Windows.Forms.Label();
            this.textBoxHolidayPlanNumber = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.comboBoxDeviceType = new System.Windows.Forms.ComboBox();
            this.label1 = new System.Windows.Forms.Label();
            this.groupBox6 = new System.Windows.Forms.GroupBox();
            this.dateTimePickerEndDateHP = new System.Windows.Forms.DateTimePicker();
            this.label3 = new System.Windows.Forms.Label();
            this.dateTimePickerStartDateHP = new System.Windows.Forms.DateTimePicker();
            this.label9 = new System.Windows.Forms.Label();
            this.label6 = new System.Windows.Forms.Label();
            this.buttonConfirm = new System.Windows.Forms.Button();
            this.groupBox6.SuspendLayout();
            this.SuspendLayout();
            // 
            // buttonAdd
            // 
            this.buttonAdd.Location = new System.Drawing.Point(138, 357);
            this.buttonAdd.Name = "buttonAdd";
            this.buttonAdd.Size = new System.Drawing.Size(75, 23);
            this.buttonAdd.TabIndex = 47;
            this.buttonAdd.Text = "Update";
            this.buttonAdd.UseVisualStyleBackColor = true;
            this.buttonAdd.Click += new System.EventHandler(this.buttonAdd_Click);
            // 
            // dateTimePickerStartTime
            // 
            this.dateTimePickerStartTime.Format = System.Windows.Forms.DateTimePickerFormat.Time;
            this.dateTimePickerStartTime.Location = new System.Drawing.Point(391, 290);
            this.dateTimePickerStartTime.Name = "dateTimePickerStartTime";
            this.dateTimePickerStartTime.ShowUpDown = true;
            this.dateTimePickerStartTime.Size = new System.Drawing.Size(92, 21);
            this.dateTimePickerStartTime.TabIndex = 41;
            // 
            // columnHeader6
            // 
            this.columnHeader6.Text = "Stop time";
            this.columnHeader6.Width = 90;
            // 
            // columnHeader5
            // 
            this.columnHeader5.Text = "Open time";
            this.columnHeader5.Width = 90;
            // 
            // columnHeader4
            // 
            this.columnHeader4.Text = "Verify mode";
            this.columnHeader4.Width = 90;
            // 
            // columnHeader3
            // 
            this.columnHeader3.Text = "Door status mode";
            this.columnHeader3.Width = 90;
            // 
            // columnHeader2
            // 
            this.columnHeader2.Text = "Enable";
            this.columnHeader2.Width = 55;
            // 
            // columnHeader1
            // 
            this.columnHeader1.Text = "Time Segment";
            this.columnHeader1.Width = 89;
            // 
            // listViewTimeSegment
            // 
            this.listViewTimeSegment.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.listViewTimeSegment.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader1,
            this.columnHeader2,
            this.columnHeader3,
            this.columnHeader4,
            this.columnHeader5,
            this.columnHeader6});
            this.listViewTimeSegment.FullRowSelect = true;
            this.listViewTimeSegment.GridLines = true;
            this.listViewTimeSegment.HideSelection = false;
            this.listViewTimeSegment.Location = new System.Drawing.Point(12, 121);
            this.listViewTimeSegment.Name = "listViewTimeSegment";
            this.listViewTimeSegment.Size = new System.Drawing.Size(509, 157);
            this.listViewTimeSegment.TabIndex = 46;
            this.listViewTimeSegment.UseCompatibleStateImageBehavior = false;
            this.listViewTimeSegment.View = System.Windows.Forms.View.Details;
            this.listViewTimeSegment.ItemSelectionChanged += new System.Windows.Forms.ListViewItemSelectionChangedEventHandler(this.listViewTimeSegment_ItemSelectionChanged);
            // 
            // comboBoxVerificationMode
            // 
            this.comboBoxVerificationMode.FormattingEnabled = true;
            this.comboBoxVerificationMode.Items.AddRange(new object[] {
            "Invalid",
            "Sleep",
            "Card and Password",
            "Card",
            "Card or Password",
            "Fingerprint",
            "Fingerprint and Password",
            "Fingerprint or Card",
            "Fingerprint and Card",
            "Fingerprint and Card and Password"});
            this.comboBoxVerificationMode.Location = new System.Drawing.Point(137, 326);
            this.comboBoxVerificationMode.Name = "comboBoxVerificationMode";
            this.comboBoxVerificationMode.Size = new System.Drawing.Size(121, 20);
            this.comboBoxVerificationMode.TabIndex = 45;
            this.comboBoxVerificationMode.Text = "Invalid";
            // 
            // comboBoxDoorStateMode
            // 
            this.comboBoxDoorStateMode.FormattingEnabled = true;
            this.comboBoxDoorStateMode.Items.AddRange(new object[] {
            "Invalid",
            "Sleep",
            "Normally Open",
            "Normally Close"});
            this.comboBoxDoorStateMode.Location = new System.Drawing.Point(137, 290);
            this.comboBoxDoorStateMode.Name = "comboBoxDoorStateMode";
            this.comboBoxDoorStateMode.Size = new System.Drawing.Size(121, 20);
            this.comboBoxDoorStateMode.TabIndex = 44;
            this.comboBoxDoorStateMode.Text = "Invalid";
            // 
            // dateTimePickerEndTime
            // 
            this.dateTimePickerEndTime.Format = System.Windows.Forms.DateTimePickerFormat.Time;
            this.dateTimePickerEndTime.Location = new System.Drawing.Point(391, 323);
            this.dateTimePickerEndTime.Name = "dateTimePickerEndTime";
            this.dateTimePickerEndTime.ShowUpDown = true;
            this.dateTimePickerEndTime.Size = new System.Drawing.Size(92, 21);
            this.dateTimePickerEndTime.TabIndex = 43;
            // 
            // checkBoxEnableTimeSegment
            // 
            this.checkBoxEnableTimeSegment.AutoSize = true;
            this.checkBoxEnableTimeSegment.Location = new System.Drawing.Point(19, 285);
            this.checkBoxEnableTimeSegment.Name = "checkBoxEnableTimeSegment";
            this.checkBoxEnableTimeSegment.Size = new System.Drawing.Size(60, 16);
            this.checkBoxEnableTimeSegment.TabIndex = 17;
            this.checkBoxEnableTimeSegment.Text = "Enable";
            this.checkBoxEnableTimeSegment.UseVisualStyleBackColor = true;
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(307, 293);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(59, 12);
            this.label5.TabIndex = 42;
            this.label5.Text = "StartTime";
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(17, 217);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(95, 12);
            this.label7.TabIndex = 18;
            this.label7.Text = "Door State Mode";
            // 
            // buttonSet
            // 
            this.buttonSet.Location = new System.Drawing.Point(423, 398);
            this.buttonSet.Name = "buttonSet";
            this.buttonSet.Size = new System.Drawing.Size(75, 23);
            this.buttonSet.TabIndex = 40;
            this.buttonSet.Text = "Set";
            this.buttonSet.UseVisualStyleBackColor = true;
            this.buttonSet.Click += new System.EventHandler(this.buttonSet_Click);
            // 
            // textBoxLocalControllerID
            // 
            this.textBoxLocalControllerID.Location = new System.Drawing.Point(323, 55);
            this.textBoxLocalControllerID.Name = "textBoxLocalControllerID";
            this.textBoxLocalControllerID.Size = new System.Drawing.Size(84, 21);
            this.textBoxLocalControllerID.TabIndex = 36;
            this.textBoxLocalControllerID.Text = "0";
            this.textBoxLocalControllerID.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.textBoxLocalControllerID_KeyPress);
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(300, 253);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(47, 12);
            this.label4.TabIndex = 22;
            this.label4.Text = "EndTime";
            // 
            // buttonGet
            // 
            this.buttonGet.Location = new System.Drawing.Point(308, 398);
            this.buttonGet.Name = "buttonGet";
            this.buttonGet.Size = new System.Drawing.Size(75, 23);
            this.buttonGet.TabIndex = 38;
            this.buttonGet.Text = "Get";
            this.buttonGet.UseVisualStyleBackColor = true;
            this.buttonGet.Click += new System.EventHandler(this.buttonGet_Click);
            // 
            // checkBoxEnablePlan
            // 
            this.checkBoxEnablePlan.AutoSize = true;
            this.checkBoxEnablePlan.Checked = true;
            this.checkBoxEnablePlan.CheckState = System.Windows.Forms.CheckState.Checked;
            this.checkBoxEnablePlan.Location = new System.Drawing.Point(26, 45);
            this.checkBoxEnablePlan.Name = "checkBoxEnablePlan";
            this.checkBoxEnablePlan.Size = new System.Drawing.Size(60, 16);
            this.checkBoxEnablePlan.TabIndex = 37;
            this.checkBoxEnablePlan.Text = "Enable";
            this.checkBoxEnablePlan.UseVisualStyleBackColor = true;
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(203, 58);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(107, 12);
            this.label8.TabIndex = 34;
            this.label8.Text = "LocalControllerID";
            // 
            // textBoxHolidayPlanNumber
            // 
            this.textBoxHolidayPlanNumber.Location = new System.Drawing.Point(323, 12);
            this.textBoxHolidayPlanNumber.Name = "textBoxHolidayPlanNumber";
            this.textBoxHolidayPlanNumber.Size = new System.Drawing.Size(84, 21);
            this.textBoxHolidayPlanNumber.TabIndex = 35;
            this.textBoxHolidayPlanNumber.Text = "1";
            this.textBoxHolidayPlanNumber.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.textBoxHolidayPlanNumber_KeyPress);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label2.Location = new System.Drawing.Point(203, 15);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(119, 12);
            this.label2.TabIndex = 33;
            this.label2.Text = "Holiday Plan Number";
            // 
            // comboBoxDeviceType
            // 
            this.comboBoxDeviceType.FormattingEnabled = true;
            this.comboBoxDeviceType.Items.AddRange(new object[] {
            "Door",
            "Card reader",
            "Card"});
            this.comboBoxDeviceType.Location = new System.Drawing.Point(101, 12);
            this.comboBoxDeviceType.Name = "comboBoxDeviceType";
            this.comboBoxDeviceType.Size = new System.Drawing.Size(96, 20);
            this.comboBoxDeviceType.TabIndex = 31;
            this.comboBoxDeviceType.Text = "Door";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(24, 15);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(71, 12);
            this.label1.TabIndex = 29;
            this.label1.Text = "Device Type";
            // 
            // groupBox6
            // 
            this.groupBox6.Controls.Add(this.dateTimePickerEndDateHP);
            this.groupBox6.Controls.Add(this.label4);
            this.groupBox6.Controls.Add(this.label3);
            this.groupBox6.Controls.Add(this.label7);
            this.groupBox6.Controls.Add(this.dateTimePickerStartDateHP);
            this.groupBox6.Controls.Add(this.label9);
            this.groupBox6.Controls.Add(this.label6);
            this.groupBox6.Controls.Add(this.checkBoxEnableTimeSegment);
            this.groupBox6.Location = new System.Drawing.Point(7, 76);
            this.groupBox6.Name = "groupBox6";
            this.groupBox6.Size = new System.Drawing.Size(518, 314);
            this.groupBox6.TabIndex = 48;
            this.groupBox6.TabStop = false;
            this.groupBox6.Text = "Time Configuration";
            // 
            // dateTimePickerEndDateHP
            // 
            this.dateTimePickerEndDateHP.CustomFormat = "MM/dd/yyyy";
            this.dateTimePickerEndDateHP.Format = System.Windows.Forms.DateTimePickerFormat.Custom;
            this.dateTimePickerEndDateHP.Location = new System.Drawing.Point(257, 18);
            this.dateTimePickerEndDateHP.Name = "dateTimePickerEndDateHP";
            this.dateTimePickerEndDateHP.Size = new System.Drawing.Size(102, 21);
            this.dateTimePickerEndDateHP.TabIndex = 52;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(196, 24);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(53, 12);
            this.label3.TabIndex = 51;
            this.label3.Text = "End Date";
            // 
            // dateTimePickerStartDateHP
            // 
            this.dateTimePickerStartDateHP.CustomFormat = "MM/dd/yyyy";
            this.dateTimePickerStartDateHP.Format = System.Windows.Forms.DateTimePickerFormat.Custom;
            this.dateTimePickerStartDateHP.Location = new System.Drawing.Point(87, 18);
            this.dateTimePickerStartDateHP.Name = "dateTimePickerStartDateHP";
            this.dateTimePickerStartDateHP.Size = new System.Drawing.Size(103, 21);
            this.dateTimePickerStartDateHP.TabIndex = 50;
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Location = new System.Drawing.Point(14, 24);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(65, 12);
            this.label9.TabIndex = 49;
            this.label9.Text = "Start Date";
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(17, 253);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(107, 12);
            this.label6.TabIndex = 19;
            this.label6.Text = "Verification Mode";
            // 
            // buttonConfirm
            // 
            this.buttonConfirm.Location = new System.Drawing.Point(193, 398);
            this.buttonConfirm.Name = "buttonConfirm";
            this.buttonConfirm.Size = new System.Drawing.Size(75, 23);
            this.buttonConfirm.TabIndex = 39;
            this.buttonConfirm.Text = "Confirm";
            this.buttonConfirm.UseVisualStyleBackColor = true;
            this.buttonConfirm.Click += new System.EventHandler(this.buttonConfirm_Click);
            // 
            // HolidayPlan
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(529, 423);
            this.Controls.Add(this.buttonAdd);
            this.Controls.Add(this.dateTimePickerStartTime);
            this.Controls.Add(this.listViewTimeSegment);
            this.Controls.Add(this.comboBoxVerificationMode);
            this.Controls.Add(this.comboBoxDoorStateMode);
            this.Controls.Add(this.dateTimePickerEndTime);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.buttonSet);
            this.Controls.Add(this.textBoxLocalControllerID);
            this.Controls.Add(this.buttonGet);
            this.Controls.Add(this.checkBoxEnablePlan);
            this.Controls.Add(this.label8);
            this.Controls.Add(this.textBoxHolidayPlanNumber);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.comboBoxDeviceType);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.groupBox6);
            this.Controls.Add(this.buttonConfirm);
            this.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
            this.Name = "HolidayPlan";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "HolidayPlan";
            this.Load += new System.EventHandler(this.HolidayPlan_Load);
            this.groupBox6.ResumeLayout(false);
            this.groupBox6.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button buttonAdd;
        private System.Windows.Forms.DateTimePicker dateTimePickerStartTime;
        private System.Windows.Forms.ColumnHeader columnHeader6;
        private System.Windows.Forms.ColumnHeader columnHeader5;
        private System.Windows.Forms.ColumnHeader columnHeader4;
        private System.Windows.Forms.ColumnHeader columnHeader3;
        private System.Windows.Forms.ColumnHeader columnHeader2;
        private System.Windows.Forms.ColumnHeader columnHeader1;
        private System.Windows.Forms.ListView listViewTimeSegment;
        private System.Windows.Forms.ComboBox comboBoxVerificationMode;
        private System.Windows.Forms.ComboBox comboBoxDoorStateMode;
        private System.Windows.Forms.DateTimePicker dateTimePickerEndTime;
        private System.Windows.Forms.CheckBox checkBoxEnableTimeSegment;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.Button buttonSet;
        private System.Windows.Forms.TextBox textBoxLocalControllerID;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Button buttonGet;
        private System.Windows.Forms.CheckBox checkBoxEnablePlan;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.TextBox textBoxHolidayPlanNumber;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.ComboBox comboBoxDeviceType;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.GroupBox groupBox6;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Button buttonConfirm;
        private System.Windows.Forms.DateTimePicker dateTimePickerEndDateHP;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.DateTimePicker dateTimePickerStartDateHP;
        private System.Windows.Forms.Label label9;

    }
}