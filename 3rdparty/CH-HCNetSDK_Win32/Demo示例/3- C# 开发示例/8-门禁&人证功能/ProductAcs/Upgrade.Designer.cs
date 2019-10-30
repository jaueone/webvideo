namespace AcsDemo.ProductAcs
{
    partial class Upgrade
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
            this.m_lblRestart = new System.Windows.Forms.Label();
            this.progressBarUpgrade = new System.Windows.Forms.ProgressBar();
            this.m_btnUpdate = new System.Windows.Forms.Button();
            this.m_btnBrowse = new System.Windows.Forms.Button();
            this.m_txtBrowsePath = new System.Windows.Forms.TextBox();
            this.labelFirmWare = new System.Windows.Forms.Label();
            this.groupBox1.SuspendLayout();
            this.SuspendLayout();
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.m_lblRestart);
            this.groupBox1.Controls.Add(this.progressBarUpgrade);
            this.groupBox1.Controls.Add(this.m_btnUpdate);
            this.groupBox1.Controls.Add(this.m_btnBrowse);
            this.groupBox1.Controls.Add(this.m_txtBrowsePath);
            this.groupBox1.Controls.Add(this.labelFirmWare);
            this.groupBox1.Location = new System.Drawing.Point(12, 12);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(543, 156);
            this.groupBox1.TabIndex = 2;
            this.groupBox1.TabStop = false;
            // 
            // m_lblRestart
            // 
            this.m_lblRestart.AutoSize = true;
            this.m_lblRestart.Location = new System.Drawing.Point(16, 54);
            this.m_lblRestart.Name = "m_lblRestart";
            this.m_lblRestart.Size = new System.Drawing.Size(167, 12);
            this.m_lblRestart.TabIndex = 15;
            this.m_lblRestart.Text = "Please restart your device ";
            // 
            // progressBarUpgrade
            // 
            this.progressBarUpgrade.BackColor = System.Drawing.SystemColors.ButtonHighlight;
            this.progressBarUpgrade.Location = new System.Drawing.Point(16, 72);
            this.progressBarUpgrade.Name = "progressBarUpgrade";
            this.progressBarUpgrade.Size = new System.Drawing.Size(507, 23);
            this.progressBarUpgrade.TabIndex = 14;
            // 
            // m_btnUpdate
            // 
            this.m_btnUpdate.Location = new System.Drawing.Point(16, 118);
            this.m_btnUpdate.Name = "m_btnUpdate";
            this.m_btnUpdate.Size = new System.Drawing.Size(75, 23);
            this.m_btnUpdate.TabIndex = 7;
            this.m_btnUpdate.Text = "Update";
            this.m_btnUpdate.UseVisualStyleBackColor = true;
            this.m_btnUpdate.Click += new System.EventHandler(this.m_btnUpdate_Click);
            // 
            // m_btnBrowse
            // 
            this.m_btnBrowse.Location = new System.Drawing.Point(448, 18);
            this.m_btnBrowse.Name = "m_btnBrowse";
            this.m_btnBrowse.Size = new System.Drawing.Size(75, 23);
            this.m_btnBrowse.TabIndex = 5;
            this.m_btnBrowse.Text = "Browse";
            this.m_btnBrowse.UseVisualStyleBackColor = true;
            this.m_btnBrowse.Click += new System.EventHandler(this.m_btnBrowse_Click);
            // 
            // m_txtBrowsePath
            // 
            this.m_txtBrowsePath.Location = new System.Drawing.Point(73, 20);
            this.m_txtBrowsePath.Name = "m_txtBrowsePath";
            this.m_txtBrowsePath.Size = new System.Drawing.Size(359, 21);
            this.m_txtBrowsePath.TabIndex = 4;
            this.m_txtBrowsePath.Text = "C:\\digicap";
            // 
            // labelFirmWare
            // 
            this.labelFirmWare.AutoSize = true;
            this.labelFirmWare.Location = new System.Drawing.Point(14, 23);
            this.labelFirmWare.Name = "labelFirmWare";
            this.labelFirmWare.Size = new System.Drawing.Size(53, 12);
            this.labelFirmWare.TabIndex = 3;
            this.labelFirmWare.Text = "FirmWare";
            // 
            // Upgrade
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(568, 183);
            this.Controls.Add(this.groupBox1);
            this.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
            this.Name = "Upgrade";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Upgrade";
            this.Load += new System.EventHandler(this.Upgrade_Load);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Label m_lblRestart;
        private System.Windows.Forms.ProgressBar progressBarUpgrade;
        private System.Windows.Forms.Button m_btnUpdate;
        private System.Windows.Forms.Button m_btnBrowse;
        private System.Windows.Forms.TextBox m_txtBrowsePath;
        private System.Windows.Forms.Label labelFirmWare;
    }
}