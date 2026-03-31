#include "types.h"
#include "stat.h"
#include "user.h"

int main(int argc, char **argv){
    // fd = 1 : stdout 으로 출력
    printf(1, "Hello xv6 World\n");
    exit();
}