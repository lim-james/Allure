#include "AudioFile.h"

#include "../Logger/Logger.h"
#include "../Math/Math.hpp"

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

template<typename T>
std::vector<float> AudioFile<T>::Spectrum(float const& t, float const td, unsigned const& bands, unsigned const& startF, unsigned const& endF) {
	std::vector<T> samples = SampleDuration(t, td);

	const unsigned N = samples.size();
	const unsigned dF = (endF - startF) / N;
	
	std::vector<std::complex<double>> spectrum(N, 0.0);

	for (unsigned i = 0; i < N; ++i) {
		spectrum[i] = static_cast<double>(samples[i]) / 32768.0;
	}

	fft(spectrum, startF, dF);
	
	const unsigned bR = N / bands / 2;

	std::vector<float> result(bands, 0.f);

	const double unit = 2.0 / static_cast<double>(N);
	double m = 0.0;
	
	for (unsigned i = 0; i < bands; ++i) {
		const double a = spectrum[i].real();
		const double b = spectrum[i].imag();
		result[i] = sqrt(a * a + b * b) * unit; 
	}

	return result;
}

template<typename T>
void AudioFile<T>::fft(std::vector<std::complex<double>>& samples, unsigned const& offset, unsigned const& df) {
	const unsigned N = samples.size();

	if (N <= 1) return;

	// split even and odd

	const unsigned M = N / 2;
	std::vector<std::complex<double>> odd(M, 0);
	std::vector<std::complex<double>> even(M, 0);

	for (unsigned i = 0; i < M; ++i) {
		even[i] = samples[i * 2];
		odd[i] = samples[i * 2 + 1];
	}

	// Split on tasks
	fft(even, offset, df);
	fft(odd, offset, df);

	// Calculate DFT
	for (unsigned i = 0; i < M; ++i) {
		const unsigned k = i * df + offset;
		std::complex<double> t = exp(std::complex<double>(0, -2 * Math::PI * i / N)) * odd[i];
		samples[i] = even[i] + t;
		samples[N / 2 + i] = even[i] - t;
	}
}

