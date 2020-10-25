#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
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
#include <vector> // for vector
#include <iterator> // for std::istream_iterator and std::ostream_iterator
#include <algorithm>

#pragma comment(lib, "ws2_32.lib")
using namespace std;


int main(int argc, char** argv)
{

    WSADATA wsaData;
    SOCKET SendingSocket;
    SOCKADDR_IN ReceiverAddr, SrcInfo;

    int Port = 9001;

    //Make-shift read from file on client side
    ifstream fin;
    fin.open("base.txt");
    stringstream buffman;
    buffman << fin.rdbuf();
    string bufferstr = buffman.str();
    //cout << "\"" << buffman.str() << "\"\n";
    
    ifstream file("base.jpeg", ios::binary); //reads in the file
    // Prepare iterator pairs to iterate the file content!
    istream_iterator<unsigned char> begin(file), end; //creates an iterator of type unsigned char to read from begin of ile to end
    vector<unsigned int> buffer(begin, end); //putting the values in a vector called buffer
    //ofstream output("based.jpeg", ios::binary);
    //copy(buffer.begin(), buffer.end(), ostream_iterator<unsigned int>(cout <<","));
    // Printing out byte data of the jpeg
    /*cout << "JPEG bytes: ";
    for (int i = 0; i < buffer.size(); i++) {
        //output << buffer[i];
        cout << buffer[i] << " ";
    }
    cout << "\n";*/

    //const char* SendBuf = "pretend im a jpeg hehe xd";//&buffman.str()[0];
    char* SendBuf = new char[bufferstr.size() + 1];
    strcpy(SendBuf, bufferstr.c_str());
    int BufLength = 1024;
    int TotalByteSent;

    // Initialize Winsock version 2.2
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        cout << "C: WSAStartup failed with error " << WSAGetLastError() << ".\n";
        // Clean up
        WSACleanup();
        // Exit with error
        return -1;
    }
    else
    {
        cout << "C: The Winsock DLL status is " << wsaData.szSystemStatus << ".\n";
    }

    // Create a new socket to receive datagrams on.
    SendingSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if (SendingSocket == INVALID_SOCKET)
    {
        cout << "C: Error at socket(): " << WSAGetLastError() << ".\n";
        // Clean up
        WSACleanup();
        // Exit with error
        return -1;
    }
    else
    {
        cout << "C: socket() is OK!\n";
    }

    // Set up a SOCKADDR_IN structure that will identify who we
    // will send datagrams to. For demonstration purposes, let's
    // assume our receiver's IP address is 127.0.0.1 and waiting
    // for datagrams on port 5150.

    ReceiverAddr.sin_family = AF_INET;
    ReceiverAddr.sin_port = htons(Port);
    ReceiverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Send a datagram to the receiver.
    cout << "C: Data to be sent: \"" << SendBuf << "\".\n";
    cout << "C: Sending datagrams...\n";
    TotalByteSent = sendto(SendingSocket, SendBuf, BufLength, 0, (SOCKADDR*)&ReceiverAddr, sizeof(ReceiverAddr));
    cout << "C: sendto() looks OK!\n";


    // Some info on the receiver side...
    // Allocate the required resources
    //memset(&SrcInfo, 0, sizeof(SrcInfo));
    int len = sizeof(SrcInfo);

    getsockname(SendingSocket, (SOCKADDR*)&SrcInfo, &len);

    cout << "C: Sending IP(s) used: " << inet_ntoa(SrcInfo.sin_addr) << ".\n";
    cout << "C: Sending port used: " << htons(SrcInfo.sin_port) << ".\n";

    // Some info on the sender side
    getpeername(SendingSocket, (SOCKADDR*)&ReceiverAddr, (int*)sizeof(ReceiverAddr));

    cout << "C: Receiving IP used: " << inet_ntoa(ReceiverAddr.sin_addr) << ".\n";
    cout << "C: Receiving port used: " << htons(ReceiverAddr.sin_port) << ".\n";
    cout << "C: Total byte sent: " << TotalByteSent << ".\n";

    // When your application is finished receiving datagrams close the socket.
    cout << "C: Finished sending. Closing the sending socket...\n";

    if (closesocket(SendingSocket) != 0)
    {
        cout << "C: closesocket() failed! Error code: " << WSAGetLastError() << ".\n";
    }
    else
    {
        cout << "C: closesocket() is OK\n";
    }

    // When your application is finished call WSACleanup.
    cout << "C: Cleaning up...\n";

    if (WSACleanup() != 0)
    {
        cout << "C: WSACleanup() failed! Error code: " << WSAGetLastError() << ".\n";
    }
    else
    {
        cout << "C: WSACleanup() is OK\n";
    }

    delete[] SendBuf;
    // Back to the system
    return 0;
}
