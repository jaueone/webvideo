namespace AcsDemo.ProductAcs
{
    partial class VideoCall
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
            this.buttonCall = new System.Windows.Forms.Button();
            this.buttonAnswer = new System.Windows.Forms.Button();
            this.buttonCancel = new System.Windows.Forms.Button();
            this.buttonHangUp = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.labelShowStatus = new System.Windows.Forms.Label();
            this.PreViewBox = new System.Windows.Forms.PictureBox();
            ((System.ComponentModel.ISupportInitialize)(this.PreViewBox)).BeginInit();
            this.SuspendLayout();
            // 
            // buttonCall
            // 
            this.buttonCall.Location = new System.Drawing.Point(33, 25);
            this.buttonCall.Name = "buttonCall";
            this.buttonCall.Size = new System.Drawing.Size(86, 23);
            this.buttonCall.TabIndex = 0;
            this.buttonCall.Text = "Call";
            this.buttonCall.UseVisualStyleBackColor = true;
            this.buttonCall.Click += new System.EventHandler(this.buttonCall_Click);
            // 
            // buttonAnswer
            // 
            this.buttonAnswer.Location = new System.Drawing.Point(137, 25);
            this.buttonAnswer.Name = "buttonAnswer";
            this.buttonAnswer.Size = new System.Drawing.Size(86, 23);
            this.buttonAnswer.TabIndex = 1;
            this.buttonAnswer.Text = "Answer";
            this.buttonAnswer.UseVisualStyleBackColor = true;
            this.buttonAnswer.Click += new System.EventHandler(this.buttonAnswer_Click);
            // 
            // buttonCancel
            // 
            this.buttonCancel.Location = new System.Drawing.Point(241, 25);
            this.buttonCancel.Name = "buttonCancel";
            this.buttonCancel.Size = new System.Drawing.Size(86, 23);
            this.buttonCancel.TabIndex = 2;
            this.buttonCancel.Text = "Cancel";
            this.buttonCancel.UseVisualStyleBackColor = true;
            this.buttonCancel.Click += new System.EventHandler(this.buttonCancel_Click);
            // 
            // buttonHangUp
            // 
            this.buttonHangUp.Location = new System.Drawing.Point(346, 25);
            this.buttonHangUp.Name = "buttonHangUp";
            this.buttonHangUp.Size = new System.Drawing.Size(86, 23);
            this.buttonHangUp.TabIndex = 3;
            this.buttonHangUp.Text = "HangUp";
            this.buttonHangUp.UseVisualStyleBackColor = true;
            this.buttonHangUp.Click += new System.EventHandler(this.buttonHangUp_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(24, 424);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(95, 12);
            this.label1.TabIndex = 4;
            this.label1.Text = "Current Status:";
            // 
            // labelShowStatus
            // 
            this.labelShowStatus.AutoSize = true;
            this.labelShowStatus.Location = new System.Drawing.Point(141, 424);
            this.labelShowStatus.Name = "labelShowStatus";
            this.labelShowStatus.Size = new System.Drawing.Size(0, 12);
            this.labelShowStatus.TabIndex = 5;
            // 
            // PreViewBox
            // 
            this.PreViewBox.Location = new System.Drawing.Point(45, 54);
            this.PreViewBox.Name = "PreViewBox";
            this.PreViewBox.Size = new System.Drawing.Size(350, 350);
            this.PreViewBox.TabIndex = 6;
            this.PreViewBox.TabStop = false;
            // 
            // VideoCall
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(471, 463);
            this.Controls.Add(this.PreViewBox);
            this.Controls.Add(this.labelShowStatus);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.buttonHangUp);
            this.Controls.Add(this.buttonCancel);
            this.Controls.Add(this.buttonAnswer);
            this.Controls.Add(this.buttonCall);
            this.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
            this.Name = "VideoCall";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Video Call";
            this.Load += new System.EventHandler(this.VideoCall_Load);
            ((System.ComponentModel.ISupportInitialize)(this.PreViewBox)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button buttonCall;
        private System.Windows.Forms.Button buttonAnswer;
        private System.Windows.Forms.Button buttonCancel;
        private System.Windows.Forms.Button buttonHangUp;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label labelShowStatus;
        private System.Windows.Forms.PictureBox PreViewBox;
    }
}