/******************************************************************************
* 版权所有 (C)2012, Sandongcun开发组
*
* 文件名称：packetbuf.h
* 内容摘要：缓存类 
* 其它说明：注意内存释放问题
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
#ifndef _PACKETBUF_H
#define _PACKETBUF_H
#include "gclass.h"
#include "netbase.h"

// 接收缓冲区元素格式 
typedef struct _RecvPacket
{
    CNetAddr stFromAddr;		// 注意，如果使用CNetAddr,会造成字节不对齐的结果，因为NetAddr是字节对齐的,除非再把CNetAddr再做字节对齐
    int nDataSize;  
    byte *pData;  
    _RecvPacket(): nDataSize(0), pData(0){}
    _RecvPacket(const CNetAddr &stFromAddr1, int nDataSize1, void *pData1)
    : stFromAddr(stFromAddr1)
    {
		nDataSize = nDataSize1;
        if(nDataSize1 > 0)
        {
            pData = new byte[nDataSize];
            memcpy(pData, pData1, nDataSize);
        }
    }
}RecvPacket, *PRecvPacket;

// 发送缓冲区元素格式 
typedef struct _SendPacket
{
    CNetAddr stToAddr;
    int nDataSize;  
    byte *pData;      
    _SendPacket(): nDataSize(0), pData(0){}
    _SendPacket(const CNetAddr &stToAddr1, int nDataSize1, void *pData1)
    : stToAddr(stToAddr1), nDataSize(nDataSize1)
    {
        if(nDataSize1 > 0)
        {
            pData = new byte[nDataSize];
            memcpy(pData, pData1, nDataSize);
        }
    }
}SendPacket, *PSendPacket;



// 接收缓存区
class CRecvBuf : public CQueueBuf<CRecvBuf, RecvPacket>
{
public:
    // 存入缓存 
    bool EnterBuf(const CNetAddr &stFromAddr, void *pData, int nLen)
    {
        RecvPacket *pItem = new RecvPacket(stFromAddr, nLen, pData);
        bool bRet = CQueueBuf<CRecvBuf, RecvPacket>::EnterBuf(pItem);
		//CLogError::WriteLog("CRecvBuf::EnterBuf() count: %d/%lld, pData: %lld, nLen: %d/%d", GetCount(), (int64)pItem, (int64)(char *)pItem->pData, /**/pItem->nDataSize, sizeof(RecvPacket), sizeof(CRecvBuf));

		return bRet;
    }
    // 从缓存中取出 
    void *LeaveBuf(CNetAddr &stFromAddr, int &nLen)    
    {
		time_t tm = time(NULL);
        RecvPacket *pItem = CQueueBuf<CRecvBuf, RecvPacket>::LeaveBuf();
		//if(pItem)
		//{
		//	CLogError::WriteLog("CRecvBuf::LeaveBuf() count: %d/%lld, pData: %lld, nLen: %d/%d", GetCount(), (int64)pItem, (int64)(char *)pItem->pData, /**/pItem->nDataSize, sizeof(RecvPacket), sizeof(CRecvBuf));
		//	//CLogError::WriteLog("CRecvBuf::LeaveBuf() test: %d/%d/%d/%d/%d/%d", pItem->test1, pItem->test2, pItem->test3, pItem->test4, pItem->test5, time(NULL) - tm);
		//}
        if(!pItem) return 0;
        stFromAddr = pItem->stFromAddr;
        nLen = pItem->nDataSize;
		void *pRet = pItem->pData;
        delete pItem;
        return pRet;
    }  
private:
	CRecvBuf(){}
    ~CRecvBuf(){}

friend class CSingle<CRecvBuf>;
};
//template <> CRecvBuf *CSingle<CRecvBuf>::m_pInstance = 0;
// CRecvBuf *CSingle<CRecvBuf>::m_pInstance = 0;            // 老版本格式 


//// 外网发送缓存区
//class CSendBuf : public CSafeSingle<CSendBuf>
//{
//public:
//    // 获取数据个数
//    uint GetCount(){return m_queue.size();} 
//	static bool EnterBufS(const CNetAddr &stToAddr, void *pData, int nLen)
//	{
//		CSendBuf *pSend = CSendBuf::GetInstance();
//		return pSend->EnterBuf(stToAddr, pData, nLen);
//	}
//public:
//    // 存入缓存 
//    bool EnterBuf(const CNetAddr &stToAddr, void *pData, int nLen)
//    {
//        SendPacket packet(stToAddr, nLen, pData);
//        if(m_queue.size() >= m_nMaxPacket) return false;
//        Lock();
//        try{
//            SendPacket *pItem = new SendPacket(stToAddr, nLen, pData);
//            m_queue.push(pItem);
//			CLogError::WriteLog("CSendBuf::EnterBuf() count: %d/%lld, pData: %lld, nLen: %d/%d", GetCount(), (int64)pItem, (int64)(char *)pItem->pData, /**/pItem->nDataSize, sizeof(SendPacket));
//        }catch(...){}
//        UnLock();
//        return true;
//    }
//    // 从缓存中取出 
//    void *LeaveBuf(CNetAddr &stToAddr, int &nLen)    
//    {
//        SendPacket *pItem(0);
//        Lock();
//        if (!m_queue.empty())
//		{
//			try{
//				pItem = m_queue.front();
//				m_queue.pop();
//				CLogError::WriteLog("CSendBuf::LeaveBuf() count: %d/%lld, pData: %lld, nLen: %d/%d", GetCount(), (int64)pItem, (int64)(char *)pItem->pData, /**/pItem->nDataSize, sizeof(SendPacket));
//			}catch(...){}
//		}
//        UnLock();
//
//        if(!pItem) return 0;
//        stToAddr = pItem->stToAddr;
//        nLen = pItem->nDataSize;
//		void *pData = pItem->pData;
//        delete pItem;
//		return pData;
//    }  
//private:
//    // 最大缓存的报文个数 
//    uint m_nMaxPacket;
//    queue<SendPacket *> m_queue;
//
//	CSendBuf(): m_nMaxPacket(1024){}
//    ~CSendBuf(){}
//friend class CSingle<CSendBuf>;
//};
// template <> CSendBuf *CSingle<CSendBuf>::m_pInstance = 0;
class CSendBuf : public CQueueBuf<CSendBuf, SendPacket>
{
public:
	static bool EnterBufS(const CNetAddr &stToAddr, void *pData, int nLen)
	{
		CSendBuf *pSend = CSendBuf::GetInstance();
		return pSend->EnterBuf(stToAddr, pData, nLen);
	}
    // 存入缓存 
    bool EnterBuf(const CNetAddr &stToAddr, void *pData, int nLen)
    {
        SendPacket *pItem = new SendPacket(stToAddr, nLen, pData);
        bool bRet = CQueueBuf<CSendBuf, SendPacket>::EnterBuf(pItem);
		//CLogError::WriteLog("CSendBuf::EnterBuf() count: %d/%lld, pData: %lld, nLen: %d/%d", GetCount(), (int64)pItem, (int64)(char *)pItem->pData, /**/pItem->nDataSize, sizeof(SendPacket));

		return bRet;
    }
    // 从缓存中取出 
    void *LeaveBuf(CNetAddr &stToAddr, int &nLen)    
    {
        SendPacket *pItem = CQueueBuf<CSendBuf, SendPacket>::LeaveBuf();
		if(pItem)
		{
			//CLogError::WriteLog("CSendBuf::LeaveBuf() count: %d/%lld, pData: %lld, nLen: %d/%d", GetCount(), (int64)pItem, (int64)(char *)pItem->pData, /**/pItem->nDataSize, sizeof(SendPacket));
			//CLogError::WriteLog("CSendBuf::LeaveBuf() test: %d/%d/%d/%d/%d/%d", pItem->test1, pItem->test2, pItem->test3, pItem->test4, pItem->test5, time(NULL) - tm);
		}
        if(!pItem) return 0;
        stToAddr = pItem->stToAddr;
        nLen = pItem->nDataSize;
		void *pRet = pItem->pData;
        delete pItem;
        return pRet;
    }  
private:
	CSendBuf(){}
    ~CSendBuf(){}

friend class CSingle<CSendBuf>;
};


#endif
