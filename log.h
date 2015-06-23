/**
 * @file log.h
 * @brief a thread-safe log class used in multithreading
 * @author duanmeng
 * @version 1.0
 * @date 2015-06-18
 */

#ifndef __LOG_H__
#define __LOG_H__

#include <cstdio> // FILE
#include <cstdarg> // va_list, va_list, va_start, va_end
#include <string>
#include <pthread.h>

namespace logging {

class Log {
public:
    enum LogLevel {
        DEBUG   = 0,
        INFO    = 1,
        WARNING = 2,
        ERROR   = 3,
        FATAL   = 4,
        LOG_LEVEL_CNT
    };

    Log();

    static Log& get_logger();
    static void init_log(const char* path, int level);

    void debug(const char* fmt, ...);
    void info(const char* fmt, ...);
    void warning(const char* fmt, ...);
    void error(const char* fmt, ...);
    void fatal(const char* fmt, ...);

private:
    static Log log_;

    void log_helper(int level, const char* fmt, va_list ap);
    FILE* get_log_file(const char* file_path);

    FILE* log_file_;
    std::string log_file_path_;
    std::string last_log_file_path_;
    int log_level_;
    
    //disable copy and assign
    Log(const Log&);
    void operator=(const Log&);
};

} // namespace logging

#define DEBUG(format, ...) \
    logging::Log::get_logger().debug(format, ##__VA_ARGS__)
#define INFO(format, ...) \
    logging::Log::get_logger().info(format, ##__VA_ARGS__)
#define WARNING(format, ...) \
    logging::Log::get_logger().warning(format, ##__VA_ARGS__)
#define ERROR(format, ...) \
    logging::Log::get_logger().error(format, ##__VA_ARGS__)
#define FATAL(format, ...) \
    logging::Log::get_logger().fatal(format, ##__VA_ARGS__)

#endif // __LOG_H__
