# Audio Headers

Header files for RIFF, FMT headers

## AudioChunk

Variables | Type
--- | ---
chunkID | `char[4]`
chunkSize | `uint32_t`

## RIFFHeader

Variables | Type
--- | ---
format | `uint8_t[4]`

## FMTHeader

Variables | Type
--- | ---
audioFormat | `uint16_t`
numChannels | `uint16_t`
sampleRate | `uint32_t`
byteRate | `uint32_t`
blockAlign | `uint16_t`
bitsPerSample | `uint16_t`