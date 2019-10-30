namespace AcsDemo.ProductAcs
{
    partial class DevicePlanStatus
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
            this.buttonSetCurrentPlan = new System.Windows.Forms.Button();
            this.buttonGetCurrentPlan = new System.Windows.Forms.Button();
            this.textBoxPlanTemplateNumber = new System.Windows.Forms.TextBox();
            this.label19 = new System.Windows.Forms.Label();
            this.comboBoxDeviceType = new System.Windows.Forms.ComboBox();
            this.label17 = new System.Windows.Forms.Label();
            this.textBoxNumber = new System.Windows.Forms.TextBox();
            this.label18 = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // buttonSetCurrentPlan
            // 
            this.buttonSetCurrentPlan.Location = new System.Drawing.Point(155, 138);
            this.buttonSetCurrentPlan.Name = "buttonSetCurrentPlan";
            this.buttonSetCurrentPlan.Size = new System.Drawing.Size(75, 23);
            this.buttonSetCurrentPlan.TabIndex = 36;
            this.buttonSetCurrentPlan.Text = "Set";
            this.buttonSetCurrentPlan.UseVisualStyleBackColor = true;
            this.buttonSetCurrentPlan.Click += new System.EventHandler(this.buttonSetCurrentPlan_Click);
            // 
            // buttonGetCurrentPlan
            // 
            this.buttonGetCurrentPlan.Location = new System.Drawing.Point(53, 138);
            this.buttonGetCurrentPlan.Name = "buttonGetCurrentPlan";
            this.buttonGetCurrentPlan.Size = new System.Drawing.Size(75, 23);
            this.buttonGetCurrentPlan.TabIndex = 35;
            this.buttonGetCurrentPlan.Text = "Get";
            this.buttonGetCurrentPlan.UseVisualStyleBackColor = true;
            this.buttonGetCurrentPlan.Click += new System.EventHandler(this.buttonGetCurrentPlan_Click);
            // 
            // textBoxPlanTemplateNumber
            // 
            this.textBoxPlanTemplateNumber.Location = new System.Drawing.Point(155, 95);
            this.textBoxPlanTemplateNumber.Name = "textBoxPlanTemplateNumber";
            this.textBoxPlanTemplateNumber.Size = new System.Drawing.Size(100, 21);
            this.textBoxPlanTemplateNumber.TabIndex = 34;
            this.textBoxPlanTemplateNumber.Text = "1";
            // 
            // label19
            // 
            this.label19.AutoSize = true;
            this.label19.Location = new System.Drawing.Point(12, 98);
            this.label19.Name = "label19";
            this.label19.Size = new System.Drawing.Size(125, 12);
            this.label19.TabIndex = 33;
            this.label19.Text = "Plan Template Number";
            // 
            // comboBoxDeviceType
            // 
            this.comboBoxDeviceType.FormattingEnabled = true;
            this.comboBoxDeviceType.Items.AddRange(new object[] {
            "Door",
            "Card reader"});
            this.comboBoxDeviceType.Location = new System.Drawing.Point(155, 58);
            this.comboBoxDeviceType.Name = "comboBoxDeviceType";
            this.comboBoxDeviceType.Size = new System.Drawing.Size(100, 20);
            this.comboBoxDeviceType.TabIndex = 32;
            this.comboBoxDeviceType.Text = "Door";
            // 
            // label17
            // 
            this.label17.AutoSize = true;
            this.label17.Location = new System.Drawing.Point(13, 61);
            this.label17.Name = "label17";
            this.label17.Size = new System.Drawing.Size(71, 12);
            this.label17.TabIndex = 31;
            this.label17.Text = "Device Type";
            // 
            // textBoxNumber
            // 
            this.textBoxNumber.Enabled = false;
            this.textBoxNumber.Location = new System.Drawing.Point(154, 19);
            this.textBoxNumber.Name = "textBoxNumber";
            this.textBoxNumber.Size = new System.Drawing.Size(100, 21);
            this.textBoxNumber.TabIndex = 38;
            this.textBoxNumber.Text = "1";
            // 
            // label18
            // 
            this.label18.AutoSize = true;
            this.label18.Location = new System.Drawing.Point(14, 22);
            this.label18.Name = "label18";
            this.label18.Size = new System.Drawing.Size(35, 12);
            this.label18.TabIndex = 37;
            this.label18.Text = "Index";
            // 
            // DevicePlanStatus
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(284, 176);
            this.Controls.Add(this.textBoxNumber);
            this.Controls.Add(this.label18);
            this.Controls.Add(this.buttonSetCurrentPlan);
            this.Controls.Add(this.buttonGetCurrentPlan);
            this.Controls.Add(this.textBoxPlanTemplateNumber);
            this.Controls.Add(this.label19);
            this.Controls.Add(this.comboBoxDeviceType);
            this.Controls.Add(this.label17);
            this.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.Name = "DevicePlanStatus";
            this.Text = "DevicePlanStatus";
            this.Load += new System.EventHandler(this.DevicePlanStatus_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button buttonSetCurrentPlan;
        private System.Windows.Forms.Button buttonGetCurrentPlan;
        private System.Windows.Forms.TextBox textBoxPlanTemplateNumber;
        private System.Windows.Forms.Label label19;
        private System.Windows.Forms.ComboBox comboBoxDeviceType;
        private System.Windows.Forms.Label label17;
        private System.Windows.Forms.TextBox textBoxNumber;
        private System.Windows.Forms.Label label18;
    }
}