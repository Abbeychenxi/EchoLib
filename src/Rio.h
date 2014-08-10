#ifndef RIO_H
#define RIO_H 

#include "NonCopyable.h"
#include <stdio.h>
#define MAXLINE 1024
#define RIO_BUFFER 8192

class Rio : NonCopyable
{
    public:
        explicit Rio(int fd);        

        ssize_t readn(char *usrbuf, size_t n);
        ssize_t readline(char *usrbuf, size_t n);
        ssize_t writen(const char *usrbuf, size_t n);

    private:
        ssize_t read(char *usrbuf, size_t n);

        int fd_;
        int left_;
        char *bufptr_;
        char buffer_[RIO_BUFFER];
};

#endif  /*RIO_H*/
