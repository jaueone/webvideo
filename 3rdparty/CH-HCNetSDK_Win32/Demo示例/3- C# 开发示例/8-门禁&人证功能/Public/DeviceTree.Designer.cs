namespace AcsDemo.Public
{
    partial class DeviceTree
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
                tagFont.Dispose();
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
            this.components = new System.ComponentModel.Container();
            System.Windows.Forms.TreeNode treeNode1 = new System.Windows.Forms.TreeNode("Card Reader 1", -2, -2);
            System.Windows.Forms.TreeNode treeNode2 = new System.Windows.Forms.TreeNode("Card Reader 2", -2, -2);
            System.Windows.Forms.TreeNode treeNode3 = new System.Windows.Forms.TreeNode("Door 1", 6, 6, new System.Windows.Forms.TreeNode[] {
            treeNode1,
            treeNode2});
            System.Windows.Forms.TreeNode treeNode4 = new System.Windows.Forms.TreeNode("Card Reader 3");
            System.Windows.Forms.TreeNode treeNode5 = new System.Windows.Forms.TreeNode("Card Reader 4");
            System.Windows.Forms.TreeNode treeNode6 = new System.Windows.Forms.TreeNode("Door 2", 6, 6, new System.Windows.Forms.TreeNode[] {
            treeNode4,
            treeNode5});
            System.Windows.Forms.TreeNode treeNode7 = new System.Windows.Forms.TreeNode("Card Reader 5");
            System.Windows.Forms.TreeNode treeNode8 = new System.Windows.Forms.TreeNode("Card Reader 6");
            System.Windows.Forms.TreeNode treeNode9 = new System.Windows.Forms.TreeNode("Door 3", 6, 6, new System.Windows.Forms.TreeNode[] {
            treeNode7,
            treeNode8});
            System.Windows.Forms.TreeNode treeNode10 = new System.Windows.Forms.TreeNode("Card Reader 7");
            System.Windows.Forms.TreeNode treeNode11 = new System.Windows.Forms.TreeNode("Card Reader 8");
            System.Windows.Forms.TreeNode treeNode12 = new System.Windows.Forms.TreeNode("Door 4", 6, 6, new System.Windows.Forms.TreeNode[] {
            treeNode10,
            treeNode11});
            System.Windows.Forms.TreeNode treeNode13 = new System.Windows.Forms.TreeNode("Device", 2, 2, new System.Windows.Forms.TreeNode[] {
            treeNode3,
            treeNode6,
            treeNode9,
            treeNode12});
            System.Windows.Forms.TreeNode treeNode14 = new System.Windows.Forms.TreeNode("Device Tree(Right Click to Add Device)", new System.Windows.Forms.TreeNode[] {
            treeNode13});
            this.treeViewDevice = new System.Windows.Forms.TreeView();
            this.treeImageList = new System.Windows.Forms.ImageList(this.components);
            this.treeDeviceMenuStrip = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.guardToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.loginoutToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.deleteToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            this.attributeToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.uguardToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.treeDoorMenuStrip = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.openToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.closeToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.alwaysOpenToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.alwaysCloseToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.treeDeviceMenuStrip.SuspendLayout();
            this.treeDoorMenuStrip.SuspendLayout();
            this.SuspendLayout();
            // 
            // treeViewDevice
            // 
            this.treeViewDevice.Dock = System.Windows.Forms.DockStyle.Fill;
            this.treeViewDevice.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.treeViewDevice.ImageIndex = 0;
            this.treeViewDevice.ImageList = this.treeImageList;
            this.treeViewDevice.Location = new System.Drawing.Point(0, 0);
            this.treeViewDevice.Name = "treeViewDevice";
            treeNode1.ImageIndex = -2;
            treeNode1.Name = "Card Reader 1";
            treeNode1.SelectedImageIndex = -2;
            treeNode1.Text = "Card Reader 1";
            treeNode2.ImageIndex = -2;
            treeNode2.Name = "Card Reader 2";
            treeNode2.SelectedImageIndex = -2;
            treeNode2.Text = "Card Reader 2";
            treeNode3.ImageIndex = 6;
            treeNode3.Name = "Door 1";
            treeNode3.SelectedImageIndex = 6;
            treeNode3.Text = "Door 1";
            treeNode4.Name = "Card Reader 3";
            treeNode4.Text = "Card Reader 3";
            treeNode5.Name = "Card Reader 4";
            treeNode5.Text = "Card Reader 4";
            treeNode6.ImageIndex = 6;
            treeNode6.Name = "Door 2";
            treeNode6.SelectedImageIndex = 6;
            treeNode6.Text = "Door 2";
            treeNode7.Name = "Card Reader 5";
            treeNode7.Text = "Card Reader 5";
            treeNode8.Name = "Card Reader 6";
            treeNode8.Text = "Card Reader 6";
            treeNode9.ImageIndex = 6;
            treeNode9.Name = "Door 3";
            treeNode9.SelectedImageIndex = 6;
            treeNode9.Text = "Door 3";
            treeNode10.Name = "Card Reader 7";
            treeNode10.Text = "Card Reader 7";
            treeNode11.Name = "Card Reader 8";
            treeNode11.Text = "Card Reader 8";
            treeNode12.ImageIndex = 6;
            treeNode12.Name = "Door 4";
            treeNode12.SelectedImageIndex = 6;
            treeNode12.Text = "Door 4";
            treeNode13.ImageIndex = 2;
            treeNode13.Name = "Device";
            treeNode13.SelectedImageIndex = 2;
            treeNode13.Text = "Device";
            treeNode14.Name = "TreeBoot";
            treeNode14.Text = "Device Tree(Right Click to Add Device)";
            this.treeViewDevice.Nodes.AddRange(new System.Windows.Forms.TreeNode[] {
            treeNode14});
            this.treeViewDevice.SelectedImageIndex = 0;
            this.treeViewDevice.Size = new System.Drawing.Size(221, 374);
            this.treeViewDevice.TabIndex = 21;
            this.treeViewDevice.DrawNode += new System.Windows.Forms.DrawTreeNodeEventHandler(this.treeViewDevice_DrawNode);
            this.treeViewDevice.BeforeSelect += new System.Windows.Forms.TreeViewCancelEventHandler(this.treeViewDevice_BeforeSelect);
            this.treeViewDevice.NodeMouseClick += new System.Windows.Forms.TreeNodeMouseClickEventHandler(this.treeViewDevice_NodeMouseClick);
            this.treeViewDevice.Leave += new System.EventHandler(this.treeViewDevice_Leave);
            this.treeViewDevice.MouseLeave += new System.EventHandler(this.treeViewDevice_MouseLeave);
            // 
            // treeImageList
            // 
            this.treeImageList.ColorDepth = System.Windows.Forms.ColorDepth.Depth8Bit;
            this.treeImageList.ImageSize = new System.Drawing.Size(16, 16);
            this.treeImageList.TransparentColor = System.Drawing.Color.Transparent;
            // 
            // treeDeviceMenuStrip
            // 
            this.treeDeviceMenuStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.guardToolStripMenuItem,
            this.loginoutToolStripMenuItem,
            this.deleteToolStripMenuItem,
            this.toolStripSeparator1,
            this.attributeToolStripMenuItem});
            this.treeDeviceMenuStrip.Name = "treeDeviceMenuStrip";
            this.treeDeviceMenuStrip.Size = new System.Drawing.Size(127, 98);
            // 
            // guardToolStripMenuItem
            // 
            this.guardToolStripMenuItem.Enabled = false;
            this.guardToolStripMenuItem.Name = "guardToolStripMenuItem";
            this.guardToolStripMenuItem.Size = new System.Drawing.Size(126, 22);
            this.guardToolStripMenuItem.Text = "Guard";
            this.guardToolStripMenuItem.Click += new System.EventHandler(this.guardToolStripMenuItem_Click);
            // 
            // loginoutToolStripMenuItem
            // 
            this.loginoutToolStripMenuItem.Name = "loginoutToolStripMenuItem";
            this.loginoutToolStripMenuItem.Size = new System.Drawing.Size(126, 22);
            this.loginoutToolStripMenuItem.Text = "Logout";
            this.loginoutToolStripMenuItem.Click += new System.EventHandler(this.loginoutToolStripMenuItem_Click);
            // 
            // deleteToolStripMenuItem
            // 
            this.deleteToolStripMenuItem.Name = "deleteToolStripMenuItem";
            this.deleteToolStripMenuItem.Size = new System.Drawing.Size(126, 22);
            this.deleteToolStripMenuItem.Text = "Delete";
            this.deleteToolStripMenuItem.Click += new System.EventHandler(this.deleteToolStripMenuItem_Click);
            // 
            // toolStripSeparator1
            // 
            this.toolStripSeparator1.Name = "toolStripSeparator1";
            this.toolStripSeparator1.Size = new System.Drawing.Size(123, 6);
            // 
            // attributeToolStripMenuItem
            // 
            this.attributeToolStripMenuItem.Name = "attributeToolStripMenuItem";
            this.attributeToolStripMenuItem.Size = new System.Drawing.Size(126, 22);
            this.attributeToolStripMenuItem.Text = "Attribute";
            this.attributeToolStripMenuItem.Click += new System.EventHandler(this.attributeToolStripMenuItem_Click);
            // 
            // uguardToolStripMenuItem
            // 
            this.uguardToolStripMenuItem.Name = "uguardToolStripMenuItem";
            this.uguardToolStripMenuItem.Size = new System.Drawing.Size(32, 19);
            // 
            // treeDoorMenuStrip
            // 
            this.treeDoorMenuStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.openToolStripMenuItem,
            this.closeToolStripMenuItem,
            this.alwaysOpenToolStripMenuItem,
            this.alwaysCloseToolStripMenuItem});
            this.treeDoorMenuStrip.Name = "treeDoorMenuStrip";
            this.treeDoorMenuStrip.Size = new System.Drawing.Size(153, 114);
            // 
            // openToolStripMenuItem
            // 
            this.openToolStripMenuItem.Name = "openToolStripMenuItem";
            this.openToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
            this.openToolStripMenuItem.Text = "Open";
            this.openToolStripMenuItem.Click += new System.EventHandler(this.openToolStripMenuItem_Click);
            // 
            // closeToolStripMenuItem
            // 
            this.closeToolStripMenuItem.Name = "closeToolStripMenuItem";
            this.closeToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
            this.closeToolStripMenuItem.Text = "Close";
            this.closeToolStripMenuItem.Click += new System.EventHandler(this.closeToolStripMenuItem_Click);
            // 
            // alwaysOpenToolStripMenuItem
            // 
            this.alwaysOpenToolStripMenuItem.Name = "alwaysOpenToolStripMenuItem";
            this.alwaysOpenToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
            this.alwaysOpenToolStripMenuItem.Text = "AlwaysOpen";
            this.alwaysOpenToolStripMenuItem.Click += new System.EventHandler(this.alwaysOpenToolStripMenuItem_Click);
            // 
            // alwaysCloseToolStripMenuItem
            // 
            this.alwaysCloseToolStripMenuItem.Name = "alwaysCloseToolStripMenuItem";
            this.alwaysCloseToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
            this.alwaysCloseToolStripMenuItem.Text = "AlwaysClose";
            this.alwaysCloseToolStripMenuItem.Click += new System.EventHandler(this.alwaysCloseToolStripMenuItem_Click);
            // 
            // DeviceTree
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(96F, 96F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Dpi;
            this.AutoSize = true;
            this.Controls.Add(this.treeViewDevice);
            this.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.Name = "DeviceTree";
            this.Size = new System.Drawing.Size(221, 374);
            this.treeDeviceMenuStrip.ResumeLayout(false);
            this.treeDoorMenuStrip.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.TreeView treeViewDevice;
        private System.Windows.Forms.ContextMenuStrip treeDeviceMenuStrip;
        private System.Windows.Forms.ToolStripMenuItem guardToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem loginoutToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem deleteToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem attributeToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem uguardToolStripMenuItem;
        private System.Windows.Forms.ContextMenuStrip treeDoorMenuStrip;
        private System.Windows.Forms.ToolStripMenuItem openToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem closeToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem alwaysOpenToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem alwaysCloseToolStripMenuItem;
        private System.Windows.Forms.ImageList treeImageList;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;

    }
}
