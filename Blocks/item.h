#pragma once
#include "SFML\Graphics.hpp"
#include <vector>

using namespace sf;

struct Block {
	Block(Vector2f position, Vector2f size, float block_speed);
	RectangleShape * block;
	Vector2f block_size;
	Vector2f block_position;
	Vector2f start_position;
	float speed;

	float block_color;
	float color_iter;

	float size_iter;

	float block_rotate;

};

struct Group {
	std::vector<Block> blocks;

	int location = 1;

	int efect_number;
	int direction;

	void left();
	void right();
	void up();
	void down();

	void back_from_left();
	void back_from_right();
	void back_from_up();
	void back_from_down();

	void efect_color();
	void efect_size();
	void efect_rotate();

};

void initiation(Group & group);

void draw(Group & group, RenderWindow & window);

void delete_blocks(Group & group);