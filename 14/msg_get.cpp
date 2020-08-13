#include <iostream>
#include <sys/msg.h>

using namespace std;

int main(int argc, char *argv[]) {
    int msg_id = msgget(1234, 0666 | IPC_CREAT);
    if (msg_id < 0) {
        cerr << "msgget error." << endl;
        return -1;
    }
    cout << msg_id << endl;

    msqid_ds buf;

    msgctl(msg_id, IPC_STAT, &buf);

//    sscanf("600", "%o", &buf.msg_perm.mode);
    buf.msg_perm.mode = 0600;
    msgctl(msg_id, IPC_SET, &buf);
    msgctl(msg_id, IPC_RMID, nullptr);
    return 0;
}