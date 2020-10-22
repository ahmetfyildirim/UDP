#define _WINSOCK_DEPRECATED_NO_WARNINGS
//#ifndef WIN32_LEAN_AND_MEAN
//#define WIN32_LEAN_AND_MEAN
//#endif

//#include <windows.h>

#include <string>
#include <WinSock2.h>
//#include <Ws2tcpip.h>

//#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>

#pragma comment(lib, "ws2_32.lib")
using namespace std;


int main(int argc, char** argv)
{

    WSADATA wsaData;
    SOCKET ReceivingSocket;
    SOCKADDR_IN ReceiverAddr;

    int Port = 9001;
    char ReceiveBuf[1024];
    int BufLength = 1024;

    SOCKADDR_IN SenderAddr;

    int SenderAddrSize = sizeof(SenderAddr);
    int ByteReceived = 5;

    // Initialize Winsock version 2.2
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        cout << "S: WSAStartup failed with error " << WSAGetLastError() << ".\n";
        return -1;
    }
    else
    {
        cout << "S: The Winsock DLL status is " << wsaData.szSystemStatus << ".\n";
    }

    // Create a new socket to receive datagrams on.
    ReceivingSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if (ReceivingSocket == INVALID_SOCKET)
    {
        cout << "S: Error at socket(): " << WSAGetLastError() << ".\n";
        // Clean up
        WSACleanup();
        // Exit with error
        return -1;
    }
    else
    {
        cout << "S: socket() is OK!\n";
    }

    // Set up a SOCKADDR_IN structure that will tell bind that we
    // want to receive datagrams from all interfaces using port 5150.

    // The IPv4 family
    ReceiverAddr.sin_family = AF_INET;

    // Port no. 5150
    ReceiverAddr.sin_port = htons(Port);

    // From all interface (0.0.0.0)
    ReceiverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    // Associate the address information with the socket using bind.
    // At this point you can receive datagrams on your bound socket.

    if (bind(ReceivingSocket, (SOCKADDR*)&ReceiverAddr, sizeof(ReceiverAddr)) == SOCKET_ERROR)
    {
        cout << "S: bind() failed! Error: " << WSAGetLastError() << ".\n";
        // Close the socket
        closesocket(ReceivingSocket);
        // Do the clean up
        WSACleanup();
        // and exit with error
        return -1;
    }
    else
    {
        cout << "S: bind() is OK!\n";
    }


    // Some info on the receiver side...
    int len = sizeof(ReceiverAddr);
    getsockname(ReceivingSocket, (SOCKADDR*)&ReceiverAddr, &len);

    cout << "S: Receiving IP(s) used: " << inet_ntoa(ReceiverAddr.sin_addr) << ".\n";
    cout << "S: Receiving port used: " << htons(ReceiverAddr.sin_port) << ".\n";
    cout << "S: I\'m ready to receive a datagram...\n";

    // At this point you can receive datagrams on your bound socket.
    ByteReceived = recvfrom(ReceivingSocket, ReceiveBuf, BufLength, 0, (SOCKADDR*)&SenderAddr, &SenderAddrSize);

    if (ByteReceived > 0)
    {
        cout << "S: Total Bytes received: " << ByteReceived << ".\n";
        cout << "S: The data is \"" << ReceiveBuf << "\".\n";
    }
    else if (ByteReceived <= 0)
    {
        cout << "S: Connection closed with error code: " << WSAGetLastError() << ".\n";
    }
    else
    {
        cout << "S: recvfrom() failed with error code: " << WSAGetLastError() << ".\n";
    }


    // Some info on the sender side
    getpeername(ReceivingSocket, (SOCKADDR*)&SenderAddr, &SenderAddrSize);

    cout << "S: Sending IP used: " << inet_ntoa(SenderAddr.sin_addr) << ".\n";
    cout << "S: Sending port used: " << htons(SenderAddr.sin_port) << ".\n";

    // When your application is finished receiving datagrams close the socket.
    cout << "S: Finished receiving. Closing the listening socket...\n";

    if (closesocket(ReceivingSocket) != 0)
    {
        cout << "S: closesocket() failed! Error code: " << WSAGetLastError() << ".\n";
    }
    else
    {
        cout << "S: closesocket() is OK\n";
    }

    // When your application is finished call WSACleanup.
    cout << "S: Cleaning up...\n";

    if (WSACleanup() != 0)
    {
        cout << "S: WSACleanup() failed! Error code: " << WSAGetLastError() << ".\n";
    }
    else
    {
        cout << "S: WSACleanup() is OK\n";
    }

    //Make-shift write to file on server side
    ofstream fout("base_rec.txt");
    fout << ReceiveBuf;
    fout.close();

    // Back to the system
    return 0;
}