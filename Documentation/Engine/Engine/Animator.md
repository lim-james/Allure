# Animator

`Animator.h` `Animator.cpp`

Parses WAV files for sampling of waveform, energy and frequency spectrums

## Members

Variable | Type | Description
--- | --- | ---
animations | [`std::map<int, std::vector<AnimationBase*>>`](AnimationBase.md) | Stores animation queues based on the address of target.

Function | Accessor | Description
--- | --- | ---
[```~Animator() override```](Animator/Descrutor.md) | `public` |
[```void Initialize() override```](Animator/) | `public` | 
[```Component* Clone() const override```](Animator/) | `public` | 
[```void SetActive(bool const& state) override```](Animator/) | `public` |
[```void Queue(AnimationBase const& base, T* target, T const& outcome)```](Animator/) | `public` |
[```unsigned Count(T* target) const```](Animator/) | `public` |
[```void Pop(T* target)```](Animator/) | `public` |
[```void Clear(T* target)```](Animator/) | `public` |
[```void Clear()```](Animator/) | `public` |