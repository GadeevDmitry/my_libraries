#ifndef STACK_SETTINGS_H
#define STACK_SETTINGS_H

//#define STACK_NVERIFY
//#define STACK_NDEBUG

//--------------------------------------------------------------------------------------------------------------------------------

#if defined(NVERIFY)
#define STACK_NVERIFY
#endif

#if defined(NDEBUG)
#define STACK_NDEBUG
#endif

#endif // STACK_SETTINGS_H
