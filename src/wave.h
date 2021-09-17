#ifndef __WAVE_H
#define __WAVE_H
#ifdef __cplusplus
extern "C"{
#endif

struct RIFFChunkHeader {
    char id[4];
    unsigned int size;
};

struct RIFFChunk {
    struct RIFFChunkHeader header;
    char format[4];
};

struct WAVEFormatChunk {
    struct RIFFChunkHeader header;
    unsigned short formatCode;
    unsigned short channelCount;
    unsigned int sampleRate;
    unsigned int byteRate;
    unsigned short blockAlign;
    unsigned short bitDepth;
};

struct WAVEDataChunk {
    struct RIFFChunkHeader header;
};

struct WAVE {
    struct RIFFChunk* riffChunk;
    struct WAVEFormatChunk* waveFormat;
    struct WAVEDataChunk* waveData;
    unsigned char *data;
};

#ifdef __cplusplus
}
#endif
#endif
