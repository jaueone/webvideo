namespace AcsDemo
{
    partial class FaceRecordManagement
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
            this.listViewFaceRecordManage = new System.Windows.Forms.ListView();
            this.columnHeader1 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader2 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader3 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader4 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader5 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.groupBoxFaceRecordAddOrModifyOrSearch = new System.Windows.Forms.GroupBox();
            this.buttonSearch = new System.Windows.Forms.Button();
            this.textBoxFDID = new System.Windows.Forms.TextBox();
            this.labelFDID = new System.Windows.Forms.Label();
            this.textBoxFPID = new System.Windows.Forms.TextBox();
            this.labelFPID = new System.Windows.Forms.Label();
            this.buttonUpdateList = new System.Windows.Forms.Button();
            this.buttonModify = new System.Windows.Forms.Button();
            this.textBoxPictureDataPath = new System.Windows.Forms.TextBox();
            this.labelPictureDataPath = new System.Windows.Forms.Label();
            this.buttonSend = new System.Windows.Forms.Button();
            this.buttonAdd = new System.Windows.Forms.Button();
            this.groupBoxFaceRecordDelete = new System.Windows.Forms.GroupBox();
            this.groupBoxFPIDCfg = new System.Windows.Forms.GroupBox();
            this.textBoxFPID2 = new System.Windows.Forms.TextBox();
            this.labelFPID2 = new System.Windows.Forms.Label();
            this.buttonUpdatFPID = new System.Windows.Forms.Button();
            this.listViewFPID = new System.Windows.Forms.ListView();
            this.columnHeader16 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader17 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.buttonDeleteByFPID = new System.Windows.Forms.Button();
            this.textBoxFDID2 = new System.Windows.Forms.TextBox();
            this.labelFDID2 = new System.Windows.Forms.Label();
            this.buttonDeleteAll = new System.Windows.Forms.Button();
            this.buttonClearList = new System.Windows.Forms.Button();
            this.listViewMessage = new System.Windows.Forms.ListView();
            this.columnHeader21 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader22 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.groupBoxFaceRecordCount = new System.Windows.Forms.GroupBox();
            this.buttonGetByFDID = new System.Windows.Forms.Button();
            this.textBoxFDID3 = new System.Windows.Forms.TextBox();
            this.labelFDID3 = new System.Windows.Forms.Label();
            this.textBoxRecordDataNumber = new System.Windows.Forms.TextBox();
            this.labelRecordDataNumber = new System.Windows.Forms.Label();
            this.buttonGetAllCount = new System.Windows.Forms.Button();
            this.groupBoxFaceRecordAddOrModifyOrSearch.SuspendLayout();
            this.groupBoxFaceRecordDelete.SuspendLayout();
            this.groupBoxFPIDCfg.SuspendLayout();
            this.groupBoxFaceRecordCount.SuspendLayout();
            this.SuspendLayout();
            // 
            // listViewFaceRecordManage
            // 
            this.listViewFaceRecordManage.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader1,
            this.columnHeader2,
            this.columnHeader3,
            this.columnHeader4,
            this.columnHeader5});
            this.listViewFaceRecordManage.FullRowSelect = true;
            this.listViewFaceRecordManage.GridLines = true;
            this.listViewFaceRecordManage.Location = new System.Drawing.Point(12, 12);
            this.listViewFaceRecordManage.MultiSelect = false;
            this.listViewFaceRecordManage.Name = "listViewFaceRecordManage";
            this.listViewFaceRecordManage.Size = new System.Drawing.Size(779, 236);
            this.listViewFaceRecordManage.TabIndex = 18;
            this.listViewFaceRecordManage.TabStop = false;
            this.listViewFaceRecordManage.UseCompatibleStateImageBehavior = false;
            this.listViewFaceRecordManage.View = System.Windows.Forms.View.Details;
            this.listViewFaceRecordManage.ItemSelectionChanged += new System.Windows.Forms.ListViewItemSelectionChangedEventHandler(this.listViewFaceRecordManage_ItemSelectionChanged);
            // 
            // columnHeader1
            // 
            this.columnHeader1.Text = "No";
            this.columnHeader1.Width = 45;
            // 
            // columnHeader2
            // 
            this.columnHeader2.Text = "faceLibType";
            this.columnHeader2.Width = 80;
            // 
            // columnHeader3
            // 
            this.columnHeader3.Text = "FDID";
            this.columnHeader3.Width = 70;
            // 
            // columnHeader4
            // 
            this.columnHeader4.Text = "FPID";
            this.columnHeader4.Width = 90;
            // 
            // columnHeader5
            // 
            this.columnHeader5.Text = "Picture Data Path";
            this.columnHeader5.Width = 200;
            // 
            // groupBoxFaceRecordAddOrModifyOrSearch
            // 
            this.groupBoxFaceRecordAddOrModifyOrSearch.Controls.Add(this.buttonSearch);
            this.groupBoxFaceRecordAddOrModifyOrSearch.Controls.Add(this.textBoxFDID);
            this.groupBoxFaceRecordAddOrModifyOrSearch.Controls.Add(this.labelFDID);
            this.groupBoxFaceRecordAddOrModifyOrSearch.Controls.Add(this.textBoxFPID);
            this.groupBoxFaceRecordAddOrModifyOrSearch.Controls.Add(this.labelFPID);
            this.groupBoxFaceRecordAddOrModifyOrSearch.Controls.Add(this.buttonUpdateList);
            this.groupBoxFaceRecordAddOrModifyOrSearch.Controls.Add(this.buttonModify);
            this.groupBoxFaceRecordAddOrModifyOrSearch.Controls.Add(this.textBoxPictureDataPath);
            this.groupBoxFaceRecordAddOrModifyOrSearch.Controls.Add(this.labelPictureDataPath);
            this.groupBoxFaceRecordAddOrModifyOrSearch.Controls.Add(this.buttonSend);
            this.groupBoxFaceRecordAddOrModifyOrSearch.Controls.Add(this.buttonAdd);
            this.groupBoxFaceRecordAddOrModifyOrSearch.Location = new System.Drawing.Point(12, 264);
            this.groupBoxFaceRecordAddOrModifyOrSearch.Name = "groupBoxFaceRecordAddOrModifyOrSearch";
            this.groupBoxFaceRecordAddOrModifyOrSearch.Size = new System.Drawing.Size(501, 122);
            this.groupBoxFaceRecordAddOrModifyOrSearch.TabIndex = 51;
            this.groupBoxFaceRecordAddOrModifyOrSearch.TabStop = false;
            this.groupBoxFaceRecordAddOrModifyOrSearch.Text = "Face Record Add Or Modify Or Search";
            // 
            // buttonSearch
            // 
            this.buttonSearch.Location = new System.Drawing.Point(424, 88);
            this.buttonSearch.Name = "buttonSearch";
            this.buttonSearch.Size = new System.Drawing.Size(65, 23);
            this.buttonSearch.TabIndex = 60;
            this.buttonSearch.Text = "Search";
            this.buttonSearch.UseVisualStyleBackColor = true;
            this.buttonSearch.Click += new System.EventHandler(this.buttonSearch_Click);
            // 
            // textBoxFDID
            // 
            this.textBoxFDID.Location = new System.Drawing.Point(46, 20);
            this.textBoxFDID.Name = "textBoxFDID";
            this.textBoxFDID.Size = new System.Drawing.Size(54, 21);
            this.textBoxFDID.TabIndex = 53;
            this.textBoxFDID.Text = "1";
            // 
            // labelFDID
            // 
            this.labelFDID.AutoSize = true;
            this.labelFDID.Location = new System.Drawing.Point(11, 23);
            this.labelFDID.Name = "labelFDID";
            this.labelFDID.Size = new System.Drawing.Size(29, 12);
            this.labelFDID.TabIndex = 52;
            this.labelFDID.Text = "FDID";
            // 
            // textBoxFPID
            // 
            this.textBoxFPID.Location = new System.Drawing.Point(168, 20);
            this.textBoxFPID.Name = "textBoxFPID";
            this.textBoxFPID.Size = new System.Drawing.Size(81, 21);
            this.textBoxFPID.TabIndex = 59;
            this.textBoxFPID.Text = "1";
            // 
            // labelFPID
            // 
            this.labelFPID.AutoSize = true;
            this.labelFPID.Location = new System.Drawing.Point(117, 23);
            this.labelFPID.Name = "labelFPID";
            this.labelFPID.Size = new System.Drawing.Size(29, 12);
            this.labelFPID.TabIndex = 58;
            this.labelFPID.Text = "FPID";
            // 
            // buttonUpdateList
            // 
            this.buttonUpdateList.Location = new System.Drawing.Point(124, 88);
            this.buttonUpdateList.Name = "buttonUpdateList";
            this.buttonUpdateList.Size = new System.Drawing.Size(88, 23);
            this.buttonUpdateList.TabIndex = 57;
            this.buttonUpdateList.Text = "Update List";
            this.buttonUpdateList.UseVisualStyleBackColor = true;
            this.buttonUpdateList.Click += new System.EventHandler(this.buttonUpdateList_Click);
            // 
            // buttonModify
            // 
            this.buttonModify.Location = new System.Drawing.Point(333, 88);
            this.buttonModify.Name = "buttonModify";
            this.buttonModify.Size = new System.Drawing.Size(65, 23);
            this.buttonModify.TabIndex = 55;
            this.buttonModify.Text = "Modify";
            this.buttonModify.UseVisualStyleBackColor = true;
            this.buttonModify.Click += new System.EventHandler(this.buttonModify_Click);
            // 
            // textBoxPictureDataPath
            // 
            this.textBoxPictureDataPath.Location = new System.Drawing.Point(124, 51);
            this.textBoxPictureDataPath.Name = "textBoxPictureDataPath";
            this.textBoxPictureDataPath.Size = new System.Drawing.Size(365, 21);
            this.textBoxPictureDataPath.TabIndex = 33;
            // 
            // labelPictureDataPath
            // 
            this.labelPictureDataPath.AutoSize = true;
            this.labelPictureDataPath.Location = new System.Drawing.Point(11, 54);
            this.labelPictureDataPath.Name = "labelPictureDataPath";
            this.labelPictureDataPath.Size = new System.Drawing.Size(107, 12);
            this.labelPictureDataPath.TabIndex = 32;
            this.labelPictureDataPath.Text = "Picture Data Path";
            // 
            // buttonSend
            // 
            this.buttonSend.Location = new System.Drawing.Point(240, 88);
            this.buttonSend.Name = "buttonSend";
            this.buttonSend.Size = new System.Drawing.Size(65, 23);
            this.buttonSend.TabIndex = 2;
            this.buttonSend.Text = "Send";
            this.buttonSend.UseVisualStyleBackColor = true;
            this.buttonSend.Click += new System.EventHandler(this.buttonSend_Click);
            // 
            // buttonAdd
            // 
            this.buttonAdd.Location = new System.Drawing.Point(13, 88);
            this.buttonAdd.Name = "buttonAdd";
            this.buttonAdd.Size = new System.Drawing.Size(88, 23);
            this.buttonAdd.TabIndex = 1;
            this.buttonAdd.Text = "Add To List";
            this.buttonAdd.UseVisualStyleBackColor = true;
            this.buttonAdd.Click += new System.EventHandler(this.buttonAdd_Click);
            // 
            // groupBoxFaceRecordDelete
            // 
            this.groupBoxFaceRecordDelete.Controls.Add(this.groupBoxFPIDCfg);
            this.groupBoxFaceRecordDelete.Controls.Add(this.listViewFPID);
            this.groupBoxFaceRecordDelete.Controls.Add(this.buttonDeleteByFPID);
            this.groupBoxFaceRecordDelete.Controls.Add(this.textBoxFDID2);
            this.groupBoxFaceRecordDelete.Controls.Add(this.labelFDID2);
            this.groupBoxFaceRecordDelete.Controls.Add(this.buttonDeleteAll);
            this.groupBoxFaceRecordDelete.Location = new System.Drawing.Point(12, 402);
            this.groupBoxFaceRecordDelete.Name = "groupBoxFaceRecordDelete";
            this.groupBoxFaceRecordDelete.Size = new System.Drawing.Size(501, 211);
            this.groupBoxFaceRecordDelete.TabIndex = 61;
            this.groupBoxFaceRecordDelete.TabStop = false;
            this.groupBoxFaceRecordDelete.Text = "Face Record Delete By FDID";
            // 
            // groupBoxFPIDCfg
            // 
            this.groupBoxFPIDCfg.Controls.Add(this.textBoxFPID2);
            this.groupBoxFPIDCfg.Controls.Add(this.labelFPID2);
            this.groupBoxFPIDCfg.Controls.Add(this.buttonUpdatFPID);
            this.groupBoxFPIDCfg.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.groupBoxFPIDCfg.Location = new System.Drawing.Point(206, 80);
            this.groupBoxFPIDCfg.Name = "groupBoxFPIDCfg";
            this.groupBoxFPIDCfg.Size = new System.Drawing.Size(177, 87);
            this.groupBoxFPIDCfg.TabIndex = 62;
            this.groupBoxFPIDCfg.TabStop = false;
            this.groupBoxFPIDCfg.Text = "FPID Cfg";
            // 
            // textBoxFPID2
            // 
            this.textBoxFPID2.Location = new System.Drawing.Point(87, 20);
            this.textBoxFPID2.Name = "textBoxFPID2";
            this.textBoxFPID2.Size = new System.Drawing.Size(59, 21);
            this.textBoxFPID2.TabIndex = 28;
            this.textBoxFPID2.Text = "1";
            // 
            // labelFPID2
            // 
            this.labelFPID2.AutoSize = true;
            this.labelFPID2.Location = new System.Drawing.Point(14, 23);
            this.labelFPID2.Name = "labelFPID2";
            this.labelFPID2.Size = new System.Drawing.Size(29, 12);
            this.labelFPID2.TabIndex = 27;
            this.labelFPID2.Text = "FPID";
            // 
            // buttonUpdatFPID
            // 
            this.buttonUpdatFPID.Location = new System.Drawing.Point(87, 47);
            this.buttonUpdatFPID.Name = "buttonUpdatFPID";
            this.buttonUpdatFPID.Size = new System.Drawing.Size(75, 23);
            this.buttonUpdatFPID.TabIndex = 20;
            this.buttonUpdatFPID.Text = "Update";
            this.buttonUpdatFPID.UseVisualStyleBackColor = true;
            this.buttonUpdatFPID.Click += new System.EventHandler(this.buttonUpdatFPID_Click);
            // 
            // listViewFPID
            // 
            this.listViewFPID.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader16,
            this.columnHeader17});
            this.listViewFPID.FullRowSelect = true;
            this.listViewFPID.GridLines = true;
            this.listViewFPID.HideSelection = false;
            this.listViewFPID.Location = new System.Drawing.Point(13, 56);
            this.listViewFPID.Name = "listViewFPID";
            this.listViewFPID.Size = new System.Drawing.Size(177, 111);
            this.listViewFPID.TabIndex = 61;
            this.listViewFPID.UseCompatibleStateImageBehavior = false;
            this.listViewFPID.View = System.Windows.Forms.View.Details;
            this.listViewFPID.ItemSelectionChanged += new System.Windows.Forms.ListViewItemSelectionChangedEventHandler(this.listViewFPID_ItemSelectionChanged);
            // 
            // columnHeader16
            // 
            this.columnHeader16.Text = "No";
            this.columnHeader16.Width = 50;
            // 
            // columnHeader17
            // 
            this.columnHeader17.Text = "FPID";
            this.columnHeader17.Width = 120;
            // 
            // buttonDeleteByFPID
            // 
            this.buttonDeleteByFPID.Location = new System.Drawing.Point(266, 178);
            this.buttonDeleteByFPID.Name = "buttonDeleteByFPID";
            this.buttonDeleteByFPID.Size = new System.Drawing.Size(102, 23);
            this.buttonDeleteByFPID.TabIndex = 60;
            this.buttonDeleteByFPID.Text = "Delete By FPID";
            this.buttonDeleteByFPID.UseVisualStyleBackColor = true;
            this.buttonDeleteByFPID.Click += new System.EventHandler(this.buttonDeleteByFPID_Click);
            // 
            // textBoxFDID2
            // 
            this.textBoxFDID2.Location = new System.Drawing.Point(46, 20);
            this.textBoxFDID2.Name = "textBoxFDID2";
            this.textBoxFDID2.Size = new System.Drawing.Size(54, 21);
            this.textBoxFDID2.TabIndex = 53;
            this.textBoxFDID2.Text = "1";
            // 
            // labelFDID2
            // 
            this.labelFDID2.AutoSize = true;
            this.labelFDID2.Location = new System.Drawing.Point(11, 23);
            this.labelFDID2.Name = "labelFDID2";
            this.labelFDID2.Size = new System.Drawing.Size(29, 12);
            this.labelFDID2.TabIndex = 52;
            this.labelFDID2.Text = "FDID";
            // 
            // buttonDeleteAll
            // 
            this.buttonDeleteAll.Location = new System.Drawing.Point(104, 178);
            this.buttonDeleteAll.Name = "buttonDeleteAll";
            this.buttonDeleteAll.Size = new System.Drawing.Size(86, 23);
            this.buttonDeleteAll.TabIndex = 55;
            this.buttonDeleteAll.Text = "Delete All";
            this.buttonDeleteAll.UseVisualStyleBackColor = true;
            this.buttonDeleteAll.Click += new System.EventHandler(this.buttonDeleteAll_Click);
            // 
            // buttonClearList
            // 
            this.buttonClearList.Location = new System.Drawing.Point(702, 254);
            this.buttonClearList.Name = "buttonClearList";
            this.buttonClearList.Size = new System.Drawing.Size(88, 23);
            this.buttonClearList.TabIndex = 62;
            this.buttonClearList.Text = "Clear List";
            this.buttonClearList.UseVisualStyleBackColor = true;
            this.buttonClearList.Click += new System.EventHandler(this.buttonClearList_Click);
            // 
            // listViewMessage
            // 
            this.listViewMessage.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader21,
            this.columnHeader22});
            this.listViewMessage.FullRowSelect = true;
            this.listViewMessage.GridLines = true;
            this.listViewMessage.Location = new System.Drawing.Point(530, 392);
            this.listViewMessage.Name = "listViewMessage";
            this.listViewMessage.Size = new System.Drawing.Size(260, 221);
            this.listViewMessage.TabIndex = 64;
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
            // groupBoxFaceRecordCount
            // 
            this.groupBoxFaceRecordCount.Controls.Add(this.buttonGetByFDID);
            this.groupBoxFaceRecordCount.Controls.Add(this.textBoxFDID3);
            this.groupBoxFaceRecordCount.Controls.Add(this.labelFDID3);
            this.groupBoxFaceRecordCount.Controls.Add(this.textBoxRecordDataNumber);
            this.groupBoxFaceRecordCount.Controls.Add(this.labelRecordDataNumber);
            this.groupBoxFaceRecordCount.Controls.Add(this.buttonGetAllCount);
            this.groupBoxFaceRecordCount.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.groupBoxFaceRecordCount.Location = new System.Drawing.Point(531, 283);
            this.groupBoxFaceRecordCount.Name = "groupBoxFaceRecordCount";
            this.groupBoxFaceRecordCount.Size = new System.Drawing.Size(260, 103);
            this.groupBoxFaceRecordCount.TabIndex = 63;
            this.groupBoxFaceRecordCount.TabStop = false;
            this.groupBoxFaceRecordCount.Text = "Face Record Count";
            // 
            // buttonGetByFDID
            // 
            this.buttonGetByFDID.Location = new System.Drawing.Point(143, 73);
            this.buttonGetByFDID.Name = "buttonGetByFDID";
            this.buttonGetByFDID.Size = new System.Drawing.Size(87, 23);
            this.buttonGetByFDID.TabIndex = 31;
            this.buttonGetByFDID.Text = "Get By FDID";
            this.buttonGetByFDID.UseVisualStyleBackColor = true;
            this.buttonGetByFDID.Click += new System.EventHandler(this.buttonGetByFDID_Click);
            // 
            // textBoxFDID3
            // 
            this.textBoxFDID3.Location = new System.Drawing.Point(134, 14);
            this.textBoxFDID3.Name = "textBoxFDID3";
            this.textBoxFDID3.Size = new System.Drawing.Size(59, 21);
            this.textBoxFDID3.TabIndex = 30;
            this.textBoxFDID3.Text = "1";
            // 
            // labelFDID3
            // 
            this.labelFDID3.AutoSize = true;
            this.labelFDID3.Location = new System.Drawing.Point(90, 14);
            this.labelFDID3.Name = "labelFDID3";
            this.labelFDID3.Size = new System.Drawing.Size(29, 12);
            this.labelFDID3.TabIndex = 29;
            this.labelFDID3.Text = "FDID";
            // 
            // textBoxRecordDataNumber
            // 
            this.textBoxRecordDataNumber.Location = new System.Drawing.Point(134, 46);
            this.textBoxRecordDataNumber.Name = "textBoxRecordDataNumber";
            this.textBoxRecordDataNumber.Size = new System.Drawing.Size(59, 21);
            this.textBoxRecordDataNumber.TabIndex = 28;
            this.textBoxRecordDataNumber.Text = "0";
            // 
            // labelRecordDataNumber
            // 
            this.labelRecordDataNumber.AutoSize = true;
            this.labelRecordDataNumber.Location = new System.Drawing.Point(6, 49);
            this.labelRecordDataNumber.Name = "labelRecordDataNumber";
            this.labelRecordDataNumber.Size = new System.Drawing.Size(113, 12);
            this.labelRecordDataNumber.TabIndex = 27;
            this.labelRecordDataNumber.Text = "record Data Number";
            // 
            // buttonGetAllCount
            // 
            this.buttonGetAllCount.Location = new System.Drawing.Point(27, 73);
            this.buttonGetAllCount.Name = "buttonGetAllCount";
            this.buttonGetAllCount.Size = new System.Drawing.Size(78, 23);
            this.buttonGetAllCount.TabIndex = 20;
            this.buttonGetAllCount.Text = "Get All";
            this.buttonGetAllCount.UseVisualStyleBackColor = true;
            this.buttonGetAllCount.Visible = false;
            this.buttonGetAllCount.Click += new System.EventHandler(this.buttonGetAllCount_Click);
            // 
            // FaceRecordManagement
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(806, 621);
            this.Controls.Add(this.listViewMessage);
            this.Controls.Add(this.groupBoxFaceRecordCount);
            this.Controls.Add(this.buttonClearList);
            this.Controls.Add(this.groupBoxFaceRecordDelete);
            this.Controls.Add(this.groupBoxFaceRecordAddOrModifyOrSearch);
            this.Controls.Add(this.listViewFaceRecordManage);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
            this.Name = "FaceRecordManagement";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Face Record Management";
            this.Load += new System.EventHandler(this.FaceRecordManagement_Load);
            this.groupBoxFaceRecordAddOrModifyOrSearch.ResumeLayout(false);
            this.groupBoxFaceRecordAddOrModifyOrSearch.PerformLayout();
            this.groupBoxFaceRecordDelete.ResumeLayout(false);
            this.groupBoxFaceRecordDelete.PerformLayout();
            this.groupBoxFPIDCfg.ResumeLayout(false);
            this.groupBoxFPIDCfg.PerformLayout();
            this.groupBoxFaceRecordCount.ResumeLayout(false);
            this.groupBoxFaceRecordCount.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.ListView listViewFaceRecordManage;
        private System.Windows.Forms.ColumnHeader columnHeader1;
        private System.Windows.Forms.ColumnHeader columnHeader2;
        private System.Windows.Forms.ColumnHeader columnHeader3;
        private System.Windows.Forms.ColumnHeader columnHeader4;
        private System.Windows.Forms.ColumnHeader columnHeader5;
        private System.Windows.Forms.GroupBox groupBoxFaceRecordAddOrModifyOrSearch;
        private System.Windows.Forms.Button buttonUpdateList;
        private System.Windows.Forms.Button buttonModify;
        private System.Windows.Forms.TextBox textBoxPictureDataPath;
        private System.Windows.Forms.Label labelPictureDataPath;
        private System.Windows.Forms.Button buttonSend;
        private System.Windows.Forms.Button buttonAdd;
        private System.Windows.Forms.TextBox textBoxFDID;
        private System.Windows.Forms.Label labelFDID;
        private System.Windows.Forms.TextBox textBoxFPID;
        private System.Windows.Forms.Label labelFPID;
        private System.Windows.Forms.Button buttonSearch;
        private System.Windows.Forms.GroupBox groupBoxFaceRecordDelete;
        private System.Windows.Forms.Button buttonDeleteByFPID;
        private System.Windows.Forms.TextBox textBoxFDID2;
        private System.Windows.Forms.Label labelFDID2;
        private System.Windows.Forms.Button buttonDeleteAll;
        private System.Windows.Forms.ListView listViewFPID;
        private System.Windows.Forms.ColumnHeader columnHeader16;
        private System.Windows.Forms.ColumnHeader columnHeader17;
        private System.Windows.Forms.GroupBox groupBoxFPIDCfg;
        private System.Windows.Forms.TextBox textBoxFPID2;
        private System.Windows.Forms.Label labelFPID2;
        private System.Windows.Forms.Button buttonUpdatFPID;
        private System.Windows.Forms.Button buttonClearList;
        private System.Windows.Forms.ListView listViewMessage;
        private System.Windows.Forms.ColumnHeader columnHeader21;
        private System.Windows.Forms.ColumnHeader columnHeader22;
        private System.Windows.Forms.GroupBox groupBoxFaceRecordCount;
        private System.Windows.Forms.Button buttonGetByFDID;
        private System.Windows.Forms.TextBox textBoxFDID3;
        private System.Windows.Forms.Label labelFDID3;
        private System.Windows.Forms.TextBox textBoxRecordDataNumber;
        private System.Windows.Forms.Label labelRecordDataNumber;
        private System.Windows.Forms.Button buttonGetAllCount;
    }
}