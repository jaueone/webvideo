namespace AcsDemo.Public
{
    partial class AlarmInfoShow
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
            this.label_show = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // label_show
            // 
            this.label_show.Location = new System.Drawing.Point(29, 24);
            this.label_show.Name = "label_show";
            this.label_show.Size = new System.Drawing.Size(478, 178);
            this.label_show.TabIndex = 0;
            // 
            // AlarmInfoShow
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(535, 236);
            this.Controls.Add(this.label_show);
            this.Name = "AlarmInfoShow";
            this.Text = "AlarmInfo";
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Label label_show;
    }
}