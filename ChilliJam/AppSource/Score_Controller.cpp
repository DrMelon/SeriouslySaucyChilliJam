#pragma once

#include "Score_Contoller.h"

Score_Controller::Score_Controller()
{
	prev_TotalMoney = 0;
	prev_TotalJuice = 0;
	totalMoney = 0;
	totalJuice = 0;
}

Score_Controller::~Score_Controller()
{}

void Score_Controller::addJuiceToTotal(int juice)
{
	totalJuice = totalJuice + juice;
}

void Score_Controller::addMoneyToTotal(int money)
{

	totalMoney = totalMoney + money;
}

void Score_Controller::removeFromTotalMoney(int moneyLost)
{
	totalMoney = totalMoney - moneyLost;
}

void Score_Controller::setPrevTotalMoney(int prevMoneyTotal)
{
	prev_TotalMoney = prevMoneyTotal;
}

void Score_Controller::setPrevTotalJuice(int prevJuiceTotal)
{
	prev_TotalJuice = prevJuiceTotal;
}

void Score_Controller::removeFromTotalJuice(int juiceLost)
{
	totalJuice = totalJuice - juiceLost;
}

int Score_Controller::getTotalJuice()
{
	return totalJuice;
}

int Score_Controller::getTotalMoney()
{
	return totalMoney;
}

int Score_Controller::getPrevTotalMoney()
{
	return prev_TotalMoney;
}
int Score_Controller::getPrevTotalJuice()
{
	return prev_TotalJuice;
}