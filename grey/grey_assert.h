#ifndef GREY_ASSERT_H
#define GREY_ASSERT_H

#ifdef GREY_NO_ASSERT
#define GREY_ASSERT(cond, message, ...)
#else

#ifdef GREY_TARGETS_ANDROID
#ifndef __ANDROID__
#error "Trying to use Android log in non-Android environment"
#endif

#include <android/log.h>
#define TERMLOG(...) __android_log_print(ANDROID_LOG_ERROR, "grey", __VA_ARGS__)
#else
#include <stdio.h>
#define TERMLOG(...) printf(__VA_ARGS__)
#endif

#if defined(__clang__) || defined(__GNUC__)
#define TRAP_PROGRAM() __builtin_trap()
#else
#include <stdlib.h>
#define TRAP_PROGRAM() abort()
#endif

#define GREY_ASSERT(cond, message, ...)                                        \
  do {                                                                         \
    if (!(cond)) {                                                             \
      TERMLOG("%s:%d " message "\n", __FILE__, __LINE__, ##__VA_ARGS__);       \
      TRAP_PROGRAM();                                                          \
    }                                                                          \
  } while (0)
#endif

#endif