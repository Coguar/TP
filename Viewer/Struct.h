#pragma once
#include "stdafx.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <list>

struct Config
{
	std::string left = "./files/left.png";
	std::string right = "./files/right.png";
	std::string zoom = "./files/zoom.png";
	std::string unzoom = "./files/unzoom.png";
	std::string fon = "./files/tool_fon.png";
};

struct Viewer {
	sf::RenderWindow window;
	Vector2u window_size;
	Clock clock;
	View view;

	sf::Texture text_zoom;
	sf::Sprite s_zoom;

	sf::Texture text_unzoom;
	sf::Sprite s_unzoom;

	sf::Texture text_right;
	sf::Sprite s_right;

	sf::Texture text_left;
	sf::Sprite s_left;

	sf::Texture text_fon;
	sf::Sprite s_fon;

	float value_zoom = 1;
	float scale = 1;
	string path = "";

	bool move = false;
	Vector2u size;

	float center_x;
	float center_y;
};