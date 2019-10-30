namespace AcsDemo.ProductAcs.CardManage
{
    partial class CardModify
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
            this.dateTimePickerEnd = new System.Windows.Forms.DateTimePicker();
            this.dateTimePickerStart = new System.Windows.Forms.DateTimePicker();
            this.textBoxCardModifyCardNo = new System.Windows.Forms.TextBox();
            this.labelCardAddCardNo = new System.Windows.Forms.Label();
            this.labelInterval = new System.Windows.Forms.Label();
            this.labelCardAddValid = new System.Windows.Forms.Label();
            this.textBoxCardModifyPassword = new System.Windows.Forms.TextBox();
            this.labelCardAddPassword = new System.Windows.Forms.Label();
            this.comboBoxCardModifyCardType = new System.Windows.Forms.ComboBox();
            this.labelCardAddCardType = new System.Windows.Forms.Label();
            this.buttonCancel = new System.Windows.Forms.Button();
            this.buttonConfirm = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // dateTimePickerEnd
            // 
            this.dateTimePickerEnd.CustomFormat = "MM/dd/yyyy HH:mm:ss";
            this.dateTimePickerEnd.Format = System.Windows.Forms.DateTimePickerFormat.Custom;
            this.dateTimePickerEnd.Location = new System.Drawing.Point(250, 80);
            this.dateTimePickerEnd.Name = "dateTimePickerEnd";
            this.dateTimePickerEnd.Size = new System.Drawing.Size(145, 21);
            this.dateTimePickerEnd.TabIndex = 55;
            // 
            // dateTimePickerStart
            // 
            this.dateTimePickerStart.CustomFormat = "MM/dd/yyyy HH:mm:ss";
            this.dateTimePickerStart.Format = System.Windows.Forms.DateTimePickerFormat.Custom;
            this.dateTimePickerStart.Location = new System.Drawing.Point(92, 80);
            this.dateTimePickerStart.Name = "dateTimePickerStart";
            this.dateTimePickerStart.Size = new System.Drawing.Size(135, 21);
            this.dateTimePickerStart.TabIndex = 56;
            // 
            // textBoxCardModifyCardNo
            // 
            this.textBoxCardModifyCardNo.Location = new System.Drawing.Point(92, 115);
            this.textBoxCardModifyCardNo.Name = "textBoxCardModifyCardNo";
            this.textBoxCardModifyCardNo.Size = new System.Drawing.Size(207, 21);
            this.textBoxCardModifyCardNo.TabIndex = 53;
            // 
            // labelCardAddCardNo
            // 
            this.labelCardAddCardNo.AutoSize = true;
            this.labelCardAddCardNo.ImeMode = System.Windows.Forms.ImeMode.NoControl;
            this.labelCardAddCardNo.Location = new System.Drawing.Point(0, 118);
            this.labelCardAddCardNo.Name = "labelCardAddCardNo";
            this.labelCardAddCardNo.Size = new System.Drawing.Size(47, 12);
            this.labelCardAddCardNo.TabIndex = 52;
            this.labelCardAddCardNo.Text = "Card No";
            // 
            // labelInterval
            // 
            this.labelInterval.AutoSize = true;
            this.labelInterval.ImeMode = System.Windows.Forms.ImeMode.NoControl;
            this.labelInterval.Location = new System.Drawing.Point(233, 86);
            this.labelInterval.Name = "labelInterval";
            this.labelInterval.Size = new System.Drawing.Size(17, 12);
            this.labelInterval.TabIndex = 50;
            this.labelInterval.Text = "To";
            // 
            // labelCardAddValid
            // 
            this.labelCardAddValid.AutoSize = true;
            this.labelCardAddValid.ImeMode = System.Windows.Forms.ImeMode.NoControl;
            this.labelCardAddValid.Location = new System.Drawing.Point(0, 86);
            this.labelCardAddValid.Name = "labelCardAddValid";
            this.labelCardAddValid.Size = new System.Drawing.Size(95, 12);
            this.labelCardAddValid.TabIndex = 51;
            this.labelCardAddValid.Text = "Validity Period";
            // 
            // textBoxCardModifyPassword
            // 
            this.textBoxCardModifyPassword.Location = new System.Drawing.Point(92, 43);
            this.textBoxCardModifyPassword.Name = "textBoxCardModifyPassword";
            this.textBoxCardModifyPassword.Size = new System.Drawing.Size(303, 21);
            this.textBoxCardModifyPassword.TabIndex = 49;
            // 
            // labelCardAddPassword
            // 
            this.labelCardAddPassword.AutoSize = true;
            this.labelCardAddPassword.ImeMode = System.Windows.Forms.ImeMode.NoControl;
            this.labelCardAddPassword.Location = new System.Drawing.Point(0, 52);
            this.labelCardAddPassword.Name = "labelCardAddPassword";
            this.labelCardAddPassword.Size = new System.Drawing.Size(53, 12);
            this.labelCardAddPassword.TabIndex = 48;
            this.labelCardAddPassword.Text = "Password";
            // 
            // comboBoxCardModifyCardType
            // 
            this.comboBoxCardModifyCardType.FormattingEnabled = true;
            this.comboBoxCardModifyCardType.Items.AddRange(new object[] {
            "Normal Card",
            "Disabled Card",
            "Black List Card",
            "Night Watch Card",
            "Stress Card",
            "Super Card",
            "Guest Card",
            "Release Card"});
            this.comboBoxCardModifyCardType.Location = new System.Drawing.Point(92, 12);
            this.comboBoxCardModifyCardType.Name = "comboBoxCardModifyCardType";
            this.comboBoxCardModifyCardType.Size = new System.Drawing.Size(303, 20);
            this.comboBoxCardModifyCardType.TabIndex = 46;
            // 
            // labelCardAddCardType
            // 
            this.labelCardAddCardType.AutoSize = true;
            this.labelCardAddCardType.ImeMode = System.Windows.Forms.ImeMode.NoControl;
            this.labelCardAddCardType.Location = new System.Drawing.Point(0, 20);
            this.labelCardAddCardType.Name = "labelCardAddCardType";
            this.labelCardAddCardType.Size = new System.Drawing.Size(59, 12);
            this.labelCardAddCardType.TabIndex = 47;
            this.labelCardAddCardType.Text = "Card Type";
            // 
            // buttonCancel
            // 
            this.buttonCancel.Location = new System.Drawing.Point(250, 161);
            this.buttonCancel.Name = "buttonCancel";
            this.buttonCancel.Size = new System.Drawing.Size(101, 22);
            this.buttonCancel.TabIndex = 57;
            this.buttonCancel.Text = "Cancel";
            this.buttonCancel.UseVisualStyleBackColor = true;
            this.buttonCancel.Click += new System.EventHandler(this.buttonCancel_Click);
            // 
            // buttonConfirm
            // 
            this.buttonConfirm.Location = new System.Drawing.Point(121, 161);
            this.buttonConfirm.Name = "buttonConfirm";
            this.buttonConfirm.Size = new System.Drawing.Size(101, 22);
            this.buttonConfirm.TabIndex = 58;
            this.buttonConfirm.Text = "Confirm";
            this.buttonConfirm.UseVisualStyleBackColor = true;
            this.buttonConfirm.Click += new System.EventHandler(this.buttonConfirm_Click);
            // 
            // CardModify
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(403, 195);
            this.Controls.Add(this.buttonCancel);
            this.Controls.Add(this.buttonConfirm);
            this.Controls.Add(this.dateTimePickerEnd);
            this.Controls.Add(this.dateTimePickerStart);
            this.Controls.Add(this.textBoxCardModifyCardNo);
            this.Controls.Add(this.labelCardAddCardNo);
            this.Controls.Add(this.labelInterval);
            this.Controls.Add(this.labelCardAddValid);
            this.Controls.Add(this.textBoxCardModifyPassword);
            this.Controls.Add(this.labelCardAddPassword);
            this.Controls.Add(this.comboBoxCardModifyCardType);
            this.Controls.Add(this.labelCardAddCardType);
            this.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.Name = "CardModify";
            this.Text = "CardModify";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.DateTimePicker dateTimePickerEnd;
        private System.Windows.Forms.DateTimePicker dateTimePickerStart;
        private System.Windows.Forms.TextBox textBoxCardModifyCardNo;
        private System.Windows.Forms.Label labelCardAddCardNo;
        private System.Windows.Forms.Label labelInterval;
        private System.Windows.Forms.Label labelCardAddValid;
        private System.Windows.Forms.TextBox textBoxCardModifyPassword;
        private System.Windows.Forms.Label labelCardAddPassword;
        private System.Windows.Forms.ComboBox comboBoxCardModifyCardType;
        private System.Windows.Forms.Label labelCardAddCardType;
        private System.Windows.Forms.Button buttonCancel;
        private System.Windows.Forms.Button buttonConfirm;
    }
}