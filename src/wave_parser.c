#include "wave_parser.h"

inline unsigned int parseRIFFChunk(struct WAVE *wave, char *buffer, unsigned int size) {
    // Verify fixed portion of chunk is within buffer.
    struct RIFFChunk *chunk = (struct RIFFChunk*)buffer;
    if (sizeof(struct RIFFChunk) > size ||
        chunk->header.id[0] != 'R' ||
        chunk->header.id[1] != 'I' ||
        chunk->header.id[2] != 'F' ||
        chunk->header.id[3] != 'F') {
        return 0; // Not a riff chunk.
    }
    // Verify the file size matches the buffer size.
    if(chunk->header.size + 8 != size) {
        return 0; // File is malformed.
    }
    // Verify this file uses WAVE format.
    if (chunk->format[0] != 'W' ||
        chunk->format[1] != 'A' ||
        chunk->format[2] != 'V' ||
        chunk->format[3] != 'E') {
        return 0;
    }
    wave->riffChunk = (struct RIFFChunk*)buffer;
    return sizeof(struct RIFFChunk);
}

inline unsigned int parseWAVEFormatChunk(struct WAVE *wave, char *buffer, unsigned int size) {
    // Verify fixed portion of chunk is within buffer.
    struct WAVEFormatChunk *chunk = (struct WAVEFormatChunk*)buffer;
    if (sizeof(struct WAVEFormatChunk) > size ||
        chunk->header.id[0] != 'f' ||
        chunk->header.id[1] != 'm' ||
        chunk->header.id[2] != 't' ||
        chunk->header.id[3] != ' ') {
        return 0; // Not a format chunk.
    }
    // Verify total size of chunk is within buffer.
    unsigned int waveFormatSize = chunk->header.size + 8;
    if (waveFormatSize > size) {
        return 0; // File is malformed.
    }
    wave->waveFormat = (struct WAVEFormatChunk*)buffer;
    return waveFormatSize;
}

inline unsigned int parseWAVEDataChunk(struct WAVE *wave, char *buffer, unsigned int size) {
    // Verify fixed portion of chunk is within buffer.
    struct WAVEDataChunk *chunk = (struct WAVEDataChunk*)buffer;
    if (sizeof(struct WAVEDataChunk) > size ||
        chunk->header.id[0] != 'd' ||
        chunk->header.id[1] != 'a' ||
        chunk->header.id[2] != 't' ||
        chunk->header.id[3] != 'a') {
        return 0; // Not a data chunk.
    }
    // Verify total size of chunk is within buffer.
    unsigned int waveDataSize = chunk->header.size + 8;
    if(waveDataSize > size) {
        return 0; // File is malformed.
    }
    // Add wave data.
    wave->waveData = (struct WAVEDataChunk*)buffer;
    wave->data = (unsigned char*)buffer + 8;
    return waveDataSize;
}

unsigned int parseWAVE(struct WAVE *wave, char *buffer, unsigned int size) {
    int offset = 0;
    if ((offset += parseRIFFChunk(wave, buffer, size)) &&
        (offset += parseWAVEFormatChunk(wave, buffer+offset, size-offset)) &&
        (offset += parseWAVEDataChunk(wave, buffer+offset, size-offset)));
    return offset;
}
