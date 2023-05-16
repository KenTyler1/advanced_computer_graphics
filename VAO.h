#ifndef VAO_CLASS_H
#define VAO_CLASS_H

#include<glad/glad.h>
#include"VBO.h"

class VAO
{
public:
	// ID tham chiếu Vertex Array Object
	GLuint ID;
	// Constructor tạo ra một VAO ID
	VAO();

	// Liên kết Thuộc tính VBO chẳng hạn như vị trí hoặc màu sắc vào VAO
	void LinkAttrib(VBO& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset);
	// Binds the VAO
	void Bind();
	// Unbinds the VAO
	void Unbind();
	// Deletes the VAO
	void Delete();
};

#endif