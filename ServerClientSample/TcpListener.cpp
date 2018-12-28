
#include "pch.h"
#include "Server.h"

TcpListener::TcpListener(int port)
{
	TcpListener::info = GetAddress(port);
	CreateListenSocket();
	BindSocket();
}

 TcpListener::TcpListener()
{
}

bool TcpListener::BeginListen()
{
	ListenToServerSocket();
	return true;
}

SOCKET TcpListener::Accept(User *info)
{
	 return AcceptClientSocket(ListenSocket, info);
}
