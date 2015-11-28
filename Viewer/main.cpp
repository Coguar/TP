// ConsoleApplication1.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <SFML/Graphics.hpp>
#include <string>
#include <windows.h>
#include <iostream>
#include <sstream>
#include "main.h"
#ifdef _DEBUG
#include <vld.h>
#endif
#ifdef _DEBUG
#include <crtdbg.h>
#define _CRTDBG_MAP_ALLOC
#endif


using namespace sf;
using namespace std;




View view;

void TestVld()
{
	for (size_t i = 0; i < 10; i++)
	{
		int *p = new int[1UL << i];
	}
	Sleep(30000);
}

bool is_picture(string name) {

	if (!strrchr(name.c_str(), '.')) {
		return false;
	}
	string ext_array[] = { "jpg","jpeg","png","gif","bmp" };
	int i;
	for (i = 0; i < 5; i++)
		if (name.substr(name.find_last_of(".") + 1) == ext_array[i])
			return true;
	return false;
}

Files get_file_list(string & old_path) {
	string path = old_path + string("*");
	Files files;
	files.path = old_path;
	unsigned long i = 0;

	WIN32_FIND_DATA file_data;
	HANDLE h_file = FindFirstFile(path.c_str(), &file_data);//поиск первого файла
	if (h_file != INVALID_HANDLE_VALUE) {//Если файл существует
		do {
			if (!(file_data.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)) {//если файл не папка
				files.size++;
			}

		} while (FindNextFile(h_file, &file_data));

		files.files = new string[files.size];
		i = 0;
		h_file = FindFirstFile(path.c_str(), &file_data);
		do {
			if (!(file_data.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)) {
				if (is_picture(file_data.cFileName)) {
					files.files[i] = file_data.cFileName;
					i++;
				}
			}

		} while (FindNextFile(h_file, &file_data));
		FindClose(h_file);
	}
	files.size = i;
	return files;
	//delete[] files;
}

void border_view(View & view, Picture *pic, float scale, float value_zoom) {
	if (pic->texture->getSize().x * scale * value_zoom  > view.getSize().x) {
		if (view.getCenter().x  + view.getSize().x / 2  > pic->texture->getSize().x / 2 * scale * value_zoom + pic->sprite->getPosition().x) {
			view.setCenter(pic->texture->getSize().x / 2 * scale * value_zoom + pic->sprite->getPosition().x - view.getSize().x / 2, view.getCenter().y);
	}
		else if (view.getCenter().x - view.getSize().x / 2 < pic->sprite->getPosition().x - pic->texture->getSize().x / 2 * scale * value_zoom) {
			view.setCenter(pic->sprite->getPosition().x - pic->texture->getSize().x / 2 * scale * value_zoom + view.getSize().x / 2, view.getCenter().y);
		}
	}
	if (pic->texture->getSize().y * scale * value_zoom  > view.getSize().y * 0.9) {
		if (view.getCenter().y + view.getSize().y / 2  > pic->texture->getSize().y / 2 * scale * value_zoom + pic->sprite->getPosition().y + 0.1 * view.getSize().y) {
			view.setCenter(view.getCenter().x, pic->texture->getSize().y / 2 * scale * value_zoom + pic->sprite->getPosition().y - view.getSize().y / 2 + 0.1 * view.getSize().y);
		}
		else if (view.getCenter().y - view.getSize().y / 2 < pic->sprite->getPosition().y - pic->texture->getSize().y / 2 * scale * value_zoom) {
			view.setCenter(view.getCenter().x, pic->sprite->getPosition().y - pic->texture->getSize().y / 2 * scale * value_zoom + view.getSize().y / 2);
		}
	}
	
}


void initiation(Vector2u window_size, Files files, Picture *pic) {
	Image *image = new Image();
	string path = files.path + files.files[pic->number];
	image->loadFromFile(files.path + files.files[pic->number]);
	pic->texture = new Texture();
	pic->texture->loadFromImage(*image);
	delete(image);
	pic->sprite = new Sprite();
	pic->sprite->setPosition(float(window_size.x) / 2.0, (float(window_size.y) * 0.75) / 2.0);
	//cout << float(window_size.x) / 2.0 << endl << (float(window_size.y) * 0.75) / 2.0 << endl;
	pic->sprite->setTexture(*(pic->texture));
	pic->sprite->setOrigin(pic->texture->getSize().x / 2.0, pic->texture->getSize().y / 2.0);
	pic->name = string(files.files[pic->number]);

}

void ResizedEvent(Event & event, RenderWindow & window, Vector2u & window_size, Files & files, Picture & picture) {
	if (event.size.width < 300) {
		window_size.x = 300;
		window.setSize(window_size);
	}
	if (event.size.height < 300) {
		window_size.y = 300;
		window.setSize(window_size);
	}
	window_size = window.getSize();
	initiation(window_size, files, &picture);
	view.setSize(window_size.x, window_size.y);
	view.setCenter(float(window_size.x) / 2.0, float(window_size.y) / 2.0);
}

//Нужны структуры
/*void MousPressedEvent(Event & event, ) {
	if (event.key.code == Mouse::Left) {
		Vector2i localPosition = Mouse::getPosition(window);
		float x(mousePos.x), y(mousePos.y);
		if ((FloatRect(window.getSize().x / 2 - text_fon.getSize().x / 2, window.getSize().y - 1.5 * text_fon.getSize().y, text_left.getSize().x, text_left.getSize().y).intersects(FloatRect(x, y, 1, 1)))) {
			if (picture.number == 0) {
				picture.number = files.size;
			}
			picture.number--;
			initiation(window_size, files, &picture);
			view.setCenter(float(window_size.x) / 2.0, float(window_size.y) / 2.0);
			value_zoom = 1;
		}
		else if ((FloatRect(window.getSize().x / 2 + text_fon.getSize().x / 2 - text_right.getSize().x, window.getSize().y - 1.5 * text_fon.getSize().y, text_right.getSize().x, text_right.getSize().y).intersects(FloatRect(x, y, 1, 1)))) {
			if (picture.number + 1 == files.size) {
				picture.number = -1;
			}
			picture.number++;
			initiation(window_size, files, &picture);
			view.setCenter(float(window_size.x) / 2.0, float(window_size.y) / 2.0);
			value_zoom = 1;
		}
		else if ((FloatRect(window.getSize().x / 2 - 5 - text_zoom.getSize().x, window.getSize().y - 1.5 * text_fon.getSize().y, text_zoom.getSize().x, text_zoom.getSize().y).intersects(FloatRect(x, y, 1, 1)))) {
			if (value_zoom < 8) {
				value_zoom++;
			}
		}
		else if ((FloatRect(window.getSize().x / 2 + 5, window.getSize().y - 1.5 * text_fon.getSize().y, text_zoom.getSize().x, text_zoom.getSize().y).intersects(FloatRect(x, y, 1, 1)))) {
			if (value_zoom > 1) {
				value_zoom--;
			}

		}
		else {
			move = true;
			center_x = Mouse::getPosition(window).x;
			center_y = Mouse::getPosition(window).y;
		}
	}
}*/

int main() {


	float value_zoom = 1;
	float scale = 1;
	ContextSettings settings;
	settings.antialiasingLevel = 4;
	RenderWindow window(VideoMode(800, 600), "Viwer", Style::Default, settings); //, sf::Style::Close
	window.setVerticalSyncEnabled(true);
	Vector2u window_size = window.getSize();
	Clock clock;

	//view.setViewport(sf::FloatRect(0, 0, 1.0f, 0.75f));
	view.setSize(window_size.x, window_size.y);
	view.setCenter(float(window_size.x) / 2.0, float(window_size.y)  / 2.0);
	//Vector2u view_size = view.getSize();
	//путь
	string path = "C:\\lab4\\";
	cout << "Enter Path" << endl;
	cin >> path;

	//изображение
	Files files = get_file_list(path);//get_picture_list(get_list_of_files(path));
	Picture picture;
	picture.texture->loadFromFile("./files/transparent.png");
	initiation(window_size, files, &picture);

	//кнопки
	Texture text_left, text_right, text_zoom, text_unzoom, text_fon;
	text_left.loadFromFile("./files/left.png");
	text_right.loadFromFile("./files/right.png");
	text_zoom.loadFromFile("./files/zoom.png");
	text_unzoom.loadFromFile("./files/unzoom.png");
	text_fon.loadFromFile("./files/tool_fon.png");

	Sprite left, right, zoom, unzoom, fon;
	left.setTexture(text_left);
	right.setTexture(text_right);
	zoom.setTexture(text_zoom);
	unzoom.setTexture(text_unzoom);
	fon.setTexture(text_fon);

	Image icon;
	icon.loadFromFile("./files/ico.png");
	window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

	bool move = false;
	Vector2u size;

	float center_x ;
	float center_y ;

	while (window.isOpen())
	{

		float time = clock.getElapsedTime().asMicroseconds(); //дать прошедшее время в микросекундах
		clock.restart();
		time = time / 800;
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) {
				delete[] files.files;
				window.close();
			}

			if (event.type == sf::Event::Resized) {
				ResizedEvent(event, window, window_size, files, picture);
			}
			Vector2i mousePos = Mouse::getPosition(window);
			Vector2i localPosition = Mouse::getPosition(window);

			if (event.type == Event::MouseButtonPressed){//если нажата клавиша мыши
				if (event.key.code == Mouse::Left) {
					Vector2i localPosition = Mouse::getPosition(window);
					float x(mousePos.x), y(mousePos.y);
					if ((FloatRect(window.getSize().x / 2 - text_fon.getSize().x / 2, window.getSize().y - 1.5 * text_fon.getSize().y, text_left.getSize().x, text_left.getSize().y).intersects(FloatRect(x, y, 1, 1)))) {
						if (picture.number == 0) {
							picture.number = files.size;
						}
						picture.number--;
						initiation(window_size, files, &picture);
						view.setCenter(float(window_size.x) / 2.0, float(window_size.y) / 2.0);
						value_zoom = 1;
					}
					else if ((FloatRect(window.getSize().x / 2 + text_fon.getSize().x / 2 - text_right.getSize().x, window.getSize().y - 1.5 * text_fon.getSize().y, text_right.getSize().x, text_right.getSize().y).intersects(FloatRect(x, y, 1, 1)))) {
						if (picture.number + 1 == files.size) {
							picture.number = -1;
						}
						picture.number++;
						initiation(window_size, files, &picture);
						view.setCenter(float(window_size.x) / 2.0, float(window_size.y) / 2.0);
						value_zoom = 1;
					}
					else if ((FloatRect(window.getSize().x / 2 - 5 - text_zoom.getSize().x, window.getSize().y - 1.5 * text_fon.getSize().y, text_zoom.getSize().x, text_zoom.getSize().y).intersects(FloatRect(x, y, 1, 1)))) {
						if (value_zoom < 8) {
							value_zoom++;
						}
					}
					else if ((FloatRect(window.getSize().x / 2 + 5, window.getSize().y - 1.5 * text_fon.getSize().y, text_zoom.getSize().x, text_zoom.getSize().y).intersects(FloatRect(x, y, 1, 1)))) {
						if (value_zoom > 1) {
							value_zoom--;
						}

					}
					else {
						move = true;
						center_x = Mouse::getPosition(window).x;
						center_y = Mouse::getPosition(window).y;
					}
				}
		}
			if (event.type == Event::MouseButtonReleased)//если нажата клавиша мыши
				if (event.key.code == Mouse::Left) {
					move = false;
				}

			if (Keyboard::isKeyPressed(sf::Keyboard::Left)) {
				if (picture.number == 0) {
					picture.number = files.size;
				}
				picture.number--;
				initiation(window_size, files, &picture);
				view.setCenter(float(window_size.x) / 2.0, float(window_size.y) / 2.0);
				value_zoom = 1;
			}
			if (Keyboard::isKeyPressed(sf::Keyboard::Right)) {
				if (picture.number + 1 == files.size) {
					picture.number = -1;
				}
				picture.number++;
				initiation(window_size, files, &picture);
				view.setCenter(float(window_size.x) / 2.0, float(window_size.y) / 2.0);
				value_zoom = 1;
			}
		}
		if (value_zoom == 1) {
			view.setCenter(float(window_size.x) / 2.0, float(window_size.y) / 2.0);
		}
		//view.setSize(window_size.x, window_size.y);
		//view.setCenter(float(window_size.x) / 2.0, float(window_size.y) / 2.0);
		window.setTitle(picture.name + " - Viewer1.0");
		scale = 1;
		if (picture.texture->getSize().x > window_size.x) {
			scale = float(window_size.x) / picture.texture->getSize().x;
			picture.sprite->setScale(Vector2f(scale, scale));
		}
		if (picture.texture->getSize().y * scale > (0.75 * window_size.y)) {
			scale = (float(window_size.y) * 0.75) / picture.texture->getSize().y;
			picture.sprite->setScale(Vector2f(scale, scale));
		}
		picture.sprite->setScale(Vector2f(scale * value_zoom, scale * value_zoom));

		border_view(view, &picture, scale, value_zoom);

		if (move && value_zoom != 1 && (picture.texture->getSize().x * scale * value_zoom > view.getSize().x | picture.texture->getSize().y * scale * value_zoom > view.getSize().y * 0.9)) {
			if (picture.texture->getSize().x * scale * value_zoom > view.getSize().x && picture.texture->getSize().y * scale * value_zoom <= view.getSize().y * 0.9) {
				view.setCenter(view.getCenter().x - (Mouse::getPosition(window).x - center_x), float(window_size.y) / 2.0);
			}
			else if (picture.texture->getSize().x * scale * value_zoom <= view.getSize().x && picture.texture->getSize().y * scale * value_zoom > view.getSize().y * 0.9) {
				view.setCenter(float(window_size.x) / 2.0, view.getCenter().y - (Mouse::getPosition(window).y - center_y));
			}
			else {
				view.setCenter(view.getCenter().x - (Mouse::getPosition(window).x - center_x), view.getCenter().y - (Mouse::getPosition(window).y - center_y));
			}
			center_x = Mouse::getPosition(window).x;
			center_y = Mouse::getPosition(window).y;
		}


		left.setPosition(view.getCenter().x - text_fon.getSize().x / 2, view.getCenter().y + (view.getSize().y / 2) - 1.5 * text_fon.getSize().y);
		right.setPosition(view.getCenter().x + text_fon.getSize().x / 2 - text_right.getSize().x, view.getCenter().y + (view.getSize().y / 2) - 1.5 * text_fon.getSize().y);
		zoom.setPosition(view.getCenter().x - 5 - text_zoom.getSize().x, view.getCenter().y + (view.getSize().y / 2) - 1.5 * text_fon.getSize().y);
		unzoom.setPosition(view.getCenter().x + 5, view.getCenter().y + (view.getSize().y / 2) - 1.5 * text_fon.getSize().y);
		fon.setPosition(view.getCenter().x - text_fon.getSize().x / 2, view.getCenter().y + (view.getSize().y / 2) - 1.5 * text_fon.getSize().y);
		window.setView(view);
		window.draw((*picture.sprite));
		window.draw(fon);
		window.draw(left);
		window.draw(right);
		window.draw(zoom);
		window.draw(unzoom);
		window_size = window.getSize();
		window.display();
		window.clear(Color(240,240,240,200));
	}

}