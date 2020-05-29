#include "FreeRTOS.h"
#include "task.h"
#include "libc/log.h"
#include "libc/ustdio.h"
#include "driver/include/ioctl_values.h"
#include "newlib/syscalls.h"
#include <string.h>
#include <stdlib.h>

#define SHELL_BANNER "> "

struct function_list {
    const char *cmd;
    int (*fp)(int, char **);
    const char *help_str;
};

static const struct function_list cmd_list[];

/**
 * @brief Parses a line and chooses a command to execute
 * 
 * @param line 
 */
static void process_line(char *line);

/**
 * @brief Greeter that shows information on Shell start
 */
static void greeter(void);

/**
 * @brief Variable that stores a line to process
 */
static char line[80];

void shell_task(void *arg)
{
    (void)arg;
    int pos = 0;

    greeter();

    uprintf(SHELL_BANNER);

    while(1) {
        char c = ugetchar();

        switch (c) {
        case '\b': // Backspace
        case 0x7f:
            if (pos > 0) uputchar(c);
            pos = ((pos - 1) < 0) ? 0 : pos - 1;
            break;

        case '\n':
        case '\r':
            uprintf("\r\n");
            line[pos] = '\0'; // EOL
            process_line(line);
            pos = 0;
            uprintf(SHELL_BANNER); // New line
            break;
        
        default:
            // If line buffer is full stop printint chars
            if (pos > sizeof(line)) break;
            uputchar(c);
            line[pos++] = c;
            break;
        }
    }
}

void greeter(void)
{
    uprintf("Shell\r\n");
    uprintf("Build date: %s, %s\r\n", __DATE__ , __TIME__);
}

/**
 * @brief Prints help
 * 
 * @param argc 
 * @param argv 
 * @return int 
 */
static int help(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    uprintf("\r\n");
    for(int i = 0; cmd_list[i].cmd; i++) {
        uprintf("%s: %s\r\n", cmd_list[i].cmd, cmd_list[i].help_str);
    }

    return 0;
}

static int hexdump(int argc, char **argv)
{
    if (argc < 2) return -1;
    char *endptr = NULL;
    uint32_t addr = strtoul(argv[0], &endptr, 16);
    if (endptr[0] != '\0') return -1;
    uint32_t len = strtoul(argv[1], &endptr, 10);
    if (endptr[0] != '\0') return -1;
    const uint8_t *uaddr = (const uint8_t *)addr;
    HEXDUMP(uaddr, len);
    return 0;
}

/**
 * @brief Manipulates GPIO pins
 *
 * @param argc
 * @param argv
 * @return int
 */
static int gpio(int argc, char **argv)
{
    int fd = 0, ret = 0;
    struct gpio_ioctl gpio;

    if((fd = open("gpio0", 0)) < 0) {
        ERROR("shell", "Problem opening 'gpio0'");
        ret = -1;
        goto exit;
    }

    switch (argv[0][0]) {
    case 'r':
    case 'R':
        gpio.pin = strtol(argv[1], NULL, 10);
        if (ioctl(fd, IOCTL_GPIO_READ_PIN, &gpio)) {
            ERROR("gpio", "Problem reading pin [%d]", gpio.pin);
            ret = -1;
            goto exit;
        }
        INFO("gpio", "Value read for pin %d is: %d", gpio.pin, gpio.value);
        ret = 0;
        break;
    
    case 'w':
    case 'W':
        WARN("gpio", "Write values unimplemented yet");
        break;

    default:
        ERROR("shell", "Unknown argument: %c", argv[0][0]);
        ret = -1;
        break;
    }

exit:
    if (fd > 0) {
        close(fd);
    }
    return ret;
}

static const struct function_list cmd_list[] = {
    {"help", help, "Show help"},
    {"?", help, "Show help"},
    {"hexdump", hexdump, "Dumps memory. Usage: dumpmem [hexaddr] [len]"},
    {"gpio", gpio, "Toggles a pin high or low. Usage gpio [r/w] [pin] [0/1]"},
    {NULL, NULL, NULL}
};

static void process_line(char *line)
{
    char *saveptr, *cmd, *argv[8];
    int argc;

    // Extracts command name
    cmd = strtok_r(line, " ", &saveptr);
    if (cmd == NULL) return;

    // Extracts arguments
    for(argc = 0; argc < 8; argc++) {
        char *token = strtok_r(NULL, " ", &saveptr);
        if (token == NULL) break;
        argv[argc] = token;
    }

    // Looks for a command in the command list
    for(int i = 0; cmd_list[i].cmd; i++) {
        if(strcmp(cmd, cmd_list[i].cmd) == 0) {
            int ret = cmd_list[i].fp(argc, argv);
            if (ret) {
                uprintf("Error executing command: %s\r\n", cmd);
            }

            return; // Found command. Executed (?) and now quit
        }
    }
   uprintf("Command not foud: %s\r\n", cmd);
}