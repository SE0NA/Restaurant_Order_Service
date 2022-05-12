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
#include <termios.h>
#include "struct.h"

#define BUF 512
#define IPserver "127.0.0.1"

void inputOrderlist(order*, menu*);
void inputCustomerInformation(order*);
int getkey();

int main(int argc, char**argv){
	int sock;
	struct sockaddr_in server_addr;

	int str_len;
	char msg[BUF];
	
	// 주문 전송을 위한 order 구조체 필요: myorder
	order myorder;
	menu mymenu;

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
	server_addr.sin_addr.s_addr = inet_addr(IPserver);
	server_addr.sin_port = htons(atoi(argv[1]));

	// connect()
	if(connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0){
		// server IP는 지정 →  오류가 났다면 서버(식당)이 열리지 않음)
		// 관련 ui 출력
		printf("현재는 영업을 하지 않습니다!");
		exit(1);
	}
	//메뉴 읽기
	str_len = read(sock, (void*)&mymenu, sizeof(menu));
	printf("%s\n", mymenu.menu_str);
	
	//메뉴 선택
	inputOrderlist(&myorder, &mymenu);
	printf("%s\n\n", myorder.list_str);

	
	// 임시
	printf("이름: ");
        fgets(msg, sizeof(msg), stdin);
        msg[strlen(msg)-1] = 0;
        strcpy(myorder.name, msg);

        printf("전화번호: ");
        fgets(msg, sizeof(msg), stdin);
        msg[strlen(msg)-1] = 0;
        strcpy(myorder.phone, msg);

        printf("주소: ");
        fgets(msg, sizeof(msg), stdin);
        msg[strlen(msg)-1] = 0;
        strcpy(myorder.addr, msg);




	//서버로 보내기
	write(sock, (void*)&myorder, sizeof(order));

	// 서버로부터 주문 내역반환 받음
	str_len = read(sock, (void*)&myorder, sizeof(order));

	printf("주문 내역: \n");
	printf(" %s\n %s\n %s\n %s\n", myorder.name, myorder.phone, myorder.addr, myorder.ordertime);


	// connect -> menu를 전송 받음
	//str_len = read(sock, msg, BUF-1);
	
	

	// order 전송
	// write(sock, myorder, sizeof(order));
	
	

	close(sock);

	return 0;
}

// 메뉴 수량 입력(방향키)
void inputOrderlist(order* myorder, menu* mymenu){
	char **menu_list, *str, *str2;
	int *cost_list, *order_list;
	int total = 0, row = 0, len, tab_len, k=0, max_len=0;
	char key=0;
	
	// 메뉴 내용 저장을 위한 메모리 동적할당
	menu_list = (char**)malloc(sizeof(char*)*mymenu->menu_len);	// 메뉴 이름 저장
	cost_list = (int*)malloc(sizeof(int)*mymenu->menu_len);		// 가격 저장
	order_list = (int*)malloc(sizeof(int)*mymenu->menu_len);	// (입력) 주문 수량
	for(int i=0;i<mymenu->menu_len;i++){
		menu_list[i] = (char*)malloc(sizeof(char)*50);
		cost_list[i] = 0;
		order_list[i] = 0;
	}
	// 내용 저장
	str = strtok(mymenu->menu_str, "@");	// 메뉴 이름
	strcpy(menu_list[0], str);
	for(int i=1;i<mymenu->menu_len;i++){
		str = strtok(NULL, "@");
		strcpy(menu_list[i], str);
		if(max_len < strlen(menu_list[i]))
			max_len = strlen(menu_list[i]);
	}
	str = strtok(mymenu->cost_str, "@");	// 메뉴 가격
	cost_list[0] = atoi(str);
	for(int i=1;i<mymenu->menu_len;i++){
		str = strtok(NULL, "@");
		cost_list[i] = atoi(str);
	}
	
	while(1){
		system("clear");	// 화면 초기화
		printf("+-------------------------------------------------------------------+\n");
		for(int i=0;i<mymenu->menu_len;i++){	// 메뉴 출력
			printf("| ");
			len = strlen(menu_list[i]);
			if(row == i){	// 현재 선택된 메뉴
				printf("%c[1;33m", 27);	// 글자색 변경-> 노랑
				printf("%s", menu_list[i]);
				for(int i=len; i<max_len; i++)
					printf(" ");
				printf("\t\t%d won\t", cost_list[i]);
				printf("%c[1;36m", 27);	// 글자색 변경 -> 밝은 파랑
				printf("◀  %2d ▶   ", order_list[i]);
				printf("%c[0m", 27);	// 글자색 원래대로
			}
			else{
				printf("%s", menu_list[i]);
				for(int i=len; i<max_len;i++)
					printf(" ");
				printf("\t\t%d won\t", cost_list[i]);
				if(order_list[i] == 0)
					printf("          ");
				else
					printf("   %2d     ", order_list[i]);
			}
			printf("\n");
		}
		printf("\n\t\t\ttotal: ");
		printf("%c[1;36m", 27);		// 글자색 변경 -> 밝은 파랑
		printf(" %d ", total);		// 총 액 출력
		printf("%c[0m", 27);		// 글자색 원래대로
		printf("won \n");
		printf("+-------------------------------------------------------------------+\n");

		// 메뉴 선택(방향키)
		key = getkey();
		switch(key){
			case 65:	// ↑ 키
				if(row > 0)	row--;
				break;
			case 66: 	// ↓ 키
				if(row < mymenu->menu_len-1)	row++;
				break;
			case 68:	// ← 키
				if(order_list[row] > 0){
					order_list[row]--;
					total -= cost_list[row];
				}
				break;
			case 67:	// → 키
				if(order_list[row] < 99){
					order_list[row]++;
					total += cost_list[row];
				}
				break;
		//	case 10:
		//		break;
			default:
				break;
		}
	}
/*
	// 주문 정보 정리
	sprintf(str, "%d", order_list[0]);
	for(int i=1;i<mymenu->menu_len;i++){
		sprintf(str2, "@%d", order_list[i]);
		strcat(str, str2);
	}
	strcpy(myorder->list_str, str);
	myorder->total = total;
*/
}

// 주문자 정보 입력
void inputCustomerInformation(order* myorder){

}

int getkey(){
	int ch;
	struct termios old, current;

	tcgetattr(0, &old);
	current = old;

	current.c_lflag &= ~(ICANON|ECHO);	// 입력값 화면 표시X
	current.c_cc[VMIN] = 1;
	current.c_cc[VTIME] = 0;
	tcsetattr(0, TCSANOW, &current);
	ch = getchar();
	tcsetattr(0, TCSANOW, &old);
	return ch;
}
