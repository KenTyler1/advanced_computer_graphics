#include"shaderClass.h"

// Đọc text file và xuất ra một chuỗi chứa mọi thứ trong text file
std::string get_file_contents(const char* filename)
{
	std::ifstream in(filename, std::ios::binary);
	if (in)
	{
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return(contents);
	}
	throw(errno);
}

// Xây dựng Shader Program từ 2 shader khác nhau
Shader::Shader(const char* vertexFile, const char* fragmentFile)
{
	// Đọc vertexFile và fragmentFile và lưu trữ các chuỗi
	std::string vertexCode = get_file_contents(vertexFile);
	std::string fragmentCode = get_file_contents(fragmentFile);

	// Chuyển đổi shader source strings thành mảng ký tự
	const char* vertexSource = vertexCode.c_str();
	const char* fragmentSource = fragmentCode.c_str();

	// Tạo Vertex Shader Object và lấy tham chiếu của nó
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	// Gắn Vertex Shader source vào Vertex Shader Object
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	// Biên dịch Vertex Shader thành machine code
	glCompileShader(vertexShader);
	// Kiểm tra xem Shader đã biên dịch thành công chưa
	compileErrors(vertexShader, "VERTEX");

	// Tạo Fragment Shader Object  và lấy tham chiếu của nó
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	// Đính kèm Fragment Shader source vào Fragment Shader Object
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	// Biên dịch Vertex Shader thành machine code
	glCompileShader(fragmentShader);
	// Kiểm tra xem Shader đã biên dịch thành công chưa
	compileErrors(fragmentShader, "FRAGMENT");

	// Tạo Shader Program Object và lấy tham chiếu của nó
	ID = glCreateProgram();
	// Đính kèm Vertex và Fragment Shader vào Shader Program
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	// Wrap-up/Link tất cả các shader lại với nhau thành Shader Program
	glLinkProgram(ID);
	// Kiểm tra nếu Shader được liên kết thành công
	compileErrors(ID, "PROGRAM");

	// Xóa các Vertex và Fragment Shader objects hiện vô dụng
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

}

// Activates the Shader Program
void Shader::Activate()
{
	glUseProgram(ID);
}

// Deletes the Shader Program
void Shader::Delete()
{
	glDeleteProgram(ID);
}


// Kiểm tra xem các Shader khác nhau đã được biên dịch đúng chưa
void Shader::compileErrors(unsigned int shader, const char* type)
{
	// Lưu trữ trạng thái biên dịch
	GLint hasCompiled;
	// Mảng ký tự để lưu thông báo lỗi trong
	char infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &hasCompiled);
		if (hasCompiled == GL_FALSE)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "SHADER_COMPILATION_ERROR for:" << type << "\n" << infoLog << std::endl;
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &hasCompiled);
		if (hasCompiled == GL_FALSE)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "SHADER_LINKING_ERROR for:" << type << "\n" << infoLog << std::endl;
		}
	}
}