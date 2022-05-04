/*  파일명: customer_clinet.c
 *  
 *  주문자(client)는 식당(server)으로 주문 정보를 전송(order 구조체)
 *  
 *  1. server connect 요청 승인 시, menu 출력
 *  2. 사용자는 콘솔 ui를 이용하여 주문을 선택(order 구조체 입력)
 *  3. 주문을 결정하면 해당 order 구조체 변수를 server에 전송
 *  4. server에 전송 완료 시 주문 정보 띄우기
 *  5. 화면 초기화
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#define BUF 512
#define IPserver "127.0.0.1"


void InitialScreen();
void PrintMenu(char*);
void SelectMenu();

int main(int argc, char**argv){
	int sock;
	struct sockaddr_in server_addr;

	int str_len;
	char msg[BUF];
	// 주문 전송을 위한 order 구조체 필요: myorder
	
	if(argc != 2){
		printf("Usage: %s <port>\n", argv[0]);
		exit(1);
	}

	// socket()
	sock = socket(PF_INET, SOCK_STREAM, 0);
	if(sock < 0){
		perror("socket creation error\n");
		exit(1);
	}
	
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_add.sin_addr.s_addr = inet_addr(IPserver);
	server_addr.sin_port = htons(atoi(argv[1]));

	// connect()
	if(connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0){
		// server IP는 지정 →  오류가 났다면 서버(식당)이 열리지 않음)
		// 관련 ui 출력
		printf("현재는 영업을 하지 않습니다!");
	}

	// connect -> menu를 전송 받음
	str_len = read(sock, msg, BUF-1);
	// menu(msg) 출력 ui
	PrintfMenu(msg);

	// order 매개변수 입력 필요
	SelectMenu();	

	// order 전송
	// write(sock, myorder, sizeof(order));
	
	printf("주문 접수 완료!");

	closc(sock);

	return 0
}
