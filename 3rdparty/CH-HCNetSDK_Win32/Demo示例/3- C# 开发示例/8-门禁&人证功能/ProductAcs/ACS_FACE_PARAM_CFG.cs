using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using AcsDemo.Public;
using System.IO;

namespace AcsDemo.ProductAcs
{
    public partial class ACS_FACE_PARAM_CFG : Form
    {
        public ACS_FACE_PARAM_CFG()
        {
            InitializeComponent();
        }

        public int m_lServerID = 0;
        public int m_iDevIndex = 0;
        private int m_dwDataType = 0;
        private int IndexOfListview = 0;
        private List<CHCNetSDK.NET_DVR_FACE_PARAM_CFG> LpArr = new List<CHCNetSDK.NET_DVR_FACE_PARAM_CFG>();
        private int m_lSetFaceParamCfgHandle = -1;
        private int m_lGetFaceParamCfgHandle = -1;
        private CHCNetSDK.RemoteConfigCallback g_fSetFaceParamCallback = null;
        private CHCNetSDK.RemoteConfigCallback g_fGetFaceParamCallback = null;
        private DeviceLogList g_formList = DeviceLogList.Instance();
        private int m_dwSendIndex = 0;
        private CHCNetSDK.NET_DVR_FACE_PARAM_CFG m_lpNowSendFace = new CHCNetSDK.NET_DVR_FACE_PARAM_CFG();
        
        //private Boolean bFailedFaceInfo = false;

        private void btnAdd_Click(object sender, EventArgs e)
        {
            CHCNetSDK.NET_DVR_FACE_PARAM_CFG struFaceParamCfg = new CHCNetSDK.NET_DVR_FACE_PARAM_CFG();
            struFaceParamCfg.Init();
            UpdateFaceParamCfg(ref struFaceParamCfg);
            AddToFaceParamList(ref struFaceParamCfg);
        }

        void UpdateFaceParamCfg(ref CHCNetSDK.NET_DVR_FACE_PARAM_CFG struFaceParamCfg)
        {
            struFaceParamCfg.dwSize = (uint)Marshal.SizeOf(struFaceParamCfg);
            struFaceParamCfg.byFaceDataType = (Byte)comboBoxDataType.SelectedIndex;
            StrToByteArray(ref struFaceParamCfg.byCardNo, textBoxAssociatedCardNo.Text);
            GetCardReaderIndex(ref struFaceParamCfg.byEnableCardReader);
            struFaceParamCfg.byFaceID = Convert.ToByte(textBoxFaceID.Text.ToString());
            StrToByteArray(ref struFaceParamCfg.byRes, textBoxFacePath.Text);

            LpArr.Add(struFaceParamCfg);

            if(0==comboBoxDataType.SelectedIndex)
            {
                m_dwDataType = (int)CHCNetSDK.LONG_CFG_SEND_DATA_TYPE_ENUM.ENUM_ACS_SEND_DATA;
            }
            if(1==comboBoxDataType.SelectedIndex)
            {
                m_dwDataType = (int)CHCNetSDK.LONG_CFG_SEND_DATA_TYPE_ENUM.ENUM_ACS_INTELLIGENT_IDENTITY_DATA;
            }
        }

        private void AddToFaceParamList(ref CHCNetSDK.NET_DVR_FACE_PARAM_CFG struFaceParamCfg)
        {
            listViewFaceConfig.BeginUpdate();
            ListViewItem item = new ListViewItem();
            item.Text = (++IndexOfListview).ToString();
            item.SubItems.Add(System.Text.Encoding.Default.GetString(struFaceParamCfg.byCardNo));
            item.SubItems.Add(System.Text.Encoding.Default.GetString(struFaceParamCfg.byRes));
            item.SubItems.Add(struFaceParamCfg.byFaceID.ToString());
            item.SubItems.Add(struFaceParamCfg.byFaceDataType.ToString());
            item.SubItems.Add("/");

            listViewFaceConfig.Items.Add(item);
            listViewFaceConfig.EndUpdate();
        }

        private void GetCardReaderIndex(ref byte[]CardReader)
        {
            if (-1==checkedListBoxCardReader.SelectedIndex)
            {
                MessageBox.Show("Please select CardReadNo");
                return;
            }
            else
            {
                for(int i=0;i<checkedListBoxCardReader.Items.Count;++i)
                {
                    if(checkedListBoxCardReader.GetItemChecked(i))
                    {
                        CardReader[i] = 1;
                    }
                }
            }
        }

        private void btnClear_Click(object sender, EventArgs e)
        {
            IndexOfListview = 0;
            listViewFaceConfig.Items.Clear();
            LpArr.Clear();
            m_dwSendIndex = 0;
        }

        private void btnSet_Click(object sender, EventArgs e)
        {
            if(m_lSetFaceParamCfgHandle!=-1)
            {
                CHCNetSDK.NET_DVR_StopRemoteConfig(m_lSetFaceParamCfgHandle);
            }

            CHCNetSDK.NET_DVR_FACE_PARAM_COND StruCond =new CHCNetSDK.NET_DVR_FACE_PARAM_COND();
            StruCond.dwSize = (uint)Marshal.SizeOf(StruCond);
            StruCond.dwFaceNum = (uint)IndexOfListview;
            int dwSize=(int)StruCond.dwSize;
            IntPtr PtrStruCond=Marshal.AllocHGlobal(dwSize);
            Marshal.StructureToPtr(StruCond, PtrStruCond, true);
            g_fSetFaceParamCallback=new CHCNetSDK.RemoteConfigCallback(ProcessSetFaceParamCallback);
            m_lSetFaceParamCfgHandle = CHCNetSDK.NET_DVR_StartRemoteConfig(m_lServerID, CHCNetSDK.NET_DVR_SET_FACE_PARAM_CFG, PtrStruCond, dwSize, g_fSetFaceParamCallback, this.Handle);
            //int i =(int)CHCNetSDK.NET_DVR_GetLastError();
            if(-1==m_lSetFaceParamCfgHandle)
            {
                g_formList.AddLog(m_iDevIndex, AcsDemoPublic.OPERATION_FAIL_T, "NET_DVR_SET_FACE_PARAM_CFG failed");
            }
            else
            {
                g_formList.AddLog(m_iDevIndex, AcsDemoPublic.OPERATION_SUCC_T, "NET_DVR_SET_FACE_PARAM_CFG success");
            }
            Marshal.FreeHGlobal(PtrStruCond);

            m_dwSendIndex = 0;
            if(!SendFirstFace())
            {
                CHCNetSDK.NET_DVR_StopRemoteConfig(m_lSetFaceParamCfgHandle);
                m_lSetFaceParamCfgHandle = -1;
            }
        }

        private void ProcessSetFaceParamCallback(uint dwType, IntPtr lpBuffer, uint dwBufLen, IntPtr pUserData)
        {
            if(dwType==(uint)CHCNetSDK.NET_SDK_CALLBACK_TYPE.NET_SDK_CALLBACK_TYPE_STATUS)
            {
                int dwStatus = Marshal.ReadInt32(lpBuffer);
                switch(dwStatus)
                {
                    case (int)CHCNetSDK.NET_SDK_CALLBACK_STATUS_NORMAL.NET_SDK_CALLBACK_STATUS_PROCESSING:
                        g_formList.AddLog(m_iDevIndex, AcsDemoPublic.OPERATION_SUCC_T, "SetFaceParam PROCESSING");
                        SendNextFace();
                        break;
                    case (int)CHCNetSDK.NET_SDK_CALLBACK_STATUS_NORMAL.NET_SDK_CALLBACK_STATUS_FAILED:
                        g_formList.AddLog(m_iDevIndex, AcsDemoPublic.OPERATION_FAIL_T, "SetFaceParam Err:NET_SDK_CALLBACK_STATUS_FAILED");
                        SendNextFace();
                        break;
                    //下面的状态是关闭长连接
                    case (int)CHCNetSDK.NET_SDK_CALLBACK_STATUS_NORMAL.NET_SDK_CALLBACK_STATUS_SUCCESS:
                        g_formList.AddLog(m_iDevIndex, AcsDemoPublic.OPERATION_SUCC_T, "SetFaceParam SUCCESS");
                        CHCNetSDK.PostMessage(pUserData, CHCNetSDK.WM_MSG_SET_FACE_PARAM_FINISH, 0, 0);
                        break;
                    case (int)CHCNetSDK.NET_SDK_CALLBACK_STATUS_NORMAL.NET_SDK_CALLBACK_STATUS_EXCEPTION:
                        g_formList.AddLog(m_iDevIndex, AcsDemoPublic.OPERATION_SUCC_T, "SetFaceParam Exception");
                        CHCNetSDK.PostMessage(pUserData, CHCNetSDK.WM_MSG_SET_FACE_PARAM_FINISH, 0, 0);
                        break;
                    default:
                        g_formList.AddLog(m_iDevIndex, AcsDemoPublic.OPERATION_SUCC_T, "SetFaceParam SUCCESS");
                        CHCNetSDK.PostMessage(pUserData, CHCNetSDK.WM_MSG_SET_FACE_PARAM_FINISH, 0, 0);
                        break;
                }
            }
            else if(dwType==(uint)CHCNetSDK.NET_SDK_CALLBACK_TYPE.NET_SDK_CALLBACK_TYPE_DATA)
            {
                CHCNetSDK.NET_DVR_FACE_PARAM_STATUS StruCfg = new CHCNetSDK.NET_DVR_FACE_PARAM_STATUS();
                StruCfg = (CHCNetSDK.NET_DVR_FACE_PARAM_STATUS)Marshal.PtrToStructure(lpBuffer, typeof(CHCNetSDK.NET_DVR_FACE_PARAM_STATUS));
                Boolean SendOk = false;
                for(int i=0;i<CHCNetSDK.MAX_CARD_READER_NUM_512;++i)
                {
                    if(1==StruCfg.byCardReaderRecvStatus[i])
                    {
                        SendOk = true;
                        g_formList.AddLog(m_iDevIndex, AcsDemoPublic.OPERATION_SUCC_T, "SetFaceParam PROCESSING,CardNo:" + StruCfg.byCardNo.ToString());
                    }
                }

                if(false==SendOk)
                {
                    g_formList.AddLog(m_iDevIndex, AcsDemoPublic.OPERATION_FAIL_T, "SetFaceParam Failed, CardReaderRecvStatus" +
                        StruCfg.byCardReaderRecvStatus[0].ToString() + ",cardNo:" + StruCfg.byCardNo.ToString());
                    
                }

                SendNextFace();
            }
        }

        private void button1_Click(object sender, EventArgs e)
        {
            if(openFileFacePath.ShowDialog()==DialogResult.OK)
            {
                textBoxFacePath.Text = System.IO.Path.GetFullPath(openFileFacePath.FileName);
            }
        }

        private Boolean SendNextFace()
        {
            if(-1==m_lSetFaceParamCfgHandle)
            {
                return false;
            }

            m_dwSendIndex++;
            if(m_dwSendIndex>=listViewFaceConfig.Items.Count)
            {
                return true;
            }

            if(m_dwSendIndex>=LpArr.Count)
            {
                return false;
            }

            return SendFaceBinaryData();
        }

        private Boolean SendFaceBinaryData()
        {
            m_lpNowSendFace = LpArr[m_dwSendIndex];
            string FilePath = System.Text.Encoding.Default.GetString(m_lpNowSendFace.byRes);
            string temp = null;
            for (int i = 0; i < FilePath.Length;++i )
            {
                if(FilePath[i]!='\0')
                {
                    temp += FilePath[i];
                }
            }
            FilePath = temp;

            if (!File.Exists(FilePath))
            {
                MessageBox.Show("The Path of pic doesn't exist", "Error", MessageBoxButtons.OK);
                return false;
            }

            try
            {
                using (FileStream fs = new FileStream(FilePath, FileMode.Open))
                {
                    if (0 == fs.Length)
                    {
                        MessageBox.Show("The face picture is 0k,please input another picture!", "Error", MessageBoxButtons.OK);
                        return false;
                    }
                    if (200 * 1024 < fs.Length)
                    {
                        MessageBox.Show("The face picture is larger than 200k,please input another picture!", "Error", MessageBoxButtons.OK);
                        return false;
                    }

                    m_lpNowSendFace.dwFaceLen = (uint)fs.Length;
                    int Length = (int)m_lpNowSendFace.dwFaceLen;
                    byte[] by = new byte[Length];

                    m_lpNowSendFace.pFaceBuffer = Marshal.AllocHGlobal(Length);
                    fs.Read(by, 0, Length);
                    Marshal.Copy(by, 0, m_lpNowSendFace.pFaceBuffer, Length);
                    fs.Close();
                }
            }
            catch (IOException ep)
            {
                MessageBox.Show(ep.ToString(), "Error", MessageBoxButtons.OK);
            }

            uint dwSize = (uint)Marshal.SizeOf(m_lpNowSendFace);
            IntPtr ptrm_lpNowSendFace = Marshal.AllocHGlobal((int)dwSize);
            Marshal.StructureToPtr(m_lpNowSendFace, ptrm_lpNowSendFace, true);

            if (!CHCNetSDK.NET_DVR_SendRemoteConfig(m_lSetFaceParamCfgHandle, (uint)m_dwDataType, ptrm_lpNowSendFace, dwSize))
            {
                g_formList.AddLog(m_iDevIndex, AcsDemoPublic.OPERATION_FAIL_T, "Send Fail,CardNo:" + m_lpNowSendFace.byCardNo.ToString());
            }
            Marshal.FreeHGlobal(m_lpNowSendFace.pFaceBuffer);
            Marshal.FreeHGlobal(ptrm_lpNowSendFace);
            return true;
        }

        private Boolean SendFirstFace()
        {
            if(-1==m_lSetFaceParamCfgHandle)
            {
                return false;
            }
            if(listViewFaceConfig.Items.Count<1)
            {
                return false;
            }
            m_dwSendIndex = 0;
            return SendFaceBinaryData();
        }

        protected override void DefWndProc(ref Message m)
        {
            switch(m.Msg)
            {
                case CHCNetSDK.WM_MSG_SET_FACE_PARAM_FINISH:
                    OnMsgSetFaceParamCfgFinish();
                    break;
                case CHCNetSDK.WM_MSG_ADD_FACE_PARAM_TOLIST:
                    OnMsgAddFaceParamCfgToList(ref m);
                    break;
                case CHCNetSDK.WM_MSG_GET_FACE_PARAM_FINISH:
                    OnMsgGetFaceParamCfgFinish();
                    break;
                default:
                    base.DefWndProc(ref m);
                    break;
            }
        }

        private void OnMsgGetFaceParamCfgFinish()
        {
            CHCNetSDK.NET_DVR_StopRemoteConfig(m_lGetFaceParamCfgHandle);
            m_lGetFaceParamCfgHandle = -1;
            g_formList.AddLog(m_iDevIndex, AcsDemoPublic.OPERATION_SUCC_T, "Get Finish");
        }

        private void OnMsgAddFaceParamCfgToList(ref Message m)
        {
            if(0==m.LParam.ToInt32())
            {
                //bFailedFaceInfo = false;
                IntPtr lpCardCfg = (IntPtr)m.WParam.ToInt32();
                CHCNetSDK.NET_DVR_FACE_PARAM_CFG CardCfg = new CHCNetSDK.NET_DVR_FACE_PARAM_CFG();
                CardCfg = (CHCNetSDK.NET_DVR_FACE_PARAM_CFG)Marshal.PtrToStructure(lpCardCfg, typeof(CHCNetSDK.NET_DVR_FACE_PARAM_CFG));
                Marshal.AllocHGlobal(lpCardCfg);

                if(0==CardCfg.dwSize)
                {
                    return;
                }
                AddToFaceParamList(ref CardCfg);
            }
            else
            {
                //bFailedFaceInfo = true;
                IntPtr lpInfo=(IntPtr)m.WParam.ToInt32();
                CHCNetSDK.NET_DVR_FAILED_FACE_INFO pInfo = new CHCNetSDK.NET_DVR_FAILED_FACE_INFO();
                pInfo = (CHCNetSDK.NET_DVR_FAILED_FACE_INFO)Marshal.PtrToStructure(lpInfo, typeof(CHCNetSDK.NET_DVR_FAILED_FACE_INFO));
                Marshal.AllocHGlobal(lpInfo);
                if(0==pInfo.dwSize)
                {
                    return;
                }

                AddToList(ref pInfo);
            }
        }

        private void AddToList(ref CHCNetSDK.NET_DVR_FAILED_FACE_INFO struFailedFaceInfo)
        {
            int InsertIndex = listViewFaceConfig.Items.Count+1;
            listViewFaceConfig.BeginUpdate();
            ListViewItem item = new ListViewItem();
            item.Text = InsertIndex.ToString();
            string StrCardNo = System.Text.Encoding.Default.GetString(struFailedFaceInfo.byCardNo);
            item.SubItems.Add(StrCardNo);
            item.SubItems.Add("/");
            item.SubItems.Add("/");
            item.SubItems.Add("/");
            item.SubItems.Add(struFailedFaceInfo.byErrorCode.ToString());
            listViewFaceConfig.Items.Add(item);
            listViewFaceConfig.EndUpdate();
        }

        private void OnMsgSetFaceParamCfgFinish()
        {
            if(!CHCNetSDK.NET_DVR_StopRemoteConfig(m_lSetFaceParamCfgHandle))
            {
                g_formList.AddLog(m_iDevIndex, AcsDemoPublic.OPERATION_FAIL_T, "NET_DVR_StopRemoteConfig failed");
            }
            m_lSetFaceParamCfgHandle = -1;
            g_formList.AddLog(m_iDevIndex, AcsDemoPublic.OPERATION_SUCC_T, "NET_DVR_SET_FACE_PARAM_CFG Set finish");
        }

        private void btnGet_Click(object sender, EventArgs e)
        {
            if(m_lGetFaceParamCfgHandle!=-1)
            {
                CHCNetSDK.NET_DVR_StopRemoteConfig(m_lGetFaceParamCfgHandle);
            }

            listViewFaceConfig.Items.Clear();
            LpArr.Clear();

            CHCNetSDK.NET_DVR_FACE_PARAM_COND struCond = new CHCNetSDK.NET_DVR_FACE_PARAM_COND();
            struCond.Init();
            struCond.dwSize = (uint)Marshal.SizeOf(struCond);
            struCond.dwFaceNum = (uint)IndexOfListview;
            struCond.byFaceID = Convert.ToByte(textBoxFaceID.Text.ToString());
            StrToByteArray(ref struCond.byCardNo, textBoxAssociatedCardNo.Text);
            GetCardReaderIndex(ref struCond.byEnableCardReader);

            int dwSize=Marshal.SizeOf(struCond);
            IntPtr ptrStruCond=Marshal.AllocHGlobal(dwSize);
            Marshal.StructureToPtr(struCond,ptrStruCond,true);
            g_fGetFaceParamCallback = new CHCNetSDK.RemoteConfigCallback(ProcessGetFaceParam);
            m_lGetFaceParamCfgHandle = CHCNetSDK.NET_DVR_StartRemoteConfig(m_lServerID, CHCNetSDK.NET_DVR_GET_FACE_PARAM_CFG, ptrStruCond, dwSize, g_fGetFaceParamCallback, this.Handle);
            if(-1==m_lGetFaceParamCfgHandle)
            {
                g_formList.AddLog(m_iDevIndex, AcsDemoPublic.OPERATION_FAIL_T, "NET_DVR_GET_FACE_PARAM_CFG failed");
            }
            else
            {
                g_formList.AddLog(m_iDevIndex, AcsDemoPublic.OPERATION_SUCC_T, "NET_DVR_GET_FACE_PARAM_CFG success");
            }

            Marshal.FreeHGlobal(ptrStruCond);
        }

        private void ProcessGetFaceParam(uint dwType, IntPtr lpBuffer, uint dwBufLen, IntPtr pUserData)
        {
            if(dwType==(uint)CHCNetSDK.NET_SDK_CALLBACK_TYPE.NET_SDK_CALLBACK_TYPE_DATA)
            {
                CHCNetSDK.NET_DVR_FACE_PARAM_CFG lpCardCfg = new CHCNetSDK.NET_DVR_FACE_PARAM_CFG();
                lpCardCfg = (CHCNetSDK.NET_DVR_FACE_PARAM_CFG)Marshal.PtrToStructure(lpBuffer, typeof(CHCNetSDK.NET_DVR_FACE_PARAM_CFG));

                Boolean bSendOk = false;
                for(int i=0;i<lpCardCfg.byEnableCardReader.Length;++i)
                {
                    if(1==lpCardCfg.byEnableCardReader[i])
                    {
                        bSendOk = true;
                        g_formList.AddLog(m_iDevIndex, AcsDemoPublic.OPERATION_SUCC_T, "GetFaceParam CardNo:" + lpCardCfg.byCardNo.ToString()
                            +"CardReader"+i.ToString());
                    }
                }

                if(false==bSendOk)
                {
                    g_formList.AddLog(m_iDevIndex, AcsDemoPublic.OPERATION_FAIL_T, "GetFaceParam Failed:" + lpCardCfg.byCardNo.ToString());
                }

                string szExt = "dat";
                if(1==lpCardCfg.byFaceDataType)
                {
                    szExt = "jpg";
                }

                string szPath = null;
                string ByCardNo = ByteArrayToStr(ref lpCardCfg.byCardNo);
                szPath = "c:\\picture\\" + ByCardNo + "_" + lpCardCfg.byFaceID.ToString() + "_facedata." + szExt;
                StrToByteArray(ref lpCardCfg.byRes, szPath);

                if (0==lpCardCfg.dwFaceLen)
                {
                    return;
                }
                try
                {
                    using(FileStream fs=new FileStream(szPath,FileMode.Create))
                    {
                        int Len = (int)lpCardCfg.dwFaceLen;
                        byte[] by = new byte[Len];
                        Marshal.Copy(lpCardCfg.pFaceBuffer, by, 0, Len);
                        fs.Write(by, 0, Len);
                     }
                }
                catch(IOException ep)
                {
                    MessageBox.Show(ep.ToString(), "Error", MessageBoxButtons.OK);
                }

                int dwSize = (int)lpCardCfg.dwSize;
                IntPtr ptrlpCardCfg = Marshal.AllocHGlobal(dwSize);
                Marshal.StructureToPtr(lpCardCfg, ptrlpCardCfg, true);
                CHCNetSDK.PostMessage(pUserData, CHCNetSDK.WM_MSG_ADD_FACE_PARAM_TOLIST, (int)ptrlpCardCfg,0);
            }
            else if(dwType==(uint)CHCNetSDK.NET_SDK_CALLBACK_TYPE.NET_SDK_CALLBACK_TYPE_STATUS)
            {
                int dwStatus = Marshal.ReadInt32(lpBuffer);
                if (dwStatus == (int)CHCNetSDK.NET_SDK_CALLBACK_STATUS_NORMAL.NET_SDK_CALLBACK_STATUS_SUCCESS)
                {
                    CHCNetSDK.PostMessage(pUserData, CHCNetSDK.WM_MSG_GET_FACE_PARAM_FINISH, 0, 0);
                }
                else if((int)CHCNetSDK.NET_SDK_CALLBACK_STATUS_NORMAL.NET_SDK_CALLBACK_STATUS_FAILED==dwStatus)
                {
                    int dwErrCode = 0;
                    if(8==dwBufLen)
                    {
                        dwErrCode = Marshal.ReadInt32(lpBuffer + 1);
                    }
                    g_formList.AddLog(m_iDevIndex, AcsDemoPublic.OPERATION_FAIL_T, "GetFace STATUS_FAILED, Error code" + dwErrCode.ToString());
                }
            }

        }

        public static void StrToByteArray(ref byte[] destination, string data)
        {
            if (data != "")
            {
                byte[] source = System.Text.Encoding.Default.GetBytes(data);
                if (source.Length > destination.Length)
                {
                    MessageBox.Show("The length of string data is exceeding");
                }
                else
                {
                    for(int i=0;i<source.Length;++i)
                    {
                        destination[i] = source[i];
                    }

                }
            }
        }

        public static string ByteArrayToStr(ref byte[]Source)
        {
            string temp = System.Text.Encoding.Default.GetString(Source);
            string res = null;
            for(int i=0;i<temp.Length;++i)
            {
                if(temp[i]!='\0')
                {
                    res += temp[i];
                }
            }
            return res;
        }

        private void label7_Click(object sender, EventArgs e)
        {

        }

        private void button1_Click_1(object sender, EventArgs e)
        {
            if(0==comboBoxDeleteMeth.SelectedIndex)
            {
                CHCNetSDK.NET_DVR_FACE_PARAM_CTRL_ByCard struDelCfg = new CHCNetSDK.NET_DVR_FACE_PARAM_CTRL_ByCard();
                struDelCfg.Init();
                struDelCfg.dwSize = (int)Marshal.SizeOf(struDelCfg);
                struDelCfg.byMode = 0;
                StrToByteArray(ref struDelCfg.struProcessMode.byCardNo, textBoxAssociatedCardNo.Text);
                GetCardReaderIndex(ref struDelCfg.struProcessMode.byEnableCardReader);
                GetFaceIDWhenDelByCard(ref struDelCfg);
                
                int dwSize= struDelCfg.dwSize;
                IntPtr PtrStruDelCfg = Marshal.AllocHGlobal(dwSize);
                Marshal.StructureToPtr(struDelCfg, PtrStruDelCfg, true);
                Boolean res = CHCNetSDK.NET_DVR_RemoteControl(m_lServerID, CHCNetSDK.NET_DVR_DEL_FACE_PARAM_CFG, PtrStruDelCfg, (uint)dwSize);
                if(true==res)
                {
                    g_formList.AddLog(m_iDevIndex, AcsDemoPublic.OPERATION_SUCC_T, "NET_DVR_DEL_FACE_PARAM_CFG SUCC");
                }
                else
                {
                    g_formList.AddLog(m_iDevIndex, AcsDemoPublic.OPERATION_FAIL_T, "NET_DVR_DEL_FACE_PARAM_CFG FAILED");
                }
                Marshal.FreeHGlobal(PtrStruDelCfg);
            }
        }

        private void GetFaceIDWhenDelByCard(ref CHCNetSDK.NET_DVR_FACE_PARAM_CTRL_ByCard struDelCfg)
        {
            for (int i = 0; i < checkedListBoxDelByCardNo.Items.Count; ++i)
            {
                if (checkedListBoxCardReader.GetItemChecked(i))
                {
                    struDelCfg.struProcessMode.byFaceID[i] = 1;
                }
                else
                {
                    struDelCfg.struProcessMode.byFaceID[i] = 0;
                }
            }
        }
    }
}
