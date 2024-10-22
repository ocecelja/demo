/**
 * \ingroup litembim
 * 
 * \file MbimLogging.h
 */
#ifndef __MBIM_LOGGING_H__
#define __MBIM_LOGGING_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define DEFAULT_SYSLOG_LEVEL    7

typedef void (* litembim_logger)(uint8_t lvl, const char* buff, ...);

/**
 * \ingroup litembim
 * 
 * Set external logging function.  If external logging function is not
 * configured, logging goes to syslog.
 */
void set_litembim_logger_func(litembim_logger func);

/**
 * \ingroup litembim
 * 
 * Log lite-mbim messages
 * 
 * @param[in] lvl   Logging level
 * @param[in] fmt   Logging format    
*/
void litembim_log(
    uint8_t lvl,
    const char* fmt, ...
);

#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif // __MBIM_LOGGING_H__
