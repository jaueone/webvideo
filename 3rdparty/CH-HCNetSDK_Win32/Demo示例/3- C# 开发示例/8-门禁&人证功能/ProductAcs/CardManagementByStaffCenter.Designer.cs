namespace AcsDemo.ProductAcs
{
    partial class CardManagementByStaffCenter
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
            this.listViewCardManage = new System.Windows.Forms.ListView();
            this.columnHeader1 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader2 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader3 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader4 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader5 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.groupBoxCardAddOrModify = new System.Windows.Forms.GroupBox();
            this.buttonUpdateList = new System.Windows.Forms.Button();
            this.buttonModify = new System.Windows.Forms.Button();
            this.textBoxLeaderCard = new System.Windows.Forms.TextBox();
            this.labelLeaderCard = new System.Windows.Forms.Label();
            this.comboBoxCardType = new System.Windows.Forms.ComboBox();
            this.labelCardType = new System.Windows.Forms.Label();
            this.textBoxCardNo = new System.Windows.Forms.TextBox();
            this.labelCardNo = new System.Windows.Forms.Label();
            this.textBoxEmployeeNo = new System.Windows.Forms.TextBox();
            this.labelEmployeeNo = new System.Windows.Forms.Label();
            this.textBoxAddCardNumber = new System.Windows.Forms.TextBox();
            this.labelAddCardNumber = new System.Windows.Forms.Label();
            this.buttonSend = new System.Windows.Forms.Button();
            this.buttonAdd = new System.Windows.Forms.Button();
            this.groupBoxCardSearchOrDelete = new System.Windows.Forms.GroupBox();
            this.buttonDeleteByCardNo = new System.Windows.Forms.Button();
            this.groupBoxCardNoCfg = new System.Windows.Forms.GroupBox();
            this.textBoxCardNo2 = new System.Windows.Forms.TextBox();
            this.labelCardNo2 = new System.Windows.Forms.Label();
            this.buttonUpdateCardNo = new System.Windows.Forms.Button();
            this.buttonSearchByCardNo = new System.Windows.Forms.Button();
            this.listViewCardNo = new System.Windows.Forms.ListView();
            this.columnHeader6 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader7 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.buttonDeleteByEmployeeNo = new System.Windows.Forms.Button();
            this.buttonDeleteAll = new System.Windows.Forms.Button();
            this.groupBoxEmployeeNoCfg = new System.Windows.Forms.GroupBox();
            this.textBoxEmployeeNo2 = new System.Windows.Forms.TextBox();
            this.labelEmployeeNo2 = new System.Windows.Forms.Label();
            this.buttonUpdateEmployeeNo = new System.Windows.Forms.Button();
            this.buttonSearchByEmployeeNo = new System.Windows.Forms.Button();
            this.listViewEmployeeNo = new System.Windows.Forms.ListView();
            this.columnHeader16 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader17 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.buttonSearchAll = new System.Windows.Forms.Button();
            this.groupBoxCardInfoCount = new System.Windows.Forms.GroupBox();
            this.buttonGetByEmployeeNo = new System.Windows.Forms.Button();
            this.textBoxEmployeeNo3 = new System.Windows.Forms.TextBox();
            this.labelEmployeeNo3 = new System.Windows.Forms.Label();
            this.textBoxCardInfoCount = new System.Windows.Forms.TextBox();
            this.labelCardInfoCount = new System.Windows.Forms.Label();
            this.buttonGetAllCount = new System.Windows.Forms.Button();
            this.buttonClearList = new System.Windows.Forms.Button();
            this.listViewMessage = new System.Windows.Forms.ListView();
            this.columnHeader21 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader22 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.buttonSyn = new System.Windows.Forms.Button();
            this.groupBoxCardAddOrModify.SuspendLayout();
            this.groupBoxCardSearchOrDelete.SuspendLayout();
            this.groupBoxCardNoCfg.SuspendLayout();
            this.groupBoxEmployeeNoCfg.SuspendLayout();
            this.groupBoxCardInfoCount.SuspendLayout();
            this.SuspendLayout();
            // 
            // listViewCardManage
            // 
            this.listViewCardManage.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader1,
            this.columnHeader2,
            this.columnHeader3,
            this.columnHeader4,
            this.columnHeader5});
            this.listViewCardManage.FullRowSelect = true;
            this.listViewCardManage.GridLines = true;
            this.listViewCardManage.Location = new System.Drawing.Point(12, 12);
            this.listViewCardManage.MultiSelect = false;
            this.listViewCardManage.Name = "listViewCardManage";
            this.listViewCardManage.Size = new System.Drawing.Size(878, 236);
            this.listViewCardManage.TabIndex = 17;
            this.listViewCardManage.TabStop = false;
            this.listViewCardManage.UseCompatibleStateImageBehavior = false;
            this.listViewCardManage.View = System.Windows.Forms.View.Details;
            this.listViewCardManage.ItemSelectionChanged += new System.Windows.Forms.ListViewItemSelectionChangedEventHandler(this.listViewCardManage_ItemSelectionChanged);
            // 
            // columnHeader1
            // 
            this.columnHeader1.Text = "No";
            this.columnHeader1.Width = 45;
            // 
            // columnHeader2
            // 
            this.columnHeader2.Text = "employeeNo";
            this.columnHeader2.Width = 75;
            // 
            // columnHeader3
            // 
            this.columnHeader3.Text = "cardNo";
            this.columnHeader3.Width = 70;
            // 
            // columnHeader4
            // 
            this.columnHeader4.Text = "cardType";
            this.columnHeader4.Width = 80;
            // 
            // columnHeader5
            // 
            this.columnHeader5.Text = "leaderCard";
            this.columnHeader5.Width = 108;
            // 
            // groupBoxCardAddOrModify
            // 
            this.groupBoxCardAddOrModify.Controls.Add(this.buttonUpdateList);
            this.groupBoxCardAddOrModify.Controls.Add(this.buttonModify);
            this.groupBoxCardAddOrModify.Controls.Add(this.textBoxLeaderCard);
            this.groupBoxCardAddOrModify.Controls.Add(this.labelLeaderCard);
            this.groupBoxCardAddOrModify.Controls.Add(this.comboBoxCardType);
            this.groupBoxCardAddOrModify.Controls.Add(this.labelCardType);
            this.groupBoxCardAddOrModify.Controls.Add(this.textBoxCardNo);
            this.groupBoxCardAddOrModify.Controls.Add(this.labelCardNo);
            this.groupBoxCardAddOrModify.Controls.Add(this.textBoxEmployeeNo);
            this.groupBoxCardAddOrModify.Controls.Add(this.labelEmployeeNo);
            this.groupBoxCardAddOrModify.Controls.Add(this.textBoxAddCardNumber);
            this.groupBoxCardAddOrModify.Controls.Add(this.labelAddCardNumber);
            this.groupBoxCardAddOrModify.Controls.Add(this.buttonSend);
            this.groupBoxCardAddOrModify.Controls.Add(this.buttonAdd);
            this.groupBoxCardAddOrModify.Location = new System.Drawing.Point(12, 267);
            this.groupBoxCardAddOrModify.Name = "groupBoxCardAddOrModify";
            this.groupBoxCardAddOrModify.Size = new System.Drawing.Size(196, 319);
            this.groupBoxCardAddOrModify.TabIndex = 50;
            this.groupBoxCardAddOrModify.TabStop = false;
            this.groupBoxCardAddOrModify.Text = "Card Add Or Modify";
            // 
            // buttonUpdateList
            // 
            this.buttonUpdateList.Location = new System.Drawing.Point(102, 214);
            this.buttonUpdateList.Name = "buttonUpdateList";
            this.buttonUpdateList.Size = new System.Drawing.Size(88, 23);
            this.buttonUpdateList.TabIndex = 57;
            this.buttonUpdateList.Text = "Update List";
            this.buttonUpdateList.UseVisualStyleBackColor = true;
            this.buttonUpdateList.Click += new System.EventHandler(this.buttonUpdateList_Click);
            // 
            // buttonModify
            // 
            this.buttonModify.Location = new System.Drawing.Point(117, 260);
            this.buttonModify.Name = "buttonModify";
            this.buttonModify.Size = new System.Drawing.Size(65, 23);
            this.buttonModify.TabIndex = 55;
            this.buttonModify.Text = "Modify";
            this.buttonModify.UseVisualStyleBackColor = true;
            this.buttonModify.Click += new System.EventHandler(this.buttonModify_Click);
            // 
            // textBoxLeaderCard
            // 
            this.textBoxLeaderCard.Location = new System.Drawing.Point(95, 163);
            this.textBoxLeaderCard.Name = "textBoxLeaderCard";
            this.textBoxLeaderCard.Size = new System.Drawing.Size(86, 21);
            this.textBoxLeaderCard.TabIndex = 39;
            this.textBoxLeaderCard.Text = "1,3,5";
            // 
            // labelLeaderCard
            // 
            this.labelLeaderCard.AutoSize = true;
            this.labelLeaderCard.Location = new System.Drawing.Point(18, 167);
            this.labelLeaderCard.Name = "labelLeaderCard";
            this.labelLeaderCard.Size = new System.Drawing.Size(65, 12);
            this.labelLeaderCard.TabIndex = 38;
            this.labelLeaderCard.Text = "leaderCard";
            // 
            // comboBoxCardType
            // 
            this.comboBoxCardType.FormattingEnabled = true;
            this.comboBoxCardType.Items.AddRange(new object[] {
            "normalCard",
            "patrolCard",
            "hijackCard",
            "superCard",
            "dismissingCard",
            "emergencyCard"});
            this.comboBoxCardType.Location = new System.Drawing.Point(82, 122);
            this.comboBoxCardType.Name = "comboBoxCardType";
            this.comboBoxCardType.Size = new System.Drawing.Size(88, 20);
            this.comboBoxCardType.TabIndex = 36;
            // 
            // labelCardType
            // 
            this.labelCardType.AutoSize = true;
            this.labelCardType.Location = new System.Drawing.Point(17, 127);
            this.labelCardType.Name = "labelCardType";
            this.labelCardType.Size = new System.Drawing.Size(53, 12);
            this.labelCardType.TabIndex = 35;
            this.labelCardType.Text = "cardType";
            // 
            // textBoxCardNo
            // 
            this.textBoxCardNo.Location = new System.Drawing.Point(70, 85);
            this.textBoxCardNo.Name = "textBoxCardNo";
            this.textBoxCardNo.Size = new System.Drawing.Size(112, 21);
            this.textBoxCardNo.TabIndex = 33;
            this.textBoxCardNo.Text = "111";
            // 
            // labelCardNo
            // 
            this.labelCardNo.AutoSize = true;
            this.labelCardNo.Location = new System.Drawing.Point(11, 88);
            this.labelCardNo.Name = "labelCardNo";
            this.labelCardNo.Size = new System.Drawing.Size(41, 12);
            this.labelCardNo.TabIndex = 32;
            this.labelCardNo.Text = "cardNo";
            // 
            // textBoxEmployeeNo
            // 
            this.textBoxEmployeeNo.Location = new System.Drawing.Point(86, 49);
            this.textBoxEmployeeNo.Name = "textBoxEmployeeNo";
            this.textBoxEmployeeNo.Size = new System.Drawing.Size(54, 21);
            this.textBoxEmployeeNo.TabIndex = 31;
            this.textBoxEmployeeNo.Text = "1";
            // 
            // labelEmployeeNo
            // 
            this.labelEmployeeNo.AutoSize = true;
            this.labelEmployeeNo.Location = new System.Drawing.Point(15, 52);
            this.labelEmployeeNo.Name = "labelEmployeeNo";
            this.labelEmployeeNo.Size = new System.Drawing.Size(65, 12);
            this.labelEmployeeNo.TabIndex = 30;
            this.labelEmployeeNo.Text = "employeeNo";
            // 
            // textBoxAddCardNumber
            // 
            this.textBoxAddCardNumber.Location = new System.Drawing.Point(116, 14);
            this.textBoxAddCardNumber.Name = "textBoxAddCardNumber";
            this.textBoxAddCardNumber.Size = new System.Drawing.Size(54, 21);
            this.textBoxAddCardNumber.TabIndex = 29;
            this.textBoxAddCardNumber.Text = "1";
            // 
            // labelAddCardNumber
            // 
            this.labelAddCardNumber.AutoSize = true;
            this.labelAddCardNumber.Location = new System.Drawing.Point(15, 17);
            this.labelAddCardNumber.Name = "labelAddCardNumber";
            this.labelAddCardNumber.Size = new System.Drawing.Size(95, 12);
            this.labelAddCardNumber.TabIndex = 28;
            this.labelAddCardNumber.Text = "Add Card Number";
            // 
            // buttonSend
            // 
            this.buttonSend.Location = new System.Drawing.Point(18, 260);
            this.buttonSend.Name = "buttonSend";
            this.buttonSend.Size = new System.Drawing.Size(65, 23);
            this.buttonSend.TabIndex = 2;
            this.buttonSend.Text = "Send";
            this.buttonSend.UseVisualStyleBackColor = true;
            this.buttonSend.Click += new System.EventHandler(this.buttonSend_Click);
            // 
            // buttonAdd
            // 
            this.buttonAdd.Location = new System.Drawing.Point(6, 214);
            this.buttonAdd.Name = "buttonAdd";
            this.buttonAdd.Size = new System.Drawing.Size(88, 23);
            this.buttonAdd.TabIndex = 1;
            this.buttonAdd.Text = "Add To List";
            this.buttonAdd.UseVisualStyleBackColor = true;
            this.buttonAdd.Click += new System.EventHandler(this.buttonAdd_Click);
            // 
            // groupBoxCardSearchOrDelete
            // 
            this.groupBoxCardSearchOrDelete.Controls.Add(this.buttonDeleteByCardNo);
            this.groupBoxCardSearchOrDelete.Controls.Add(this.groupBoxCardNoCfg);
            this.groupBoxCardSearchOrDelete.Controls.Add(this.buttonSearchByCardNo);
            this.groupBoxCardSearchOrDelete.Controls.Add(this.listViewCardNo);
            this.groupBoxCardSearchOrDelete.Controls.Add(this.buttonDeleteByEmployeeNo);
            this.groupBoxCardSearchOrDelete.Controls.Add(this.buttonDeleteAll);
            this.groupBoxCardSearchOrDelete.Controls.Add(this.groupBoxEmployeeNoCfg);
            this.groupBoxCardSearchOrDelete.Controls.Add(this.buttonSearchByEmployeeNo);
            this.groupBoxCardSearchOrDelete.Controls.Add(this.listViewEmployeeNo);
            this.groupBoxCardSearchOrDelete.Controls.Add(this.buttonSearchAll);
            this.groupBoxCardSearchOrDelete.Location = new System.Drawing.Point(214, 267);
            this.groupBoxCardSearchOrDelete.Name = "groupBoxCardSearchOrDelete";
            this.groupBoxCardSearchOrDelete.Size = new System.Drawing.Size(410, 319);
            this.groupBoxCardSearchOrDelete.TabIndex = 56;
            this.groupBoxCardSearchOrDelete.TabStop = false;
            this.groupBoxCardSearchOrDelete.Text = "Card Search Or Delete";
            // 
            // buttonDeleteByCardNo
            // 
            this.buttonDeleteByCardNo.Location = new System.Drawing.Point(243, 289);
            this.buttonDeleteByCardNo.Name = "buttonDeleteByCardNo";
            this.buttonDeleteByCardNo.Size = new System.Drawing.Size(137, 23);
            this.buttonDeleteByCardNo.TabIndex = 63;
            this.buttonDeleteByCardNo.Text = "Delete By CardNo";
            this.buttonDeleteByCardNo.UseVisualStyleBackColor = true;
            this.buttonDeleteByCardNo.Click += new System.EventHandler(this.buttonDeleteByCardNo_Click);
            // 
            // groupBoxCardNoCfg
            // 
            this.groupBoxCardNoCfg.Controls.Add(this.textBoxCardNo2);
            this.groupBoxCardNoCfg.Controls.Add(this.labelCardNo2);
            this.groupBoxCardNoCfg.Controls.Add(this.buttonUpdateCardNo);
            this.groupBoxCardNoCfg.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.groupBoxCardNoCfg.Location = new System.Drawing.Point(218, 167);
            this.groupBoxCardNoCfg.Name = "groupBoxCardNoCfg";
            this.groupBoxCardNoCfg.Size = new System.Drawing.Size(177, 87);
            this.groupBoxCardNoCfg.TabIndex = 60;
            this.groupBoxCardNoCfg.TabStop = false;
            this.groupBoxCardNoCfg.Text = "CardNo Cfg";
            // 
            // textBoxCardNo2
            // 
            this.textBoxCardNo2.Location = new System.Drawing.Point(103, 20);
            this.textBoxCardNo2.Name = "textBoxCardNo2";
            this.textBoxCardNo2.Size = new System.Drawing.Size(59, 21);
            this.textBoxCardNo2.TabIndex = 28;
            this.textBoxCardNo2.Text = "111";
            // 
            // labelCardNo2
            // 
            this.labelCardNo2.AutoSize = true;
            this.labelCardNo2.Location = new System.Drawing.Point(3, 23);
            this.labelCardNo2.Name = "labelCardNo2";
            this.labelCardNo2.Size = new System.Drawing.Size(41, 12);
            this.labelCardNo2.TabIndex = 27;
            this.labelCardNo2.Text = "cardNo";
            // 
            // buttonUpdateCardNo
            // 
            this.buttonUpdateCardNo.Location = new System.Drawing.Point(87, 47);
            this.buttonUpdateCardNo.Name = "buttonUpdateCardNo";
            this.buttonUpdateCardNo.Size = new System.Drawing.Size(75, 23);
            this.buttonUpdateCardNo.TabIndex = 20;
            this.buttonUpdateCardNo.Text = "Update";
            this.buttonUpdateCardNo.UseVisualStyleBackColor = true;
            this.buttonUpdateCardNo.Click += new System.EventHandler(this.buttonUpdateCardNo_Click);
            // 
            // buttonSearchByCardNo
            // 
            this.buttonSearchByCardNo.Location = new System.Drawing.Point(243, 260);
            this.buttonSearchByCardNo.Name = "buttonSearchByCardNo";
            this.buttonSearchByCardNo.Size = new System.Drawing.Size(137, 23);
            this.buttonSearchByCardNo.TabIndex = 62;
            this.buttonSearchByCardNo.Text = "Search By CardNo";
            this.buttonSearchByCardNo.UseVisualStyleBackColor = true;
            this.buttonSearchByCardNo.Click += new System.EventHandler(this.buttonSearchByCardNo_Click);
            // 
            // listViewCardNo
            // 
            this.listViewCardNo.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader6,
            this.columnHeader7});
            this.listViewCardNo.FullRowSelect = true;
            this.listViewCardNo.GridLines = true;
            this.listViewCardNo.HideSelection = false;
            this.listViewCardNo.Location = new System.Drawing.Point(218, 49);
            this.listViewCardNo.Name = "listViewCardNo";
            this.listViewCardNo.Size = new System.Drawing.Size(177, 111);
            this.listViewCardNo.TabIndex = 61;
            this.listViewCardNo.UseCompatibleStateImageBehavior = false;
            this.listViewCardNo.View = System.Windows.Forms.View.Details;
            this.listViewCardNo.ItemSelectionChanged += new System.Windows.Forms.ListViewItemSelectionChangedEventHandler(this.listViewCardNo_ItemSelectionChanged);
            // 
            // columnHeader6
            // 
            this.columnHeader6.Text = "No";
            this.columnHeader6.Width = 50;
            // 
            // columnHeader7
            // 
            this.columnHeader7.Text = "cardNo";
            this.columnHeader7.Width = 120;
            // 
            // buttonDeleteByEmployeeNo
            // 
            this.buttonDeleteByEmployeeNo.Location = new System.Drawing.Point(47, 289);
            this.buttonDeleteByEmployeeNo.Name = "buttonDeleteByEmployeeNo";
            this.buttonDeleteByEmployeeNo.Size = new System.Drawing.Size(137, 23);
            this.buttonDeleteByEmployeeNo.TabIndex = 59;
            this.buttonDeleteByEmployeeNo.Text = "Delete By EmployeeNo";
            this.buttonDeleteByEmployeeNo.UseVisualStyleBackColor = true;
            this.buttonDeleteByEmployeeNo.Click += new System.EventHandler(this.buttonDeleteByEmployeeNo_Click);
            // 
            // buttonDeleteAll
            // 
            this.buttonDeleteAll.Location = new System.Drawing.Point(118, 20);
            this.buttonDeleteAll.Name = "buttonDeleteAll";
            this.buttonDeleteAll.Size = new System.Drawing.Size(88, 23);
            this.buttonDeleteAll.TabIndex = 58;
            this.buttonDeleteAll.Text = "Delete All";
            this.buttonDeleteAll.UseVisualStyleBackColor = true;
            this.buttonDeleteAll.Click += new System.EventHandler(this.buttonDeleteAll_Click);
            // 
            // groupBoxEmployeeNoCfg
            // 
            this.groupBoxEmployeeNoCfg.Controls.Add(this.textBoxEmployeeNo2);
            this.groupBoxEmployeeNoCfg.Controls.Add(this.labelEmployeeNo2);
            this.groupBoxEmployeeNoCfg.Controls.Add(this.buttonUpdateEmployeeNo);
            this.groupBoxEmployeeNoCfg.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.groupBoxEmployeeNoCfg.Location = new System.Drawing.Point(22, 167);
            this.groupBoxEmployeeNoCfg.Name = "groupBoxEmployeeNoCfg";
            this.groupBoxEmployeeNoCfg.Size = new System.Drawing.Size(177, 87);
            this.groupBoxEmployeeNoCfg.TabIndex = 51;
            this.groupBoxEmployeeNoCfg.TabStop = false;
            this.groupBoxEmployeeNoCfg.Text = "EmployeeNo Cfg";
            // 
            // textBoxEmployeeNo2
            // 
            this.textBoxEmployeeNo2.Location = new System.Drawing.Point(103, 20);
            this.textBoxEmployeeNo2.Name = "textBoxEmployeeNo2";
            this.textBoxEmployeeNo2.Size = new System.Drawing.Size(59, 21);
            this.textBoxEmployeeNo2.TabIndex = 28;
            this.textBoxEmployeeNo2.Text = "1";
            // 
            // labelEmployeeNo2
            // 
            this.labelEmployeeNo2.AutoSize = true;
            this.labelEmployeeNo2.Location = new System.Drawing.Point(3, 23);
            this.labelEmployeeNo2.Name = "labelEmployeeNo2";
            this.labelEmployeeNo2.Size = new System.Drawing.Size(65, 12);
            this.labelEmployeeNo2.TabIndex = 27;
            this.labelEmployeeNo2.Text = "employeeNo";
            // 
            // buttonUpdateEmployeeNo
            // 
            this.buttonUpdateEmployeeNo.Location = new System.Drawing.Point(87, 47);
            this.buttonUpdateEmployeeNo.Name = "buttonUpdateEmployeeNo";
            this.buttonUpdateEmployeeNo.Size = new System.Drawing.Size(75, 23);
            this.buttonUpdateEmployeeNo.TabIndex = 20;
            this.buttonUpdateEmployeeNo.Text = "Update";
            this.buttonUpdateEmployeeNo.UseVisualStyleBackColor = true;
            this.buttonUpdateEmployeeNo.Click += new System.EventHandler(this.buttonUpdateEmployeeNo_Click);
            // 
            // buttonSearchByEmployeeNo
            // 
            this.buttonSearchByEmployeeNo.Location = new System.Drawing.Point(47, 260);
            this.buttonSearchByEmployeeNo.Name = "buttonSearchByEmployeeNo";
            this.buttonSearchByEmployeeNo.Size = new System.Drawing.Size(137, 23);
            this.buttonSearchByEmployeeNo.TabIndex = 57;
            this.buttonSearchByEmployeeNo.Text = "Search By EmployeeNo";
            this.buttonSearchByEmployeeNo.UseVisualStyleBackColor = true;
            this.buttonSearchByEmployeeNo.Click += new System.EventHandler(this.buttonSearchByEmployeeNo_Click);
            // 
            // listViewEmployeeNo
            // 
            this.listViewEmployeeNo.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader16,
            this.columnHeader17});
            this.listViewEmployeeNo.FullRowSelect = true;
            this.listViewEmployeeNo.GridLines = true;
            this.listViewEmployeeNo.HideSelection = false;
            this.listViewEmployeeNo.Location = new System.Drawing.Point(22, 49);
            this.listViewEmployeeNo.Name = "listViewEmployeeNo";
            this.listViewEmployeeNo.Size = new System.Drawing.Size(177, 111);
            this.listViewEmployeeNo.TabIndex = 56;
            this.listViewEmployeeNo.UseCompatibleStateImageBehavior = false;
            this.listViewEmployeeNo.View = System.Windows.Forms.View.Details;
            this.listViewEmployeeNo.ItemSelectionChanged += new System.Windows.Forms.ListViewItemSelectionChangedEventHandler(this.listViewEmployeeNo_ItemSelectionChanged);
            // 
            // columnHeader16
            // 
            this.columnHeader16.Text = "No";
            this.columnHeader16.Width = 50;
            // 
            // columnHeader17
            // 
            this.columnHeader17.Text = "employeeNo";
            this.columnHeader17.Width = 120;
            // 
            // buttonSearchAll
            // 
            this.buttonSearchAll.Location = new System.Drawing.Point(6, 20);
            this.buttonSearchAll.Name = "buttonSearchAll";
            this.buttonSearchAll.Size = new System.Drawing.Size(88, 23);
            this.buttonSearchAll.TabIndex = 56;
            this.buttonSearchAll.Text = "Search All";
            this.buttonSearchAll.UseVisualStyleBackColor = true;
            this.buttonSearchAll.Click += new System.EventHandler(this.buttonSearchAll_Click);
            // 
            // groupBoxCardInfoCount
            // 
            this.groupBoxCardInfoCount.Controls.Add(this.buttonGetByEmployeeNo);
            this.groupBoxCardInfoCount.Controls.Add(this.textBoxEmployeeNo3);
            this.groupBoxCardInfoCount.Controls.Add(this.labelEmployeeNo3);
            this.groupBoxCardInfoCount.Controls.Add(this.textBoxCardInfoCount);
            this.groupBoxCardInfoCount.Controls.Add(this.labelCardInfoCount);
            this.groupBoxCardInfoCount.Controls.Add(this.buttonGetAllCount);
            this.groupBoxCardInfoCount.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.groupBoxCardInfoCount.Location = new System.Drawing.Point(630, 287);
            this.groupBoxCardInfoCount.Name = "groupBoxCardInfoCount";
            this.groupBoxCardInfoCount.Size = new System.Drawing.Size(260, 103);
            this.groupBoxCardInfoCount.TabIndex = 59;
            this.groupBoxCardInfoCount.TabStop = false;
            this.groupBoxCardInfoCount.Text = "Card Info Count";
            // 
            // buttonGetByEmployeeNo
            // 
            this.buttonGetByEmployeeNo.Location = new System.Drawing.Point(106, 74);
            this.buttonGetByEmployeeNo.Name = "buttonGetByEmployeeNo";
            this.buttonGetByEmployeeNo.Size = new System.Drawing.Size(148, 23);
            this.buttonGetByEmployeeNo.TabIndex = 31;
            this.buttonGetByEmployeeNo.Text = "Get By EmployeeNo";
            this.buttonGetByEmployeeNo.UseVisualStyleBackColor = true;
            this.buttonGetByEmployeeNo.Click += new System.EventHandler(this.buttonGetByEmployeeNo_Click);
            // 
            // textBoxEmployeeNo3
            // 
            this.textBoxEmployeeNo3.Location = new System.Drawing.Point(106, 14);
            this.textBoxEmployeeNo3.Name = "textBoxEmployeeNo3";
            this.textBoxEmployeeNo3.Size = new System.Drawing.Size(59, 21);
            this.textBoxEmployeeNo3.TabIndex = 30;
            this.textBoxEmployeeNo3.Text = "1";
            // 
            // labelEmployeeNo3
            // 
            this.labelEmployeeNo3.AutoSize = true;
            this.labelEmployeeNo3.Location = new System.Drawing.Point(6, 17);
            this.labelEmployeeNo3.Name = "labelEmployeeNo3";
            this.labelEmployeeNo3.Size = new System.Drawing.Size(65, 12);
            this.labelEmployeeNo3.TabIndex = 29;
            this.labelEmployeeNo3.Text = "employeeNo";
            // 
            // textBoxCardInfoCount
            // 
            this.textBoxCardInfoCount.Location = new System.Drawing.Point(106, 46);
            this.textBoxCardInfoCount.Name = "textBoxCardInfoCount";
            this.textBoxCardInfoCount.Size = new System.Drawing.Size(59, 21);
            this.textBoxCardInfoCount.TabIndex = 28;
            this.textBoxCardInfoCount.Text = "0";
            // 
            // labelCardInfoCount
            // 
            this.labelCardInfoCount.AutoSize = true;
            this.labelCardInfoCount.Location = new System.Drawing.Point(6, 49);
            this.labelCardInfoCount.Name = "labelCardInfoCount";
            this.labelCardInfoCount.Size = new System.Drawing.Size(95, 12);
            this.labelCardInfoCount.TabIndex = 27;
            this.labelCardInfoCount.Text = "Card Info Count";
            // 
            // buttonGetAllCount
            // 
            this.buttonGetAllCount.Location = new System.Drawing.Point(8, 74);
            this.buttonGetAllCount.Name = "buttonGetAllCount";
            this.buttonGetAllCount.Size = new System.Drawing.Size(78, 23);
            this.buttonGetAllCount.TabIndex = 20;
            this.buttonGetAllCount.Text = "Get All";
            this.buttonGetAllCount.UseVisualStyleBackColor = true;
            this.buttonGetAllCount.Click += new System.EventHandler(this.buttonGetAllCount_Click);
            // 
            // buttonClearList
            // 
            this.buttonClearList.Location = new System.Drawing.Point(802, 251);
            this.buttonClearList.Name = "buttonClearList";
            this.buttonClearList.Size = new System.Drawing.Size(88, 23);
            this.buttonClearList.TabIndex = 60;
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
            this.listViewMessage.Location = new System.Drawing.Point(629, 396);
            this.listViewMessage.Name = "listViewMessage";
            this.listViewMessage.Size = new System.Drawing.Size(260, 190);
            this.listViewMessage.TabIndex = 61;
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
            // buttonSyn
            // 
            this.buttonSyn.Location = new System.Drawing.Point(638, 258);
            this.buttonSyn.Name = "buttonSyn";
            this.buttonSyn.Size = new System.Drawing.Size(109, 23);
            this.buttonSyn.TabIndex = 62;
            this.buttonSyn.Text = "syn to the lock";
            this.buttonSyn.UseVisualStyleBackColor = true;
            this.buttonSyn.Click += new System.EventHandler(this.buttonSyn_Click);
            // 
            // CardManagementByStaffCenter
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(901, 594);
            this.Controls.Add(this.buttonSyn);
            this.Controls.Add(this.listViewMessage);
            this.Controls.Add(this.groupBoxCardInfoCount);
            this.Controls.Add(this.buttonClearList);
            this.Controls.Add(this.groupBoxCardSearchOrDelete);
            this.Controls.Add(this.groupBoxCardAddOrModify);
            this.Controls.Add(this.listViewCardManage);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
            this.Name = "CardManagementByStaffCenter";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Card Management";
            this.Load += new System.EventHandler(this.CardManagementByStaffCenter_Load);
            this.groupBoxCardAddOrModify.ResumeLayout(false);
            this.groupBoxCardAddOrModify.PerformLayout();
            this.groupBoxCardSearchOrDelete.ResumeLayout(false);
            this.groupBoxCardNoCfg.ResumeLayout(false);
            this.groupBoxCardNoCfg.PerformLayout();
            this.groupBoxEmployeeNoCfg.ResumeLayout(false);
            this.groupBoxEmployeeNoCfg.PerformLayout();
            this.groupBoxCardInfoCount.ResumeLayout(false);
            this.groupBoxCardInfoCount.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.ListView listViewCardManage;
        private System.Windows.Forms.ColumnHeader columnHeader1;
        private System.Windows.Forms.ColumnHeader columnHeader2;
        private System.Windows.Forms.ColumnHeader columnHeader3;
        private System.Windows.Forms.ColumnHeader columnHeader4;
        private System.Windows.Forms.ColumnHeader columnHeader5;
        private System.Windows.Forms.GroupBox groupBoxCardAddOrModify;
        private System.Windows.Forms.Button buttonModify;
        private System.Windows.Forms.TextBox textBoxLeaderCard;
        private System.Windows.Forms.Label labelLeaderCard;
        private System.Windows.Forms.ComboBox comboBoxCardType;
        private System.Windows.Forms.Label labelCardType;
        private System.Windows.Forms.TextBox textBoxCardNo;
        private System.Windows.Forms.Label labelCardNo;
        private System.Windows.Forms.TextBox textBoxEmployeeNo;
        private System.Windows.Forms.Label labelEmployeeNo;
        private System.Windows.Forms.TextBox textBoxAddCardNumber;
        private System.Windows.Forms.Label labelAddCardNumber;
        private System.Windows.Forms.Button buttonSend;
        private System.Windows.Forms.Button buttonAdd;
        private System.Windows.Forms.GroupBox groupBoxCardSearchOrDelete;
        private System.Windows.Forms.Button buttonDeleteByEmployeeNo;
        private System.Windows.Forms.Button buttonDeleteAll;
        private System.Windows.Forms.GroupBox groupBoxEmployeeNoCfg;
        private System.Windows.Forms.TextBox textBoxEmployeeNo2;
        private System.Windows.Forms.Label labelEmployeeNo2;
        private System.Windows.Forms.Button buttonUpdateEmployeeNo;
        private System.Windows.Forms.Button buttonSearchByEmployeeNo;
        private System.Windows.Forms.ListView listViewEmployeeNo;
        private System.Windows.Forms.ColumnHeader columnHeader16;
        private System.Windows.Forms.ColumnHeader columnHeader17;
        private System.Windows.Forms.Button buttonSearchAll;
        private System.Windows.Forms.Button buttonDeleteByCardNo;
        private System.Windows.Forms.GroupBox groupBoxCardNoCfg;
        private System.Windows.Forms.TextBox textBoxCardNo2;
        private System.Windows.Forms.Label labelCardNo2;
        private System.Windows.Forms.Button buttonUpdateCardNo;
        private System.Windows.Forms.Button buttonSearchByCardNo;
        private System.Windows.Forms.ListView listViewCardNo;
        private System.Windows.Forms.ColumnHeader columnHeader6;
        private System.Windows.Forms.ColumnHeader columnHeader7;
        private System.Windows.Forms.GroupBox groupBoxCardInfoCount;
        private System.Windows.Forms.TextBox textBoxCardInfoCount;
        private System.Windows.Forms.Label labelCardInfoCount;
        private System.Windows.Forms.Button buttonGetAllCount;
        private System.Windows.Forms.Button buttonClearList;
        private System.Windows.Forms.TextBox textBoxEmployeeNo3;
        private System.Windows.Forms.Label labelEmployeeNo3;
        private System.Windows.Forms.Button buttonGetByEmployeeNo;
        private System.Windows.Forms.ListView listViewMessage;
        private System.Windows.Forms.ColumnHeader columnHeader21;
        private System.Windows.Forms.ColumnHeader columnHeader22;
        private System.Windows.Forms.Button buttonUpdateList;
        private System.Windows.Forms.Button buttonSyn;
    }
}