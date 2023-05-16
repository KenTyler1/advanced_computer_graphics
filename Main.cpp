//------- Bỏ qua điều này ----------
#include<filesystem>
namespace fs = std::filesystem;
//------------------------------

#include"Model.h"


const unsigned int width = 800;
const unsigned int height = 800;



float skyboxVertices[] =
{
	//   Coordinates
	-1.0f, -1.0f,  1.0f,//        7--------6
	 1.0f, -1.0f,  1.0f,//       /|       /|
	 1.0f, -1.0f, -1.0f,//      4--------5 |
	-1.0f, -1.0f, -1.0f,//      | |      | |
	-1.0f,  1.0f,  1.0f,//      | 3------|-2
	 1.0f,  1.0f,  1.0f,//      |/       |/
	 1.0f,  1.0f, -1.0f,//      0--------1
	-1.0f,  1.0f, -1.0f
};

unsigned int skyboxIndices[] =
{
	// Right
	1, 2, 6,
	6, 5, 1,
	// Left
	0, 4, 7,
	7, 3, 0,
	// Top
	4, 5, 6,
	6, 7, 4,
	// Bottom
	0, 3, 2,
	2, 1, 0,
	// Back
	0, 1, 5,
	5, 4, 0,
	// Front
	3, 7, 6,
	6, 2, 3
};



int main()
{
	// Khởi tạo GLFW
	glfwInit();

	// Cho GLFW biết phiên bản OpenGL nào ta đang sử dụng
	// Trong trường hợp này, ta đang sử dụng OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Nói với GLFW rằng ta đang sử dụng CORE profile
	// Vì vậy, điều đó có nghĩa là ta chỉ có modern functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Tạo đối tượng GLFWwindow có kích thước 800 x 800 pixel, đặt tên là "MyProject"
	GLFWwindow* window = glfwCreateWindow(width, height, "MyProject", NULL, NULL);
	// Kiểm tra lỗi nếu cửa sổ không tạo được
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// Đưa cửa sổ vào bối cảnh hiện tại
	glfwMakeContextCurrent(window);

	//Tải GLAD để cấu hình OpenGL
	gladLoadGL();
	// Chỉ định chế độ xem của OpenGL in the Window
	// Trong trường hợp này, khung nhìn đi từ x = 0, y = 0, đến x = 800, y = 800
	glViewport(0, 0, width, height);





	// Tạo các đối tượng Shader
	Shader shaderProgram("default.vert", "default.frag");
	Shader skyboxShader("skybox.vert", "skybox.frag");

	// Take care tất cả các thứ liên quan đến ánh sáng (the light)
	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);
	
	shaderProgram.Activate();
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

	skyboxShader.Activate();
	glUniform1i(glGetUniformLocation(skyboxShader.ID, "skybox"), 0);


	// Kích hoạt Depth Buffer
	glEnable(GL_DEPTH_TEST);

	// Kích hoạt Cull Facing
	glEnable(GL_CULL_FACE);
	// Giữ lại front faces
	glCullFace(GL_FRONT);
	// Uses counter clock-wise standard (Sử dụng tiêu chuẩn ngược chiều kim đồng hồ)
	glFrontFace(GL_CCW);

	// Tạo camera object
	Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));


	/*
	* Ta đang thực hiện điều này theo đường dẫn tương đối để tập trung tất cả các tài nguyên vào một thư mục chứ không phải
	* sao chép chúng giữa các thư mục hướng dẫn. Bạn chỉ có thể sao chép, dán tài nguyên từ 'Tài nguyên'
	* và sau đó cung cấp một đường dẫn tương đối từ thư mục này đến bất kỳ tài nguyên nào bạn muốn truy cập.
	* Cũng lưu ý rằng điều này yêu cầu C++ 17, vì vậy hãy chuyển đến Thuộc tính dự án, C/C++, Ngôn ngữ và chọn C++ 17
	*/
	std::string parentDir = (fs::current_path().fs::path::parent_path()).string();
	std::string modelPath = "/Resources/images/models/airplane/scene.gltf";
	
	// Tải models
	Model modelObject((parentDir + modelPath).c_str());


	// Các biến để tạo sự kiện định kỳ cho hiển thị FPS
	double prevTime = 0.0;
	double crntTime = 0.0;
	double timeDiff;
	// Theo dõi số lượng frames trong timeDiff
	unsigned int counter = 0;

	// Use this to disable VSync (not advized)
	//glfwSwapInterval(0);


	// Tạo VAO, VBO, và EBO cho the skybox
	unsigned int skyboxVAO, skyboxVBO, skyboxEBO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glGenBuffers(1, &skyboxEBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skyboxEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(skyboxIndices), &skyboxIndices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


	// Tất cả các mặt của cubemap (đảm bảo rằng chúng theo thứ tự chính xác này)
	std::string facesCubemap[6] =
	{
		parentDir + "/Resources/images/skybox/right.jpg",
		parentDir + "/Resources/images/skybox/left.jpg",
		parentDir + "/Resources/images/skybox/top.jpg",
		parentDir + "/Resources/images/skybox/bottom.jpg",
		parentDir + "/Resources/images/skybox/front.jpg",
		parentDir + "/Resources/images/skybox/back.jpg"
	};

	// Tạo ra cubemap texture object
	unsigned int cubemapTexture;
	glGenTextures(1, &cubemapTexture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	// Đây là những điều rất quan trọng để ngăn chặn các đường nối (seams)
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	// This might help with seams on some systems
	// glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);


	// Quay vòng qua tất cả các textures và gắn chúng vào cubemap object
	for (unsigned int i = 0; i < 6; i++)
	{
		int width, height, nrChannels;
		unsigned char* data = stbi_load(facesCubemap[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			stbi_set_flip_vertically_on_load(false);
			glTexImage2D
			(
				GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0,
				GL_RGB,
				width,
				height,
				0,
				GL_RGB,
				GL_UNSIGNED_BYTE,
				data
			);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Failed to load texture: " << facesCubemap[i] << std::endl;
			stbi_image_free(data);
		}
	}
	
	
	// Điểm bắt đầu ban đầu của đối tượng
	// float translateX = 0.0f;

	// Main while loop
	while (!glfwWindowShouldClose(window))
	{

		// Updates counter and times
		crntTime = glfwGetTime();
		timeDiff = crntTime - prevTime;
		counter++;

		if (timeDiff >= 1.0 / 30.0)
		{
			// Creates new title
			std::string FPS = std::to_string((1.0 / timeDiff) * counter);
			std::string ms = std::to_string((timeDiff / counter) * 1000);
			std::string newTitle = "MyProjectOpenGL - " + FPS + "FPS / " + ms + "ms";
			glfwSetWindowTitle(window, newTitle.c_str());

			// Resets times and counter
			prevTime = crntTime;
			counter = 0;

			// Use this if you have disabled VSync
			//camera.Inputs(window);
		}

		// Chỉ định màu nền
		//glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		// Clean the back buffer and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);;

		// Xử lý đầu vào máy ảnh (xóa phần này nếu bạn đã tắt VSync)
		camera.Inputs(window);
		// Updates và exports ma trận camera sang Vertex Shader
		camera.updateMatrix(45.0f, 0.1f, 100.0f);

		// Draw the model
		modelObject.Draw(shaderProgram, camera, window);

		// Update translateX for the next frame
		//translateX += 0.01f;
		//if (translateX > 30.0f)
		//	translateX = -1.0f;

		// Vì cubemap sẽ luôn có độ sâu bằng 1.0, nên ta cần equal sign đó để nó không bị loại bỏ
		glDepthFunc(GL_LEQUAL); 

		//shaderProgram.Activate();
		skyboxShader.Activate();

		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);
		//glm::mat4 model = glm::mat4(1.0f);

		// Biến mat4 thành mat3 rồi lại thành mat4 lần nữa loại bỏ hàng và cột cuối cùng
		// Hàng và cột cuối cùng ảnh hưởng đến translation của skybox(mà ta không muốn ảnh hưởng)
		
		//model = glm::translate(model, glm::vec3(translateX, 0.0f, 0.0f));
		view = glm::mat4(glm::mat3(glm::lookAt(camera.Position, camera.Position + camera.Orientation, camera.Up)));
		projection = glm::perspective(glm::radians(45.0f), (float)width / height, 0.1f, 100.0f);

		glUniformMatrix4fv(glGetUniformLocation(skyboxShader.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(skyboxShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));


		// Draws cubemap làm đối tượng cuối cùng để chúng tôi có thể tiết kiệm một chút của performance bằng cách loại bỏ tất cả fragments
		// nơi có object là đại diện cho (một depth của 1.0f sẽ luôn thất bại so với object's depth value)
		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		// Chuyển về normal depth function
		glDepthFunc(GL_LESS);

		// Hoán đổi back buffer với front buffer
		glfwSwapBuffers(window);
		// Take care tất cả GLFW events
		glfwPollEvents();
	}



	// Xóa tất cả objects mà ta đã tạo ra
	shaderProgram.Delete();
	skyboxShader.Delete();
	// Xóa cửa sổ trước khi kết thúc chương trình
	glfwDestroyWindow(window);
	// Terminate GLFW trước khi kết thúc chương trình
	glfwTerminate();
	return 0;
}