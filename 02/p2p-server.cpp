#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

using namespace std;

int main(int argc, char *argv[]) {
    int listen_fd;
    if ((listen_fd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
        cerr << "socket open error." << endl;
        return -1;
    }

    sockaddr_in server_addr = {.sin_family=AF_INET, .sin_port = htons(8080), .sin_addr.s_addr = inet_addr(
            "192.168.0.104")};
    //*********************************** REUSEADDR ***********************************
    int on = 1;
    if (setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0) {
        cerr << "setsockopt error." << endl;
        return -1;
    }
    //*********************************** REUSEADDR ***********************************
    if (bind(listen_fd, (sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
        cerr << "bind error." << endl;
        return -1;
    }

    if (listen(listen_fd, SOMAXCONN) < 0) {
        cerr << "listen error." << endl;
        return -1;
    }

    sockaddr_in client_addr;
    socklen_t len = sizeof(client_addr);

    int con_fd;
    if ((con_fd = accept(listen_fd, (sockaddr *) &client_addr, &len)) < 0) {
        cerr << "accept error." << endl;
        return -1;
    }
    cout << "client ip: " << inet_ntoa(client_addr.sin_addr) << ", client port: " << ntohs(client_addr.sin_port)
         << endl;

    pid_t pid = fork();

    if (pid < 0) {
        cerr << "fork error." << endl;
        return -1;
    } else if (pid == 0) {//子进程用来获取对方数据
//        signal(SIGUSR1, handle);
        char send_buf[1024];
        while (fgets(send_buf, sizeof(send_buf), stdin) != nullptr) {
            write(con_fd, send_buf, strlen(send_buf));
            memset(send_buf, 0, sizeof(send_buf));
        }
        cout << "child process close." << endl;
    } else {//父进程
        char recv_buf[1024];
        while (true) {
            memset(recv_buf, 0, sizeof(recv_buf));
            auto r = read(con_fd, recv_buf, sizeof(recv_buf));
            if (r > 0) {
                cout << recv_buf;
            } else if (r == 0) {
//                kill(pid, SIGUSR1);
                break;//client 关闭
            } else {
                cerr << "read error." << endl;
                break;
            }
        }
        cout << "parent process close." << endl;
    }
    close(con_fd);
    close(listen_fd);
    return 0;
}