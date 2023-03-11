#pragma once

#include <SFML/Graphics.hpp>
using namespace sf;

class Meteor
{
public:
	Meteor();   // конструктор
	~Meteor();  // Деструктор

void move(float& time);                   // перемещение объекта
void draw(RenderWindow& window);          // отрисовка объекта
void animation();                         // анимация объекта
bool Collision(FloatRect object);         // столкновение с объектом

virtual void restart();                   // установка новых координат
const FloatRect getMeteorBounds();        // положение в глобальных координатах
bool newborn;                             // новый объект

Vector2f getPosBonus()                // Возвращает последнии координаты
{
	return PosBonus;
}

protected:

Sprite SpaceObject;                   // объект
Texture TextureObject;                // текстура объекта
Vector2f PosBonus;                    // последнии координаты

private:

	
	int xsp[5]{ 3,73,135,198,262 };       // координаты текстуры по x
	int ysp[6]{ 3,68,132,200,265,325 };   // координаты текстуры по y
	int ix=0, iy=0;                       // смещение по x и по y
	int st = 0;							  // шаг анимации
		                          
};

