#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "include/ssemaphore.h"
#define MAX 10

/* structure for message queue*/
struct mesg_buffer
{
    long mesg_type;
    char mesg_text[5];
} message;

int main()
{
    key_t key;
    int msgid;

    /* ftok to generate unique key*/
    key = KeyCreate(5);

    /* msgget creates a message queue*/
    /* and returns identifier*/
    msgid = msgget(key, 0666 | IPC_CREAT);
    message.mesg_type = 1;

    message.mesg_text[0] = 'p';
    message.mesg_text[1] = 'i';
    message.mesg_text[2] = 'n';
    message.mesg_text[3] = 'g';
    message.mesg_text[4] = '\0';

    /* msgsnd to send message*/
    msgsnd(msgid, &message, sizeof(message), 0);
    msgsnd(msgid, &message, sizeof(message), 0);
    msgsnd(msgid, &message, sizeof(message), 0);
    

    /* display the message*/
    printf("Data send is : %s \n", message.mesg_text);

    return 0;
}