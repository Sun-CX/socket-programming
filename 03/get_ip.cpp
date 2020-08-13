#include <zconf.h>
#include <iostream>
#include <netdb.h>
#include <arpa/inet.h>
#include <cstring>

using namespace std;

/**
 * 获取本机 IP
 * @param ip
 * @return
 */
int get_local_ip(char *ip) {
    char host[128];
    gethostname(host, sizeof(host));
    hostent *hp = gethostbyname(host);
    strcpy(ip, inet_ntoa(*(in_addr *) hp->h_addr));
    return 0;
}

//
// Created by suncx on 2020/3/19.
//
int main(int argc, char *argv[]) {

//    char host[100];
//    gethostname(host, sizeof(host));
//
//    cout << host << endl;
//
//    hostent *hp = gethostbyname(host);
//
//    for (int i = 0; hp->h_addr_list[i] != nullptr; i++) {
//        cout << inet_ntoa(*(in_addr *) hp->h_addr_list[i]) << endl;
//    }

    char ip[15];
    get_local_ip(ip);
    cout << ip << endl;
    return 0;
}
