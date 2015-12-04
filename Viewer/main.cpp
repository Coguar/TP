// ConsoleApplication1.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <SFML/Graphics.hpp>
#include <string>
#include <windows.h>
#include <iostream>
#include <sstream>
#include "main.h"

#include "Struct.h"

#ifdef _DEBUG
#include <vld.h>
#endif



using namespace sf;
using namespace std;





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
			view.setCenter(float(view.getCenter().x), float(pic->texture->getSize().y / 2 * scale * value_zoom + pic->sprite->getPosition().y - view.getSize().y / 2 + 0.1 * view.getSize().y));
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
	pic->sprite->setPosition(float(window_size.x / 2.0), float((window_size.y * 0.75) / 2.0));
	pic->sprite->setTexture(*(pic->texture));
	pic->sprite->setOrigin(float(pic->texture->getSize().x / 2.0), float(pic->texture->getSize().y / 2.0));
	pic->name = string(files.files[pic->number]);

}

void ResizedEvent(Event & event, RenderWindow & window, Vector2u & window_size, Files & files, Picture & picture, View & view) {
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
	view.setSize(float(window_size.x), float(window_size.y));
	view.setCenter(float(window_size.x / 2.0), float(window_size.y / 2.0));
}

void InitButton(Config & config, Viewer & viewer) {
	viewer.text_left.loadFromFile(config.left);
	viewer.text_right.loadFromFile(config.right);
	viewer.text_zoom.loadFromFile(config.zoom);
	viewer.text_unzoom.loadFromFile(config.unzoom);
	viewer.text_fon.loadFromFile(config.fon);

	viewer.s_left.setTexture(viewer.text_left);
	viewer.s_right.setTexture(viewer.text_right);
	viewer.s_zoom.setTexture(viewer.text_zoom);
	viewer.s_unzoom.setTexture(viewer.text_unzoom);
	viewer.s_fon.setTexture(viewer.text_fon);
}

void InitViewer(Viewer & viewer) {
	ContextSettings settings;
	settings.antialiasingLevel = 4;
	viewer.window.create(VideoMode(800, 600), "Viwer", Style::Default, settings);
	viewer.window.setVerticalSyncEnabled(true);
	viewer.window_size = viewer.window.getSize();

	viewer.view.setSize(float(viewer.window_size.x), float(viewer.window_size.y));
	viewer.view.setCenter(float(viewer.window_size.x / 2.0), float(viewer.window_size.y / 2.0));
	//путь
	viewer.path = "";
	cout << "Enter Path" << endl;
	cin >> viewer.path;

	Image icon;
	icon.loadFromFile("./files/ico.png");
	viewer.window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

	viewer.move = false;
}

void ScaleSet(Viewer & viewer, Picture * pic) {
	viewer.scale = 1;
	if (pic->texture->getSize().x > viewer.window_size.x) {
		viewer.scale = float(viewer.window_size.x) / pic->texture->getSize().x;
		pic->sprite->setScale(Vector2f(viewer.scale, viewer.scale));
	}
	if (pic->texture->getSize().y * viewer.scale > (0.75 * viewer.window_size.y)) {
		viewer.scale = float((viewer.window_size.y * 0.75) / pic->texture->getSize().y);
		pic->sprite->setScale(Vector2f(viewer.scale, viewer.scale));
	}
	pic->sprite->setScale(Vector2f(viewer.scale * viewer.value_zoom, viewer.scale * viewer.value_zoom));
}

void SetCentre(Viewer & viewer, Picture * pic) {
	if (viewer.move && viewer.value_zoom != 1 && (pic->texture->getSize().x * viewer.scale * viewer.value_zoom > viewer.view.getSize().x || pic->texture->getSize().y * viewer.scale * viewer.value_zoom > viewer.view.getSize().y * 0.9)) {
		if (pic->texture->getSize().x * viewer.scale * viewer.value_zoom > viewer.view.getSize().x && pic->texture->getSize().y * viewer.scale * viewer.value_zoom <= viewer.view.getSize().y * 0.9) {
			viewer.view.setCenter(float(viewer.view.getCenter().x - (Mouse::getPosition(viewer.window).x - viewer.center_x)), float(viewer.window_size.y / 2.0));
		}
		else if (pic->texture->getSize().x * viewer.scale * viewer.value_zoom <= viewer.view.getSize().x && pic->texture->getSize().y * viewer.scale * viewer.value_zoom > viewer.view.getSize().y * 0.9) {
			viewer.view.setCenter(float(viewer.window_size.x / 2.0), viewer.view.getCenter().y - (Mouse::getPosition(viewer.window).y - viewer.center_y));
		}
		else {
			viewer.view.setCenter(viewer.view.getCenter().x - (Mouse::getPosition(viewer.window).x - viewer.center_x), viewer.view.getCenter().y - (Mouse::getPosition(viewer.window).y - viewer.center_y));
		}
		viewer.center_x = float(Mouse::getPosition(viewer.window).x);
		viewer.center_y = float(Mouse::getPosition(viewer.window).y);
	}
}

void SetButtonPosition(Viewer & viewer) {
	viewer.s_left.setPosition(float(viewer.view.getCenter().x - viewer.text_fon.getSize().x / 2), float(viewer.view.getCenter().y + (viewer.view.getSize().y / 2) - 1.5 * viewer.text_fon.getSize().y));
	viewer.s_right.setPosition(float(viewer.view.getCenter().x + viewer.text_fon.getSize().x / 2 - viewer.text_right.getSize().x), float(viewer.view.getCenter().y + (viewer.view.getSize().y / 2) - 1.5 * viewer.text_fon.getSize().y));
	viewer.s_zoom.setPosition(float(viewer.view.getCenter().x - 5 - viewer.text_zoom.getSize().x), float(viewer.view.getCenter().y + (viewer.view.getSize().y / 2) - 1.5 * viewer.text_fon.getSize().y));
	viewer.s_unzoom.setPosition(float(viewer.view.getCenter().x + 5), float(viewer.view.getCenter().y + (viewer.view.getSize().y / 2) - 1.5 * viewer.text_fon.getSize().y));
	viewer.s_fon.setPosition(float(viewer.view.getCenter().x - viewer.text_fon.getSize().x / 2), float(viewer.view.getCenter().y + (viewer.view.getSize().y / 2) - 1.5 * viewer.text_fon.getSize().y));

}

void Draw(Viewer & viewer, Picture & picture) {
	viewer.window.setView(viewer.view);
	viewer.window.draw((*picture.sprite));
	viewer.window.draw(viewer.s_fon);
	viewer.window.draw(viewer.s_left);
	viewer.window.draw(viewer.s_right);
	viewer.window.draw(viewer.s_zoom);
	viewer.window.draw(viewer.s_unzoom);
	viewer.window_size = viewer.window.getSize();
	viewer.window.display();
	viewer.window.clear(Color(240, 240, 240, 200));
}

void Start(Viewer & viewer, Files & files, Picture & picture) {
	while (viewer.window.isOpen())
	{

		float time = float(viewer.clock.getElapsedTime().asMicroseconds()); //дать прошедшее время в микросекундах
		viewer.clock.restart();
		time = time / 800;
		sf::Event event;
		while (viewer.window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) {
				delete[] files.files;
				viewer.window.close();
			}

			if (event.type == sf::Event::Resized) {
				ResizedEvent(event, viewer.window, viewer.window_size, files, picture, viewer.view);
			}
			Vector2i mousePos = Mouse::getPosition(viewer.window);

			if (event.type == Event::MouseButtonPressed) {//если нажата клавиша мыши
				if (event.key.code == Mouse::Left) {
					Vector2i localPosition = Mouse::getPosition(viewer.window);
					float x(float(mousePos.x));
					float y(float(mousePos.y));
					if ((FloatRect(float(viewer.window.getSize().x / 2 - viewer.text_fon.getSize().x / 2), float(viewer.window.getSize().y - 1.5 * viewer.text_fon.getSize().y), float(viewer.text_left.getSize().x), float(viewer.text_left.getSize().y)).intersects(FloatRect(x, y, 1, 1)))) {
						if (picture.number == 0) {
							picture.number = files.size;
						}
						picture.number--;
						initiation(viewer.window_size, files, &picture);
						viewer.view.setCenter(float(viewer.window_size.x / 2.0), float(viewer.window_size.y / 2.0));
						viewer.value_zoom = 1;
					}
					else if ((FloatRect(float(viewer.window.getSize().x / 2 + viewer.text_fon.getSize().x / 2 - viewer.text_right.getSize().x), float(viewer.window.getSize().y - 1.5 * viewer.text_fon.getSize().y), float(viewer.text_right.getSize().x), float(viewer.text_right.getSize().y)).intersects(FloatRect(x, y, 1, 1)))) {
						if (picture.number + 1 == files.size) {
							picture.number = -1;
						}
						picture.number++;
						initiation(viewer.window_size, files, &picture);
						viewer.view.setCenter(float(viewer.window_size.x / 2.0), float(viewer.window_size.y / 2.0));
						viewer.value_zoom = 1;
					}
					else if ((FloatRect(float(viewer.window.getSize().x / 2 - 5 - viewer.text_zoom.getSize().x), float(viewer.window.getSize().y - 1.5 * viewer.text_fon.getSize().y), float(viewer.text_zoom.getSize().x), float(viewer.text_zoom.getSize().y)).intersects(FloatRect(x, y, 1, 1)))) {
						if (viewer.value_zoom < 8) {
							viewer.value_zoom++;
						}
					}
					else if ((FloatRect(float(viewer.window.getSize().x / 2 + 5), float(viewer.window.getSize().y - 1.5 * viewer.text_fon.getSize().y), float(viewer.text_zoom.getSize().x), float(viewer.text_zoom.getSize().y)).intersects(FloatRect(x, y, 1, 1)))) {
						if (viewer.value_zoom > 1) {
							viewer.value_zoom--;
						}

					}
					else {
						viewer.move = true;
						viewer.center_x = float(Mouse::getPosition(viewer.window).x);
						viewer.center_y = float(Mouse::getPosition(viewer.window).y);
					}
				}
			}
			if (event.type == Event::MouseButtonReleased)
				if (event.key.code == Mouse::Left) {
					viewer.move = false;
				}

			if (Keyboard::isKeyPressed(sf::Keyboard::Left)) {
				if (picture.number == 0) {
					picture.number = files.size;
				}
				picture.number--;
				initiation(viewer.window_size, files, &picture);
				viewer.view.setCenter(float(viewer.window_size.x / 2.0), float(viewer.window_size.y / 2.0));
				viewer.value_zoom = 1;
			}
			if (Keyboard::isKeyPressed(sf::Keyboard::Right)) {
				if (picture.number + 1 == files.size) {
					picture.number = -1;
				}
				picture.number++;
				initiation(viewer.window_size, files, &picture);
				viewer.view.setCenter(float(viewer.window_size.x / 2.0), float(viewer.window_size.y / 2.0));
				viewer.value_zoom = 1;
			}
		}
		if (viewer.value_zoom == 1) {
			viewer.view.setCenter(float(viewer.window_size.x / 2.0), float(viewer.window_size.y / 2.0));
		}

		viewer.window.setTitle(picture.name + " - Viewer1.0");

		ScaleSet(viewer, &picture);
		border_view(viewer.view, &picture, viewer.scale, viewer.value_zoom);
		SetCentre(viewer, &picture);
		SetButtonPosition(viewer);
		Draw(viewer, picture);
	}
}


int main() {
	Config config;
	Viewer viewer;

	InitButton(config, viewer);
	InitViewer(viewer);

	Files files = get_file_list(viewer.path);
	cout << files.size;
	Picture picture;
	picture.texture->loadFromFile("./files/transparent.png");
	initiation(viewer.window_size, files, &picture);

	Start(viewer, files, picture);
	}
