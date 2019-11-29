#include "head.h"

/*TODO -- refresh shader every sec*/

class Shader {

public:

    Shader(std::string const& nameFragment, std::string const& nameVertex)
        : nameFragment_(nameFragment), nameVertex_(nameVertex) {

		update();
    }

    void update(){

		if (nameFragment_.empty() || nameVertex_.empty() || (nameFragment_.empty() && nameVertex_.empty())) {
			return;
		}
			

			bool recreate = false;
			std::ifstream stream(nameFragment_);
			std::string tmp = std::string(std::istreambuf_iterator<char>(stream), std::istreambuf_iterator<char>());
			stream.close();
			if (tmp != fragment_) {
				setFragment(tmp);
				recreate = true;
			}

			stream.open(nameVertex_);
			tmp = std::string(std::istreambuf_iterator<char>(stream), std::istreambuf_iterator<char>());
			stream.close();
			if (tmp != vertex_) {
				setVertex(tmp);
				recreate = true;
			}

			if (recreate) {
				createFragmentObject();
				createVertexObject();
				createProgramObject();
				deleteUselessShaders();
				std::cout << nameFragment_ << " & " << nameVertex_ << " Shaders loaded.\n";
			}

			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}

	void use() {
		glUseProgram(programObject_);
	}

    void setFragmentName(std::string const& name) {
        nameFragment_ = name;
    }

	void setVertexName(std::string const& name) {
		nameVertex_ = name;
	}

    std::string getFragmentName() {
        return nameFragment_;
    }

	std::string getVertexName() {
		return nameVertex_;
	}

    void setFragment(std::string const& fragment) {
        fragment_ = fragment;
        fs_ptr_ = fragment_.c_str();
    }

    const GLchar*& getFragment() {
        return fs_ptr_;
    }

    void setVertex(std::string const& vertex) {
        vertex_ = vertex;
        vs_ptr_ = vertex_.c_str();
    }

    const GLchar*& getVertex() {
        return vs_ptr_;
    }

	void createVertexObject() {
		/*Creating a VS object*/
		vertexObject_ = glCreateShader(GL_VERTEX_SHADER);

		/*Link the VS code to the VS object*/
		glShaderSource(vertexObject_, 1, &vs_ptr_, NULL);
		glCompileShader(vertexObject_);

		/*Testing the VS compilation*/
		int  success;
		char infoLog[512];
		glGetShaderiv(vertexObject_, GL_COMPILE_STATUS, &success);

		if (!success) {
			glGetShaderInfoLog(vertexObject_, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		}
	}

	const unsigned int & getVertexObject() {
		return vertexObject_;
	}

	void createFragmentObject() {
		/*As the VS, same for FS*/
		fragmentObject_ = glCreateShader(GL_FRAGMENT_SHADER);

		glShaderSource(fragmentObject_, 1, &fs_ptr_, NULL);
		glCompileShader(fragmentObject_);

		int  success;
		char infoLog[512];

		glGetShaderiv(fragmentObject_, GL_COMPILE_STATUS, &success);

		if (!success) {
			glGetShaderInfoLog(fragmentObject_, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		}
	}

	const unsigned int& getFragmentObject() {
		return fragmentObject_;
	}

	void createProgramObject() {
		/*Creating the program Shader*/
		programObject_ = glCreateProgram();

		glAttachShader(programObject_, vertexObject_);
		glAttachShader(programObject_, fragmentObject_);
		glLinkProgram(programObject_);

		int  success;
		char infoLog[512];

		/*Testing PS compilation*/
		glGetProgramiv(programObject_, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(programObject_, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;
		}
	}

	const unsigned int& getProgramObject() {
		return programObject_;
	}

	void deleteUselessShaders() {
		/*Deleting shaders already used*/
		glDeleteShader(vertexObject_);
		glDeleteShader(fragmentObject_);
	}

	void setBool(const std::string& name, bool value) const
	{
		glUniform1i(glGetUniformLocation(programObject_, name.c_str()), (int)value);
	}
	// ------------------------------------------------------------------------
	void setInt(const std::string& name, int value) const
	{
		glUniform1i(glGetUniformLocation(programObject_, name.c_str()), value);
	}
	// ------------------------------------------------------------------------
	void setFloat(const std::string& name, float value) const
	{
		glUniform1f(glGetUniformLocation(programObject_, name.c_str()), value);
	}
	// ------------------------------------------------------------------------
	void setVec2(const std::string& name, const glm::vec2& value) const
	{
		glUniform2fv(glGetUniformLocation(programObject_, name.c_str()), 1, &value[0]);
	}
	void setVec2(const std::string& name, float x, float y) const
	{
		glUniform2f(glGetUniformLocation(programObject_, name.c_str()), x, y);
	}
	// ------------------------------------------------------------------------
	void setVec3(const std::string& name, const glm::vec3& value) const
	{
		glUniform3fv(glGetUniformLocation(programObject_, name.c_str()), 1, &value[0]);
	}
	void setVec3(const std::string& name, float x, float y, float z) const
	{
		glUniform3f(glGetUniformLocation(programObject_, name.c_str()), x, y, z);
	}
	// ------------------------------------------------------------------------
	void setVec4(const std::string& name, const glm::vec4& value) const
	{
		glUniform4fv(glGetUniformLocation(programObject_, name.c_str()), 1, &value[0]);
	}
	void setVec4(const std::string& name, float x, float y, float z, float w) const
	{
		glUniform4f(glGetUniformLocation(programObject_, name.c_str()), x, y, z, w);
	}
	// ------------------------------------------------------------------------
	void setMat2(const std::string& name, const glm::mat2& mat) const
	{
		glUniformMatrix2fv(glGetUniformLocation(programObject_, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}
	// ------------------------------------------------------------------------
	void setMat3(const std::string& name, const glm::mat3& mat) const
	{
		glUniformMatrix3fv(glGetUniformLocation(programObject_, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}
	// ------------------------------------------------------------------------
	void setMat4(const std::string& name, const glm::mat4& mat) const
	{
		glUniformMatrix4fv(glGetUniformLocation(programObject_, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}


private:
	std::string nameFragment_;
	std::string nameVertex_;
    std::string vertex_;
    std::string fragment_;
    const GLchar *vs_ptr_;
    const GLchar *fs_ptr_;
	unsigned int vertexObject_;
	unsigned int fragmentObject_;
	unsigned int programObject_;
};