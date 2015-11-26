// Mayatnik.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "math.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <windows.h>
#define _USE_MATH_DEFINES
#include <cmath>

using namespace sf;
const double PI = 3.14159265;
// создаём пустую фигуру

ConvexShape stick(){
	sf::ConvexShape convex;
	convex.setPointCount(4);
	convex.setPoint(0, sf::Vector2f(0, 0));
	convex.setPoint(1, sf::Vector2f(-80, 120));
	convex.setPoint(2, sf::Vector2f(0, 10));
	convex.setPoint(3, sf::Vector2f(80, 120));
	convex.setFillColor(Color::Yellow);
	//convex.setRotation(10);
	return convex;
}

ConvexShape cargo() {
	sf::ConvexShape convex;
	convex.setPointCount(7);
	convex.setPoint(0, sf::Vector2f(0, 0));
	convex.setPoint(1, sf::Vector2f(-40, 0));
	convex.setPoint(2, sf::Vector2f(-10, -40));
	convex.setPoint(3, sf::Vector2f(-10, -250));
	convex.setPoint(1, sf::Vector2f(10, 250));
	convex.setPoint(2, sf::Vector2f(10, -40));
	convex.setPoint(3, sf::Vector2f(40, 0));
	convex.setFillColor(Color::Yellow);
	convex.setOrigin(0, -250);
	//convex.setRotation(10);
	return convex;
}

ConvexShape shark(float x_pos, float y_pos, int tooth, int rad_b, int rad_s) {
	sf::ConvexShape convex;
	float x1 = 0;
	float y1 = 0;
	float x, y;
	float grad = 360 / (tooth * 3);
	int point_num = tooth * 3;
	bool big = true;
	int flag = 0;
	float grad_pos = 0;
	convex.setPointCount(point_num);
	convex.setPosition(x_pos, y_pos);
	convex.setFillColor(Color::Green);
	int i = 0;
	for (i; i < point_num;)
	{

		if (big) {
			x = x1 + (rad_b * cos((grad_pos - grad) * PI / 180));
			y = y1 + rad_b * sin((grad_pos - grad) * PI / 180);
			flag++;
			if (flag == 1) {
				big = false;
				flag = 0;
			}
		}
		else {
			x = x1 + (rad_s * cos(grad_pos * PI / 180));
			y = y1 + rad_s * sin(grad_pos * PI / 180);
			flag++;
			if (flag == 2) {
				big = true;
				flag = 0;
			}
		}
		if (i == 0) {

		}
		std::cout << x << std::endl;
		std::cout << y << std::endl;
		convex.setPoint(i, sf::Vector2f(x, y));
		grad_pos = grad_pos + grad;
		i++;
	}
	return convex;
}

ConvexShape gear(float x_pos, float y_pos, int tooth, int rad_b, int rad_s) {
	sf::ConvexShape convex;
	float x1 = 0;
	float y1 = 0;
	float x, y;
	float grad = 360 / (tooth * 4);
	int point_num = tooth * 4;
	bool big = true;
	int flag = 0;
	float grad_pos = 0;
	convex.setPointCount(point_num);
	convex.setPosition(x_pos, y_pos);
	convex.setFillColor(Color::Blue);
	int i = 0;
	for (i; i < point_num;)
	{

		if (big) {
			x = x1 + (rad_b * cos(grad_pos * PI / 180));
			y = y1 + rad_b * sin(grad_pos * PI / 180);
			flag++;
			if (flag == 2) {
				big = false;
				flag = 0;
			}
		}
		else {
			x = x1 + (rad_s * cos(grad_pos * PI / 180));
			y = y1 + rad_s * sin(grad_pos * PI / 180);
			flag++;
			if (flag == 2) {
				big = true;
				flag = 0;
			}
		}
		if (i == 0) {

		}
		std::cout << x << std::endl;
		std::cout << y << std::endl;
		convex.setPoint(i, sf::Vector2f(x, y));
		grad_pos = grad_pos + grad;
		i++;
	}
	return convex;
}

struct Pendulum {
	ConvexShape gear1 = ConvexShape();
	ConvexShape gear2 = ConvexShape();
	ConvexShape shark1 = ConvexShape();
	ConvexShape bob = ConvexShape();
	RectangleShape rope = RectangleShape();
	CircleShape plumb = CircleShape();

};

void PendulumInit(Pendulum & pendulum) {
	pendulum.rope = RectangleShape(sf::Vector2f(10, 300));
	pendulum.rope.setOrigin(5, 0);
	pendulum.rope.setPosition(490, 110);
	pendulum.rope.setFillColor(Color::Yellow);

	pendulum.plumb = CircleShape(30);
	pendulum.plumb.setOrigin(30, -300);
	pendulum.plumb.setPosition(490, 100);
	pendulum.plumb.setFillColor(Color::Yellow);

	pendulum.gear1 = gear(360, 295, 45, 100, 75);

	pendulum.shark1 = shark(490, 300, 15, 100, 80);
	pendulum.shark1.setFillColor(Color::Red);

	pendulum.gear2 = gear(490, 300, 15, 40, 20);

	pendulum.bob = stick();
	pendulum.bob.setPosition(490, 100);
}

void DrawPendulum(Pendulum & pendulum, RenderWindow & window) {
	window.clear();
	window.draw(pendulum.plumb);
	window.draw(pendulum.rope);
	window.draw(pendulum.shark1);
	window.draw(pendulum.bob);
	window.draw(pendulum.gear1);
	window.draw(pendulum.gear2);
	window.display();
}

void GoPendulumGo(Pendulum & pendulum, RenderWindow & window) {
	Clock clock;
	Clock clock1;
	int dir = 1;
	int pos;
	float speed_val;
	while (window.isOpen()) {
		float time = clock.getElapsedTime().asMilliseconds();
		float time1 = clock1.getElapsedTime().asMilliseconds();
		time = time / 100;
		time1 = time1 / 100;
		pendulum.gear1.setRotation(-time * 0.338);
		if (pendulum.gear1.getGlobalBounds().intersects(pendulum.shark1.getGlobalBounds())) {
			pendulum.shark1.setRotation(time);
			pendulum.gear2.setRotation(time);
		}
		if (time1 >= 18) {
			dir = dir * (-1);
			clock1.restart();
		}

		if (dir < 0) {
			pos = 10;
		}
		else {
			pos = 350;
		}

		speed_val = 1 - ((time1 / 10) - 1);
		if (time >= 9) {
			speed_val = speed_val + 1.0;
		}


		pendulum.bob.setRotation(pos + time1 * dir * 0.94 * speed_val);
		pendulum.plumb.setRotation(pos + time1 * dir * 0.94 * speed_val);
		pendulum.rope.setRotation(pos + time1 * dir * 0.94 * speed_val);

		if (time >= 360) {
			clock.restart();
		}
		DrawPendulum(pendulum, window);
	}
}


int main()
{
	RenderWindow window(VideoMode(800, 600), "MyPendulum");
	Pendulum pendulum;
	PendulumInit(pendulum);
	
	GoPendulumGo(pendulum, window);

    return 0;
}

