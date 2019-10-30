namespace AcsDemo.ProductAcs
{
    partial class CardManagement
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
            this.buttonDeleteCard = new System.Windows.Forms.Button();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.listViewPlaTemplateNumber = new System.Windows.Forms.ListView();
            this.columnHeader18 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader19 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader20 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.groupBox4 = new System.Windows.Forms.GroupBox();
            this.textBoxControllerNo = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.buttonEdit = new System.Windows.Forms.Button();
            this.buttonBatchAdd = new System.Windows.Forms.Button();
            this.checkBoxCheckNumber = new System.Windows.Forms.CheckBox();
            this.buttonSendAll = new System.Windows.Forms.Button();
            this.buttonGetAll = new System.Windows.Forms.Button();
            this.listViewMessage = new System.Windows.Forms.ListView();
            this.columnHeader21 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader22 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.buttonSendIndividual = new System.Windows.Forms.Button();
            this.buttonObtainIndividual = new System.Windows.Forms.Button();
            this.buttonDeleteAllCard = new System.Windows.Forms.Button();
            this.columnHeader17 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.buttonExit = new System.Windows.Forms.Button();
            this.columnHeader16 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.listViewGataManage = new System.Windows.Forms.ListView();
            this.columnHeader1 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader2 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader3 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader4 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader5 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader6 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader7 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader8 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader9 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader10 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader11 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader12 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader13 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader23 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.listViewDoorAuthority = new System.Windows.Forms.ListView();
            this.columnHeader14 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader15 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.listViewGroup = new System.Windows.Forms.ListView();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.textBoxCardNo = new System.Windows.Forms.TextBox();
            this.groupBox3.SuspendLayout();
            this.groupBox4.SuspendLayout();
            this.groupBox1.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.SuspendLayout();
            // 
            // buttonDeleteCard
            // 
            this.buttonDeleteCard.Location = new System.Drawing.Point(844, 554);
            this.buttonDeleteCard.Name = "buttonDeleteCard";
            this.buttonDeleteCard.Size = new System.Drawing.Size(95, 23);
            this.buttonDeleteCard.TabIndex = 26;
            this.buttonDeleteCard.Text = "Delete card";
            this.buttonDeleteCard.UseVisualStyleBackColor = true;
            this.buttonDeleteCard.Click += new System.EventHandler(this.buttonDeleteCard_Click);
            // 
            // groupBox3
            // 
            this.groupBox3.Controls.Add(this.listViewPlaTemplateNumber);
            this.groupBox3.Location = new System.Drawing.Point(649, 335);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(290, 211);
            this.groupBox3.TabIndex = 17;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "Plan template number";
            // 
            // listViewPlaTemplateNumber
            // 
            this.listViewPlaTemplateNumber.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader18,
            this.columnHeader19,
            this.columnHeader20});
            this.listViewPlaTemplateNumber.FullRowSelect = true;
            this.listViewPlaTemplateNumber.GridLines = true;
            this.listViewPlaTemplateNumber.Location = new System.Drawing.Point(6, 20);
            this.listViewPlaTemplateNumber.MultiSelect = false;
            this.listViewPlaTemplateNumber.Name = "listViewPlaTemplateNumber";
            this.listViewPlaTemplateNumber.Size = new System.Drawing.Size(278, 185);
            this.listViewPlaTemplateNumber.TabIndex = 0;
            this.listViewPlaTemplateNumber.TabStop = false;
            this.listViewPlaTemplateNumber.UseCompatibleStateImageBehavior = false;
            this.listViewPlaTemplateNumber.View = System.Windows.Forms.View.Details;
            // 
            // columnHeader18
            // 
            this.columnHeader18.Text = "Portal index";
            this.columnHeader18.Width = 84;
            // 
            // columnHeader19
            // 
            this.columnHeader19.Text = "Card access index";
            this.columnHeader19.Width = 119;
            // 
            // columnHeader20
            // 
            this.columnHeader20.Text = "Plan template number";
            this.columnHeader20.Width = 180;
            // 
            // groupBox4
            // 
            this.groupBox4.Controls.Add(this.textBoxControllerNo);
            this.groupBox4.Controls.Add(this.label1);
            this.groupBox4.Controls.Add(this.buttonEdit);
            this.groupBox4.Controls.Add(this.buttonBatchAdd);
            this.groupBox4.Controls.Add(this.checkBoxCheckNumber);
            this.groupBox4.Location = new System.Drawing.Point(12, 568);
            this.groupBox4.Name = "groupBox4";
            this.groupBox4.Size = new System.Drawing.Size(289, 118);
            this.groupBox4.TabIndex = 21;
            this.groupBox4.TabStop = false;
            this.groupBox4.Text = "Add configuration";
            // 
            // textBoxControllerNo
            // 
            this.textBoxControllerNo.Location = new System.Drawing.Point(162, 86);
            this.textBoxControllerNo.Name = "textBoxControllerNo";
            this.textBoxControllerNo.Size = new System.Drawing.Size(103, 21);
            this.textBoxControllerNo.TabIndex = 9;
            this.textBoxControllerNo.Text = "0";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(19, 89);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(137, 12);
            this.label1.TabIndex = 8;
            this.label1.Text = "on-site controller no.";
            // 
            // buttonEdit
            // 
            this.buttonEdit.Location = new System.Drawing.Point(143, 29);
            this.buttonEdit.Name = "buttonEdit";
            this.buttonEdit.Size = new System.Drawing.Size(75, 23);
            this.buttonEdit.TabIndex = 1;
            this.buttonEdit.Text = "Edit";
            this.buttonEdit.UseVisualStyleBackColor = true;
            this.buttonEdit.Click += new System.EventHandler(this.buttonEdit_Click);
            // 
            // buttonBatchAdd
            // 
            this.buttonBatchAdd.Location = new System.Drawing.Point(21, 29);
            this.buttonBatchAdd.Name = "buttonBatchAdd";
            this.buttonBatchAdd.Size = new System.Drawing.Size(97, 23);
            this.buttonBatchAdd.TabIndex = 0;
            this.buttonBatchAdd.Text = "Batch adding";
            this.buttonBatchAdd.UseVisualStyleBackColor = true;
            this.buttonBatchAdd.Click += new System.EventHandler(this.buttonBatchAdd_Click);
            // 
            // checkBoxCheckNumber
            // 
            this.checkBoxCheckNumber.AutoSize = true;
            this.checkBoxCheckNumber.Checked = true;
            this.checkBoxCheckNumber.CheckState = System.Windows.Forms.CheckState.Checked;
            this.checkBoxCheckNumber.Location = new System.Drawing.Point(21, 59);
            this.checkBoxCheckNumber.Name = "checkBoxCheckNumber";
            this.checkBoxCheckNumber.Size = new System.Drawing.Size(96, 16);
            this.checkBoxCheckNumber.TabIndex = 7;
            this.checkBoxCheckNumber.Text = "Check number";
            this.checkBoxCheckNumber.UseVisualStyleBackColor = true;
            this.checkBoxCheckNumber.Visible = false;
            // 
            // buttonSendAll
            // 
            this.buttonSendAll.Location = new System.Drawing.Point(467, 553);
            this.buttonSendAll.Name = "buttonSendAll";
            this.buttonSendAll.Size = new System.Drawing.Size(121, 23);
            this.buttonSendAll.TabIndex = 20;
            this.buttonSendAll.Text = "Send all";
            this.buttonSendAll.UseVisualStyleBackColor = true;
            this.buttonSendAll.Click += new System.EventHandler(this.buttonSendAll_Click);
            // 
            // buttonGetAll
            // 
            this.buttonGetAll.Location = new System.Drawing.Point(330, 553);
            this.buttonGetAll.Name = "buttonGetAll";
            this.buttonGetAll.Size = new System.Drawing.Size(121, 23);
            this.buttonGetAll.TabIndex = 19;
            this.buttonGetAll.Text = "Get all";
            this.buttonGetAll.UseVisualStyleBackColor = true;
            this.buttonGetAll.Click += new System.EventHandler(this.buttonGetAll_Click);
            // 
            // listViewMessage
            // 
            this.listViewMessage.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader21,
            this.columnHeader22});
            this.listViewMessage.FullRowSelect = true;
            this.listViewMessage.GridLines = true;
            this.listViewMessage.Location = new System.Drawing.Point(619, 583);
            this.listViewMessage.Name = "listViewMessage";
            this.listViewMessage.Size = new System.Drawing.Size(320, 97);
            this.listViewMessage.TabIndex = 24;
            this.listViewMessage.UseCompatibleStateImageBehavior = false;
            this.listViewMessage.View = System.Windows.Forms.View.Details;
            // 
            // columnHeader21
            // 
            this.columnHeader21.Text = "State";
            this.columnHeader21.Width = 62;
            // 
            // columnHeader22
            // 
            this.columnHeader22.Text = "Operation\r\n";
            this.columnHeader22.Width = 250;
            // 
            // buttonSendIndividual
            // 
            this.buttonSendIndividual.Location = new System.Drawing.Point(467, 620);
            this.buttonSendIndividual.Name = "buttonSendIndividual";
            this.buttonSendIndividual.Size = new System.Drawing.Size(121, 23);
            this.buttonSendIndividual.TabIndex = 23;
            this.buttonSendIndividual.Text = "Send individual";
            this.buttonSendIndividual.UseVisualStyleBackColor = true;
            this.buttonSendIndividual.Click += new System.EventHandler(this.buttonSendIndividual_Click);
            // 
            // buttonObtainIndividual
            // 
            this.buttonObtainIndividual.Location = new System.Drawing.Point(330, 620);
            this.buttonObtainIndividual.Name = "buttonObtainIndividual";
            this.buttonObtainIndividual.Size = new System.Drawing.Size(121, 23);
            this.buttonObtainIndividual.TabIndex = 22;
            this.buttonObtainIndividual.Text = "Obtain individual";
            this.buttonObtainIndividual.UseVisualStyleBackColor = true;
            this.buttonObtainIndividual.Click += new System.EventHandler(this.buttonObtainIndividual_Click);
            // 
            // buttonDeleteAllCard
            // 
            this.buttonDeleteAllCard.Location = new System.Drawing.Point(329, 657);
            this.buttonDeleteAllCard.Name = "buttonDeleteAllCard";
            this.buttonDeleteAllCard.Size = new System.Drawing.Size(121, 23);
            this.buttonDeleteAllCard.TabIndex = 25;
            this.buttonDeleteAllCard.Text = "Delete all card";
            this.buttonDeleteAllCard.UseVisualStyleBackColor = true;
            this.buttonDeleteAllCard.Click += new System.EventHandler(this.buttonDeleteAllCard_Click);
            // 
            // columnHeader17
            // 
            this.columnHeader17.Text = "Belong to";
            this.columnHeader17.Width = 80;
            // 
            // buttonExit
            // 
            this.buttonExit.Location = new System.Drawing.Point(467, 657);
            this.buttonExit.Name = "buttonExit";
            this.buttonExit.Size = new System.Drawing.Size(121, 23);
            this.buttonExit.TabIndex = 27;
            this.buttonExit.Text = "Exit";
            this.buttonExit.UseVisualStyleBackColor = true;
            this.buttonExit.Click += new System.EventHandler(this.buttonExit_Click);
            // 
            // columnHeader16
            // 
            this.columnHeader16.Text = "Group number";
            this.columnHeader16.Width = 100;
            // 
            // listViewGataManage
            // 
            this.listViewGataManage.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader1,
            this.columnHeader2,
            this.columnHeader3,
            this.columnHeader4,
            this.columnHeader5,
            this.columnHeader6,
            this.columnHeader7,
            this.columnHeader8,
            this.columnHeader9,
            this.columnHeader10,
            this.columnHeader11,
            this.columnHeader12,
            this.columnHeader13,
            this.columnHeader23});
            this.listViewGataManage.FullRowSelect = true;
            this.listViewGataManage.GridLines = true;
            this.listViewGataManage.Location = new System.Drawing.Point(12, 12);
            this.listViewGataManage.MultiSelect = false;
            this.listViewGataManage.Name = "listViewGataManage";
            this.listViewGataManage.Size = new System.Drawing.Size(927, 297);
            this.listViewGataManage.TabIndex = 15;
            this.listViewGataManage.TabStop = false;
            this.listViewGataManage.UseCompatibleStateImageBehavior = false;
            this.listViewGataManage.View = System.Windows.Forms.View.Details;
            this.listViewGataManage.ItemSelectionChanged += new System.Windows.Forms.ListViewItemSelectionChangedEventHandler(this.listViewGataManage_ItemSelectionChanged);
            // 
            // columnHeader1
            // 
            this.columnHeader1.Text = "No.";
            this.columnHeader1.Width = 31;
            // 
            // columnHeader2
            // 
            this.columnHeader2.Text = "Card number";
            this.columnHeader2.Width = 80;
            // 
            // columnHeader3
            // 
            this.columnHeader3.Text = "Card valid";
            this.columnHeader3.Width = 80;
            // 
            // columnHeader4
            // 
            this.columnHeader4.Text = "Card password";
            this.columnHeader4.Width = 90;
            // 
            // columnHeader5
            // 
            this.columnHeader5.Text = "Card type";
            this.columnHeader5.Width = 74;
            // 
            // columnHeader6
            // 
            this.columnHeader6.Text = "First card";
            this.columnHeader6.Width = 78;
            // 
            // columnHeader7
            // 
            this.columnHeader7.Text = "Maximum Swiping Num";
            this.columnHeader7.Width = 88;
            // 
            // columnHeader8
            // 
            this.columnHeader8.Text = "Swaped Times";
            this.columnHeader8.Width = 86;
            // 
            // columnHeader9
            // 
            this.columnHeader9.Text = "Validity period";
            this.columnHeader9.Width = 61;
            // 
            // columnHeader10
            // 
            this.columnHeader10.Text = "Start time";
            this.columnHeader10.Width = 80;
            // 
            // columnHeader11
            // 
            this.columnHeader11.Text = "End time";
            // 
            // columnHeader12
            // 
            this.columnHeader12.Text = "Send state";
            this.columnHeader12.Width = 72;
            // 
            // columnHeader13
            // 
            this.columnHeader13.Text = "RoomNo.";
            this.columnHeader13.Width = 80;
            // 
            // columnHeader23
            // 
            this.columnHeader23.Text = "FloorNo.";
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.listViewDoorAuthority);
            this.groupBox1.Location = new System.Drawing.Point(11, 335);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(290, 211);
            this.groupBox1.TabIndex = 16;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Door authority";
            // 
            // listViewDoorAuthority
            // 
            this.listViewDoorAuthority.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader14,
            this.columnHeader15});
            this.listViewDoorAuthority.FullRowSelect = true;
            this.listViewDoorAuthority.GridLines = true;
            this.listViewDoorAuthority.Location = new System.Drawing.Point(6, 20);
            this.listViewDoorAuthority.MultiSelect = false;
            this.listViewDoorAuthority.Name = "listViewDoorAuthority";
            this.listViewDoorAuthority.Size = new System.Drawing.Size(278, 185);
            this.listViewDoorAuthority.TabIndex = 0;
            this.listViewDoorAuthority.TabStop = false;
            this.listViewDoorAuthority.UseCompatibleStateImageBehavior = false;
            this.listViewDoorAuthority.View = System.Windows.Forms.View.Details;
            // 
            // columnHeader14
            // 
            this.columnHeader14.Text = "Door number";
            this.columnHeader14.Width = 90;
            // 
            // columnHeader15
            // 
            this.columnHeader15.Text = "Authority";
            this.columnHeader15.Width = 80;
            // 
            // listViewGroup
            // 
            this.listViewGroup.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader16,
            this.columnHeader17});
            this.listViewGroup.FullRowSelect = true;
            this.listViewGroup.GridLines = true;
            this.listViewGroup.Location = new System.Drawing.Point(6, 20);
            this.listViewGroup.MultiSelect = false;
            this.listViewGroup.Name = "listViewGroup";
            this.listViewGroup.Size = new System.Drawing.Size(278, 185);
            this.listViewGroup.TabIndex = 0;
            this.listViewGroup.TabStop = false;
            this.listViewGroup.UseCompatibleStateImageBehavior = false;
            this.listViewGroup.View = System.Windows.Forms.View.Details;
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.listViewGroup);
            this.groupBox2.Location = new System.Drawing.Point(329, 335);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(290, 211);
            this.groupBox2.TabIndex = 18;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "The group";
            // 
            // textBoxCardNo
            // 
            this.textBoxCardNo.Location = new System.Drawing.Point(330, 586);
            this.textBoxCardNo.Name = "textBoxCardNo";
            this.textBoxCardNo.Size = new System.Drawing.Size(100, 21);
            this.textBoxCardNo.TabIndex = 28;
            // 
            // CardManagement
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(949, 696);
            this.Controls.Add(this.textBoxCardNo);
            this.Controls.Add(this.buttonDeleteCard);
            this.Controls.Add(this.groupBox3);
            this.Controls.Add(this.groupBox4);
            this.Controls.Add(this.buttonSendAll);
            this.Controls.Add(this.buttonGetAll);
            this.Controls.Add(this.listViewMessage);
            this.Controls.Add(this.buttonSendIndividual);
            this.Controls.Add(this.buttonObtainIndividual);
            this.Controls.Add(this.buttonDeleteAllCard);
            this.Controls.Add(this.buttonExit);
            this.Controls.Add(this.listViewGataManage);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.groupBox2);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
            this.Name = "CardManagement";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "CardManagement";
            this.Load += new System.EventHandler(this.CardManagement_Load);
            this.groupBox3.ResumeLayout(false);
            this.groupBox4.ResumeLayout(false);
            this.groupBox4.PerformLayout();
            this.groupBox1.ResumeLayout(false);
            this.groupBox2.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button buttonDeleteCard;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.ListView listViewPlaTemplateNumber;
        private System.Windows.Forms.ColumnHeader columnHeader18;
        private System.Windows.Forms.ColumnHeader columnHeader19;
        private System.Windows.Forms.ColumnHeader columnHeader20;
        private System.Windows.Forms.GroupBox groupBox4;
        private System.Windows.Forms.Button buttonEdit;
        private System.Windows.Forms.Button buttonBatchAdd;
        private System.Windows.Forms.CheckBox checkBoxCheckNumber;
        private System.Windows.Forms.Button buttonSendAll;
        private System.Windows.Forms.Button buttonGetAll;
        private System.Windows.Forms.ListView listViewMessage;
        private System.Windows.Forms.ColumnHeader columnHeader21;
        private System.Windows.Forms.ColumnHeader columnHeader22;
        private System.Windows.Forms.Button buttonSendIndividual;
        private System.Windows.Forms.Button buttonObtainIndividual;
        private System.Windows.Forms.Button buttonDeleteAllCard;
        private System.Windows.Forms.ColumnHeader columnHeader17;
        private System.Windows.Forms.Button buttonExit;
        private System.Windows.Forms.ColumnHeader columnHeader16;
        private System.Windows.Forms.ListView listViewGataManage;
        private System.Windows.Forms.ColumnHeader columnHeader1;
        private System.Windows.Forms.ColumnHeader columnHeader2;
        private System.Windows.Forms.ColumnHeader columnHeader3;
        private System.Windows.Forms.ColumnHeader columnHeader4;
        private System.Windows.Forms.ColumnHeader columnHeader5;
        private System.Windows.Forms.ColumnHeader columnHeader6;
        private System.Windows.Forms.ColumnHeader columnHeader7;
        private System.Windows.Forms.ColumnHeader columnHeader8;
        private System.Windows.Forms.ColumnHeader columnHeader9;
        private System.Windows.Forms.ColumnHeader columnHeader10;
        private System.Windows.Forms.ColumnHeader columnHeader11;
        private System.Windows.Forms.ColumnHeader columnHeader12;
        private System.Windows.Forms.ColumnHeader columnHeader13;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.ListView listViewDoorAuthority;
        private System.Windows.Forms.ColumnHeader columnHeader14;
        private System.Windows.Forms.ColumnHeader columnHeader15;
        private System.Windows.Forms.ListView listViewGroup;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.ColumnHeader columnHeader23;
        private System.Windows.Forms.TextBox textBoxControllerNo;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox textBoxCardNo;
    }
}