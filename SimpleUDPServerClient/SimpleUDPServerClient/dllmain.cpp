// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
//#include "SimpleUDPServerClient.h"

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
        // TODO: Close additional sockets if we support multi-socket
        //_CloseSocket(1);
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

