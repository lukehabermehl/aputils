//
//  autil_logger.hpp
//  libaputils
//
//  Created by Luke on 5/6/17.
//  Copyright © 2017 Luke Habermehl. All rights reserved.
//

#ifndef autil_logger_h
#define autil_logger_h

#define LOG_LEVEL_URGENT 0
#define LOG_LEVEL_ERROR 1
#define LOG_LEVEL_WARNING 2
#define LOG_LEVEL_DEBUG 3

#include <memory>
#include "autil_obj.hpp"

/// Interface for logger objects
/// Implementations must also conform to APUObjectInterface
class APULoggerInterface
: public APUObjectInterface
{
public:
    virtual void log(const char *tag, int logLevel, const char *fmt, ...) = 0;
    virtual void setOutputFilepath(const char *filepath) = 0;
    virtual void setLogLevel(int level) = 0;

    virtual ~APULoggerInterface() {}
};

/// Built-in thread-safe logger
class APUDefaultLogger
: public APULoggerInterface
, public APUObject
{
    typedef APUObject BASE_CLASS;
public:
    APUOBJ_FWDDECL

    APUDefaultLogger();
    virtual ~APUDefaultLogger();

    virtual void log(const char *tag, int logLevel, const char *fmt, ...);
    virtual void setOutputFilepath(const char *filepath);
    virtual void setLogLevel(int level);

    class Pimpl;

private:
    Pimpl *pimpl_;
};

void APUSetLogger(APULoggerInterface *logger);
APULoggerInterface * APUGetLogger();

#endif /* autil_logger_h */
