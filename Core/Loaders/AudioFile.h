#ifndef AUDIO_FILE_H
#define AUDIO_FILE_H

#include <cstdint>

struct AudioChunk {
	char chunkID[4];		// 4 bytes
	uint32_t chunkSize;		// 4 bytes
};

// RIFF header
struct RIFFHeader {
	uint8_t format[4];		// 4 bytes
};

// fmt
struct FMTHeader {
	uint16_t audioFormat;	// 2 bytes
	uint16_t numChannels;	// 2 bytes
	uint32_t sampleRate;	// 4 bytes
	uint32_t byteRate;		// 4 bytes
	uint16_t blockAlign;	// 2 bytes
	uint16_t bitsPerSample;	// 2 bytes
};

#endif
