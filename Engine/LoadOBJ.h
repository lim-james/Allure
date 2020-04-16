#ifndef LOAD_OBJ_H
#define LOAD_OBJ_H

#include "Model.h"

#include <map>
#include <string>

namespace Load {

	static const std::vector<int> triangleIndices = { 1, 2, 3 };
	static const std::vector<int> quadIndices = { 1, 2, 3, 3, 4, 1 };

	static std::map<std::string, Model*> modelCache;

	Model* OBJ(std::string const& filepath);

	void ClearModelCache();

}

#endif