#pragma once
#include "Meteor.h";
#include "Full.h";
#include <sstream>
#include <string>
#include <SFML/Audio.hpp>
using namespace sf;
using namespace std;

struct FrameAnim
{
	int Frame = 0;
	int Step = 100;
	int Frame1 = 0;
	int Step1 = 100;
};

void playermove(Sprite & player, Vector2f & moveRec);

void playeranim(Sprite& player, FrameAnim& FramePlAnim, int traffic);
// ѕроверка на совпадение координат метеор
void Correct(Full & canister, int i, Meteor * meteor, int nmeteor );  
// ѕроверка на совпадение координат канистры с топливом
void CorrectFull(Full& canister, Meteor* meteor, int nmeteor);
// ѕриведение целочисленного типа  к строковому
string IntToStr(int number);