#ifndef LOG_H_
#define LOG_H_

#include <stdint.h>

/**
 * @brief Logging helper functions
 * 
 */

enum log_level {
    DEBUG_LVL,  // Green color
    INFO_LVL,   // White color
    WARN_LVL,   // Yellow color
    ERROR_LVL   // Red color
};

#define DBG_COLOR "\e[1m\e[32m"
#define INFO_COLOR "\e[1m\e[36m"
#define WARN_COLOR "\e[1m\e[33m"
#define ERROR_COLOR "\e[1m\e[31m"
#define END_COLOR "\e[0m"

/**
 * @brief Logs something to the default output
 * 
 * @param level LogLevel
 * @param fmt 
 * @param ... 
 */
extern void ulog(enum log_level level, const char *tag, const char *fmt, ...);

/**
 * @brief Prints log in hexadecimal format
 * 
 * @param data 
 * @param len 
 */
extern void hex_ulog(const void *data, uint32_t len);

/**
 * @brief Helper marco for DEBUG
 */
#if (RELEASE)
#define DBG(tag, fmt, ...)
#else
#define DBG(tag, fmt, ...) ulog(DBG_LVL, tag, fmt, ##__VA_ARGS__)
#endif

/**
 * @brief Helper macro for INFO
 */
#if (RELEASE)
#define INFO(tag, fmt, ...)
#else
#define INFO(tag, fmt, ...) ulog(INFO_LVL, tag, fmt, ##__VA_ARGS__)
#endif

/**
 * @brief Helper macro for WARN
 */
#if (RELEASE)
#define WARN(tag, fmt, ...)
#else
#define WARN(tag, fmt, ...) ulog(WARN_LVL, tag, fmt, ##__VA_ARGS__)
#endif

/**
 * @brief Helper macro for ERROR
 */
#define ERROR(tag, fmt, ...) ulog(ERROR_LVL, tag, fmt, ##__VA_ARGS__)

/**
 * @brief Helper macro for HexLox
 * 
 */
#if (RELEASE)
#define HEXDUMP(data, len)
#else
#define HEXDUMP(data, len) hex_ulog(data, len)
#endif 

#endif
