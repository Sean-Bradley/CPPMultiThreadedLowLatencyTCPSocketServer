/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2016 Sean Bradley
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.

 * Author: Sean Bradley
 * Created on 24 January 2016, 16:07
 */
#include<iostream>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include <thread>
#include <chrono>
#include <list>

using namespace std;

int ClientCount = 0;
std::list<int> clientList;

void broadcastLoop() {
    while (1) {
        std::time_t serverTime = std::time(NULL);
        std::cout << "ClientCount=" << ClientCount << " : " << std::ctime(&serverTime);
        for (std::list<int>::iterator list_iter = clientList.begin(); list_iter != clientList.end(); list_iter++) {
            std::string  message =  std::to_string(serverTime) + " : You're socketID=" + std::to_string(*list_iter) + " : There are " + std::to_string(ClientCount) + " sockets connected.\r\n";
            send(*list_iter, message.c_str(), message.size(), 0);
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

void *clientConnectionHandler(void *socket_desc) {
    cout << "New Client Connection : ClientCount=" << ++ClientCount << endl;
        
    int ClientFD = *(int*) socket_desc;
    clientList.push_back(ClientFD);
    
    int numbytes;
    char buf[256];
    while (0 < (numbytes = recv(ClientFD, buf, sizeof buf, 0))) {
        buf[numbytes] = '\0';
        send(ClientFD, buf, strlen(buf), 0);
        memset(buf, 0,256);
    }
    
    clientList.remove(ClientFD);
    cout << "Client Disconnected : ClientCount=" << --ClientCount << endl;
}

int main(int argc, char** argv) {
    int ListenSocket;
    struct sockaddr_in saServer;

    ListenSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == ListenSocket) {
        perror("Could not create socket");
        exit(EXIT_FAILURE);
    }

    saServer.sin_family = AF_INET;
    saServer.sin_addr.s_addr = INADDR_ANY;
    saServer.sin_port = htons(8080);

    if (-1 == bind(ListenSocket, (struct sockaddr *) &saServer, sizeof (saServer))) {
        perror("Error : Bind Failed");
        close(ListenSocket);
        exit(EXIT_FAILURE);
    }
    if (-1 == listen(ListenSocket, 10)) {
        perror("Error : Listen Failed");
        close(ListenSocket);
        exit(EXIT_FAILURE);
    }
    cout << "Server listening on port " << ntohs(saServer.sin_port) << endl;

    std::thread broadcastLoopThread(broadcastLoop);

    while (1) {
        int NewClient = accept(ListenSocket, NULL, NULL);
        std::thread clientThread(clientConnectionHandler,  (void*) &NewClient);
        clientThread.detach();
    }

    return 0;
}




