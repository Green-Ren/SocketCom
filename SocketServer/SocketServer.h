// SocketServer.h : header file
//

#pragma once
#include <atlbase.h>
#pragma warning(push)
#pragma warning(disable:4995)
#include <list>
#pragma warning(pop)
#include "CritSection.h"
#include "SocketHandle.h"
#include "SocketServerImpl.h"
#if defined(SOCKHANDLE_USE_OVERLAPPED)
#include "AsyncSocketServerImpl.h"
#endif

typedef std::list<SOCKET> CSocketList;

// CSocketServer
class CSocketServer : public ISocketServerHandler
{
#if defined(SOCKHANDLE_USE_OVERLAPPED)
	typedef ASocketServerImpl<ISocketServerHandler> Server;
#else
	typedef SocketServerImpl<ISocketServerHandler> Server;
#endif

	friend Server;

	// Construction
public:
	CSocketServer(); // standard constructor

	virtual ~CSocketServer();

protected:
	// ISocketServerHandler
	virtual void OnThreadBegin(CSocketHandle* pSH);
	virtual void OnThreadExit(CSocketHandle* pSH);
	virtual void OnAddConnection(CSocketHandle* pSH, SOCKET newSocket);
	virtual void OnDataReceived(CSocketHandle* pSH, const BYTE* pbData, DWORD dwCount, const SockAddrIn& addr);
	virtual void OnConnectionFailure(CSocketHandle* pSH, SOCKET newSocket);
	virtual void OnConnectionDropped(CSocketHandle* pSH);
	virtual void OnConnectionError(CSocketHandle* pSH, DWORD dwError);

	// Implementation
protected:
	int m_nMode;
	int m_nSockType;

	Server m_Server;

public:
	void Start();
	void Stop();
	void Send(const LPBYTE lpBuffer, DWORD dwCount);
};
