#pragma once

#ifndef MAX_ONCE_CALLS
#define MAX_ONCE_CALLS 32
#endif

#ifndef MAX_MUTEXES
#define MAX_MUTEXES 5
#endif

#ifndef PREF_NS
#define PREF_NS "esptoolkit"
#endif

#ifndef MAX_HTTP_HEADERS
#define MAX_HTTP_HEADERS 10
#endif

#ifndef MAX_FACES
#define MAX_FACES 6
#endif

#ifdef __JPEGDEC__
#define CAN_DECODE_JPEG 1
#else
#define CAN_DECODE_JPEG 0
#endif