#ifndef LOG_H
#define LOG_H

// 로깅 레벨 정의
typedef enum {
    LOG_DEBUG,
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR
} LogLevel;

// 로깅 함수 선언
void log_event(LogLevel level, const char* action, int fd, int count, const char* details);

#endif // LOG_H


