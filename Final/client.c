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
#define IPserver "220.69.209.200"

void inputOrderlist(order*, menu*);
void inputCustomerInformation(order*, int);
int getkey();
void MiddleLine(unsigned char*, int);
void PrintArea(unsigned char *, int);
void orderPrint(order*, int);

char** menu_list;
int* cost_list, *order_list;

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
	
	// 주문자 정보 입력
	inputCustomerInformation(&myorder, mymenu.menu_len);

	//서버로 보내기
	write(sock, (void*)&myorder, sizeof(order));

	// 서버로부터 주문 내역반환 받음
	str_len = read(sock, (void*)&myorder, sizeof(order));

	// 완료된 주문 정보 출력
	orderPrint(&myorder, mymenu.menu_len);
	

	close(sock);

	// free memory

	return 0;
}

// 메뉴 수량 입력(방향키)
void inputOrderlist(order* myorder, menu* mymenu){
	char *str, *str2, str3[100], str4[100];
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
			case 32:	// space
				if(total > 0){
					// 주문 정보 정리
					sprintf(str3, "%d", order_list[0]);
					for(int i=1;i<mymenu->menu_len;i++){
						sprintf(str4, "@%d", order_list[i]);
						strcat(str3, str4);
					}
					strcpy(myorder->list_str, str3);
					myorder->total = total;
					return;
				}
				break;
			default:
				break;
		}
	}
}

// 주문자 정보 입력
void inputCustomerInformation(order* myorder, int menu_len){
	int space_count = 30;
	char str[100];

	// print 주문 정보
	system("clear");
	printf("+-----------------------------------------+\n");
	printf("|                주문\n");
	printf("+-----------------------------------------+\n");
	for(int i=0;i<menu_len;i++)
		if(order_list[i]!=0)
			printf("| %s  ... x%d  - %d won\n", menu_list[i], order_list[i], cost_list[i]*order_list[i]);
	printf("+-----------------------------------------+\n\n");
	
	// 주문자 정보 입력
	printf("\n 주문자 이름: ");
	fgets(str, sizeof(str), stdin);
	str[strlen(str)-1] = 0;
	strcpy(myorder->name, str);	

	printf("\n 휴대폰 번호: ");
        fgets(str, sizeof(str), stdin);
	str[strlen(str)-1] = 0;
	strcpy(myorder->phone, str);
	
	printf("\n 배송지: ");
	fgets(str, sizeof(str), stdin);
	str[strlen(str)-1]=0;
	strcpy(myorder->addr, str);
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

void MiddleLine(unsigned char *info_string, int space_count){
	printf("│  %-*s │\n", space_count, info_string);
}
void PrintArea(unsigned char *info_string, int space_count){
	int i, len=0;
	for(i=0;i<strlen(info_string);){
		if(info_string[i] & 0x80){
			len++;
			i = i+3;
		}
		else
			i++;
	}

	MiddleLine("", space_count);
	MiddleLine(info_string, space_count+len);
}

void orderPrint(order* myorder, int menu_len){
	int space_count = 54, k=0;
	char tmp[200];

	system("clear");
	printf("\n\n┌───────────────────────────────────────────────────┐\n");
	printf("│");
	printf("%c[1;33m", 27);
	printf("                  *** 주문 완료***                 ");
	printf("%c[0m",27);
	printf("│\n");
	printf("├---------------------------------------------------┤\n");
	PrintArea(myorder->ordertime, 48);
	PrintArea(myorder->name, 48);
	PrintArea(myorder->phone, 48);
	PrintArea(myorder->addr, 48);	
	printf("├---------------------------------------------------│\n");
	for(int i=0;i<menu_len;i++){
		if(order_list[i] != 0){
			sprintf(tmp, "%s ... %2d - %6d won", menu_list[i], order_list[i], cost_list[i]*order_list[i]);
			PrintArea(tmp, 48);
		}
	}
	printf("├---------------------------------------------------│\n");
	printf("│ total: %10d won                             │\n", myorder->total);
	printf("└───────────────────────────────────────────────────┘\n");
}
