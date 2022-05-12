#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
//중간 부분 출력방법
int MiddleLine(unsigned char *info_string, int space_count)
{
        printf("│  %-*s│\n", space_count, info_string);
}

//고객 정보 출력창UI
void PrintArea(unsigned char *info_string, int space_count)
{
        //중간 줄
        if(*info_string > 127 )  //한글일 때
        {
        MiddleLine("", space_count);
        MiddleLine(info_string, space_count+3);
        MiddleLine("", space_count);
        }
        else
        {
        MiddleLine("", space_count);
        MiddleLine(info_string, space_count);
        MiddleLine("", space_count);
        }


}

void TopLine(int space_count)
{
        int i;
        //첫 줄
        printf("┌");
        for(i = 0; i < space_count+2; i++)
                printf("─");
        printf("┐\n");

}
void BottomLine(int space_count)
{
        int i;
        //마지막 줄
        printf("└");
        for(i = 0; i < space_count+2; i++)
                printf("─");
        printf("┘\n");

}
int main()
{
        char name[10];
        char num[30];
        char addr[100];

        //print 주문 정보
        system("clear");
        printf("+------------------------------------+\n");
        printf("|                주문\n");
        for(int i=0;i<3;i++)
                printf("주문정보\n\n\n");
        printf("+------------------------------------+\n\n");

        //주문자 정보 입력
        printf("이름 입력: ");
        scanf("%s", name);
        printf("번호 입력: ");
        scanf("%s", num);
        printf("배송지 입력: ");
        scanf("%s", addr);
        printf("\n\n\n");

        //출력UI
        system("clear");
        TopLine(30);
        PrintArea("         주문자 정보          ", 30);
        PrintArea("-----------------------------",30);
        PrintArea(name,30);
        PrintArea(num, 30);
        PrintArea(addr, 30);
        BottomLine(30);

        return 0;
}
