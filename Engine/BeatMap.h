#ifndef BEAT_MAP_H
#define BEAT_MAP_H

#include <vector>

class BeatMap {

	float et, bt, offset;
	float t;
	unsigned index;
	
	std::vector<float> beats;

public:
	
	void Initialize();
	void Update(float const& dt);

	void Push();
	bool Check();

	void Load(std::string const& filepath);
	void Save(std::string const& filepath);

	float GetEt() const;
	void SetOffset(float const& value);

};

#endif
