//
//  apu_logger.hpp
//  libaputils
//
//  Created by Luke on 5/6/17.
//  Copyright © 2017 Luke Habermehl. All rights reserved.
//

#ifndef apu_logger_h
#define apu_logger_h

#define LOG_LEVEL_URGENT 0
#define LOG_LEVEL_ERROR 1
#define LOG_LEVEL_WARNING 2
#define LOG_LEVEL_DEBUG 3

#include "apu_obj.hpp"

/// Interface for logger objects
/// Implementations must also conform to APUObjectInterface
class APULoggerInterface
: public APUObjectInterface
{
public:
    /** Log a message.
      * @param tag prefix for the message
      * @param logLevel the urgency of the message
      * @param fmt a format string followed by any format arguments
      */
    virtual void log(const char *tag, int logLevel, const char *fmt, ...) = 0;
    /** Set an optional file to which to write the logs */
    virtual void setOutputFilepath(const char *filepath) = 0;
    /** Set the urgency level to filter to */
    virtual void setLogLevel(int level) = 0;

    /** Terminate the logger */
    virtual void shutdown() = 0;

    virtual ~APULoggerInterface() {}
};

/** Built-in thread-safe logger. This is the default logger if you do not call APUSetLogger() */
class APUDefaultLogger
: public APULoggerInterface
, private APUObject
{
    typedef APUObject BASE_CLASS;
public:
    APUOBJ_FWDDECL

    APUDefaultLogger();
    virtual ~APUDefaultLogger();

    virtual void log(const char *tag, int logLevel, const char *fmt, ...);
    virtual void setOutputFilepath(const char *filepath);
    virtual void setLogLevel(int level);
    virtual void shutdown();

    class Pimpl;

private:
    Pimpl *m_pimpl;
};

/** Set the singleton logger. If no logger is set, the built-in logger is used */
void APUSetLogger(APULoggerInterface *logger);
/** Get the singleton logger */
APULoggerInterface * APUGetLogger();

#endif /* apu_logger_h */
