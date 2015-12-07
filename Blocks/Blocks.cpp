// Blocks.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <SFML/Graphics.hpp>
#include "item.h"
#include "Config.h"
#include <random>

int random_number(int number) {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dist(0, 32700); // 
	int random_number = dist(gen) % number;
	return random_number + 1;
}

void update(RenderWindow & window, Group & group) {
	if (group.location == 1) {
		group.direction = random_number(4);
		group.efect_number = random_number(3);
		group.location++;
	}
	if (group.location == 2) {
		switch (group.direction)
		{
		case 1: group.left(); break;
		case 2: group.right(); break;
		case 3: group.up(); break;
		case 4: group.down(); break;
		}
		switch (group.efect_number)
		{
		case 1: group.efect_color(); break;
		case 2: group.efect_rotate(); break;
		case 3: group.efect_size(); break;
		}

	}
	if (group.location == 3) {

		switch (group.efect_number)
		{
		case 1: group.efect_color(); break;
		case 2: group.efect_rotate(); break;
		case 3: group.efect_size(); break;
		}

		switch (group.direction)
		{
		case 1: group.back_from_left(); break;
		case 2: group.back_from_right(); break;
		case 3: group.back_from_up(); break;
		case 4: group.back_from_down(); break;
		}
	}
}

void start() {
	ContextSettings settings;
	settings.antialiasingLevel = 8;
	RenderWindow window(VideoMode(800, 800), "Blocks", sf::Style::Default, settings);
	Group *group = new Group;
	initiation(*group);

	while (window.isOpen()) {
		update(window, *group);
		draw(*group, window);
	}
	delete_blocks(*group);
	delete group;

}

int main() {
start();

}

