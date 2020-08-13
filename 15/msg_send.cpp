#include <iostream>
#include <sys/msg.h>

using namespace std;

int main(int argc, char *argv[]) {
    if (argc != 3) {
        cerr << "param error" << endl;
        return -1;
    }

    auto len = atoi(argv[1]);
    auto type = atoi(argv[2]);


    int msg_id = msgget(1234, 0600 | IPC_CREAT);
    if (msg_id < 0) {
        cerr << "msgget error." << endl;
        return -1;
    }


    msgbuf *buf = new msgbuf;
    buf->mtype = type;

    msgsnd(msg_id, buf, len, 0);
    return 0;
}