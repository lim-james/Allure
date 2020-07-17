# Audio File

`AudioFile.h` `AudioFile.inl`

Parses WAV files for sampling of waveform, energy and frequency spectrums

## Template parameters

**T** - Type of elements of the sample.

## Members

Variable | Type | Description
--- | --- | ---
riff | [`RIFFHeader`](AudioHeaders.md) | Store the riff header of the audio file
fmt | [`FMTHeader`](AudioHeaders.md) | Store the fmt header of the audio file
file | `FILE*` | Pointer to the opened audio file
dataOffset | `size_t` | Header size of the audio file
bytesPerSample | `size_t` | The size of each audio sample
numSample | `size_t` | Number of samples in file
timeUnit | `float` | 
duration | `float` | Duration of audio clip

Function | Description
--- | ---
[```bool Open(std::string const& filepath)```](AudioFile/Open.md) | Open audio file at relative path
[```void Close()```](AudioFile/Close.md) | Closes opened file.
[```float const& GetDuration() const```](AudioFile/GetDuration.md) | Returns the duration of the audio clip.
[```size_t BPM(float const& t, size_t const& numBlocks)```](AudioFile/BPM.md) | 
[```size_t GetTime(float const& t) const```](AudioFile/GetTime.md) |
[```std::vector<T> SampleDuration(float const& t, float const& duration)```](AudioFile/SampleDuration.md) | 
[```std::vector<T> SampleCount(float const& t, size_t const& count)```](AudioFile/SampleCount.md) | 
[```T SamplePoint(float const& t)```](AudioFile/SamplePoint.md) | 
[```T BlockEnergy(float const& t, size_t const& blockSize)```](AudioFile/BlockEnergy.md) | Returns in 2D representation of frequncy sample.
[```T AverageEnergy(float const& t, size_t const& numBlocks, size_t const& blockSize)```](AudioFile/AverageEnergy.md) | 
[```T Varience(float const& t, size_t const& numBlocks)```](AudioFile/Varience.md) | Returns varience of audio amplitude between different blocks
[```T C(float const& t, size_t const& numBlocks)```](AudioFile/C.md) | Returns the C constant for determining BPM.
[```std::vector<vec2d> Spectrum(float const& t, unsigned const& startF, unsigned const& endF)```](AudioFile/Spectrum.md) | Returns in 2D representation of frequncy sample.
[```void fft(std::vector<std::complex<double>>& samples, unsigned const& offset, unsigned const& df)```](AudioFile/fft.md) | Recursively performs fast fourier transform on time scale and evaluates the frequncy spectrum.