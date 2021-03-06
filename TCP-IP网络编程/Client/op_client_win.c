#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<WinSock2.h>

#define BUF_SIZE 1024
#define RLT_SIZE 4  //result size
#define OPSZ 4	//operand size
void ErrorHandling(char* message);

int main(int argc, char* argv[]) {
	WSADATA wsaData;
	SOCKET hSocket;
	char opmsg[BUF_SIZE];	//operand message
	int result, opndCnt, i;	//operand count
	SOCKADDR_IN servAddr;

	if (argc != 3) {
		printf("Usage:%s <port>\n", argv[0]);
		exit(1);
	}

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		ErrorHandling("Wsastartup() error");
	}

	hSocket = socket(PF_INET, SOCK_STREAM, 0);
	if (hSocket == INVALID_SOCKET)
		ErrorHandling("Hsocket() error!");

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(argv[1]);
	servAddr.sin_port = htons(atoi(argv[2]));

	if (connect(hSocket, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
		ErrorHandling("Connection error!");
	else
		puts("Connected............");



	fputs("Input message(Q to quit): ", stdout);
	scanf("%d", &opndCnt);
	opmsg[0] = (char)opndCnt;

	for (i = 0; i < opndCnt; i++)
	{
		printf("Operand %d: ", i + 1);
		scanf("%d", (int*)&opmsg[i*OPSZ + 1]);
	}

	fgetc(stdin);
	fputs("Operator: ", stdout);
	scanf("%c", &opmsg[opndCnt*OPSZ + 1]);
	send(hSocket, opmsg, opndCnt*OPSZ + 2, 0);
	recv(hSocket, &result, RLT_SIZE, 0);

	printf("Operation result: %d \n", result);

	closesocket(hSocket);
	WSACleanup();
	return 0;
}

void ErrorHandling(char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}