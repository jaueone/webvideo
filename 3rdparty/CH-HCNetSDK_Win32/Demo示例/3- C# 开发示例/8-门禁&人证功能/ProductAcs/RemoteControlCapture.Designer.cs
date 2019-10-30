namespace AcsDemo.ProductAcs
{
    partial class RemoteControlCapture
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
            this.groupBoxCapture = new System.Windows.Forms.GroupBox();
            this.btnCapture = new System.Windows.Forms.Button();
            this.comboBoxPicQua = new System.Windows.Forms.ComboBox();
            this.label2 = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.textBoxPicType = new System.Windows.Forms.TextBox();
            this.groupBoxCapture.SuspendLayout();
            this.SuspendLayout();
            // 
            // groupBoxCapture
            // 
            this.groupBoxCapture.Controls.Add(this.textBoxPicType);
            this.groupBoxCapture.Controls.Add(this.btnCapture);
            this.groupBoxCapture.Controls.Add(this.comboBoxPicQua);
            this.groupBoxCapture.Controls.Add(this.label2);
            this.groupBoxCapture.Controls.Add(this.label1);
            this.groupBoxCapture.Location = new System.Drawing.Point(12, 29);
            this.groupBoxCapture.Name = "groupBoxCapture";
            this.groupBoxCapture.Size = new System.Drawing.Size(260, 151);
            this.groupBoxCapture.TabIndex = 0;
            this.groupBoxCapture.TabStop = false;
            this.groupBoxCapture.Text = "Capture";
            // 
            // btnCapture
            // 
            this.btnCapture.Location = new System.Drawing.Point(176, 114);
            this.btnCapture.Name = "btnCapture";
            this.btnCapture.Size = new System.Drawing.Size(75, 23);
            this.btnCapture.TabIndex = 4;
            this.btnCapture.Text = "Capture";
            this.btnCapture.UseVisualStyleBackColor = true;
            this.btnCapture.Click += new System.EventHandler(this.btnCapture_Click);
            // 
            // comboBoxPicQua
            // 
            this.comboBoxPicQua.FormattingEnabled = true;
            this.comboBoxPicQua.Items.AddRange(new object[] {
            "best",
            "better",
            "average"});
            this.comboBoxPicQua.Location = new System.Drawing.Point(92, 73);
            this.comboBoxPicQua.Name = "comboBoxPicQua";
            this.comboBoxPicQua.Size = new System.Drawing.Size(121, 20);
            this.comboBoxPicQua.TabIndex = 3;
            this.comboBoxPicQua.Text = "average";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(15, 76);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(71, 12);
            this.label2.TabIndex = 2;
            this.label2.Text = "Pic Quality";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(17, 30);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(53, 12);
            this.label1.TabIndex = 0;
            this.label1.Text = "Pic Type";
            // 
            // textBoxPicType
            // 
            this.textBoxPicType.Location = new System.Drawing.Point(92, 27);
            this.textBoxPicType.Name = "textBoxPicType";
            this.textBoxPicType.Size = new System.Drawing.Size(100, 21);
            this.textBoxPicType.TabIndex = 5;
            this.textBoxPicType.Text = "0";
            // 
            // RemoteControlCapture
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(284, 262);
            this.Controls.Add(this.groupBoxCapture);
            this.Name = "RemoteControlCapture";
            this.Text = "RemoteControlCapture";
            this.groupBoxCapture.ResumeLayout(false);
            this.groupBoxCapture.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.GroupBox groupBoxCapture;
        private System.Windows.Forms.Button btnCapture;
        private System.Windows.Forms.ComboBox comboBoxPicQua;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox textBoxPicType;
    }
}