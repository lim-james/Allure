#include "BeatMap.h"

#include <Logger/Logger.h>
#include <fstream>

void BeatMap::Initialize() {
	et = bt = 0.f;
	index = 0;
}

void BeatMap::Update(float const & dt) {
	et += dt;
	bt += dt;
}

void BeatMap::Push() {
	beats.push_back(bt);
	bt = 0.f;
}

bool BeatMap::Check() {
	if (index + 1 >= beats.size()) return false;

	if (et + offset > t) {
		t += beats[++index];
		return true;
	}
	return false;
}

void BeatMap::Load(std::string const & filepath) {
	std::ifstream ifs(filepath);
	
	if (ifs.is_open()) {
		while (!ifs.eof()) {
			float temp;
			ifs >> temp;
			beats.push_back(temp);
		}
		ifs.close();
		t = beats[0];
	} else {
		Debug::Error << "Beat map \"" << filepath << "\" cannot be opened.";
	}
}

void BeatMap::Save(std::string const & filepath) {
	std::ofstream ofs(filepath);

	for (float const& b : beats)
		ofs << b << ' ';

	ofs.close();
}

float BeatMap::GetEt() const {
	return et;
}

void BeatMap::SetOffset(float const & value) {
	offset = value;
}
