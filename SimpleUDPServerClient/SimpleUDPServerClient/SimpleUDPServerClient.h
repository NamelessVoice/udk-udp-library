// The following ifdef block is the standard way of creating macros which make exporting
// from a DLL simpler. All files within this DLL are compiled with the SIMPLEUDPSERVERCLIENT_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see
// SIMPLEUDPSERVERCLIENT_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifndef SIMPLEUDPSERVERCLIENT_H
#define SIMPLEUDPSERVERCLIENT_H

#ifdef SIMPLEUDPSERVERCLIENT_EXPORTS
#define SIMPLEUDPSERVERCLIENT_API __declspec(dllexport)
#else
#define SIMPLEUDPSERVERCLIENT_API __declspec(dllimport)
#endif

#include <WinSock2.h>

extern "C"
{
	WSADATA wsaData;

#define DEFAULT_BUFLEN 10240
	char recvbuf[DEFAULT_BUFLEN];
	int recvbuflen = DEFAULT_BUFLEN;

	char sendbuf[DEFAULT_BUFLEN];
	int sendbuflen = DEFAULT_BUFLEN;

	wchar_t stringBuffer[DEFAULT_BUFLEN];
	int stringBufferLen = DEFAULT_BUFLEN;

	int lastPort;
	int lastIP;

	int lastError;


	__declspec(dllexport) int _BindSocket(int port);
	__declspec(dllexport) int _CloseSocket(int socketId);
	__declspec(dllexport) int _SendString(int socketId, const wchar_t* str, int ip, int port);

	__declspec(dllexport) int _RecvString(int socketId);
	__declspec(dllexport) wchar_t* _GetRecvStringData();
	__declspec(dllexport) int _GetRecvStringIP();
	__declspec(dllexport) int _GetRecvStringPort();

	__declspec(dllexport) int _GetLastError();
}
#endif // SIMPLEUDPSERVERCLIENT_H
