// Clock.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <SFML/Graphics.hpp>
#include "iostream"
#include <windows.h>

#define _USE_MATH_DEFINES
#include <math.h>

using namespace sf;

struct HourHands
{
	ConvexShape hour = ConvexShape();
	const float hour_height = 100;
	const float hour_width = 10;

	ConvexShape minute = ConvexShape();
	const float minute_height = 150;
	const float minute_width = 5;

	ConvexShape second = ConvexShape();
	const float second_height = 200;
	const float second_width = 3;

	const int count_hours = 12;
	const int count_minutes = 60;

};

Vector2f DentPosition(float x, float y, float  radius, float  degree) {
	float position_x = x + (radius * cos(degree * M_PI / 180));
	float position_y = y + (radius * sin(degree * M_PI / 180));
	return Vector2f(position_x, position_y);

}

struct ClockFace
{
	const float radius = 220;
	CircleShape dial = CircleShape(radius);
	RectangleShape quarter = RectangleShape(sf::Vector2f(20, 4));
	RectangleShape regular = RectangleShape(sf::Vector2f(10, 2));
	RectangleShape common = RectangleShape(sf::Vector2f(3, 1));
};

ConvexShape InitHand(float height, float width) {
	sf::ConvexShape convex;
	convex.setPointCount(4);
	convex.setPoint(0, sf::Vector2f(0, 0));
	convex.setPoint(1, sf::Vector2f(- width, 0));
	convex.setPoint(2, sf::Vector2f(0, -height));
	convex.setPoint(3, sf::Vector2f(width, 0));
	convex.setOrigin(0, 0);
	return convex;
}

void CreateFace(ClockFace & face, Window & window) {
	face.dial.setOutlineThickness(5);
	face.dial.setOutlineColor(Color::Black);
	face.dial.setOrigin(face.dial.getRadius() / 2, face.dial.getRadius() / 2);
	face.dial.setPosition(window.getSize().x / float(2), window.getSize().y / float(2));
	face.quarter.setFillColor(Color::Black);
	face.quarter.setOrigin(0, face.quarter.getSize().y / 2);
	face.regular.setFillColor(Color::Black);
	face.regular.setOrigin(0, face.regular.getSize().y / 2);
	face.common.setFillColor(Color::Black);
	face.common.setOrigin(0, face.common.getSize().y / 2);
}

void SetFace(ClockFace & face, RenderWindow & window) {
	int degree = 0;
	while (degree < 360) {
		if (degree % 90 == 0) {
			face.quarter.setPosition(DentPosition(face.dial.getPosition().x, face.dial.getPosition().y, face.radius, float(degree)));
			face.quarter.setRotation(float(degree));
			window.draw(face.quarter);
			degree++;
		}
		else if (degree % 30 == 0) {
			face.regular.setPosition(DentPosition(face.dial.getPosition().x, face.dial.getPosition().y, face.radius, float(degree)));
			face.regular.setRotation(float(degree));
			window.draw(face.regular);
			degree++;
		}
		else if (degree % 6 == 0) {
			face.common.setPosition(DentPosition(face.dial.getPosition().x, face.dial.getPosition().y, face.radius, float(degree)));
			face.common.setRotation(float(degree));
			window.draw(face.common);
			degree++;
		}
		else {
			degree++;
		}
	}
}

void CreateHand(HourHands & hand, RenderWindow & window) {

	hand.hour = InitHand(hand.hour_height, hand.hour_width);
	hand.hour.setPosition(float(window.getSize().x / 2), float(window.getSize().y / 2));
	hand.hour.setFillColor(Color::Black);
	hand.minute = InitHand(hand.minute_height, hand.minute_width);
	hand.minute.setPosition(float(window.getSize().x / 2), float(window.getSize().y / 2));
	hand.minute.setFillColor(Color::Black);
	hand.second = InitHand(hand.second_height, hand.second_width);
	hand.second.setPosition(float(window.getSize().x / 2), float(window.getSize().y / 2));
	hand.second.setFillColor(Color::Red);
	
}

void Process(SYSTEMTIME & sysTime, HourHands & hand){
	GetLocalTime(&sysTime);
	float rotate = float((sysTime.wHour % hand.count_hours) * (360 / hand.count_hours)  + sysTime.wMinute * (360 / (hand.count_hours * hand.count_minutes)) + sysTime.wSecond * (360 / (hand.count_hours * pow(hand.count_minutes, 2))));
	hand.hour.setRotation(rotate);
	rotate = float(sysTime.wMinute * (360 / hand.count_minutes) + sysTime.wSecond * (360 / pow(hand.count_minutes, 2)));
	hand.minute.setRotation(rotate);
	rotate = float(sysTime.wSecond * (360 / hand.count_minutes));
	hand.second.setRotation(rotate);
}

void DrawClock(HourHands & hand, RenderWindow & window, CircleShape & circle, CircleShape & circle1) {
	window.draw(circle);
	window.draw(hand.hour);
	window.draw(hand.minute);
	window.draw(circle1);
	window.draw(hand.second);
	window.display();
	window.clear(Color::White);
}

int main()
{
	SYSTEMTIME sysTime;
	ContextSettings settings;
	settings.antialiasingLevel = 8;
	RenderWindow window (VideoMode(600, 600), "Clock", sf::Style::Default, settings);

	Image icon;
	icon.loadFromFile("icon.png");
	window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

	HourHands hand;
	ClockFace face;

	CircleShape circle(10);
	CircleShape circle1(3);

	circle.setPosition(float(window.getSize().x / 2), float(window.getSize().y / 2));
	circle.setFillColor(Color::Black);
	circle.setOrigin(circle.getRadius(), circle.getRadius());

	circle1.setPosition(float(window.getSize().x / 2), float(window.getSize().y / 2));
	circle1.setFillColor(Color::Red);
	circle1.setOrigin(circle1.getRadius(), circle1.getRadius());

	CreateHand(hand, window);
	CreateFace(face, window);
	while (window.isOpen()) {
		Process(sysTime, hand);
		SetFace(face, window);
		DrawClock(hand, window, circle, circle1);
	}
    return 0;
}

