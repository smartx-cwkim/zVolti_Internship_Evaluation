//
//  Server.c
//  TestSocket
//
//  Created by Axlis on 2017. 3. 7..
//  Copyright © 2017년 Axlis. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

#define MAX_BUFF 1000

// the thread function
void *connection_handler(void *);

int main(int argc, char *argv[]) {
    int socket_desc, client_sock, c;
    
    struct sockaddr_in server, client;
    
    if(argc != 2) {
        printf("%s PortNumber 와 같은 형식으로 사용하세요\n", argv[0]);
        exit(0);
    }
    
    // Create socket
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if(socket_desc == -1) {
        printf("소켓 생성 실패\n");
        exit(1);
    }
    
    printf("소켓 생성 완료\n");
    server.sin_family = AF_INET;
    server.sin_port = htons(atoi(argv[1]));
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    
    // Bind
    if(bind(socket_desc,(struct sockaddr *)&server, sizeof(server)) < 0) {
        printf("bind 실행 에러\n");
        exit(1);
    }
    
    printf("bind까지 성공\n");
    
    
    listen(socket_desc, 5);  // socket_desc이라는 소켓과 결합된 상태의 포트를 대기 상태로 변경.
                             // 뒷 인자 '5'는 5개의 소켓 생성 가능을 의미.
    
    printf("클라이언트 연결 대기중...\n");
    c = sizeof(struct sockaddr_in);
    
    // 연결 확인 및 전송 대기 (스레드)
    pthread_t thread_id;
    
    // 접속한 클라이언트와 통신이 가능하도록 새로운 client_sock 소켓을 생성.
    // 만약 소켓이 생성되지 않을 경우에는 -1이 반환.
    while((client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c))) {
        printf("클라이언트 연결 완료\n");
        
        // int pthread_create(pthread_t * thread, const pthread_attr_t *attr, void* (*start_routine)(void*), void *arg);
        // (1) 생성된 스레드의 ID 저장할 포인터 변수
        // (2) 스레드 특성 설정, 주로 NULL을 넣음.
        // (3) 스레드 생성된 후에 실행될 함수
        // (4) (3)을 통해 호출되는 함수에 들어갈 인자값
        if(pthread_create( &thread_id, NULL,  connection_handler, (void*) &client_sock) < 0) {
            printf("스레드를 생성하지 못했습니다.\n");
            exit(1);
        }
        
        // pthread_join은 main함수가 종료될 때까지 기다리는 함수
        // 스레드는 main함수가 종료되면 자동으로 종료되기 때문(?)
        // pthread_join( thread_id, NULL);
        printf("스레드 영역\n");
    }
    if (client_sock < 0) {
        perror("연결 실패");
        return 1;
    }
    return 0;
}

// main 함수에서 스레드 생성 후에 실행되는 함수

void *connection_handler(void *socket_desc) {
    int sock = *(int*)socket_desc;  // 형변환
    long read_size;  // 아래의 recv의 리턴형이 long이라서.
    char client_message[MAX_BUFF];
    
    // 클라이언트로부터 메시지를 읽어온다.
    while((read_size = recv(sock, client_message, 2000, 0)) > 0)  {
        // 널문자를 입력을 통해 문장 끝 설정.
        
        client_message[read_size] = '\0';
        printf("클라이언트로부터 입력된 문자열 : %s\n", client_message);
        
        // 클라이언트로부터 들어온 메시지를 다시 전송(에코서버)
        write(sock, client_message, strlen(client_message)+1);
        // client_message 배열 초기화(0으로)
        memset(client_message, 0, MAX_BUFF);
    }
    if(read_size == 0) {
        printf("연결 종료\n");
        // __fpurge(stdout);
    }
    else if(read_size == -1) {
        printf("데이터 받기 실패\n");
    }
    return 0;
}
