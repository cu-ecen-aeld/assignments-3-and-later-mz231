#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <syslog.h>

#include <sys/socket.h>
#include <netinet/in.h>

int socket_fd, connection_fd, aesdsocketdata_fd = 0;

void exit_handler(int signum) {
    printf("Caught signal, exiting\n");
    syslog(LOG_INFO, "Caught signal, exiting");
    close(socket_fd);
    close(connection_fd);
    close(aesdsocketdata_fd);
    unlink("/var/tmp/aesdsocketdata");

    exit(0);
}

int main(int argc, char *argv[]) {
    openlog(NULL, 0, LOG_USER);
    signal(SIGINT, exit_handler);
    signal(SIGTERM, exit_handler);
    int daemon_mode = 0;
    int i;
    for (i = 0; i < argc; i++) {
        if(strcmp(argv[i], "-d") == 0) {
            daemon_mode = 1;
        }
    }

    struct sockaddr_in serv_addr;

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(9000);
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    

    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(socket_fd < 0) return -1;
    
    int bindresult = bind(socket_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    if(bindresult < 0) return -1;

    if(daemon_mode) {
        int rc = fork();
        if(rc < 0) {
            return -1;
        } else if (rc != 0) {
            return 0;
        }
    }

    listen(socket_fd, 2);



    aesdsocketdata_fd = open("/var/tmp/aesdsocketdata", O_RDWR|O_TRUNC|O_CREAT, 0644);

    printf("---aesdsocket running---\n");

    while(1) {
        char recv_buffer[1024] = {0};
        connection_fd = accept(socket_fd, (struct sockaddr*)NULL, NULL);
        int bytes_received = recv(connection_fd, &recv_buffer, sizeof(recv_buffer), 0);
        while(bytes_received == sizeof(recv_buffer)) {
            write(aesdsocketdata_fd, &recv_buffer, bytes_received);
            fsync(aesdsocketdata_fd);
            bytes_received = recv(connection_fd, &recv_buffer, sizeof(recv_buffer), 0);
        }
        write(aesdsocketdata_fd, &recv_buffer, bytes_received);
        fsync(aesdsocketdata_fd);
        lseek(aesdsocketdata_fd, 0, SEEK_SET);
        char read_buffer[1024] = {0};
        int bytes_read = read(aesdsocketdata_fd, &read_buffer,sizeof(read_buffer));    
        while(bytes_read == sizeof(read_buffer)) {
            send(connection_fd, &read_buffer, bytes_read, 0);
            bytes_read = read(aesdsocketdata_fd, &read_buffer,sizeof(read_buffer));  
        }
        send(connection_fd, &read_buffer, bytes_read, 0); 
    }

}