#ifndef _GCONST_H
#define _GCONST_H

//----------------------------------------------------------------------------
//-- 动作代码:

//客户端 -> 服务器端: (占用代码段: 1 .. 10000)
const int CS_DEBUG                      = 1;        //调试							
//const int CS_ACK                        = 2;        //应答服务器			------------------------------------	*
//const int CS_GET_USEROFFLINEFILE        = 3;        //获取用户离线文件信息	------------------------------------	*
const int CS_LOGIN                      = 4;        //登录					------------------------------------	*
const int CS_LOGOUT                     = 5;        //注销					------------------------------------	*
const int CS_KEEP_ALIVE                 = 6;        //保活					------------------------------------	*
//const int CS_USER_UPDATE                = 7;        //用户在线升级			------------------------------------	*
//const int CS_USER_VERSION				= 8;        //用户版本检查			------------------------------------	*
//const int CS_DEL_USEROFFLINEFILE        = 10;       //删除用户离线文件信息	------------------------------------	*
const int CS_REONLINE                   = 11;       //上线					------------------------------------	*
const int CS_OFFLINE                    = 12;       //下线					------------------------------------	*
//const int CS_GET_FRIEND_MSGS            = 13;       //获取和好友操作相关的信息(如被加好友通知等)
//const int CS_GET_ALL_USERS              = 14;       //获取所有好友/坏人
//const int CS_GET_ALL_ADDSELFFRIENDS     = 15;       //获取所有加自己为好友的用户
//const int CS_GET_USER_INFO              = 16;       //获取用户资料
//const int CS_MODIFY_USER_INFO           = 17;       //修改用户资料
//const int CS_GET_USER_OL_INFO           = 18;       //获取用户的在线信息
//const int CS_GET_XIAOXINGSHI_INFO       = 19;       //获取小信使信息
//const int CS_FIND_OL_USERS              = 20;       //看看谁在线上
//const int CS_FIND_USERS                 = 21;       //查找用户
const int CS_SEND_GROUPMSG              = 22;       //请求服务器转发群讯息
const int CS_FW_PACKET                  = 23;       //请求服务器转发私聊讯息
const int CS_GET_GROUPMSG               = 24;       //请求下载服务器群讯息
const int CS_GET_PERSONALMSG            = 25;       //请求下载服务器私聊讯息
//const int CS_SAVE_OFFLINE_MSGS          = 26;       //请求寄存的用户讯息
//const int CS_GET_OFFLINE_MSGS           = 27;       //获取寄存的用户讯息
//const int CS_GET_SYS_MSGS               = 28;       //获取系统消息
//const int CS_GET_AD_DATA                = 29;       //获取广告数据
//const int CS_GET_VERIFY_CODE            = 30;       //获取一个验证码
const int CS_GET_ONLINEUSER             = 31;       //获取在线用户
const int CS_DOWNLOAD_IMAGE				= 32;		//用户下载图片信息
const int CS_UPLOAD_IMAGE				= 33;		//用户上传图片信息
const int CS_GET_USER_MEMBERINFO		= 34;		//获取用户成员资料
const int CS_SET_USER_ONLINESTATE		= 35;		//设置用户在线状态
const int CS_GET_USERDISTROYMSG			= 36;		//用户获取历史消息
const int CS_RECVDATA					= 10;        // 收到客户端数据							
const int CS_GETCONTACT_ACK				= 37;		//用户应答获取联系人
const int CS_GROUPMSG_ACK				= 38;		//用户应答私聊消息
const int CS_USERMSG_ACK				= 39;		//用户应答群消息
const int CS_USEONLINENOTIFY_ACK		= 40;		//用户应答通知联系人上线

//
////服务器端 -> 客户端: (占用代码段: 10001 .. 20000)
const int SC_ACK                        = 10001;    //应答客户端
const int SC_LOGIN						= 10004;			//登录					------------------------------------	*
const int SC_LOGOUT						= 10005;        //注销					------------------------------------	*
const int SC_KEEP_ALIVE					= 10006;        //保活					------------------------------------	*
const int SC_RECVMSG					= 10007;    //服务器返回收到客户端消息
const int SC_SEND_CONTACT				= 10015;    //服务器发送联系人
const int SC_ONLINENOTIFY				= 10016;    //服务器发送联系人上线通知
const int SC_GET_USERDISTROYMSG			= 10017;	//用户获取历史消息
const int SC_UPLOAD_IMAGEFILE			= 10018;	//用户上传图片信息
const int SC_DOWNLOAD_IMAGEFILE			= 10019;	//用户下载图片信息
const int SC_GETGMSG_NOTIFY             = 10008;    //服务器发送群消息通知
const int SC_GETUMSG_NOTIFY             = 10009;    //服务器发送私聊消息通知
//const int SC_RE_KEEP_ALIVE              = 10005;    //服务器收到保活后返回的用户状态列表
//const int SC_OTHER_USER_BC              = 10006;    //其他用户的消息广播
//const int SC_FW_SEND_MSG                = 10007;    //转发用户讯息
//const int SC_SYS_MESSAGE                = 10008;    //系统消息
//const int SC_BE_ADDED_FRIEND            = 10009;    //通知用户被加好友
//const int SC_VERIFY_MSG                 = 10010;    //其他用户提交的身份验证信息
//const int SC_BE_PASSED_VERIFY           = 10011;    //通知用户加对方为好友时，对方已通过身份验证
//const int SC_RE_GET_FLT_WORDS           = 10012;    //应答 获取过滤关键词
//const int SC_FW_PACKET                  = 10013;    //转发数据包
//const int SC_SEND_GROUPMSG              = 10014;    //服务器转发群讯息
//
////=================================================================================================
////			C <==> C
//// 客户端 -> 客户端: (占用代码段: 20001 .. 30000)
////=================================================================================================
//const int CC_ACTIONCODE_BEGIN           = 20001;    // 客户端动作代码起始数
//const int CC_SHACKHAND                  = 20001;    // 客户端握手请求
//const int CC_ACK_SHACKHAND              = 20002;    // 客户端握手请求应答
//const int CC_IMMSG						= 20003;    // 客户端即时信息
//const int CC_ACK     					= 20004;    // 客户端即时信息应答
//const int CC_GROUPMSG					= 20005;    // 客户端群即时信息
//const int CC_GROUPMSG_ACK				= 20006;    // 客户端群即时信息应答
//const int CC_NOTIFYONLINE				= 20007;    // 客户端上线通知
//const int CC_NOTIFYONLINE_ACK			= 20008;    // 客户端上线通知应答
//const int CC_SENDFILE_QUESTION			= 20009;    // 客户端请求在线文件发送
//const int CC_SENDFILE_ACK				= 20010;    // 客户端请求在线文件发送应答
//const int CC_RECVFILE_QUESTION			= 20011;    // 客户端请求在线文件接收
//const int CC_RECVFILE_ACK				= 20012;    // 客户端请求在线文件接收应答
//const int CC_REFUSEFILE_QUESTION		= 20013;    // 客户端拒绝在线文件接收
//const int CC_REFUSEFILE_ACK				= 20014;    // 客户端拒绝在线文件接收应答
//const int CC_ACTIONCODE_END             = 21000;    // 客户端动作代码结束数
//


////----------------------------------------------------------------------------
////-- 数组尺寸常量:
//const int MAX_SERVERNAME_SIZE           = 20;       // 服务名称 
//const int MAX_SERVERKIND_SIZE           = 20;       // 服务种类 
//
const int MAX_USERNAME_SIZE             = 31;
//const int MAX_NICKNAME_SIZE             = 30;
//const int MAX_PASSWORD_SIZE             = 24;
//const int MAX_EMAIL_SIZE                = 24;
//const int MAX_HOMEPAGE_SIZE             = 36;
//const int MAX_TEL_SIZE                  = 20;
//const int MAX_TRUENAME_SIZE             = 15;
//const int MAX_ISPNAME_SIZE              = 11;
//const int MAX_CAREER_SIZE               = 15;
//const int MAX_COUNTRY_SIZE              = 15;
//const int MAX_PROVINCE_SIZE             = 11;
//const int MAX_CITY_SIZE                 = 9;
//const int MAX_ADDRESS_SIZE              = 36;
//const int MAX_POSTCODE_SIZE             = 8;
//const int MAX_NOTE_SIZE                 = 60;
//
//const int MAX_PWD_PROT_CERT_NO_SIZE     = 48;
//const int MAX_PWD_PROT_QUESTION_SIZE    = 48;
//const int MAX_PWD_PROT_ANSWER_SIZE      = 48;
//const int MAX_PWD_PROT_EMAIL_SIZE       = 48;
//
const int MAX_HASHED_PWD_SIZE           = 28;
//const int MAX_SEARCH_TEXT_SIZE          = 32;
//const int MAX_FILENAME_SIZE             = 96;
//const int MAX_FILE_COMM_SIZE            = 64;
//const int MAX_EXIT_KEY_SIZE             = 20;
//const int MAX_SYS_MSG_SIZE              = 300;
//const int MAX_USER_BC_MSG_SIZE          = 200;
//const int MAX_ADD_FRI_VFY_MSG_SIZE      = 50;
//const int MAX_VERSION_SIZE              = 16;
//const int MAX_AD_HINT_SIZE              = 128;
//const int MAX_AD_URL_SIZE               = 128;
//const int MAX_AD_GROUP_NOTE_SIZE        = 64;
//const int MAX_AD_NOTE_SIZE              = 64;
//const int MAX_FILTER_WORD_SIZE          = 21;
//const int MAX_IP_SIZE                   = 15;
//
////----------------------------------------------------------------------------
//// 时间字符串格式的最大长度
//const int MAX_TIME_SIZE                 = 20;
////----------------------------------------------------------------------------
//// 获取在线好友的最大个数
//const int MAX_GETONLINEUSER_NUMBER		= 500;
////----------------------------------------------------------------------------
//// 存放离线信息最大字符数
//const int MAX_SAVEOFFLINEMSG_NUMBER		= 1600; // 
////----------------------------------------------------------------------------
//----------------------------------------------------------------------------
// 网吧编号(50个字节)
const int MAX_BARIDNAME_SIZE = 50;
// 网吧验证码(20个字节)
const int MAX_BARCHECKCODE_SIZE = 20;
// 游戏列表更新时间(20个字节)
const int MAX_GAMELISTUPDATETIME_SIZE = 20;
//----------------------------------------------------------------------------
// 机器在网吧中的唯一编号(20个字节)
const int MAX_MACHINENAME_SIZE = 20;


// 返回代码
#define RESULTT_SUCESS    0    // 成功
//#define LOGINRET_NOUSER    10   // 无此用户 

// 登录返回代码
#define LOGINRET_SUCESS    0    // 成功
#define LOGINRET_NOUSER    10   // 无此用户 
#define LOGINRET_ERRPASS   20   // 密码错误
#define LOGINRET_NOONLINE   30   // 该用户不在线

// 服务相关错误
#define SERVERERRORCODE_NOBARID				-2   // 无此用户 
#define SERVERERRORCODE_ERRORCHECKCODE		-3   // 校验码错误 
#define SERVERERRORCODE_EXCEPTION			-4   // 异常错误 
#define SERVERERRORCODE_MEMNEW  -5   // 内存分配错误 
#define SERVERERRORCODE_OTHERERR  -1   // 其它错误 

//// 相关业务返回代码
//// 数据超长(存放离线信息业务)
//#define RET_WORK_ERR_LENGTHOUT	100
//
//// 联系人类型
//#define CONTACTBASEDATA_TYPE_FRIEND		1		// 好友
//#define CONTACTBASEDATA_TYPE_FRIENDED	2		// 加我为好友的用户
#define PROTOCOL_COMMON_SPLIT_CHAR   '\3'   // 协议通用分割符
#define PROTOCOL_COMMON_SPLIT_CHAR1   '|'   // 协议通用分割符1级
#define PROTOCOL_COMMON_SPLIT_LCHAR   '{'   // 协议通用复合左分割符
#define PROTOCOL_COMMON_SPLIT_RCHAR   '}'   // 协议通用复合右分割符

//----------------------------------------------------------------------------
 
#endif
