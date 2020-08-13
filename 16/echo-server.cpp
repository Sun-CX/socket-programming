#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <wait.h>

using namespace std;

//void handle(int sig_no) {
//    int status;
//    pid_t pid = wait(&status);
//    printf("child %d exit, status: %d\n", pid, status);
//}

void func_handle(int sig_no) {
    pid_t pid;
    int stat;
    while ((pid = waitpid(-1, &stat, WNOHANG)) > 0) {
        cout << "child pid: " << pid << ",exit status: " << stat << endl;
    }
}

void service(int con_fd) {
    char recv_buf[1024];
    while (true) {
        memset(recv_buf, 0, sizeof(recv_buf));
        auto r = read(con_fd, recv_buf, sizeof(recv_buf));
        if (r > 0) {
            cout << recv_buf;
            write(con_fd, recv_buf, sizeof(recv_buf));
        } else if (r == 0) break;//client 关闭
        else {
            cerr << "read error." << endl;
            break;
        }
    }
}

void *thread_routine(void *arg) {
    pthread_detach(pthread_self());
    int conn = *(int *) arg;
    delete (int *) arg;
    service(conn);
    cout << "thread exit." << endl;
    return nullptr;
}

int main(int argc, char *argv[]) {

    int listen_fd;
    if ((listen_fd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
        cerr << "socket open error." << endl;
        return -1;
    }

    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("192.168.0.107");
    server_addr.sin_port = htons(8080);
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

    int ret;
    pthread_t tid;
    int con_fd;
    int *p;
    while (true) {

        if ((con_fd = accept(listen_fd, (sockaddr *) &client_addr, &len)) < 0) {
            cerr << "accept error." << endl;
            return -1;
        }
        cout << "client ip: " << inet_ntoa(client_addr.sin_addr) << ", client port: " << ntohs(client_addr.sin_port)
             << endl;

        p = new int(con_fd);
        ret = pthread_create(&tid, nullptr, thread_routine, p);
        if (ret != 0) {
            cerr << strerror(ret) << endl;
            return ret;
        }

//        if (pid < 0) {
//            cerr << "fork error." << endl;
//            return -1;
//        } else if (pid == 0) {//子进程处理通信的细节
//            close(listen_fd);
//            service(con_fd);
//            break;
//        } else {//父进程用于接受连接
//            close(con_fd);
//            signal(SIGCHLD, func_handle);
//        }
    }
    return 0;
}