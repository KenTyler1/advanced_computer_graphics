#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>

#include"shaderClass.h"

class Camera
{
public:
	// Lưu trữ các vectơ chính của máy ảnh
	glm::vec3 Position;
	glm::vec3 Orientation = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::mat4 cameraMatrix = glm::mat4(1.0f);

	// Ngăn camera nhảy xung quanh khi nhấp chuột trái lần đầu tiên
	bool firstClick = true;

	// Lưu trữ chiều rộng và chiều cao của cửa sổ
	int width;
	int height;

	// Điều chỉnh tốc độ của camera và độ nhạy của nó khi quan sát xung quanh
	float speed = 0.01f;
	float sensitivity = 50.0f;

	// Camera constructor để thiết lập các giá trị ban đầu
	Camera(int width, int height, glm::vec3 position);

	// Cập nhật ma trận camera cho Vertex Shader
	void updateMatrix(float FOVdeg, float nearPlane, float farPlane);
	// Xuất ma trận camera sang shader
	void Matrix(Shader& shader, const char* uniform);
	// Xử lý đầu vào camera
	void Inputs(GLFWwindow* window);
};
#endif