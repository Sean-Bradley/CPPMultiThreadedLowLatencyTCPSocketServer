# CPP Multi Threaded Low Latency TCP Socket Server
C++ Multi Threaded Low Latency TCP Socket Server


To build the Server on Linux

g++ TCP4Server.cpp -pthread -std=c++11 -o SeansTCP4Server


To run the server

./SeansTCP4Server


To test use Telnet client

telnet [host address] 8080

or use compile and use TCP4Client.cpp

To build the client on Linux

g++ TCP4Client.cpp -std=c++11 -o SeansTCP4Client


To run the client

./SeansTCP4Client [IP address of SeansTCP4Server]






