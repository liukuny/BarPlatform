/******************************************************************************
* 版权所有 (C)2012, Sandongcun开发组
*
* 文件名称：imserver.cpp
* 内容摘要：主服务类
* 其它说明：
* 当前版本：v1.0
* 作    者：刘坤
* 完成日期：2012.6.10
* 修改记录：
* 1.
* 修改日期：
* 版 本 号：
* 修 改 人：
* 修改内容：
******************************************************************************/
#ifndef _SERVER_H
#define _SERVER_H

#include "datatypedef.h"

class CServer
{
public:
	CServer();
	~CServer();
	void Run();
private:
	// 
	bool m_bStop;
	// 初始化
	void init();
	void uninit();
private:
	// 加载配置信息
	bool LoadConfig();
};


#endif
