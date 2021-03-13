#pragma once

#include <string>

#include <include/glm.h>
#include <Core/GPU/Mesh.h>

namespace Object2D
{

	// Create square with given bottom left corner, length and color
	Mesh* CreateRectangle(std::string name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill = false);
	Mesh* CreateStar1(std::string name, glm::vec3 leftBottomCorner, float length, glm::vec3 color);
	Mesh* CreateStar2(std::string name, glm::vec3 leftBottomCorner, float length, glm::vec3 color);
	Mesh* CreateStar3(std::string name, glm::vec3 leftBottomCorner, float length, glm::vec3 color);
	Mesh* CreateStar4(std::string name, glm::vec3 leftBottomCorner, float length, glm::vec3 color);
	Mesh* CreateBow(std::string name, glm::vec3 leftBottomCorner, float length, glm::vec3 color);
	Mesh* CreateArrowHead(std::string name, glm::vec3 leftBottomCorner, float length, glm::vec3 color);
	Mesh* CreateArrowBody(std::string name, glm::vec3 leftBottomCorner, float length, glm::vec3 color);
	Mesh* CreateBaloonBody(std::string name, glm::vec3 leftBottomCorner, float length, glm::vec3 color);
	Mesh* CreateBaloonTriangle(std::string name, glm::vec3 corner, float length, glm::vec3 color);
	Mesh* CreateBaloonString(std::string name, glm::vec3 corner, float length, glm::vec3 color);
	Mesh* CreateBow(std::string name, glm::vec3 leftBottomCorner, float length, glm::vec3 color);
	Mesh* CreateBaloonHitbox(std::string name, glm::vec3 corner, float length, glm::vec3 color);
	Mesh* CreateBowHitbox(std::string name, glm::vec3 corner, float length, glm::vec3 color);
	Mesh* CreateStarHitbox(std::string name, glm::vec3 corner, float length, glm::vec3 color);
}


