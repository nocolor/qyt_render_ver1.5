//
//  QYTError.h
//  lightCoreRender
//
//  Created by nocolor on 14/11/5.
//  Copyright (c) 2014年 ___NOCOLOR___. All rights reserved.
//

#if defined(_MSC_VER)
#pragma once
#endif

#ifndef __lightCoreRender__QYTError__
#define __lightCoreRender__QYTError__

#ifdef __GNUG__
#define PRINTF_FUNC __attribute__ ((__format__ (__printf__, 1, 2)))
#else
#define PRINTF_FUNC
#endif
#if defined(QYTR_IS_WINDOWS)
#include <windows.h>
#else
#include <sys/ioctl.h>
#include <unistd.h>
#include <errno.h>
#endif
#include <stdarg.h>
#include <ctype.h>
#include <iostream>

namespace QYT
{
    void QYTInfo(const char *, ...) PRINTF_FUNC;
    void QYTWarning(const char *, ...) PRINTF_FUNC;
    void QYTError(const char *, ...) PRINTF_FUNC;
    void QYTSevere(const char *, ...) PRINTF_FUNC;
    
    inline int QYTTerminalWidth()
    {
#if defined(QYTR_IS_WINDOWS)
        HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
        if (h == INVALID_HANDLE_VALUE || h == NULL) {
            fprintf(stderr, "GetStdHandle() call failed");
            return 80;
        }
        CONSOLE_SCREEN_BUFFER_INFO bufferInfo = { 0 };
        GetConsoleScreenBufferInfo(h, &bufferInfo);
        return bufferInfo.dwSize.X;
#else
        struct winsize w;
        if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) < 0) {
            fprintf(stderr, "Error in ioctl() in TerminalWidth(): %d", errno);
            return 80;
        }
        return w.ws_col;
#endif
    }
}


#endif /* defined(__lightCoreRender__QYTError__) */
