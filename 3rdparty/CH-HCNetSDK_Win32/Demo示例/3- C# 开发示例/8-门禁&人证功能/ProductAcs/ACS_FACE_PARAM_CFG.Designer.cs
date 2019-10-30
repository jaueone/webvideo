namespace AcsDemo.ProductAcs
{
    partial class ACS_FACE_PARAM_CFG
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
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.listViewFaceConfig = new System.Windows.Forms.ListView();
            this.columnHeader1 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader2 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader3 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader4 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader5 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader6 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.comboBoxDataType = new System.Windows.Forms.ComboBox();
            this.label6 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.btnGet = new System.Windows.Forms.Button();
            this.btnSet = new System.Windows.Forms.Button();
            this.btnClear = new System.Windows.Forms.Button();
            this.btnAdd = new System.Windows.Forms.Button();
            this.textBoxNo = new System.Windows.Forms.TextBox();
            this.textBoxFaceID = new System.Windows.Forms.TextBox();
            this.label4 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.btnBrowse = new System.Windows.Forms.Button();
            this.textBoxFacePath = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.textBoxAssociatedCardNo = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.checkedListBoxCardReader = new System.Windows.Forms.CheckedListBox();
            this.openFileFacePath = new System.Windows.Forms.OpenFileDialog();
            this.label7 = new System.Windows.Forms.Label();
            this.comboBoxDeleteMeth = new System.Windows.Forms.ComboBox();
            this.groupBoxCardNumber = new System.Windows.Forms.GroupBox();
            this.checkedListBoxDelByCardNo = new System.Windows.Forms.CheckedListBox();
            this.btnDel = new System.Windows.Forms.Button();
            this.groupBox1.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.groupBoxCardNumber.SuspendLayout();
            this.SuspendLayout();
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.listViewFaceConfig);
            this.groupBox1.Location = new System.Drawing.Point(12, 12);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(542, 249);
            this.groupBox1.TabIndex = 0;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Human Face function";
            // 
            // listViewFaceConfig
            // 
            this.listViewFaceConfig.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader1,
            this.columnHeader2,
            this.columnHeader3,
            this.columnHeader4,
            this.columnHeader5,
            this.columnHeader6});
            this.listViewFaceConfig.GridLines = true;
            this.listViewFaceConfig.Location = new System.Drawing.Point(6, 20);
            this.listViewFaceConfig.Name = "listViewFaceConfig";
            this.listViewFaceConfig.Size = new System.Drawing.Size(530, 223);
            this.listViewFaceConfig.TabIndex = 0;
            this.listViewFaceConfig.UseCompatibleStateImageBehavior = false;
            this.listViewFaceConfig.View = System.Windows.Forms.View.Details;
            // 
            // columnHeader1
            // 
            this.columnHeader1.Text = "No";
            // 
            // columnHeader2
            // 
            this.columnHeader2.Text = "Card No";
            // 
            // columnHeader3
            // 
            this.columnHeader3.Text = "Face Data Path";
            this.columnHeader3.Width = 100;
            // 
            // columnHeader4
            // 
            this.columnHeader4.Text = "Face No";
            // 
            // columnHeader5
            // 
            this.columnHeader5.Text = "Face Data TYPE";
            this.columnHeader5.Width = 100;
            // 
            // columnHeader6
            // 
            this.columnHeader6.Text = "Module failed error code";
            this.columnHeader6.Width = 134;
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.comboBoxDataType);
            this.groupBox2.Controls.Add(this.label6);
            this.groupBox2.Controls.Add(this.label5);
            this.groupBox2.Controls.Add(this.btnGet);
            this.groupBox2.Controls.Add(this.btnSet);
            this.groupBox2.Controls.Add(this.btnClear);
            this.groupBox2.Controls.Add(this.btnAdd);
            this.groupBox2.Controls.Add(this.textBoxNo);
            this.groupBox2.Controls.Add(this.textBoxFaceID);
            this.groupBox2.Controls.Add(this.label4);
            this.groupBox2.Controls.Add(this.label3);
            this.groupBox2.Controls.Add(this.btnBrowse);
            this.groupBox2.Controls.Add(this.textBoxFacePath);
            this.groupBox2.Controls.Add(this.label2);
            this.groupBox2.Controls.Add(this.textBoxAssociatedCardNo);
            this.groupBox2.Controls.Add(this.label1);
            this.groupBox2.Controls.Add(this.checkedListBoxCardReader);
            this.groupBox2.Location = new System.Drawing.Point(13, 285);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(541, 261);
            this.groupBox2.TabIndex = 1;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Face Configration";
            // 
            // comboBoxDataType
            // 
            this.comboBoxDataType.FormattingEnabled = true;
            this.comboBoxDataType.Items.AddRange(new object[] {
            "template",
            "picture"});
            this.comboBoxDataType.Location = new System.Drawing.Point(402, 74);
            this.comboBoxDataType.Name = "comboBoxDataType";
            this.comboBoxDataType.Size = new System.Drawing.Size(100, 20);
            this.comboBoxDataType.TabIndex = 16;
            this.comboBoxDataType.Text = "picture";
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(278, 96);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(155, 12);
            this.label6.TabIndex = 15;
            this.label6.Text = "(Face picture limit 200k)";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(278, 76);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(77, 12);
            this.label5.TabIndex = 14;
            this.label5.Text = "FaceDataType";
            // 
            // btnGet
            // 
            this.btnGet.Location = new System.Drawing.Point(278, 150);
            this.btnGet.Name = "btnGet";
            this.btnGet.Size = new System.Drawing.Size(75, 23);
            this.btnGet.TabIndex = 13;
            this.btnGet.Text = "Get";
            this.btnGet.UseVisualStyleBackColor = true;
            this.btnGet.Click += new System.EventHandler(this.btnGet_Click);
            // 
            // btnSet
            // 
            this.btnSet.Location = new System.Drawing.Point(156, 150);
            this.btnSet.Name = "btnSet";
            this.btnSet.Size = new System.Drawing.Size(75, 23);
            this.btnSet.TabIndex = 12;
            this.btnSet.Text = "Set";
            this.btnSet.UseVisualStyleBackColor = true;
            this.btnSet.Click += new System.EventHandler(this.btnSet_Click);
            // 
            // btnClear
            // 
            this.btnClear.Location = new System.Drawing.Point(278, 121);
            this.btnClear.Name = "btnClear";
            this.btnClear.Size = new System.Drawing.Size(75, 23);
            this.btnClear.TabIndex = 11;
            this.btnClear.Text = "clear";
            this.btnClear.UseVisualStyleBackColor = true;
            this.btnClear.Click += new System.EventHandler(this.btnClear_Click);
            // 
            // btnAdd
            // 
            this.btnAdd.Location = new System.Drawing.Point(156, 121);
            this.btnAdd.Name = "btnAdd";
            this.btnAdd.Size = new System.Drawing.Size(75, 23);
            this.btnAdd.TabIndex = 10;
            this.btnAdd.Text = "add";
            this.btnAdd.UseVisualStyleBackColor = true;
            this.btnAdd.Click += new System.EventHandler(this.btnAdd_Click);
            // 
            // textBoxNo
            // 
            this.textBoxNo.Location = new System.Drawing.Point(402, 46);
            this.textBoxNo.Name = "textBoxNo";
            this.textBoxNo.Size = new System.Drawing.Size(100, 21);
            this.textBoxNo.TabIndex = 9;
            this.textBoxNo.Text = "1";
            // 
            // textBoxFaceID
            // 
            this.textBoxFaceID.Location = new System.Drawing.Point(402, 18);
            this.textBoxFaceID.Name = "textBoxFaceID";
            this.textBoxFaceID.Size = new System.Drawing.Size(100, 21);
            this.textBoxFaceID.TabIndex = 8;
            this.textBoxFaceID.Text = "1";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(276, 49);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(47, 12);
            this.label4.TabIndex = 7;
            this.label4.Text = "number:";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(276, 21);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(119, 12);
            this.label3.TabIndex = 6;
            this.label3.Text = "Face Serial number:";
            // 
            // btnBrowse
            // 
            this.btnBrowse.Location = new System.Drawing.Point(156, 76);
            this.btnBrowse.Name = "btnBrowse";
            this.btnBrowse.Size = new System.Drawing.Size(75, 23);
            this.btnBrowse.TabIndex = 5;
            this.btnBrowse.Text = "Browse";
            this.btnBrowse.UseVisualStyleBackColor = true;
            this.btnBrowse.Click += new System.EventHandler(this.button1_Click);
            // 
            // textBoxFacePath
            // 
            this.textBoxFacePath.Location = new System.Drawing.Point(156, 49);
            this.textBoxFacePath.Name = "textBoxFacePath";
            this.textBoxFacePath.Size = new System.Drawing.Size(100, 21);
            this.textBoxFacePath.TabIndex = 4;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(9, 49);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(89, 12);
            this.label2.TabIndex = 3;
            this.label2.Text = "Face data path";
            // 
            // textBoxAssociatedCardNo
            // 
            this.textBoxAssociatedCardNo.Location = new System.Drawing.Point(156, 18);
            this.textBoxAssociatedCardNo.Name = "textBoxAssociatedCardNo";
            this.textBoxAssociatedCardNo.Size = new System.Drawing.Size(100, 21);
            this.textBoxAssociatedCardNo.TabIndex = 2;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(7, 21);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(143, 12);
            this.label1.TabIndex = 1;
            this.label1.Text = "Assocoiated card number";
            // 
            // checkedListBoxCardReader
            // 
            this.checkedListBoxCardReader.CheckOnClick = true;
            this.checkedListBoxCardReader.FormattingEnabled = true;
            this.checkedListBoxCardReader.Items.AddRange(new object[] {
            "CardReader1",
            "CardReader2",
            "CardReader3",
            "CardReader4",
            "CardReader5",
            "CardReader6",
            "CardReader7",
            "CardReader8",
            "CardReader9",
            "CardReader10"});
            this.checkedListBoxCardReader.Location = new System.Drawing.Point(7, 85);
            this.checkedListBoxCardReader.Name = "checkedListBoxCardReader";
            this.checkedListBoxCardReader.Size = new System.Drawing.Size(136, 164);
            this.checkedListBoxCardReader.TabIndex = 0;
            // 
            // openFileFacePath
            // 
            this.openFileFacePath.FileName = "openFiledialog1";
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(560, 15);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(83, 12);
            this.label7.TabIndex = 2;
            this.label7.Text = "Delete Method";
            this.label7.Click += new System.EventHandler(this.label7_Click);
            // 
            // comboBoxDeleteMeth
            // 
            this.comboBoxDeleteMeth.FormattingEnabled = true;
            this.comboBoxDeleteMeth.Items.AddRange(new object[] {
            "Del by CardNo"});
            this.comboBoxDeleteMeth.Location = new System.Drawing.Point(649, 12);
            this.comboBoxDeleteMeth.Name = "comboBoxDeleteMeth";
            this.comboBoxDeleteMeth.Size = new System.Drawing.Size(121, 20);
            this.comboBoxDeleteMeth.TabIndex = 3;
            this.comboBoxDeleteMeth.Text = "Del by CardNo";
            // 
            // groupBoxCardNumber
            // 
            this.groupBoxCardNumber.Controls.Add(this.checkedListBoxDelByCardNo);
            this.groupBoxCardNumber.Location = new System.Drawing.Point(562, 70);
            this.groupBoxCardNumber.Name = "groupBoxCardNumber";
            this.groupBoxCardNumber.Size = new System.Drawing.Size(200, 127);
            this.groupBoxCardNumber.TabIndex = 4;
            this.groupBoxCardNumber.TabStop = false;
            this.groupBoxCardNumber.Text = "Delete Depend ON Card Number";
            // 
            // checkedListBoxDelByCardNo
            // 
            this.checkedListBoxDelByCardNo.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.checkedListBoxDelByCardNo.CheckOnClick = true;
            this.checkedListBoxDelByCardNo.FormattingEnabled = true;
            this.checkedListBoxDelByCardNo.Items.AddRange(new object[] {
            "ID1",
            "ID2"});
            this.checkedListBoxDelByCardNo.Location = new System.Drawing.Point(0, 15);
            this.checkedListBoxDelByCardNo.Name = "checkedListBoxDelByCardNo";
            this.checkedListBoxDelByCardNo.Size = new System.Drawing.Size(200, 128);
            this.checkedListBoxDelByCardNo.TabIndex = 0;
            // 
            // btnDel
            // 
            this.btnDel.Location = new System.Drawing.Point(623, 232);
            this.btnDel.Name = "btnDel";
            this.btnDel.Size = new System.Drawing.Size(75, 23);
            this.btnDel.TabIndex = 5;
            this.btnDel.Text = "Delete";
            this.btnDel.UseVisualStyleBackColor = true;
            this.btnDel.Click += new System.EventHandler(this.button1_Click_1);
            // 
            // ACS_FACE_PARAM_CFG
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(799, 558);
            this.Controls.Add(this.btnDel);
            this.Controls.Add(this.groupBoxCardNumber);
            this.Controls.Add(this.comboBoxDeleteMeth);
            this.Controls.Add(this.label7);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.groupBox1);
            this.Name = "ACS_FACE_PARAM_CFG";
            this.Text = "ACS_FACE_PARAM_CFG";
            this.groupBox1.ResumeLayout(false);
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.groupBoxCardNumber.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.ListView listViewFaceConfig;
        private System.Windows.Forms.ColumnHeader columnHeader1;
        private System.Windows.Forms.ColumnHeader columnHeader2;
        private System.Windows.Forms.ColumnHeader columnHeader3;
        private System.Windows.Forms.ColumnHeader columnHeader4;
        private System.Windows.Forms.ColumnHeader columnHeader5;
        private System.Windows.Forms.ColumnHeader columnHeader6;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.CheckedListBox checkedListBoxCardReader;
        private System.Windows.Forms.Button btnGet;
        private System.Windows.Forms.Button btnSet;
        private System.Windows.Forms.Button btnClear;
        private System.Windows.Forms.Button btnAdd;
        private System.Windows.Forms.TextBox textBoxNo;
        private System.Windows.Forms.TextBox textBoxFaceID;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Button btnBrowse;
        private System.Windows.Forms.TextBox textBoxFacePath;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox textBoxAssociatedCardNo;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.ComboBox comboBoxDataType;
        private System.Windows.Forms.OpenFileDialog openFileFacePath;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.ComboBox comboBoxDeleteMeth;
        private System.Windows.Forms.GroupBox groupBoxCardNumber;
        private System.Windows.Forms.CheckedListBox checkedListBoxDelByCardNo;
        private System.Windows.Forms.Button btnDel;
    }
}