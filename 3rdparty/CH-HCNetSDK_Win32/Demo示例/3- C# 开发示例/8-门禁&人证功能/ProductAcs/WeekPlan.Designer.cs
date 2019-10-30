namespace AcsDemo.ProductAcs
{
    partial class WeekPlan
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
            this.buttonSetWP = new System.Windows.Forms.Button();
            this.buttonGetWP = new System.Windows.Forms.Button();
            this.checkBoxEnableWP = new System.Windows.Forms.CheckBox();
            this.textBoxWeekPlanNumberWP = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.comboBoxDeviceTypeWP = new System.Windows.Forms.ComboBox();
            this.label1 = new System.Windows.Forms.Label();
            this.comboBoxVerificationMode = new System.Windows.Forms.ComboBox();
            this.comboBoxDoorStateMode = new System.Windows.Forms.ComboBox();
            this.dateTimePickerEndTime = new System.Windows.Forms.DateTimePicker();
            this.label4 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.dateTimePickerStartTime = new System.Windows.Forms.DateTimePicker();
            this.label6 = new System.Windows.Forms.Label();
            this.label7 = new System.Windows.Forms.Label();
            this.checkBoxEnable = new System.Windows.Forms.CheckBox();
            this.listViewTimeSegment = new System.Windows.Forms.ListView();
            this.columnHeader1 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader2 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader3 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader4 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader5 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader6 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.groupBox6 = new System.Windows.Forms.GroupBox();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.buttonAdd = new System.Windows.Forms.Button();
            this.label8 = new System.Windows.Forms.Label();
            this.textBoxLocalControllerIDWP = new System.Windows.Forms.TextBox();
            this.buttonConfirm = new System.Windows.Forms.Button();
            this.comboBoxDateWP = new System.Windows.Forms.ComboBox();
            this.label3 = new System.Windows.Forms.Label();
            this.groupBox6.SuspendLayout();
            this.groupBox1.SuspendLayout();
            this.SuspendLayout();
            // 
            // buttonSetWP
            // 
            this.buttonSetWP.Location = new System.Drawing.Point(419, 413);
            this.buttonSetWP.Name = "buttonSetWP";
            this.buttonSetWP.Size = new System.Drawing.Size(75, 23);
            this.buttonSetWP.TabIndex = 16;
            this.buttonSetWP.Text = "Set";
            this.buttonSetWP.UseVisualStyleBackColor = true;
            this.buttonSetWP.Click += new System.EventHandler(this.buttonSetWP_Click);
            // 
            // buttonGetWP
            // 
            this.buttonGetWP.Location = new System.Drawing.Point(304, 413);
            this.buttonGetWP.Name = "buttonGetWP";
            this.buttonGetWP.Size = new System.Drawing.Size(75, 23);
            this.buttonGetWP.TabIndex = 15;
            this.buttonGetWP.Text = "Get";
            this.buttonGetWP.UseVisualStyleBackColor = true;
            this.buttonGetWP.Click += new System.EventHandler(this.buttonGetWP_Click);
            // 
            // checkBoxEnableWP
            // 
            this.checkBoxEnableWP.AutoSize = true;
            this.checkBoxEnableWP.Checked = true;
            this.checkBoxEnableWP.CheckState = System.Windows.Forms.CheckState.Checked;
            this.checkBoxEnableWP.Location = new System.Drawing.Point(22, 52);
            this.checkBoxEnableWP.Name = "checkBoxEnableWP";
            this.checkBoxEnableWP.Size = new System.Drawing.Size(60, 16);
            this.checkBoxEnableWP.TabIndex = 14;
            this.checkBoxEnableWP.Text = "Enable";
            this.checkBoxEnableWP.UseVisualStyleBackColor = true;
            // 
            // textBoxWeekPlanNumberWP
            // 
            this.textBoxWeekPlanNumberWP.Location = new System.Drawing.Point(306, 15);
            this.textBoxWeekPlanNumberWP.Name = "textBoxWeekPlanNumberWP";
            this.textBoxWeekPlanNumberWP.Size = new System.Drawing.Size(84, 21);
            this.textBoxWeekPlanNumberWP.TabIndex = 13;
            this.textBoxWeekPlanNumberWP.Text = "1";
            this.textBoxWeekPlanNumberWP.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.textBoxWeekPlanNumberWP_KeyPress);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(199, 18);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(101, 12);
            this.label2.TabIndex = 12;
            this.label2.Text = "Week Plan Number";
            // 
            // comboBoxDeviceTypeWP
            // 
            this.comboBoxDeviceTypeWP.FormattingEnabled = true;
            this.comboBoxDeviceTypeWP.Items.AddRange(new object[] {
            "Door",
            "Card reader",
            "Card"});
            this.comboBoxDeviceTypeWP.Location = new System.Drawing.Point(97, 15);
            this.comboBoxDeviceTypeWP.Name = "comboBoxDeviceTypeWP";
            this.comboBoxDeviceTypeWP.Size = new System.Drawing.Size(96, 20);
            this.comboBoxDeviceTypeWP.TabIndex = 10;
            this.comboBoxDeviceTypeWP.Text = "Door";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(20, 18);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(71, 12);
            this.label1.TabIndex = 8;
            this.label1.Text = "Device Type";
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
            this.comboBoxVerificationMode.Location = new System.Drawing.Point(139, 343);
            this.comboBoxVerificationMode.Name = "comboBoxVerificationMode";
            this.comboBoxVerificationMode.Size = new System.Drawing.Size(121, 20);
            this.comboBoxVerificationMode.TabIndex = 25;
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
            this.comboBoxDoorStateMode.Location = new System.Drawing.Point(139, 307);
            this.comboBoxDoorStateMode.Name = "comboBoxDoorStateMode";
            this.comboBoxDoorStateMode.Size = new System.Drawing.Size(121, 20);
            this.comboBoxDoorStateMode.TabIndex = 24;
            this.comboBoxDoorStateMode.Text = "Invalid";
            // 
            // dateTimePickerEndTime
            // 
            this.dateTimePickerEndTime.Format = System.Windows.Forms.DateTimePickerFormat.Time;
            this.dateTimePickerEndTime.Location = new System.Drawing.Point(393, 340);
            this.dateTimePickerEndTime.Name = "dateTimePickerEndTime";
            this.dateTimePickerEndTime.ShowUpDown = true;
            this.dateTimePickerEndTime.Size = new System.Drawing.Size(92, 21);
            this.dateTimePickerEndTime.TabIndex = 23;
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(303, 62);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(47, 12);
            this.label4.TabIndex = 22;
            this.label4.Text = "EndTime";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(309, 310);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(59, 12);
            this.label5.TabIndex = 21;
            this.label5.Text = "StartTime";
            // 
            // dateTimePickerStartTime
            // 
            this.dateTimePickerStartTime.Format = System.Windows.Forms.DateTimePickerFormat.Time;
            this.dateTimePickerStartTime.Location = new System.Drawing.Point(393, 307);
            this.dateTimePickerStartTime.Name = "dateTimePickerStartTime";
            this.dateTimePickerStartTime.ShowUpDown = true;
            this.dateTimePickerStartTime.Size = new System.Drawing.Size(92, 21);
            this.dateTimePickerStartTime.TabIndex = 20;
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(11, 64);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(107, 12);
            this.label6.TabIndex = 19;
            this.label6.Text = "Verification Mode";
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(11, 28);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(95, 12);
            this.label7.TabIndex = 18;
            this.label7.Text = "Door State Mode";
            // 
            // checkBoxEnable
            // 
            this.checkBoxEnable.AutoSize = true;
            this.checkBoxEnable.Location = new System.Drawing.Point(13, 94);
            this.checkBoxEnable.Name = "checkBoxEnable";
            this.checkBoxEnable.Size = new System.Drawing.Size(60, 16);
            this.checkBoxEnable.TabIndex = 17;
            this.checkBoxEnable.Text = "Enable";
            this.checkBoxEnable.UseVisualStyleBackColor = true;
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
            this.listViewTimeSegment.Location = new System.Drawing.Point(4, 40);
            this.listViewTimeSegment.Name = "listViewTimeSegment";
            this.listViewTimeSegment.Size = new System.Drawing.Size(515, 157);
            this.listViewTimeSegment.TabIndex = 26;
            this.listViewTimeSegment.UseCompatibleStateImageBehavior = false;
            this.listViewTimeSegment.View = System.Windows.Forms.View.Details;
            this.listViewTimeSegment.ItemSelectionChanged += new System.Windows.Forms.ListViewItemSelectionChangedEventHandler(this.listViewTimeSegment_ItemSelectionChanged);
            // 
            // columnHeader1
            // 
            this.columnHeader1.Text = "Time Segment";
            this.columnHeader1.Width = 89;
            // 
            // columnHeader2
            // 
            this.columnHeader2.Text = "Enable";
            this.columnHeader2.Width = 55;
            // 
            // columnHeader3
            // 
            this.columnHeader3.Text = "Door status mode";
            this.columnHeader3.Width = 111;
            // 
            // columnHeader4
            // 
            this.columnHeader4.Text = "Verify mode";
            this.columnHeader4.Width = 103;
            // 
            // columnHeader5
            // 
            this.columnHeader5.Text = "Open time";
            this.columnHeader5.Width = 80;
            // 
            // columnHeader6
            // 
            this.columnHeader6.Text = "Stop time";
            this.columnHeader6.Width = 90;
            // 
            // groupBox6
            // 
            this.groupBox6.Controls.Add(this.groupBox1);
            this.groupBox6.Controls.Add(this.listViewTimeSegment);
            this.groupBox6.Location = new System.Drawing.Point(3, 79);
            this.groupBox6.Name = "groupBox6";
            this.groupBox6.Size = new System.Drawing.Size(525, 328);
            this.groupBox6.TabIndex = 28;
            this.groupBox6.TabStop = false;
            this.groupBox6.Text = "Time Configuration";
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.label4);
            this.groupBox1.Controls.Add(this.label7);
            this.groupBox1.Controls.Add(this.label6);
            this.groupBox1.Controls.Add(this.checkBoxEnable);
            this.groupBox1.Location = new System.Drawing.Point(4, 203);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(497, 119);
            this.groupBox1.TabIndex = 27;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "EditTimeSegment";
            // 
            // buttonAdd
            // 
            this.buttonAdd.Location = new System.Drawing.Point(140, 374);
            this.buttonAdd.Name = "buttonAdd";
            this.buttonAdd.Size = new System.Drawing.Size(75, 23);
            this.buttonAdd.TabIndex = 27;
            this.buttonAdd.Text = "Update";
            this.buttonAdd.UseVisualStyleBackColor = true;
            this.buttonAdd.Click += new System.EventHandler(this.buttonAdd_Click);
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(199, 60);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(107, 12);
            this.label8.TabIndex = 12;
            this.label8.Text = "LocalControllerID";
            // 
            // textBoxLocalControllerIDWP
            // 
            this.textBoxLocalControllerIDWP.Location = new System.Drawing.Point(306, 54);
            this.textBoxLocalControllerIDWP.Name = "textBoxLocalControllerIDWP";
            this.textBoxLocalControllerIDWP.Size = new System.Drawing.Size(84, 21);
            this.textBoxLocalControllerIDWP.TabIndex = 13;
            this.textBoxLocalControllerIDWP.Text = "0";
            this.textBoxLocalControllerIDWP.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.textBoxLocalControllerIDWP_KeyPress);
            // 
            // buttonConfirm
            // 
            this.buttonConfirm.Location = new System.Drawing.Point(189, 413);
            this.buttonConfirm.Name = "buttonConfirm";
            this.buttonConfirm.Size = new System.Drawing.Size(75, 23);
            this.buttonConfirm.TabIndex = 15;
            this.buttonConfirm.Text = "Confirm";
            this.buttonConfirm.UseVisualStyleBackColor = true;
            this.buttonConfirm.Click += new System.EventHandler(this.buttonConfirm_Click);
            // 
            // comboBoxDateWP
            // 
            this.comboBoxDateWP.FormattingEnabled = true;
            this.comboBoxDateWP.Items.AddRange(new object[] {
            "Monday",
            "Tuesday",
            "Wednesday",
            "Thursday",
            "Friday",
            "Saturday",
            "Sunday"});
            this.comboBoxDateWP.Location = new System.Drawing.Point(199, 93);
            this.comboBoxDateWP.Name = "comboBoxDateWP";
            this.comboBoxDateWP.Size = new System.Drawing.Size(101, 20);
            this.comboBoxDateWP.TabIndex = 9;
            this.comboBoxDateWP.Text = "Monday";
            this.comboBoxDateWP.SelectedIndexChanged += new System.EventHandler(this.comboBoxDateWP_SelectedIndexChanged);
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(158, 96);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(29, 12);
            this.label3.TabIndex = 11;
            this.label3.Text = "Date";
            // 
            // WeekPlan
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(530, 440);
            this.Controls.Add(this.buttonAdd);
            this.Controls.Add(this.comboBoxVerificationMode);
            this.Controls.Add(this.comboBoxDoorStateMode);
            this.Controls.Add(this.dateTimePickerEndTime);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.dateTimePickerStartTime);
            this.Controls.Add(this.buttonSetWP);
            this.Controls.Add(this.buttonConfirm);
            this.Controls.Add(this.buttonGetWP);
            this.Controls.Add(this.checkBoxEnableWP);
            this.Controls.Add(this.textBoxLocalControllerIDWP);
            this.Controls.Add(this.label8);
            this.Controls.Add(this.textBoxWeekPlanNumberWP);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.comboBoxDeviceTypeWP);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.comboBoxDateWP);
            this.Controls.Add(this.groupBox6);
            this.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
            this.Name = "WeekPlan";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "WeekPlan";
            this.Load += new System.EventHandler(this.WeekPlan_Load);
            this.groupBox6.ResumeLayout(false);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button buttonSetWP;
        private System.Windows.Forms.Button buttonGetWP;
        private System.Windows.Forms.CheckBox checkBoxEnableWP;
        private System.Windows.Forms.TextBox textBoxWeekPlanNumberWP;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.ComboBox comboBoxDeviceTypeWP;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.ComboBox comboBoxVerificationMode;
        private System.Windows.Forms.ComboBox comboBoxDoorStateMode;
        private System.Windows.Forms.DateTimePicker dateTimePickerEndTime;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.DateTimePicker dateTimePickerStartTime;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.CheckBox checkBoxEnable;
        private System.Windows.Forms.ListView listViewTimeSegment;
        private System.Windows.Forms.ColumnHeader columnHeader1;
        private System.Windows.Forms.ColumnHeader columnHeader2;
        private System.Windows.Forms.ColumnHeader columnHeader3;
        private System.Windows.Forms.ColumnHeader columnHeader4;
        private System.Windows.Forms.ColumnHeader columnHeader5;
        private System.Windows.Forms.ColumnHeader columnHeader6;
        private System.Windows.Forms.GroupBox groupBox6;
        private System.Windows.Forms.Button buttonAdd;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.TextBox textBoxLocalControllerIDWP;
        private System.Windows.Forms.Button buttonConfirm;
        private System.Windows.Forms.ComboBox comboBoxDateWP;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.GroupBox groupBox1;
    }
}