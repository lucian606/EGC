#include "Tema1.h"
#include <vector>
#include <iostream>
#include <vector>
#include <math.h>
#include <Core/Engine.h>
#include "Transform2D.h"
#include "Object2D.h"
#include <ctime>
#include <cstdio>

#define YELLOW 1
#define RED 2
#define LEFT 0
#define RIGHT 1
#define GREEN 3

using namespace std;
float balloon_length = 50;
float bow_y;
float bow_length = 75;
float arrow_length = 100;
float bow_x = 0;
float angular_step = 0;
float cx, cy;
float bow_angle = 0;
float shuriken_length = 25;
int player_score = 0;
int lives = 3;
int power_arrows = 0;
int difficulty = 1;
clock_t startMouseHold;
clock_t endMouseHold;
clock_t balloon_timer;
clock_t shuriken_timer;
bool clock_set = false;
bool mouse_release = false;
glm::ivec2 resolution;

typedef struct {
	glm::vec3 position;
	float launch_angle;
	double duration;
	double x_speed;
	double y_speed;
}Arrow; //Structure used to store details about an arrow

typedef struct {
	glm::vec3 position;
	int color;
	bool hit = false;
	float factor = 0;
	int direction;
}Balloon; //Structure used to store details about a balloon

typedef struct {
	glm::vec3 position;
	bool hit_target = false;
	bool hit = false;
}Shuriken; //Structure used to store details about a shuriken

vector<Arrow> arrows;
vector<Balloon> balloons;
vector<Shuriken> shurikens;


Tema1::Tema1()
{
}

Tema1::~Tema1()
{
}

Mesh* Tema1::CreateMesh(const char* name, const std::vector<VertexFormat>& vertices, const std::vector<unsigned short>& indices)
{
	unsigned int VAO = 0;
	// TODO: Create the VAO and bind it
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);


	// TODO: Create the VBO and bind it
	unsigned int VBO = 0;
	glGenBuffers(1, &VBO);

	// TODO: Send vertices data into the VBO buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

	// TODO: Crete the IBO and bind it
	unsigned int IBO = 0;
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	// TODO: Send indices data into the IBO buffer
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);
	// ========================================================================
	// This section describes how the GPU Shader Vertex Shader program receives data
	// It will be learned later, when GLSL shaders will be introduced
	// For the moment just think that each property value from our vertex format needs to be send to a certain channel
	// in order to know how to receive it in the GLSL vertex shader

	// set vertex position attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), 0);

	// set vertex normal attribute
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(sizeof(glm::vec3)));

	// set texture coordinate attribute
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3)));

	// set vertex color attribute
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3) + sizeof(glm::vec2)));
	// ========================================================================

	// TODO: Unbind the VAO
	glBindVertexArray(0);
	// Check for OpenGL errors
	CheckOpenGLError();

	// Mesh information is saved into a Mesh object
	meshes[name] = new Mesh(name);
	meshes[name]->InitFromBuffer(VAO, static_cast<unsigned short>(indices.size()));
	return meshes[name];
}

void Tema1::Init()
{
	resolution = window->GetResolution();
	auto camera = GetSceneCamera();
	camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
	camera->SetPosition(glm::vec3(0, 0, 50));
	camera->SetRotation(glm::vec3(0, 0, 0));
	camera->Update();
	GetCameraInput()->SetActive(false);

	glm::vec3 corner = glm::vec3(0, 0, 0);
	

	// compute coordinates of square center

	// initialize angularStep
	angularStep = 0;

	cx = corner.x;
	cy = corner.y;

	Mesh* arrowHead = Object2D::CreateArrowHead("arrowHead", corner, arrow_length, glm::vec3(0, 1, 0));
	AddMeshToList(arrowHead);

	Mesh* arrowBody = Object2D::CreateArrowBody("arrowBody", corner, arrow_length, glm::vec3(0, 1, 0));
	AddMeshToList(arrowBody);
	
	Mesh* bowBody = Object2D::CreateBow("bowBody", corner, bow_length, glm::vec3(0.8, 0.4, 0));
	AddMeshToList(bowBody);

	Mesh* balloonBodyRed = Object2D::CreateBaloonBody("balloonBodyRed", corner, balloon_length, glm::vec3(1, 0, 0));
	AddMeshToList(balloonBodyRed);

	Mesh* balloonTriangleRed = Object2D::CreateBaloonTriangle("balloonTriangleRed", corner, balloon_length, glm::vec3(1, 0, 0));
	AddMeshToList(balloonTriangleRed);

	Mesh* balloonStringRed = Object2D::CreateBaloonString("balloonStringRed", corner, balloon_length, glm::vec3(1, 0, 0));
	AddMeshToList(balloonStringRed);

	Mesh* balloonBodyYellow = Object2D::CreateBaloonBody("balloonBodyYellow", corner, balloon_length, glm::vec3(1, 1, 0));
	AddMeshToList(balloonBodyYellow);

	Mesh* balloonTriangleYellow = Object2D::CreateBaloonTriangle("balloonTriangleYellow", corner, balloon_length, glm::vec3(1, 1, 0));
	AddMeshToList(balloonTriangleYellow);

	Mesh* balloonStringYellow = Object2D::CreateBaloonString("balloonStringYellow", corner, balloon_length, glm::vec3(1, 1, 0));
	AddMeshToList(balloonStringYellow);

	Mesh* balloonBodyGreen = Object2D::CreateBaloonBody("balloonBodyGreen", corner, balloon_length, glm::vec3(0, 1, 0));
	AddMeshToList(balloonBodyGreen);

	Mesh* balloonTriangleGreen = Object2D::CreateBaloonTriangle("balloonTriangleGreen", corner, balloon_length, glm::vec3(0, 1, 0));
	AddMeshToList(balloonTriangleGreen);

	Mesh* balloonStringGreen = Object2D::CreateBaloonString("balloonStringGreen", corner, balloon_length, glm::vec3(0, 1, 0));
	AddMeshToList(balloonStringGreen);

	Mesh* powerBar = Object2D::CreateRectangle("power", corner, bow_length, glm::vec3(0, 1, 0), true);
	AddMeshToList(powerBar);

	Mesh* star1 = Object2D::CreateStar1("star1", corner, shuriken_length, glm::vec3(0.55, 0.55, 0.55));
	AddMeshToList(star1);

	Mesh* star2 = Object2D::CreateStar2("star2", corner, shuriken_length, glm::vec3(0.55, 0.55, 0.55));
	AddMeshToList(star2);

	Mesh* star3 = Object2D::CreateStar3("star3", corner, shuriken_length, glm::vec3(0.55, 0.55, 0.55));
	AddMeshToList(star3);

	Mesh* star4 = Object2D::CreateStar4("star4", corner, shuriken_length, glm::vec3(0.55, 0.55, 0.55));
	AddMeshToList(star4);
}

void Tema1::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0.5, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}

void Tema1::Update(float deltaTimeSeconds)
{

	// TODO: update steps for translation, rotation, scale, in order to create animations
	resolution = window->GetResolution();
	double duration; // duration of the mouse hold
	clock_t endMouseHold = clock();
	duration = double  (endMouseHold - startMouseHold) / CLOCKS_PER_SEC;
	if (duration > 3) {
		duration = 3;
	} // getting the duration of the mouse hold to render the power bar
	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform2D::Translate(bow_x, bow_y);
	modelMatrix *= Transform2D::Rotate(bow_angle);
	angular_step += deltaTimeSeconds;
	RenderMesh2D(meshes["bowBody"], shaders["VertexColor"], modelMatrix);
	if (arrows.size() == 0) {
		modelMatrix *= Transform2D::Translate(arrow_length, 0);
		RenderMesh2D(meshes["arrowHead"], shaders["VertexColor"], modelMatrix);
		RenderMesh2D(meshes["arrowBody"], shaders["VertexColor"], modelMatrix);
	} // if the arrow isnt fired we render it inside the bow
	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform2D::Translate(bow_x - (3 - (double) duration) * bow_length / 3, bow_y - bow_length - 20);
	if (clock_set) {
		RenderMesh2D(meshes["power"], shaders["VertexColor"], modelMatrix);
	} // if the mouse is hold we display the power bar
	if (mouse_release) {
		clock_set = false;
		mouse_release = false;
	}
	for (int i = 0; i < arrows.size(); i++) {
		bool shuriken_hit = false;
		glm::vec3 arrow_center = arrows[i].position; // getting the arrow position
		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(arrow_center.x, arrow_center.y);
		modelMatrix *= Transform2D::Rotate(arrows[i].launch_angle);
		RenderMesh2D(meshes["arrowHead"], shaders["VertexColor"], modelMatrix);
		RenderMesh2D(meshes["arrowBody"], shaders["VertexColor"], modelMatrix); //rendering the arrow
		for (int j = 0; j < balloons.size(); j++) {
			Balloon ballon = balloons[j];
			glm::vec3 balloon_center = ballon.position;
			float balloon_x = balloon_center.x;
			float balloon_y = balloon_center.y;
			float arrow_head_x = arrow_center.x;
			float arrow_head_y = arrow_center.y;
			if (balloons[j].hit == false && balloons[j].factor == 0) {
				float dx = (balloon_x - arrow_head_x);
				dx = dx * dx;
				dx = dx / (balloon_length * balloon_length * 0.25);
				float dy = (balloon_y - arrow_head_y);
				dy = dy * dy;
				dy = dy / (balloon_length * balloon_length * 0.5625);
				if (dx + dy <= 1) {
					balloons[j].hit = true;
					if (balloons[j].color == YELLOW)
						player_score -= (5 * difficulty);
					else if (balloons[j].color == RED)
						player_score += (10 * difficulty);
					else if (balloons[j].color == GREEN) {
						player_score += (10 * difficulty);
						power_arrows = 10;
						cout << "You have gotten the fast arrows power up. Your next 10 arrows move at max speed and have no reload time." << endl;
					}
					cout << player_score << endl;
				} //checking balloon collision
			}
		}
		for (int j = 0; j < shurikens.size(); j++) {
			Shuriken shuriken = shurikens[j];
			glm::vec3 shuriken_center = shuriken.position;
			float shuriken_x = shuriken_center.x;
			float shuriken_y = shuriken_center.y;
			float arrow_head_x = arrow_center.x;
			float arrow_head_y = arrow_center.y;
			float dx = arrow_head_x - shuriken_x;
			float dy = arrow_head_y - shuriken_y;
			float distance = sqrt(dx * dx + dy * dy);
			if (distance < shuriken_length) {
				if (shuriken.hit == false) {
					player_score += (5 * difficulty);
					cout << player_score << endl;
					shurikens[j].hit = true;
				} // checking if the shuriken wasn't hit already
				arrows[i].position.x = resolution.x + arrow_length;
				shuriken_hit = true;
			} // if the arrow hit a shuriken we remove it from the screen
		} // checking the shuriken collision
		if (!shuriken_hit) {
			arrow_center += glm::vec3(arrows[i].x_speed * arrows[i].duration * deltaTimeSeconds, arrows[i].y_speed * arrows[i].duration * deltaTimeSeconds, 0);
			arrows[i].position = arrow_center;
		} // if the arrow didn't hit a shuriken we move it
	}
	
	//removing the off screen arrows
	arrows.erase(remove_if(arrows.begin(), arrows.end(), [](Arrow arrow) {return arrow.position.x > resolution.x; }), arrows.end()); //Remove arrows which are out of the screen
	arrows.erase(remove_if(arrows.begin(), arrows.end(), [](Arrow arrow) {return arrow.position.y > resolution.y + arrow_length; }), arrows.end()); //Remove arrows which are out of the screen
	arrows.erase(remove_if(arrows.begin(), arrows.end(), [](Arrow arrow) {return arrow.position.y < -arrow_length; }), arrows.end()); //Remove arrows which are out of the screen
	

	for (int i = 0; i < balloons.size(); i++) {
		Balloon b = balloons[i];
		if (b.hit) {
				modelMatrix = glm::mat3(1);
				float factor = balloons[i].factor;
				modelMatrix *= Transform2D::Translate(b.position.x, b.position.y);
				modelMatrix *= Transform2D::Scale(1 - factor, 1 - factor);
				if (b.color == YELLOW) {
					RenderMesh2D(meshes["balloonBodyYellow"], shaders["VertexColor"], modelMatrix);
					RenderMesh2D(meshes["balloonTriangleYellow"], shaders["VertexColor"], modelMatrix);
				}
				else if (b.color == RED) {
					RenderMesh2D(meshes["balloonBodyRed"], shaders["VertexColor"], modelMatrix);
					RenderMesh2D(meshes["balloonTriangleRed"], shaders["VertexColor"], modelMatrix);
				}
				else {
					RenderMesh2D(meshes["balloonBodyGreen"], shaders["VertexColor"], modelMatrix);
					RenderMesh2D(meshes["balloonTriangleGreen"], shaders["VertexColor"], modelMatrix);
				}
				balloons[i].factor += deltaTimeSeconds;
		}
	} // rendering the hit balloons (for animation)

	// erasing the hit balloons and the ones off screen
	balloons.erase(remove_if(balloons.begin(), balloons.end(), [](Balloon balloon) {return balloon.factor >= 1; }), balloons.end()); //Remove hit balloons
	balloons.erase(remove_if(balloons.begin(), balloons.end(), [](Balloon balloon) {return balloon.position.y >= resolution.y; }), balloons.end()); //Remove hit balloons

	for (int i = 0; i < balloons.size(); i++) {
		Balloon balloon = balloons[i];
		glm::vec3 balloon_center = balloon.position;
		modelMatrix = glm::mat3(1);
		if (balloons[i].hit == false) {
			modelMatrix *= Transform2D::Translate(balloon_center.x, balloon_center.y);
			if (balloon.color == YELLOW) {
				RenderMesh2D(meshes["balloonBodyYellow"], shaders["VertexColor"], modelMatrix);
				RenderMesh2D(meshes["balloonTriangleYellow"], shaders["VertexColor"], modelMatrix);
				RenderMesh2D(meshes["balloonStringYellow"], shaders["VertexColor"], modelMatrix);
			}
			else if (balloon.color == RED) {
				RenderMesh2D(meshes["balloonBodyRed"], shaders["VertexColor"], modelMatrix);
				RenderMesh2D(meshes["balloonTriangleRed"], shaders["VertexColor"], modelMatrix);
				RenderMesh2D(meshes["balloonStringRed"], shaders["VertexColor"], modelMatrix);
			}
			else {
				RenderMesh2D(meshes["balloonBodyGreen"], shaders["VertexColor"], modelMatrix);
				RenderMesh2D(meshes["balloonTriangleGreen"], shaders["VertexColor"], modelMatrix);
				RenderMesh2D(meshes["balloonStringGreen"], shaders["VertexColor"], modelMatrix);
			}
			float x_dist;
			if (balloon.direction == LEFT)
				x_dist = -150 * deltaTimeSeconds;
			else
				x_dist = 150 * deltaTimeSeconds;
			if (balloon.position.x <= resolution.x / 2)
				balloons[i].direction = RIGHT;
			else if (balloon.position.x >= resolution.x)
				balloons[i].direction = LEFT;
			balloon_center += glm::vec3(x_dist * difficulty, 150 * deltaTimeSeconds * difficulty, 0);
			balloons[i].position = balloon_center;
		}
	} // rendering the balloons not hit

	for (int i = 0; i < shurikens.size(); i++) {
		Shuriken shuriken = shurikens[i];
		glm::vec3 shuriken_center = shuriken.position;
		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(shuriken_center.x, shuriken_center.y);
		modelMatrix *= Transform2D::Translate(cx, cy);
		modelMatrix *= Transform2D::Rotate(25 * angular_step);
		modelMatrix *= Transform2D::Translate(-cx, -cy);
		RenderMesh2D(meshes["star1"], shaders["VertexColor"], modelMatrix);
		RenderMesh2D(meshes["star2"], shaders["VertexColor"], modelMatrix);
		RenderMesh2D(meshes["star3"], shaders["VertexColor"], modelMatrix);
		RenderMesh2D(meshes["star4"], shaders["VertexColor"], modelMatrix);
		float dx = bow_x - shuriken_center.x;
		float dy = bow_y - shuriken_center.y;
		float distance = sqrt(dx * dx + dy * dy);
		if (distance < bow_length + shuriken_length && shurikens[i].hit_target == false) {
			lives--;
			shurikens[i].hit_target = true;
			cout << "You have been hit, you have " << lives << " lives left!"  << endl;
			if (lives == 0) {
				cout << "You have lost all your lives. GAME OVER!" << endl;
				window->Close();
				exit(1);
			}
		} // checking if the player was hit by a shuriken
		shuriken_center.x -= 200 * deltaTimeSeconds * difficulty;
		shurikens[i].position = shuriken_center;
	} // rendering the shurikens


	if (balloons.size() < 10) {
		clock_t end_balloon = clock();
		float duration = double(end_balloon - balloon_timer) / CLOCKS_PER_SEC;
		if (duration >= (0.35f * difficulty) || balloons.size() == 0) {
			balloon_timer = end_balloon;
			Balloon newBalloon;
			int random_color = rand() % 15;
			int wind_direction = rand() % 2;
			int lower_bound = (int)(resolution.x / 2);
			int upper_bound = (int)(resolution.x - 3 * balloon_length);
			int random_pos = rand() % upper_bound + lower_bound;
			newBalloon.position = glm::vec3(random_pos, 0, 0);
			newBalloon.direction = wind_direction;
			if (random_color < 10)
				newBalloon.color = RED;
			else if (random_color >= 10)
				newBalloon.color = YELLOW;
			if (random_color == 14)
				newBalloon.color = GREEN;
			balloons.push_back(newBalloon);
		}
	} // generating ballons

	if (shurikens.size() < 2 * difficulty) {
		clock_t end_shuriken = clock();
		Shuriken newShuriken;
		float duration = double(end_shuriken - shuriken_timer) / CLOCKS_PER_SEC;
		if (duration >= 0.5f || shurikens.size() == 0) {
			int random_height = rand() % ((int)(resolution.y - shuriken_length));
			newShuriken.position = glm::vec3(resolution.x, random_height, 0);
			shurikens.push_back(newShuriken);
			shuriken_timer = end_shuriken;
		}
	} // rendering shurikens

	// removing off screen shurikens
	shurikens.erase(remove_if(shurikens.begin(), shurikens.end(), [](Shuriken shuriken) {return shuriken.position.x <= -shuriken_length; }), shurikens.end());
}

void Tema1::FrameEnd()
{

}

void Tema1::OnInputUpdate(float deltaTime, int mods)
{
	// add key press event
	if (window->KeyHold(GLFW_KEY_W) && bow_y <= resolution.y)
		bow_y += 500 * deltaTime;
	if (window->KeyHold(GLFW_KEY_S) && bow_y >= 0)
		bow_y -= 500 * deltaTime;
}

void Tema1::OnKeyPress(int key, int mods)
{
	if (key == GLFW_KEY_E) {
		cout << "Difficulty set to Easy. Normal points gain." << endl;
		difficulty = 1;
	}
	else if (key == GLFW_KEY_M) {
		cout << "Difficulty set to Medium. Double points gain." << endl;
		difficulty = 2;
	}
	else if (key == GLFW_KEY_H) {
		cout << "Difficulty set to Hard. Triple points gain." << endl;
		difficulty = 3;
	}
}

void Tema1::OnKeyRelease(int key, int mods)
{
	// add key release event
}

void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event
	glm::vec3 launch_pos = glm::vec3(bow_x, bow_y, 0);
	float new_mouse_y = window->GetResolution().y - mouseY;
	bow_angle = atan((new_mouse_y - bow_y) / (mouseX - bow_x));
} // getting the angle of the mouse for the rotation of the bow

void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
	if (arrows.size() == 0 || power_arrows > 0) {
		startMouseHold = clock();
		clock_set = true;
	}
} // starting the clock for the duration of the mouse hold

void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
	if (arrows.size() == 0 || power_arrows > 0) {
		double duration;
		clock_t endMouseHold = clock();
		duration = double(endMouseHold - startMouseHold) / CLOCKS_PER_SEC;
		if (duration > 3 || power_arrows > 0) {
			duration = 3;
			power_arrows--;
		}
		duration = 10 + duration * 25;
		glm::vec3 launch_pos = glm::vec3(bow_x, bow_y, 0);
		Arrow new_arrow;
		new_arrow.position = launch_pos;
		float new_mouse_y = window->GetResolution().y - mouseY;
		new_arrow.launch_angle = atan((new_mouse_y - bow_y) / (mouseX - bow_x));
		new_arrow.duration = duration;
		new_arrow.x_speed = 20 * cos(new_arrow.launch_angle);
		new_arrow.y_speed = 20 * sin(new_arrow.launch_angle);
		if (mouseX > bow_x) {
			arrows.push_back(new_arrow);
		}
		mouse_release = true;
	}
} // releasing the arrow based on the duration the mouse was hold

void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Tema1::OnWindowResize(int width, int height)
{
}
