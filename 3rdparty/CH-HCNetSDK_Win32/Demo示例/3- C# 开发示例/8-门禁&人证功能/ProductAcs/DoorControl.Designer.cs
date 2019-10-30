namespace AcsDemo.ProductAcs
{
    partial class DoorControl
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
            this.buttonDoorOpen = new System.Windows.Forms.Button();
            this.buttonDoorClose = new System.Windows.Forms.Button();
            this.buttonDoorAlwaysOpen = new System.Windows.Forms.Button();
            this.buttonDoorAlwaysClose = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // buttonDoorOpen
            // 
            this.buttonDoorOpen.ImeMode = System.Windows.Forms.ImeMode.NoControl;
            this.buttonDoorOpen.Location = new System.Drawing.Point(12, 25);
            this.buttonDoorOpen.Name = "buttonDoorOpen";
            this.buttonDoorOpen.Size = new System.Drawing.Size(106, 22);
            this.buttonDoorOpen.TabIndex = 19;
            this.buttonDoorOpen.Text = "Open";
            this.buttonDoorOpen.UseVisualStyleBackColor = true;
            // 
            // buttonDoorClose
            // 
            this.buttonDoorClose.ImeMode = System.Windows.Forms.ImeMode.NoControl;
            this.buttonDoorClose.Location = new System.Drawing.Point(133, 25);
            this.buttonDoorClose.Name = "buttonDoorClose";
            this.buttonDoorClose.Size = new System.Drawing.Size(106, 22);
            this.buttonDoorClose.TabIndex = 20;
            this.buttonDoorClose.Text = "Close";
            this.buttonDoorClose.UseVisualStyleBackColor = true;
            // 
            // buttonDoorAlwaysOpen
            // 
            this.buttonDoorAlwaysOpen.ImeMode = System.Windows.Forms.ImeMode.NoControl;
            this.buttonDoorAlwaysOpen.Location = new System.Drawing.Point(12, 65);
            this.buttonDoorAlwaysOpen.Name = "buttonDoorAlwaysOpen";
            this.buttonDoorAlwaysOpen.Size = new System.Drawing.Size(106, 22);
            this.buttonDoorAlwaysOpen.TabIndex = 21;
            this.buttonDoorAlwaysOpen.Text = "Always Open";
            this.buttonDoorAlwaysOpen.UseVisualStyleBackColor = true;
            // 
            // buttonDoorAlwaysClose
            // 
            this.buttonDoorAlwaysClose.ImeMode = System.Windows.Forms.ImeMode.NoControl;
            this.buttonDoorAlwaysClose.Location = new System.Drawing.Point(133, 65);
            this.buttonDoorAlwaysClose.Name = "buttonDoorAlwaysClose";
            this.buttonDoorAlwaysClose.Size = new System.Drawing.Size(106, 22);
            this.buttonDoorAlwaysClose.TabIndex = 22;
            this.buttonDoorAlwaysClose.Text = "Always Close";
            this.buttonDoorAlwaysClose.UseVisualStyleBackColor = true;
            // 
            // DoorControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(256, 118);
            this.Controls.Add(this.buttonDoorAlwaysClose);
            this.Controls.Add(this.buttonDoorAlwaysOpen);
            this.Controls.Add(this.buttonDoorClose);
            this.Controls.Add(this.buttonDoorOpen);
            this.Name = "DoorControl";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Remote Control Door";
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button buttonDoorOpen;
        private System.Windows.Forms.Button buttonDoorClose;
        private System.Windows.Forms.Button buttonDoorAlwaysOpen;
        private System.Windows.Forms.Button buttonDoorAlwaysClose;
    }
}