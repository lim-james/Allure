#include "AudioFile.h"

#include "../Logger/Logger.h"

template<typename T>
AudioFile<T>::AudioFile()
	: file(nullptr)
	, dataOffset(0)
	, bytesPerSample(0)
	, numSample(0) {}

template<typename T>
bool AudioFile<T>::Open(std::string const & filepath) {
	fopen_s(&file, filepath.c_str(), "rb");

	if (!file) {
		Debug::Error << "Unable to open \"" << filepath << "\"\n";
		return false;
	}

	AudioChunk chunk;
	size_t bytesRead = 0;

	while ((bytesRead = fread(&chunk, 1, sizeof(chunk), file)) > 0) {
		dataOffset += bytesRead;
		const std::string str = std::string(chunk.chunkID).substr(0, 4);
		if (str == "RIFF") {
			bytesRead = fread(&riff, 1, sizeof(riff), file);
		} else if (str == "fmt ") {
			bytesRead = fread(&fmt, 1, sizeof(fmt), file);
			timeUnit = static_cast<float>(fmt.byteRate);
		} else if (str == "data") {
			bytesPerSample = fmt.bitsPerSample / 8;
			numSample = (chunk.chunkSize / fmt.numChannels) / bytesPerSample;
			duration = static_cast<float>(chunk.chunkSize / fmt.byteRate);
			break;
		} else {
			int8_t* buffer = new int8_t[chunk.chunkSize];
			const size_t elementSize = sizeof(buffer[0]);
			const size_t elementCount = chunk.chunkSize / elementSize;
			bytesRead = fread_s(buffer, chunk.chunkSize, elementSize, elementCount, file);
		}
		dataOffset += bytesRead;
	}

	return true;
}

template<typename T>
void AudioFile<T>::Close() {
	if (file) fclose(file);
}

template<typename T>
float const & AudioFile<T>::GetDuration() const {
	return duration;
}

template<typename T>
size_t AudioFile<T>::BPM(float const & t, size_t const & numBlocks) {
	size_t result = 0;

	const T c = C(t, numBlocks);
	const T avg = AverageEnergy(t, numBlocks);
	const T threshold = c * avg;

	const size_t blockSize = fmt.sampleRate / numBlocks;
	const float blocks = static_cast<float>(numBlocks);
	const float invT = 1.f / blocks;

	for (float i = 0; i < blocks; ++i)
		if (BlockEnergy(i * invT + t, blockSize) > threshold)
			++result;

	return result;
}

template<typename T>
std::vector<T> AudioFile<T>::SampleDuration(float const & t, float const & duration) {
	const size_t resultOffset = static_cast<size_t>(t * timeUnit) + dataOffset;
	const size_t resultSize = static_cast<size_t>(duration * timeUnit / bytesPerSample);

	std::vector<T> data(resultSize);

	const size_t BUFFER_SIZE = min(4096, resultSize);
	T* buffer = new T[BUFFER_SIZE];
	memset(buffer, 0, BUFFER_SIZE);

	const size_t elementSize = sizeof(T);
	const size_t elementCount = BUFFER_SIZE / elementSize;

	size_t offset = 0;
	size_t bytesRead = 0;

	// offset cursor
	fseek(file, resultOffset, SEEK_SET);

	while ((bytesRead = fread_s(buffer, BUFFER_SIZE, elementSize, elementCount, file)) > 0) {
		size_t i = 0;
		for (; i < bytesRead; ++i) {
			data[offset++] = buffer[i];
			if (offset >= resultSize) break;
		}

		if (i != bytesRead) break;
	}

	delete[] buffer;

	return data;
}

template<typename T>
std::vector<T> AudioFile<T>::SampleCount(float const & t, size_t const & count) {
	const size_t resultOffset = static_cast<size_t>(t * timeUnit) + dataOffset;

	std::vector<T> data(count);

	const size_t BUFFER_SIZE = min(4096, count);
	T* buffer = new T[BUFFER_SIZE];
	memset(buffer, 0, BUFFER_SIZE);

	const size_t elementSize = sizeof(T);
	const size_t elementCount = BUFFER_SIZE / elementSize;

	size_t offset = 0;
	size_t bytesRead = 0;

	// offset cursor
	fseek(file, resultOffset, SEEK_SET);

	while ((bytesRead = fread_s(buffer, BUFFER_SIZE, elementSize, elementCount, file)) > 0) {
		size_t i = 0;
		for (; i < bytesRead; ++i) {
			data[offset++] = buffer[i];
			if (offset >= count) break;
		}

		if (i != bytesRead) break;
	}

	delete[] buffer;

	return data;
}

template<typename T>
T AudioFile<T>::SamplePoint(float const & t) {
	const size_t resultOffset = static_cast<size_t>(t * timeUnit) + dataOffset;
	T result;

	// offset cursor
	fseek(file, resultOffset, SEEK_SET);
	fread_s(&result, 1, sizeof(T), 1, file);

	return result;
}

template<typename T>
T AudioFile<T>::BlockEnergy(float const& t, size_t const& blockSize) {
	const auto data = SampleCount(t, blockSize);

	T sum = static_cast<T>(0);
	for (T const& i : data)
		sum += i * i;

	return sum;
}

template<typename T>
T AudioFile<T>::AverageEnergy(float const& t, size_t const & numBlocks, size_t const& blockSize) {
	T sum = static_cast<T>(0);

	const float blocks = static_cast<float>(numBlocks);
	const float invT = 1.f / blocks;

	for (float i = 0; i < blocks; ++i)
		sum += BlockEnergy(i * invT + t, blockSize);

	return sum / static_cast<T>(numBlocks);
}

template<typename T>
T AudioFile<T>::Varience(float const& t, size_t const & numBlocks) {
	const T avg = AverageEnergy(t, numBlocks);
	T sum = static_cast<T>(0);

	const size_t blockSize = fmt.sampleRate / numBlocks;
	const float blocks = static_cast<float>(numBlocks);
	const float invT = 1.f / blocks;

	for (float i = 0; i < blocks; ++i) {
		const T a = avg - BlockEnergy(i * invT + t, blockSize);
		sum += a * a;
	}

	return sum / static_cast<T>(numBlocks);
}

template<typename T>
T AudioFile<T>::C(float const& t, size_t const & numBlocks) {
	return static_cast<T>(-0.0000015 * Varience(t, numBlocks) + 1.5142857);
}


