namespace AcsDemo.ProductAcs
{
    partial class PlanTemplate
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
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.textBoxWeekPlanNo = new System.Windows.Forms.TextBox();
            this.textBoxHolidayGroupNo = new System.Windows.Forms.TextBox();
            this.label3 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.buttonUpdate = new System.Windows.Forms.Button();
            this.textBoxLocalControllerID = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.buttonSetTemplate = new System.Windows.Forms.Button();
            this.buttonGetTemplate = new System.Windows.Forms.Button();
            this.listViewTemplate = new System.Windows.Forms.ListView();
            this.columnHeader1 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader2 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.textBoxTemplateName = new System.Windows.Forms.TextBox();
            this.label10 = new System.Windows.Forms.Label();
            this.checkBoxEnableHG = new System.Windows.Forms.CheckBox();
            this.textBoxTemplateNumber = new System.Windows.Forms.TextBox();
            this.label9 = new System.Windows.Forms.Label();
            this.comboBoxDeviceTypeHG = new System.Windows.Forms.ComboBox();
            this.label8 = new System.Windows.Forms.Label();
            this.groupBox1.SuspendLayout();
            this.SuspendLayout();
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.textBoxWeekPlanNo);
            this.groupBox1.Controls.Add(this.textBoxHolidayGroupNo);
            this.groupBox1.Controls.Add(this.label3);
            this.groupBox1.Controls.Add(this.label2);
            this.groupBox1.Controls.Add(this.buttonUpdate);
            this.groupBox1.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.groupBox1.Location = new System.Drawing.Point(232, 146);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(208, 99);
            this.groupBox1.TabIndex = 42;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Template Config";
            // 
            // textBoxWeekPlanNo
            // 
            this.textBoxWeekPlanNo.Location = new System.Drawing.Point(116, 45);
            this.textBoxWeekPlanNo.Name = "textBoxWeekPlanNo";
            this.textBoxWeekPlanNo.Size = new System.Drawing.Size(84, 21);
            this.textBoxWeekPlanNo.TabIndex = 28;
            this.textBoxWeekPlanNo.Text = "1";
            this.textBoxWeekPlanNo.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.textBoxWeekPlanNo_KeyPress);
            // 
            // textBoxHolidayGroupNo
            // 
            this.textBoxHolidayGroupNo.Location = new System.Drawing.Point(116, 20);
            this.textBoxHolidayGroupNo.Name = "textBoxHolidayGroupNo";
            this.textBoxHolidayGroupNo.Size = new System.Drawing.Size(84, 21);
            this.textBoxHolidayGroupNo.TabIndex = 28;
            this.textBoxHolidayGroupNo.Text = "1";
            this.textBoxHolidayGroupNo.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.textBoxHolidayGroupNo_KeyPress);
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(3, 48);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(65, 12);
            this.label3.TabIndex = 27;
            this.label3.Text = "WeekPlanNo";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(3, 23);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(113, 12);
            this.label2.TabIndex = 27;
            this.label2.Text = "HolidayGroupPlanNo";
            // 
            // buttonUpdate
            // 
            this.buttonUpdate.Location = new System.Drawing.Point(124, 72);
            this.buttonUpdate.Name = "buttonUpdate";
            this.buttonUpdate.Size = new System.Drawing.Size(75, 23);
            this.buttonUpdate.TabIndex = 20;
            this.buttonUpdate.Text = "Update";
            this.buttonUpdate.UseVisualStyleBackColor = true;
            this.buttonUpdate.Click += new System.EventHandler(this.buttonUpdate_Click);
            // 
            // textBoxLocalControllerID
            // 
            this.textBoxLocalControllerID.Location = new System.Drawing.Point(348, 116);
            this.textBoxLocalControllerID.Name = "textBoxLocalControllerID";
            this.textBoxLocalControllerID.Size = new System.Drawing.Size(84, 21);
            this.textBoxLocalControllerID.TabIndex = 41;
            this.textBoxLocalControllerID.Text = "0";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(235, 119);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(107, 12);
            this.label1.TabIndex = 40;
            this.label1.Text = "LocalControllerID";
            // 
            // buttonSetTemplate
            // 
            this.buttonSetTemplate.Location = new System.Drawing.Point(264, 253);
            this.buttonSetTemplate.Name = "buttonSetTemplate";
            this.buttonSetTemplate.Size = new System.Drawing.Size(75, 23);
            this.buttonSetTemplate.TabIndex = 33;
            this.buttonSetTemplate.Text = "Set";
            this.buttonSetTemplate.UseVisualStyleBackColor = true;
            this.buttonSetTemplate.Click += new System.EventHandler(this.buttonSetTemplate_Click);
            // 
            // buttonGetTemplate
            // 
            this.buttonGetTemplate.Location = new System.Drawing.Point(148, 253);
            this.buttonGetTemplate.Name = "buttonGetTemplate";
            this.buttonGetTemplate.Size = new System.Drawing.Size(75, 23);
            this.buttonGetTemplate.TabIndex = 30;
            this.buttonGetTemplate.Text = "Get";
            this.buttonGetTemplate.UseVisualStyleBackColor = true;
            this.buttonGetTemplate.Click += new System.EventHandler(this.buttonGetTemplate_Click);
            // 
            // listViewTemplate
            // 
            this.listViewTemplate.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader1,
            this.columnHeader2});
            this.listViewTemplate.FullRowSelect = true;
            this.listViewTemplate.GridLines = true;
            this.listViewTemplate.HideSelection = false;
            this.listViewTemplate.Location = new System.Drawing.Point(12, 85);
            this.listViewTemplate.Name = "listViewTemplate";
            this.listViewTemplate.Size = new System.Drawing.Size(217, 160);
            this.listViewTemplate.TabIndex = 39;
            this.listViewTemplate.UseCompatibleStateImageBehavior = false;
            this.listViewTemplate.View = System.Windows.Forms.View.Details;
            this.listViewTemplate.ItemSelectionChanged += new System.Windows.Forms.ListViewItemSelectionChangedEventHandler(this.listViewTemplate_ItemSelectionChanged);
            // 
            // columnHeader1
            // 
            this.columnHeader1.Text = "No.";
            this.columnHeader1.Width = 33;
            // 
            // columnHeader2
            // 
            this.columnHeader2.Text = "HolidayGroupNo";
            this.columnHeader2.Width = 97;
            // 
            // textBoxTemplateName
            // 
            this.textBoxTemplateName.Location = new System.Drawing.Point(253, 48);
            this.textBoxTemplateName.Name = "textBoxTemplateName";
            this.textBoxTemplateName.Size = new System.Drawing.Size(179, 21);
            this.textBoxTemplateName.TabIndex = 38;
            this.textBoxTemplateName.Text = "test";
            this.textBoxTemplateName.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.textBoxTemplateName_KeyPress);
            // 
            // label10
            // 
            this.label10.AutoSize = true;
            this.label10.Location = new System.Drawing.Point(251, 22);
            this.label10.Name = "label10";
            this.label10.Size = new System.Drawing.Size(83, 12);
            this.label10.TabIndex = 37;
            this.label10.Text = "Template Name";
            // 
            // checkBoxEnableHG
            // 
            this.checkBoxEnableHG.AutoSize = true;
            this.checkBoxEnableHG.Checked = true;
            this.checkBoxEnableHG.CheckState = System.Windows.Forms.CheckState.Checked;
            this.checkBoxEnableHG.Location = new System.Drawing.Point(371, 88);
            this.checkBoxEnableHG.Name = "checkBoxEnableHG";
            this.checkBoxEnableHG.Size = new System.Drawing.Size(60, 16);
            this.checkBoxEnableHG.TabIndex = 31;
            this.checkBoxEnableHG.Text = "Enable";
            this.checkBoxEnableHG.UseVisualStyleBackColor = true;
            // 
            // textBoxTemplateNumber
            // 
            this.textBoxTemplateNumber.Location = new System.Drawing.Point(134, 48);
            this.textBoxTemplateNumber.Name = "textBoxTemplateNumber";
            this.textBoxTemplateNumber.Size = new System.Drawing.Size(89, 21);
            this.textBoxTemplateNumber.TabIndex = 36;
            this.textBoxTemplateNumber.Text = "1";
            this.textBoxTemplateNumber.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.textBoxTemplateNumber_KeyPress);
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Location = new System.Drawing.Point(9, 51);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(125, 12);
            this.label9.TabIndex = 35;
            this.label9.Text = "Plan Template Number";
            // 
            // comboBoxDeviceTypeHG
            // 
            this.comboBoxDeviceTypeHG.FormattingEnabled = true;
            this.comboBoxDeviceTypeHG.Items.AddRange(new object[] {
            "Door",
            "Card reader",
            "Card"});
            this.comboBoxDeviceTypeHG.Location = new System.Drawing.Point(134, 14);
            this.comboBoxDeviceTypeHG.Name = "comboBoxDeviceTypeHG";
            this.comboBoxDeviceTypeHG.Size = new System.Drawing.Size(89, 20);
            this.comboBoxDeviceTypeHG.TabIndex = 34;
            this.comboBoxDeviceTypeHG.Text = "Door";
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(10, 17);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(71, 12);
            this.label8.TabIndex = 32;
            this.label8.Text = "Device Type";
            // 
            // PlanTemplate
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(444, 285);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.textBoxLocalControllerID);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.buttonSetTemplate);
            this.Controls.Add(this.buttonGetTemplate);
            this.Controls.Add(this.listViewTemplate);
            this.Controls.Add(this.textBoxTemplateName);
            this.Controls.Add(this.label10);
            this.Controls.Add(this.checkBoxEnableHG);
            this.Controls.Add(this.textBoxTemplateNumber);
            this.Controls.Add(this.label9);
            this.Controls.Add(this.comboBoxDeviceTypeHG);
            this.Controls.Add(this.label8);
            this.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
            this.Name = "PlanTemplate";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "PlanTemplate";
            this.Load += new System.EventHandler(this.PlanTemplate_Load);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.TextBox textBoxHolidayGroupNo;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Button buttonUpdate;
        private System.Windows.Forms.TextBox textBoxLocalControllerID;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button buttonSetTemplate;
        private System.Windows.Forms.Button buttonGetTemplate;
        private System.Windows.Forms.ListView listViewTemplate;
        private System.Windows.Forms.ColumnHeader columnHeader1;
        private System.Windows.Forms.ColumnHeader columnHeader2;
        private System.Windows.Forms.TextBox textBoxTemplateName;
        private System.Windows.Forms.Label label10;
        private System.Windows.Forms.CheckBox checkBoxEnableHG;
        private System.Windows.Forms.TextBox textBoxTemplateNumber;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.ComboBox comboBoxDeviceTypeHG;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.TextBox textBoxWeekPlanNo;
        private System.Windows.Forms.Label label3;
    }
}