//
//  Client.c
//  TestClient
//
//  Created by Axlis on 2017. 3. 7..
//  Copyright © 2017년 Axlis. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>

#define MAX_BUFF 1024

// LinkedList를 위한 구조체 정의
typedef struct _Data {
    char *value;
    struct _Data *next;
} Data;

// LinkedList 최상단과 최하단을 맡을 놈들..
Data *head;
Data *tail;


// LinkedList 생성과 Queue, Stack과 관련된 함수 선언
void init_linkedlist(void);
void push(char *);
void pop(void);
void printStack_list(void);
void EnQueue(char *);
void DeQueue(void);
void PrintQueue_list(void);

// main함수에서 인자값을 받기 위함.
int main(int argc, char * argv[]) {
    int client_socket;
    char buff_snd[MAX_BUFF];
    char *serverIP = argv[1];  // 서버의 주소(보통은 localhost..)
    
    // sockaddr을 소켓 통신에 사용하기 위해 재정의된 구조체
    // 자세한 내용은 "https://kldp.org/node/103123" 을 통해 확인 가능.
    struct sockaddr_in server_adder;
    
    int x, i, cnt=0;
    char input[MAX_BUFF][MAX_BUFF];  // 데이터를 입력 받을 때
                                     // 해당 부분을 이차원 배열로 한 이유는 후술. 이차원 배열의 포인터로 하면 char (*input)[]이 되겠지..
    
    
    // 입력인자가 3개가 아닌 경우 (argv[0]는 이 코드의 빌드를 통해 생성되는 실행 파일.)
    if(argc != 3) {
        printf("%s IP_Address PortNumber 와 같은 형식으로 사용하세요\n", argv[0]);
        exit(0);
    }
    
    init_linkedlist();
    
    client_socket = socket(PF_INET, SOCK_STREAM, 0);
    
    if(client_socket == -1) {
        printf("소켓 생성 실패\n");
        return 0;
    }
    
    memset(&server_adder, 0, sizeof(server_adder));
    server_adder.sin_family = AF_INET;
    server_adder.sin_port = htons(atoi(argv[2]));
    // server_adder.sin_addr.s_addr = inet_addr("192.168.0.149");  // 서버의 주소가 들어가는 부분. (localhost는 확인 필요)
    // 문자열로 입력 받은 IP주소를 128비트 2진 데이터로 변환하는 함수. (범용)
    inet_pton(AF_INET, serverIP, &server_adder.sin_addr);
    
    
    if(connect(client_socket, (struct sockaddr *)&server_adder, sizeof(server_adder)) == -1) {
        printf("접속 실패\n");
        return 0;
    }
    
    printf("서버에 접속 성공\n");
    
    printf("원하는 자료 구조 선택(Queue:1, Stack:2) : ");
    scanf("%d", &x);
    getchar(); // Why not fflush(stdin)? -> Windows, POSIX UNIX 일부에서만 동작
               // __fpurge, fpurge(deprecate)가 대신한다. 다만 이것은 해당 스트림버퍼를 깔끔하게 비워버린다...ㅎㄷㄷ
               // 근데 문제는... __fpurge가 여기 없ㅋ음ㅋ
    
    
    if(x==1) {
        while(1) {
            printf("input : ");
            fgets(buff_snd, sizeof(buff_snd), stdin);
            buff_snd[strlen(buff_snd)]='\0';  // buff_snd[strlen(buff_snd)-1]='\0';
            // 데이터 보내기
            write(client_socket, buff_snd, strlen(buff_snd));
            
            // 서버로부터 들어온 데이터 읽기.
            read(client_socket, input[cnt], MAX_BUFF);
            input[cnt][strlen(input[cnt])-1]='\0';
            printf("서버로부터 들어온 데이터 : %s\n", input[cnt]);
            
            // 들어온 데이터가 quit이면 종료!!
            if(strcmp(input[cnt], "quit") == 0) {
                break;
            }
            // 만약 1차원 배열(input[cnt])라고 하는 경우에는 문제가 발생.
            // 들어간 값 위에 또 중첩과 같은.(마지막에 들어온 문자열로 모조리 바뀌는..)
            EnQueue(input[cnt]);
            cnt++;
        }
        
        PrintQueue_list();
        
        for(i=0;i<cnt;i++) {
            DeQueue();
        }
    }
    else if(x==2) {
        while(1) {
            printf("input : ");
            fgets(buff_snd, sizeof(buff_snd), stdin);
            buff_snd[strlen(buff_snd)]='\0';  // buff_snd[strlen(buff_snd)-1]='\0';
            write(client_socket, buff_snd, strlen(buff_snd));
            
            read(client_socket, input[cnt], MAX_BUFF);
            input[cnt][strlen(input[cnt])-1]='\0';
            printf("서버로부터 들어온 데이터 : %s\n", input[cnt]);
            
            if(strcmp(input[cnt], "quit") == 0) {
                break;
            }
            
            push(input[cnt]);
            cnt++;
        }
        
        printStack_list();
        
        for(i=0;i<cnt;i++) {
            pop();
        }
    }
    
    // 클라이언트는 연결 종료.
    close(client_socket);
    
    return 0;
}

// LinkedList 관련 생성 함수
void init_linkedlist() {
    // 최상위, 최하위에 대한 동적할당.
    head = (Data *)malloc(sizeof(Data));
    tail = (Data *)malloc(sizeof(Data));
    
    head->next = tail;
    tail->next = tail;
}

// Stack 관련 함수
void push(char *input) {
    Data *newData, *temp;
    
    temp = head->next;
    
    newData = (Data *)malloc(sizeof(Data));
    
    newData->value = input;
    
    // printf("%s\n", newData->value);
    newData->next = temp;
    head->next = newData;
}

void printStack_list(void) {
    Data *printData;
    
    printf("현재 LinkedList에 저장된 것을 Stack 방식으로 추출(LIFO)\n");
    
    for (printData = head->next; printData != tail; printData = printData->next) {
        printf("%s\n", printData->value);
    }
    
    printf("===============================================\n");
}

void pop(void) {
    Data *nextData;
    
    if(head->next == tail) {
        printf("Stack Underflow\n");
        exit(0);
    }
    
    nextData = head->next;
    
    printf("LIFO 방식으로 제거 시작\n");
    head->next = nextData->next;
    printf("%s 추출\n", nextData->value);
    
    free(nextData);
}


// Queue 관련 함수
void EnQueue(char *x) {
    Data *newData, *temp;
    
    newData = (Data *)malloc(sizeof(Data));
    temp = head;
    
    newData->value = x;
    
    while(temp->next != tail) {
        temp = temp->next;
    }
    
    newData->next = tail;
    temp->next = newData;
}


void DeQueue() {
    Data *nextData;
    
    if(head->next == tail) {
        printf("Queue Underflow\n");  // 모든 큐가 소비되었으면 그냥 여기를 조지면 된다.
        exit(0);
    }
    
    printf("FIFO 방식으로 제거 시작\n");
    nextData = head->next;
    head->next = nextData->next;
    printf("%s 추출\n", nextData->value);
    
    free(nextData);
}

void PrintQueue_list(void) {
    Data *printData;
    
    printf("현재 LinkedList에 저장된 것을 Queue 방식으로 추출(FIFO)\n");
    
    for(printData = head->next;printData != tail;printData = printData->next) {
        printf("%s\n", printData->value);
    }

    printf("===============================================\n");
}
