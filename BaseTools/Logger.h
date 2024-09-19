#ifndef _LOGGER_H_
#define _LOGGER_H_

#define LogDebug(log, ...) printf(log"\n", ##__VA_ARGS__)
#define LogInfo(log, ...) printf(log"\n", ##__VA_ARGS__)
#define LogWarn(log, ...) printf(log"\n", ##__VA_ARGS__)
#define LogError(log, ...) printf(log"\n", ##__VA_ARGS__)
#define LogFatal(log, ...) printf(log"\n", ##__VA_ARGS__)

#endif
