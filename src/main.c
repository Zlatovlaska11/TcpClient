#include <arpa/inet.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 6969

void *receiveMessages(void *arg) {
  int sock = *(int *)arg;
  char buffer[1024];

  while (1) {
    int valread = read(sock, buffer, sizeof(buffer) - 1);
    if (valread > 0) {
      buffer[valread] = '\0';
      printf("\n[Server] %s\n", buffer);
      printf("Enter message: "); // prompt again after incoming message
      fflush(stdout);
    } else if (valread == 0) {
      printf("\n[Server disconnected]\n");
      exit(0);
    } else {
      printf("read");
      break;
    }
  }
  return NULL;
}

void *getUserInput(char **buffer, int *size) {
  int bufferSize = 64;
  int curSize = 0;
  char c;

  char *buff = (char *)malloc(bufferSize);
  if (!buff) {
    printf("malloc");
    exit(-1);
  }

  while (scanf("%c", &c) == 1 && c != '\n') {
    if (curSize >= bufferSize - 1) {
      bufferSize *= 2;
      char *temp = (char *)realloc(buff, bufferSize);
      if (!temp) {
        free(buff);
        printf("realloc");
        exit(-1);
      }
      buff = temp;
    }
    buff[curSize++] = c;
  }
  buff[curSize] = '\0';
  *buffer = buff;
  *size = curSize;
  return NULL;
}

int main() {
  int sock = 0;
  struct sockaddr_in serv_addr;
  pthread_t recvThread;

  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    printf("Socket creation error");
    return -1;
  }

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(PORT);

  if (inet_pton(AF_INET, "192.168.17.149", &serv_addr.sin_addr) <= 0) {
    printf("Invalid address / Address not supported");
    return -1;
  }

  if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
    printf("Connection failed");
    return -1;
  }

  printf("Connected to server.\n");

  if (pthread_create(&recvThread, NULL, receiveMessages, &sock) != 0) {
    printf("pthread_create");
    return -1;
  }

  while (1) {
    printf("Enter message: ");
    char *message = NULL;
    int messageSize = 0;
    getUserInput(&message, &messageSize);

    if (strcmp(message, "exit") == 0) {
      free(message);
      break;
    }

    send(sock, message, messageSize, 0);
    free(message);
  }

  close(sock);
  return 0;
}
