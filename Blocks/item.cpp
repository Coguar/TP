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
	for (Block &thing : blocks) {
		if (thing.block->getPosition().x < RIGHT_POS) {
			thing.block->move(thing.speed, 0);
		}
	}
	if (int(blocks[0].block->getPosition().x) == int(blocks[5].block->getPosition().x)) {
		location = 3;
	}
}

void Group::left() {
	for (Block &thing : blocks) {
		if (thing.block->getPosition().x > LEFT_POS) {
			thing.block->move(-thing.speed, 0);
		}
	}
	if (int(blocks[0].block->getPosition().x) == int(blocks[5].block->getPosition().x)) {
		location = 3;
	}
}

void Group::up() {
	for (Block &thing : blocks) {
		if (thing.block->getPosition().y > UP_POS) {
			thing.block->move(0, -thing.speed);
		}
	}
	if (int(blocks[0].block->getPosition().y) == int(blocks[5].block->getPosition().y)) {
		location = 3;
	}
}

void Group::down() {
	for (Block &thing : blocks) {
		if (thing.block->getPosition().y < DOWN_POS) {
			thing.block->move(0, thing.speed);
		}
	}
	if (int(blocks[0].block->getPosition().y) == int(blocks[5].block->getPosition().y)) {
		location = 3;
	}
}
//Back
void Group::back_from_right() {
	for (Block &thing : blocks) {
		if (thing.block->getPosition().x > thing.block_position.x) {
			thing.block->move(-thing.speed, 0);
		}
	}
	if (int(blocks[0].block->getPosition().x) == int(blocks[0].block_position.x) && int(blocks[5].block->getPosition().x) == int(blocks[5].block_position.x)) {
		location = 1;
	}
}

void Group::back_from_left() {
	for (Block &thing : blocks) {
		if (thing.block->getPosition().x < thing.block_position.x) {
			thing.block->move(thing.speed, 0);
		}
	}
	if (int(blocks[0].block->getPosition().x) == int(blocks[0].block_position.x) &&  int(blocks[5].block->getPosition().x) == int(blocks[5].block_position.x)) {
		location = 1;
	}
}

void Group::back_from_up() {
	for (Block &thing : blocks) {
		if (thing.block->getPosition().y < thing.block_position.y) {
			thing.block->move(0, +thing.speed);
		}
	}
	if (int(blocks[0].block->getPosition().y) == int(blocks[0].block_position.y) &&  int(blocks[5].block->getPosition().y) == int(blocks[5].block_position.y)) {
		location = 1;
	}
}

void Group::back_from_down() {
	for (Block &thing : blocks) {
		if (thing.block->getPosition().y > thing.block_position.y) {
			thing.block->move(0, -thing.speed);
		}
	}
	if (int(blocks[0].block->getPosition().y) == int(blocks[0].block_position.y) &&  int(blocks[5].block->getPosition().y) == int(blocks[5].block_position.y)) {
		location = 1;
	}
}
//effects


void Group::effect_color() {
	for (Block &thing : blocks) {
		thing.block->setFillColor(Color(0, int(thing.block_color), 0));

		thing.block_color += thing.color_iter;
		if (thing.block_color <= 150 || thing.block_color >= 250) {
			thing.color_iter = thing.color_iter * (-1);
		}
	}
}

void Group::effect_size() {
	for (Block &thing : blocks){
		thing.block->setSize(thing.block_size);
	
		thing.block_size.x += thing.size_iter;
		thing.block_size.y += thing.size_iter;

		if (thing.block_size.x >= MAX_SIZE.x || thing.block_size.y >= MAX_SIZE.y || thing.block_size.x <= MIN_SIZE.x || thing.block_size.y <= MIN_SIZE.y) {
			thing.size_iter = thing.size_iter * (-1);
		}
	}

}

void Group::effect_rotate() {
	for (Block &thing : blocks)
		thing.block->setRotation(float(thing.block->getRotation() + 0.01));
}


void initiation(Group & group) {
	for (int i = 0; i < 6; ++i) {
		Block new_block({ START_POS.x - i * SIZE.x, START_POS.y - i * SIZE.y }, SIZE, SPEED);
		group.blocks.push_back(new_block);
	}
}


void draw(Group & group, RenderWindow & window) 
{
	for(Block &thing: group.blocks)
		window.draw(*thing.block);
	window.display();
	window.clear(Color::White);
}


void delete_blocks(Group & group) {
	for (Block &thing : group.blocks)
		delete &thing;
}