#pragma comment(lib, "Ws2_32.lib")
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <windows.h>
#include <stdio.h>

#include <iostream>
using namespace std;

int main()
{
    WSADATA wsadata;

    int res = WSAStartup(MAKEWORD(2, 2), &wsadata);
    if (res != NO_ERROR)
    {
        cout << "WSAStartup failked with error " << res << endl;
        return 1;
    }

    SOCKET udpSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (udpSocket == INVALID_SOCKET)
    {
        cout << "socket failed with error " << WSAGetLastError() << endl;
        return 2;
    }

    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(23000);
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr.s_addr);

    if (bind(udpSocket, (SOCKADDR*)&addr, sizeof(addr)) != NO_ERROR)
    {
        cout << "bind failed with error " << WSAGetLastError() << endl;
        return 3;
    }

    SYSTEMTIME st;
    GetSystemTime(&st);
    if (st.wHour >= 3 && st.wHour <= 11) 
    {
        cout << "Good morning!";
    }
    else if (st.wHour >= 12 && st.wHour <= 15)
    {
        cout << "Good day!";
    }
    else if (st.wHour >= 16 && st.wHour <= 21)
    {
        cout << "Good evening!";
    }
    else
    {
        cout << "Good night!";
    }
    cout << "\nenter \'bye\' to close the chat\n";

    while(1)
    {
        const size_t receiveBufSize = 1024;
        char receiveBuf[receiveBufSize];

        sockaddr_in senderAddr;
        int senderAddrSize = sizeof(senderAddr);
        //
        cout << "Receiving data...." << endl;
        int bytesReceived = recvfrom(udpSocket, receiveBuf, receiveBufSize, 0, (SOCKADDR*)&senderAddr, &senderAddrSize);

        if (bytesReceived == SOCKET_ERROR)
        {
            cout << "recvfrom failed with error " << WSAGetLastError() << endl;
            return 4;
        }

        receiveBuf[bytesReceived] = '\0';
        cout << "Received from " << int(senderAddr.sin_addr.S_un.S_un_b.s_b1) << '.' << int(senderAddr.sin_addr.S_un.S_un_b.s_b2) << '.' << int(senderAddr.sin_addr.S_un.S_un_b.s_b3) << '.' << int(senderAddr.sin_addr.S_un.S_un_b.s_b4) << '.' << endl;
        cout << "Data: " << receiveBuf << endl;


        const size_t sendBufSize = 1024;
        char sendBuf[sendBufSize];
        cin.getline(sendBuf, sendBufSize);//тут через син гетлайн

        int sendResult = sendto(udpSocket, sendBuf, strlen(sendBuf), 0, (SOCKADDR*)&senderAddr, senderAddrSize);
        if (sendResult == SOCKET_ERROR)
        {
            cout << "sendto failed with error " << WSAGetLastError() << endl;
            return 4;
        }

        if (strcmp(receiveBuf, "bye") == 0)break;
        
    } 

    closesocket(udpSocket);
    WSACleanup();
    
    return 0;
}

