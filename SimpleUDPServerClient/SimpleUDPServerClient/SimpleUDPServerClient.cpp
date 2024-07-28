// SimpleUDPServerClient.cpp : Defines the exported functions for the DLL.
//

#include "pch.h"
#include "framework.h"
#include "SimpleUDPServerClient.h"

#include <wchar.h>

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#pragma comment(lib, "Ws2_32.lib")
#pragma warning(disable:4996) 


__declspec(dllexport) int _BindSocket(const int port)
{
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != NO_ERROR) {
        lastError = -1001; // Arbitrary error code
        return -1;
    }

    // Create a SOCKET for listening for incoming connection requests
    SOCKET ListenSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (ListenSocket == INVALID_SOCKET) {
        lastError = WSAGetLastError();
        WSACleanup();
        return -2;
    }

    // Set socket into non-blocking mode
    unsigned long ul = 1;
    int nRet = ioctlsocket(ListenSocket, FIONBIO, (unsigned long*)&ul);

    // Set up structure for binding address / port
    struct sockaddr_in service;
    memset(&service, 0, sizeof(service));
    service.sin_family = AF_INET;
    service.sin_addr.s_addr = INADDR_ANY;
    service.sin_port = htons(port);

    // Bind the socket.
    iResult = bind(ListenSocket, reinterpret_cast<SOCKADDR*>(&service), sizeof(service));
    if (iResult == SOCKET_ERROR) {
        lastError = WSAGetLastError();
        closesocket(ListenSocket);
        WSACleanup();
        return -3;
    }

    WSACleanup();
    return static_cast<int>(ListenSocket);
}

__declspec(dllexport) int _CloseSocket(const int socketId)
{
    if (socketId != INVALID_SOCKET)
    {
        int iResult = closesocket(socketId);
        if (iResult == SOCKET_ERROR) {
            lastError = WSAGetLastError();
        }
        WSACleanup();
    }

    return 1;
}

__declspec(dllexport) int _SendString(const int socketId, const wchar_t* str, int ip, int port)
{
    int iResult = -1;

    if (socketId != INVALID_SOCKET)
    {
        WideCharToMultiByte(CP_ACP, 0, str, -1, sendbuf, sendbuflen, NULL, NULL);

        // Set up address to send to
        struct sockaddr_in RecvAddr;
        memset(&RecvAddr, 0, sizeof(RecvAddr));
        RecvAddr.sin_addr.s_addr = ip;
        RecvAddr.sin_family = AF_INET;
        RecvAddr.sin_port = htons(port);

        // Send a datagram to the receiver
        iResult = sendto(socketId, sendbuf, sendbuflen, 0, reinterpret_cast<SOCKADDR*>(&RecvAddr), sizeof(RecvAddr));
        if (iResult == SOCKET_ERROR) {
            lastError = WSAGetLastError();
        }
    }

    return iResult;
}

__declspec(dllexport) int _RecvString(const int socketId)
{
    wcscpy_s(stringBuffer, DEFAULT_BUFLEN, (L""));

    struct sockaddr_in SenderAddr;
    memset(&SenderAddr, 0, sizeof(SenderAddr));
    int SenderAddrSize = sizeof(struct sockaddr_in);

    int bytesReceived = recvfrom(socketId, recvbuf, recvbuflen, 0, (SOCKADDR*)&SenderAddr, &SenderAddrSize);
    if (bytesReceived > 0)
    {
        int count = MultiByteToWideChar(CP_ACP, 0, recvbuf, recvbuflen, stringBuffer, stringBufferLen);

        lastIP = SenderAddr.sin_addr.s_addr;
        lastPort = ntohs(SenderAddr.sin_port);
    }
    else {
        bytesReceived = -1;
    }

    return bytesReceived;
}

wchar_t* _GetRecvStringData()
{
    return stringBuffer;
}

int _GetRecvStringIP()
{
    return lastIP;
}

int _GetRecvStringPort()
{
    return lastPort;
}

int _GetLastError()
{
    return lastError;
}
