/* 
 *  ** 서버 코드 **
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
 * 
 *   +) 다중 사용자 접속이 가능하도록 하여야 함!
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "struct.h"

#define BUF 1024

#define COST_STR "15000@12000@13000@9000"
#define MENU_STR "핫불고기 피자@콤비네이션 피자@새우 피자@기본 피자"
#define MENU_LEN 4

int main(int argc, char** argv){
	int server_sock;
	int client_sock;
	struct sockaddr_in server_addr;
	struct sockaddr_in client_addr;
	int client_addr_size;

	int n;
	int request_num = 1;
	char msg[BUF];
	char *str;

	pid_t pid;
	int state;

	// order
	order neworder;
	int orderlist[MENU_LEN] = {0};
	char menulist[MENU_LEN][2][30];
	struct tm *t;
	time_t timer;

	// menu 구조체 데이터 선언
	menu mymenu;
	mymenu.menu_len = MENU_LEN;
	
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

	// 메뉴 내용 입력
	strcpy(mymenu.cost_str, COST_STR);
	strcpy(mymenu.menu_str, MENU_STR);

	// 메뉴 이름
	strcpy(msg, MENU_STR);
	str = strtok(msg, "@");
	strcpy(menulist[0][0], str);

	for(int i=1;i<MENU_LEN;i++){
		str = strtok(NULL, "@");
		strcpy(menulist[i][0], str);
	}
	strcpy(msg, COST_STR);
	str = strtok(msg, "@");
	strcpy(menulist[0][1], str);
	for(int i=1;i<MENU_LEN;i++){
		str = strtok(NULL, "@");
		strcpy(menulist[i][1], str);
	}

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
			close(client_sock);
			continue;
		}

		else{				// child process
			close(server_sock);

			// communication with a client
			// 1) send menu to client
			write(client_sock, (void*)&mymenu, sizeof(menu));

			// 2) read order from client
			n = read(client_sock, (void*)&neworder, sizeof(order));
			timer = time(NULL);
			t = localtime(&timer);
			sprintf(msg, "%02d-%02d-%02d %02d:%02d:%02d", t->tm_year+1900, t->tm_mon+1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);
			strcpy(neworder.ordertime, msg);

			// 3) print UI with new orderlist
			printf("%c[1;33m", 27);		// 글자색: 노랑
			printf("			*** new! ***\n");
			printf("%c[0m", 27);		// 글자색 리셋
			printf("	%s (%s) - %s\n", neworder.name, neworder.phone, neworder.ordertime);
			printf("		%s\n\n", neworder.addr);
		
			strcpy(msg, neworder.list_str);
			msg[sizeof(msg)-1] = 0;
			str = strtok(msg, "@");
			orderlist[0] = atoi(str);
			for(int i=1;i<MENU_LEN;i++){
				str = strtok(NULL, "@");
				orderlist[i] = atoi(str);
			}
			for(int i=0;i<MENU_LEN;i++){	// 메뉴 출력
				if(orderlist[i]!=0){
					printf("		%s		%d	%d won\n",
						menulist[i][0], orderlist[i], atoi(menulist[i][1])*orderlist[i] );
				}
			}
			printf("\n						total: ");
			printf("%c[1;36m", 27);		// 글자색 밝은 파랑
			printf("%d", neworder.total);
			printf("%c[0m", 27);
		        printf(" won\n\n");
			printf("------------------------------------------------------------------\n");

			// 4) return this order information to client
			write(client_sock, (void*)&neworder, sizeof(order));
		}
		
		// close()
		close(client_sock);
		return 0;
	}
	
}
