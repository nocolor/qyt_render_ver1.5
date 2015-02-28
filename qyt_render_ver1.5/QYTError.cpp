//
//  QYTError.cpp
//  lightCoreRender
//
//  Created by nocolor on 14/11/5.
//  Copyright (c) 2014年 ___NOCOLOR___. All rights reserved.
//

#include "QYTError.h"

#include "qytr.h"

#define QYTR_ERROR_IGNORE   0
#define QYTR_ERROR_CONTINUE 1
#define QYTR_ERROR_ABORT    2


inline const char* QYTFindWordEnd(const char* buf)
{
    while (*buf != '\0' && !isspace(*buf))
        ++buf;
    return buf;
}

static void QYTProcessError(const char* format,
                            va_list args,
                            const char* errorType,
                            int disposition)
{
    if(disposition == QYTR_ERROR_IGNORE)
        return;
    
    std::string errorString;

#ifdef QYTR_IS_WINDOWS
    int width = max(20, QYTTerminalWidth() - 2);
#else
	int width = std::max(20, QYT::QYTTerminalWidth() - 2);
#endif

    errorString += errorType;
    errorString += ": ";
    size_t column = errorString.size();
    
#if !defined(QYTR_IS_WINDOWS)
    char *errorBuf;
    if (vasprintf(&errorBuf, format, args) == -1) {
        fprintf(stderr, "vasprintf() unable to allocate memory!\n");
        abort();
    }
#else
    char errorBuf[2048];
    vsnprintf_s(errorBuf, sizeof(errorBuf), _TRUNCATE, format, args);
#endif
    
    const char *msgPos = errorBuf;
    while (true) {
        while (*msgPos != '\0' && isspace(*msgPos))
            ++msgPos;
        if (*msgPos == '\0')
            break;
        
        const char *wordEnd = QYTFindWordEnd(msgPos);
        if (column + wordEnd - msgPos > width) {
            errorString += "\n    ";
            column = 4;
        }
        while (msgPos != wordEnd) {
            errorString += *msgPos++;
            ++column;
        }
        errorString += ' ';
        ++column;
    }
    
    fprintf(stderr, "%s\n", errorString.c_str());
    
    if(disposition == QYTR_ERROR_ABORT)
    {
#if defined(QYTR_IS_WINDOWS)
        __debugbreak();
#else
        abort();
#endif
    }
#if !defined(QYTR_IS_WINDOWS)
    free(errorBuf);
#endif
}

void QYT::QYTInfo(const char* format, ...)
{
    va_list args;
    va_start(args, format);
    QYTProcessError(format, args, "Notice", QYTR_ERROR_CONTINUE);
    va_end(args);
}

void QYT::QYTWarning(const char* format, ...)
{
    va_list args;
    va_start(args, format);
    QYTProcessError(format, args, "Warning", QYTR_ERROR_CONTINUE);
    va_end(args);
}

void QYT::QYTError(const char* format, ...)
{
    va_list args;
    va_start(args, format);
    QYTProcessError(format, args, "Error", QYTR_ERROR_CONTINUE);
    va_end(args);
}

void QYT::QYTSevere(const char* format, ...)
{
    va_list args;
    va_start(args, format);
    QYTProcessError(format, args, "Fatal Error", QYTR_ERROR_ABORT);
    va_end(args);
}