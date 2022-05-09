/* 
 *  파일명: restaurant_server.c
 *  
 *  # 서버(식당)가 커져 있어야 클라이언트가 접속이 가능
 *    →  클라이언트 소켓이 종료되더라도 다른 클라이언트의 접속을 받아야 하기 때문에
 *       계속 대기해야 함
 * 
 *  1. 클라이언트 연결 -> 메뉴 전송
 *
 *  2. 주문 요청: 클라이언트: 주문 구조체 형태로 전송 →  이를 서버의 주문 내역에 저장.
 *                  서버: 클라이언트에게 주문이 완료됨을 전송(문자열)
 *  
 *  3. 주문 내역은 동적 리스트로 저장
 *
 *   +) 다중 사용자 접속이 가능하도록 하여야 함!
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "struct.h"

#define BUF 1024

int main(int argc, char** argv){
	int server_sock;
	int client_sock;
	struct sockaddr_in server_addr;
	struct sockaddr_in client_addr;
	int client_addr_size;

	int str_len;
	int request_num;
	char[BUF] msg;

	menu mymenu;
	mymenu.menu_len = 4;



	// order list 동적 리스트 필요(order 구조체 작성 완료후)

	if(argc != 2){
		printf("Usage: %s <port>\n", argv[0]);
		exit(1);
	}
	
	// socket()
	server_sock = socket(PF_INET, SOCK_STREAM, 0);
	if(server_sock < 0){
		perror("socket creation error\n");
		exit(1);
	}
	
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(atoi(argv[1]));

	// bind()
	if(bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0){
		perror("socket bind error\n");
		exit(1);
	}

	// listen() 
	if(listen(server_sock, 10) < 0){
		peeror("listen error");
		exit(1);
	}

	client_addr_size = sizeof(client_addr);

	// function
	while(1){
		// connect() -> send menu to client
		client_sock = accept(server_sock, (struct sockaddr*)&client_addr, &client_addr_size);
		if(client_sock < 0){
			perror("socket accept error\n");
			exit(1);
		}

		// fork() : 다중 클라이언트 접속

		write(client_sock, menu, sizeof(menu));
		

		// 주문 대기
			// 1. read로 클라이언트 소켓에서 order 내용 입력받기 -> neworder
			// 2. neworder를 리스트에 넣기
			// 3. 화면에 새로운 주문 내역 출력
		
		

		// close()
		close(client_sock);
	}		
}
