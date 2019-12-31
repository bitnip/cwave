#ifndef __WAVE_PARSER_H
#define __WAVE_PARSER_H
#ifdef __cplusplus
extern "C"{
#endif

#include "wave.h"

unsigned int parseWAVE(struct WAVE* wave, char* buffer, unsigned int size);

#ifdef __cplusplus
}
#endif
#endif