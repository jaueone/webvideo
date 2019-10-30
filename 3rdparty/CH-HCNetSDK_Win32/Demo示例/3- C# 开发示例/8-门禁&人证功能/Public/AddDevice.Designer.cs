namespace AcsDemo.Public
{
    partial class AddDevice
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(AddDevice));
            this.textBoxPassword = new System.Windows.Forms.TextBox();
            this.labelPassword = new System.Windows.Forms.Label();
            this.textBoxUserName = new System.Windows.Forms.TextBox();
            this.labelUserName = new System.Windows.Forms.Label();
            this.textBoxPort = new System.Windows.Forms.TextBox();
            this.labelPort = new System.Windows.Forms.Label();
            this.textBoxDeviceAddress = new System.Windows.Forms.TextBox();
            this.labelDeviceAddress = new System.Windows.Forms.Label();
            this.textBoxLocalNode = new System.Windows.Forms.TextBox();
            this.labelLocalNode = new System.Windows.Forms.Label();
            this.buttonCancel = new System.Windows.Forms.Button();
            this.buttonAdd = new System.Windows.Forms.Button();
            this.AysnLogincheckBox = new System.Windows.Forms.CheckBox();
            this.SuspendLayout();
            // 
            // textBoxPassword
            // 
            this.textBoxPassword.Cursor = System.Windows.Forms.Cursors.IBeam;
            resources.ApplyResources(this.textBoxPassword, "textBoxPassword");
            this.textBoxPassword.Name = "textBoxPassword";
            this.textBoxPassword.UseSystemPasswordChar = true;
            // 
            // labelPassword
            // 
            resources.ApplyResources(this.labelPassword, "labelPassword");
            this.labelPassword.Name = "labelPassword";
            this.labelPassword.UseWaitCursor = true;
            // 
            // textBoxUserName
            // 
            this.textBoxUserName.Cursor = System.Windows.Forms.Cursors.IBeam;
            resources.ApplyResources(this.textBoxUserName, "textBoxUserName");
            this.textBoxUserName.Name = "textBoxUserName";
            // 
            // labelUserName
            // 
            resources.ApplyResources(this.labelUserName, "labelUserName");
            this.labelUserName.Name = "labelUserName";
            this.labelUserName.UseWaitCursor = true;
            // 
            // textBoxPort
            // 
            this.textBoxPort.Cursor = System.Windows.Forms.Cursors.IBeam;
            resources.ApplyResources(this.textBoxPort, "textBoxPort");
            this.textBoxPort.Name = "textBoxPort";
            // 
            // labelPort
            // 
            resources.ApplyResources(this.labelPort, "labelPort");
            this.labelPort.Name = "labelPort";
            this.labelPort.UseWaitCursor = true;
            // 
            // textBoxDeviceAddress
            // 
            this.textBoxDeviceAddress.Cursor = System.Windows.Forms.Cursors.IBeam;
            resources.ApplyResources(this.textBoxDeviceAddress, "textBoxDeviceAddress");
            this.textBoxDeviceAddress.Name = "textBoxDeviceAddress";
            // 
            // labelDeviceAddress
            // 
            resources.ApplyResources(this.labelDeviceAddress, "labelDeviceAddress");
            this.labelDeviceAddress.Name = "labelDeviceAddress";
            this.labelDeviceAddress.UseWaitCursor = true;
            // 
            // textBoxLocalNode
            // 
            this.textBoxLocalNode.Cursor = System.Windows.Forms.Cursors.IBeam;
            resources.ApplyResources(this.textBoxLocalNode, "textBoxLocalNode");
            this.textBoxLocalNode.Name = "textBoxLocalNode";
            // 
            // labelLocalNode
            // 
            resources.ApplyResources(this.labelLocalNode, "labelLocalNode");
            this.labelLocalNode.Name = "labelLocalNode";
            this.labelLocalNode.UseWaitCursor = true;
            // 
            // buttonCancel
            // 
            this.buttonCancel.Cursor = System.Windows.Forms.Cursors.Default;
            this.buttonCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            resources.ApplyResources(this.buttonCancel, "buttonCancel");
            this.buttonCancel.Name = "buttonCancel";
            this.buttonCancel.UseVisualStyleBackColor = true;
            this.buttonCancel.Click += new System.EventHandler(this.buttonCancel_Click);
            // 
            // buttonAdd
            // 
            this.buttonAdd.Cursor = System.Windows.Forms.Cursors.Arrow;
            resources.ApplyResources(this.buttonAdd, "buttonAdd");
            this.buttonAdd.Name = "buttonAdd";
            this.buttonAdd.UseVisualStyleBackColor = true;
            this.buttonAdd.Click += new System.EventHandler(this.buttonAdd_Click);
            // 
            // AysnLogincheckBox
            // 
            resources.ApplyResources(this.AysnLogincheckBox, "AysnLogincheckBox");
            this.AysnLogincheckBox.Name = "AysnLogincheckBox";
            this.AysnLogincheckBox.UseVisualStyleBackColor = true;
            this.AysnLogincheckBox.CheckedChanged += new System.EventHandler(this.AysnLogincheckBox_CheckedChanged);
            // 
            // AddDevice
            // 
            this.AcceptButton = this.buttonAdd;
            resources.ApplyResources(this, "$this");
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.CancelButton = this.buttonCancel;
            this.Controls.Add(this.AysnLogincheckBox);
            this.Controls.Add(this.buttonCancel);
            this.Controls.Add(this.buttonAdd);
            this.Controls.Add(this.textBoxPassword);
            this.Controls.Add(this.labelPassword);
            this.Controls.Add(this.textBoxUserName);
            this.Controls.Add(this.labelUserName);
            this.Controls.Add(this.textBoxPort);
            this.Controls.Add(this.labelPort);
            this.Controls.Add(this.textBoxDeviceAddress);
            this.Controls.Add(this.labelDeviceAddress);
            this.Controls.Add(this.textBoxLocalNode);
            this.Controls.Add(this.labelLocalNode);
            this.Cursor = System.Windows.Forms.Cursors.Default;
            this.Name = "AddDevice";
            this.Load += new System.EventHandler(this.AddDevice_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox textBoxPassword;
        private System.Windows.Forms.Label labelPassword;
        private System.Windows.Forms.TextBox textBoxUserName;
        private System.Windows.Forms.Label labelUserName;
        private System.Windows.Forms.TextBox textBoxPort;
        private System.Windows.Forms.Label labelPort;
        private System.Windows.Forms.TextBox textBoxDeviceAddress;
        private System.Windows.Forms.Label labelDeviceAddress;
        private System.Windows.Forms.TextBox textBoxLocalNode;
        private System.Windows.Forms.Label labelLocalNode;
        private System.Windows.Forms.Button buttonCancel;
        private System.Windows.Forms.Button buttonAdd;
        private System.Windows.Forms.CheckBox AysnLogincheckBox;
    }
}