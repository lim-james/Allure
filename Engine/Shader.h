#ifndef SHADER_H
#define SHADER_H

#include <Math/Vectors.h>
#include <Math/Mat4.hpp>

#include <map>
#include <vector>
#include <string>

class Shader {

	unsigned id;

	std::vector<std::string> uniforms;
	std::map<std::string, int> locations;

public:

	Shader();
	Shader(std::string const& computePath);
	Shader(std::string const& vertexPath, std::string const& fragmentPath);
	Shader(std::string const& vertexPath, std::string const& geometryPath, std::string const& fragmentPath);
	~Shader();

	unsigned const& GetID() const;

	void Use() const;
	void Delete() const;

	std::vector<std::string> const& GetUniforms() const;

	void SetInt(std::string const& name, int const& value);
	void SetFloat(std::string const& name, float const& value);
	void SetVector2(std::string const& name, vec2f const& value);
	void SetVector3(std::string const& name, vec3f const& value);
	void SetVector4(std::string const& name, vec4f const& value);
	void SetMatrix4(std::string const& name, mat4f const& value);

private:

	unsigned CreateShader(std::string const& path, int const& type);
	
	void GetUniformNames();
	int GetUniformLocation(std::string const& name);

};

#endif