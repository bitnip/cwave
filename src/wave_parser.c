#include "wave_parser.h"

const char RIFF_CHUNK_ID[] = "RIFF";
const char WAVE_FORMAT_ID[] = "WAVE";
const char FMT_CHUNK_ID[] = "fmt ";
const char DATA_CHUNK_ID[] = "data";

unsigned int parseRIFFChunk(struct WAVE* wave, char** buffer, unsigned int *size) {
    // Verify fixed portion of chunk is within buffer.
    if (sizeof(struct RIFFChunk) > *size) {
        return 0;
    }
    wave->riffChunk = (struct RIFFChunk*) *buffer;
    // Verify file starts with `RIFF`.
    for (int i = 0; i < 4; i++) {
        if (wave->riffChunk->header.id[i] != RIFF_CHUNK_ID[i]) {
            return 0;
        }
    }
    // Verify the file size matches the buffer size.
    if (wave->riffChunk->header.size + 8 != *size) {
        // File is malformed.
        return 0;
    }
    // Verify this file uses WAVE format.
    for (int i = 0; i < 4; i++) {
        if (wave->riffChunk->format[i] != WAVE_FORMAT_ID[i]) {
            // Unexpected format.
            return 0;
        }
    }
    *buffer += sizeof(struct RIFFChunk);
    *size -= sizeof(struct RIFFChunk);
    return sizeof(struct RIFFChunk);
}

unsigned int parseWAVEFormatChunk(struct WAVE* wave, char **buffer, unsigned int *size) {
    // Verify fixed portion of chunk is within buffer.
    if (sizeof(struct WAVEFormatChunk) > *size) {
        return 0;
    }
    wave->waveFormat = (struct WAVEFormatChunk*) *buffer;
    // Verify chunk starts with `fmt `.
    for (int i = 0; i < 4; i++) {
        if (wave->waveFormat->header.id[i] != FMT_CHUNK_ID[i]) {
            return 0;
        }
    }
    // Verify total size of chunk is within buffer.
    if (wave->waveFormat->header.size + 8 > *size) {
        // File is malformed.
        return 0;
    }
    *buffer += wave->waveFormat->header.size + 8;
    *size -= wave->waveFormat->header.size + 8;
    return wave->waveFormat->header.size + 8;
}

unsigned int parseWAVEDataChunk(struct WAVE* wave, char **buffer, unsigned int *size) {
    // Verify fixed portion of chunk is within buffer.
    if (sizeof(struct WAVEDataChunk) > *size) {
        return 0;
    }
    wave->waveData = (struct WAVEDataChunk*) *buffer;
    // Verify chunk starts with `data`.
    for (int i = 0; i < 4; i++) {
        if (wave->waveData->header.id[i] != DATA_CHUNK_ID[i]) {
            return 0;
        }
    }
    // Verify total size of chunk is within buffer.
    if (wave->waveData->header.size + 8 > *size) {
        // File is malformed.
        return 0;
    }

    wave->data = (unsigned char*) *buffer + 8;

    *buffer += wave->waveData->header.size + 8;
    *size -= wave->waveData->header.size + 8;
    return wave->waveData->header.size + 8;
}

unsigned int parseWAVE(struct WAVE* wave, char* buffer, unsigned int size) {
    return parseRIFFChunk(wave, &buffer, &size) &&
           parseWAVEFormatChunk(wave, &buffer, &size) &&
           parseWAVEDataChunk(wave, &buffer, &size);
}
