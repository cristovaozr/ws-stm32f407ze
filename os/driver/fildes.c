#include <driver/include/fildes.h>
#include <string.h>

// TODO: increase files array size
static struct fildes *files[1];

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
    int ret = -1;

    for(int i = 0; i < ARRAY_SIZE(files); i++) {
        if(files[i] == NULL) {
            if (fildes->fops->init(fildes)) {
                // TODO: Signal something went wrong
                break;
            }
            fildes->fd = i;
            files[i] = fildes;
            ret = 0;
            break;
        }
    }

    return ret;
}
