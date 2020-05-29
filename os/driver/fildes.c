#include <driver/include/fildes.h>
#include <string.h>

// TODO: increase files array size
static struct fildes *files[2];

#define ARRAY_SIZE(x) (sizeof((x))/sizeof((x)[0]))

struct fildes * const obtain_fildes(int fd)
{
    for(int i = 0; i < ARRAY_SIZE(files); i++) {
        if(files[i]->fd == fd)  return files[i];
    }

    return NULL;
}

int store_fildes(struct fildes *fildes)
{
    int ret;

    for(int i = 0; i < ARRAY_SIZE(files); i++) {
        if(files[i] == NULL) {
            if (fildes->fops->init(fildes)) {
                ret = -1;
                goto exit;
            }
            fildes->fd = i;
            files[i] = fildes;
            ret = 0;
            break;
        }
    }

exit:
    return ret;
}

struct fildes * const obtain_fildes_str(const char *str)
{
    for(int i = 0; i < ARRAY_SIZE(files); i++) {
        if (strcmp(files[i]->path, str) == 0) return files[i];
    }

    return NULL;
}
