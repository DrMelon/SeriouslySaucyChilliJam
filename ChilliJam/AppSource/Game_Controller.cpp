#pragma once

#include "Game_Controller.h"

Game_Controller::Game_Controller()
{
	total_Juice = -1;
	total_Money = -1;
	total_Spice = -1;
	total_Beef = -1;

	gift = -1;
	day = -1;

}

Game_Controller::~Game_Controller()
{

}

void Game_Controller::setGameValues(int initMoney,int initJuice)
{

	total_Money = total_Money + initMoney;
	total_Juice = total_Juice + initJuice;

}

void Game_Controller::countInfected()
{

}

