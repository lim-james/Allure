# AudioFile::Spectrum

> private member function
```c++
std::vector<vec2d> Spectrum(float const& t, unsigned const& startF, unsigned const& endF);
```
Returns in 2D representation of frequncy sample.

### Return value
`vec2d` Returns the 2D representation of frequncy sample. X axis representing the frequncy amplitude, Y axis representing the freuqncy bands.

### Parameters
Name | Type | Description
--- | --- | ---
t | `float const&` | Stores the result of frequency spectrum.
startF | `unsigned const&` | Start frequncy of result
endF | `unsigned const&` | Start end of result
