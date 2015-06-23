#include "log.h"
#include <time.h> // localtime_r, struct tm, time_t, time
#include <sys/time.h> // gettimeofday, struct time_val
#include <string.h> // strlen

namespace logging {
namespace internal {

const char* log_level_name[] = {
        "DEBUG  ",
        "INFO   ",
        "WARNING",
        "ERROR  ",
        "FATAL  "
};

enum {
    MAX_LOG_FILE_PATH_LEN = 256,
    MAX_LOG_ITEM_LEN = 1024,
    MAX_LOG_TIME_BUF_LEN = 32
};

class Mutex {
public:
    Mutex() {
        pthread_mutex_init(&mutex_, NULL);
    }

    ~Mutex() {
        pthread_mutex_destroy(&mutex_);
    }

    int lock() {
        return pthread_mutex_lock(&mutex_);
    }

    int unlock() {
        return pthread_mutex_unlock(&mutex_);
    }

private:
    pthread_mutex_t mutex_;
};

Mutex& get_locker()
{
    static Mutex mu;
    return mu;
}

} // namespace internal

Log Log::log_;

Log::Log() : log_file_(NULL), log_level_(DEBUG) {
}

Log& Log::get_logger() {
    return log_;
}

void Log::init_log(const char* path, int level) {
    internal::get_locker();
    if (path) {
        log_.log_file_path_.assign(path);
    }
    if (level >= DEBUG || level < LOG_LEVEL_CNT) {
        log_.log_level_ = level;
    }
}

void Log::debug(const char* fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    log_helper(DEBUG, fmt, ap);
    va_end(ap);
}

void Log::info(const char* fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    log_helper(INFO, fmt, ap);
    va_end(ap);
}

void Log::warning(const char* fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    log_helper(WARNING, fmt, ap);
    va_end(ap);
}

void Log::error(const char* fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    log_helper(ERROR, fmt, ap);
    va_end(ap);
}

void Log::fatal(const char* fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    log_helper(FATAL, fmt, ap);
    va_end(ap);
}

void Log::log_helper(int level, const char* fmt, va_list ap)
{
    if (level < log_level_) {
        return;
    }

    // event time file:line threadid \t userlog
    char log_item_buf[internal::MAX_LOG_ITEM_LEN]; 
    char log_time_buf[internal::MAX_LOG_TIME_BUF_LEN];

    // get time info
    struct timeval cur_time;
    gettimeofday(&cur_time, NULL);
    time_t time_sec = cur_time.tv_sec;
    long int msec = cur_time.tv_usec / 1000;
    struct tm cur_time_tm;
    localtime_r(&time_sec, &cur_time_tm);

    strftime(log_time_buf, internal::MAX_LOG_TIME_BUF_LEN, "%Y-%m-%d %H:%M:%S", &cur_time_tm);
    snprintf(log_time_buf + strlen(log_time_buf), 8, ":%ld", msec);

    snprintf(
        log_item_buf,
        internal::MAX_LOG_ITEM_LEN,
        "%s %s %s:%d %lu\t%s\n",
        internal::log_level_name[level],
        log_time_buf,
        __FILE__,
        __LINE__,
        pthread_self(),
        fmt);

    char file_path[internal::MAX_LOG_FILE_PATH_LEN];
    strftime(file_path, internal::MAX_LOG_FILE_PATH_LEN, log_file_path_.c_str(), &cur_time_tm);

    FILE* log_file = get_log_file(file_path);
    internal::get_locker().lock();
    vfprintf(log_file, log_item_buf, ap);
    fflush(log_file);
    internal::get_locker().unlock();
}

FILE* Log::get_log_file(const char* file_path) 
{
    if (log_file_path_.empty()) {
        return stderr;
    }

    if (log_file_ == NULL || last_log_file_path_ != file_path) {
        if (log_file_ != NULL) {
            fclose(log_file_);
            log_file_ = NULL;
        }
        log_file_ = fopen(file_path, "a");
        last_log_file_path_.assign(file_path);
    }
    return log_file_;
}

} // namespace logging
