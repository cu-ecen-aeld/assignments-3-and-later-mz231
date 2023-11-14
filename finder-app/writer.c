#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <syslog.h>


int main(int argc, char *argv[]) {
    openlog(NULL, 0, LOG_USER);
    if(argc < 3) {
        printf("Two arguments are required\n");
        syslog(LOG_ERR, "Two arguments are required\n");
        return 1;
    }
    char *writefile = argv[1];
    char *writestr = argv[2];

    int fd;
    fd = open(writefile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if(fd == -1) {
        printf("error opening a file: %s\n", strerror( errno ));
        syslog(LOG_ERR, "error opening a file: %s\n", strerror( errno ));
        return 1;
    }

    ssize_t bytes_written = write(fd, writestr, strlen(writestr));
    if(bytes_written == -1) {
        printf("eror writing to file %s\n:", strerror( errno ));
        syslog(LOG_ERR, "eror writing to file %s\n:", strerror( errno ));
        return 1;
    }

    syslog(LOG_DEBUG, "Writing %s to %s", writestr, writefile);
    
    return 0;

}