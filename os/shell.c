#include <FreeRTOS.h>
#include <task.h>

#include <string.h>
#include <stdlib.h>
#include <libc/log.h>
#include <libc/ustdio.h>

#define SHELL_BANNER "> "

typedef struct FunctionList {
    const char *cmd;
    int (*functionPointer)(int, char **);
    const char *cmdHelp;
} FunctionList;

static const FunctionList cmdList[];

/**
 * @brief Parses a line and chooses a command to execute
 * 
 * @param line 
 */
static void processLine(char *line);

/**
 * @brief Greeter that shows information on Shell start
 */
static void greeter(void);

/**
 * @brief Variable that stores a line to process
 */
static char line[80];

void ShellTask(void *arg)
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
            processLine(line);
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
    for(int i = 0; cmdList[i].cmd; i++) {
        uprintf("%s: %s\r\n", cmdList[i].cmd, cmdList[i].cmdHelp);
    }

    return 0;
}

static int hexdump(int argc, char **argv)
{
    if (argc < 2) return -1;
    char *endptr = NULL;
    uint32_t addressVal = strtoul(argv[0], &endptr, 16);
    if (endptr[0] != '\0') return -1;
    uint32_t len = strtoul(argv[1], &endptr, 10);
    if (endptr[0] != '\0') return -1;
    const uint8_t *addr = (const uint8_t *)addressVal;
    HEXDUMP(addr, len);
    return 0;
}

/**
 * @brief Manipulates GPIO pins
 *
 * @param argc
 * @param argv
 * @return int
 */
// static int gpio(int argc, char **argv)
// {
//     #define ARRAY_SIZE(x) (sizeof(x)/sizeof(x[0]))
//     const static struct { const char *pinName; GpioDriver *gpio; } gpios[6] = {
//         {"reg5v", OUTPUT_REG5V_ENABLE},
//         {"user", INPUT_USER_BUTTON},
//         {"redled", OUTPUT_RED_LED},
//         {"yellowled", OUTPUT_YELLOW_LED},
//         {"sigreset", OUTPUT_SIGFOX_RESET},
//         {"sigwake", OUTPUT_SIGFOX_WAKEUP}
//     };

//     if (argv[0][0] == 'h' || argv[0][0] == 'H') {
//         uprintf("Available pins:\r\n");
//         for(int j = 0; j < ARRAY_SIZE(gpios); j++) {
//             uprintf("%s\r\n", gpios[j].pinName);
//         }
//         return 0;
//     }

//     if (argc < 2) {
//         uprintf("Usage: gpio [r/w/h] [pinName] [0/1]\r\n");
//         return -1;
//     }

//     char *gpioName = argv[1];
//     int gpioVal = strtol(argv[2], NULL, 10);
//     int i;

//     for(i = 0; i < ARRAY_SIZE(gpios); i++) {
//         if(strcmp(gpios[i].pinName, gpioName) == 0) {
//             switch(argv[0][0]) {
//             case 'r':
//             case 'R':
//                 uprintf("Pin value: %d\r\n", ReadPin((void *)gpios[i].gpio));
//                 break;

//             case 'w':
//             case 'W':
//                 WritePin((void *)gpios[i].gpio, !!gpioVal);
//                 break;

//             default:
//                 uprintf("Usage: gpio [r/w/h] [pinName] [0/1]\r\n");
//                 return -1;
//             }
//             break;
//         }
//     }

//     if(i == ARRAY_SIZE(gpios)) {
//         uprintf("pinName not found: %s\r\n", gpioName);
//         return -1;
//     }

//     return 0;
// }

static const FunctionList cmdList[] = {
    {"help", help, "Show help"},
    {"?", help, "Show help"},
    {"hexdump", hexdump, "Dumps memory. Usage: dumpmem [hexaddr] [len]"},
    // {"gpio", gpio, "Toggles a pin high or low. Usage gpio [r/w/h] [pin] [0/1]"},
    {NULL, NULL, NULL}
};

static void processLine(char *line)
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
    for(int i = 0; cmdList[i].cmd; i++) {
        if(strcmp(cmd, cmdList[i].cmd) == 0) {
            int ret = cmdList[i].functionPointer(argc, argv);
            if (ret) {
                uprintf("Error executing command: %s\r\n", cmd);
            }

            return; // Found command. Executed (?) and now quit
        }
    }

   uprintf("Command not foud: %s\r\n", cmd);
}