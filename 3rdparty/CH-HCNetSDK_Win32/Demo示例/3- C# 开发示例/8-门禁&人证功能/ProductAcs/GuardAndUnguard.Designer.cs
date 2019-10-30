namespace AcsDemo.ProductAcs
{
    partial class GuardAndUnguard
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
            this.buttonGuardAndUnguard = new System.Windows.Forms.Button();
            this.buttonExit = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // buttonGuardAndUnguard
            // 
            this.buttonGuardAndUnguard.ImeMode = System.Windows.Forms.ImeMode.NoControl;
            this.buttonGuardAndUnguard.Location = new System.Drawing.Point(33, 28);
            this.buttonGuardAndUnguard.Name = "buttonGuardAndUnguard";
            this.buttonGuardAndUnguard.Size = new System.Drawing.Size(198, 22);
            this.buttonGuardAndUnguard.TabIndex = 19;
            this.buttonGuardAndUnguard.Text = "Guard/Unguard";
            this.buttonGuardAndUnguard.UseVisualStyleBackColor = true;
            // 
            // buttonExit
            // 
            this.buttonExit.Cursor = System.Windows.Forms.Cursors.Default;
            this.buttonExit.Location = new System.Drawing.Point(33, 82);
            this.buttonExit.Name = "buttonExit";
            this.buttonExit.Size = new System.Drawing.Size(198, 22);
            this.buttonExit.TabIndex = 27;
            this.buttonExit.Text = "Exit";
            this.buttonExit.UseVisualStyleBackColor = true;
            this.buttonExit.Click += new System.EventHandler(this.buttonExit_Click);
            // 
            // GuardAndUnguard
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(266, 130);
            this.Controls.Add(this.buttonExit);
            this.Controls.Add(this.buttonGuardAndUnguard);
            this.Name = "GuardAndUnguard";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Guard/Unguard";
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button buttonGuardAndUnguard;
        private System.Windows.Forms.Button buttonExit;
    }
}