namespace AcsDemo.ProductAcs.CardManage
{
    partial class CardAdd
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
            this.comboBoxCardAddCardType = new System.Windows.Forms.ComboBox();
            this.labelCardAddCardType = new System.Windows.Forms.Label();
            this.textBoxCardAddPassword = new System.Windows.Forms.TextBox();
            this.labelCardAddPassword = new System.Windows.Forms.Label();
            this.labelCardAddValid = new System.Windows.Forms.Label();
            this.textBoxCardAddCardNo = new System.Windows.Forms.TextBox();
            this.labelCardAddCardNo = new System.Windows.Forms.Label();
            this.buttonCardAddAdd = new System.Windows.Forms.Button();
            this.listViewUserAddCard = new System.Windows.Forms.ListView();
            this.columnHeader1 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader2 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader3 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader4 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.dateTimePickerStart = new System.Windows.Forms.DateTimePicker();
            this.dateTimePickerEnd = new System.Windows.Forms.DateTimePicker();
            this.labelInterval = new System.Windows.Forms.Label();
            this.buttonConfirm = new System.Windows.Forms.Button();
            this.buttonCancel = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // comboBoxCardAddCardType
            // 
            this.comboBoxCardAddCardType.FormattingEnabled = true;
            this.comboBoxCardAddCardType.Items.AddRange(new object[] {
            "invalid",
            "Normal Card",
            "Disabled Card",
            "Black List Card",
            "Night Watch Card",
            "Stress Card",
            "Super Card",
            "Guest Card",
            "Release Card"});
            this.comboBoxCardAddCardType.Location = new System.Drawing.Point(104, 11);
            this.comboBoxCardAddCardType.Name = "comboBoxCardAddCardType";
            this.comboBoxCardAddCardType.Size = new System.Drawing.Size(303, 20);
            this.comboBoxCardAddCardType.TabIndex = 22;
            this.comboBoxCardAddCardType.Text = "Normal Card";
            // 
            // labelCardAddCardType
            // 
            this.labelCardAddCardType.AutoSize = true;
            this.labelCardAddCardType.ImeMode = System.Windows.Forms.ImeMode.NoControl;
            this.labelCardAddCardType.Location = new System.Drawing.Point(12, 19);
            this.labelCardAddCardType.Name = "labelCardAddCardType";
            this.labelCardAddCardType.Size = new System.Drawing.Size(59, 12);
            this.labelCardAddCardType.TabIndex = 23;
            this.labelCardAddCardType.Text = "Card Type";
            // 
            // textBoxCardAddPassword
            // 
            this.textBoxCardAddPassword.Location = new System.Drawing.Point(104, 42);
            this.textBoxCardAddPassword.Name = "textBoxCardAddPassword";
            this.textBoxCardAddPassword.Size = new System.Drawing.Size(303, 21);
            this.textBoxCardAddPassword.TabIndex = 39;
            this.textBoxCardAddPassword.UseSystemPasswordChar = true;
            // 
            // labelCardAddPassword
            // 
            this.labelCardAddPassword.AutoSize = true;
            this.labelCardAddPassword.ImeMode = System.Windows.Forms.ImeMode.NoControl;
            this.labelCardAddPassword.Location = new System.Drawing.Point(12, 51);
            this.labelCardAddPassword.Name = "labelCardAddPassword";
            this.labelCardAddPassword.Size = new System.Drawing.Size(53, 12);
            this.labelCardAddPassword.TabIndex = 38;
            this.labelCardAddPassword.Text = "Password";
            // 
            // labelCardAddValid
            // 
            this.labelCardAddValid.AutoSize = true;
            this.labelCardAddValid.ImeMode = System.Windows.Forms.ImeMode.NoControl;
            this.labelCardAddValid.Location = new System.Drawing.Point(12, 85);
            this.labelCardAddValid.Name = "labelCardAddValid";
            this.labelCardAddValid.Size = new System.Drawing.Size(95, 12);
            this.labelCardAddValid.TabIndex = 40;
            this.labelCardAddValid.Text = "Validity Period";
            // 
            // textBoxCardAddCardNo
            // 
            this.textBoxCardAddCardNo.Location = new System.Drawing.Point(104, 114);
            this.textBoxCardAddCardNo.Name = "textBoxCardAddCardNo";
            this.textBoxCardAddCardNo.Size = new System.Drawing.Size(170, 21);
            this.textBoxCardAddCardNo.TabIndex = 42;
            this.textBoxCardAddCardNo.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.textBoxCardAddCardNo_KeyPress);
            // 
            // labelCardAddCardNo
            // 
            this.labelCardAddCardNo.AutoSize = true;
            this.labelCardAddCardNo.ImeMode = System.Windows.Forms.ImeMode.NoControl;
            this.labelCardAddCardNo.Location = new System.Drawing.Point(12, 117);
            this.labelCardAddCardNo.Name = "labelCardAddCardNo";
            this.labelCardAddCardNo.Size = new System.Drawing.Size(47, 12);
            this.labelCardAddCardNo.TabIndex = 41;
            this.labelCardAddCardNo.Text = "Card No";
            // 
            // buttonCardAddAdd
            // 
            this.buttonCardAddAdd.ImeMode = System.Windows.Forms.ImeMode.NoControl;
            this.buttonCardAddAdd.Location = new System.Drawing.Point(306, 114);
            this.buttonCardAddAdd.Name = "buttonCardAddAdd";
            this.buttonCardAddAdd.Size = new System.Drawing.Size(101, 22);
            this.buttonCardAddAdd.TabIndex = 43;
            this.buttonCardAddAdd.Text = "Add";
            this.buttonCardAddAdd.UseVisualStyleBackColor = true;
            this.buttonCardAddAdd.Click += new System.EventHandler(this.buttonCardAddAdd_Click);
            // 
            // listViewUserAddCard
            // 
            this.listViewUserAddCard.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader1,
            this.columnHeader2,
            this.columnHeader3,
            this.columnHeader4});
            this.listViewUserAddCard.FullRowSelect = true;
            this.listViewUserAddCard.GridLines = true;
            this.listViewUserAddCard.Location = new System.Drawing.Point(0, 149);
            this.listViewUserAddCard.Margin = new System.Windows.Forms.Padding(0);
            this.listViewUserAddCard.Name = "listViewUserAddCard";
            this.listViewUserAddCard.Size = new System.Drawing.Size(420, 178);
            this.listViewUserAddCard.TabIndex = 44;
            this.listViewUserAddCard.UseCompatibleStateImageBehavior = false;
            this.listViewUserAddCard.View = System.Windows.Forms.View.Details;
            // 
            // columnHeader1
            // 
            this.columnHeader1.Text = "Index";
            this.columnHeader1.Width = 80;
            // 
            // columnHeader2
            // 
            this.columnHeader2.Text = "Card No";
            this.columnHeader2.Width = 100;
            // 
            // columnHeader3
            // 
            this.columnHeader3.Text = "Card Type";
            this.columnHeader3.Width = 100;
            // 
            // columnHeader4
            // 
            this.columnHeader4.Text = "Card Validity";
            this.columnHeader4.Width = 135;
            // 
            // dateTimePickerStart
            // 
            this.dateTimePickerStart.CustomFormat = "MM/dd/yyyy HH:mm:ss";
            this.dateTimePickerStart.Format = System.Windows.Forms.DateTimePickerFormat.Custom;
            this.dateTimePickerStart.Location = new System.Drawing.Point(105, 79);
            this.dateTimePickerStart.Name = "dateTimePickerStart";
            this.dateTimePickerStart.Size = new System.Drawing.Size(144, 21);
            this.dateTimePickerStart.TabIndex = 45;
            this.dateTimePickerStart.Value = new System.DateTime(2017, 3, 1, 0, 0, 0, 0);
            // 
            // dateTimePickerEnd
            // 
            this.dateTimePickerEnd.CustomFormat = "MM/dd/yyyy HH:mm:ss";
            this.dateTimePickerEnd.Format = System.Windows.Forms.DateTimePickerFormat.Custom;
            this.dateTimePickerEnd.Location = new System.Drawing.Point(275, 79);
            this.dateTimePickerEnd.Name = "dateTimePickerEnd";
            this.dateTimePickerEnd.Size = new System.Drawing.Size(134, 21);
            this.dateTimePickerEnd.TabIndex = 45;
            this.dateTimePickerEnd.Value = new System.DateTime(2017, 3, 1, 0, 0, 0, 0);
            // 
            // labelInterval
            // 
            this.labelInterval.AutoSize = true;
            this.labelInterval.ImeMode = System.Windows.Forms.ImeMode.NoControl;
            this.labelInterval.Location = new System.Drawing.Point(255, 85);
            this.labelInterval.Name = "labelInterval";
            this.labelInterval.Size = new System.Drawing.Size(17, 12);
            this.labelInterval.TabIndex = 40;
            this.labelInterval.Text = "To";
            // 
            // buttonConfirm
            // 
            this.buttonConfirm.Location = new System.Drawing.Point(177, 343);
            this.buttonConfirm.Name = "buttonConfirm";
            this.buttonConfirm.Size = new System.Drawing.Size(101, 22);
            this.buttonConfirm.TabIndex = 46;
            this.buttonConfirm.Text = "Confirm";
            this.buttonConfirm.UseVisualStyleBackColor = true;
            this.buttonConfirm.Click += new System.EventHandler(this.buttonConfirm_Click);
            // 
            // buttonCancel
            // 
            this.buttonCancel.Location = new System.Drawing.Point(306, 343);
            this.buttonCancel.Name = "buttonCancel";
            this.buttonCancel.Size = new System.Drawing.Size(101, 22);
            this.buttonCancel.TabIndex = 46;
            this.buttonCancel.Text = "Cancel";
            this.buttonCancel.UseVisualStyleBackColor = true;
            this.buttonCancel.Click += new System.EventHandler(this.buttonCancel_Click);
            // 
            // CardAdd
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(419, 375);
            this.Controls.Add(this.buttonCancel);
            this.Controls.Add(this.buttonConfirm);
            this.Controls.Add(this.dateTimePickerEnd);
            this.Controls.Add(this.dateTimePickerStart);
            this.Controls.Add(this.listViewUserAddCard);
            this.Controls.Add(this.buttonCardAddAdd);
            this.Controls.Add(this.textBoxCardAddCardNo);
            this.Controls.Add(this.labelCardAddCardNo);
            this.Controls.Add(this.labelInterval);
            this.Controls.Add(this.labelCardAddValid);
            this.Controls.Add(this.textBoxCardAddPassword);
            this.Controls.Add(this.labelCardAddPassword);
            this.Controls.Add(this.comboBoxCardAddCardType);
            this.Controls.Add(this.labelCardAddCardType);
            this.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
            this.Name = "CardAdd";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Card Add";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.ComboBox comboBoxCardAddCardType;
        private System.Windows.Forms.Label labelCardAddCardType;
        private System.Windows.Forms.TextBox textBoxCardAddPassword;
        private System.Windows.Forms.Label labelCardAddPassword;
        private System.Windows.Forms.Label labelCardAddValid;
        private System.Windows.Forms.TextBox textBoxCardAddCardNo;
        private System.Windows.Forms.Label labelCardAddCardNo;
        private System.Windows.Forms.Button buttonCardAddAdd;
        private System.Windows.Forms.ListView listViewUserAddCard;
        private System.Windows.Forms.ColumnHeader columnHeader1;
        private System.Windows.Forms.ColumnHeader columnHeader2;
        private System.Windows.Forms.ColumnHeader columnHeader3;
        private System.Windows.Forms.ColumnHeader columnHeader4;
        private System.Windows.Forms.DateTimePicker dateTimePickerStart;
        private System.Windows.Forms.DateTimePicker dateTimePickerEnd;
        private System.Windows.Forms.Label labelInterval;
        private System.Windows.Forms.Button buttonConfirm;
        private System.Windows.Forms.Button buttonCancel;
    }
}