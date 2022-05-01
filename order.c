struct order{
	int num;		// 주문 번호 (< server에 입력된 순)
	char[20] name;		// 주문자 이름 (< clinet)
	char[20] phone;		// 주문자 휴대 번호 (< client)
	char[50] address;	// 주문자 주소 (< client)
	int* orderlist;		// 주문 내역 (< client) / 동적 배열 [n][2]: {메뉴, 개수}
	int cost;		// 비용 (< client)
	time_t ordertime;	// 주문 시간 (< client)
}
