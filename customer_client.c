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

void InitialScreen();
void SelectMenu();

int main(int argc, char**argv){
	
}
