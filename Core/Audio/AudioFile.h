#ifndef AUDIO_FILE_H
#define AUDIO_FILE_H

#include "AudioHeaders.h"

#include <string>
#include <vector>
#include <complex>

template<typename T>
class AudioFile {

	RIFFHeader riff;
	FMTHeader fmt;

	FILE* file;
	size_t dataOffset;
	size_t bytesPerSample;
	size_t numSample;

	float timeUnit;
	float duration;

public:

	AudioFile();

	bool Open(std::string const& filepath);
	void Close();

	float const& GetDuration() const;

	size_t BPM(float const& t, size_t const& numBlocks);

	std::vector<T> SampleDuration(float const& t, float const& duration);
	std::vector<T> SampleCount(float const& t, size_t const& count);
	T SamplePoint(float const& t);

	T BlockEnergy(float const& t, size_t const& blockSize);
	T AverageEnergy(float const& t, size_t const& numBlocks, size_t const& blockSize);
	T Varience(float const& t, size_t const& numBlocks);
	T C(float const& t, size_t const& numBlocks);

	std::vector<float> Spectrum(float const& t, float const td, unsigned const& bands, unsigned const& startF, unsigned const& endF);
	
private:

	void fft(std::vector<std::complex<double>>& samples, unsigned const& offset, unsigned const& df);

};

#include "AudioFile.inl"

#endif
