using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using System.Xml;
using System.IO;

using AcsDemo.ProductAcs;

namespace AcsDemo.Public
{
    public delegate void mouseClickDeviceTreeNodeEventHander(object sender, TreeNodeMouseClickEventArgs e);
    public partial class DeviceTree : UserControl
    {
        public AcsDemoPublic.STRU_DEVICE_INFO[] g_struDeviceInfo = new AcsDemoPublic.STRU_DEVICE_INFO[AcsDemoPublic.MAX_DEVICES];
        private static DeviceTree g_DeviceTree = new DeviceTree();
        private DeviceLogList g_formList = DeviceLogList.Instance();

        // Create a Font object for the node tags.
        Font tagFont = new Font("Arial", 9, FontStyle.Bold);

        const uint XML_ABILITY_OUT_LEN = 3 * 1024 * 1024;

        public event mouseClickDeviceTreeNodeEventHander TreeClick;

        private CHCNetSDK.MSGCallBack m_falarmData = null;
        private int m_iCurDoorIndex = -1;
        private int m_iCurCardReaderIndex = -1;
        private int m_iCurDeviceIndex = -1;
        private int m_iCurChanIndex = -1;

        public DeviceTree()
        {
            InitializeComponent();
            InitImageList();
        }

        private void InitImageList()
        {
            //this.treeViewDevice.ImageList = new ImageList();

            this.treeViewDevice.ImageList.Images.Add("tree", Properties.Resources.tree);
            this.treeViewDevice.ImageList.Images.Add("logout", Properties.Resources.logout);
            this.treeViewDevice.ImageList.Images.Add("login", Properties.Resources.login);
            this.treeViewDevice.ImageList.Images.Add("fortify", Properties.Resources.fortify);
            this.treeViewDevice.ImageList.Images.Add("dev_alarm", Properties.Resources.dev_alarm);
            this.treeViewDevice.ImageList.Images.Add("fortify_alarm", Properties.Resources.fortify_alarm);
            this.treeViewDevice.ImageList.Images.Add("door_closed", Properties.Resources.door_closed);
            this.treeViewDevice.ImageList.Images.Add("door_open", Properties.Resources.door_open);
            this.treeViewDevice.ImageList.Images.Add("card_reader", Properties.Resources.card_reader);
            this.treeViewDevice.ImageList.Images.Add("card_reader_offline", Properties.Resources.card_reader_offline);
        }  

        public static DeviceTree Instance()
        {
            for (int i = 0; i < g_DeviceTree.g_struDeviceInfo.Length; i++)
            {
                if (g_DeviceTree.g_struDeviceInfo[i].bInit)
                {
                    break;
                }
                g_DeviceTree.g_struDeviceInfo[i].Init();

                //door status init
                for (int j = 0; j < CHCNetSDK.MAX_DOOR_NUM_256; j += 3)
                {
                    g_DeviceTree.g_struDeviceInfo[i].iDoorStatus[j] = AcsDemoPublic.DOOR_COLSED;
                    //the following two is card reader status with this door
                    g_DeviceTree.g_struDeviceInfo[i].iDoorStatus[j + 1] = AcsDemoPublic.CARD_OFF_LINE;
                    g_DeviceTree.g_struDeviceInfo[i].iDoorStatus[j + 2] = AcsDemoPublic.CARD_OFF_LINE;
                }
            }

            return g_DeviceTree;
        }

        public void UpdateTreeNode()
        {
            int iImage = 0;
            int iDeviceIndex = -1;
            int iDoorIndex = -1;

            TreeNode node = treeViewDevice.SelectedNode;

            if (treeViewDevice.SelectedNode.Level == 0)  // DirectoryNode
            {
                return;
            }
            else if (treeViewDevice.SelectedNode.Level == 1) // device Node
            {
                iDeviceIndex = int.Parse(treeViewDevice.SelectedNode.Name) % 1000;
                iImage = getSelDeviceDisplayImageIndex(iDeviceIndex);
                System.Diagnostics.Debug.Assert(iImage >= 0);

                // get image from image list
                treeViewDevice.SelectedNode.SelectedImageIndex = iImage;
                treeViewDevice.SelectedNode.ImageIndex = iImage;

                // indicate device login out
                if (iImage == 1)
                {
                    return;
                } 
            }
            else if (treeViewDevice.SelectedNode.Level == 2) // door Node
            {
                iDeviceIndex = int.Parse(treeViewDevice.SelectedNode.Parent.Name) % 1000;
                node = treeViewDevice.SelectedNode.Parent;
            }
            else if (treeViewDevice.SelectedNode.Level == 3)
            {
                iDeviceIndex = int.Parse(treeViewDevice.SelectedNode.Parent.Parent.Name) % 1000;
                node = treeViewDevice.SelectedNode.Parent.Parent;
            }

            TreeNodeCollection tncDoorNodes = node.Nodes;// door Node
            foreach (TreeNode tnDoorNode in tncDoorNodes)
            {
                iDoorIndex = int.Parse(tnDoorNode.Name) % 1000;
                iImage = getSelDoorDisplayImageIndex(iDeviceIndex, iDoorIndex);
                System.Diagnostics.Debug.Assert(iImage >= 0);

                // get image from image list
                tnDoorNode.SelectedImageIndex = iImage;
                tnDoorNode.ImageIndex = iImage;
                string doorName = GetCurDoorName(iDoorIndex);
                if (doorName != null && doorName != "")
                {
                    tnDoorNode.Text = doorName;
                }

                TreeNodeCollection tncCardNodes = tnDoorNode.Nodes;// card reader Node
                foreach (TreeNode tnCardNode in tncCardNodes)
                {
                    int iCardReaderIndex = int.Parse(tnCardNode.Name) % 1000;
                    iImage = getSelCardReaderDisplayImageIndex(iDeviceIndex, iDoorIndex, iCardReaderIndex);

                    System.Diagnostics.Debug.Assert(iImage >= 0);

                    // get image from image list
                    tnCardNode.SelectedImageIndex = iImage;
                    tnCardNode.ImageIndex = iImage;
                }
            }
        }
        public void ClearAllNodes()
        {
            treeViewDevice.Nodes.Clear();
            TreeNode treeNodeRoot = new TreeNode("Device Tree(Right Click to Add Device)");
            treeViewDevice.Nodes.Add(treeNodeRoot);
        }

        public int GetCurDoorIndex()
        {
            if (m_iCurDoorIndex < 0 || m_iCurDeviceIndex > AcsDemoPublic.MAX_DEVICES)
            {
                return -1;
            }

            return m_iCurDoorIndex;
        }

        public string GetCurDoorName(int doorIndex)
        {
            if (doorIndex < 0 || doorIndex > AcsDemoPublic.MAX_DEVICES || m_iCurDeviceIndex < 0)
            {
                return null;
            }

            return g_DeviceTree.g_struDeviceInfo[m_iCurDeviceIndex].sDoorName[doorIndex];
        }


        public int GetCurCardReaderIndex()
        {
            if (m_iCurCardReaderIndex < 0 || m_iCurDeviceIndex > AcsDemoPublic.MAX_DEVICES)
            {
                return -1;
            }

            return m_iCurCardReaderIndex;
        }

        public int GetCurChannel()
        {
            if (m_iCurChanIndex >= 0 && m_iCurChanIndex < CHCNetSDK.MAX_CHANNUM_V40)
            {
                if (m_iCurDeviceIndex >= 0 || m_iCurDeviceIndex < AcsDemoPublic.MAX_DEVICES)
                {
                    return g_struDeviceInfo[m_iCurDeviceIndex].pStruChanInfo[m_iCurChanIndex].iChannelNO;
                }

            }
            return -1;
        }

        public String GetCurDeviceIp()
        {
            if (m_iCurDeviceIndex < 0 || m_iCurDeviceIndex > AcsDemoPublic.MAX_DEVICES)
            {
                return null;
            }
            return g_struDeviceInfo[m_iCurDeviceIndex].chDeviceIP;
        }
        public long GetCurLoginID()
        {
            if (m_iCurDeviceIndex < 0 || m_iCurDeviceIndex > AcsDemoPublic.MAX_DEVICES)
            {
                return -1;
            }
            return g_struDeviceInfo[m_iCurDeviceIndex].lLoginID;
        }
        public String GetCurLocalNodeName()
        {
            if (m_iCurDeviceIndex < 0 || m_iCurDeviceIndex > AcsDemoPublic.MAX_DEVICES)
            {
                return null;
            }
            return g_struDeviceInfo[m_iCurDeviceIndex].chLocalNodeName;
        }
        public int GetCurDeviceIndex()
        {
            if (m_iCurDeviceIndex < 0 || m_iCurDeviceIndex > AcsDemoPublic.MAX_DEVICES)
            {
                return -1;
            }

            return m_iCurDeviceIndex;
        }

        public int SetCurRealHandle(Int32 lRealHandle)
        {
            if (m_iCurChanIndex >= 0 && m_iCurChanIndex < CHCNetSDK.MAX_CHANNUM_V40)
            {
                if (m_iCurDeviceIndex >= 0 || m_iCurDeviceIndex < AcsDemoPublic.MAX_DEVICES)
                {
                    g_struDeviceInfo[m_iCurDeviceIndex].pStruChanInfo[m_iCurChanIndex].lRealHandle = lRealHandle;
                    return lRealHandle;
                }

            }
            return -1;
        }
        public long GetCurRealHandle()
        {
            if (m_iCurChanIndex >= 0 && m_iCurChanIndex < CHCNetSDK.MAX_CHANNUM_V40)
            {
                if (m_iCurDeviceIndex >= 0 || m_iCurDeviceIndex < AcsDemoPublic.MAX_DEVICES)
                {
                    return g_struDeviceInfo[m_iCurDeviceIndex].pStruChanInfo[m_iCurChanIndex].lRealHandle;
                }

            }
            return -1;
        }
        public AcsDemoPublic.STRU_DEVICE_INFO GetCurDeviceInfo()
        {
            if (m_iCurDeviceIndex < 0 || m_iCurDeviceIndex > AcsDemoPublic.MAX_DEVICES)
            {
                return g_struDeviceInfo[0];
            }
            return g_struDeviceInfo[m_iCurDeviceIndex];
        }

        public AcsDemoPublic.STRU_DEVICE_INFO GetCurDeviceInfobyIndex(int iDeviceIndex)
        {
            if (iDeviceIndex < 0 || iDeviceIndex > AcsDemoPublic.MAX_DEVICES)
            {
                return g_struDeviceInfo[0];
            }
            return g_struDeviceInfo[iDeviceIndex];
        }

        public AcsDemoPublic.STRU_CHANNEL_INFO GetCurChanInfo()
        {
            if (m_iCurChanIndex < 0 || m_iCurChanIndex > CHCNetSDK.MAX_CHANNUM_V40)
            {
                if (m_iCurDeviceIndex < 0 || m_iCurDeviceIndex > AcsDemoPublic.MAX_DEVICES)
                {
                    return g_struDeviceInfo[0].pStruChanInfo[0];
                }
                return g_struDeviceInfo[m_iCurDeviceIndex].pStruChanInfo[0];
            }
            return g_struDeviceInfo[m_iCurDeviceIndex].pStruChanInfo[m_iCurChanIndex];
        }

        public AcsDemoPublic.STRU_CHANNEL_INFO GetCurChanInfoByIndex(int iChanIndex)
        {
            if (iChanIndex < 0 || iChanIndex > CHCNetSDK.MAX_CHANNUM_V40)
            {
                if (m_iCurDeviceIndex < 0 || m_iCurDeviceIndex > AcsDemoPublic.MAX_DEVICES)
                {
                    return g_struDeviceInfo[0].pStruChanInfo[0];
                }
                return g_struDeviceInfo[m_iCurDeviceIndex].pStruChanInfo[0];
            }
            return g_struDeviceInfo[m_iCurDeviceIndex].pStruChanInfo[iChanIndex];
        }

        private uint GetCurACSDeviceDoorNum()
        {
            //get device ability for door num ,read carder num is door num*2
            String exceptionInfo = null;
            uint doorNum = 1;

            int lLoginID = g_struDeviceInfo[m_iCurDeviceIndex].lLoginID;
            string pOutXMLBuf = GetACSDeviceAbility(lLoginID, CHCNetSDK.ACS_ABILITY, "<AcsAbility version=\"2.0\">\r\n</AcsAbility>");
            if (pOutXMLBuf != null)
            {
                try
                {
                    XmlDocument doc = new XmlDocument();

                    // wait to determine
                    doc.LoadXml(pOutXMLBuf);
                    XmlNode root = doc.DocumentElement;
                    XmlNode node = root.SelectSingleNode("doorNo");
                
                    XmlNode doorNumAtttibuteNode = node.SelectSingleNode("@max");
                    doorNum = uint.Parse(doorNumAtttibuteNode.InnerText);
                }
                catch (System.Xml.XmlException ex)
                {
                    exceptionInfo = ex.Message;
                }
                finally
                {
                    if (exceptionInfo != null)
                    {
                        MessageBox.Show("operation exception!" + exceptionInfo, "Prompt", MessageBoxButtons.OK, MessageBoxIcon.Information);
                    }
                }
                g_struDeviceInfo[m_iCurDeviceIndex].iDoorNum = (int)doorNum;
            }
            return doorNum;
        }

        private string GetACSDeviceAbility(int lUserID, int iAbilityType, string inParam)
        {
            String exceptionInfo = null;
            String errorinfo = null;
            int dwLastError = 0;

            IntPtr ptrCfgVer;
            IntPtr m_pOutBuf;
            bool dwReturnValue = false;
            uint dwInSize = 0;

            string pOutXMLBuf = null;
            string strInputParam = inParam;

            m_pOutBuf = Marshal.AllocHGlobal((Int32)XML_ABILITY_OUT_LEN);
            try
            {


                dwInSize = (uint)strInputParam.Length;
                ptrCfgVer = Marshal.StringToCoTaskMemAnsi(strInputParam);
                dwReturnValue = CHCNetSDK.NET_DVR_GetDeviceAbility(lUserID, (uint)iAbilityType, ptrCfgVer, dwInSize, m_pOutBuf, XML_ABILITY_OUT_LEN);
                Marshal.FreeHGlobal(ptrCfgVer);

                if (dwReturnValue)
                {
                    pOutXMLBuf = Marshal.PtrToStringAnsi(m_pOutBuf);
                }

                dwLastError = (int)CHCNetSDK.NET_DVR_GetLastError();
                errorinfo = Marshal.PtrToStringAnsi(CHCNetSDK.NET_DVR_GetErrorMsg(ref dwLastError));

            }
            catch (Exception ex)
            {
                exceptionInfo = ex.Message;
            }
            finally
            {
                Marshal.FreeHGlobal(m_pOutBuf);
                
                if (exceptionInfo != null)
                {
                    MessageBox.Show("operation exception, please check the exception information" + exceptionInfo, "Prompt", MessageBoxButtons.OK, MessageBoxIcon.Information);
                }
                else
                {
                    if (dwReturnValue)
                    {
                        g_formList.AddLog(m_iCurDeviceIndex, AcsDemoPublic.OPERATION_SUCC_T, "ACS_ABILITY:Get ability success!");
                        //MessageBox.Show("Get ability success!", "Prompt", MessageBoxButtons.OK, MessageBoxIcon.Information);
                    }
                    else
                    {
                        MessageBox.Show("Get ability failed! error code：" + dwLastError + ",error message:" + errorinfo, "Prompt", MessageBoxButtons.OK, MessageBoxIcon.Information);
                    }
                }
            }
            return pOutXMLBuf;
        }

        private void treeViewDevice_NodeMouseClick(object sender, TreeNodeMouseClickEventArgs e)
        {
            treeViewDevice.SelectedNode = e.Node;
            if (e.Button == MouseButtons.Left)
            {

                if (e.Node.IsExpanded)
                {
                    e.Node.Expand();
                }
                else
                {
                    e.Node.Collapse();
                }
                if (1 == e.Node.Level)
                {
                    m_iCurDeviceIndex = int.Parse(e.Node.Name) % 1000;

                    if (g_struDeviceInfo[m_iCurDeviceIndex].lLoginID < 0)
                    {
                        AddDevice addDevice = new AddDevice();
                        addDevice.m_iDeviceIndex = m_iCurDeviceIndex;
                        addDevice.Login(false);

                        // enable guard item 
                        ToolStripItem Ts = treeDeviceMenuStrip.Items[0];
                        Ts.Enabled = true;

                        //just trigger drawnode event
                        treeViewDevice.SelectedNode.BackColor = Color.Empty;
                        e.Node.ExpandAll();
                        UpdateTreeNode();
                    }

                    m_iCurChanIndex = -1;
                    m_iCurDoorIndex = -1;
                    m_iCurCardReaderIndex = -1;
                }
                else if (2 == e.Node.Level)
                {
                    m_iCurChanIndex = int.Parse(e.Node.Name) % 1000;
                    m_iCurDoorIndex = int.Parse(e.Node.Name) % 1000;
                    m_iCurDeviceIndex = int.Parse(e.Node.Parent.Name) % 1000;
                    m_iCurCardReaderIndex = -1;
                }
                else if (3 == e.Node.Level)
                {
                    m_iCurChanIndex = int.Parse(e.Node.Parent.Name) % 1000;
                    m_iCurDoorIndex = int.Parse(e.Node.Parent.Name) % 1000;
                    m_iCurDeviceIndex = int.Parse(e.Node.Parent.Parent.Name) % 1000;
                    m_iCurCardReaderIndex = int.Parse(e.Node.Name) % 1000;
                }

                TreeClick(sender, e);
            }

            if ((e.Node.Level == 0) && (e.Button == MouseButtons.Right))
            {
                int i = 0;
                for (i = 0; i < AcsDemoPublic.MAX_DEVICES; i++)
                {
                    // chSerialNumber is unique id of device
                    if (null == g_struDeviceInfo[i].chSerialNumber)
                    {
                        AddDevice addDevice = new AddDevice();
                        addDevice.m_iDeviceIndex = i;
                        m_iCurDeviceIndex = i;
                        if (addDevice.ShowDialog() == DialogResult.OK)
                        {
                            //get the door number from device ability
                            uint deviceDoorNum = GetCurACSDeviceDoorNum();
                            g_struDeviceInfo[i].iDoorNum = (int)deviceDoorNum;

                            // enable guard item 
                            ToolStripItem Ts = treeDeviceMenuStrip.Items[0];
                            Ts.Enabled = true;

                            this.SuspendLayout();

                            TreeNode deviceNode = new TreeNode();
                            deviceNode.Text = g_struDeviceInfo[i].chLocalNodeName;
                            deviceNode.Name = (AcsDemoPublic.DEVICETYPE * 1000 + g_struDeviceInfo[i].iDeviceIndex).ToString();
                            deviceNode.ImageKey = "login";
                            deviceNode.SelectedImageKey = "login";

                            // add the door node to treeView control 
                            for (int j = 0; j < deviceDoorNum; j++)
                            {
                                TreeNode doorNode = new TreeNode();
                                doorNode.Text = "Door " + (j + 1);
                                doorNode.Name = (AcsDemoPublic.DOORTYPE * 1000 + j).ToString(); ;
                                doorNode.ImageKey = "door_closed";
                                doorNode.SelectedImageKey = "door_closed";

                                // add card reader node
                                TreeNode cardReaderNode1 = new TreeNode();
                                TreeNode cardReaderNode2 = new TreeNode();
                                cardReaderNode1.Text = "Reader Card 1";
                                // door index
                                cardReaderNode1.Name = (AcsDemoPublic.CARDREADERTYPE * 1000 + 2 * j).ToString();
                                cardReaderNode1.ImageKey = "card_reader";
                                cardReaderNode1.SelectedImageKey = "card_reader";

                                cardReaderNode2.Text = "Reader Card 2";
                                cardReaderNode2.Name = (AcsDemoPublic.CARDREADERTYPE * 1000 + 2 * j + 1).ToString();
                                cardReaderNode2.ImageKey = "card_reader";
                                cardReaderNode2.SelectedImageKey = "card_reader";

                                doorNode.Nodes.Add(cardReaderNode1);
                                doorNode.Nodes.Add(cardReaderNode2);

                                deviceNode.Nodes.Add(doorNode);
                            }

                            //zero
                            //for (int j = 0; j < g_struDeviceInfo[i].byZeroChanNum; j++)
                            //{
                            //    TreeNode chanNode = new TreeNode();
                            //    chanNode.Text = g_struDeviceInfo[i].struZeroChan[j].chChanName;
                            //    chanNode.Name = (AcsDemoPublic.CHANNELTYPE * 1000 + g_struDeviceInfo[i].struZeroChan[j].iChanIndex).ToString();
                            //    deviceNode.Nodes.Add(chanNode);
                            //}
                            //for (int j = 0; j < g_struDeviceInfo[i].byMirrorChanNum && j < 16; j++)
                            //{
                            //    TreeNode chanNode = new TreeNode();
                            //    chanNode.Text = g_struDeviceInfo[i].struMirrorChan[j].chChanName;
                            //    chanNode.Name = (AcsDemoPublic.CHANNELTYPE * 1000 + g_struDeviceInfo[i].struMirrorChan[j].iChanIndex).ToString();
                            //    deviceNode.Nodes.Add(chanNode);
                            //}

                            e.Node.Nodes.Add(deviceNode);

                            
                            e.Node.Expand();
                            deviceNode.ExpandAll();

                            // update node status
                            treeViewDevice.SelectedNode = deviceNode;
                            UpdateTreeNode();

                            this.ResumeLayout();
                        }
                        return;
                    }
                }
                if (i == AcsDemoPublic.MAX_DEVICES)
                {
                    MessageBox.Show("Exceeds the maximum number of Devices");
                }
            }

            if ((e.Node.Level == 1) && (e.Button == MouseButtons.Right))
            {
                e.Node.ContextMenuStrip = treeDeviceMenuStrip;
                treeDeviceMenuStrip.Show(e.Location);
            }

            if ((e.Node.Level == 2) && (e.Button == MouseButtons.Right))
            {
                e.Node.ContextMenuStrip = treeDoorMenuStrip;
                treeDoorMenuStrip.Show(e.Location);
            }
        }

        private void treeViewDevice_BeforeSelect(object sender, TreeViewCancelEventArgs e)
        {
            if (treeViewDevice.SelectedNode != null)
            {
                //return a selected node on the background color (no color) original 
                treeViewDevice.SelectedNode.BackColor = Color.Empty;
                //restore the foreground 
                treeViewDevice.SelectedNode.ForeColor = Color.Black;
            }
        }

        private void treeViewDevice_MouseLeave(object sender, EventArgs e)
        {
            if (treeViewDevice.SelectedNode != null)
            {
                //let selected back view appear blue 
                treeViewDevice.SelectedNode.BackColor = Color.LightBlue;
                //the foreground color to white 
                treeViewDevice.SelectedNode.ForeColor = Color.White;
            }
        }

        // get selected device node picture index in treeViewImagelist to display
        private int getSelDeviceDisplayImageIndex(int iDeviceIndex)
        {
            int iImage = AcsDemoPublic.DEVICE_LOGOUT;

            if (g_struDeviceInfo[iDeviceIndex].lLoginID >= 0
                && g_struDeviceInfo[iDeviceIndex].lFortifyHandle < 0 && !g_struDeviceInfo[iDeviceIndex].bAlarm)
            {
                iImage = AcsDemoPublic.DEVICE_LOGIN;
            }
            else if (g_struDeviceInfo[iDeviceIndex].lLoginID >= 0
                && g_struDeviceInfo[iDeviceIndex].lFortifyHandle >= 0 && !g_struDeviceInfo[iDeviceIndex].bAlarm)
            {
                iImage = AcsDemoPublic.DEVICE_FORTIFY;
            }
            else if (g_struDeviceInfo[iDeviceIndex].lLoginID >= 0
                && g_struDeviceInfo[iDeviceIndex].lFortifyHandle >= 0 && g_struDeviceInfo[iDeviceIndex].bAlarm)
            {
                iImage = AcsDemoPublic.DEVICE_FORTIFY_ALARM;
            }
            else if (g_struDeviceInfo[iDeviceIndex].lLoginID >= 0
                && g_struDeviceInfo[iDeviceIndex].lFortifyHandle < 0 && g_struDeviceInfo[iDeviceIndex].bAlarm)
            {
                iImage = AcsDemoPublic.DEVICE_ALARM;
            }
            else
            {
                iImage = AcsDemoPublic.DEVICE_LOGOUT;
            }
            g_struDeviceInfo[iDeviceIndex].dwImageType = (uint)iImage;
            return iImage;
        }

        // get selected door node picture index in treeViewImagelist to display
        private int getSelDoorDisplayImageIndex(int iDeviceIndex, int doorIndex)
        {
            int iImage = AcsDemoPublic.DOOR_COLSED;
            
            if(iDeviceIndex >= 0 && doorIndex>=0)
            {
                iImage = g_struDeviceInfo[iDeviceIndex].iDoorStatus[3 * doorIndex];
            }

            return iImage;
        }

        // get selected card reader node picture index in treeViewImagelist to display
        private int getSelCardReaderDisplayImageIndex(int iDeviceIndex, int doorIndex, int cardIndex)
        {
            int iImage = AcsDemoPublic.CARD_OFF_LINE;

            //get card reader status
            if (iDeviceIndex >= 0 && doorIndex >= 0 && cardIndex >= 0)
            {
                //we defined that the iDoorStatus array element,which consis of doorstatus and following two card reader status;
                iImage = g_struDeviceInfo[iDeviceIndex].iDoorStatus[3 * doorIndex + (cardIndex%2 + 1)];
            }

            return iImage;
        }


        //when draw mode set to OwnerDrawText ,the function will work
        private void treeViewDevice_DrawNode(object sender, DrawTreeNodeEventArgs e)
        {
            this.SuspendLayout();

            // node rect
            Rectangle nodeRect = e.Node.Bounds;

            // the start position to draw
            Point drawPt = new Point(nodeRect.Location.X - 18, nodeRect.Location.Y);

            // the image size and the draw rect
            Size imgSize = new Size(16, 16);
            Rectangle imgRect = new Rectangle(drawPt, imgSize);

            // Retrieve the node font. If the node font has not been set,
            // use the TreeView font.
            Font nodeFont = e.Node.NodeFont;
            if (nodeFont == null) nodeFont = ((TreeView)sender).Font;

            if (e.Node.Level == 0)  // DirectoryNode
            {
                e.Graphics.DrawImage(treeImageList.Images[0], imgRect);
            }
            else if (e.Node.Level == 1) // device Node
            {
                int iDeviceIndex = int.Parse(e.Node.Name) % 1000;
                int iImage = getSelDeviceDisplayImageIndex(iDeviceIndex);

                System.Diagnostics.Debug.Assert(iImage >= 0);

                // get image from image list
                Image image = treeImageList.Images[iImage];
                if (image == null) image = Properties.Resources.tree;

                e.Graphics.DrawImage(image, imgRect);
            }
            else if (e.Node.Level == 2) // door Node
            {
                int iDoorIndex = int.Parse(e.Node.Name) % 1000;
                int iDeviceIndex = int.Parse(e.Node.Parent.Name) % 1000;
                int iImage = getSelDoorDisplayImageIndex(iDeviceIndex, iDoorIndex);

                System.Diagnostics.Debug.Assert(iImage >= 0);

                // get image from image list
                Image image = treeImageList.Images[iImage];
                if (image == null) image = Properties.Resources.door_closed;

                e.Graphics.DrawImage(image, imgRect);
            }
            else if (e.Node.Level == 3) // Card Reader Node
            {
                int iCardReaderIndex = int.Parse(e.Node.Name) % 1000;
                int iDoorIndex = int.Parse(e.Node.Parent.Name) % 1000;
                int iDeviceIndex = int.Parse(e.Node.Parent.Parent.Name) % 1000;
                int iImage = getSelCardReaderDisplayImageIndex(iDeviceIndex, iDoorIndex, iCardReaderIndex);

                System.Diagnostics.Debug.Assert(iImage >= 0);

                // get image from image list
                Image image = treeImageList.Images[iImage];
                if (image == null) image = Properties.Resources.card_reader_offline;

                e.Graphics.DrawImage(image, imgRect);
            }

           // e.DrawDefault = true;
 

             // Draw the background and node text for a selected node(e.State & TreeNodeStates.Selected).
            if ( e.Node.IsSelected )
            {
                // Draw the background of the selected node. The NodeBounds
                // method makes the highlight rectangle large enough to
                // include the text of a node tag, if one is present.
                e.Graphics.FillRectangle(Brushes.LightBlue, e.Node.Bounds);

                // Draw the node text.
                e.Graphics.DrawString(e.Node.Text, nodeFont, Brushes.White,
                    Rectangle.Inflate(e.Bounds, 2, 0));
            }
            else// Use the default background and node text.
            {
                e.DrawDefault = true;
                // Draw the node text.;
                e.Graphics.DrawString(e.Node.Text, nodeFont, Brushes.Black,
                    Rectangle.Inflate(e.Bounds, 2, 0));
            }
            //e.Graphics.DrawString(e.Node.Text, nodeFont, Brushes.Black,
            //    Rectangle.Inflate(e.Bounds, 2, 0));
            // If a node tag is present, draw its string representation 
            // to the right of the label text.
            if (e.Node.Tag != null)
            {
                e.Graphics.DrawString(e.Node.Tag.ToString(), tagFont,
                    Brushes.Yellow, e.Bounds.Right + 2, e.Bounds.Top);
            }

            this.ResumeLayout();
        }

        // Returns the bounds of the specified node, including the region 
        // occupied by the node label and any node tag displayed.
        private Rectangle NodeBounds(TreeNode node)
        {
            // Set the return value to the normal node bounds.
            Rectangle bounds = node.Bounds;
            if (node.Tag != null)
            {
                // Retrieve a Graphics object from the TreeView handle
                // and use it to calculate the display width of the tag.
                Graphics g = treeViewDevice.CreateGraphics();
                int tagWidth = (int)g.MeasureString
                    (node.Tag.ToString(), tagFont).Width + 6;

                // Adjust the node bounds using the calculated value.
                bounds.Offset(tagWidth / 2, 0);
                bounds = Rectangle.Inflate(bounds, tagWidth / 2, 0);
                g.Dispose();
            }

            return bounds;

        }

        public void MsgCallback(int lCommand, ref CHCNetSDK.NET_DVR_ALARMER pAlarmer, IntPtr pAlarmInfo, uint dwBufLen, IntPtr pUser)
        {
            switch (lCommand)
            {
                case CHCNetSDK.COMM_ALARM_ACS:
                    ProcessCommAlarmACS(ref pAlarmer, pAlarmInfo, dwBufLen, pUser);
                    break;
                default:
                    break;
            }
        }

        // the main event alarm type of ACS host
        void AlarmMinorTypeMap(CHCNetSDK.NET_DVR_LOG_V30 stLogInfo, char[] csTmp)
        {
            String szTemp = null;
            switch (stLogInfo.dwMinorType)
            {
                //alarm
                case CHCNetSDK.MINOR_ALARMIN_SHORT_CIRCUIT:
                    szTemp = String.Format("MINOR_ALARMIN_SHORT_CIRCUIT");
                    break;
                case CHCNetSDK.MINOR_ALARMIN_BROKEN_CIRCUIT:
                    szTemp = String.Format("MINOR_ALARMIN_BROKEN_CIRCUIT");
                    break;
                case CHCNetSDK.MINOR_ALARMIN_EXCEPTION:
                    szTemp = String.Format("MINOR_ALARMIN_EXCEPTION");
                    break;
                case CHCNetSDK.MINOR_ALARMIN_RESUME:
                    szTemp = String.Format("MINOR_ALARMIN_RESUME");
                    break;
                case CHCNetSDK.MINOR_HOST_DESMANTLE_ALARM:
                    szTemp = String.Format("MINOR_HOST_DESMANTLE_ALARM");
                    break;
                case CHCNetSDK.MINOR_HOST_DESMANTLE_RESUME:
                    szTemp = String.Format("MINOR_HOST_DESMANTLE_RESUME");
                    break;
                case CHCNetSDK.MINOR_CARD_READER_DESMANTLE_ALARM:
                    szTemp = String.Format("MINOR_CARD_READER_DESMANTLE_ALARM");
                    break;
                case CHCNetSDK.MINOR_CARD_READER_DESMANTLE_RESUME:
                    szTemp = String.Format("MINOR_CARD_READER_DESMANTLE_RESUME");
                    break;
                case CHCNetSDK.MINOR_CASE_SENSOR_ALARM:
                    szTemp = String.Format("MINOR_CASE_SENSOR_ALARM");
                    break;
                case CHCNetSDK.MINOR_CASE_SENSOR_RESUME:
                    szTemp = String.Format("MINOR_CASE_SENSOR_RESUME");
                    break;
                case CHCNetSDK.MINOR_STRESS_ALARM:
                    szTemp = String.Format("MINOR_STRESS_ALARM");
                    break;
                case CHCNetSDK.MINOR_OFFLINE_ECENT_NEARLY_FULL:
                    szTemp = String.Format("MINOR_OFFLINE_ECENT_NEARLY_FULL");
                    break;
                case CHCNetSDK.MINOR_CARD_MAX_AUTHENTICATE_FAIL:
                    szTemp = String.Format("MINOR_CARD_MAX_AUTHENTICATE_FAIL");
                    break;
                case CHCNetSDK.MINOR_SD_CARD_FULL:
                    szTemp = String.Format("MINOR_SD_CARD_FULL");
                    break;
                case CHCNetSDK.MINOR_LINKAGE_CAPTURE_PIC:
                    szTemp = String.Format("MINOR_LINKAGE_CAPTURE_PIC");
                    break;
                case CHCNetSDK.MINOR_SECURITY_MODULE_DESMANTLE_ALARM:
                    szTemp = String.Format("MINOR_SECURITY_MODULE_DESMANTLE_ALARM");
                    break;
                case CHCNetSDK.MINOR_SECURITY_MODULE_DESMANTLE_RESUME:
                    szTemp = String.Format("MINOR_SECURITY_MODULE_DESMANTLE_RESUME");
                    break;
                case CHCNetSDK.MINOR_POS_START_ALARM:
                    szTemp = String.Format("MINOR_POS_START_ALARM");
                    break;
                case CHCNetSDK.MINOR_POS_END_ALARM:
                    szTemp = String.Format("MINOR_POS_END_ALARM");
                    break;
                case CHCNetSDK.MINOR_FACE_IMAGE_QUALITY_LOW:
                    szTemp = String.Format("MINOR_FACE_IMAGE_QUALITY_LOW");
                    break;
                case CHCNetSDK.MINOR_FINGE_RPRINT_QUALITY_LOW:
                    szTemp = String.Format("MINOR_FINGE_RPRINT_QUALITY_LOW");
                    break;
                case CHCNetSDK.MINOR_FIRE_IMPORT_SHORT_CIRCUIT:
                    szTemp = String.Format("MINOR_FIRE_IMPORT_SHORT_CIRCUIT");
                    break;
                case CHCNetSDK.MINOR_FIRE_IMPORT_BROKEN_CIRCUIT:
                    szTemp = String.Format("MINOR_FIRE_IMPORT_BROKEN_CIRCUIT");
                    break;
                case CHCNetSDK.MINOR_FIRE_IMPORT_RESUME:
                    szTemp = String.Format("MINOR_FIRE_IMPORT_RESUME");
                    break;
                case CHCNetSDK.MINOR_FIRE_BUTTON_TRIGGER:
                    szTemp = String.Format("MINOR_FIRE_BUTTON_TRIGGER");
                    break;
                case CHCNetSDK.MINOR_FIRE_BUTTON_RESUME:
                    szTemp = String.Format("MINOR_FIRE_BUTTON_RESUME");
                    break;
                case CHCNetSDK.MINOR_MAINTENANCE_BUTTON_TRIGGER:
                    szTemp = String.Format("MINOR_MAINTENANCE_BUTTON_TRIGGER");
                    break;
                case CHCNetSDK.MINOR_MAINTENANCE_BUTTON_RESUME:
                    szTemp = String.Format("MINOR_MAINTENANCE_BUTTON_RESUME");
                    break;
                case CHCNetSDK.MINOR_EMERGENCY_BUTTON_TRIGGER:
                    szTemp = String.Format("MINOR_EMERGENCY_BUTTON_TRIGGER");
                    break;
                case CHCNetSDK.MINOR_EMERGENCY_BUTTON_RESUME:
                    szTemp = String.Format("MINOR_EMERGENCY_BUTTON_RESUME");
                    break;
                case CHCNetSDK.MINOR_DISTRACT_CONTROLLER_ALARM:
                    szTemp = String.Format("MINOR_DISTRACT_CONTROLLER_ALARM");
                    break;
                case CHCNetSDK.MINOR_DISTRACT_CONTROLLER_RESUME:
                    szTemp = String.Format("MINOR_DISTRACT_CONTROLLER_RESUME");
                    break;
                default:
                    szTemp = String.Format("0x" + stLogInfo.dwMinorType);
                    break;
            }
            szTemp.CopyTo(0, csTmp, 0, szTemp.Length);
            return;
        }

        // the main event exception type 
        void ExceptionMinorTypeMap(CHCNetSDK.NET_DVR_LOG_V30 stLogInfo, char[] csTmp)
        {
            String szTemp = null;
            switch (stLogInfo.dwMinorType)
            {
                //exception
                case CHCNetSDK.MINOR_NET_BROKEN:
                    szTemp = String.Format("MINOR_NET_BROKEN");
                    break;
                case CHCNetSDK.MINOR_RS485_DEVICE_ABNORMAL:
                    szTemp = String.Format("MINOR_RS485_DEVICE_ABNORMAL");
                    break;
                case CHCNetSDK.MINOR_RS485_DEVICE_REVERT:
                    szTemp = String.Format("MINOR_RS485_DEVICE_REVERT");
                    break;
                case CHCNetSDK.MINOR_DEV_POWER_ON:
                    szTemp = String.Format("MINOR_DEV_POWER_ON");
                    break;
                case CHCNetSDK.MINOR_DEV_POWER_OFF:
                    szTemp = String.Format("MINOR_DEV_POWER_OFF");
                    break;
                case CHCNetSDK.MINOR_WATCH_DOG_RESET:
                    szTemp = String.Format("MINOR_WATCH_DOG_RESET");
                    break;
                case CHCNetSDK.MINOR_LOW_BATTERY:
                    szTemp = String.Format("MINOR_LOW_BATTERY");
                    break;
                case CHCNetSDK.MINOR_BATTERY_RESUME:
                    szTemp = String.Format("MINOR_BATTERY_RESUME");
                    break;
                case CHCNetSDK.MINOR_AC_OFF:
                    szTemp = String.Format("MINOR_AC_OFF");
                    break;
                case CHCNetSDK.MINOR_AC_RESUME:
                    szTemp = String.Format("MINOR_AC_RESUME");
                    break;
                case CHCNetSDK.MINOR_NET_RESUME:
                    szTemp = String.Format("MINOR_NET_RESUME");
                    break;
                case CHCNetSDK.MINOR_FLASH_ABNORMAL:
                    szTemp = String.Format("MINOR_FLASH_ABNORMAL");
                    break;
                case CHCNetSDK.MINOR_CARD_READER_OFFLINE:
                    szTemp = String.Format("MINOR_CARD_READER_OFFLINE");
                    break;
                case CHCNetSDK.MINOR_CARD_READER_RESUME:
                    szTemp = String.Format("MINOR_CAED_READER_RESUME");
                    break;
                case CHCNetSDK.MINOR_INDICATOR_LIGHT_OFF:
                    szTemp = String.Format("MINOR_INDICATOR_LIGHT_OFF");
                    break;
                case CHCNetSDK.MINOR_INDICATOR_LIGHT_RESUME:
                    szTemp = String.Format("MINOR_INDICATOR_LIGHT_RESUME");
                    break;
                case CHCNetSDK.MINOR_CHANNEL_CONTROLLER_OFF:
                    szTemp = String.Format("MINOR_CHANNEL_CONTROLLER_OFF");
                    break;
                case CHCNetSDK.MINOR_CHANNEL_CONTROLLER_RESUME:
                    szTemp = String.Format("MINOR_CHANNEL_CONTROLLER_RESUME");
                    break;
                case CHCNetSDK.MINOR_SECURITY_MODULE_OFF:
                    szTemp = String.Format("MINOR_SECURITY_MODULE_OFF");
                    break;
                case CHCNetSDK.MINOR_SECURITY_MODULE_RESUME:
                    szTemp = String.Format("MINOR_SECURITY_MODULE_RESUME");
                    break;
                case CHCNetSDK.MINOR_BATTERY_ELECTRIC_LOW:
                    szTemp = String.Format("MINOR_BATTERY_ELECTRIC_LOW");
                    break;
                case CHCNetSDK.MINOR_BATTERY_ELECTRIC_RESUME:
                    szTemp = String.Format("MINOR_BATTERY_ELECTRIC_RESUME");
                    break;
                case CHCNetSDK.MINOR_LOCAL_CONTROL_NET_BROKEN:
                    szTemp = String.Format("MINOR_LOCAL_CONTROL_NET_BROKEN");
                    break;
                case CHCNetSDK.MINOR_LOCAL_CONTROL_NET_RSUME:
                    szTemp = String.Format("MINOR_LOCAL_CONTROL_NET_RSUME");
                    break;
                case CHCNetSDK.MINOR_MASTER_RS485_LOOPNODE_BROKEN:
                    szTemp = String.Format("MINOR_MASTER_RS485_LOOPNODE_BROKEN");
                    break;
                case CHCNetSDK.MINOR_MASTER_RS485_LOOPNODE_RESUME:
                    szTemp = String.Format("MINOR_MASTER_RS485_LOOPNODE_RESUME");
                    break;
                case CHCNetSDK.MINOR_LOCAL_CONTROL_OFFLINE:
                    szTemp = String.Format("MINOR_LOCAL_CONTROL_OFFLINE");
                    break;
                case CHCNetSDK.MINOR_LOCAL_CONTROL_RESUME:
                    szTemp = String.Format("MINOR_LOCAL_CONTROL_RESUME");
                    break;
                case CHCNetSDK.MINOR_LOCAL_DOWNSIDE_RS485_LOOPNODE_BROKEN:
                    szTemp = String.Format("MINOR_LOCAL_DOWNSIDE_RS485_LOOPNODE_BROKEN");
                    break;
                case CHCNetSDK.MINOR_LOCAL_DOWNSIDE_RS485_LOOPNODE_RESUME:
                    szTemp = String.Format("MINOR_LOCAL_DOWNSIDE_RS485_LOOPNODE_RESUME");
                    break;
                case CHCNetSDK.MINOR_DISTRACT_CONTROLLER_ONLINE:
                    szTemp = String.Format("MINOR_DISTRACT_CONTROLLER_ONLINE");
                    break;
                case CHCNetSDK.MINOR_DISTRACT_CONTROLLER_OFFLINE:
                    szTemp = String.Format("MINOR_DISTRACT_CONTROLLER_OFFLINE");
                    break;
                case CHCNetSDK.MINOR_ID_CARD_READER_NOT_CONNECT:
                    szTemp = String.Format("MINOR_ID_CARD_READER_NOT_CONNECT");
                    break;
                case CHCNetSDK.MINOR_ID_CARD_READER_RESUME:
                    szTemp = String.Format("MINOR_ID_CARD_READER_RESUME");
                    break;
                case CHCNetSDK.MINOR_FINGER_PRINT_MODULE_NOT_CONNECT:
                    szTemp = String.Format("MINOR_FINGER_PRINT_MODULE_NOT_CONNECT");
                    break;
                case CHCNetSDK.MINOR_FINGER_PRINT_MODULE_RESUME:
                    szTemp = String.Format("MINOR_FINGER_PRINT_MODULE_RESUME");
                    break;
                case CHCNetSDK.MINOR_CAMERA_NOT_CONNECT:
                    szTemp = String.Format("MINOR_CAMERA_NOT_CONNECT");
                    break;
                case CHCNetSDK.MINOR_CAMERA_RESUME:
                    szTemp = String.Format("MINOR_CAMERA_RESUME");
                    break;
                case CHCNetSDK.MINOR_COM_NOT_CONNECT:
                    szTemp = String.Format("MINOR_COM_NOT_CONNECT");
                    break;
                case CHCNetSDK.MINOR_COM_RESUME:
                    szTemp = String.Format("MINOR_COM_RESUME");
                    break;
                case CHCNetSDK.MINOR_DEVICE_NOT_AUTHORIZE:
                    szTemp = String.Format("MINOR_DEVICE_NOT_AUTHORIZE");
                    break;
                case CHCNetSDK.MINOR_PEOPLE_AND_ID_CARD_DEVICE_ONLINE:
                    szTemp = String.Format("MINOR_PEOPLE_AND_ID_CARD_DEVICE_ONLINE");
                    break;
                case CHCNetSDK.MINOR_PEOPLE_AND_ID_CARD_DEVICE_OFFLINE:
                    szTemp = String.Format("MINOR_PEOPLE_AND_ID_CARD_DEVICE_OFFLINE");
                    break;
                case CHCNetSDK.MINOR_LOCAL_LOGIN_LOCK:
                    szTemp = String.Format("MINOR_LOCAL_LOGIN_LOCK");
                    break;
                case CHCNetSDK.MINOR_LOCAL_LOGIN_UNLOCK:
                    szTemp = String.Format("MINOR_LOCAL_LOGIN_UNLOCK");
                    break;
                default:
                    szTemp = String.Format("0x" + stLogInfo.dwMinorType);
                    break;
            }
            szTemp.CopyTo(0, csTmp, 0, szTemp.Length);
            return;
        }

        // the main event for the event type 
        void EventMinorTypeMap(CHCNetSDK.NET_DVR_LOG_V30 stLogInfo, char[] csTmp)
        {
            String szTemp = null;
            switch (stLogInfo.dwMinorType)
            {
                case CHCNetSDK.MINOR_LEGAL_CARD_PASS:
                    szTemp = String.Format("MINOR_LEGAL_CARD_PASS");
                    break;
                case CHCNetSDK.MINOR_CARD_AND_PSW_PASS:
                    szTemp = String.Format("MINOR_CARD_AND_PSW_PASS");
                    break;
                case CHCNetSDK.MINOR_CARD_AND_PSW_FAIL:
                    szTemp = String.Format("MINOR_CARD_AND_PSW_FAIL");
                    break;
                case CHCNetSDK.MINOR_CARD_AND_PSW_TIMEOUT:
                    szTemp = String.Format("MINOR_CARD_AND_PSW_TIMEOUT");
                    break;
                case CHCNetSDK.MINOR_CARD_AND_PSW_OVER_TIME:
                    szTemp = String.Format("MINOR_CARD_AND_PSW_OVER_TIME");
                    break;
                case CHCNetSDK.MINOR_CARD_NO_RIGHT:
                    szTemp = String.Format("MINOR_CARD_NO_RIGHT");
                    break;
                case CHCNetSDK.MINOR_CARD_INVALID_PERIOD:
                    szTemp = String.Format("MINOR_CARD_INVALID_PERIOD");
                    break;
                case CHCNetSDK.MINOR_CARD_OUT_OF_DATE:
                    szTemp = String.Format("MINOR_CARD_OUT_OF_DATE");
                    break;
                case CHCNetSDK.MINOR_INVALID_CARD:
                    szTemp = String.Format("MINOR_INVALID_CARD");
                    break;
                case CHCNetSDK.MINOR_ANTI_SNEAK_FAIL:
                    szTemp = String.Format("MINOR_ANTI_SNEAK_FAIL");
                    break;
                case CHCNetSDK.MINOR_INTERLOCK_DOOR_NOT_CLOSE:
                    szTemp = String.Format("MINOR_INTERLOCK_DOOR_NOT_CLOSE");
                    break;
                case CHCNetSDK.MINOR_NOT_BELONG_MULTI_GROUP:
                    szTemp = String.Format("MINOR_NOT_BELONG_MULTI_GROUP");
                    break;
                case CHCNetSDK.MINOR_INVALID_MULTI_VERIFY_PERIOD:
                    szTemp = String.Format("MINOR_INVALID_MULTI_VERIFY_PERIOD");
                    break;
                case CHCNetSDK.MINOR_MULTI_VERIFY_SUPER_RIGHT_FAIL:
                    szTemp = String.Format("MINOR_MULTI_VERIFY_SUPER_RIGHT_FAIL");
                    break;
                case CHCNetSDK.MINOR_MULTI_VERIFY_REMOTE_RIGHT_FAIL:
                    szTemp = String.Format("MINOR_MULTI_VERIFY_REMOTE_RIGHT_FAIL");
                    break;
                case CHCNetSDK.MINOR_MULTI_VERIFY_SUCCESS:
                    szTemp = String.Format("MINOR_MULTI_VERIFY_SUCCESS");
                    break;
                case CHCNetSDK.MINOR_LEADER_CARD_OPEN_BEGIN:
                    szTemp = String.Format("MINOR_LEADER_CARD_OPEN_BEGIN");
                    break;
                case CHCNetSDK.MINOR_LEADER_CARD_OPEN_END:
                    szTemp = String.Format("MINOR_LEADER_CARD_OPEN_END");
                    break;
                case CHCNetSDK.MINOR_ALWAYS_OPEN_BEGIN:
                    szTemp = String.Format("MINOR_ALWAYS_OPEN_BEGIN");
                    break;
                case CHCNetSDK.MINOR_ALWAYS_OPEN_END:
                    szTemp = String.Format("MINOR_ALWAYS_OPEN_END");
                    break;
                case CHCNetSDK.MINOR_LOCK_OPEN:
                    szTemp = String.Format("MINOR_LOCK_OPEN");
                    break;
                case CHCNetSDK.MINOR_LOCK_CLOSE:
                    szTemp = String.Format("MINOR_LOCK_CLOSE");
                    break;
                case CHCNetSDK.MINOR_DOOR_BUTTON_PRESS:
                    szTemp = String.Format("MINOR_DOOR_BUTTON_PRESS");
                    break;
                case CHCNetSDK.MINOR_DOOR_BUTTON_RELEASE:
                    szTemp = String.Format("MINOR_DOOR_BUTTON_RELEASE");
                    break;
                case CHCNetSDK.MINOR_DOOR_OPEN_NORMAL:
                    szTemp = String.Format("MINOR_DOOR_OPEN_NORMAL");
                    break;
                case CHCNetSDK.MINOR_DOOR_CLOSE_NORMAL:
                    szTemp = String.Format("MINOR_DOOR_CLOSE_NORMAL");
                    break;
                case CHCNetSDK.MINOR_DOOR_OPEN_ABNORMAL:
                    szTemp = String.Format("MINOR_DOOR_OPEN_ABNORMAL");
                    break;
                case CHCNetSDK.MINOR_DOOR_OPEN_TIMEOUT:
                    szTemp = String.Format("MINOR_DOOR_OPEN_TIMEOUT");
                    break;
                case CHCNetSDK.MINOR_ALARMOUT_ON:
                    szTemp = String.Format("MINOR_ALARMOUT_ON");
                    break;
                case CHCNetSDK.MINOR_ALARMOUT_OFF:
                    szTemp = String.Format("MINOR_ALARMOUT_OFF");
                    break;
                case CHCNetSDK.MINOR_ALWAYS_CLOSE_BEGIN:
                    szTemp = String.Format("MINOR_ALWAYS_CLOSE_BEGIN");
                    break;
                case CHCNetSDK.MINOR_ALWAYS_CLOSE_END:
                    szTemp = String.Format("MINOR_ALWAYS_CLOSE_END");
                    break;
                case CHCNetSDK.MINOR_MULTI_VERIFY_NEED_REMOTE_OPEN:
                    szTemp = String.Format("MINOR_MULTI_VERIFY_NEED_REMOTE_OPEN");
                    break;
                case CHCNetSDK.MINOR_MULTI_VERIFY_SUPERPASSWD_VERIFY_SUCCESS:
                    szTemp = String.Format("MINOR_MULTI_VERIFY_SUPERPASSWD_VERIFY_SUCCESS");
                    break;
                case CHCNetSDK.MINOR_MULTI_VERIFY_REPEAT_VERIFY:
                    szTemp = String.Format("MINOR_MULTI_VERIFY_REPEAT_VERIFY");
                    break;
                case CHCNetSDK.MINOR_MULTI_VERIFY_TIMEOUT:
                    szTemp = String.Format("MINOR_MULTI_VERIFY_TIMEOUT");
                    break;
                case CHCNetSDK.MINOR_DOORBELL_RINGING:
                    szTemp = String.Format("MINOR_DOORBELL_RINGING");
                    break;
                case CHCNetSDK.MINOR_FINGERPRINT_COMPARE_PASS:
                    szTemp = String.Format("MINOR_FINGERPRINT_COMPARE_PASS");
                    break;
                case CHCNetSDK.MINOR_FINGERPRINT_COMPARE_FAIL:
                    szTemp = String.Format("MINOR_FINGERPRINT_COMPARE_FAIL");
                    break;
                case CHCNetSDK.MINOR_CARD_FINGERPRINT_VERIFY_PASS:
                    szTemp = String.Format("MINOR_CARD_FINGERPRINT_VERIFY_PASS");
                    break;
                case CHCNetSDK.MINOR_CARD_FINGERPRINT_VERIFY_FAIL:
                    szTemp = String.Format("MINOR_CARD_FINGERPRINT_VERIFY_FAIL");
                    break;
                case CHCNetSDK.MINOR_CARD_FINGERPRINT_VERIFY_TIMEOUT:
                    szTemp = String.Format("MINOR_CARD_FINGERPRINT_VERIFY_TIMEOUT");
                    break;
                case CHCNetSDK.MINOR_CARD_FINGERPRINT_PASSWD_VERIFY_PASS:
                    szTemp = String.Format("MINOR_CARD_FINGERPRINT_PASSWD_VERIFY_PASS");
                    break;
                case CHCNetSDK.MINOR_CARD_FINGERPRINT_PASSWD_VERIFY_FAIL:
                    szTemp = String.Format("MINOR_CARD_FINGERPRINT_PASSWD_VERIFY_FAIL");
                    break;
                case CHCNetSDK.MINOR_CARD_FINGERPRINT_PASSWD_VERIFY_TIMEOUT:
                    szTemp = String.Format("MINOR_CARD_FINGERPRINT_PASSWD_VERIFY_TIMEOUT");
                    break;
                case CHCNetSDK.MINOR_FINGERPRINT_PASSWD_VERIFY_PASS:
                    szTemp = String.Format("MINOR_FINGERPRINT_PASSWD_VERIFY_PASS");
                    break;
                case CHCNetSDK.MINOR_FINGERPRINT_PASSWD_VERIFY_FAIL:
                    szTemp = String.Format("MINOR_FINGERPRINT_PASSWD_VERIFY_FAIL");
                    break;
                case CHCNetSDK.MINOR_FINGERPRINT_PASSWD_VERIFY_TIMEOUT:
                    szTemp = String.Format("MINOR_FINGERPRINT_PASSWD_VERIFY_TIMEOUT");
                    break;
                case CHCNetSDK.MINOR_FINGERPRINT_INEXISTENCE:
                    szTemp = String.Format("MINOR_FINGERPRINT_INEXISTENCE");
                    break;
                case CHCNetSDK.MINOR_CARD_PLATFORM_VERIFY:
                    szTemp = String.Format("MINOR_CARD_PLATFORM_VERIFY");
                    break;
                case CHCNetSDK.MINOR_CALL_CENTER:
                    szTemp = String.Format("MINOR_CALL_CENTER");
                    break;
                case CHCNetSDK.MINOR_FIRE_RELAY_TURN_ON_DOOR_ALWAYS_OPEN:
                    szTemp = String.Format("MINOR_FIRE_RELAY_TURN_ON_DOOR_ALWAYS_OPEN");
                    break;
                case CHCNetSDK.MINOR_FIRE_RELAY_RECOVER_DOOR_RECOVER_NORMAL:
                    szTemp = String.Format("MINOR_FIRE_RELAY_RECOVER_DOOR_RECOVER_NORMAL");
                    break;
                case CHCNetSDK.MINOR_FACE_AND_FP_VERIFY_PASS:
                    szTemp = String.Format("MINOR_FACE_AND_FP_VERIFY_PASS");
                    break;
                case CHCNetSDK.MINOR_FACE_AND_FP_VERIFY_FAIL:
                    szTemp = String.Format("MINOR_FACE_AND_FP_VERIFY_FAIL");
                    break;
                case CHCNetSDK.MINOR_FACE_AND_FP_VERIFY_TIMEOUT:
                    szTemp = String.Format("MINOR_FACE_AND_FP_VERIFY_TIMEOUT");
                    break;
                case CHCNetSDK.MINOR_FACE_AND_PW_VERIFY_PASS:
                    szTemp = String.Format("MINOR_FACE_AND_PW_VERIFY_PASS");
                    break;
                case CHCNetSDK.MINOR_FACE_AND_PW_VERIFY_FAIL:
                    szTemp = String.Format("MINOR_FACE_AND_PW_VERIFY_FAIL");
                    break;
                case CHCNetSDK.MINOR_FACE_AND_PW_VERIFY_TIMEOUT:
                    szTemp = String.Format("MINOR_FACE_AND_PW_VERIFY_TIMEOUT");
                    break;
                case CHCNetSDK.MINOR_FACE_AND_CARD_VERIFY_PASS:
                    szTemp = String.Format("MINOR_FACE_AND_CARD_VERIFY_PASS");
                    break;
                case CHCNetSDK.MINOR_FACE_AND_CARD_VERIFY_FAIL:
                    szTemp = String.Format("MINOR_FACE_AND_CARD_VERIFY_FAIL");
                    break;
                case CHCNetSDK.MINOR_FACE_AND_CARD_VERIFY_TIMEOUT:
                    szTemp = String.Format("MINOR_FACE_AND_CARD_VERIFY_TIMEOUT");
                    break;
                case CHCNetSDK.MINOR_FACE_AND_PW_AND_FP_VERIFY_PASS:
                    szTemp = String.Format("MINOR_FACE_AND_PW_AND_FP_VERIFY_PASS");
                    break;
                case CHCNetSDK.MINOR_FACE_AND_PW_AND_FP_VERIFY_FAIL:
                    szTemp = String.Format("MINOR_FACE_AND_PW_AND_FP_VERIFY_FAIL");
                    break;
                case CHCNetSDK.MINOR_FACE_AND_PW_AND_FP_VERIFY_TIMEOUT:
                    szTemp = String.Format("MINOR_FACE_AND_PW_AND_FP_VERIFY_TIMEOUT");
                    break;
                case CHCNetSDK.MINOR_FACE_CARD_AND_FP_VERIFY_PASS:
                    szTemp = String.Format("MINOR_FACE_CARD_AND_FP_VERIFY_PASS");
                    break;
                case CHCNetSDK.MINOR_FACE_CARD_AND_FP_VERIFY_FAIL:
                    szTemp = String.Format("MINOR_FACE_CARD_AND_FP_VERIFY_FAIL");
                    break;
                case CHCNetSDK.MINOR_FACE_CARD_AND_FP_VERIFY_TIMEOUT:
                    szTemp = String.Format("MINOR_FACE_CARD_AND_FP_VERIFY_TIMEOUT");
                    break;
                case CHCNetSDK.MINOR_EMPLOYEENO_AND_FP_VERIFY_PASS:
                    szTemp = String.Format("MINOR_EMPLOYEENO_AND_FP_VERIFY_PASS");
                    break;
                case CHCNetSDK.MINOR_EMPLOYEENO_AND_FP_VERIFY_FAIL:
                    szTemp = String.Format("MINOR_EMPLOYEENO_AND_FP_VERIFY_FAIL");
                    break;
                case CHCNetSDK.MINOR_EMPLOYEENO_AND_FP_VERIFY_TIMEOUT:
                    szTemp = String.Format("MINOR_EMPLOYEENO_AND_FP_VERIFY_TIMEOUT");
                    break;
                case CHCNetSDK.MINOR_EMPLOYEENO_AND_FP_AND_PW_VERIFY_PASS:
                    szTemp = String.Format("MINOR_EMPLOYEENO_AND_FP_AND_PW_VERIFY_PASS");
                    break;
                case CHCNetSDK.MINOR_EMPLOYEENO_AND_FP_AND_PW_VERIFY_FAIL:
                    szTemp = String.Format("MINOR_EMPLOYEENO_AND_FP_AND_PW_VERIFY_FAIL");
                    break;
                case CHCNetSDK.MINOR_EMPLOYEENO_AND_FP_AND_PW_VERIFY_TIMEOUT:
                    szTemp = String.Format("MINOR_EMPLOYEENO_AND_FP_AND_PW_VERIFY_TIMEOUT");
                    break;
                case CHCNetSDK.MINOR_FACE_VERIFY_PASS:
                    szTemp = String.Format("MINOR_FACE_VERIFY_PASS");
                    break;
                case CHCNetSDK.MINOR_FACE_VERIFY_FAIL:
                    szTemp = String.Format("MINOR_FACE_VERIFY_FAIL");
                    break;
                case CHCNetSDK.MINOR_EMPLOYEENO_AND_FACE_VERIFY_PASS:
                    szTemp = String.Format("MINOR_EMPLOYEENO_AND_FACE_VERIFY_PASS");
                    break;
                case CHCNetSDK.MINOR_EMPLOYEENO_AND_FACE_VERIFY_FAIL:
                    szTemp = String.Format("MINOR_EMPLOYEENO_AND_FACE_VERIFY_FAIL");
                    break;
                case CHCNetSDK.MINOR_EMPLOYEENO_AND_FACE_VERIFY_TIMEOUT:
                    szTemp = String.Format("MINOR_EMPLOYEENO_AND_FACE_VERIFY_TIMEOUT");
                    break;
                case CHCNetSDK.MINOR_FACE_RECOGNIZE_FAIL:
                    szTemp = String.Format("MINOR_FACE_RECOGNIZE_FAIL");
                    break;
                case CHCNetSDK.MINOR_FIRSTCARD_AUTHORIZE_BEGIN:
                    szTemp = String.Format("MINOR_FIRSTCARD_AUTHORIZE_BEGIN");
                    break;
                case CHCNetSDK.MINOR_FIRSTCARD_AUTHORIZE_END:
                    szTemp = String.Format("MINOR_FIRSTCARD_AUTHORIZE_END");
                    break;
                case CHCNetSDK.MINOR_DOORLOCK_INPUT_SHORT_CIRCUIT:
                    szTemp = String.Format("MINOR_DOORLOCK_INPUT_SHORT_CIRCUIT");
                    break;
                case CHCNetSDK.MINOR_DOORLOCK_INPUT_BROKEN_CIRCUIT:
                    szTemp = String.Format("MINOR_DOORLOCK_INPUT_BROKEN_CIRCUIT");
                    break;
                case CHCNetSDK.MINOR_DOORLOCK_INPUT_EXCEPTION:
                    szTemp = String.Format("MINOR_DOORLOCK_INPUT_EXCEPTION");
                    break;
                case CHCNetSDK.MINOR_DOORCONTACT_INPUT_SHORT_CIRCUIT:
                    szTemp = String.Format("MINOR_DOORCONTACT_INPUT_SHORT_CIRCUIT");
                    break;
                case CHCNetSDK.MINOR_DOORCONTACT_INPUT_BROKEN_CIRCUIT:
                    szTemp = String.Format("MINOR_DOORCONTACT_INPUT_BROKEN_CIRCUIT");
                    break;
                case CHCNetSDK.MINOR_DOORCONTACT_INPUT_EXCEPTION:
                    szTemp = String.Format("MINOR_DOORCONTACT_INPUT_EXCEPTION");
                    break;
                case CHCNetSDK.MINOR_OPENBUTTON_INPUT_SHORT_CIRCUIT:
                    szTemp = String.Format("MINOR_OPENBUTTON_INPUT_SHORT_CIRCUIT");
                    break;
                case CHCNetSDK.MINOR_OPENBUTTON_INPUT_BROKEN_CIRCUIT:
                    szTemp = String.Format("MINOR_OPENBUTTON_INPUT_BROKEN_CIRCUIT");
                    break;
                case CHCNetSDK.MINOR_OPENBUTTON_INPUT_EXCEPTION:
                    szTemp = String.Format("MINOR_OPENBUTTON_INPUT_EXCEPTION");
                    break;
                case CHCNetSDK.MINOR_DOORLOCK_OPEN_EXCEPTION:
                    szTemp = String.Format("MINOR_DOORLOCK_OPEN_EXCEPTION");
                    break;
                case CHCNetSDK.MINOR_DOORLOCK_OPEN_TIMEOUT:
                    szTemp = String.Format("MINOR_DOORLOCK_OPEN_TIMEOUT");
                    break;
                case CHCNetSDK.MINOR_FIRSTCARD_OPEN_WITHOUT_AUTHORIZE:
                    szTemp = String.Format("MINOR_FIRSTCARD_OPEN_WITHOUT_AUTHORIZE");
                    break;
                case CHCNetSDK.MINOR_CALL_LADDER_RELAY_BREAK:
                    szTemp = String.Format("MINOR_CALL_LADDER_RELAY_BREAK");
                    break;
                case CHCNetSDK.MINOR_CALL_LADDER_RELAY_CLOSE:
                    szTemp = String.Format("MINOR_CALL_LADDER_RELAY_CLOSE");
                    break;
                case CHCNetSDK.MINOR_AUTO_KEY_RELAY_BREAK:
                    szTemp = String.Format("MINOR_AUTO_KEY_RELAY_BREAK");
                    break;
                case CHCNetSDK.MINOR_AUTO_KEY_RELAY_CLOSE:
                    szTemp = String.Format("MINOR_AUTO_KEY_RELAY_CLOSE");
                    break;
                case CHCNetSDK.MINOR_KEY_CONTROL_RELAY_BREAK:
                    szTemp = String.Format("MINOR_KEY_CONTROL_RELAY_BREAK");
                    break;
                case CHCNetSDK.MINOR_KEY_CONTROL_RELAY_CLOSE:
                    szTemp = String.Format("MINOR_KEY_CONTROL_RELAY_CLOSE");
                    break;
                case CHCNetSDK.MINOR_EMPLOYEENO_AND_PW_PASS:
                    szTemp = String.Format("MINOR_EMPLOYEENO_AND_PW_PASS");
                    break;
                case CHCNetSDK.MINOR_EMPLOYEENO_AND_PW_FAIL:
                    szTemp = String.Format("MINOR_EMPLOYEENO_AND_PW_FAIL");
                    break;
                case CHCNetSDK.MINOR_EMPLOYEENO_AND_PW_TIMEOUT:
                    szTemp = String.Format("MINOR_EMPLOYEENO_AND_PW_TIMEOUT");
                    break;
                case CHCNetSDK.MINOR_HUMAN_DETECT_FAIL:
                    szTemp = String.Format("MINOR_HUMAN_DETECT_FAIL");
                    break;
                case CHCNetSDK.MINOR_PEOPLE_AND_ID_CARD_COMPARE_PASS:
                    szTemp = String.Format("MINOR_PEOPLE_AND_ID_CARD_COMPARE_PASS");
                    break;
                case CHCNetSDK.MINOR_PEOPLE_AND_ID_CARD_COMPARE_FAIL:
                    szTemp = String.Format("MINOR_PEOPLE_AND_ID_CARD_COMPARE_FAIL");
                    break;
                case CHCNetSDK.MINOR_CERTIFICATE_BLACK_LIST:
                    szTemp = String.Format("MINOR_CERTIFICATE_BLACK_LIST");
                    break;
                case CHCNetSDK.MINOR_LEGAL_MESSAGE:
                    szTemp = String.Format("MINOR_LEGAL_MESSAGE");
                    break;
                case CHCNetSDK.MINOR_ILLEGAL_MESSAGE:
                    szTemp = String.Format("MINOR_ILLEGAL_MESSAGE");
                    break;
                case CHCNetSDK.MINOR_MAC_DETECT:
                    szTemp = String.Format("MINOR_MAC_DETECT");
                    break;
                default:
                    szTemp = String.Format("Main Event unknown:"+"0x" + "stLogInfo.dwMinorType");
                    break;
            }
            szTemp.CopyTo(0, csTmp, 0, szTemp.Length);
            return;
        }
        // the main event type for this operation 
        void OperationMinorTypeMap(CHCNetSDK.NET_DVR_LOG_V30 stLogInfo, char[] csTmp)
        {
            String szTemp = null;
            char[] csParaType = new char[256];
            switch (stLogInfo.dwMinorType)
            {
                //operation
                case CHCNetSDK.MINOR_LOCAL_UPGRADE:
                    szTemp = String.Format("MINOR_LOCAL_UPGRADE");
                    break;
                case CHCNetSDK.MINOR_REMOTE_LOGIN:
                    szTemp = String.Format("REMOTE_LOGIN");
                    break;
                case CHCNetSDK.MINOR_REMOTE_LOGOUT:
                    szTemp = String.Format("REMOTE_LOGOUT");
                    break;
                case CHCNetSDK.MINOR_REMOTE_ARM:
                    szTemp = String.Format("REMOTE_ARM");
                    break;
                case CHCNetSDK.MINOR_REMOTE_DISARM:
                    szTemp = String.Format("REMOTE_DISARM");
                    break;
                case CHCNetSDK.MINOR_REMOTE_REBOOT:
                    szTemp = String.Format("REMOTE_REBOOT");
                    break;
                case CHCNetSDK.MINOR_REMOTE_UPGRADE:
                    szTemp = String.Format("REMOTE_UPGRADE");
                    break;
                case CHCNetSDK.MINOR_REMOTE_CFGFILE_OUTPUT:
                    szTemp = String.Format("REMOTE_CFGFILE_OUTPUT");
                    break;
                case CHCNetSDK.MINOR_REMOTE_CFGFILE_INTPUT:
                    szTemp = String.Format("REMOTE_CFGFILE_INTPUT");
                    break;
                case CHCNetSDK.MINOR_REMOTE_ALARMOUT_OPEN_MAN:
                    szTemp = String.Format("MINOR_REMOTE_ALARMOUT_OPEN_MAN");
                    break;
                case CHCNetSDK.MINOR_REMOTE_ALARMOUT_CLOSE_MAN:
                    szTemp = String.Format("MINOR_REMOTE_ALARMOUT_CLOSE_MAN");
                    break;
                case CHCNetSDK.MINOR_REMOTE_OPEN_DOOR:
                    szTemp = String.Format("MINOR_REMOTE_OPEN_DOOR");
                    break;
                case CHCNetSDK.MINOR_REMOTE_CLOSE_DOOR:
                    szTemp = String.Format("MINOR_REMOTE_CLOSE_DOOR");
                    break;
                case CHCNetSDK.MINOR_REMOTE_ALWAYS_OPEN:
                    szTemp = String.Format("MINOR_REMOTE_ALWAYS_OPEN");
                    break;
                case CHCNetSDK.MINOR_REMOTE_ALWAYS_CLOSE:
                    szTemp = String.Format("MINOR_REMOTE_ALWAYS_CLOSE");
                    break;
                case CHCNetSDK.MINOR_REMOTE_CHECK_TIME:
                    szTemp = String.Format("MINOR_REMOTE_CHECK_TIME");
                    break;
                case CHCNetSDK.MINOR_NTP_CHECK_TIME:
                    szTemp = String.Format("MINOR_NTP_CHECK_TIME");
                    break;
                case CHCNetSDK.MINOR_REMOTE_CLEAR_CARD:
                    szTemp = String.Format("MINOR_REMOTE_CLEAR_CARD"); ;
                    break;
                case CHCNetSDK.MINOR_REMOTE_RESTORE_CFG:
                    szTemp = String.Format("MINOR_REMOTE_RESTORE_CFG");
                    break;
                case CHCNetSDK.MINOR_ALARMIN_ARM:
                    szTemp = String.Format("MINOR_ALARMIN_ARM");
                    break;
                case CHCNetSDK.MINOR_ALARMIN_DISARM:
                    szTemp = String.Format("MINOR_ALARMIN_DISARM");
                    break;
                case CHCNetSDK.MINOR_LOCAL_RESTORE_CFG:
                    szTemp = String.Format("MINOR_LOCAL_RESTORE_CFG");
                    break;
                case CHCNetSDK.MINOR_MOD_NET_REPORT_CFG:
                    szTemp = String.Format("MINOR_MOD_NET_REPORT_CFG");
                    break;
                case CHCNetSDK.MINOR_MOD_GPRS_REPORT_PARAM:
                    szTemp = String.Format("MINOR_MOD_GPRS_REPORT_PARAM");
                    break;
                case CHCNetSDK.MINOR_MOD_REPORT_GROUP_PARAM:
                    szTemp = String.Format("MINOR_MOD_REPORT_GROUP_PARAM");
                    break;
                case CHCNetSDK.MINOR_UNLOCK_PASSWORD_OPEN_DOOR:
                    szTemp = String.Format("MINOR_UNLOCK_PASSWORD_OPEN_DOOR");
                    break;
                case CHCNetSDK.MINOR_REMOTE_CAPTURE_PIC:
                    szTemp = String.Format("MINOR_REMOTE_CAPTURE_PIC"); ;
                    break;
                case CHCNetSDK.MINOR_AUTO_RENUMBER:
                    szTemp = String.Format("MINOR_AUTO_RENUMBER");
                    break;
                case CHCNetSDK.MINOR_AUTO_COMPLEMENT_NUMBER:
                    szTemp = String.Format("MINOR_AUTO_COMPLEMENT_NUMBER");
                    break;
                case CHCNetSDK.MINOR_NORMAL_CFGFILE_INPUT:
                    szTemp = String.Format("MINOR_NORMAL_CFGFILE_INPUT");
                    break;
                case CHCNetSDK.MINOR_NORMAL_CFGFILE_OUTTPUT:
                    szTemp = String.Format("MINOR_NORMAL_CFGFILE_OUTTPUT");
                    break;
                case CHCNetSDK.MINOR_CARD_RIGHT_INPUT:
                    szTemp = String.Format("MINOR_CARD_RIGHT_INPUT");
                    break;
                case CHCNetSDK.MINOR_CARD_RIGHT_OUTTPUT:
                    szTemp = String.Format("MINOR_CARD_RIGHT_OUTTPUT");
                    break;
                case CHCNetSDK.MINOR_LOCAL_USB_UPGRADE:
                    szTemp = String.Format("MINOR_LOCAL_USB_UPGRADE");
                    break;
                case CHCNetSDK.MINOR_REMOTE_VISITOR_CALL_LADDER:
                    szTemp = String.Format("MINOR_REMOTE_VISITOR_CALL_LADDER"); ;
                    break;
                case CHCNetSDK.MINOR_REMOTE_HOUSEHOLD_CALL_LADDER:
                    szTemp = String.Format("MINOR_REMOTE_HOUSEHOLD_CALL_LADDER"); ;
                    break;
                default:
                    szTemp = String.Format("0x" + stLogInfo.dwMinorType);
                    break;
            }
            szTemp.CopyTo(0, csTmp, 0, szTemp.Length);
            return;
        }

        public void ProcessCommAlarmACS(ref CHCNetSDK.NET_DVR_ALARMER pAlarmer, IntPtr pAlarmInfo, uint dwBufLen, IntPtr pUser)
        {
            CHCNetSDK.NET_DVR_ACS_ALARM_INFO struAcsAlarmInfo = new CHCNetSDK.NET_DVR_ACS_ALARM_INFO();

            struAcsAlarmInfo = (CHCNetSDK.NET_DVR_ACS_ALARM_INFO)Marshal.PtrToStructure(pAlarmInfo, typeof(CHCNetSDK.NET_DVR_ACS_ALARM_INFO));
            CHCNetSDK.NET_DVR_LOG_V30 struFileInfo = new CHCNetSDK.NET_DVR_LOG_V30(); ;
            struFileInfo.dwMajorType = struAcsAlarmInfo.dwMajor;
            struFileInfo.dwMinorType = struAcsAlarmInfo.dwMinor;
            char[] csTmp = new char[256];

            if (CHCNetSDK.MAJOR_ALARM == struFileInfo.dwMajorType)
            {
                AlarmMinorTypeMap(struFileInfo, csTmp);
            }
            else if (CHCNetSDK.MAJOR_OPERATION == struFileInfo.dwMajorType)
            {
                OperationMinorTypeMap(struFileInfo, csTmp);
            }
            else if (CHCNetSDK.MAJOR_EXCEPTION == struFileInfo.dwMajorType)
            {
                ExceptionMinorTypeMap(struFileInfo, csTmp);
            }
            else if (CHCNetSDK.MAJOR_EVENT == struFileInfo.dwMajorType)
            {
                EventMinorTypeMap(struFileInfo, csTmp);
            }
            String szInfo = new String(csTmp).TrimEnd('\0');
            String szInfoBuf = null;
            char[] szNetUser = new char[struAcsAlarmInfo.sNetUser.GetLength(0) + 1];

            struAcsAlarmInfo.sNetUser.CopyTo(szNetUser, 0);

            szInfoBuf = string.Format("{0} {1,4}-{2,2}-{3} {4,2}:{5,2}:{6,2} [{7}]({8})", szInfo, struAcsAlarmInfo.struTime.dwYear, struAcsAlarmInfo.struTime.dwMonth,
                struAcsAlarmInfo.struTime.dwDay, struAcsAlarmInfo.struTime.dwHour, struAcsAlarmInfo.struTime.dwMinute, struAcsAlarmInfo.struTime.dwSecond,
                struAcsAlarmInfo.struRemoteHostAddr.sIpV4, System.Text.Encoding.UTF8.GetString(struAcsAlarmInfo.sNetUser).TrimEnd('\0'));
            if (struAcsAlarmInfo.struAcsEventInfo.byCardNo[0] != 0)
            {
                szInfoBuf = szInfoBuf + " Card Number:" + System.Text.Encoding.UTF8.GetString(struAcsAlarmInfo.struAcsEventInfo.byCardNo).TrimEnd('\0');
            }
            String[] szCardType = { "normal card", "disabled card", "blacklist card", "night watch card", "stress card", "super card", "guest card" };
            byte byCardType = struAcsAlarmInfo.struAcsEventInfo.byCardType;

            if (byCardType != 0 && byCardType <= szCardType.Length)
            {
                szInfoBuf = szInfoBuf + " Card Type:" + szCardType[byCardType - 1];
            }

            if (struAcsAlarmInfo.struAcsEventInfo.dwCardReaderNo != 0)
            {
                szInfoBuf = szInfoBuf + " Card Reader Number:" + struAcsAlarmInfo.struAcsEventInfo.dwCardReaderNo;
            }
            if (struAcsAlarmInfo.struAcsEventInfo.dwDoorNo != 0)
            {
                szInfoBuf = szInfoBuf + " Door Number:" + struAcsAlarmInfo.struAcsEventInfo.dwDoorNo;
            }
            if (struAcsAlarmInfo.struAcsEventInfo.dwVerifyNo != 0)
            {
                szInfoBuf = szInfoBuf + " Multiple Card Authentication Serial Number:" + struAcsAlarmInfo.struAcsEventInfo.dwVerifyNo;
            }
            if (struAcsAlarmInfo.struAcsEventInfo.dwAlarmInNo != 0)
            {
                szInfoBuf = szInfoBuf + " Alarm Input Number:" + struAcsAlarmInfo.struAcsEventInfo.dwAlarmInNo;
            }
            if (struAcsAlarmInfo.struAcsEventInfo.dwAlarmOutNo != 0)
            {
                szInfoBuf = szInfoBuf + " Alarm Output Number:" + struAcsAlarmInfo.struAcsEventInfo.dwAlarmOutNo;
            }
            if (struAcsAlarmInfo.struAcsEventInfo.dwCaseSensorNo != 0)
            {
                szInfoBuf = szInfoBuf + " Event Trigger Number:" + struAcsAlarmInfo.struAcsEventInfo.dwCaseSensorNo;
            }
            if (struAcsAlarmInfo.struAcsEventInfo.dwRs485No != 0)
            {
                szInfoBuf = szInfoBuf + " RS485 Channel Number:" + struAcsAlarmInfo.struAcsEventInfo.dwRs485No;
            }
            if (struAcsAlarmInfo.struAcsEventInfo.dwMultiCardGroupNo != 0)
            {
                szInfoBuf = szInfoBuf + " Multi Recombinant Authentication ID:" + struAcsAlarmInfo.struAcsEventInfo.dwMultiCardGroupNo;
            }
            if (struAcsAlarmInfo.struAcsEventInfo.byCardReaderKind != 0)
            {
                szInfoBuf = szInfoBuf + " CardReaderKind:" + struAcsAlarmInfo.struAcsEventInfo.byCardReaderKind.ToString();
            }
            if(struAcsAlarmInfo.dwPicDataLen>0)
            {
                Random rand = new Random(unchecked((int)DateTime.Now.Ticks));
                DateTime curTime = DateTime.Now; 
                string path = null;
                path=string.Format(@"C:/Picture/ACS_LocalTime{0}_{1}.bmp",szInfo,rand.Next());
                FileStream fs = new FileStream(path, FileMode.Create);
                int iLen= (int)struAcsAlarmInfo.dwPicDataLen;
                byte[] by = new byte[iLen];
                Marshal.Copy(struAcsAlarmInfo.pPicData, by, 0, iLen);
                fs.Write(by, 0, iLen);
                fs.Close();
                szInfoBuf = szInfoBuf + " SavePath:" + path;
            }
            g_formList.AddLog(m_iCurDeviceIndex, AcsDemoPublic.ALARM_INFO_T, szInfoBuf);
        }

        private void guardToolStripMenuItem_Click(object sender, EventArgs e)
        {
            int iCurDeviceIndex = m_iCurDeviceIndex;
            CHCNetSDK.NET_DVR_SETUPALARM_PARAM struSetupAlarmParam = new CHCNetSDK.NET_DVR_SETUPALARM_PARAM();
            struSetupAlarmParam.dwSize = (uint)Marshal.SizeOf(struSetupAlarmParam);
            struSetupAlarmParam.byLevel = 1;
            struSetupAlarmParam.byAlarmInfoType = 1;

            if (treeViewDevice.SelectedNode != null && iCurDeviceIndex >=0)
            {
                if (-1 == g_struDeviceInfo[iCurDeviceIndex].lFortifyHandle)
                {
                    g_struDeviceInfo[iCurDeviceIndex].lFortifyHandle = (int)CHCNetSDK.NET_DVR_SetupAlarmChan_V41(
                        (int)g_struDeviceInfo[iCurDeviceIndex].lLoginID, ref struSetupAlarmParam);
                    if (-1 == g_struDeviceInfo[iCurDeviceIndex].lFortifyHandle)
                    {
                        g_formList.AddLog(iCurDeviceIndex, AcsDemoPublic.OPERATION_FAIL_T, "NET_DVR_SetupAlarmChan_V41");
                        MessageBox.Show("Setup alarm chan failed");
                    }
                    else
                    {
                        MessageBox.Show("Setup alarm chan succeed");
                        guardToolStripMenuItem.Text = "Unguard";
                        g_formList.AddLog(iCurDeviceIndex, AcsDemoPublic.OPERATION_SUCC_T, "NET_DVR_SetupAlarmChan_V41");
                        m_falarmData = new CHCNetSDK.MSGCallBack(MsgCallback);
                        if (CHCNetSDK.NET_DVR_SetDVRMessageCallBack_V50(0,m_falarmData, IntPtr.Zero))
                        {
                            g_formList.AddLog(iCurDeviceIndex, AcsDemoPublic.OPERATION_SUCC_T, "NET_DVR_SetDVRMessageCallBack_V50 Succeed");
                        }
                        else
                        {
                            g_formList.AddLog(iCurDeviceIndex, AcsDemoPublic.OPERATION_FAIL_T, "NET_DVR_SetDVRMessageCallBack_V50 Fail");
                        }
                    }
                }
                else if (g_struDeviceInfo[iCurDeviceIndex].lFortifyHandle >= 0)
                {
                    if (CHCNetSDK.NET_DVR_CloseAlarmChan_V30((int)g_struDeviceInfo[iCurDeviceIndex].lFortifyHandle))
                    {
                        g_struDeviceInfo[iCurDeviceIndex].lFortifyHandle = -1;
                        g_formList.AddLog(iCurDeviceIndex, AcsDemoPublic.OPERATION_SUCC_T, "NET_DVR_CloseAlarmChan_V30");
                        guardToolStripMenuItem.Text = "Guard";
                        MessageBox.Show("Close alarm chan succeed");
                    }
                    else
                    {
                        g_formList.AddLog(iCurDeviceIndex, AcsDemoPublic.OPERATION_FAIL_T, "NET_DVR_CloseAlarmChan_V30");
                        MessageBox.Show("Close alarm chan failed");
                    }
                }

                UpdateTreeNode();
            }
        }

        private void loginoutToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if(LoginOut())
            {
                //just trigger draw node event
                treeViewDevice.SelectedNode.BackColor = Color.Empty;
                treeViewDevice.SelectedNode.Toggle();

                UpdateTreeNode();

                // disable guard item 
                ToolStripItem Ts = treeDeviceMenuStrip.Items[0];
                Ts.Enabled = false;
            }
        }

        private bool LoginOut()
        {
            //for (int i = 0; i < g_struDeviceInfo[m_iCurDeviceIndex].iDeviceChanNum; i++)
            //{
            //    if ((g_struDeviceInfo[m_iCurDeviceIndex].pStruChanInfo[i].lRealHandle >= 0 && i < CHCNetSDK.MAX_CHANNUM_V30 * 2) || g_struDeviceInfo[m_iCurDeviceIndex].pStruChanInfo[i].bLocalManualRec)
            //    {
            //        MessageBox.Show("Some channels of this device is recording or preview");
            //        return false;
            //    }
            //}
            
            if (g_struDeviceInfo[m_iCurDeviceIndex].lLoginID >= 0)
            {
                //remove guard
                if (g_struDeviceInfo[m_iCurDeviceIndex].lFortifyHandle >= 0)
                {
                    if (CHCNetSDK.NET_DVR_CloseAlarmChan_V30((int)g_struDeviceInfo[m_iCurDeviceIndex].lFortifyHandle))
                    {
                        g_struDeviceInfo[m_iCurDeviceIndex].lFortifyHandle = -1;
                    }
                }

                if (!CHCNetSDK.NET_DVR_Logout_V30((int)g_struDeviceInfo[m_iCurDeviceIndex].lLoginID))
                {
                    MessageBox.Show("Logout Failed");
                    return false;
                }
                g_formList.AddLog(m_iCurDeviceIndex, AcsDemoPublic.OPERATION_SUCC_T, "NET_DVR_Logout_V30");
            }
            g_struDeviceInfo[m_iCurDeviceIndex].lLoginID = -1;
            g_struDeviceInfo[m_iCurDeviceIndex].bPlayDevice = false;

            //for (int i = 0; i < g_struDeviceInfo[m_iCurDeviceIndex].iDeviceChanNum; i++)
            //{
            //    g_struDeviceInfo[m_iCurDeviceIndex].pStruChanInfo[i].lRealHandle = -1;
            //    g_struDeviceInfo[m_iCurDeviceIndex].pStruChanInfo[i].bAlarm = false;
            //    g_struDeviceInfo[m_iCurDeviceIndex].pStruChanInfo[i].bLocalManualRec = false;
            //}

            return true;
        }

        private void treeViewDevice_Leave(object sender, EventArgs e)
        {
            if (treeViewDevice.SelectedNode != null)
            {
                //let selected back view appear gray 
                treeViewDevice.SelectedNode.BackColor = Color.LightGray;
                //the foreground color to white 
                treeViewDevice.SelectedNode.ForeColor = Color.White;
            }
        }

        private void deleteToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (MessageBox.Show("Sure to delete this node", "Confirm Message", MessageBoxButtons.OKCancel) == DialogResult.OK)
            {
                if (LoginOut())
                {
                    g_struDeviceInfo[m_iCurDeviceIndex].chLoginUserName = null;
                    g_struDeviceInfo[m_iCurDeviceIndex].chLoginPwd = null;
                    g_struDeviceInfo[m_iCurDeviceIndex].chLocalNodeName = null; ;
                    g_struDeviceInfo[m_iCurDeviceIndex].chDeviceIP = null;
                    g_struDeviceInfo[m_iCurDeviceIndex].chDeviceMultiIP = null;
                    g_struDeviceInfo[m_iCurDeviceIndex].chSerialNumber = null;
                    g_struDeviceInfo[m_iCurDeviceIndex].lLoginID = -1;
                    g_struDeviceInfo[m_iCurDeviceIndex].lDevicePort = 8000;
                    g_struDeviceInfo[m_iCurDeviceIndex].iDeviceType = 0;
                    g_struDeviceInfo[m_iCurDeviceIndex].iDeviceLoginType = 0;
                    g_struDeviceInfo[m_iCurDeviceIndex].iDeviceChanNum = -1;
                    g_struDeviceInfo[m_iCurDeviceIndex].iDoorNum = -1;
                    treeViewDevice.Nodes.Remove(treeViewDevice.SelectedNode);
                }
            }
        }

        private void attributeToolStripMenuItem_Click(object sender, EventArgs e)
        {
            DeviceInfo deviceInfo = new DeviceInfo();
            deviceInfo.GetDeviceInfo(m_iCurDeviceIndex);
            if (deviceInfo.ShowDialog() == DialogResult.OK)
            {
                treeViewDevice.SelectedNode.Text = g_struDeviceInfo[m_iCurDeviceIndex].chLocalNodeName;
            }
        }

        private void openToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (treeViewDevice.SelectedNode != null)
            {
                // when this event triggered, the SelectedNode is door node
                int iDoorIndex = int.Parse(treeViewDevice.SelectedNode.Name) % 1000;

                System.Diagnostics.Debug.Assert(iDoorIndex >= 0);

                // 3 times index is door status and the other is card reader status
                g_struDeviceInfo[m_iCurDeviceIndex].iDoorStatus[3 * iDoorIndex] = AcsDemoPublic.DOOR_OPEN;


                // 1-represent open door ,iDoorIndex begin with 0,but the parameter begin with 1,thus here plus one
                if (CHCNetSDK.NET_DVR_ControlGateway(g_struDeviceInfo[m_iCurDeviceIndex].lLoginID, iDoorIndex + 1, 1))
                {
                    g_formList.AddLog(m_iCurDeviceIndex, AcsDemoPublic.OPERATION_SUCC_T, "open door");
                }
                else
                {
                    g_formList.AddLog(m_iCurDeviceIndex, AcsDemoPublic.OPERATION_FAIL_T, "NET_DVR_ControlGateway");
                    MessageBox.Show("NET_DVR_ControlGateway: open door failed");
                }
                UpdateTreeNode();
            }
        }

        private void closeToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (treeViewDevice.SelectedNode != null)
            {
                // when this event triggered, the SelectedNode is door node
                int iDoorIndex = int.Parse(treeViewDevice.SelectedNode.Name) % 1000;

                System.Diagnostics.Debug.Assert(iDoorIndex >= 0);

                g_struDeviceInfo[m_iCurDeviceIndex].iDoorStatus[3 * iDoorIndex] = AcsDemoPublic.DOOR_COLSED;

                //,iDoorIndex begin with 0,but the parameter begin with 1,thus here plus one
                if (CHCNetSDK.NET_DVR_ControlGateway(g_struDeviceInfo[m_iCurDeviceIndex].lLoginID, iDoorIndex + 1, 0))
                {
                    g_formList.AddLog(m_iCurDeviceIndex, AcsDemoPublic.OPERATION_SUCC_T, "close door");
                }
                else
                {
                    g_formList.AddLog(m_iCurDeviceIndex, AcsDemoPublic.OPERATION_FAIL_T, "NET_DVR_ControlGateway");
                    MessageBox.Show("NET_DVR_ControlGateway: close door failed");
                }
                UpdateTreeNode();
            }
        }

        private void alwaysOpenToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (treeViewDevice.SelectedNode != null)
            {
                // when this event triggered, the SelectedNode is door node
                int iDoorIndex = int.Parse(treeViewDevice.SelectedNode.Name) % 1000;

                System.Diagnostics.Debug.Assert(iDoorIndex >= 0);

                g_struDeviceInfo[m_iCurDeviceIndex].iDoorStatus[3 * iDoorIndex] = AcsDemoPublic.DOOR_ALWAYSOPEN;

                //,iDoorIndex begin with 0,but the parameter begin with 1,thus here plus one
                if (CHCNetSDK.NET_DVR_ControlGateway(g_struDeviceInfo[m_iCurDeviceIndex].lLoginID, iDoorIndex + 1, 2))
                {
                    g_formList.AddLog(m_iCurDeviceIndex, AcsDemoPublic.OPERATION_SUCC_T, "always open door");
                }
                else
                {
                    g_formList.AddLog(m_iCurDeviceIndex, AcsDemoPublic.OPERATION_FAIL_T, "NET_DVR_ControlGateway");
                    MessageBox.Show("NET_DVR_ControlGateway: always open door failed");
                }
                UpdateTreeNode();
            }
        }

        private void alwaysCloseToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (treeViewDevice.SelectedNode != null)
            {
                // when this event triggered, the SelectedNode is door node
                int iDoorIndex = int.Parse(treeViewDevice.SelectedNode.Name) % 1000;

                System.Diagnostics.Debug.Assert(iDoorIndex >= 0);

                g_struDeviceInfo[m_iCurDeviceIndex].iDoorStatus[3 * iDoorIndex] = AcsDemoPublic.DOOR_ALWAYSCOLSED;

                //,iDoorIndex begin with 0,but the parameter begin with 1,thus here plus one
                if (CHCNetSDK.NET_DVR_ControlGateway(g_struDeviceInfo[m_iCurDeviceIndex].lLoginID, iDoorIndex + 1, 3))
                {
                    g_formList.AddLog(m_iCurDeviceIndex, AcsDemoPublic.OPERATION_SUCC_T, "always close door");
                }
                else
                {
                    g_formList.AddLog(m_iCurDeviceIndex, AcsDemoPublic.OPERATION_FAIL_T, "NET_DVR_ControlGateway");
                    MessageBox.Show("NET_DVR_ControlGateway:always closed door failed");
                }
                UpdateTreeNode();
            }
        }


    }
}
