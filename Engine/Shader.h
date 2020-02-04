#ifndef SHADER_H
#define SHADER_H

#include <Math/Vectors.hpp>
#include <Math/Matrix.hpp>

#include <map>
#include <string>

class Shader {

	unsigned id;

	std::map<std::string, int> locations;

public:

	Shader();
	Shader(const std::string& computePath);
	Shader(const std::string& vertexPath, const std::string& fragmentPath);
	Shader(const std::string& vertexPath, const std::string& geometryPath, const std::string& fragmentPath);
	~Shader();

	const unsigned& GetID() const;

	void Use() const;
	void Delete() const;

	void SetInt(const std::string& name, const int& value);
	void SetFloat(const std::string& name, const float& value);
	void SetVector2(const std::string& name, const vec2f& value);
	void SetVector3(const std::string& name, const vec3f& value);
	void SetVector4(const std::string& name, const vec4f& value);
	void SetMatrix4(const std::string& name, const mat4f& value);

private:

	unsigned CreateShader(const std::string& path, const int& type);

	int GetUniformLocation(const std::string& name);

};

#endif