## Overview
This is a simple UDP library, in DLL format, which can be used by UDK to make UDP connections to send and receive data.

Because the 64-bit version of UDK has problems sending pointer-type parameters into DLLs, this library uses separate functions to retrieve each piece of data when receiving from the UDP socket, rather than returning multiple pieces of data in a single function call.

I wrote this for a friend and am putting it up here as-is in case someone else might find it useful.


## DLL bindings
The following are the DLL bindings for UnrealScript in UDK:

    dllimport final function int _BindSocket(int p);
    dllimport final function int _CloseSocket(int s);
    dllimport final function int _SendString(int s, string str, int ip, int p);

    dllimport final function int _RecvString(int s);
    dllimport final function string _GetRecvStringData();
    dllimport final function int _GetRecvStringIP();
    dllimport final function int _GetRecvStringPort();

## Sample usage
The below UnrealScript logic shows how to use this library.

### Bind socket
	local int UDPSocket;
	local int localPort = 7250;

	UDPSocket = _BindSocket(localport)

	if (UDPSocket <= 0) {
		`log("Error binding socket");
	}

### Close socket
	_CloseSocket(UDPSocket);


## Send data over open socket
	local string text = "This is the message to send";
	// See "Convert an IP address into the appropriate format", below
	local int targetIP = IPToLong("127.0.0.1");
	local int targetPort = 7250;

	if (UDPSocket <= 0) return;

	_SendString(UDPSocket, text, targetIP, targetPort);

## Receive data over socket
(Run this each frame, or whenever you want to check for UDP input.)

	local int UDPSocket;
	local int res;
	local int sourceIP;
	local int sourcePort;
	local string message;

	res = _RecvString(UDPSocket);

	while (res != -1)
	{
		message = _GetRecvStringData();
		sourceIP = _GetRecvStringIP();
		sourcePort = _GetRecvStringPort();
		
		`log("Received message: " $ message $ " - (" $ res $ " bytes) from " $ sourceIP $ ":" $ sourcePort);

		res = _RecvString(UDPSocket);
	}

## Convert an IP address into the appropriate format
IP addresses need to be converted into a numeric form, where each of the parts of the IP is encoded into a byte in an integer, with the bytes being in reverse order (last part of the IP address is the largest part of the integer.)
The below UnrealScript function will do the conversion:

	exec function int IPToLong(String ipAddress)
	{
		local array<string> ipArray;
		local int result;
		
		ParseStringIntoArray(ipAddress, ipArray, ".", true);
		
		result = 0;
		result += int(ipArray[0]);
		result += int(ipArray[1] * 256);
		result += int(ipArray[2] * 256 * 256);
		result += int(ipArray[3] * 256 * 256 * 256);
		
		return result;
	}
