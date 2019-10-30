namespace AcsDemo.ProductAcs
{
    partial class SynInfo
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
            this.progressBarSynProgress = new System.Windows.Forms.ProgressBar();
            this.labelSynProgress = new System.Windows.Forms.Label();
            this.buttonStart = new System.Windows.Forms.Button();
            this.listViewResponse = new System.Windows.Forms.ListView();
            this.columnHeader1 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader2 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.SuspendLayout();
            // 
            // progressBarSynProgress
            // 
            this.progressBarSynProgress.Location = new System.Drawing.Point(15, 50);
            this.progressBarSynProgress.Name = "progressBarSynProgress";
            this.progressBarSynProgress.Size = new System.Drawing.Size(427, 11);
            this.progressBarSynProgress.TabIndex = 0;
            // 
            // labelSynProgress
            // 
            this.labelSynProgress.AutoSize = true;
            this.labelSynProgress.Location = new System.Drawing.Point(13, 74);
            this.labelSynProgress.Name = "labelSynProgress";
            this.labelSynProgress.Size = new System.Drawing.Size(53, 12);
            this.labelSynProgress.TabIndex = 1;
            this.labelSynProgress.Text = "progress";
            // 
            // buttonStart
            // 
            this.buttonStart.Location = new System.Drawing.Point(320, 12);
            this.buttonStart.Name = "buttonStart";
            this.buttonStart.Size = new System.Drawing.Size(122, 23);
            this.buttonStart.TabIndex = 2;
            this.buttonStart.Text = "Start Synchronize";
            this.buttonStart.UseVisualStyleBackColor = true;
            this.buttonStart.Click += new System.EventHandler(this.buttonStart_Click);
            // 
            // listViewResponse
            // 
            this.listViewResponse.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader1,
            this.columnHeader2});
            this.listViewResponse.Location = new System.Drawing.Point(15, 99);
            this.listViewResponse.Name = "listViewResponse";
            this.listViewResponse.Size = new System.Drawing.Size(427, 151);
            this.listViewResponse.TabIndex = 3;
            this.listViewResponse.UseCompatibleStateImageBehavior = false;
            this.listViewResponse.View = System.Windows.Forms.View.Details;
            // 
            // columnHeader1
            // 
            this.columnHeader1.Text = "seq";
            // 
            // columnHeader2
            // 
            this.columnHeader2.Text = "response";
            this.columnHeader2.Width = 380;
            // 
            // SynInfo
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(454, 262);
            this.Controls.Add(this.listViewResponse);
            this.Controls.Add(this.buttonStart);
            this.Controls.Add(this.labelSynProgress);
            this.Controls.Add(this.progressBarSynProgress);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
            this.Name = "SynInfo";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "SynInfo";
            this.Load += new System.EventHandler(this.SynInfo_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.ProgressBar progressBarSynProgress;
        private System.Windows.Forms.Label labelSynProgress;
        private System.Windows.Forms.Button buttonStart;
        private System.Windows.Forms.ListView listViewResponse;
        private System.Windows.Forms.ColumnHeader columnHeader1;
        private System.Windows.Forms.ColumnHeader columnHeader2;
    }
}