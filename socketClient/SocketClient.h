#pragma once
#include "SocketHandle.h"
#include "SocketClientImpl.h"
#include <iostream>
#include <assert.h>
#include <memory>
#include <comdef.h>

const int AF_IPV4 = 0;
const int AF_IPV6 = 1;
const int SOCK_TCP = SOCK_STREAM - 1;
const int SOCK_UDP = SOCK_DGRAM - 1;

class CSocketClient : public ISocketClientHandler
{
	typedef SocketClientImpl<ISocketClientHandler> Client;
	friend Client;
public:
	CSocketClient()
		: m_nMode(AF_IPV4)
		, m_nSockType(SOCK_TCP)
	{
		CSocketHandle::InitLibrary(2);
		m_Client.SetInterface(this);
	} 

	virtual ~CSocketClient() 
	{
		m_Client.Terminate();
		CSocketHandle::ReleaseLibrary();
	}

	void Start()
	{
		int nFamily = (m_nMode == AF_IPV4) ? AF_INET : AF_INET6;
		if (!m_Client.StartClient(TEXT("127.0.0.1"), TEXT("127.0.0.1"), TEXT("2345"), nFamily, (m_nSockType + 1)))
		{
			std::cout << "Failed to start client connection." << std::endl;
		}
	}

	void Stop()
	{
		m_Client.Terminate();
	}

	void Send()
	{
		if (m_Client.IsOpen())
		{
			std::string strMsg = "hello world!";
			m_Client.Write((LPBYTE)strMsg.c_str(),strMsg.size(), NULL);
		}
		else
		{
			std::cout << " Socket is not connected " << std::endl;
		}
	}

	// SocketClient Interface handler
	virtual void OnThreadBegin(CSocketHandle* pSH)
	{
		assert(pSH == m_Client);
		(pSH);

		SockAddrIn sockAddr;
		m_Client->GetSockName(sockAddr);

		TCHAR szIPAddr[MAX_PATH] = { 0 };
		CSocketHandle::FormatIP(szIPAddr, MAX_PATH, sockAddr);

		std::cout << "Client Running on: " << szIPAddr << " , " << ntohs(sockAddr.GetPort()) << std::endl;		
	}

	virtual void OnThreadExit(CSocketHandle* pSH)
	{
		assert(pSH == m_Client);
		(pSH);

		std::cout << "exit";
	}

	virtual void OnDataReceived(CSocketHandle* pSH, const BYTE* pbData, DWORD dwCount, const SockAddrIn& addr)
	{
		assert(pSH == m_Client);
		(pSH);
		std::unique_ptr<byte[]> data(new byte[dwCount + 1]);
		memset(data.get(), 0, dwCount + 1);
		memcpy(data.get(), pbData, dwCount);

		std::cout << data.get() << std::endl;
	}
	virtual void OnConnectionDropped(CSocketHandle* pSH)
	{
		assert(pSH == m_Client);
		(pSH);

		std::cout << "Connection lost with server. Need restart" << std::endl;
	}

	virtual void OnConnectionError(CSocketHandle* pSH, DWORD dwError)
	{
		assert(pSH == m_Client);
		(pSH);
		_com_error err(dwError);
		std::cout << "Communication Error: " << err.ErrorMessage() << std::endl;
	}

protected:
	Client m_Client;

	int m_nMode;
	
	int m_nSockType;
};
