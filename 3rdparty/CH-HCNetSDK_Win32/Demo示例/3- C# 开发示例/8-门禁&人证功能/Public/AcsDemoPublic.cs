using System;
using System.Runtime.InteropServices;
using System.Collections.Generic;
using System.Text;
using System.IO;
using System.Runtime.Serialization;
using System.Web.Script.Serialization;

namespace AcsDemo.Public
{
    public enum SynInfoCommand
    {
        NO_DEFINE,
        CARD_INFO_SYN,
        USER_INFO_SYN
    }

    public class UserInfo
    {
        public string memberNo { get; set; }
        public string memberName { get; set; }
        public string cardNum { get; set; }
        public string fingerPrintNum { get; set; }

        public CardInfo[] cards { get; set; }
    }

    public class CardInfo
    {
        public string cardSerialNumber { get; set; }
        public string cardNumber { get; set; }
        public string cardType { get; set; }
        public string cardPassword { get; set; }
        public string cardValidityPeriod { get; set; }
        public string fingerSerialNumber { get; set; }
        public string fingerPrintDescriptions { get; set; }
    }

    /// <summary>
    /// before using System.Web.Script.Serialization,need to add System.Web.Extensions program repository
    /// </summary>
    public class JsonConvert_test
    {
        public static string Serializer<T>(T t)
        {
            JavaScriptSerializer jsonSerialize = new JavaScriptSerializer();
            return jsonSerialize.Serialize(t);
        }

        public static T Deserialize<T>(string jsonString)
        {
            JavaScriptSerializer jsonSerialize = new JavaScriptSerializer();
            return (T)jsonSerialize.Deserialize<T>(jsonString);
        }


        // runtime using like following
        //[DataContract]
        //public class test
        //{
        //    [DataMember(Name = "d1")]
        //    public string d1 { get; set; }
        //    [DataMember(Name = "d2")]
        //    public string d2 { get; set; }
        //    [DataMember(Name = "d3")]
        //    public string d3 { get; set; }
        //}
        //using System.Runtime.Serialization.Json;
        //public static string SerializerIO<T>(T t)
        //{
        //    DataContractJsonSerializer ser = new DataContractJsonSerializer(typeof(T));
        //    using (MemoryStream ms = new MemoryStream())
        //    {
        //        ser.WriteObject(ms, t);
        //        string jsonString = Encoding.UTF8.GetString(ms.ToArray());
        //        ms.Close();
        //        return jsonString;
        //    }
        //}

        //public static T DeserializeIO<T>(string jsonString)
        //{
        //    DataContractJsonSerializer ser = new DataContractJsonSerializer(typeof(T));
        //    using (MemoryStream ms = new MemoryStream(Encoding.UTF8.GetBytes(jsonString)))
        //    {
        //        T obj = (T)ser.ReadObject(ms);
        //        return obj;
        //    }
        //}

    }

    public class AcsDemoPublic
    {
        public static readonly string[] strDoorStatus = { "invalid", "sleep", "Normally open", "Normally close" };
        public static readonly string[] strVerify = {"invalid", "sleep", "card and password", "card", "card or password", "fingerprint", "fingerprint and password", "fingerprint or card",
                                                 "fingerprint and card", "fingerprint and card and password","face or fingerprint or card or password","face and fingerprint",
                                                 "face and password", "face and card", "face", "work number and password", "fingerprint or password", "work number and fingerprint",
                                                 "work number and fingerprint and password", "face and fingerprint and card", "face and password and fingerprint", "work number and face",
                                                 "face or face and swipe card"};
        public static readonly string[] strFingerType = { "Common fingerprint", "Stress fingerprint", "Patrol fingerprint", "Super fingerprint", "Dismissing fingerprint" };
        public static readonly string[] strCardType = { "unknown", "ordinary card", "disabled card", "black list card", "patrol card", "stress card", "super card", "guest card", "remove card", 
                                                          "employee card", "emergency card","emergency management card"};
        
        public const int MAX_DEVICES = 512;//max device number
        //bmp status
        public const int TREE_ALL = 0;//device list	
        public const int DEVICE_LOGOUT = 1;//device not log in
        public const int DEVICE_LOGIN = 2;//device login
        public const int DEVICE_FORTIFY = 3;//on guard
        public const int DEVICE_ALARM = 4;//alarm on device
        public const int DEVICE_FORTIFY_ALARM = 5;//onguard & alarm on device
        public const int DOOR_COLSED = 6;
        public const int DOOR_OPEN = 7;
        public const int CARD_ON_LINE = 8;   //preview & record & alarm
        public const int CARD_OFF_LINE = 9;	 //channel off-line
        public const int DOOR_ALWAYSCOLSED = 6;
        public const int DOOR_ALWAYSOPEN = 7;

        public const int CHAN_ORIGINAL = 6;//no preview, no record
        public const int CHAN_PLAY = 7;   //preview
        public const int CHAN_RECORD = 8;   //record
        public const int CHAN_PLAY_RECORD = 9;   //preview and record
        public const int CHAN_ALARM = 10;   //no preview, no record, only alarm
        public const int CHAN_PLAY_ALARM = 11;   //review, no record, with alarm info
        public const int CHAN_PLAY_RECORD_ALARM = 12;   //preview & record & alarm
        public const int CHAN_OFF_LINE = 13;	 //channel off-line

        public const int ALARM_INFO_T = 0;
        public const int OPERATION_SUCC_T = 1;
        public const int OPERATION_FAIL_T = 2;
        public const int PLAY_SUCC_T = 3;
        public const int PLAY_FAIL_T = 4;

        
        public const int REGIONTYPE = 0;
        public const int DEVICETYPE = 2;
        public const int CHANNELTYPE = 3;
        public const int DOORTYPE = 4;
        public const int CARDREADERTYPE = 6;
        public const int USERTYPE = 5;

        //batch
        public const int ZERO_CHAN_INDEX = 500;
        public const int MIRROR_CHAN_INDEX = 400;

        [StructLayout(LayoutKind.Sequential)]
        public struct LOCAL_LOG_INFO
        {
            public int iLogType;
            public string strTime;
            public string strLogInfo;
            public string strDevInfo;
            public string strErrInfo;
            public string strSavePath;
        }

        [StructLayout(LayoutKind.Sequential)]
        public struct PREVIEW_IFNO
        {
            public int iDeviceIndex;  	//device index
            public int iChanIndex;  	//channel index
            public byte PanelNo;
            public int lRealHandle;
            public IntPtr hPlayWnd;
        }

        public enum DEMO_CHANNEL_TYPE
        {
            DEMO_CHANNEL_TYPE_INVALID = -1,
            DEMO_CHANNEL_TYPE_ANALOG = 0,
            DEMO_CHANNEL_TYPE_IP = 1,
            DEMO_CHANNEL_TYPE_MIRROR = 2
        }


        [StructLayout(LayoutKind.Sequential)]
        public struct PASSIVEDECODE_CHANINFO
        {
            public Int32 lPassiveHandle;
            public Int32 lRealHandle;
            public Int32 lUserID;
            public Int32 lSel;
            public IntPtr hFileThread;
            public IntPtr hFileHandle;
            public IntPtr hExitThread;
            public IntPtr hThreadExit;
            public string strRecordFilePath;

            public void init()
            {
                lPassiveHandle = -1;
                lRealHandle = -1;
                lUserID = -1;
                lSel = -1;
                hFileThread = IntPtr.Zero;
                hFileHandle = IntPtr.Zero;
                hExitThread = IntPtr.Zero;
                hThreadExit = IntPtr.Zero;
                strRecordFilePath = null;
            }
        }

        [StructLayout(LayoutKind.Sequential)]
        public struct STRU_CHANNEL_INFO
        {
            public int iDeviceIndex;  	//device index
            public int iChanIndex;  	//channel index
            public DEMO_CHANNEL_TYPE iChanType;
            public int iChannelNO;         //channel NO.       
            [MarshalAsAttribute(UnmanagedType.ByValTStr, SizeConst = 100)]
            public string chChanName;         //channel name
            public uint dwProtocol;  	//network protocol
            public uint dwStreamType; //stream type，0-main stream，1-sub code stream，2-code stream 3，
            public uint dwLinkMode;//Stream connection: 0：TCP,1：UDP,2：multicast,3 - RTP，4-RTP/RTSP,5-RSTP/HTTP
            public bool bPassbackRecord; //0- disable the video comes back,1 enable the video comes back
            public uint dwPreviewMode;  //Preview mode 0-normal 1-delay 
            public int iPicResolution;    //resolution
            public int iPicQuality;    //image quality
            public Int32 lRealHandle;          //preview handle
            public bool bLocalManualRec;     //manual record
            public bool bAlarm;    //alarm
            public bool bEnable;  	//enable
            public uint dwImageType;  //channel status icon
            [MarshalAsAttribute(UnmanagedType.ByValTStr, SizeConst = 16)]
            public string chAccessChanIP;   //ip addr of IP channel
            public uint nPreviewProtocolType;	//stream protocol type 0-proprietary protocol 1-RTSP protocol
            public IntPtr pNext;

            public void init()
            {
                iDeviceIndex = -1;
                iChanIndex = -1;
                iChannelNO = -1;
                iChanType = DEMO_CHANNEL_TYPE.DEMO_CHANNEL_TYPE_INVALID;
                chChanName = null;
                dwProtocol = 0;

                dwStreamType = 0;
                dwLinkMode = 0;

                iPicResolution = 0;
                iPicQuality = 2;

                lRealHandle = -1;
                bLocalManualRec = false;
                bAlarm = false;
                bEnable = false;
                dwImageType = CHAN_ORIGINAL;
                chAccessChanIP = null;
                pNext = IntPtr.Zero;
                dwPreviewMode = 0;
                bPassbackRecord = false;
                nPreviewProtocolType = 0;
            }
        }

        [StructLayoutAttribute(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
        public struct STRU_DEVICE_INFO
        {
            public int iDeviceIndex;  	//device index
            public Int32 lLoginID;    //ID
            public uint dwDevSoftVer;  	//erserved
            [MarshalAsAttribute(UnmanagedType.ByValTStr, SizeConst = 100)]
            public string chLocalNodeName;      //local device node
            [MarshalAsAttribute(UnmanagedType.ByValTStr, SizeConst = 100)]
            public string chDeviceName;         //device name
            [MarshalAsAttribute(UnmanagedType.ByValTStr, SizeConst = 130)]
            public string chDeviceIP;           //device IP: IP,pppoe address, or network gate address, etc
            [MarshalAsAttribute(UnmanagedType.ByValTStr, SizeConst = 130)]
            public string chDeviceIPInFileName; //if chDeviceIP includes ':'(IPV6),change it to '.', for '.'is not allowed in window's file name
            [MarshalAsAttribute(UnmanagedType.ByValTStr, SizeConst = CHCNetSDK.NAME_LEN)]
            public string chLoginUserName;      //login user name
            [MarshalAsAttribute(UnmanagedType.ByValTStr, SizeConst = CHCNetSDK.NAME_LEN)]
            public string chLoginPwd;           //password
            [MarshalAsAttribute(UnmanagedType.ByValTStr, SizeConst = 130)]
            public string chDeviceMultiIP;      //multi-cast group address
            [MarshalAsAttribute(UnmanagedType.ByValTStr, SizeConst = 50)]
            public string chSerialNumber;       //SN
            public int iDeviceChanNum;      //channel numder  (analog + ip)
            public int iStartChan;    //start channel number
            public int iStartDChan;    //start IP channel number
            public int iDeviceType;  	//device type
            public int iDiskNum;    //HD number
            public Int32 lDevicePort;  	//port number
            public int iAlarmInNum;  	//alarm in number
            public int iAlarmOutNum;  	//alarm out number
            public int iAudioNum;    //voice talk number
            public int iAnalogChanNum;  	//analog channel number
            public int iIPChanNum;    //IP channel number
            public int iGroupNO;               //IP Group NO.
            public bool bCycle;    	//if this device is on cycle recording
            public bool bPlayDevice;  	//will be added later
            public bool bVoiceTalk;    //on voice talkor not
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = CHCNetSDK.MAX_AUDIO_V40)]
            public Int32[] lAudioHandle;         //voicebroadcast handle
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = CHCNetSDK.MAX_AUDIO_V40)]
            public bool[] bCheckBroadcast;      //Add to broad cast group
            public Int32 lFortifyHandle;  	//on guard handle
            public bool bAlarm;    	//whether there is alarm
            public int iDeviceLoginType;  //register mode  0 - fix IP   1- IPSERVER mode    2 -  domain name 
            public uint dwImageType;  	//device status icon
            public bool bIPRet;    	//support IP conection
            public int iEnableChanNum;  	//valid channel number
            public bool bDVRLocalAllRec;  //local recording
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = CHCNetSDK.MAX_AUDIO_V40)]
            public Int32[] lVoiceCom;            //voice transmit
            public Int32 lFirstEnableChanIndex;  	//first enabled channel index
            public Int32 lTranHandle;    //232 transparent channel handle
            public byte byZeroChanNum;  //Zero channel number
            public byte byMainProto;  	//main stream protocol type 0-Private, 1-rtp/tcp, 2-rtp/rtsp
            public byte bySubProto;    //sub stream protocol type 0-Private, 1-rtp/tcp, 2-rtp/rtsp
            public byte bySupport;             //ability
            public byte byStartDTalkChan;
            public byte byAudioInputChanNum;
            public byte byStartAudioInputChanNo;
            public byte byLanguageType;
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = CHCNetSDK.MAX_CHANNUM_V40, ArraySubType = UnmanagedType.Struct)]
            public STRU_CHANNEL_INFO[] pStruChanInfo; //channel structure
            public CHCNetSDK.NET_DVR_IPPARACFG_V40[] pStruIPParaCfgV40;    //IP channel parameters
            public CHCNetSDK.NET_DVR_IPALARMINCFG struAlarmInCfg;
            public CHCNetSDK.NET_DVR_IPALARMINCFG_V40 pStruIPAlarmInCfgV40;  // IP alarm In parameters
            public CHCNetSDK.NET_DVR_IPALARMOUTCFG_V40 pStruIPAlarmOutCfgV40; // IP alarm Out parameters
            public CHCNetSDK.NET_DVR_IPALARMOUTCFG struAlarmOutCfg;
            public IntPtr pNext;
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 16, ArraySubType = UnmanagedType.Struct)]
            public STRU_CHANNEL_INFO[] struZeroChan;
            [MarshalAsAttribute(UnmanagedType.ByValTStr, SizeConst = 16)]
            public string sSecretKey;
            public int iAudioEncType;
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 256, ArraySubType = UnmanagedType.Struct)]
            public PASSIVEDECODE_CHANINFO[] struPassiveDecode;
            public byte bySupport1;	//ability，0-no support，1-support
            // bySupport1 & 0x1, indicates whether or not the support with snmp v30
            //bySupport1 & 0x2, support to distinguish the playback and download	
            //bySupport1 & 0x4, support fortify priority	
            //bySupport1 & 0x8, smart devices support protection period extension
            //bySupport1 & 0x10, indicates whether or not the support multiple disk number (more than 33)
            //bySupport1 & 0x40 indicates whether or not the delay support preview
            public byte bySupport2; //ability to set extended, and 0-no support，1-support
            //bySupport2 & 0x1, indicates whether the decoder support via the URL decoding
            //bySupport2 & 0x2,  is support FTPV40
            //bySupport2 & 0x4,  is support ANR (broken network video)
            public byte byStartIPAlarmOutNo;  //starting IP number alarm output
            public byte byMirrorChanNum; //image channel number(recorded in the host is used to represent a director channel)
            public ushort wStartMirrorChanNo;  //The initial image channel number
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 16, ArraySubType = UnmanagedType.Struct)]
            public STRU_CHANNEL_INFO[] struMirrorChan;
            public byte bySupport5;
            public byte bySupport7;// bySupport7 & 0x2  indicates whether the device supports the IPC HVT model extension
            public byte byCharaterEncodeType;
            public bool bInit;
            public byte byPanelNo; //
            public int iDoorNum;
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 3 * CHCNetSDK.MAX_DOOR_NUM_256, ArraySubType = UnmanagedType.I1)]
            public int[] iDoorStatus;
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = CHCNetSDK.MAX_DOOR_NUM_256, ArraySubType = UnmanagedType.ByValTStr)]
            public string[] sDoorName; // door name

            public void Init()
            {
                iGroupNO = -1;
                iDeviceIndex = -1;
                lLoginID = -1;
                dwDevSoftVer = 0;
                chLocalNodeName = null;
                chDeviceName = null;
                chDeviceIP = null;
                chDeviceIPInFileName = null;
                //chDevNetCard1IP[0]	= '\0';
                chLoginUserName = null;
                chLoginPwd = null;
                chDeviceMultiIP = null;
                chSerialNumber = null;
                iDeviceChanNum = -1;
                iDoorNum = -1;
                iDoorStatus = new int[3 * CHCNetSDK.MAX_DOOR_NUM_256];
                sDoorName = new string[CHCNetSDK.MAX_DOOR_NUM_256];
                iStartChan = 0;
                iDeviceType = 0;
                iDiskNum = 0;
                lDevicePort = 8000;
                iAlarmInNum = 0;
                iAlarmOutNum = 0;
                iAnalogChanNum = 0;
                iIPChanNum = 0;
                byAudioInputChanNum = 0;
                byStartAudioInputChanNo = 0;
                bCycle = false;
                bPlayDevice = false;
                bVoiceTalk = false;
                //lAudioHandle = new Int32[CHCNetSDK.MAX_AUDIO_V40];
                //bCheckBroadcast = new bool[CHCNetSDK.MAX_AUDIO_V40];
                lFortifyHandle = -1;
                bAlarm = false;
                iDeviceLoginType = 0;
                dwImageType = DEVICE_LOGOUT;
                bIPRet = false;
                pNext = IntPtr.Zero;
                //lVoiceCom = new Int32[CHCNetSDK.MAX_AUDIO_V40];
                //for (int i = 0; i < lVoiceCom.Length; i++)
                //{
                //    lVoiceCom[i] = -1;
                //}
                lFirstEnableChanIndex = 0;
                lTranHandle = -1;
                byZeroChanNum = 0;
                //lAudioHandle[0] = -1;
                //lAudioHandle[1] = -1;
                //pStruChanInfo = new STRU_CHANNEL_INFO[CHCNetSDK.MAX_CHANNUM_V40];
                //struZeroChan = new STRU_CHANNEL_INFO[16];
                //struMirrorChan = new STRU_CHANNEL_INFO[16];
                //pStruIPAlarmInCfgV40 = new CHCNetSDK.NET_DVR_IPALARMINCFG_V40();
                //pStruIPAlarmOutCfgV40 = new CHCNetSDK.NET_DVR_IPALARMOUTCFG_V40();
                //struAlarmInCfg = new CHCNetSDK.NET_DVR_IPALARMINCFG();
                //struAlarmOutCfg = new CHCNetSDK.NET_DVR_IPALARMOUTCFG();
                sSecretKey = "StreamNotEncrypt";
                iAudioEncType = 0;
                bySupport1 = 0;
                bySupport2 = 0;
                bySupport5 = 0;
                bySupport7 = 0;
                byStartDTalkChan = 0;
                byLanguageType = 0;
                byCharaterEncodeType = 0;
                bInit = true;
                byPanelNo = 4;
            }
        }

        public static bool CheckState(CHCNetSDK.NET_DVR_DATE struItem)
        {
            if (struItem.wYear < 1970 || struItem.byMonth > 12 || struItem.byDay > 31)
            {
                return false;
            }
            return true;
        }
        public static bool CheckDate(CHCNetSDK.NET_DVR_SIMPLE_DAYTIME struItem)
        {
            if (struItem.byHour > 24 || struItem.byMinute > 59 || struItem.bySecond > 59)
            {
                return false;
            }
            return true;
        }

    }
}