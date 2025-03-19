#include "assertTools.h"
#include <stdlib.h>
#include <sys/stat.h>
#include <errno.h>
#undef errno
extern int errno;

extern int errno;
extern int __io_getchar(void) __attribute__((weak));

int _getpid(void)
{
    return 1;
}

int _kill(int, int)
{
    errno = EINVAL;
    return -1;
}

void _exit(int status)
{
    _kill(status, -1);
    ASSERT(false);
}

int _read(int, char* ptr, int len)
{
    int DataIdx;

    for(DataIdx = 0; DataIdx < len; DataIdx++)
    {
        *ptr++ = __io_getchar();
    }

    return len;
}

int _close(int)
{
    return -1;
}

int _fstat(int, struct stat* st)
{
    st->st_mode = S_IFCHR;
    return 0;
}

int _isatty(int)
{
    return 1;
}

int _lseek(int, int, int)
{
    return 0;
}

int _open(char*, int, ...)
{
    return -1;
}
