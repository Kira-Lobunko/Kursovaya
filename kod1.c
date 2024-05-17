#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>

// Определение структуры сообщения
struct msg_buffer {
	long msg_type;
	char msg_text[100];
};

int main(){
	key_t key;
	int msgid;

	// Создаём уникальный ключ для очереди сообщений
	key = ftok("progfile", 65);

	// Создаём очередь сообщений и возвращаем индетификатор
	msgid = msgget(key, 0666 | IPC_CREAT);
	if (msgid == -1){
		perror("msgett");
		exit(EXIT_FAILURE);
	}

	// Иницилизируем структуру сооббщения для отправки
	struct msg_buffer message;
	message.msg_type = 1;
	strcpy(message.msg_text, "Hello, this is a messege!");

	// Отправляем сообщение в очередь
	if (msgsnd(msgid, &message, sizeof(message.msg_text), 0) ==  -1){
		perror("msgsnd");
		exit(EXIT_FAILURE);
	}

	printf("Sent message: %s\n", message.msg_text);

	// Получаем сообщение из очереди
	if (msgrcv(msgid, &message, sizeof(message.msg_text), 1, 0) == -1){
		perror("msgrcv");
		exit(EXIT_FAILURE);
	}

	printf("Received message: %s\n", message.msg_text);

	// Удаляем очередь сообщений
	if (msgctl(msgid, IPC_RMID, NULL) == -1) {
		perror("msgctl");
		exit (EXIT_FAILURE);
	}

	return 0;
}

