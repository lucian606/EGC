#include "Object2D.h"

#include <Core/Engine.h>

Mesh* Object2D::CreateRectangle(std::string name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill)
{
	glm::vec3 corner = leftBottomCorner;

	std::vector<VertexFormat> vertices =
	{
		VertexFormat(corner, color),
		VertexFormat(corner + glm::vec3(length, 0, 0), color),
		VertexFormat(corner + glm::vec3(length, 10, 0), color),
		VertexFormat(corner + glm::vec3(0, 10, 0), color)
	};

	Mesh* square = new Mesh(name);
	std::vector<unsigned short> indices = { 0, 1, 2, 3 };

	if (!fill) {
		square->SetDrawMode(GL_LINE_LOOP);
	}
	else {
		// draw 2 triangles. Add the remaining 2 indices
		indices.push_back(0);
		indices.push_back(2);
	}

	square->InitFromData(vertices, indices);
	return square;
}

Mesh* Object2D::CreateStar1(std::string name, glm::vec3 center, float length, glm::vec3 color) {
	glm::vec3 corner = center;

	std::vector<VertexFormat> vertices
	{
		VertexFormat(corner, color), // 0
		VertexFormat(corner + glm::vec3(0, length, 0), color), // 1
		VertexFormat(corner + glm::vec3(-length, length, 0), color), // 2
	};

	std::vector<unsigned short> indices
	{ 0, 1, 2,
	};

	Mesh* star = new Mesh(name);
	star->SetDrawMode(GL_TRIANGLE_FAN);

	star->InitFromData(vertices, indices);
	return star;
}

Mesh* Object2D::CreateStar2(std::string name, glm::vec3 center, float length, glm::vec3 color) {
	glm::vec3 corner = center;

	std::vector<VertexFormat> vertices
	{
		VertexFormat(corner, color), // 0
		VertexFormat(corner + glm::vec3(-length, 0, 0), color), // 3
		VertexFormat(corner + glm::vec3(-length, -length, 0), color), // 4
	};

	std::vector<unsigned short> indices
	{ 0, 1, 2,
	};

	Mesh* star = new Mesh(name);
	star->SetDrawMode(GL_TRIANGLE_FAN);

	star->InitFromData(vertices, indices);
	return star;
}

Mesh* Object2D::CreateStar3(std::string name, glm::vec3 center, float length, glm::vec3 color) {
	glm::vec3 corner = center;

	std::vector<VertexFormat> vertices
	{
		VertexFormat(corner, color), // 0
		VertexFormat(corner + glm::vec3(0, -length, 0), color), // 5
		VertexFormat(corner + glm::vec3(length, -length, 0), color), // 6
	};

	std::vector<unsigned short> indices
	{ 0, 1, 2,
	};

	Mesh* star = new Mesh(name);
	star->SetDrawMode(GL_TRIANGLE_FAN);

	star->InitFromData(vertices, indices);
	return star;
}

Mesh* Object2D::CreateStar4(std::string name, glm::vec3 center, float length, glm::vec3 color) {
	glm::vec3 corner = center;

	std::vector<VertexFormat> vertices
	{
		VertexFormat(corner, color), // 0
		VertexFormat(corner + glm::vec3(length, 0, 0), color), // 7 
		VertexFormat(corner + glm::vec3(length, length, 0), color) // 8
	};

	std::vector<unsigned short> indices
	{ 0, 1, 2,
	};

	Mesh* star = new Mesh(name);
	star->SetDrawMode(GL_TRIANGLE_FAN);

	star->InitFromData(vertices, indices);
	return star;
}

Mesh* Object2D::CreateArrowHead(std::string name, glm::vec3 leftCorner, float length, glm::vec3 color) {
	glm::vec3 corner = leftCorner;
	std::vector<VertexFormat> vertices
	{
	VertexFormat(corner + glm::vec3(-length * 0.1, length / 10, 0), color), // 2
	VertexFormat(corner + glm::vec3(0, 0, 0), color), // 3
	VertexFormat(corner + glm::vec3(-length * 0.1, -length / 10, 0), color), // 4
	};
	std::vector<unsigned short> indices;
	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);
	Mesh* arrowHead = new Mesh(name);
	arrowHead->SetDrawMode(GL_TRIANGLE_FAN);
	arrowHead->InitFromData(vertices, indices);
	return arrowHead;
}

Mesh* Object2D::CreateArrowBody(std::string name, glm::vec3 leftCorner, float length, glm::vec3 color) {
	glm::vec3 corner = leftCorner;
	std::vector<VertexFormat> vertices
	{
		VertexFormat(corner, color), // 0
		VertexFormat(corner + glm::vec3(-length * 1.1, 0, 0), color), // 1
	};
	std::vector<unsigned short> indices;
	indices.push_back(0);
	indices.push_back(1);
	Mesh* arrowHead = new Mesh(name);
	arrowHead->SetDrawMode(GL_LINE_STRIP);
	arrowHead->InitFromData(vertices, indices);
	return arrowHead;
}

Mesh* Object2D::CreateBow(std::string name, glm::vec3 center, float length, glm::vec3 color) {

	std::vector<VertexFormat> vertices; //varfurile triunghiurilor
	std::vector<unsigned short> indices; //indicii
	float u = (GLfloat)M_PI / 2; //unghiul curent

	vertices.push_back(VertexFormat(center, color)); //centrul ceruclui
	Mesh* bow = new Mesh(name);
	bow->SetDrawMode(GL_LINE_LOOP);
	for (int i = 0; i <= 41; i++) {
		vertices.push_back(VertexFormat(center + glm::vec3(length *  cos(u), length * sin(u), 0), color)); //pun punctul in functie de sin si cos de unghi
		indices.push_back(i); //pun indicele
		u = u - (GLfloat)M_PI / 40; //trec la urmatoarea perioada
	}

	bow->InitFromData(vertices, indices);
	return bow;
}

Mesh* Object2D::CreateBowHitbox(std::string name, glm::vec3 corner, float length, glm::vec3 color) {

	
	std::vector<unsigned short> indices; //indicii
	float u = (GLfloat)M_PI / 2; //unghiul curent
	std::vector<VertexFormat> vertices= //varfurile triunghiurilor
	{
		VertexFormat(corner + glm::vec3(0, -length, 0), glm::vec3(0, 0, 1)), // 2
			VertexFormat(corner + glm::vec3(length * 0.75, -length, 0), glm::vec3(0, 0, 1)), // 3
			VertexFormat(corner + glm::vec3(length * 0.75, length, 0), glm::vec3(0, 0, 1)), // 4
			VertexFormat(corner + glm::vec3(0, length, 0), glm::vec3(0, 0, 1)), // 4
	};

	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);
	indices.push_back(3);

	Mesh* baloonBody = new Mesh(name);
	baloonBody->SetDrawMode(GL_LINE_LOOP);

	baloonBody->InitFromData(vertices, indices);
	return baloonBody;
}

Mesh* Object2D::CreateStarHitbox(std::string name, glm::vec3 corner, float length, glm::vec3 color) {


	std::vector<unsigned short> indices; //indicii
	float u = (GLfloat)M_PI / 2; //unghiul curent
	std::vector<VertexFormat> vertices = //varfurile triunghiurilor
	{
			VertexFormat(corner + glm::vec3(-length, -length, 0), glm::vec3(0, 0, 1)), // 2
			VertexFormat(corner + glm::vec3(length, -length, 0), glm::vec3(0, 0, 1)), // 3
			VertexFormat(corner + glm::vec3(length, length, 0), glm::vec3(0, 0, 1)), // 4
			VertexFormat(corner + glm::vec3(-length, length, 0), glm::vec3(0, 0, 1)), // 4
	};

	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);
	indices.push_back(3);

	Mesh* baloonBody = new Mesh(name);
	baloonBody->SetDrawMode(GL_LINE_LOOP);

	baloonBody->InitFromData(vertices, indices);
	return baloonBody;
}

Mesh* Object2D::CreateBaloonBody(std::string name, glm::vec3 center, float length, glm::vec3 color) {
	std::vector<VertexFormat> vertices; //varfurile triunghiurilor
	std::vector<unsigned short> indices; //indicii
	float u = 0; //unghiul curent

	//vertices.push_back(VertexFormat(leftBottomCorner, glm::vec3(1.0f, 0, 0))); //centrul ceruclui
	Mesh* baloonBody = new Mesh(name);
	baloonBody->SetDrawMode(GL_TRIANGLE_FAN);
	for (int i = 0; i <= 40; i++) {
		vertices.push_back(VertexFormat(center + glm::vec3(length * 0.5 * cos(u), length * 0.75 * sin(u), 0), color)); //pun punctul in functie de sin si cos de unghi
		indices.push_back(i); //pun indicele
		u = u + (GLfloat)M_PI * 2 / 40; //trec la urmatoarea perioada
	}

	baloonBody->InitFromData(vertices, indices);
	return baloonBody;
}

Mesh* Object2D::CreateBaloonHitbox(std::string name, glm::vec3 corner, float length, glm::vec3 color) {
	std::vector<VertexFormat> vertices = //varfurile triunghiurilor
	{
		VertexFormat(corner + glm::vec3(-length * 0.5, -length * 0.75, 0) , glm::vec3(0,0,1)), // 2
		VertexFormat(corner + glm::vec3(length * 0.5, 0.75 * length, 0), glm::vec3(0,0,1)), // 3
		VertexFormat(corner + glm::vec3(length * 0.5, -0.75 * length, 0), glm::vec3(0,0,1)), // 4
		VertexFormat(corner + glm::vec3(-length * 0.5, 0.75 * length, 0), glm::vec3(0,0,1)), // 4
	};
	std::vector<unsigned short> indices; //indicii
	indices.push_back(0);
	indices.push_back(3);
	indices.push_back(1);
	indices.push_back(2);

	Mesh* baloonBody = new Mesh(name);
	baloonBody->SetDrawMode(GL_LINE_LOOP);

	baloonBody->InitFromData(vertices, indices);
	return baloonBody;
}

Mesh* Object2D::CreateBaloonTriangle(std::string name, glm::vec3 center, float length, glm::vec3 color) {
	std::vector<VertexFormat> vertices = //varfurile triunghiurilor
	{
		VertexFormat(center + glm::vec3(0, -length * 0.75, 0) , color), // 2
		VertexFormat(center + glm::vec3(-length * 0.15, -length, 0), color), // 3
		VertexFormat(center + glm::vec3(length * 0.15, -length, 0), color), // 4
	};
	std::vector<unsigned short> indices; //indicii
	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);


	//vertices.push_back(VertexFormat(leftBottomCorner, glm::vec3(1.0f, 0, 0))); //centrul ceruclui
	Mesh* baloonBody = new Mesh(name);
	baloonBody->SetDrawMode(GL_TRIANGLE_FAN);

	baloonBody->InitFromData(vertices, indices);
	return baloonBody;
}

Mesh* Object2D::CreateBaloonString(std::string name, glm::vec3 center, float length, glm::vec3 color) {
	std::vector<VertexFormat> vertices = //varfurile triunghiurilor
	{
		VertexFormat(center + glm::vec3(0, -length, 0) , color), // 2
		VertexFormat(center + glm::vec3(-length * 0.15, -length * 1.1, 0), color), // 3
		VertexFormat(center + glm::vec3(length * 0.05, -length * 1.2, 0), color), // 4
		VertexFormat(center + glm::vec3(-length * 0.1, -length * 1.3, 0), color), // 4
	};
	std::vector<unsigned short> indices; //indicii
	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);
	indices.push_back(3);


	//vertices.push_back(VertexFormat(leftBottomCorner, glm::vec3(1.0f, 0, 0))); //centrul ceruclui
	Mesh* baloonBody = new Mesh(name);
	baloonBody->SetDrawMode(GL_LINE_STRIP);

	baloonBody->InitFromData(vertices, indices);
	return baloonBody;
}
