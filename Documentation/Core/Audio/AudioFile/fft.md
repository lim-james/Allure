# AudioFile::fft

> private member function
```c++
void fft(std::vector<std::complex<double>>& samples, unsigned const& offset, unsigned const& df);
```
Recursively performs fast fourier transform on time scale and evaluates the frequncy spectrum.

### Return value
None

### Parameters
Name | Type | Description
--- | --- | ---
samples | `std::vector<std::complex<double>>&` | Stores the result of frequency spectrum.
offset | `unsigned const&` | Offset of audio file based on number of samples.
df | `unsigned const&` | The intervals between frequencies.
