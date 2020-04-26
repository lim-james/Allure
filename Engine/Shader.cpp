#include "Shader.h"

#include <Logger/Logger.h>

#include <gl/glew.h>

#include <fstream>
#include <sstream>

Shader::Shader()
	: id(0)
	, locations({}) {}

Shader::Shader(std::string const& computePath) {
	const unsigned computeShader = CreateShader(computePath, GL_COMPUTE_SHADER);

	id = glCreateProgram();
	glAttachShader(id, computeShader);
	glLinkProgram(id);

	int success;
	glGetProgramiv(id, GL_LINK_STATUS, &success);

	if (!success) {
		char infoLog[512];
		glGetProgramInfoLog(id, 512, 0, infoLog);
		Debug::Error << "Shader program link failed.\n" << infoLog << '\n';
	}

	GetUniformNames();
}

Shader::Shader(std::string const& vertexPath, std::string const& fragmentPath) {
	const unsigned vertexShader = CreateShader(vertexPath, GL_VERTEX_SHADER);
	const unsigned fragmentShader = CreateShader(fragmentPath, GL_FRAGMENT_SHADER);

	id = glCreateProgram();
	glAttachShader(id, vertexShader);
	glAttachShader(id, fragmentShader);
	glLinkProgram(id);

	int success;
	glGetProgramiv(id, GL_LINK_STATUS, &success);

	if (!success) {
		char infoLog[512];
		glGetProgramInfoLog(id, 512, 0, infoLog);
		Debug::Error << "Shader program link failed. VS: \"" << vertexPath << "\", FS: \"" << fragmentPath << "\"\n" << infoLog << '\n';
	}

	GetUniformNames();
}

Shader::Shader(std::string const& vertexPath, std::string const& geometryPath, std::string const& fragmentPath) {
	const unsigned vertexShader = CreateShader(vertexPath, GL_VERTEX_SHADER);
	const unsigned geometryShader = CreateShader(geometryPath, GL_GEOMETRY_SHADER);
	const unsigned fragmentShader = CreateShader(fragmentPath, GL_FRAGMENT_SHADER);

	id = glCreateProgram();
	glAttachShader(id, vertexShader);
	glAttachShader(id, geometryShader);
	glAttachShader(id, fragmentShader);
	glLinkProgram(id);

	int success;
	glGetProgramiv(id, GL_LINK_STATUS, &success);

	if (!success) {
		char infoLog[512];
		glGetProgramInfoLog(id, 512, 0, infoLog);
		Debug::Error << "Shader program link failed.\n" << infoLog << '\n';
	}

	GetUniformNames();
}

Shader::~Shader() {
	glDeleteProgram(id);
	locations.clear();
}

unsigned const& Shader::GetID() const {
	return id;
}

void Shader::Use() const {
	glUseProgram(id);
}

void Shader::Delete() const {
	glDeleteProgram(id);
}

std::vector<std::string> const & Shader::GetUniforms() const {
	return uniforms;
}

void Shader::SetInt(std::string const& name, int const& value) {
	glUniform1i(GetUniformLocation(name), value);
}

void Shader::SetFloat(std::string const& name, float const& value) {
	glUniform1f(GetUniformLocation(name), value);
}

void Shader::SetVector2(std::string const& name, vec2f const& value) {
	glUniform2fv(GetUniformLocation(name), 1, &value.x);
}

void Shader::SetVector3(std::string const& name, vec3f const& value) {
	glUniform3fv(GetUniformLocation(name), 1, &value.x);
}

void Shader::SetVector4(std::string const& name, vec4f const& value) {
	glUniform4fv(GetUniformLocation(name), 1, &value.x);
}

void Shader::SetMatrix4(std::string const& name, mat4f const& value) {
	glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &value.a[0]);
}

unsigned Shader::CreateShader(std::string const& path, int const& type) {
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
			std::string typeString = "Vertex";
			if (type == GL_GEOMETRY_SHADER) {
				typeString = "Geometry";
			} else if (type == GL_FRAGMENT_SHADER) {
				typeString = "Fragment";
			}
			Debug::Error << typeString << " shader failed to compile\n" << infoLog << '\n';
		}
	} else {
		Debug::Error << "Failed to open " << path << '\n';
	}

	return shader;
}

void Shader::GetUniformNames() {
	int count;
	glGetProgramiv(id, GL_ACTIVE_UNIFORMS, &count);

	for (int i = 0; i < count; ++i) {
		GLsizei length;
		GLint size;
		GLenum type;
		GLchar* name = new GLchar[256];

		glGetActiveUniform(id, static_cast<GLuint>(i), 256, &length, &size, &type, name);

		uniforms.push_back(std::string(name));
		delete[] name;
	}
}

int Shader::GetUniformLocation(std::string const& name) {
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