#include "JsonCode.h"
#include "log.h"
#include "ConfigFile.h"
#include "atlenc.h"
#include "BarInfo.h"
//#include "UserOnlineInfo.h"
template <> CJsonEncode *CSingle<CJsonEncode>::m_pInstance = 0;

CJsonEncode::CJsonEncode()
{
}

CJsonEncode::~CJsonEncode()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////

template <> CJsonDecode *CSingle<CJsonDecode>::m_pInstance = 0;

CJsonDecode::CJsonDecode()
{
}

CJsonDecode::~CJsonDecode()
{
}

// 解析自升级配置
bool CJsonDecode::DeCodeSelfUpdateData(const string &strData, UpLevelDataItem &Item)
{
	// {"update_pack":{"ver":"201704200002","size":2429716,"url":"http://myb-upload.oss-cn-shanghai.aliyuncs.com/myoubox_201704200002.zip","md5":"6ed2bbad07b99640823c2a30a501f773"},"ret_msg":"success","ret_code":0}
	CLogJsonCode::WriteLog("DeCodeSelfUpdateData beg, src: %s", strData.c_str());
	bool bRet(false);
	int nRet = 0;
	auto vl = json::Deserialize(strData);
	if (vl.GetType() == json::ValueType::NULLVal)
	{
		CLogJsonCode::WriteLog("no json obj, src: %s", strData.substr(0, 100).c_str());
		return bRet;
	}
	if (!vl.HasKey("ret_code"))
	{
		// 失败
		CLogJsonCode::WriteLog("没有update_pack字段, src: %s", strData.substr(0, 100).c_str());
		return bRet;
	}
	if (vl["ret_code"].GetType() == json::ValueType::IntVal)
	{
		nRet = vl["ret_code"].ToInt();
		if (nRet != 0)
		{
			string strError = "Error";
			if (vl.HasKey("ret_msg"))
			{
				// 失败
				if (vl["ret_msg"].GetType() == json::ValueType::StringVal)
				{
					strError = vl["ret_msg"].ToString();
				}
			}
			CLogJsonCode::WriteLog("错误的升级配置: %s", strError.c_str());
			return bRet;
		}
	}
	if (!vl.HasKey("update_pack"))
	{
		// 失败
		CLogJsonCode::WriteLog("没有update_pack字段, src: %s", strData.substr(0, 100).c_str());
		return bRet;
	}
	auto vItem = vl["update_pack"];
	if (vItem.GetType() == json::ValueType::NULLVal)
	{
		CLogJsonCode::WriteLog("no json obj, src: %s", strData.substr(0, 100).c_str());
		return bRet;
	}
	if (vItem.HasKey("ver"))
	{
		if (vItem["ver"].GetType() == json::ValueType::StringVal)
		{
			Item.ver = vItem["ver"].ToString();
		}
	}
	if (vItem.HasKey("size"))
	{
		if (vItem["size"].GetType() == json::ValueType::IntVal)
		{
			Item.size = vItem["size"].ToInt();
		}
	}
	if (vItem.HasKey("url"))
	{
		if (vItem["url"].GetType() == json::ValueType::StringVal)
		{
			Item.url = vItem["url"].ToString();
		}
	}
	if (vItem.HasKey("md5"))
	{
		if (vItem["md5"].GetType() == json::ValueType::StringVal)
		{
			Item.md5 = vItem["md5"].ToString();
		}
	}
	return bRet;
}

// 解析游戏库
bool CJsonDecode::DeCodeGameData(const string &strData, list<GameLibItem> &lt)
{
	CLogJsonCode::WriteLog("DeCodeGameData beg, src len: %d", strData.size());
	bool bRet(false);
	auto vl = json::Deserialize(strData);
	if (vl.GetType() == json::ValueType::NULLVal)
	{
		CLogJsonCode::WriteLog("no json obj, src: %s", strData.substr(0, 100).c_str());
		return bRet;
	}
	//成员ID列表
	json::Array arrData = vl.ToArray();
	//json::Array arrData = vl["data"].ToArray();
	for (auto it : arrData)
	{
		GameLibItem item;
		if (!it.HasKey("center_game_id"))
		{
			// 失败
			CLogJsonCode::WriteLog("没有center_game_id字段, src: %s", strData.substr(0, 100).c_str());
			continue;
		}
		if (it["center_game_id"].IsNumeric())
		{
			item.center_game_id = it["center_game_id"].ToInt64();
		}
		if (it.HasKey("game_name"))
		{
			if (it["game_name"].GetType() == json::ValueType::StringVal)
			{
				item.game_name = it["game_name"].ToString();
			}
		}
		if (it.HasKey("game_dir"))
		{
			if (it["game_dir"].GetType() == json::ValueType::StringVal)
			{
				item.game_showname = it["game_dir"].ToString();
			}
		}
		if (it.HasKey("game_icon"))
		{
			if (it["game_icon"].GetType() == json::ValueType::StringVal)
			{
				item.game_icon = it["game_icon"].ToString();
			}
		}
		if (it.HasKey("category_dir"))
		{
			if (it["category_dir"].GetType() == json::ValueType::StringVal)
			{
				item.category_dir = it["category_dir"].ToString();
			}
		}
		if (it.HasKey("launcher"))
		{
			if (it["launcher"].GetType() == json::ValueType::StringVal)
			{
				item.launcher = it["launcher"].ToString();
			}
		}
		if (it.HasKey("name_py"))
		{
			if (it["name_py"].GetType() == json::ValueType::StringVal)
			{
				item.name_py = it["name_py"].ToString();
			}
		}
		if (it.HasKey("process"))
		{
			if (it["process"].GetType() == json::ValueType::StringVal)
			{
				item.process = it["process"].ToString();
			}
		}
		if (it.HasKey("game_dp"))
		{
			if (it["game_dp"].GetType() == json::ValueType::IntVal)
			{
				item.game_dp = it["game_dp"].ToInt();
			}
		}
		lt.push_back(item);
	}
	CLogJsonCode::WriteLog("DeCodeGameData end");
	return true;
}

// 解析配置数据
bool CJsonDecode::DeCodeConfigData(const string &strData, ConfigDataItem &Item)
{
	// {"data":[{"date":"2016-12-05","games":[{"game_name":"英雄联盟", "icon" : "http://static.myoubox.com/6e/e6/10001.6ee6ea10846f2bb4dc5de3d6fbbe6bde.png", "rank" : 900, "game_id" : 10001, "launcher" : "http://static.myoubox.com/d5/95/lol_launcher.d595837d74f28b3713e50a1597a4208f.zip"}, { "game_name":"守望先锋", "icon" : "http://static.myoubox.com/89/c4/10002.89c48d77026d5b50178ed0bdad2705ab.png", "rank" : 800, "game_id" : 10002, "launcher" : "http://static.myoubox.com/7a/54/lol_launcher.7a544390b9535f23fa3d87077b083692.zip" }], "update_packs" : [{"ver":"20161101000000", "size" : 115679, "type" : 1, "url" : "http://static.myoubox.com/d5/95/update_test1.d595837d74f28b3713e50a1597a4208f.zip"}], "programs" : [{"size":115679, "type" : 1, "url" : "http://static.myoubox.com/54/f1/sm1_test.54f1f58401a8a1abf67d408961e43b67.zip"}]}], "retCode" : 0, "retMsg" : "success" }	// 获取配置
	// {"data":[{"date":"2016-12-30","games":[{"game_name":null,"icon_md5":"","launcher_md5":"","icon":"","rank":10000,"game_id":11001,"launcher":""},{"game_name":null,"icon_md5":"","launcher_md5":"","icon":"","rank":998,"game_id":11002,"launcher":""},{"game_name":null,"icon_md5":"","launcher_md5":"","icon":"","rank":995,"game_id":11003,"launcher":""},{"game_name":null,"icon_md5":"","launcher_md5":"","icon":"","rank":30000,"game_id":11004,"launcher":""},{"game_name":null,"icon_md5":"","launcher_md5":"","icon":"","rank":999,"game_id":11005,"launcher":""},{"game_name":"QQ华夏","icon_md5":"","launcher_md5":"afe6f94702e7b1bd5002958b0f2ccdba","icon":"","rank":50000,"game_id":100644,"launcher":"http://myb-upload.oss-cn-shanghai.aliyuncs.com/SW_PlayPlay.zip"},{"game_name":"QQ西游","icon_md5":"","launcher_md5":"afe6f94702e7b1bd5002958b0f2ccdba","icon":"","rank":20000,"game_id":100679,"launcher":"http://myb-upload.oss-cn-shanghai.aliyuncs.com/SW_PlayPlay.zip"}]}],"ret_msg":"success","ret_code":0,"config":{"resources":[{"size":20984703,"type":1,"url":"http://myb-upload.oss-cn-shanghai.aliyuncs.com/game_icons.zip","md5":"2cf0c87d74f333de1e6935c0ca82d207"}],"talking_url":"http://106.14.45.195:80/","update_packs":[{"ver":"20161101000000","size":15413,"type":1,"url":"http://myb-upload.oss-cn-shanghai.aliyuncs.com/update1.zip","md5":"e1036d161a5e55fe8cbfba140dbbc8f2"}],"programs":[{"size":15413,"type":1,"url":"http://myb-upload.oss-cn-shanghai.aliyuncs.com/smallprogram1.zip","md5":"e1036d161a5e55fe8cbfba140dbbc8f2"}]}}

	//CLogJsonCode::WriteLog("DeCodeConfigData beg, src: %s", strData.substr(0, 100).c_str());
	CLogJsonCode::WriteLog("DeCodeConfigData beg, src: %s", strData.c_str());
	bool bRet(false);
	auto vl = json::Deserialize(strData);
	if (vl.GetType() == json::ValueType::NULLVal)
	{
		CLogJsonCode::WriteLog("no json obj, src: %s", strData.substr(0, 100).c_str());
		return bRet;
	}
	if (!vl.HasKey("ret_code") || !vl.HasKey("ret_msg") || !vl.HasKey("data"))
	{
		// 失败
		CLogJsonCode::WriteLog("不完整的json, src: %s", strData.substr(0, 100).c_str());
		return bRet;
	}

	if (vl["data"].GetType() != json::ValueType::ArrayVal)
	{
		// 失败
		CLogJsonCode::WriteLog("不符合格式的json, src: %s", strData.substr(0, 100).c_str());
		return bRet;
	}
	//成员ID列表
	json::Array arrData = vl["data"].ToArray();
	for (auto it : arrData)
	{
		SortDataItem item;
		if (!it.HasKey("date"))
		{
			// 失败
			CLogJsonCode::WriteLog("没有date字段, src: %s", strData.substr(0, 100).c_str());
			continue;
		}
		if (it["date"].GetType() == json::ValueType::StringVal)
		{
			item.strDate = it["date"].ToString();
		}

		if (it.HasKey("games"))
		{
			// 排位库数据
			json::Array arrSData = it["games"].ToArray();
			for (auto its : arrSData)
			{
				SortGameItem items;
				if (its.HasKey("game_id"))
				{
					if (its["game_id"].GetType() == json::ValueType::Int64Val || its["game_id"].GetType() == json::ValueType::IntVal)
					{
						items.game_id = its["game_id"].ToInt64();
					}
				}
				if (its.HasKey("game_name"))
				{
					if (its["game_name"].GetType() == json::ValueType::StringVal)
					{
						items.game_name = its["game_name"].ToString();
					}
				}
				if (its.HasKey("icon"))
				{
					if (its["icon"].GetType() == json::ValueType::StringVal)
					{
						items.icon = its["icon"].ToString();
					}
				}
				if (its.HasKey("icon_md5"))
				{
					if (its["icon_md5"].GetType() == json::ValueType::StringVal)
					{
						items.iconmd5 = its["icon_md5"].ToString();
					}
				}
				if (its.HasKey("launcher"))
				{
					if (its["launcher"].GetType() == json::ValueType::StringVal)
					{
						items.launcher = its["launcher"].ToString();
					}
				}
				if (its.HasKey("launcher_md5"))
				{
					if (its["launcher_md5"].GetType() == json::ValueType::StringVal)
					{
						items.launchermd5 = its["launcher_md5"].ToString();
					}
				}
				if (its.HasKey("rank"))
				{
					if (its["rank"].GetType() == json::ValueType::IntVal)
					{
						items.rank = its["rank"].ToInt();
					}
				}
				if (its.HasKey("flag"))
				{
					if (its["flag"].GetType() == json::ValueType::IntVal)
					{
						items.flag = its["flag"].ToInt();
					}
				}
				if (its.HasKey("pop_url"))
				{
					if (its["pop_url"].GetType() == json::ValueType::StringVal)
					{
						items.popurl = its["pop_url"].ToString();
					}
				}
				if (its.HasKey("pop_url_md5"))
				{
					if (its["pop_url_md5"].GetType() == json::ValueType::StringVal)
					{
						items.popmd5 = its["pop_url_md5"].ToString();
					}
				}
				item.ltGame.push_back(items);
			}
		}
		Item.ltSD.push_back(item);
	}

	if (vl.HasKey("config"))
	{
		if (vl["config"].HasKey("update_packs"))
		{
			list<UpLevelDataItem> ltUD;
			// 升级包数据
			json::Array arrUData = vl["config"]["update_packs"].ToArray();
			for (auto itu : arrUData)
			{
				UpLevelDataItem itemu;
				if (itu.HasKey("size"))
				{
					if (itu["size"].GetType() == json::ValueType::IntVal)
					{
						itemu.size = itu["size"].ToInt();
					}
				}
				if (itu.HasKey("type"))
				{
					if (itu["type"].GetType() == json::ValueType::IntVal)
					{
						itemu.type = itu["type"].ToInt();
					}
				}
				if (itu.HasKey("url"))
				{
					if (itu["url"].GetType() == json::ValueType::StringVal)
					{
						itemu.url = itu["url"].ToString();
					}
				}
				if (itu.HasKey("ver"))
				{
					if (itu["ver"].GetType() == json::ValueType::StringVal)
					{
						itemu.ver = itu["ver"].ToString();
					}
				}
				if (itu.HasKey("md5"))
				{
					if (itu["md5"].GetType() == json::ValueType::StringVal)
					{
						itemu.md5 = itu["md5"].ToString();
					}
				}
				Item.ltUD.push_back(itemu);
			}
		}
		if (vl["config"].HasKey("programs"))
		{
			// 小程序数据
			json::Array arrPData = vl["config"]["programs"].ToArray();
			for (auto itp : arrPData)
			{
				ProgramDataItem itemp;
				if (itp.HasKey("size"))
				{
					if (itp["size"].GetType() == json::ValueType::IntVal)
					{
						itemp.size = itp["size"].ToInt();
					}
				}
				if (itp.HasKey("type"))
				{
					if (itp["type"].GetType() == json::ValueType::IntVal)
					{
						itemp.type = itp["type"].ToInt();
					}
				}
				if (itp.HasKey("url"))
				{
					if (itp["url"].GetType() == json::ValueType::StringVal)
					{
						itemp.url = itp["url"].ToString();
					}
				}
				if (itp.HasKey("md5"))
				{
					if (itp["md5"].GetType() == json::ValueType::StringVal)
					{
						itemp.md5 = itp["md5"].ToString();
					}
				}
				Item.ltPD.push_back(itemp);
			}
		}
		if (vl["config"].HasKey("resources"))
		{
			// 资源数据
			json::Array arrPData = vl["config"]["resources"].ToArray();
			for (auto itr : arrPData)
			{
				ResourceDataItem itemr;
				if (itr.HasKey("size"))
				{
					if (itr["size"].GetType() == json::ValueType::IntVal)
					{
						itemr.size = itr["size"].ToInt();
					}
				}
				if (itr.HasKey("type"))
				{
					if (itr["type"].GetType() == json::ValueType::IntVal)
					{
						itemr.type = itr["type"].ToInt();
					}
				}
				if (itr.HasKey("url"))
				{
					if (itr["url"].GetType() == json::ValueType::StringVal)
					{
						itemr.url = itr["url"].ToString();
					}
				}
				if (itr.HasKey("md5"))
				{
					if (itr["md5"].GetType() == json::ValueType::StringVal)
					{
						itemr.md5 = itr["md5"].ToString();
					}
				}
				Item.ltRD.push_back(itemr);
			}
		}
		if (vl["config"].HasKey("talking_url"))
		{
			if (vl["config"]["talking_url"].GetType() == json::ValueType::StringVal)
			{
				Item.talking_url = vl["config"]["talking_url"].ToString();
			}
		}
		if (vl["config"].HasKey("game_menu"))
		{
			if (vl["config"]["game_menu"].HasKey("ql_name"))
			{
				if (vl["config"]["game_menu"]["ql_name"].GetType() == json::ValueType::StringVal)
				{
					Item.game_menu = vl["config"]["game_menu"]["ql_name"].ToString();
					if (Item.game_menu.size() >= 40)
					{
						Item.game_menu = Item.game_menu.substr(0, 38);
					}
				}
			}
		}
	}
	CLogJsonCode::WriteLog("DeCodeConfigData end");
	return true;
}

// 解析配置数据2
bool CJsonDecode::DeCodeConfigIIData(const string &strData, ConfigIIItem &Item)
{
	CLogJsonCode::WriteLog("DeCodeConfigIIData beg, src: %s", strData.c_str());
	bool bRet(false);
	int nRet = 0;
	auto vl = json::Deserialize(strData);
	if (vl.GetType() == json::ValueType::NULLVal)
	{
		CLogJsonCode::WriteLog("no json obj, src: %s", strData.substr(0, 100).c_str());
		return bRet;
	}
	if (!vl.HasKey("ret_code"))
	{
		// 失败
		CLogJsonCode::WriteLog("没有ret_code字段, src: %s", strData.substr(0, 100).c_str());
		return bRet;
	}
	if (vl["ret_code"].GetType() == json::ValueType::IntVal)
	{
		nRet = vl["ret_code"].ToInt();
		if (nRet != 0)
		{
			string strError = "Error";
			if (vl.HasKey("ret_msg"))
			{
				// 失败
				if (vl["ret_msg"].GetType() == json::ValueType::StringVal)
				{
					strError = vl["ret_msg"].ToString();
				}
			}
			CLogJsonCode::WriteLog("错误的升级配置: %s", strError.c_str());
			return bRet;
		}
		//成员ID列表
		if (vl["data"].GetType() != json::ValueType::ArrayVal)
		{
			CLogJsonCode::WriteLog("没有配置策略");
			return true;
		}
		json::Array arrData = vl["data"].ToArray();
		for (auto it : arrData)
		{
			ConfigIIDataItem item;
			if (it.HasKey("type"))
			{
				if (it["type"].GetType() == json::ValueType::Int64Val || it["type"].GetType() == json::ValueType::IntVal)
				{
					item.type = it["type"].ToInt();
				}
			}
			if (it.HasKey("url"))
			{
				if (it["url"].GetType() == json::ValueType::StringVal)
				{
					item.url = it["url"].ToString();
				}
			}
			if (it.HasKey("md5"))
			{
				if (it["md5"].GetType() == json::ValueType::StringVal)
				{
					item.md5 = it["md5"].ToString();
				}
			}
			if (it.HasKey("size"))
			{
				if (it["size"].GetType() == json::ValueType::Int64Val || it["size"].GetType() == json::ValueType::IntVal)
				{
					item.size = it["size"].ToInt();
				}
			}
			if (it.HasKey("param"))
			{
				if (it["param"].GetType() == json::ValueType::StringVal)
				{
					item.param = it["param"].ToString();
				}
			}
			Item.lt.push_back(item);
		}
	}
	return true;
}
