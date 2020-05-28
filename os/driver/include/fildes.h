#ifndef OS_DRIVER_INCLUDE_FILDES_H_
#define OS_DRIVER_INCLUDE_FILDES_H_

#include <sys/types.h>
#include <FreeRTOS.h>
#include <semphr.h>

struct f_ops;

/**
 * @brief Defines a struct fildes
 */
struct fildes {
    int fd;
    const char *path;
    const struct f_ops *fops;
    SemaphoreHandle_t mutex;
    void *internal_device;
};

/**
 * @brief Basic operations on files
 */
struct f_ops {
    int         (*init)(struct fildes *);
    int         (*open)(struct fildes *, int);
    int         (*close)(struct fildes *);
    int         (*ioctl)(struct fildes *, unsigned long, long);
    ssize_t     (*read)(struct fildes *, void *, size_t);
    ssize_t     (*write)(struct fildes *, const void *, size_t);
    off_t       (*lseek)(struct fildes*, off_t, int);
};

/**
 * @brief Obtains a fildes object from a file descriptor number
 *
 * @param fd 
 * @return struct fildes* const 
 */
extern struct fildes * const obtain_fildes(int fd);

/**
 * @brief Stores a fildes object to the fildes subsystem
 *
 * @param fildes
 * @return int
 */
extern int store_fildes(struct fildes *fildes);

/**
 * @brief Obtain a fildes by name
 *
 * @param str
 * @return struct fildes* const
 */
extern struct fildes * const obtain_fildes_str(const char *str);

#endif // OS_DRIVER_INCLUDE_FILDES_H_
