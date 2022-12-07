# Restaurant_Order_Service
#### 네트워크 프로그래밍 텀프로젝트

***

<h5>
 식당 주문 서비스를 구현한 프로그램이다. <br>
 서버 프로그램은 식당, 클라이언트 프로그램은 식당을 이용할 고객이 사용하게 된다.<br>
 서버에서 메뉴 정보를 클라이언트로 전송하면 클라이언트는 해당 메뉴 정보를 이용하여 메뉴를 선택한다.<br>
 주문자가 주문 정보를 입력하고 서버로 전송하면 서버는 입력받은 주문 정보를 출력한다.<br><br>
 IPv4와 TCP 프로토콜을 적용하였으며, 서버의 IP주소는 putty의 네트워크 기본 IP 주소로 지정하여 다른 PC에서도 접속할 수 있도록 하였다.<br>
 프로세스 생성을 통한 Multitasking 서버 형태의 다중 서버로 구현되어, 클라이언트들이 동시에 접속할 수 있도록 하였다.<br><br>
</h5>

<img src="https://user-images.githubusercontent.com/85846475/206102773-3f3f05ff-1916-4132-a758-9a0f5c74cca6.png" width="300">

***

<h5>
 <ol>
  <li>프로젝트명: 식당 주문 서비스</li>
  <li>2인 팀 프로젝트</li>
  <li>개발 언어: C </li>
  <li>네트워크 실행 환경: putty </li>
 </ol>
</h5>

***

### 시현 모습
#### 클라이언트

##### 1. 메뉴 선택
<h6>
 <img src="https://user-images.githubusercontent.com/85846475/206103172-114e4fc8-f391-4827-8117-72c4a8e1e498.png"><br>
 키보드의 방향키를 이용하여 메뉴의 수량을 입력한다. 수량이 0인 메뉴는 수량이 표시되지 않는다.<br>
 스페이스 바를 누르면 주문 정보가 저장된다.<br>
 메뉴 정보는 클라이언트가 서버에 요청하여 전달받아 배열 형태로 저장하여 적용한다.
</h6>

##### 2. 주문자 정보 입력
<h6>
 <img src="https://user-images.githubusercontent.com/85846475/206103621-a4f9e54b-0584-4677-9181-94ef2b2c76d1.png"><br>
 선택한 메뉴의 주분 정보를 표시하고, 주문자의 정보를 입력받는다.<br>
 주문자 정보를 입력하면 주문 내용(메뉴 선택, 주문자 정보)은 서버로 전송된다.
</h6>

##### 3. 서버 접수 확인
<h6>
 <img src="https://user-images.githubusercontent.com/85846475/206103921-bb84bd57-28bd-44d7-b93e-33f8ee9fb963.png"><br>
 서버에서 해당 주문 내용이 올바르게 수신함을 수신 시간을 통해 확인할 수 있다.<br>
 서버 접수 시간, 주문 정보를 출력하여 사용자에게 안내한다.<br>
 해당 내용은 주문 내용을 서버로 전송한 후, 서버에서 이를 다시 반환한 것이다.
</h6>

#### 서버
<h6>
 <img src="https://user-images.githubusercontent.com/85846475/206104201-86dbd106-bdca-4e39-9094-537458a277f7.png"><br>
 클라이언트가 전송한 정보에서 수신 시간이 추가된 내용을 서버의 UI에 출력한다.
</h6>

#### 다중 클라이언트

##### 1. 다중 클라이언트 동시 접속
<h6>
 <img src="https://user-images.githubusercontent.com/85846475/206104479-8ee82904-cf53-4f28-9295-7c052e9f480d.png"><br>
 여러 클라이언트가 동시에 접속한 내용이다.<br>
 클라이언트는 서버가 메뉴를 전송해줘야만 메뉴 정보를 알 수 있다.
</h6>

##### 2. 다중 클라이언트 결과
<h6>
 <img src="https://user-images.githubusercontent.com/85846475/206104637-ce963d30-8b24-4cab-b568-a72d2302954a.png"><br>
 모든 클라이언트의 주문 정보가 서버에 전송된 것을 확인할 수 있다.
</h6>

##### 3. 서버 입력 내용
<h6>
 <img src="https://user-images.githubusercontent.com/85846475/206104750-22442a02-d1b1-47e5-9a96-16ec43a2daf6.png"><br>
 다중 클라이언트 결과에 대한 서버 출력 내용
</h6>
<br><br>

***

### 주요 코드
<h5><ul>
 <li><a href="https://github.com/SE0NA/Restaurant_Order_Service/blob/main/Final/server.c">server.c</a></li>
 <li><a href="https://github.com/SE0NA/Restaurant_Order_Service/blob/main/Final/client.c">client.c</a></li>
 <li><a href="https://github.com/SE0NA/Restaurant_Order_Service/blob/main/Final/struct.h">struct.h</a></li>
</ul></h5>
