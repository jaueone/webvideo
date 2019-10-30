namespace AcsDemo.ProductAcs
{
    partial class HolidayGroupPlan
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
            this.buttonSetTemplate = new System.Windows.Forms.Button();
            this.buttonGetTemplate = new System.Windows.Forms.Button();
            this.listViewHG = new System.Windows.Forms.ListView();
            this.columnHeader1 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader2 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.textBoxHolidayGroupNameHG = new System.Windows.Forms.TextBox();
            this.label10 = new System.Windows.Forms.Label();
            this.checkBoxEnableHG = new System.Windows.Forms.CheckBox();
            this.textBoxHolidayGroupNumberHG = new System.Windows.Forms.TextBox();
            this.label9 = new System.Windows.Forms.Label();
            this.comboBoxDeviceTypeHG = new System.Windows.Forms.ComboBox();
            this.label8 = new System.Windows.Forms.Label();
            this.textBoxLocalControllerID = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.textBoxHolidayNo = new System.Windows.Forms.TextBox();
            this.buttonUpdate = new System.Windows.Forms.Button();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.groupBox1.SuspendLayout();
            this.SuspendLayout();
            // 
            // buttonSetTemplate
            // 
            this.buttonSetTemplate.Location = new System.Drawing.Point(269, 256);
            this.buttonSetTemplate.Name = "buttonSetTemplate";
            this.buttonSetTemplate.Size = new System.Drawing.Size(75, 23);
            this.buttonSetTemplate.TabIndex = 20;
            this.buttonSetTemplate.Text = "Set";
            this.buttonSetTemplate.UseVisualStyleBackColor = true;
            this.buttonSetTemplate.Click += new System.EventHandler(this.buttonSetHG_Click);
            // 
            // buttonGetTemplate
            // 
            this.buttonGetTemplate.Location = new System.Drawing.Point(153, 256);
            this.buttonGetTemplate.Name = "buttonGetTemplate";
            this.buttonGetTemplate.Size = new System.Drawing.Size(75, 23);
            this.buttonGetTemplate.TabIndex = 17;
            this.buttonGetTemplate.Text = "Get";
            this.buttonGetTemplate.UseVisualStyleBackColor = true;
            this.buttonGetTemplate.Click += new System.EventHandler(this.buttonGetHG_Click);
            // 
            // listViewHG
            // 
            this.listViewHG.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader1,
            this.columnHeader2});
            this.listViewHG.FullRowSelect = true;
            this.listViewHG.GridLines = true;
            this.listViewHG.HideSelection = false;
            this.listViewHG.Location = new System.Drawing.Point(17, 90);
            this.listViewHG.Name = "listViewHG";
            this.listViewHG.Size = new System.Drawing.Size(211, 160);
            this.listViewHG.TabIndex = 26;
            this.listViewHG.UseCompatibleStateImageBehavior = false;
            this.listViewHG.View = System.Windows.Forms.View.Details;
            this.listViewHG.ItemSelectionChanged += new System.Windows.Forms.ListViewItemSelectionChangedEventHandler(this.listViewHG_ItemSelectionChanged);
            // 
            // columnHeader1
            // 
            this.columnHeader1.Text = "No.";
            this.columnHeader1.Width = 77;
            // 
            // columnHeader2
            // 
            this.columnHeader2.Text = "Holiday Plan No";
            this.columnHeader2.Width = 128;
            // 
            // textBoxHolidayGroupNameHG
            // 
            this.textBoxHolidayGroupNameHG.Location = new System.Drawing.Point(258, 53);
            this.textBoxHolidayGroupNameHG.Name = "textBoxHolidayGroupNameHG";
            this.textBoxHolidayGroupNameHG.Size = new System.Drawing.Size(179, 21);
            this.textBoxHolidayGroupNameHG.TabIndex = 25;
            this.textBoxHolidayGroupNameHG.Text = "test";
            this.textBoxHolidayGroupNameHG.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.textBoxHolidayGroupNameHG_KeyPress);
            // 
            // label10
            // 
            this.label10.AutoSize = true;
            this.label10.Location = new System.Drawing.Point(256, 22);
            this.label10.Name = "label10";
            this.label10.Size = new System.Drawing.Size(113, 12);
            this.label10.TabIndex = 24;
            this.label10.Text = "Holiday Group Name";
            // 
            // checkBoxEnableHG
            // 
            this.checkBoxEnableHG.AutoSize = true;
            this.checkBoxEnableHG.Checked = true;
            this.checkBoxEnableHG.CheckState = System.Windows.Forms.CheckState.Checked;
            this.checkBoxEnableHG.Location = new System.Drawing.Point(376, 93);
            this.checkBoxEnableHG.Name = "checkBoxEnableHG";
            this.checkBoxEnableHG.Size = new System.Drawing.Size(60, 16);
            this.checkBoxEnableHG.TabIndex = 18;
            this.checkBoxEnableHG.Text = "Enable";
            this.checkBoxEnableHG.UseVisualStyleBackColor = true;
            // 
            // textBoxHolidayGroupNumberHG
            // 
            this.textBoxHolidayGroupNumberHG.Location = new System.Drawing.Point(139, 53);
            this.textBoxHolidayGroupNumberHG.Name = "textBoxHolidayGroupNumberHG";
            this.textBoxHolidayGroupNumberHG.Size = new System.Drawing.Size(89, 21);
            this.textBoxHolidayGroupNumberHG.TabIndex = 23;
            this.textBoxHolidayGroupNumberHG.Text = "1";
            this.textBoxHolidayGroupNumberHG.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.textBoxHolidayGroupNumberHG_KeyPress);
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Location = new System.Drawing.Point(14, 56);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(125, 12);
            this.label9.TabIndex = 22;
            this.label9.Text = "Holiday Group Number";
            // 
            // comboBoxDeviceTypeHG
            // 
            this.comboBoxDeviceTypeHG.FormattingEnabled = true;
            this.comboBoxDeviceTypeHG.Items.AddRange(new object[] {
            "Door",
            "Card reader",
            "Card"});
            this.comboBoxDeviceTypeHG.Location = new System.Drawing.Point(139, 19);
            this.comboBoxDeviceTypeHG.Name = "comboBoxDeviceTypeHG";
            this.comboBoxDeviceTypeHG.Size = new System.Drawing.Size(89, 20);
            this.comboBoxDeviceTypeHG.TabIndex = 21;
            this.comboBoxDeviceTypeHG.Text = "Door";
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(15, 22);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(71, 12);
            this.label8.TabIndex = 19;
            this.label8.Text = "Device Type";
            // 
            // textBoxLocalControllerID
            // 
            this.textBoxLocalControllerID.Location = new System.Drawing.Point(353, 121);
            this.textBoxLocalControllerID.Name = "textBoxLocalControllerID";
            this.textBoxLocalControllerID.Size = new System.Drawing.Size(84, 21);
            this.textBoxLocalControllerID.TabIndex = 28;
            this.textBoxLocalControllerID.Text = "0";
            this.textBoxLocalControllerID.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.textBoxLocalControllerID_KeyPress);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(240, 124);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(107, 12);
            this.label1.TabIndex = 27;
            this.label1.Text = "LocalControllerID";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(3, 23);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(83, 12);
            this.label2.TabIndex = 27;
            this.label2.Text = "HolidayPlanNo";
            // 
            // textBoxHolidayNo
            // 
            this.textBoxHolidayNo.Location = new System.Drawing.Point(116, 20);
            this.textBoxHolidayNo.Name = "textBoxHolidayNo";
            this.textBoxHolidayNo.Size = new System.Drawing.Size(84, 21);
            this.textBoxHolidayNo.TabIndex = 28;
            this.textBoxHolidayNo.Text = "1";
            this.textBoxHolidayNo.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.textBoxHolidayNo_KeyPress);
            // 
            // buttonUpdate
            // 
            this.buttonUpdate.Location = new System.Drawing.Point(125, 49);
            this.buttonUpdate.Name = "buttonUpdate";
            this.buttonUpdate.Size = new System.Drawing.Size(75, 23);
            this.buttonUpdate.TabIndex = 20;
            this.buttonUpdate.Text = "Update";
            this.buttonUpdate.UseVisualStyleBackColor = true;
            this.buttonUpdate.Click += new System.EventHandler(this.buttonUpdate_Click);
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.textBoxHolidayNo);
            this.groupBox1.Controls.Add(this.label2);
            this.groupBox1.Controls.Add(this.buttonUpdate);
            this.groupBox1.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.groupBox1.Location = new System.Drawing.Point(237, 151);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(208, 87);
            this.groupBox1.TabIndex = 29;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Holiday Plan No Config";
            // 
            // HolidayGroupPlan
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(449, 288);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.textBoxLocalControllerID);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.buttonSetTemplate);
            this.Controls.Add(this.buttonGetTemplate);
            this.Controls.Add(this.listViewHG);
            this.Controls.Add(this.textBoxHolidayGroupNameHG);
            this.Controls.Add(this.label10);
            this.Controls.Add(this.checkBoxEnableHG);
            this.Controls.Add(this.textBoxHolidayGroupNumberHG);
            this.Controls.Add(this.label9);
            this.Controls.Add(this.comboBoxDeviceTypeHG);
            this.Controls.Add(this.label8);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
            this.Name = "HolidayGroupPlan";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "HolidayGroupPlan";
            this.Load += new System.EventHandler(this.HolidayGroupPlan_Load);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button buttonSetTemplate;
        private System.Windows.Forms.Button buttonGetTemplate;
        private System.Windows.Forms.ListView listViewHG;
        private System.Windows.Forms.ColumnHeader columnHeader1;
        private System.Windows.Forms.ColumnHeader columnHeader2;
        private System.Windows.Forms.TextBox textBoxHolidayGroupNameHG;
        private System.Windows.Forms.Label label10;
        private System.Windows.Forms.CheckBox checkBoxEnableHG;
        private System.Windows.Forms.TextBox textBoxHolidayGroupNumberHG;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.ComboBox comboBoxDeviceTypeHG;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.TextBox textBoxLocalControllerID;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox textBoxHolidayNo;
        private System.Windows.Forms.Button buttonUpdate;
        private System.Windows.Forms.GroupBox groupBox1;
    }
}