typedef struct order{
	int no;		// 주문번호(서버)
	char[20] name;	// 주문자 이름(클라이언트)
	char[20] phone;	// 주문자 휴대번호(클라이언트)
	char[30] add;	// 주문 배송지(클라이언트)
	int** list;	// 주문 리스트 [menu_len][2]
} order;

typedef struct menu{
	int menu_len;		// 총 메뉴 수
	char[512] menu_str;	// 메뉴 내용
} menu;
