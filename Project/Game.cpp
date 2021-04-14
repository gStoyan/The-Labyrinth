#include "Game.h"
#include <stdio.h>
#include <tuple>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <ctime>
//Private functions
void Game::initGLFW()
{
	//INIT GLFW
	if (glfwInit() == GLFW_FALSE)
	{
		std::cout << "ERROR::GLFW_INIT_FAILED" << "\n";
		glfwTerminate();
	}
}

void Game::initWindow(
	const char* title,
	bool resizable
)
{
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, this->GL_VERSION_MAJOR);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, this->GL_VERSION_MINOR);
	glfwWindowHint(GLFW_RESIZABLE, resizable);

	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); MAC OS

	this->window = glfwCreateWindow(this->WINDOW_WIDTH, this->WINDOW_HEIGHT, title, NULL, NULL);

	if (this->window == nullptr)
	{
		std::cout << "ERROR::GLFW_WINDOW_INIT_FAILED" << "\n";
		glfwTerminate();
	}

	glfwGetFramebufferSize(this->window, &this->framebufferWidth, &this->framebufferHeight);
	glfwSetFramebufferSizeCallback(window, Game::framebuffer_resize_callback);
	//IMPORTANT WHITH PERSPECTIVE MATRIX!!!

	//glViewport(0, 0, framebufferWidth, framebufferHeight);

	glfwMakeContextCurrent(this->window); //IMPORTANT!!
}

void Game::initGLEW()
{
	//INIT GLEW (NEEDS WINDOW AND OPENGL CONTEXT)
	glewExperimental = GL_TRUE;

	//Error
	if (glewInit() != GLEW_OK)
	{
		std::cout << "ERROR::MAIN.CPP::GLEW_INIT_FAILED" << "\n";
		glfwTerminate();
	}
}

void Game::initOpenGLOptions()
{
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	//Input
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Game::initMatrices()
{
	this->ViewMatrix = glm::mat4(1.f);
	this->ViewMatrix = glm::lookAt(this->camPosition, this->camPosition + this->camFront, this->worldUp);

	this->ProjectionMatrix = glm::mat4(1.f);
	this->ProjectionMatrix = glm::perspective(
		glm::radians(this->fov),
		static_cast<float>(this->framebufferWidth) / this->framebufferHeight,
		this->nearPlane,
		this->farPlane
	);
}

void Game::initShaders()
{
	this->shaders.push_back(new Shader(this->GL_VERSION_MAJOR, this->GL_VERSION_MINOR,
		"vertex_core.glsl", "fragment_core.glsl"));
}

void Game::initTextures()
{
	//TEXTURE 0
	this->textures.push_back(new Texture("Images/wall.png", GL_TEXTURE_2D));
	this->textures.push_back(new Texture("Images/wall.png", GL_TEXTURE_2D));

	//TEXTURE 1
	this->textures.push_back(new Texture("Images/wall.png", GL_TEXTURE_2D));
	this->textures.push_back(new Texture("Images/wall.png", GL_TEXTURE_2D));
}

void Game::initMaterials()
{
	this->materials.push_back(new Material(glm::vec3(0.1f), glm::vec3(1.f), glm::vec3(1.f),
		0, 1));
}

void Game::initOBJModels()
{

}

std::vector<float> Game::drawCorridorForward(std::vector<Mesh*>meshes, float x, float y, float z) {
	
	float leftX = x - 1.8; // the X coord of the left wall
	float length = z - 3.f;	// the length of the corridor

	for (z; z > length; z -= 0.5)
	{
		this->models.push_back(new Model(
			glm::vec3(x, y, z),
			this->materials[0],
			this->textures[TEX_CONTAINER],
			this->textures[TEX_CONTAINER_SPECULAR],
			meshes
		)
		);
		
		this->models.push_back(new Model(
			glm::vec3(leftX, y, z),
			this->materials[0],
			this->textures[TEX_CONTAINER],
			this->textures[TEX_CONTAINER_SPECULAR],
			meshes
		)
		);
	}
	std::vector<float> vector;
	vector.push_back(x);
	vector.push_back(y);
	vector.push_back(z);

	return vector;

}
std::vector<float> Game::drawTurnRight(std::vector<Mesh*>meshes, float x, float y, float z) {
	float turnWall = z - 2;
	float length = x + 4;
	for (z; z > turnWall; z -= 0.5) {
		this->models.push_back(new Model(
			glm::vec3(x-1.8, y, z),
			this->materials[0],
			this->textures[TEX_CONTAINER],
			this->textures[TEX_CONTAINER_SPECULAR],
			meshes
		)
		);
	}
	
	for (x; x < length; x += 0.5) {
		this->models.push_back(new Model(
			glm::vec3(x -1.8, y, z),
			this->materials[0],
			this->textures[TEX_CONTAINER],
			this->textures[TEX_CONTAINER_SPECULAR],
			meshes
		)
		);
		this->models.push_back(new Model(
			glm::vec3(x , y, z+2),
			this->materials[0],
			this->textures[TEX_CONTAINER],
			this->textures[TEX_CONTAINER_SPECULAR],
			meshes
		)
		);
	}
	
	std::vector<float> vector;
	vector.push_back(x);
	vector.push_back(y);
	vector.push_back(z);

	return vector;
}
std::vector<float> Game::drawTurnLeft(std::vector<Mesh*>meshes, float x, float y, float z) {
	float turnWall = z - 2;
	float length = x - 4;
	for (z; z > turnWall; z -= 0.5) {
		this->models.push_back(new Model(
			glm::vec3(x , y, z),
			this->materials[0],
			this->textures[TEX_CONTAINER],
			this->textures[TEX_CONTAINER_SPECULAR],
			meshes
		)
		);
	}
	
	for (x; x > length; x -= 0.5) {
		this->models.push_back(new Model(
			glm::vec3(x, y, z),
			this->materials[0],
			this->textures[TEX_CONTAINER],
			this->textures[TEX_CONTAINER_SPECULAR],
			meshes
		)
		);
		this->models.push_back(new Model(
			glm::vec3(x-1.8, y, z + 2),
			this->materials[0],
			this->textures[TEX_CONTAINER],
			this->textures[TEX_CONTAINER_SPECULAR],
			meshes
		)
		);
	}
	
	
	
	std::vector<float> vector;
	vector.push_back(x);
	vector.push_back(y);
	vector.push_back(z);

	return vector;
}
std::vector<float> Game::drawTurnLeftZ(std::vector<Mesh*>meshes, float x, float y, float z) {
	
	float turnWall = x - 2;
	float length = z - 4;
	for (x; x < turnWall; x += 0.5) {
		this->models.push_back(new Model(
			glm::vec3(x, y, z),
			this->materials[0],
			this->textures[TEX_CONTAINER],
			this->textures[TEX_CONTAINER_SPECULAR],
			meshes
		)
		);
	}
	this->models.push_back(new Model(
		glm::vec3(x, y, z+0.5),
		this->materials[0],
		this->textures[TEX_CONTAINER],
		this->textures[TEX_CONTAINER_SPECULAR],
		meshes
	)
	);
	this->models.push_back(new Model(
		glm::vec3(x, y, z+1.f),
		this->materials[0],
		this->textures[TEX_CONTAINER],
		this->textures[TEX_CONTAINER_SPECULAR],
		meshes
	)
	);
	this->models.push_back(new Model(
		glm::vec3(x, y, z + 1.5),
		this->materials[0],
		this->textures[TEX_CONTAINER],
		this->textures[TEX_CONTAINER_SPECULAR],
		meshes
	)
	);
	for (z; z > length; z -= 0.5) {
		this->models.push_back(new Model(
			glm::vec3(x, y, z ),
			this->materials[0],
			this->textures[TEX_CONTAINER],
			this->textures[TEX_CONTAINER_SPECULAR],
			meshes
		)
		);
		this->models.push_back(new Model(
			glm::vec3(x - 1.8, y, z),
			this->materials[0],
			this->textures[TEX_CONTAINER],
			this->textures[TEX_CONTAINER_SPECULAR],
			meshes
		)
		);
	}


	std::vector<float> vector;
	vector.push_back(x);
	vector.push_back(y);
	vector.push_back(z);

	return vector;
}
std::vector<float> Game::drawTurnRightZ(std::vector<Mesh*>meshes, float x, float y, float z)
 {

	float turnWall = x - 2;
	float length = z - 4;
	for (x; x < turnWall; x += 0.5) {
		this->models.push_back(new Model(
			glm::vec3(x, y, z),
			this->materials[0],
			this->textures[TEX_CONTAINER],
			this->textures[TEX_CONTAINER_SPECULAR],
			meshes
		)
		);
	}
	this->models.push_back(new Model(
		glm::vec3(x-0.5, y, z+2 ),
		this->materials[0],
		this->textures[TEX_CONTAINER],
		this->textures[TEX_CONTAINER_SPECULAR],
		meshes
	)
	);
	this->models.push_back(new Model(
		glm::vec3(x-1.2, y, z+2 ),
		this->materials[0],
		this->textures[TEX_CONTAINER],
		this->textures[TEX_CONTAINER_SPECULAR],
		meshes
	)
	);
	this->models.push_back(new Model(
		glm::vec3(x-1.7, y, z +2),
		this->materials[0],
		this->textures[TEX_CONTAINER],
		this->textures[TEX_CONTAINER_SPECULAR],
		meshes
	)
	);
	this->models.push_back(new Model(
		glm::vec3(x - 1.8, y, z +1.5),
		this->materials[0],
		this->textures[TEX_CONTAINER],
		this->textures[TEX_CONTAINER_SPECULAR],
		meshes
	)
	);
	this->models.push_back(new Model(
		glm::vec3(x - 1.8, y, z + 1.f),
		this->materials[0],
		this->textures[TEX_CONTAINER],
		this->textures[TEX_CONTAINER_SPECULAR],
		meshes
	)
	);
	this->models.push_back(new Model(
		glm::vec3(x - 1.8, y, z + 0.5),
		this->materials[0],
		this->textures[TEX_CONTAINER],
		this->textures[TEX_CONTAINER_SPECULAR],
		meshes
	)
	);
	for (z; z > length; z -= 0.5) {
		this->models.push_back(new Model(
			glm::vec3(x, y, z),
			this->materials[0],
			this->textures[TEX_CONTAINER],
			this->textures[TEX_CONTAINER_SPECULAR],
			meshes
		)
		);
		this->models.push_back(new Model(
			glm::vec3(x - 1.8, y, z),
			this->materials[0],
			this->textures[TEX_CONTAINER],
			this->textures[TEX_CONTAINER_SPECULAR],
			meshes
		)
		);
	}


	std::vector<float> vector;
	vector.push_back(x);
	vector.push_back(y);
	vector.push_back(z);

	return vector;
}
std::vector<float> Game::drawCorridorRight(std::vector<Mesh*>meshes, float x, float y, float z)
{
	
	float length = x + 4;
	for (x; x < length; x += 0.5) {
		this->models.push_back(new Model(
			glm::vec3(x-2.f, y, z),
			this->materials[0],
			this->textures[TEX_CONTAINER],
			this->textures[TEX_CONTAINER_SPECULAR],
			meshes
		)
		);
		this->models.push_back(new Model(
			glm::vec3(x, y, z+2),
			this->materials[0],
			this->textures[TEX_CONTAINER],
			this->textures[TEX_CONTAINER_SPECULAR],
			meshes
		)
		);
	}
	std::vector<float> vector;
	vector.push_back(x);
	vector.push_back(y);
	vector.push_back(z);

	return vector;
}
std::vector<float> Game::drawCorridorLeft(std::vector<Mesh*>meshes, float x, float y, float z)
{
	
	float length = x - 4;
	for (x; x > length; x -= 0.5) {
		this->models.push_back(new Model(
			glm::vec3(x, y, z),
			this->materials[0],
			this->textures[TEX_CONTAINER],
			this->textures[TEX_CONTAINER_SPECULAR],
			meshes
		)
		);
		this->models.push_back(new Model(
			glm::vec3(x, y, z+2),
			this->materials[0],
			this->textures[TEX_CONTAINER],
			this->textures[TEX_CONTAINER_SPECULAR],
			meshes
		)
		);
	}
	std::vector<float> vector;
	vector.push_back(x);
	vector.push_back(y);
	vector.push_back(z);

	return vector;
}



void Game::initModels()
{
	int checker = true;
	float x;
	float y;
	float z;
	int labirinthLength = 7;
	int random = 0;
	std::vector<float> vector;
	std::vector<float> tempVector;

	std::vector<Mesh*>meshes;
	std::vector<Mesh*>meshes2;

	meshes.push_back(
		new Mesh(
			&Pyramid(),
			glm::vec3(1.f, 0.f, 0.f),
			glm::vec3(0.f),
			glm::vec3(0.f),
			glm::vec3(1.f)
		)
	);
	

	
	meshes2.push_back(
		new Mesh(
			&Quad(),
			glm::vec3(0.f, 0.f, 0.f),
			glm::vec3(0.f),
			glm::vec3(-90.f, 0.f, 0.f),
			glm::vec3(100.f)
		)
	);
	srand((unsigned)time(0));
	//FIRST CORRIDOR
	vector = drawCorridorForward(meshes, 0.2, 0.f, 0.f);
	for (int i = 0; i < labirinthLength; i++)
	{
		random = rand() % 3 +1;
		if (random == 1) {
			checker ? tempVector = drawCorridorForward(meshes, vector[0], vector[1], vector[2]) :
				vector = drawCorridorForward(meshes, tempVector[0], tempVector[1], tempVector[2]);
			checker = !checker;
		}
		if (random == 2) {
			checker ? tempVector = drawTurnRight(meshes, vector[0], vector[1], vector[2]) :
				vector = drawTurnRight(meshes, tempVector[0], tempVector[1], tempVector[2]);
			checker = !checker;
			while (random != 1) {
				random = rand() % 2 +1;
				if (random == 1) {
					checker ? tempVector = drawTurnLeftZ(meshes, vector[0], vector[1], vector[2]) :
						vector = drawTurnLeftZ(meshes, tempVector[0], tempVector[1], tempVector[2]);
					checker = !checker;
				}
				if (random == 2) {
					checker ? tempVector = drawCorridorRight(meshes, vector[0], vector[1], vector[2]) :
						vector = drawCorridorRight(meshes, tempVector[0], tempVector[1], tempVector[2]);
					checker = !checker;
				}
			}
		}
		if (random == 3) {
			checker ? tempVector = drawTurnLeft(meshes, vector[0], vector[1], vector[2]) :
				vector = drawTurnLeft(meshes, tempVector[0], tempVector[1], tempVector[2]);
			checker = !checker;
			while (random != 1) {
				random = rand() % 2 +1;
				if (random == 1) {
					checker ? tempVector = drawTurnRightZ(meshes, vector[0], vector[1], vector[2]) :
						vector = drawTurnRightZ(meshes, tempVector[0], tempVector[1], tempVector[2]);
					checker = !checker;
				}
				if (random == 2) {
					checker ? tempVector = drawCorridorLeft(meshes, vector[0], vector[1], vector[2]) :
						vector = drawCorridorLeft(meshes, tempVector[0], tempVector[1], tempVector[2]);
					checker = !checker;
				}
			}
		}
	}

	this->models.push_back(new Model(
		glm::vec3(0.f, 1.f, 0.f),
		this->materials[0],
		this->textures[TEX_CONTAINER],
		this->textures[TEX_CONTAINER_SPECULAR],
		meshes2
	)
	);



	glm::vec3(-5.5, 0.f, 0),
		this->models.push_back(new Model(
			glm::vec3(0.f, -5.f, 2.f),
			this->materials[0],
			this->textures[TEX_CONTAINER],
			this->textures[TEX_CONTAINER_SPECULAR],
			meshes2
		)
		);

	this->models.push_back(new Model(
		glm::vec3(4.f, 4.f, 4.f),
		this->materials[0],
		this->textures[TEX_CONTAINER],
		this->textures[TEX_CONTAINER_SPECULAR],
		"OBJFiles/ddh.obj"
	)
	);

	for (auto*& i : meshes)
		delete i;

	for (auto*& i : meshes2)
		delete i;
}

void Game::initPointLights()
{
	this->pointLights.push_back(new PointLight(glm::vec3(0.f)));
}

void Game::initLights()
{
	this->initPointLights();
}

void Game::initUniforms()
{
	//INIT UNIFORMS
	this->shaders[SHADER_CORE_PROGRAM]->setMat4fv(ViewMatrix, "ViewMatrix");
	this->shaders[SHADER_CORE_PROGRAM]->setMat4fv(ProjectionMatrix, "ProjectionMatrix");

	for each (PointLight* pl in this->pointLights)
	{
		pl->sendToShader(*this->shaders[SHADER_CORE_PROGRAM]);
	}
}

void Game::updateUniforms()
{
	//Update view matrix (camera)
	this->ViewMatrix = this->camera.getViewMatrix();

	this->shaders[SHADER_CORE_PROGRAM]->setMat4fv(this->ViewMatrix, "ViewMatrix");
	this->shaders[SHADER_CORE_PROGRAM]->setVec3f(this->camera.getPosition(), "cameraPos");

	for each (PointLight* pl in this->pointLights)
	{
		pl->sendToShader(*this->shaders[SHADER_CORE_PROGRAM]);
	}

	//Update framebuffer size and projection matrix
	glfwGetFramebufferSize(this->window, &this->framebufferWidth, &this->framebufferHeight);

	this->ProjectionMatrix = glm::perspective(
		glm::radians(this->fov),
		static_cast<float>(this->framebufferWidth) / this->framebufferHeight,
		this->nearPlane,
		this->farPlane
	);

	this->shaders[SHADER_CORE_PROGRAM]->setMat4fv(this->ProjectionMatrix, "ProjectionMatrix");
}

//Constructors / Destructors
Game::Game(
	const char* title,
	const int WINDOW_WIDTH, const int WINDOW_HEIGHT,
	const int GL_VERSION_MAJOR, const int GL_VERSION_MINOR,
	bool resizable
)
	:
	WINDOW_WIDTH(WINDOW_WIDTH),
	WINDOW_HEIGHT(WINDOW_HEIGHT),
	GL_VERSION_MAJOR(GL_VERSION_MAJOR),
	GL_VERSION_MINOR(GL_VERSION_MINOR),
	camera(glm::vec3(0.f, 0.f, 1.f), glm::vec3(0.f, 0.f, 1.f), glm::vec3(0.f, 1.f, 0.f))
{
	//Init variables
	this->window = nullptr;
	this->framebufferWidth = this->WINDOW_WIDTH;
	this->framebufferHeight = this->WINDOW_HEIGHT;

	this->camPosition = glm::vec3(0.f, 0.f, 1.f);
	this->worldUp = glm::vec3(0.f, 1.f, 0.f);
	this->camFront = glm::vec3(0.f, 0.f, -1.f);

	this->fov = 90.f;
	this->nearPlane = 0.1f;
	this->farPlane = 1000.f;

	this->dt = 0.f;
	this->curTime = 0.f;
	this->lastTime = 0.f;

	this->lastMouseX = 0.0;
	this->lastMouseY = 0.0;
	this->mouseX = 0.0;
	this->mouseY = 0.0;
	this->mouseOffsetX = 0.0;
	this->mouseOffsetY = 0.0;
	this->firstMouse = true;

	this->initGLFW();
	this->initWindow(title, resizable);
	this->initGLEW();
	this->initOpenGLOptions();

	this->initMatrices();
	this->initShaders();
	this->initTextures();
	this->initMaterials();
	this->initOBJModels();
	this->initModels();
	this->initLights();
	this->initUniforms();
}

Game::~Game()
{
	glfwDestroyWindow(this->window);
	glfwTerminate();
	
	for (size_t i = 0; i < this->shaders.size(); i++)
		delete this->shaders[i];

	for (size_t i = 0; i < this->textures.size(); i++)
		delete this->textures[i];

	for (size_t i = 0; i < this->materials.size(); i++)
		delete this->materials[i];

	for (auto*& i : this->models)
		delete i;

	for (size_t i = 0; i < this->pointLights.size(); i++)
		delete this->pointLights[i];
}

//Accessor
int Game::getWindowShouldClose()
{
	return glfwWindowShouldClose(this->window);
}

//Modifier
void Game::setWindowShouldClose()
{
	glfwSetWindowShouldClose(this->window, GLFW_TRUE);
}

//Functions
void Game::updateDt()
{
	this->curTime = static_cast<float>(glfwGetTime());
	this->dt = this->curTime - this->lastTime;
	this->lastTime = this->curTime;
}

void Game::updateMouseInput()
{
	glfwGetCursorPos(this->window, &this->mouseX, &this->mouseY);

	if (this->firstMouse)
	{
		this->lastMouseX = this->mouseX;
		this->lastMouseY = this->mouseY;
		this->firstMouse = false;
	}

	//Calc offset
	this->mouseOffsetX = this->mouseX - this->lastMouseX;
	this->mouseOffsetY = this->lastMouseY - this->mouseY;

	//Set last X and Y
	this->lastMouseX = this->mouseX;
	this->lastMouseY = this->mouseY;

	//Move light
	if (glfwGetMouseButton(this->window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS)
	{
		this->pointLights[0]->setPosition(this->camera.getPosition());
	}
}

void Game::updateKeyboardInput()
{
	//Program
	if (glfwGetKey(this->window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(this->window, GLFW_TRUE);
	}

	//Camera
	this->pointLights.front() = new PointLight(this->camera.getPosition());

	if (glfwGetKey(this->window, GLFW_KEY_W) == GLFW_PRESS)
	{
		this->camera.move(this->dt, FORWARD);
	}
	if (glfwGetKey(this->window, GLFW_KEY_S) == GLFW_PRESS)
	{
		this->camera.move(this->dt, BACKWARD);
	}
	if (glfwGetKey(this->window, GLFW_KEY_A) == GLFW_PRESS)
	{
		this->camera.move(this->dt, LEFT);
	}
	if (glfwGetKey(this->window, GLFW_KEY_D) == GLFW_PRESS)
	{
		this->camera.move(this->dt, RIGHT);
	}
	if (glfwGetKey(this->window, GLFW_KEY_C) == GLFW_PRESS)
	{
		this->camPosition.y -= 0.05f;
	}
	if (glfwGetKey(this->window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		this->camPosition.y += 0.05f;
	}
}

void Game::updateInput()
{
	glfwPollEvents();

	this->updateKeyboardInput();
	this->updateMouseInput();
	this->camera.updateInput(dt, -1, this->mouseOffsetX, this->mouseOffsetY);
}

void Game::update()
{
	//UPDATE INPUT ---
	this->updateDt();
	this->updateInput();

	//this->models[0]->rotate(glm::vec3(0.f, 1.f, 0.f));
	//this->models[1]->rotate(glm::vec3(0.f, 1.f, 0.f));
	//this->models[2]->rotate(glm::vec3(0.f, 1.f, 0.f));
}

void Game::render()
{
	//UPDATE --- 
	//updateInput(window);

	//DRAW ---
	//Clear
	glClearColor(0.f, 0.f, 0.f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	//Update the uniforms
	this->updateUniforms();

	//Render models
	for (auto&i : this->models)
		i->render(this->shaders[SHADER_CORE_PROGRAM]);

	//End Draw
	glfwSwapBuffers(window);
	glFlush();

	glBindVertexArray(0);
	glUseProgram(0);
	glActiveTexture(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

//Static functions
void Game::framebuffer_resize_callback(GLFWwindow* window, int fbW, int fbH)
{
	glViewport(0, 0, fbW, fbH);
};
