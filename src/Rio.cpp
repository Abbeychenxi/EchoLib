#include "Rio.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#define ERR_EXIT(m) \
    do { \
        perror(m);\
        exit(EXIT_FAILURE);\
    }while(0)

Rio::Rio(int fd)
    :fd_(fd),
     left_(0),
     bufptr_(buffer_)
{
}

ssize_t Rio::read(char *usrbuf, size_t n)
{
    ssize_t nread;
    while(left_ <= 0)
    {
        nread = ::read(fd_, buffer_, sizeof buffer_);
        if(nread == -1)
        {
            if(errno == EINTR)
                continue;
            return -1;
        }else if(nread == 0)
            return 0;

        left_ = nread;
        bufptr_ = buffer_;
    }

    int cnt = n;
    if(left_ < static_cast<int>(n))
        cnt = left_;
    ::memcpy(usrbuf, bufptr_, cnt);
    left_ -= cnt;
    bufptr_ += cnt;

    return cnt;
}

ssize_t Rio::readn(char *usrbuf, size_t n)
{
    size_t nleft = n;
    ssize_t nread;
    char *bufp = usrbuf;

    while(nleft > 0)
    {
        if((nread = read(bufp, nleft)) == -1)
            return -1;  /* 这里不需要处理EINTR  why */
        else if(nread == 0)
            break;

        nleft -= nread;
        bufp += nread;
    }

    return (n - nleft);
}


ssize_t Rio::readline(char *usrbuf, size_t maxlen)
{
    char *bufp = usrbuf;
    char c;
    int nread;
    int i;
    for(i = 0; i < static_cast<int>(maxlen - 1); ++i)
    {
        if((nread = read(&c, 1)) == -1)
            return -1;
        else if(nread == 0)
        {
            if(i == 0) //第一个字符读取遇到EOF
                return 0;
            break;  /* 这里可能已经读取了部分字节  */
        }

        *bufp++ = c; //字符放入缓冲区
        if(c == '\n')
            break;
    }
    *bufp = '\0';
    return i;   /* 返回成功读取的字节数，不包括最后的\0  */
}

ssize_t Rio::writen(const char *usrbuf, size_t n)
{
    size_t nleft = n;
    ssize_t nwrite;

    const char *bufp = usrbuf;

    while(nleft > 0)
    {
        //nwrite == 0也属于错误
        if((nwrite = write(fd_, bufp, nleft)) <= 0)
        {
            if(errno == EINTR)
                nwrite = 0;
            else
                return -1; // -1 和 0
        }

        nleft -= nwrite;
        bufp += nwrite;
    }
    return n;  //这里不是 n- nleft 必须是n
}




