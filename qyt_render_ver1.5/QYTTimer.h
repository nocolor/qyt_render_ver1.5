//
//  QYTTimer.h
//  LIGHT_CORE_RENDER
//
//  Created by nocolor on 14/12/2.
//  Copyright (c) 2014年 ___JinJiangTiaoZhanBei___. All rights reserved.
//

#ifndef __LIGHT_CORE_RENDER__QYTTimer__
#define __LIGHT_CORE_RENDER__QYTTimer__

#include <iostream>
#include "qytr.h"
#if defined (QYTR_IS_WINDOWS)
#include <windows.h>
#if (_MSC_VER >= 1400)
#include <stdio.h>
#define snprintf _snprintf
#endif
#else
#include <sys/time.h>
#endif

namespace QYT
{
    class QYTTimer
    {
    public:
        QYTTimer()
        {
            time0 = elapsed = 0;
            running = 0;
        }
        
        void start()
        {
            Assert( !running );
            running = 1;
            time0 = getTime();
        }

        void stop()
        {
            Assert( running );
            running = 0;
            
            elapsed += getTime() - time0;
        }
        
        void reset()
        {
            running = 0;
            elapsed = 0;
        }
        
        double time()
        {
            if (running) {
                stop();
                start();
            }
            return elapsed;
        }
        
    private:
        double time0, elapsed;
        bool running;
        double getTime()
        {
            gettimeofday( &timeofday, NULL );
            return timeofday.tv_sec + timeofday.tv_usec / 1000000.0;
        }

        struct timeval timeofday;
    };
}

#endif /* defined(__LIGHT_CORE_RENDER__QYTTimer__) */
