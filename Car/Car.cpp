// Car.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "SFML\Graphics.hpp"
#include <iostream>

#define _USE_MATH_DEFINES
#include <math.h>

using namespace sf;

struct Config
{
	std::string CarImage = "./priora.png";
	std::string WheelImage = "./wheel.png";

	const float WHEEL_RADIUS = 16;

	const Vector2f FRONT_WHEEL_POS = { 162, 62 };
	const Vector2f BACK_WHEEL_POS = { 43, 62 };
	const Vector2f ROAD_SIZE = {1600, 100};

	const IntRect WHEEL_RECT = { 0, 0, 32, 32 };

};

struct Car 
{
	Car(){}
	Texture Priora;
	Sprite SpritePriora;

	Texture Wheel;
	Sprite SpriteFrontWheel;
	Sprite SpriteBackWheel;

	float Radius;
	float CarSpeed;
	float Boost;

	RectangleShape Road;
};

void SetWheels(Car & car, Config & config) {
	car.SpriteFrontWheel.setPosition(car.SpritePriora.getPosition().x + config.FRONT_WHEEL_POS.x, car.SpritePriora.getPosition().y + config.FRONT_WHEEL_POS.y);
	car.SpriteBackWheel.setPosition(car.SpritePriora.getPosition().x + config.BACK_WHEEL_POS.x, car.SpritePriora.getPosition().y + config.BACK_WHEEL_POS.y);
}

void InitiationCar(RenderWindow & window, Car & car, Config & config) {
	car.Priora.loadFromFile(config.CarImage);
	car.SpritePriora.setTexture(car.Priora);
	car.SpritePriora.setPosition(10, float(window.getSize().y / 2));
	

	car.Wheel.loadFromFile(config.WheelImage);
	car.SpriteFrontWheel.setTexture(car.Wheel);
	car.SpriteBackWheel.setTexture(car.Wheel);

	car.SpriteFrontWheel.setTextureRect(config.WHEEL_RECT);
	car.SpriteBackWheel.setTextureRect(config.WHEEL_RECT);

	car.Radius = config.WHEEL_RADIUS;

	car.SpriteFrontWheel.setOrigin(car.Radius, car.Radius);
	car.SpriteBackWheel.setOrigin(car.Radius, car.Radius);

	SetWheels(car, config);

	car.CarSpeed = 0;
	car.Boost = 0.00001f;

	car.Road.setSize(config.ROAD_SIZE);
	car.Road.setPosition(0, car.SpriteBackWheel.getPosition().y + car.Radius);
	car.Road.setFillColor(Color::Black);
}

void Speed(Car & car) {
	car.CarSpeed += car.Boost;
}

float Rotate(float distantion, float radius) {
	float degree = float(float(180 * distantion) / (M_PI * radius));
	return degree;
}

void RotateWheels(Car & car, float distantion) {
	car.SpriteFrontWheel.rotate(Rotate(distantion, car.Radius));
	car.SpriteBackWheel.rotate(Rotate(distantion, car.Radius));
}

void Move(Car & car, Config & config) {
	Speed(car);
	car.SpritePriora.move(car.CarSpeed, 0);
	SetWheels(car, config);
	RotateWheels(car, car.CarSpeed);

}

void SetBoost(Car & car, RenderWindow & window) {
	if (car.SpritePriora.getPosition().x >= window.getSize().x / 2 - car.Priora.getSize().x && car.Boost > 0) {
		car.Boost = car.Boost * (-1);
	}
	else if ((car.SpritePriora.getPosition().x < window.getSize().x / 2 - car.Priora.getSize().x && car.Boost < 0)) {
		car.Boost = car.Boost * (-1);
	}
}

void Draw(RenderWindow & window, Car & car) {
	window.draw(car.SpriteFrontWheel);
	window.draw(car.SpriteBackWheel);
	window.draw(car.SpritePriora);
	window.draw(car.Road);
	window.display();
	window.clear(Color::White);
}

void Start() {
	ContextSettings settings;
	settings.antialiasingLevel = 8;
	RenderWindow window(VideoMode(600, 400), "Lab8.1", sf::Style::Default, settings);
	Car car;
	Config config;

	InitiationCar(window, car, config);
	Clock clock;
	while (window.isOpen())
	{
		Move(car, config);
		Draw(window, car);
		SetBoost(car, window);
	}
}

int main()
{
	Start();
    return 0;
}

