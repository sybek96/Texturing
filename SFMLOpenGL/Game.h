#ifndef GAME_H
#define GAME_H

#include <Debug.h>

#include <iostream>
#include <gl/glew.h>
#include <gl/wglew.h>
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>

#include "Vector3.h"
#include "Matrix3.h"
#include<iostream>
#include<fstream>
#include <string>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>


class Game
{
public:
	Game(sf::ContextSettings);
	~Game();
	void run();
private:
	sf::Window window;
	bool isRunning = false;
	void initialize();
	void update();
	void render();
	void unload();
	std::string loadShaderFile(std::string type, std::string contents);
	//std::string loadFragmentShaderFile();
	void applyTransformation(const Matrix3&);
	void translate(float, Matrix3::Axis);
	Matrix3 m_scale;
	Matrix3 m_rotate;
	Matrix3 m_translate;
	std::string fileContentsVertex;
	std::string fileContentsFragment;
	sf::Clock clock;
	sf::Time elapsed;

	float rotationAngle = 0.0f;
};

#endif