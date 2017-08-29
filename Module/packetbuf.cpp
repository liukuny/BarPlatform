//#include "stdafx.h"
#include "packetbuf.h"

//CRecvBuf *CRecvBuf::m_pInstance = 0;
template <> CRecvBuf *CSingle<CRecvBuf>::m_pInstance = 0;
// CRecvBuf *CSingle<CRecvBuf>::m_pInstance = 0;            // 老版本格式 

template <> CSendBuf *CSingle<CSendBuf>::m_pInstance = 0;
