#ifndef LOG_SETTINGS_H
#define LOG_SETTINGS_H

//#define NLOG          ///< отключить логи везде
//#define LOG_NLEAK     ///< отключить макросы для отслеживания утечек памяти

//#define LOG_NVERIFY   ///< отключить логирование во время верификации
//#define LOG_NDEBUG    ///< отключить логирование во время assert-ов
//#define LOG_NTRACE    ///< отключить трассировку стека вызовов

//#define NDEBUG        ///< отключить assert-ы
//#define VVERIFY       ///< отключить верификаторы

//--------------------------------------------------------------------------------------------------------------------------------

#if defined(NVERIFY) || defined(NLOG)
#define LOG_NVERIFY
#endif

#if defined(NDEBUG) || defined(NLOG)
#define LOG_NDEBUG
#define LOG_NLEAK
#define LOG_NTRACE
#endif

#endif // LOG_SETTINGS_H
