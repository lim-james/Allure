#include "Shader.h"

#include <Logger/Logger.h>

#include <gl/glew.h>

#include <fstream>
#include <sstream>

Shader::Shader()
	: id(0)
	, locations({}) {}

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath) {
	const unsigned vertexShader = CreateSahder(vertexPath, GL_VERTEX_SHADER);
	const unsigned fragmentShader = CreateSahder(fragmentPath, GL_FRAGMENT_SHADER);

	id = glCreateProgram();
	glAttachShader(id, vertexShader);
	glAttachShader(id, fragmentShader);
	glLinkProgram(id);

	int success;
	glGetProgramiv(id, GL_LINK_STATUS, &success);

	if (!success) {
		char infoLog[512];
		glGetProgramInfoLog(id, 512, 0, infoLog);
		Console::Error << "Shader program link failed.\n" << infoLog << '\n';
	}
}

Shader::~Shader() {
	glDeleteProgram(id);
	locations.clear();
}

const unsigned& Shader::GetID() const {
	return id;
}

void Shader::Use() const {
	glUseProgram(id);
}

void Shader::Delete() const {
	glDeleteProgram(id);
}

void Shader::SetInt(const std::string& name, const int& value) {
	glUniform1i(GetUniformLocation(name), value);
}

void Shader::SetFloat(const std::string& name, const float& value) {
	glUniform1f(GetUniformLocation(name), value);
}

void Shader::SetVector2(const std::string& name, const vec2f& value) {
	glUniform2fv(GetUniformLocation(name), 1, &value.x);
}

void Shader::SetVector3(const std::string& name, const vec3f& value) {
	glUniform3fv(GetUniformLocation(name), 1, &value.x);
}

void Shader::SetVector4(const std::string& name, const vec4f& value) {
	glUniform4fv(GetUniformLocation(name), 1, &value.x);
}

void Shader::SetMatrix4(const std::string& name, const mat4f& value) {
	glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &value.a[0]);
}

unsigned Shader::CreateSahder(const std::string& path, const int& type) {
	const unsigned shader = glCreateShader(type);

	std::ifstream ifs(path);
	std::stringstream ss;

	if (ifs.is_open()) {
		ss << ifs.rdbuf();
		const std::string str = ss.str();
		const char* source = str.c_str();

		glShaderSource(shader, 1, &source, 0);
		glCompileShader(shader);

		int success;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

		if (!success) {
			char infoLog[512];
			glGetShaderInfoLog(shader, 512, 0, infoLog);
			Console::Error << (type == GL_VERTEX_SHADER ? "Vertex" : "Fragment") << " shader failed to compile\n" << infoLog << '\n';
		}
	} else {
		Console::Error << "Failed to open " << path << '\n';
	}

	return shader;
}

int Shader::GetUniformLocation(const std::string& name) {
	if (locations.find(name) == locations.end()) {
		const int result = glGetUniformLocation(id, name.c_str());

		if (result != -1) {
			locations[name] = result;
			return locations[name];
		} else {
			return -1;
		}
	}

	return locations[name];
}
