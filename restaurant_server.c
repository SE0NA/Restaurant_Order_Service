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

	int n;
	int request_num;
	char msg[BUF];

	pid_t pid;
	int state;

	// order
	order* orderlist_h;
	order* orderlist_t;
	order* neworder;
	orderlist_h = NULL;

	// menu 구조체 데이터 선언
	menu mymenu;
	mymenu.menu_len = 4;

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

	// mymenu 데이터 동적 할당
	mymenu.menu_cost = (int*)malloc(sizeof(int)*mymenu.menu_len);
	mymenu.menu_str = (char**)malloc(sizeof(char*)*mymenu.menu_len);
	for(int i=0;i<mymenu.menu_len;i++)
		mymenu.menu_str[i] = (char*)malloc(sizeof(char)*30);

	// 메뉴 내용 입력
	mymenu.menu_cost[0] = 15000;
	mymenu.menu_cost[1] = 12000;
	mymenu.menu_cost[2] = 13000;
	mymenu.menu_cost[3] = 9000;

	strcpy(mymenu.menu_str[0], "핫불고기 피자");
	strcpy(mymenu.menu_str[1], "새우 피자");
	strcpy(mymenu.menu_str[2], "콤비네이션 피자");
	strcpy(mymenu.menu_str[3], "예지 피자");


	// bind()
	if(bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0){
		perror("socket bind error\n");
		exit(1);
	}

	// listen() 
	if(listen(server_sock, 10) < 0){
		perror("listen error");
		exit(1);
	}

	// clients accept -> fork()
	while(1){
		// connect() -> send menu to client
		client_addr_size = sizeof(client_addr);
		client_sock = accept(server_sock, (struct sockaddr*)&client_addr, &client_addr_size);
		if(client_sock < 0){		// connect error
			continue;
		}

		// fork()
		if((pid = fork())<0){		// fork error
			close(client_sock);
			continue;
		}

		else if( pid > 0){		// parent process
			puts("클라이언트 접속으로 child 생성 -> for test\n");
			close(client_sock);
			continue;
		}

		else{				// child process
			close(server_sock);
			puts(" ** child process\n");

			// communication with a client
			// 1) send menu to client
			write(client_sock, (void*)&mymenu, sizeof(menu));

			// 2) read order from client
			neworder = malloc(sizeof(order));
			neworder->list = (int*)malloc(sizeof(int)*mymenu.menu_len);
			if(orderlist_h == NULL){	// new order
				orderlist_h = neworder;
				orderlist_t = neworder;
				orderlist_t->next = NULL;
				request_num = 0;
			}
			else{
				orderlist_t->next = neworder;
				orderlist_t = neworder;
				orderlist_t-> next = NULL;
			}

			n = read(client_sock, neworder, sizeof(order));
			printf("read\n");
					
			neworder->no = ++request_num;
			printf("%d %s\n", neworder->no, neworder->name);
			for(int i=0;i<mymenu.menu_len;i++)
				printf("%d ", neworder->list[i]);
			printf("\n");

			// 3) get order in orderlist


			// 4) print UI with new orderlist
		}
		
		// close()
		close(client_sock);
	}		
}
