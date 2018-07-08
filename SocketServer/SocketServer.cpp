#include <assert.h>
#include <iostream>
#include <comdef.h>
#include <atlbase.h>
#include "ThreadPool.hpp"
#include "SocketServer.h"

const int AF_IPV4 = 0;
const int AF_IPV6 = 1;
const int SOCK_TCP = SOCK_STREAM - 1;
const int SOCK_UDP = SOCK_DGRAM - 1;

CSocketServer::CSocketServer()
	:m_nMode(AF_IPV4)
	,m_nSockType(SOCK_TCP)
{
	CSocketHandle::InitLibrary(2);
	m_Server.SetInterface(this);
}

CSocketServer::~CSocketServer()
{
	m_Server.Terminate();
	CSocketHandle::ReleaseLibrary();
}

// ISocketServerHandler
void CSocketServer::OnThreadBegin(CSocketHandle* pSH)
{
	assert(pSH == m_Server);
	(pSH);
	std::cout << "Server Running" << std::endl;
}

void CSocketServer::OnThreadExit(CSocketHandle* pSH)
{
	assert(pSH == m_Server);
	(pSH);
	std::cout << "Server Down!" << std::endl;
}

void CSocketServer::OnAddConnection(CSocketHandle* pSH, SOCKET newSocket)
{
	assert(pSH == m_Server);
	(pSH);
	std::cout << "Connection established!" << std::endl;
}

void CSocketServer::OnDataReceived(CSocketHandle* pSH, const BYTE* pbData, DWORD dwCount, const SockAddrIn& addr)
{
	assert(pSH == m_Server);
	(pSH);
	std::cout << "OnDataReceived, dwCount: " << dwCount << std::endl;
}

void CSocketServer::OnConnectionFailure(CSocketHandle* pSH, SOCKET newSocket)
{
	assert(pSH == m_Server);
	(pSH);
	std::cout << "Connection abandoned" << std::endl;
}

void CSocketServer::OnConnectionDropped(CSocketHandle* pSH)
{
	assert(pSH == m_Server);
	(pSH);
	std::cout << "Connection lost with client" << std::endl;
}

void CSocketServer::OnConnectionError(CSocketHandle* pSH, DWORD dwError)
{
	assert(pSH == m_Server);
	(pSH);
	_com_error err(dwError);
	std::cout << "Communication Error: " << err.ErrorMessage() << std::endl;
}


void CSocketServer::Start()
{
	int nFamily = (m_nMode == AF_IPV4) ? AF_INET : AF_INET6;
	if (!m_Server.StartServer(TEXT("0.0.0.0"), TEXT("2345"), nFamily, (m_nSockType + 1)))
	{
		std::cout << "Failed to start server." << std::endl;
	}
}

void CSocketServer::Stop()
{
	m_Server.Terminate();
}

void CSocketServer::Send(const LPBYTE lpBuffer, DWORD dwCount)
{
	if (m_Server.IsOpen())
	{
		if (m_nSockType == SOCK_TCP)
		{
			// unsafe access to Socket list!
#ifdef SOCKHANDLE_USE_OVERLAPPED
			const SocketBufferList& sl = m_Server.GetSocketList();
			for (SocketBufferList::const_iterator citer = sl.begin(); citer != sl.end(); ++citer)
#else
			const SocketList& sl = m_Server.GetSocketList();
			for (SocketList::const_iterator citer = sl.begin(); citer != sl.end(); ++citer)
#endif
			{
				CSocketHandle sockHandle;
				sockHandle.Attach((*citer));
				sockHandle.Write(lpBuffer, dwCount, NULL);
				sockHandle.Detach();
			}
		}
	}
	else
	{
		std::cout << "Socket is not connected! " << std::endl;
	}
}