namespace AcsDemo.ProductAcs
{
    partial class UserAdd
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
            this.textBoxMemberNo = new System.Windows.Forms.TextBox();
            this.labelMemberNo = new System.Windows.Forms.Label();
            this.textBoxMemberName = new System.Windows.Forms.TextBox();
            this.labelMemberName = new System.Windows.Forms.Label();
            this.tabControlUserAdd = new System.Windows.Forms.TabControl();
            this.tabPageCardInfo = new System.Windows.Forms.TabPage();
            this.buttonCardInfoDelete = new System.Windows.Forms.Button();
            this.buttonCardInfoModify = new System.Windows.Forms.Button();
            this.buttonCardInfoAdd = new System.Windows.Forms.Button();
            this.listViewUserAddCardInfo = new System.Windows.Forms.ListView();
            this.columnHeader1 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader2 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader3 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader4 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.tabPageModuleCfg = new System.Windows.Forms.TabPage();
            this.tabPageInputFingerPrint = new System.Windows.Forms.TabPage();
            this.tabControlUserAdd.SuspendLayout();
            this.tabPageCardInfo.SuspendLayout();
            this.SuspendLayout();
            // 
            // textBoxMemberNo
            // 
            this.textBoxMemberNo.Location = new System.Drawing.Point(138, 38);
            this.textBoxMemberNo.Name = "textBoxMemberNo";
            this.textBoxMemberNo.Size = new System.Drawing.Size(104, 21);
            this.textBoxMemberNo.TabIndex = 37;
            // 
            // labelMemberNo
            // 
            this.labelMemberNo.AutoSize = true;
            this.labelMemberNo.ImeMode = System.Windows.Forms.ImeMode.NoControl;
            this.labelMemberNo.Location = new System.Drawing.Point(56, 41);
            this.labelMemberNo.Name = "labelMemberNo";
            this.labelMemberNo.Size = new System.Drawing.Size(59, 12);
            this.labelMemberNo.TabIndex = 36;
            this.labelMemberNo.Text = "Member No";
            // 
            // textBoxMemberName
            // 
            this.textBoxMemberName.Location = new System.Drawing.Point(366, 38);
            this.textBoxMemberName.Name = "textBoxMemberName";
            this.textBoxMemberName.Size = new System.Drawing.Size(104, 21);
            this.textBoxMemberName.TabIndex = 39;
            // 
            // labelMemberName
            // 
            this.labelMemberName.AutoSize = true;
            this.labelMemberName.ImeMode = System.Windows.Forms.ImeMode.NoControl;
            this.labelMemberName.Location = new System.Drawing.Point(279, 41);
            this.labelMemberName.Name = "labelMemberName";
            this.labelMemberName.Size = new System.Drawing.Size(71, 12);
            this.labelMemberName.TabIndex = 38;
            this.labelMemberName.Text = "Member Name";
            // 
            // tabControlUserAdd
            // 
            this.tabControlUserAdd.Controls.Add(this.tabPageCardInfo);
            this.tabControlUserAdd.Controls.Add(this.tabPageModuleCfg);
            this.tabControlUserAdd.Controls.Add(this.tabPageInputFingerPrint);
            this.tabControlUserAdd.Location = new System.Drawing.Point(1, 76);
            this.tabControlUserAdd.Name = "tabControlUserAdd";
            this.tabControlUserAdd.SelectedIndex = 0;
            this.tabControlUserAdd.Size = new System.Drawing.Size(534, 326);
            this.tabControlUserAdd.TabIndex = 40;
            // 
            // tabPageCardInfo
            // 
            this.tabPageCardInfo.Controls.Add(this.buttonCardInfoDelete);
            this.tabPageCardInfo.Controls.Add(this.buttonCardInfoModify);
            this.tabPageCardInfo.Controls.Add(this.buttonCardInfoAdd);
            this.tabPageCardInfo.Controls.Add(this.listViewUserAddCardInfo);
            this.tabPageCardInfo.Location = new System.Drawing.Point(4, 22);
            this.tabPageCardInfo.Name = "tabPageCardInfo";
            this.tabPageCardInfo.Padding = new System.Windows.Forms.Padding(3);
            this.tabPageCardInfo.Size = new System.Drawing.Size(526, 300);
            this.tabPageCardInfo.TabIndex = 0;
            this.tabPageCardInfo.Text = "Card Info";
            this.tabPageCardInfo.UseVisualStyleBackColor = true;
            // 
            // buttonCardInfoDelete
            // 
            this.buttonCardInfoDelete.ImeMode = System.Windows.Forms.ImeMode.NoControl;
            this.buttonCardInfoDelete.Location = new System.Drawing.Point(361, 239);
            this.buttonCardInfoDelete.Name = "buttonCardInfoDelete";
            this.buttonCardInfoDelete.Size = new System.Drawing.Size(107, 22);
            this.buttonCardInfoDelete.TabIndex = 27;
            this.buttonCardInfoDelete.Text = "Delete";
            this.buttonCardInfoDelete.UseVisualStyleBackColor = true;
            // 
            // buttonCardInfoModify
            // 
            this.buttonCardInfoModify.ImeMode = System.Windows.Forms.ImeMode.NoControl;
            this.buttonCardInfoModify.Location = new System.Drawing.Point(211, 239);
            this.buttonCardInfoModify.Name = "buttonCardInfoModify";
            this.buttonCardInfoModify.Size = new System.Drawing.Size(107, 22);
            this.buttonCardInfoModify.TabIndex = 26;
            this.buttonCardInfoModify.Text = "Modify";
            this.buttonCardInfoModify.UseVisualStyleBackColor = true;
            // 
            // buttonCardInfoAdd
            // 
            this.buttonCardInfoAdd.ImeMode = System.Windows.Forms.ImeMode.NoControl;
            this.buttonCardInfoAdd.Location = new System.Drawing.Point(53, 239);
            this.buttonCardInfoAdd.Name = "buttonCardInfoAdd";
            this.buttonCardInfoAdd.Size = new System.Drawing.Size(107, 22);
            this.buttonCardInfoAdd.TabIndex = 25;
            this.buttonCardInfoAdd.Text = "Add";
            this.buttonCardInfoAdd.UseVisualStyleBackColor = true;
            this.buttonCardInfoAdd.Click += new System.EventHandler(this.buttonCardInfoAdd_Click);
            // 
            // listViewUserAddCardInfo
            // 
            this.listViewUserAddCardInfo.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader1,
            this.columnHeader2,
            this.columnHeader3,
            this.columnHeader4});
            this.listViewUserAddCardInfo.FullRowSelect = true;
            this.listViewUserAddCardInfo.GridLines = true;
            this.listViewUserAddCardInfo.Location = new System.Drawing.Point(0, 0);
            this.listViewUserAddCardInfo.Margin = new System.Windows.Forms.Padding(0);
            this.listViewUserAddCardInfo.Name = "listViewUserAddCardInfo";
            this.listViewUserAddCardInfo.Size = new System.Drawing.Size(526, 204);
            this.listViewUserAddCardInfo.TabIndex = 3;
            this.listViewUserAddCardInfo.UseCompatibleStateImageBehavior = false;
            this.listViewUserAddCardInfo.View = System.Windows.Forms.View.Details;
            // 
            // columnHeader1
            // 
            this.columnHeader1.Text = "Index";
            this.columnHeader1.Width = 100;
            // 
            // columnHeader2
            // 
            this.columnHeader2.Text = "Card No";
            this.columnHeader2.Width = 130;
            // 
            // columnHeader3
            // 
            this.columnHeader3.Text = "Card Type";
            this.columnHeader3.Width = 130;
            // 
            // columnHeader4
            // 
            this.columnHeader4.Text = "Card Validity";
            this.columnHeader4.Width = 170;
            // 
            // tabPageModuleCfg
            // 
            this.tabPageModuleCfg.Location = new System.Drawing.Point(4, 22);
            this.tabPageModuleCfg.Name = "tabPageModuleCfg";
            this.tabPageModuleCfg.Padding = new System.Windows.Forms.Padding(3);
            this.tabPageModuleCfg.Size = new System.Drawing.Size(526, 300);
            this.tabPageModuleCfg.TabIndex = 2;
            this.tabPageModuleCfg.Text = "Module Cfg";
            this.tabPageModuleCfg.UseVisualStyleBackColor = true;
            // 
            // tabPageInputFingerPrint
            // 
            this.tabPageInputFingerPrint.Location = new System.Drawing.Point(4, 22);
            this.tabPageInputFingerPrint.Name = "tabPageInputFingerPrint";
            this.tabPageInputFingerPrint.Padding = new System.Windows.Forms.Padding(3);
            this.tabPageInputFingerPrint.Size = new System.Drawing.Size(526, 300);
            this.tabPageInputFingerPrint.TabIndex = 3;
            this.tabPageInputFingerPrint.Text = "Input Finger Print";
            this.tabPageInputFingerPrint.UseVisualStyleBackColor = true;
            // 
            // UserAdd
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(534, 403);
            this.Controls.Add(this.tabControlUserAdd);
            this.Controls.Add(this.textBoxMemberName);
            this.Controls.Add(this.labelMemberName);
            this.Controls.Add(this.textBoxMemberNo);
            this.Controls.Add(this.labelMemberNo);
            this.Name = "UserAdd";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "User Add";
            this.tabControlUserAdd.ResumeLayout(false);
            this.tabPageCardInfo.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox textBoxMemberNo;
        private System.Windows.Forms.Label labelMemberNo;
        private System.Windows.Forms.TextBox textBoxMemberName;
        private System.Windows.Forms.Label labelMemberName;
        private System.Windows.Forms.TabControl tabControlUserAdd;
        private System.Windows.Forms.TabPage tabPageCardInfo;
        private System.Windows.Forms.TabPage tabPageModuleCfg;
        private System.Windows.Forms.TabPage tabPageInputFingerPrint;
        private System.Windows.Forms.ListView listViewUserAddCardInfo;
        private System.Windows.Forms.ColumnHeader columnHeader1;
        private System.Windows.Forms.ColumnHeader columnHeader2;
        private System.Windows.Forms.ColumnHeader columnHeader3;
        private System.Windows.Forms.Button buttonCardInfoAdd;
        private System.Windows.Forms.Button buttonCardInfoDelete;
        private System.Windows.Forms.Button buttonCardInfoModify;
        private System.Windows.Forms.ColumnHeader columnHeader4;
    }
}