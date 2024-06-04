#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <jpeglib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUF_SIZE 4096

void error(const char *msg) {
    perror(msg);
    exit(1);
}

void capture_screen(char *filename) {
    char command[BUF_SIZE];
    snprintf(command, sizeof(command), "ffmpeg -f x11grab -y -video_size cif -i :0.0 -vframes 1 %s", filename);
    system(command);
}

void send_mjpeg_stream(int client_sock) {
    char buffer[BUF_SIZE];
    FILE *file;

    write(client_sock, "HTTP/1.0 200 OK\r\n", 17);
    write(client_sock, "Content-Type: multipart/x-mixed-replace;boundary=boundary\r\n", 59);
    write(client_sock, "\r\n", 2);

    while (1) {
        capture_screen("screen.jpg");
        file = fopen("screen.jpg", "rb");
        if (file == NULL) {
            error("ERROR opening file");
        }

        fseek(file, 0, SEEK_END);
        long file_size = ftell(file);
        fseek(file, 0, SEEK_SET);

        snprintf(buffer, sizeof(buffer), "--boundary\r\nContent-Type: image/jpeg\r\nContent-Length: %ld\r\n\r\n", file_size);
        write(client_sock, buffer, strlen(buffer));

        while (file_size > 0) {
            int bytes_read = fread(buffer, 1, sizeof(buffer), file);
            write(client_sock, buffer, bytes_read);
            file_size -= bytes_read;
        }

        write(client_sock, "\r\n", 2);
        fclose(file);
        usleep(100000); // 延迟以减少CPU使用率
    }
}

int main() {
    int sockfd, newsockfd, portno;
    socklen_t clilen;
    struct sockaddr_in serv_addr, cli_addr;

    printf("请输入端口号: ");
    scanf("%d", &portno);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        error("ERROR opening socket");
    }

    memset((char *)&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        error("ERROR on binding");
    }

    listen(sockfd, 5);
    clilen = sizeof(cli_addr);

    printf("等待客户端连接...\n");

    newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
    if (newsockfd < 0) {
        error("ERROR on accept");
    }

    printf("客户端已连接: %s\n", inet_ntoa(cli_addr.sin_addr));

    send_mjpeg_stream(newsockfd);

    close(newsockfd);
    close(sockfd);
    return 0;
}
