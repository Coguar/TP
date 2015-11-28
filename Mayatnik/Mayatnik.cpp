// Mayatnik.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <windows.h>
#define _USE_MATH_DEFINES
#include <math.h>

using namespace sf;

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
	float grad = float(360 / (tooth * 3));
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
			x = x1 + float(rad_b * cos((grad_pos - grad) * M_PI / 180));
			y = y1 + float(rad_b * sin((grad_pos - grad) * M_PI / 180));
			flag++;
			if (flag == 1) {
				big = false;
				flag = 0;
			}
		}
		else {
			x = x1 + float(rad_s * cos(grad_pos * M_PI / 180));
			y = y1 + float(rad_s * sin(grad_pos * M_PI / 180));
			flag++;
			if (flag == 2) {
				big = true;
				flag = 0;
			}
		}
		if (i == 0) {

		}
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
	float grad = float(360 / (tooth * 4));
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
			x = x1 + float(rad_b * cos(grad_pos * M_PI / 180));
			y = y1 + float(rad_b * sin(grad_pos * M_PI / 180));
			flag++;
			if (flag == 2) {
				big = false;
				flag = 0;
			}
		}
		else {
			x = x1 + float(rad_s * cos(grad_pos * M_PI / 180));
			y = y1 + float(rad_s * sin(grad_pos * M_PI / 180));
			flag++;
			if (flag == 2) {
				big = true;
				flag = 0;
			}
		}
		if (i == 0) {

		}
		convex.setPoint(i, sf::Vector2f(x, y));
		grad_pos = grad_pos + grad;
		i++;
	}
	return convex;
}

struct Pendulum {
	ConvexShape gear1 = ConvexShape();
	int gear_tooth1 = 45;
	ConvexShape gear2 = ConvexShape();
	int gear_tooth2 = 15;
	ConvexShape shark1 = ConvexShape();
	int shark_tooth = 15;
	ConvexShape bob = ConvexShape();
	RectangleShape rope = RectangleShape();
	CircleShape plumb = CircleShape();

	float max_degree = 20;
	float present_degree = 0;
	float present_speed = 0;

};

void MechanismStep(Pendulum & pendulum, float & boost, bool & isMove) {
	if (isMove) {
		pendulum.gear1.setRotation(float(pendulum.gear1.getRotation() + (360 / pendulum.gear_tooth1) * 0.0001 ));
		pendulum.gear2.setRotation(float(pendulum.gear2.getRotation() - (360 / pendulum.gear_tooth2) * 0.0001));
		pendulum.shark1.setRotation(float(pendulum.shark1.getRotation() - (360 / pendulum.shark_tooth) * 0.0001));
	}
}

void Speed(Pendulum & pendulum, float & boost, float & time) {
	pendulum.present_speed = pendulum.present_speed + boost * time;
}

void BobStep(Pendulum & pendulum, float & boost, bool & isMove, Clock & clock, int & direction) {
	float time = float(clock.getElapsedTime().asMilliseconds());
	time = time / 10000;
	pendulum.present_degree = pendulum.present_degree + abs(time * pendulum.present_speed);
	if (pendulum.present_degree == pendulum.max_degree / 2) {
		boost = boost * (-1);
		clock.restart();
	}
	if (pendulum.present_degree >= pendulum.max_degree / 2) {
		boost = abs(boost) * (-1);
	}
	if (pendulum.present_degree >= pendulum.max_degree) {
		boost = boost * (-1);
		pendulum.present_degree = 0;
		clock.restart();
		direction = direction * (-1);
		if (direction < 0) {
			pendulum.bob.setRotation(10);
			pendulum.rope.setRotation(10);
			pendulum.plumb.setRotation(10);
		}
		else {
			pendulum.bob.setRotation(-10);
			pendulum.rope.setRotation(-10);
			pendulum.plumb.setRotation(-10);
		}
	}

	Speed(pendulum, boost, time);
	pendulum.bob.setRotation(pendulum.bob.getRotation() + direction * (time * pendulum.present_speed));
	pendulum.rope.setRotation(pendulum.bob.getRotation() + direction * (time * pendulum.present_speed));
	pendulum.plumb.setRotation(pendulum.bob.getRotation() + direction * (time * pendulum.present_speed));

	if (pendulum.present_speed < 0.1) {
		isMove = true;
	}
	else {
		isMove = false;
	}
}


void PendulumInit(Pendulum & pendulum) {
	pendulum.rope = RectangleShape(sf::Vector2f(10, 300));
	pendulum.rope.setOrigin(5, 0);
	pendulum.rope.setPosition(490, 110);
	pendulum.rope.setFillColor(Color::Yellow);
	pendulum.rope.setRotation(-10);

	pendulum.plumb = CircleShape(30);
	pendulum.plumb.setOrigin(30, -300);
	pendulum.plumb.setPosition(490, 100);
	pendulum.plumb.setFillColor(Color::Yellow);
	pendulum.plumb.setRotation(-10);

	pendulum.gear1 = gear(360, 293, pendulum.gear_tooth1, 100, 75);

	pendulum.shark1 = shark(490, 300, pendulum.shark_tooth, 100, 80);
	pendulum.shark1.setFillColor(Color::Red);

	pendulum.gear2 = gear(490, 300, pendulum.gear_tooth2, 40, 20);

	pendulum.bob = stick();
	pendulum.bob.setPosition(490, 100);
	pendulum.bob.setRotation(-10);
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

void Process(Pendulum & pendulum, RenderWindow & window) {
	int direction = 1;
	float boost = float(0.001);
	bool isMove = true;
	Clock clock;
	while (window.isOpen()) {
		//float time = clock.getElapsedTime().asMilliseconds;
		BobStep(pendulum, boost, isMove, clock, direction);
		MechanismStep(pendulum, boost, isMove);
		DrawPendulum(pendulum, window);
	}
}


int main()
{
	RenderWindow window(VideoMode(800, 600), "MyPendulum");
	Pendulum pendulum;
	PendulumInit(pendulum);
	Process(pendulum, window);

    return 0;
}

