#ifndef LOAD_WAV_H
#define LOAD_WAV_H

#include "AudioFile.h"

#include "../Logger/Logger.h"

#include <fstream>
#include <iostream>
#include <map>
#include <string>

namespace Load {
	template<typename T>
	size_t WAV(std::string const& filepath, T** data, float const& t, float const& sample);
}

template<typename T>
size_t Load::WAV(std::string const & filepath, T** data, float const& t, float const& sample) {
	FILE* file = nullptr;
	fopen_s(&file, filepath.c_str(), "rb");

	if (!file) {
		Debug::Error << "Unable to open \"" << filepath << "\"\n";
		return 0;
	}

	AudioChunk chunk;
	RIFFHeader riff;
	FMTHeader fmt;

	size_t resultOffset = 0;
	size_t resultSize = 0;

	size_t bytesRead = 0;
	
	while ((bytesRead = fread(&chunk, 1, sizeof(chunk), file)) > 0) {
		const std::string str = std::string(chunk.chunkID).substr(0, 4);
		if (str == "RIFF") {
			bytesRead = fread(&riff, 1, sizeof(riff), file);
		} else if (str == "fmt ") {
			bytesRead = fread(&fmt, 1, sizeof(fmt), file);
		} else if (str == "data") {
			const size_t bytesPerSample = fmt.bitsPerSample / 8;
			const size_t numSample = (chunk.chunkSize / fmt.numChannels) / bytesPerSample;

			const float timeUnit = static_cast<float>(fmt.byteRate);
			resultOffset = static_cast<size_t>(t * timeUnit);
			resultSize = static_cast<size_t>(sample * timeUnit / bytesPerSample);

			if (*data != nullptr) delete *data;

			//*data = new int8_t[sampleSize];
			*data = new T[resultSize];

			const size_t BUFFER_SIZE = min(4096, resultSize);
			T* buffer = new T[BUFFER_SIZE];
			memset(buffer, 0, BUFFER_SIZE);

			const size_t elementSize = sizeof(T);
			const size_t elementCount = BUFFER_SIZE / elementSize;

			size_t offset = 0;

			// offset cursor
			fseek(file, resultOffset, SEEK_CUR);

			while ((bytesRead = fread_s(buffer, BUFFER_SIZE, elementSize, elementCount, file)) > 0) {
				size_t i = 0;
				for (; i < bytesRead; ++i) {
					(*data)[offset++] = buffer[i];
					if (offset >= resultSize) break;
				}

				if (i != bytesRead) break;
			}

			delete[] buffer;

			break;
		} else {
			const size_t BUFFER_SIZE = chunk.chunkSize;
			int8_t* buffer = new int8_t[chunk.chunkSize];
			const size_t elementSize = sizeof(buffer[0]);
			const size_t elementCount = BUFFER_SIZE / elementSize;
			bytesRead = fread_s(buffer, BUFFER_SIZE, elementSize, elementCount, file);
		}
	}

	fclose(file);

	return resultSize;
}

#endif
