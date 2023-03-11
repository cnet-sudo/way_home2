#include "Meteor.h"

Meteor::Meteor()            // конструктор
{	
	TextureObject.loadFromFile("Image/asteroid.png");
	SpaceObject.setTexture(TextureObject);
	SpaceObject.setTextureRect(IntRect(262, 325, 55, 50));
	restart();
}

Meteor::~Meteor()           // деструктор
{

}

void Meteor::restart()   // установка новых координат
{
	newborn = true;
	float s = static_cast<float>(rand() % 20 + 10);      // масштаб метеорита
	float x = static_cast<float>(rand() % 1280 + 1280);  // координаты по х
	float y = static_cast<float>(rand() % 540  + 130);   // координаты по y
	SpaceObject.setPosition(Vector2f(x, y));
	SpaceObject.setScale(s/20, s/20);
	ix = rand() % 4;                                     // кадр по х
	iy= rand() % 5;                                      // кадр по y
	st = rand() % 2;	                                 // скорость смены кадров

	PosBonus = SpaceObject.getPosition();
}

const FloatRect Meteor::getMeteorBounds()
{
	return SpaceObject.getGlobalBounds();
}

void Meteor::move(float & time)
{
	newborn = false;
	SpaceObject.move( static_cast<float>(-0.5 * time),0);
	PosBonus = SpaceObject.getPosition();
	if (SpaceObject.getPosition().x<-60) restart();
}

void Meteor::draw(RenderWindow& window)
{
	window.draw(SpaceObject);
}

void Meteor::animation()
{
	if (st>0){
	SpaceObject.setTextureRect(IntRect(xsp[ix], ysp[iy], 50, 45));
	ix+=st;
	if (ix > 4) { ix = 0; iy++; if (iy > 5) iy = 0;}
	}
}

bool Meteor::Collision(FloatRect object)
{
	if (SpaceObject.getGlobalBounds().intersects(object)) return true;
	
	return false;
}

