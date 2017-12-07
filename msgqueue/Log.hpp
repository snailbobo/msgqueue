#ifndef _LOG_HPP_
#define _LOG_HPP_

#include <stdio.h>
#include <iostream>
#include <string.h>


#define DLT_LOG_VERBOSE 0
#define DLT_LOG_DEBUG 1
#define DLT_LOG_INFO 2
#define DLT_LOG_WARN 3
#define DLT_LOG_ERROR 4

//#define FILENAME(x) (strrchr(x,'/')?strrchr(x,'/')+1:x)
#define FILENAME(x) (strrchr(x,'\\')?strrchr(x,'\\')+1:x)

#define LOGP(format, args...) \
   do {        \
      printMessages(DLT_LOG_INFO, __FILE__, __LINE__, "", format, ##args);    \
   }while(0)

#define LOGA(format, args...)   \
    do {        \
        printMessages(DLT_LOG_VERBOSE,  __FILE__, __LINE__, "Dbg1", format, ##args);    \
    }while(0)

#define LOGI(format, args...)   \
    do {    \
      printMessages(DLT_LOG_INFO, __FILE__, __LINE__, "Info", format, ##args);    \
    }while(0)

#define LOGD(format, args...)   \
    do {        \
      printMessages(DLT_LOG_DEBUG , __FILE__, __LINE__, "Dbg", format, ##args);    \
    }while(0)

#define LOGW(format, args...)   \
    do {        \
      printMessages(DLT_LOG_WARN, __FILE__, __LINE__, "Warning", format, ##args);    \
    }while(0)

#define LOGE(format, args...)   \
    do {        \
      printMessages(DLT_LOG_ERROR,  __FILE__, __LINE__, "Error", format, ##args);    \
    }while(0)

void initDltViewer(void);

void printMessages(int type, const char *file, int line, const char *msgType, const char *format, ... );

#endif // _LOG_HPP_
