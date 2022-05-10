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
 *  3. 공유메모리를 이용하여 부모 프로세스와 자식프로세스가 주문 내역 리스트와 requset_num을 공유하도록 함
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
#include <sys/shm.h>	// 공유 메모리
#include <sys/ipc.h>
#include "struct.h"

#define BUF 1024

#define COST_STR "15000@12000@13000@9000"
#define MENU_STR "핫불고기 피자@콤비네이션 피자@새우 피자@기본 피자"

#define MEM_KEY 9999	// 공유메모리 사용을 위한 키
#define MEM_SIZE 1024	// 공유 메모리 사이즈
#define RAND_DISTANCE 5		

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

	// 공유 메모리: order
	int shmid;
	void *shm_addr;
	order* orderlist_h;
	order* orderlist_t;
	order *ptr, *next;

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

	// 메뉴 내용 입력
	strcpy(mymenu.cost_str, COST_STR);
	strcpy(mymenu.menu_str, MENU_STR);

	// 공유 메모리 설정
	if((shmid = shmget((key_t)MEM_KEY, MEM_SIZE, IPC_CREAT|0666))==-1) return -1;
	if((shm_addr = shmat(shmid, NULL, 0)) == (void*)-1) return -1;

	

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
			orderlist_h = NULL;
			orderlist_t = NULL;
			continue;
		}

		else{				// child process
			close(server_sock);

			// communication with a client
			// 1) send menu to client
			write(client_sock, (void*)&mymenu, sizeof(menu));

			// 2) read order from client
			order *neworder = malloc(sizeof(order));
			if(orderlist_h == NULL){	// first order
				orderlist_h = neworder;
				orderlist_t = neworder;
				neworder->next = NULL;
				request_num=0;
				printf("h:%p t:%p  : ", orderlist_h, orderlist_t);
			}
			else{				// rest orders
				printf("next ");
				orderlist_t->next = neworder;
				orderlist_t = neworder;
				neworder->next = NULL;
				printf("next - h:%p t:%p  : ", orderlist_h, orderlist_t);
			}

			n = read(client_sock, neworder, sizeof(order));
								
			neworder->no = ++request_num;
			
			ptr = orderlist_h;
			while(ptr != NULL){
				printf("%p  %p  : ", orderlist_h->next, ptr);
				printf("%d %s %s\n", ptr->no, ptr->name, ptr->list_str);
				ptr = ptr->next;
			}

			// 3) get order in orderlist
			
			
			// 4) print UI with new orderlist
		}
		
		// close()
		close(client_sock);

		// free memory
		ptr = orderlist_h->next;
		while(ptr != NULL){
			next = ptr->next;
			free(ptr);
			ptr = next;
		}
	}		
}
