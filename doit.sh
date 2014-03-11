gcc -c header.c -o header.o
gcc -c tcpserver.c -o server.o
gcc -c tcpclient.c -o client.o
gcc -g server.o header.o -o serverit -lcrypto
gcc -g client.o header.o -o clientit -lcrypto
