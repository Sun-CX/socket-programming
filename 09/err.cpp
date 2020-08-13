//
// Created by suncx on 2020/3/29.
//

#include <iostream>

#define ERR_EXIT(m) \
    do{ \
        perror(m); \
        exit(EXIT_FAILURE); \
    } while(0)

int main(int argc, char *argv[]) {

    perror("message");
    return 0;
}
