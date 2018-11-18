#ifndef INCLUDED_AZOTE_AZOTE_H
#define INCLUDED_AZOTE_AZOTE_H

/* Common macros */
#if defined(__cplusplus)
# define AZOTE_API_EXPORT   extern "C"
#else
# define AZOTE_API_EXPORT
#endif

#define AZOTE_API AZOTE_API_EXPORT

/* Structures */
typedef struct AzState_ AzState;

/* cpu.c */
AZOTE_API void          azRun(AzState* state);

/* init.c */
AZOTE_API AzState*      azInit();
AZOTE_API void          azExit(AzState* state);

/* load.c */
AZOTE_API void          azLoadFile(AzState* state, const char* path);

/* frame.c */
AZOTE_API void          azNextFrame(AzState* state);

#endif
