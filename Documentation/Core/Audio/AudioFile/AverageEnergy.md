# AudioFile::AverageEnergy

> public member function
```c++
T AverageEnergy(float const& t, size_t const& numBlocks, size_t const& blockSize);
```
Returns the average energy of a block in the audio

### Return value
`T` Returns if energy sampled

### Parameters
Name | Type | Description
--- | --- | ---
t | `float const&` | Time offset of sample.
numBlocks | `float const&` | Number of blocks to be sampled for energy.
blockSize | `size_t const&` | Number of samples per block to be sampled.
