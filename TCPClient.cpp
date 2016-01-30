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
 * 
 * Author: Sean Bradley
 * Created on 30 January 2016, 08:43
 */

#include<iostream>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include <thread>
#include <netdb.h>

using namespace std;

int main(int argc, char *argv[]) {
    if (argc < 2) {
        cout << "Usage: " << argv[0] << " DestinationIPAddress" << std::endl;
        return 1;
    }

    int ClientFD;
    char buf[256];
    struct sockaddr_in Destination;
    ClientFD = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    Destination.sin_family = AF_INET;
    Destination.sin_port = htons(8080);
    Destination.sin_addr.s_addr = inet_addr(argv[1]);
    connect(ClientFD, (struct sockaddr *) &Destination, sizeof Destination);

    int numbytes;
    while (0 < (numbytes = recv(ClientFD, buf, sizeof buf, 0))) {
        buf[numbytes] = '\0';
        printf("Data received: %s", buf);
        memset(buf, 0,256);
    }
    
    return 0;

}
