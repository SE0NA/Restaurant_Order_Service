typedef struct order{
	int no;		// 주문번호(서버)
	char name[20];	// 주문자 이름(클라이언트)
	char phone[20];	// 주문자 휴대번호(클라이언트)
	char addr[20];	// 주문 배송지(클라이언트)
	char list_str[100];
	int total;
	char ordertime[50];
	struct order* next;
} order;

typedef struct menu{
	int menu_len;		// 총 메뉴 수
	char cost_str[256];
	char menu_str[254];
} menu;
