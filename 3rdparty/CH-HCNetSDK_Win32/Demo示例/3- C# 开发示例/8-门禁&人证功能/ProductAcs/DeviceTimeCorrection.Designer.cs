namespace AcsDemo.ProductAcs
{
    partial class DeviceTimeCorrection
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
            this.btnTimeCorr = new System.Windows.Forms.Button();
            this.dateTimePickerCorrection = new System.Windows.Forms.DateTimePicker();
            this.textBoxDevTime = new System.Windows.Forms.TextBox();
            this.btnGetDeviceTime = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // btnTimeCorr
            // 
            this.btnTimeCorr.Location = new System.Drawing.Point(175, 64);
            this.btnTimeCorr.Name = "btnTimeCorr";
            this.btnTimeCorr.Size = new System.Drawing.Size(198, 22);
            this.btnTimeCorr.TabIndex = 0;
            this.btnTimeCorr.Text = "Time Correction";
            this.btnTimeCorr.UseVisualStyleBackColor = true;
            this.btnTimeCorr.Click += new System.EventHandler(this.btnTimeCorr_Click);
            // 
            // dateTimePickerCorrection
            // 
            this.dateTimePickerCorrection.CustomFormat = "yyyy-MM-dd HH:mm:ss";
            this.dateTimePickerCorrection.Format = System.Windows.Forms.DateTimePickerFormat.Custom;
            this.dateTimePickerCorrection.Location = new System.Drawing.Point(12, 65);
            this.dateTimePickerCorrection.Name = "dateTimePickerCorrection";
            this.dateTimePickerCorrection.Size = new System.Drawing.Size(157, 21);
            this.dateTimePickerCorrection.TabIndex = 1;
            // 
            // textBoxDevTime
            // 
            this.textBoxDevTime.Location = new System.Drawing.Point(13, 24);
            this.textBoxDevTime.Name = "textBoxDevTime";
            this.textBoxDevTime.Size = new System.Drawing.Size(156, 21);
            this.textBoxDevTime.TabIndex = 2;
            // 
            // btnGetDeviceTime
            // 
            this.btnGetDeviceTime.Location = new System.Drawing.Point(176, 21);
            this.btnGetDeviceTime.Name = "btnGetDeviceTime";
            this.btnGetDeviceTime.Size = new System.Drawing.Size(198, 22);
            this.btnGetDeviceTime.TabIndex = 3;
            this.btnGetDeviceTime.Text = "Get Device Time";
            this.btnGetDeviceTime.UseVisualStyleBackColor = true;
            this.btnGetDeviceTime.Click += new System.EventHandler(this.button1_Click);
            // 
            // DeviceTimeCorrection
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(379, 93);
            this.Controls.Add(this.btnGetDeviceTime);
            this.Controls.Add(this.textBoxDevTime);
            this.Controls.Add(this.dateTimePickerCorrection);
            this.Controls.Add(this.btnTimeCorr);
            this.Name = "DeviceTimeCorrection";
            this.Text = "DeviceTimeCorrection";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button btnTimeCorr;
        private System.Windows.Forms.DateTimePicker dateTimePickerCorrection;
        private System.Windows.Forms.TextBox textBoxDevTime;
        private System.Windows.Forms.Button btnGetDeviceTime;
    }
}