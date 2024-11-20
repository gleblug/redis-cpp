#pragma once

#include <stdio.h>
#include <time.h>

static void printCurrentTime() {
    time_t timer;
    char buffer[26];
    struct tm *tm_info;

    timer = time(NULL);
    tm_info = localtime(&timer);

    strftime(buffer, 26, "%Y-%m-%d %H:%M:%S", tm_info);
    printf("%s", buffer);
}

template <typename... Ts>
static void logInfo(const char *fmt, Ts... ts) {
    printf("\033[34m");
    printCurrentTime();
    printf(" [INFO]: \033[3m");
    printf(fmt, ts...);
    printf("\033[0m\n");
}

template <typename... Ts>
static void logErr(const char *fmt, Ts... ts) {
    printf("\033[31m");
    printCurrentTime();
    printf(" [ERROR]: \033[3m");
    printf(fmt, ts...);
    printf("\033[0m\n");
}