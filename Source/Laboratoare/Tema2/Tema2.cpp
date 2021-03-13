#include "Tema2.h"
#include <vector>
#include <string>
#include <iostream>
#include <ctime>
#include <Core/Engine.h>
#include "Transform3D.h"
#include "Transform2D.h"
#include <Windows.h>

#define MID 0
#define LEFT -3
#define RIGHT 3
#define MAX_FUEL 100
#define BLUE 0
#define GREEN 1
#define ORANGE 2
#define YELLOW 3
#define RED 4

using namespace std;

float start_point = 0;
float scroll_speed = 5;
float vertical_speed = 0;
float gravity = 0.75;
float ballLength = 0.5;
float fuel = MAX_FUEL;
float platform_height = 0.1f;
float platform_width = 2;
float curr_z = 0;
float player_x = 0;
float player_y = 0.5;
float player_z = 0;
float platform_length = 10;
float old_speed = 0;
int directions[3] = { MID, LEFT, RIGHT };
bool jump = false;
bool firstPerson = false;
bool isCollision = true;
bool gameOver = false;
bool isPowerUp = false;
bool gameStarted = false;
vector<int> colors;
glm::vec3 lightPosition;
glm::mat3 fuelMat;
clock_t powerUpStart;

typedef struct {
	glm::vec3 position;
	float scaleFactor;
	bool visited = false;
	bool firstContact = false;
	short color;
} Platform;

vector<Platform> platforms;


Tema2::Tema2()
{
}

Tema2::~Tema2()
{
}

Mesh* CreateFuelBar(std::string name, glm::vec3 corner, float length,const glm::vec3& color) {

	std::vector<VertexFormat> vertices
	{
		VertexFormat(corner, color), // 0
		VertexFormat(corner + glm::vec3(length, 0, 0), color), // 1
		VertexFormat(corner + glm::vec3(length, abs(length) * 0.2, 0), color), // 2
		VertexFormat(corner + glm::vec3(0, abs(length) * 0.2, 0), color), // 3
	};

	std::vector<unsigned short> indices
	{ 2, 1, 0,
	  2, 3, 0
	};

	Mesh* star = new Mesh(name);
	star->SetDrawMode(GL_TRIANGLE_FAN);

	star->InitFromData(vertices, indices);
	return star;
}

void Tema2::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color)
{
	if (!mesh || !shader || !shader->GetProgramID())
		return;

	// render an object using the specified shader and the specified position
	glUseProgram(shader->program);
	// Set shader uniforms for light & material properties
	// TODO: Set light position uniform
	GLint lightPos = glGetUniformLocation(shader->program, "light_position");
	glUniform3fv(lightPos, 1, glm::value_ptr(lightPosition));
	// TODO: Set eye position (camera position) uniform
	GLint eyePos = glGetUniformLocation(shader->program, "eye_position");
	glUniform3fv(eyePos, 1, glm::value_ptr(camera->GetTargetPosition()));
		
	// Bind model matrix
	GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
	glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	// Bind view matrix
	glm::mat4 viewMatrix = camera->GetViewMatrix();
	int loc_view_matrix = glGetUniformLocation(shader->program, "View");
	glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

	// Bind projection matrix
	int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
	glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

	// Get shader location for "Time"
	GLint timeLocation = glGetUniformLocation(shader->GetProgramID(), "time");
	// Set shader uniform "Time" to elapsed time
	glUniform1f(timeLocation, (GLfloat)Engine::GetElapsedTime());

	glUniform3fv(glGetUniformLocation(shader->program, "object_color"), 1, glm::value_ptr(color));

	// Draw the object
	glBindVertexArray(mesh->GetBuffers()->VAO);
	glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_SHORT, 0);
}

void Tema2::Init()
{
	polygonMode = GL_FILL;
	lightPosition = glm::vec3(0, 1, 1);
	materialShininess = 30;
	materialKd = 0.5;
	materialKs = 0.5;

	renderCameraTarget = false;

	camera = new Laborator::Camera();
	camera->Set(glm::vec3(0, 3, 3.5f), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));

	projectionMatrix = glm::perspective(RADIANS(90), window->props.aspectRatio, 0.01f, 200.0f);

	glm::vec3 origin = glm::vec3(0, 0, 0);

	Mesh* mesh = new Mesh("sphere");
	mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "sphere.obj");
	meshes[mesh->GetMeshID()] = mesh;

	mesh = new Mesh("platform");
	mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "box.obj");
	meshes[mesh->GetMeshID()] = mesh;

	Mesh* fuelBar = CreateFuelBar("fuelBar", glm::vec3(0, 0, 0), 2, glm::vec3(0, 1, 0));
	AddMeshToList(fuelBar);

	Mesh* consumeBar = CreateFuelBar("consumeBar", glm::vec3(0, 0, 0), -2, glm::vec3(1, 1, 1));
	AddMeshToList(consumeBar);

	//Light & material properties
	{
		lightPosition = glm::vec3(0, 1, 1);
		materialShininess = 30;
		materialKd = 0.5;
		materialKs = 0.5;
	}

	for (int i = 1; i <= 10; i++) {
		colors.push_back(BLUE);
	}


	for (int i = 1; i <= 5; i++) {
		colors.push_back(GREEN);
	}


	for (int i = 1; i <= 10; i++) {
		colors.push_back(ORANGE);
	}

	for (int i = 1; i <= 15; i++) {
		colors.push_back(YELLOW);
		colors.push_back(RED);
	}

	// Generating initial platforms
	{
		Platform p;
		p.position = glm::vec3(MID, 0, -2.5);
		p.scaleFactor = 1;
		p.color = BLUE;
		platforms.push_back(p);
		p.position = glm::vec3(RIGHT, 0, -2.5);
		platforms.push_back(p);
		p.position = glm::vec3(1, 0, -2.5);
		platforms.push_back(p);
		p.position = glm::vec3(-1, 0, -2.5);
		platforms.push_back(p);
		p.position = glm::vec3(LEFT, 0, -2.5);
		platforms.push_back(p);
	}

	// Adding the shader used for platforms
	{
		Shader* shader = new Shader("Shader");
		shader->AddShader("Source/Laboratoare/Tema2/Shaders/VertexShader.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Source/Laboratoare/Tema2/Shaders/FragmentShader.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}

	// Adding the shader used for the powerup platforms
	{
		Shader* discoShader = new Shader("NoiseShader");
		discoShader->AddShader("Source/Laboratoare/Tema2/Shaders/NoiseVertex.glsl", GL_VERTEX_SHADER);
		discoShader->AddShader("Source/Laboratoare/Tema2/Shaders/NoiseFragment.glsl", GL_FRAGMENT_SHADER);
		discoShader->CreateAndLink();
		shaders[discoShader->GetName()] = discoShader;
	}
}

void Tema2::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}

void Tema2::Update(float deltaTimeSeconds)
{
	glLineWidth(3);
	glPointSize(5);
	glPolygonMode(GL_FRONT_AND_BACK, polygonMode);

	// Checking GAME OVER
	{

		if (fuel <= 0) {
			cout << "Game over! You are out of fuel!" << endl;
			gameOver = true;
		}

		if (player_y < -10) {
			cout << "Game over! You fell down!" << endl;
			gameOver = true;
		}

		if (gameOver) {
			exit(0);
		}

	}

	// Checking if the powerup is active
	{
		if (isPowerUp) {
			clock_t endPowerUp = clock();
			double duration = double(endPowerUp - powerUpStart) / CLOCKS_PER_SEC;
			if (duration >= 3) {
				isPowerUp = false;
				scroll_speed = old_speed;
			}
		}
	}

	// Checking the camera
	{
		if (firstPerson) {
			camera->Set(glm::vec3(player_x, player_y, player_z - 0.5f), glm::vec3(player_x, player_y, player_z - 0.6f), glm::vec3(0, 1, 0));
		}
		else {
			camera->Set(glm::vec3(0, 3, 3.5f), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));
		}
	}

	// Rendering the player
	{
		scroll_speed = scroll_speed > 5 ? scroll_speed : 5;
		modelMatrix = glm::mat4(1);
		modelMatrix *= Transform3D::Translate(player_x, player_y, player_z);
		modelMatrix *= Transform3D::Scale(0.5, 0.5, 0.5);
		if (isPowerUp) {
			RenderSimpleMesh(meshes["sphere"], shaders["NoiseShader"], modelMatrix, glm::vec3(0.6, 0.3, 0.8));
		}
		else {
			RenderSimpleMesh(meshes["sphere"], shaders["Shader"], modelMatrix, glm::vec3(1, 0, 1));
		}
	}

	// Rendering the fuel 
	{
		float fuelScale = fuel / MAX_FUEL;
		fuelMat = glm::mat3(1);
		fuelMat *= Transform2D::Translate(0, 2);
		fuelMat *= Transform2D::Scale(fuelScale, 1);
		RenderMesh2D(meshes["fuelBar"], shaders["VertexColor"], fuelMat);
		fuelMat = glm::mat3(1);
		fuelMat *= Transform2D::Translate(2, 2);
		fuelMat *= Transform2D::Scale(1.0f - fuelScale, 1);
		RenderMesh2D(meshes["consumeBar"], shaders["VertexColor"], fuelMat);
	}

	// Checking the jump
	{
		if (jump) {
			vertical_speed = 12.5;
		}
	}

	// Adjusting the fuel
	{
		fuel -= deltaTimeSeconds;
	}

	// Checking the collision between player and platforms
	{
		isCollision = false;
		for (int i = 0; i < platforms.size(); i++) {
			Platform p = platforms[i];
			if (player_x >= p.position.x - platform_width * 0.5f && player_x <= p.position.x + 0.5f * platform_width && player_z >= p.position.z - platform_length * p.scaleFactor / 2 && player_z <= p.position.z + platform_length * p.scaleFactor / 2 && player_y - 0.5 >= p.position.y - 0.1 && player_y - 0.5 <= p.position.y + 0.1) {
				isCollision = true;
				if (!p.firstContact) {
					switch (p.color) {
					case GREEN:
						fuel += 30;
						fuel = fuel > 100 ? 100 : fuel;
						break;
					case RED:
						gameOver = true;
						cout << "Game over! You touched a red, yet deadly platform." << endl;
						break;
					case YELLOW:
						fuel -= 10;
						break;
					case ORANGE:
						if (!isPowerUp)
							old_speed = scroll_speed;
						isPowerUp = true;
						powerUpStart = clock();
						scroll_speed = 20;
						break;
					}
					platforms[i].firstContact = true;
				}
				platforms[i].visited = true;
			}
			else {
				platforms[i].visited = false;
			}
		}
	}

	// Adjusting the player's verticality based on the vertical speed
	{
		gravity = 0.5;
		vertical_speed -= gravity;
		if (isCollision && !jump)
			vertical_speed = 0;
		player_y += vertical_speed * deltaTimeSeconds;
		jump = false;
	}

	// Rendering the platforms and moving them
	{
		for (int i = 0; i < platforms.size(); i++) {
			Platform p = platforms[i];
			modelMatrix = glm::mat4(1);
			modelMatrix *= Transform3D::Translate(0, 0, 0);
			modelMatrix *= Transform3D::Translate(p.position.x, p.position.y, p.position.z);
			modelMatrix *= Transform3D::Scale(2, 0.2, platform_length * p.scaleFactor);
			if (platforms[i].visited) {
				RenderSimpleMesh(meshes["platform"], shaders["Shader"], modelMatrix, glm::vec3(1, 1, 1));
			}
			else {
				switch (p.color) {
				case RED:
					RenderSimpleMesh(meshes["platform"], shaders["Shader"], modelMatrix, glm::vec3(1, 0, 0));
					break;
				case GREEN:
					RenderSimpleMesh(meshes["platform"], shaders["Shader"], modelMatrix, glm::vec3(0, 1, 0));
					break;
				case BLUE:
					RenderSimpleMesh(meshes["platform"], shaders["Shader"], modelMatrix, glm::vec3(0, 0, 1));
					break;
				case YELLOW:
					RenderSimpleMesh(meshes["platform"], shaders["Shader"], modelMatrix, glm::vec3(1, 1, 0));
					break;
				case ORANGE:
					RenderSimpleMesh(meshes["platform"], shaders["Shader"], modelMatrix, glm::vec3(1, 0.5f, 0));
					break;
				}
			}
			if(gameStarted)
				p.position.z += deltaTimeSeconds * scroll_speed;
			platforms[i] = p;
		}
	}

	// Generating platforms if not enoguh
	{
		if (platforms.size() < 20) {
			int num_of_platforms = rand() % 3;
			num_of_platforms++;
			Platform lastPlatform = platforms[platforms.size() - 1];
			float distanceFactor = rand() % 11;
			int generateBadPlatform = num_of_platforms - 1;
			distanceFactor /= 10;
			float r = rand() % 10;
			r += 5;
			r /= 10;
			float new_z = lastPlatform.position.z - lastPlatform.scaleFactor * platform_length * 0.5 - r * platform_length * 0.5 - 2 * distanceFactor;
			bool generated[3];
			for (int i = 0; i <= 2; i++) {
				generated[i] = false;
			}
			for (int i = 1; i <= num_of_platforms; i++) {
				int direction = rand() % 3;
				if (!generated[direction]) {
					int colorIndex;
					Platform p;
					p.scaleFactor = r;
					p.position.z = new_z;
					p.position.y = 0;
					p.position.x = directions[direction];
					if (generateBadPlatform > 0) {
						colorIndex = rand() % 55;
						generateBadPlatform--;
					}
					else {
						colorIndex = rand() % 25;
					}
					p.color = colors[colorIndex];
					platforms.push_back(p);
					generated[direction] = true;
				}
				else {
					i--;
				}
			}
		}
	}

	// Removing the platforms which are off screen
	{
		platforms.erase(remove_if(platforms.begin(), platforms.end(), [](Platform p) {return p.position.z > 10; }), platforms.end());
	}
}

void Tema2::FrameEnd()
{
}

void Tema2::OnInputUpdate(float deltaTime, int mods)
{
	if (window->KeyHold(GLFW_KEY_A))
		player_x -= 10 * deltaTime;
	if (window->KeyHold(GLFW_KEY_D))
		player_x += 10 * deltaTime;
	if (window->KeyHold(GLFW_KEY_W) && scroll_speed <= 15 && !isPowerUp)
		scroll_speed += 10 * deltaTime;
	if (window->KeyHold(GLFW_KEY_S) && scroll_speed >= 5 && !isPowerUp)
		scroll_speed -= 10 * deltaTime;
}

void Tema2::OnKeyPress(int key, int mods)
{
	if (key == GLFW_KEY_SPACE && isCollision) {
		jump = true;
	}

	if (key == GLFW_KEY_C) {
		firstPerson = !firstPerson;
	}

	if (key == GLFW_KEY_SPACE || key == GLFW_KEY_W || key == GLFW_KEY_P) {
		gameStarted = true;
	}
}

void Tema2::OnKeyRelease(int key, int mods)
{
	// add key release event
}

void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event
}

void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
}

void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Tema2::OnWindowResize(int width, int height)
{
}