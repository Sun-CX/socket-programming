#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <arpa/inet.h>
#include <unistd.h>
#include <wait.h>

using namespace std;

void func_handle(int sig_no) {
    pid_t pid;
    int stat;
    while ((pid = waitpid(-1, &stat, WNOHANG)) > 0) {
        cout << "child pid: " << pid << ",exit status: " << stat << endl;
    }
}

//自定义消息类型：包头指定消息体的实际长度
struct packet {
    int len;
    char buf[1024];
};

void service(int con_fd) {
    packet recv_msg;
    while (true) {
        auto r = read(con_fd, &recv_msg.len, sizeof(recv_msg.len));
        if (r == sizeof(recv_msg.len)) {
            decltype(recv_msg.len) len = ntohl(recv_msg.len);
            r = read(con_fd, recv_msg.buf, len);
            if (r < 0) {
                cerr << __LINE__ << ": read error." << endl;
                break;
            } else if (r >= 0 and r < len) {
                cerr << "msg error." << endl;
                break;
            } else {
                printf("%.*s", len, recv_msg.buf);
                write(con_fd, &recv_msg, len + sizeof(recv_msg.len));
            }
        } else {
            cerr << __LINE__ << ": has read: " << r << ", read error." << endl;
            break;
        }
    }
}

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
    pid_t pid;

    while (true) {
        if ((con_fd = accept(listen_fd, (sockaddr *) &client_addr, &len)) < 0) {
            cerr << "accept error." << endl;
            return -1;
        }
        cout << "client ip: " << inet_ntoa(client_addr.sin_addr) << ", client port: " << ntohs(client_addr.sin_port)
             << endl;

        pid = fork();
        if (pid < 0) {
            cerr << "fork error." << endl;
            return -1;
        } else if (pid == 0) {//子进程处理通信的细节
            close(listen_fd);
            service(con_fd);
            break;
        } else {//父进程用于接受连接
            close(con_fd);
            signal(SIGCHLD, func_handle);
        }
    }
    return 0;
}