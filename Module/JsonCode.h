// JsonCode.h
#pragma once
#include "gclass.h"
//#include "AppClientInfo.h"
#include "json.h"
//#include "YYBBList.h"

class CJsonEncode :public CSingleServer<CJsonEncode>
{
public:
	//// 编码用户登录信息
	//wstring EnCodeUserLoginW(const CUserInfo &info, const string &strBarName, const string &strBarId, const list<CUserInfo> &ltUserInfo, const string &strFistLogin);
	//// 编码控制台登录信息
	//wstring EnCodeControlLoginW(const string &strBarId, const string &strBarName, const CUserInfo &info, const list<CUserInfo> &ltUserInfo);
	//// 编码其他用户登录信息
	//wstring EnCodeOtherUserLoginW(const CUserInfo &info);
	//// 编码其他用户登出信息
	//wstring EnCodeOtherUserLogOutW(const string &strUserNo, const string &strMachine);
	//// 编码即时消息
	//wstring EnCodeUserPChatW(const string &strUserNo, const string &strUserMachine, const string &strMsg, const string &strUserSex);
	//// 编码吧台指定弹幕
	//wstring EnCodeUserPDChatW(const string &strUserNo, const string &strMsg);
	//wstring EnCodeUserGChatW(const string &strUserNo, const string &strMsg, const string &strUserSex);
	//// 编码语音播报登录消息(nSex: 0 - 女; 1 - 男; 2 - 未知) 
	//string EnCodeUserLoginYYBB(const string &strMachine, int nSex = 0);
	//// 编码大神播报登录消息(strLevel:##) 
	//string EnCodeGodYYBB(const string &strMachine, const string &strLevel);
	//// 编码语音MP3播报消息
	//static string EnCodeYYBBMp3Message(const string &broadContent, const string &broadContent2);
	//// 编码语音TTS播报消息
	//static string EnCodeYYBBTTSMessage(const string &broadContent);
	//// 编码呼叫服务数据
	//wstring EnCodeCallServerW(const string &strMachineName);
	//// 控制台编码用户游戏数据
	//wstring EnCodeGameInfoW(const string &strMachineName, const string &strTier, const string &strLevel, const string &strWin, const string &strAreaId, const string &strUserQQId, const string &strNickName, const string &strSex, const string &strUserId);
	//// 控制台编码退出游戏数据
	//wstring EnCodeExitGameInfoW(const string &strMachineName, const string &strGameType, const string &strGameState);

	//// 控制台编码退出游戏数据
	//wstring EnCodeBeginGameInfoW(const string &strMachineName, const string &strGameType, const string &strGameState);
	//
	//// 编码比赛详情
	//static string EnCodeMatchDetail(const list<CMatchDetail> &ltDetail);
	//// 编码比赛数据
	//static string EnCodeMatchCommonData(const string &strAction, list<CMatchCommonInfo> &ltMatchInfo);
	//// 编码比赛数据 - WebSocket服务发来的数据, 发向应用层
	//static string EnCodeMatchToAppA(const string &strRecv);
	//static wstring EnCodeMatchToAppW(const string &strRecv);
	//static string EnCodeMatchToApp(const string &strActionName, const string &strRecv);
	//// 编码获取通用的上层json数据
	//static wstring EnCodeGetCommonToAppW(const string &strActionName, const string strRecv);

	//// 编码第三方接口数据
	//// 编码网吧信息
	//static wstring EnCodeThirdBarInfo();
	//// 编码用户信息
	//static wstring EnCodeThirdUserInfo();
	//// 编码扣费信息
	//static wstring EnCodeThirdChangeMoneyInfo();

	//// 1、内容：亲爱的鱼粉，本网咖为您精心准备了丰富的饮品和零食。如有需要请按服务铃！
	//static string EncodeDSBBMessage1();
	////2、亲爱的鱼粉，如果您渴了，请按桌上的服务铃，果汁咖啡统统都有哦！
	//static string EncodeDSBBMessage2();
	////3、亲爱的鱼粉，饿了别叫妈，按服务铃就行啦，服务员会飞奔过来哦！
	//static string EncodeDSBBMessage3();
	////4、亲爱的鱼粉，现在已是用餐时间，需要点餐的朋友请按服务铃，我们将竭诚为您服务。
	//static string EncodeDSBBMessage4();
	////5、亲爱的鱼粉，点餐时间到！baby喊你点餐啦！请狂击服务铃！
	//static string EncodeDSBBMessage5();
	////6、亲爱的鱼粉，为了您和他人的身体健康，请不要在网咖内吸烟，如有吸烟的顾客，请到指定的吸烟区！谢谢合作，祝您上网愉快！
	//static string EncodeDSBBMessage6();
	////7、大堂有很多妹子，为了妹子的健康，吸烟的小伙伴请去吸烟区吧。代表妹子感谢你们哦！
	//static string EncodeDSBBMessage7();
	////8、亲爱的鱼粉，游戏比赛再激烈，也要记得看管好，随身携带的贵重物品哦。
	//static string EncodeDSBBMessage8();
	////9、亲爱的鱼粉，请保管好您的贵重物品，手机钱包不要摆在桌面，以免丢失 ！谢谢合作，祝您上网愉快！
	//static string EncodeDSBBMessage9();
	////10、亲爱的鱼粉，我们开通了英雄联盟特权哦～全英雄全皮肤，10%经验加成，一般人我不告诉他。
	//static string EncodeDSBBMessage10();
	////11、亲爱的鱼粉，本网咖提供苹果和安卓充电线，如有需要请按服务铃哦。
	//static string EncodeDSBBMessage11();
	////12、各单位请注意，各单位请注意，正门走进一位漂亮妹子。
	//static string EncodeDSBBMessage12();
	////13、亲爱的鱼粉，对面的妹子要怎么搭讪？快找服务员小哥帮你送桶甜蜜的爆米花吧！
	//static string EncodeDSBBMessage13();
	////14、亲爱的鱼粉，身体是超神的本钱，打个广告，网鱼牌咖喱乌冬面，吃过都说好！
	//static string EncodeDSBBMessage14();
	////15、亲爱的鱼粉，手指太忙没空喝水吗？按下服务铃，饮品即刻奉上。
	//static string EncodeDSBBMessage15();
	////16、请您检查卡内余额。为了防止重复扣费，请不要随意更换机器。如有疑问，请联系吧台服务员。
	//static string EncodeDSBBMessage16();
	////17、亲爱的鱼粉，本网咖正在进行英雄联盟比赛，有丰厚奖品哦，赶快约上队友来吧台报名吧。
	//static string EncodeDSBBMessage17(const string &strGameName, const string &strGameType);
	////18、亲爱的鱼粉，本网咖将于10分钟后进行系统升级，麻烦您暂时下机，感谢你的配合与理解。
	//static string EncodeDSBBMessage18();
	////19、亲爱的鱼粉，经过小伙伴的不懈努力，店内的网络恢复啦，给大家带来不便深感抱歉。欢迎小伙伴们继续上机开黑，祝大家玩的开心！
	//static string EncodeDSBBMessage19();
	////20、欢迎光临网鱼网咖，一起来更精彩。
	//static string EncodeDSBBMessage20();
	////21、亲爱的鱼粉，本网咖通宵时间为晚上24点至次日早上8点，请您检查卡内余额。为了防止重复扣费，请不要随意更换机器。如有疑问，请联系吧台服务员。
	//static string EncodeDSBBMessage21(const string &strBeginTime, const string &strEndTime);
	//// 通用定时播报
	//static string EncodeDSBBCommMsg(const string &s);
	//// 增加前奏音
	//static string InsertYYBBPreVoice(const string &s);

private:
	CJsonEncode();
	~CJsonEncode();
	friend class CSingle<CJsonEncode>;

};

//struct LolTeamItem
//{
//	string strIndex;
//	string strTeamId;
//	string strName;
//	string strType;
//};
//
//struct UserInfoFromYServerItem
//{
//	string strUserId;
//	string strUserName;
//	string strBarId;
//	// 证件号码
//	string strCertId;
//	// 证件类型
//	string strCertType;
//	// 性别
//	string strSex;
//};
//
class CJsonDecode :public CSingleServer<CJsonDecode>
{
public:
	// 解析自升级配置
	static bool DeCodeSelfUpdateData(const string &strData, UpLevelDataItem &Item);
	// 解析游戏库
	static bool DeCodeGameData(const string &strData, list<GameLibItem> &lt);
	// 解析配置数据
	static bool DeCodeConfigData(const string &strData, ConfigDataItem &Item);
	// 解析配置数据2
	static bool DeCodeConfigIIData(const string &strData, ConfigIIItem &Item);
	//// 解析游戏比赛获取房间列表请求数据
	//static bool DeCodeGetRoomListReq(const json::Value &vl, CMatchCommonInfo &item);
	//// 解析游戏比赛创建房间列表请求数据
	//static bool DeCodeCreateRoomListReq(const json::Value &vl, CMatchCommonInfo &item);
	//// 解析游戏比赛修改房间列表请求数据
	//static bool DeCodeModifyRoomListReq(const json::Value &vl, CMatchCommonInfo &item);
	//// 解析语音播报获取定时列表请求数据
	//static bool DeCodeGetYYBBListRet(const string &strRecv, list<YYBBItem> &lt);
	//// 解析获取配置返回数据
	//static bool DeCodeGetBarConfigRes(const string &strR, ConfigItem &itemRes);
	//// 解析设置个人信息返回数据
	//static bool DeCodeUpdateUserRes(const string &strR);
	//// 解析登录服务应答数据
	//static void DeCodeLoginReq(const string &strInfo, string &strBarId, CUserInfo &Info);
	////Base64解码
	//static string DecodeBase64(const char* Data, int DataByte);
	//////解析LOL段位
	//static string DecodeLolLevelRes(const string &strUserInfo);
	//////解析控制台游戏信息
	//static bool DecodeControlGameInfoRes(const string &strUserInfo, string &strUserId, string &strTier, string &strLevel, string &strWin, string &strAreaId, string &strUserQQId, string &strNickName);
	//static string DecodeMatchGameInfoRes(const string &strUserInfo, const string &strUserMatchUser);
	//static bool DecodeLolInfo(string &strAction, const string &strJson, string &strUserId, string &strArea, string &strGameName, string &strTier, string &strMathInfo, string &strTeamList, string &strWinner, list<LolTeamItem> &lt);
	//static bool DecodeLolActionAndValues(const string &strJson, string &strAction, string &strValues);
	//// 解析从鱼服务中取来的用户ID
	//static bool DeCodeGetUserIDFromYS(const string &strJson, UserInfoFromYServerItem &Item);
	//static bool DeCodeGetUserInfoFromYS(const string &strJson, UserInfoFromYServerItem &Item);
	//// 解析从中心取来的用户金额
	//static bool DecodeUserMoney(const string &s, int &nMoney);
private:
	CJsonDecode();
	~CJsonDecode();
	friend class CSingle<CJsonDecode>;


};

namespace Base64
{
	static std::string const base64_chars =
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"abcdefghijklmnopqrstuvwxyz"
		"0123456789+/";

	/// Test whether a character is a valid base64 character
	/**
	* @param c The character to test
	* @return true if c is a valid base64 character
	*/
	static inline bool is_base64(unsigned char c) {
		return (c == 43 || // +
			(c >= 47 && c <= 57) || // /-9
			(c >= 65 && c <= 90) || // A-Z
			(c >= 97 && c <= 122)); // a-z
	}

	/// Encode a char buffer into a base64 string
	/**
	* @param input The input data
	* @param len The length of input in bytes
	* @return A base64 encoded string representing input
	*/
	inline std::string base64_encode(unsigned char const * input, size_t len) {
		std::string ret;
		int i = 0;
		int j = 0;
		unsigned char char_array_3[3];
		unsigned char char_array_4[4];

		while (len--) {
			char_array_3[i++] = *(input++);
			if (i == 3) {
				char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
				char_array_4[1] = ((char_array_3[0] & 0x03) << 4) +
					((char_array_3[1] & 0xf0) >> 4);
				char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) +
					((char_array_3[2] & 0xc0) >> 6);
				char_array_4[3] = char_array_3[2] & 0x3f;

				for (i = 0; (i < 4); i++) {
					ret += base64_chars[char_array_4[i]];
				}
				i = 0;
			}
		}

		if (i) {
			for (j = i; j < 3; j++) {
				char_array_3[j] = '\0';
			}

			char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
			char_array_4[1] = ((char_array_3[0] & 0x03) << 4) +
				((char_array_3[1] & 0xf0) >> 4);
			char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) +
				((char_array_3[2] & 0xc0) >> 6);
			char_array_4[3] = char_array_3[2] & 0x3f;

			for (j = 0; (j < i + 1); j++) {
				ret += base64_chars[char_array_4[j]];
			}

			while ((i++ < 3)) {
				ret += '=';
			}
		}

		return ret;
	}

	/// Encode a string into a base64 string
	/**
	* @param input The input data
	* @return A base64 encoded string representing input
	*/
	inline std::string base64_encode(std::string const & input) {
		return base64_encode(
			reinterpret_cast<const unsigned char *>(input.data()),
			input.size()
			);
	}

	/// Decode a base64 encoded string into a string of raw bytes
	/**
	* @param input The base64 encoded input data
	* @return A string representing the decoded raw bytes
	*/
	inline std::string base64_decode(std::string const & input) {
		size_t in_len = input.size();
		int i = 0;
		int j = 0;
		int in_ = 0;
		unsigned char char_array_4[4], char_array_3[3];
		std::string ret;

		while (in_len-- && (input[in_] != '=') && is_base64(input[in_])) {
			char_array_4[i++] = input[in_]; in_++;
			if (i == 4) {
				for (i = 0; i < 4; i++) {
					char_array_4[i] = static_cast<unsigned char>(base64_chars.find(char_array_4[i]));
				}

				char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
				char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
				char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

				for (i = 0; (i < 3); i++) {
					ret += char_array_3[i];
				}
				i = 0;
			}
		}

		if (i) {
			for (j = i; j < 4; j++)
				char_array_4[j] = 0;

			for (j = 0; j < 4; j++)
				char_array_4[j] = static_cast<unsigned char>(base64_chars.find(char_array_4[j]));

			char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
			char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
			char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

			for (j = 0; (j < i - 1); j++) {
				ret += static_cast<std::string::value_type>(char_array_3[j]);
			}
		}

		return ret;
	}
}