#pragma once

using namespace sf;
using namespace std;


struct Files {
	string *files = new string[0];
	int size = 0;
	string path;
}typedef Files;

struct Picture {
	string name;
	Texture *texture = new Texture;
	float width;
	float height;
	Sprite *sprite = new Sprite;
	int number = 0;

}typedef Picture;
