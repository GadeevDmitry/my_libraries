#ifndef ALGORITHM_SETTINGS_H
#define ALGORITHM_SETTINGS_H

//#define BUFFER_NVERIFY
//#define BUFFER_NDEBUG

//--------------------------------------------------------------------------------------------------------------------------------

#if defined(NVERIFY)
#define BUFFER_NVERIFY
#endif

#if defined(NDEBUG)
#define BUFFER_NDEBUG
#endif

#endif // ALGORITHM_SETTINGS_H