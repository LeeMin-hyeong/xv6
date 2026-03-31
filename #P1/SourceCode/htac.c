#include "types.h"
#include "stat.h"
#include "user.h"

int line;

void
htac(int fd)
{
    int n;               // 버퍼 인덱스
    int line_cnt = 0;    // 출력한 라인 수 카운터
    struct stat st;      // 파일 stat 변수

    // fstat 함수로 인자로 받은 파일의 크기(byte 수)를 반환받음
    fstat(fd, &st);
    // 파일 크기 만큼 버퍼 할당
    char buf[st.size];

    // 파일을 버퍼에 불러들임
    if((n = read(fd, buf, sizeof(buf))) < 0){
        printf(1, "htac: read error");
        exit();
    }

    // write를 위한 라인 글자 수 카운터
    int char_cnt = 0;

    while(n-- >= -1){
        if(buf[n] == '\n' || n == -1){
            // 버퍼 인덱스로부터 글자 수 카운터(개행 제외)만큼 출력
            // 현재 버퍼 인덱스가 개행이므로 개행 다음 문자부터 출력
            // 파일의 처음에 도달한 경우 바로 출력
            write(1, &buf[n+1], char_cnt-1);
            // 파일의 마지막 줄의 경우 개행의 존재가 불확실하므로 개행은 개별적으로 출력
            printf(1, "\n");
            // 글자 수 카운터 초기화
            char_cnt = 0;
            // 출력 목표 라인 수를 달성하면 break
            if(line <= ++line_cnt) break;
        }
        char_cnt++;
    }
}

int
main(int argc, char *argv[])
{
    int fd;

    // 인자 개수가 두 개가 아니라면 실행되지 않도록 처리
    if(argc != 3){
        printf(1, "htac: takes 2 argument but %d were given", argc-1);
        exit();
    }

    // 출력될 라인 수를 전역 변수에 저장
    line = atoi(argv[1]);
    if(line <= 0) exit();

    if((fd = open(argv[2], 0)) < 0){
        printf(1, "htac: cannot open %s\n", argv[2]);
        exit();
    }

    htac(fd);
    close(fd); // 파일 사용 후 닫기
    exit();
}
