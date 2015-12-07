#include "stdafx.h"
#include "item.h"
#include "Config.h"

Block::Block(Vector2f position, Vector2f size, float block_speed) {
	block_position = position;
	block_size = size;
	block = new RectangleShape(block_size);
	block_color = 240;
	color_iter = float(-0.01);
	block->setPosition(block_position);
	start_position = block_position;
	block->setFillColor(Color(0, int(block_color), 0));
	block->setOrigin(block_size.x / 2, block_size.y / 2);
	speed = block_speed;
	size_iter = float(0.01);
}

void Group::right() {
	for (int i = 0; i < int(blocks.size()); ++i) {
		if (blocks[i].block->getPosition().x < right_position) {
			blocks[i].block->move(blocks[i].speed, 0);
		}
	}
	if (int(blocks[0].block->getPosition().x) == int(blocks[5].block->getPosition().x)) {
		location = 3;
	}
}

void Group::left() {
	for (int i = 0; i < int(blocks.size()); ++i) {
		if (blocks[i].block->getPosition().x > left_position) {
			blocks[i].block->move(-blocks[i].speed, 0);
		}
	}
	if (int(blocks[0].block->getPosition().x) == int(blocks[5].block->getPosition().x)) {
		location = 3;
	}
}

void Group::up() {
	for (int i = 0; i < int(blocks.size()); ++i) {
		if (blocks[i].block->getPosition().y > up_position) {
			blocks[i].block->move(0, -blocks[i].speed);
		}
	}
	if (int(blocks[0].block->getPosition().y) == int(blocks[5].block->getPosition().y)) {
		location = 3;
	}
}

void Group::down() {
	for (int i = 0; i < int(blocks.size()); ++i) {
		if (blocks[i].block->getPosition().y < down_position) {
			blocks[i].block->move(0, blocks[i].speed);
		}
	}
	if (int(blocks[0].block->getPosition().y) == int(blocks[5].block->getPosition().y)) {
		location = 3;
	}
}
//Back
void Group::back_from_right() {
	for (int i = 0; i < int(blocks.size()); ++i) {
		if (blocks[i].block->getPosition().x > blocks[i].block_position.x) {
			blocks[i].block->move(-blocks[i].speed, 0);
		}
	}
	if (int(blocks[0].block->getPosition().x) == int(blocks[0].block_position.x) && int(blocks[5].block->getPosition().x) == int(blocks[5].block_position.x)) {
		location = 1;
	}
}

void Group::back_from_left() {
	for (int i = 0; i < int(blocks.size()); ++i) {
		if (blocks[i].block->getPosition().x < blocks[i].block_position.x) {
			blocks[i].block->move(blocks[i].speed, 0);
		}
	}
	if (int(blocks[0].block->getPosition().x) == int(blocks[0].block_position.x) &&  int(blocks[5].block->getPosition().x) == int(blocks[5].block_position.x)) {
		location = 1;
	}
}

void Group::back_from_up() {
	for (int i = 0; i < int(blocks.size()); ++i) {
		if (blocks[i].block->getPosition().y < blocks[i].block_position.y) {
			blocks[i].block->move(0, +blocks[i].speed);
		}
	}
	if (int(blocks[0].block->getPosition().y) == int(blocks[0].block_position.y) &&  int(blocks[5].block->getPosition().y) == int(blocks[5].block_position.y)) {
		location = 1;
	}
}

void Group::back_from_down() {
	for (int i = 0; i < int(blocks.size()); ++i) {
		if (blocks[i].block->getPosition().y > blocks[i].block_position.y) {
			blocks[i].block->move(0, -blocks[i].speed);
		}
	}
	if (int(blocks[0].block->getPosition().y) == int(blocks[0].block_position.y) &&  int(blocks[5].block->getPosition().y) == int(blocks[5].block_position.y)) {
		location = 1;
	}
}
//effects


void Group::efect_color() {
	for (int i = 0; i < int(blocks.size()); ++i) {
		blocks[i].block->setFillColor(Color(0, int(blocks[i].block_color), 0));

		blocks[i].block_color += blocks[i].color_iter;
		if (blocks[i].block_color <= 150 || blocks[i].block_color >= 250) {
			blocks[i].color_iter = blocks[i].color_iter * (-1);
		}
	}
}

void Group::efect_size() {
	for (int i = 0; i < int(blocks.size()); ++i) {
		blocks[i].block->setSize(blocks[i].block_size);

		blocks[i].block_size.x += blocks[i].size_iter;
		blocks[i].block_size.y += blocks[i].size_iter;

		if (blocks[i].block_size.x >= max_block_size.x || blocks[i].block_size.y >= max_block_size.y || blocks[i].block_size.x <= min_block_size.x || blocks[i].block_size.y <= min_block_size.y) {
			blocks[i].size_iter = blocks[i].size_iter * (-1);
		}
	}

}

void Group::efect_rotate() {
	for (int i = 0; i < int(blocks.size()); ++i) {
		blocks[i].block->setRotation(float(blocks[i].block->getRotation() + 0.01));
	}
}

void initiation(Group & group) {
	for (int i = 0; i < 6; ++i) {
		Block new_block({ start_position.x - i * 80, start_position.y - i * 60 }, block_size, block_speed);
		group.blocks.push_back(new_block);
	}
}

void draw(Group & group, RenderWindow & window) //121212
{
	for (int i = 0; i < int(group.blocks.size()); ++i) {
		window.draw(*group.blocks[i].block);
	}
	window.display();
	window.clear(Color::White);
}

void delete_blocks(Group & group) {
	for (int i = 0; i < int(group.blocks.size()); ++i) {
		delete group.blocks[i].block;
	}
}