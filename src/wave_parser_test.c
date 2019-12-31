#include "wave.h"
#include "cutil/src/assertion.h"

unsigned int parseRIFFChunk(struct WAVE* wave, char** buffer, unsigned int *size);
unsigned int parseWAVEFormatChunk(struct WAVE* wave, char **buffer, unsigned int *size);
unsigned int parseWAVEDataChunk(struct WAVE* wave, char **buffer, unsigned int *size);

void testRIFFChunk() {
    unsigned char input[16] = {
        0x52, 0x49, 0x46, 0x46, // `RIFF`
        0x04, 0x00, 0x00, 0x00, // Size of File - 8
        0x57, 0x41, 0x56, 0x45  // `WAVE`
    };
    char* buffer = (char*)input;
    struct WAVE wave;
    unsigned int size = 12;
    unsigned int offset = parseRIFFChunk(&wave, &buffer, &size);
    assertIntegersEqual(offset, 12);
    assertIntegersEqual(size, 0);
    assertIntegersEqual(wave.riffChunk->header.id[0], 'R');
    assertIntegersEqual(wave.riffChunk->header.id[1], 'I');
    assertIntegersEqual(wave.riffChunk->header.id[2], 'F');
    assertIntegersEqual(wave.riffChunk->header.id[3], 'F');
    assertIntegersEqual(wave.riffChunk->header.size, 4);
    assertIntegersEqual(wave.riffChunk->format[0], 'W');
    assertIntegersEqual(wave.riffChunk->format[1], 'A');
    assertIntegersEqual(wave.riffChunk->format[2], 'V');
    assertIntegersEqual(wave.riffChunk->format[3], 'E');
}

void testWAVEFormatChunk() {
    unsigned char input[24] = {
        0x66, 0x6d, 0x74, 0x20, // `fmt `
        0x10, 0x00, 0x00, 0x00, // Chunk size - 8
        0x01, 0x00, // Format: 1
        0x01, 0x00, // Channel count: 1
        0x80, 0xbb, 0x00, 0x00, // Sample rate
        0x80, 0xbb, 0x00, 0x00, // Byte rate
        0x01, 0x00, // Block align
        0x08, 0x00  // Bit depth
    };
    char* buffer = (char*)input;
    struct WAVE wave;
    unsigned int size = 24;
    unsigned int offset = parseWAVEFormatChunk(&wave, &buffer, &size);
    assertIntegersEqual(offset, 24);
    assertIntegersEqual(size, 0);
    assertIntegersEqual(wave.waveFormat->header.id[0], 'f');
    assertIntegersEqual(wave.waveFormat->header.id[1], 'm');
    assertIntegersEqual(wave.waveFormat->header.id[2], 't');
    assertIntegersEqual(wave.waveFormat->header.id[3], ' ');
    assertIntegersEqual(wave.waveFormat->header.size, 16);
    assertIntegersEqual(wave.waveFormat->formatCode, 1);
    assertIntegersEqual(wave.waveFormat->channelCount, 1);
    assertIntegersEqual(wave.waveFormat->sampleRate, 48000);
    assertIntegersEqual(wave.waveFormat->byteRate, 48000);
    assertIntegersEqual(wave.waveFormat->blockAlign, 1);
    assertIntegersEqual(wave.waveFormat->bitDepth, 8);
}

void testWAVEDataChunk() {
    unsigned char input[12] = {
        0x64, 0x61, 0x74, 0x61, // `data`
        0x04, 0x00, 0x00, 0x00, // Chunk size - 8
        0x57, 0x41, 0x56, 0x45  // Audo data
    };
    char* buffer = (char*)input;
    struct WAVE wave;
    unsigned int size = 12;
    unsigned int offset = parseWAVEDataChunk(&wave, &buffer, &size);
    assertIntegersEqual(offset, 12);
    assertIntegersEqual(size, 0);
    assertIntegersEqual(wave.waveData->header.id[0], 'd');
    assertIntegersEqual(wave.waveData->header.id[1], 'a');
    assertIntegersEqual(wave.waveData->header.id[2], 't');
    assertIntegersEqual(wave.waveData->header.id[3], 'a');
    assertIntegersEqual(wave.waveData->header.size, 4);
    assertIntegersEqual(wave.data[0], 0x57);
    assertIntegersEqual(wave.data[1], 0x41);
    assertIntegersEqual(wave.data[2], 0x56);
    assertIntegersEqual(wave.data[3], 0x45);
}

void test() {
    testRIFFChunk();
    testWAVEFormatChunk();
    testWAVEDataChunk();
}
