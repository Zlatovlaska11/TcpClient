#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

void loadUserMessage(char **buffer, int *size) {
  int bufferSize = 10;
  int curSize = 0;
  char c;

  char *buff = (char *)malloc(bufferSize * sizeof(char));
  if (!buff) {
    printf("Memory allocation failed");
    exit(6969);
  }

  while (scanf("%c", &c) == 1 && c != '\n') {
    if (curSize >= bufferSize - 1) {
      bufferSize *= 2;
      char *temp = (char *)realloc(buff, bufferSize * sizeof(char));
      if (!temp) {
        free(buff);
        printf("Memory reallocation failed");
        exit(6969);
      }
      buff = temp;
    }

    buff[curSize++] = c;
  }

  buff[curSize] = '\0';
  *buffer = buff;
  *size = curSize;
}

int main() {
  int sock = 0;
  struct sockaddr_in serv_addr;
  char *message = NULL;
  int messageSize = 0;

  loadUserMessage(&message, &messageSize);
  char buffer[1024] = {0};

  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    printf("Socket creation error");
    free(message);
    return 1;
  }

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(6969);

  if (inet_pton(AF_INET, "192.168.17.149", &serv_addr.sin_addr) <= 0) {
    printf("Invalid address / Address not supported");
    close(sock);
    free(message);
    return 1;
  }

  if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
    printf("Connection failed");
    close(sock);
    free(message);
    return 1;
  }

  printf("Connected to server\n");

  send(sock, message, strlen(message), 0);
  printf("Message sent: %s\n", message);

  int valread = read(sock, buffer, sizeof(buffer) - 1);
  if (valread > 0) {
    buffer[valread] = '\0';
    printf("Server response: %s\n", buffer);
  } else {
    printf("No response or error reading from server\n");
  }

  free(message);
  close(sock);
  return 0;
}
