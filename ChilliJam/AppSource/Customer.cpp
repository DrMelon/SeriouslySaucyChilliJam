#pragma once

#include <random>

#include "Customer.h"

Customer::Customer()
{
	hunger = 0.0f;
	heatTolerance = 0.0f;
	customerSpeed = 0.0f;
	blastRadius = 0.0f;
	giftGiven = false;
	customerGift = NULL;
	

}

Customer::~Customer()
{}

void Customer::setCustomerValues()
{
	/*
	Hunger is a values between 0.1 - 10
	Heat Tolerance is a value between 0.1 - 10
	Customer Speed  is a Value between 0.1 - 10
	Customer Money is a Value beteen 0.10 - 10.00

	Customer Gift is used for enums (1 -10)
	*/

	hunger = ((double)rand() / (10)) + 0.1f;
	heatTolerance = ((double)rand() / (10)) + 0.1f;
	customerSpeed = ((float)rand() / (10)) + 0.1f;
	customerMoney = ((double)rand() / (10)) + 0.1f;
	customerGift = ((int)rand() / (10)) + 1;
	customerRadius = 5;
	infected = false;
}

void Customer::setBlastRadius(float radius)
{
	blastRadius = blastRadius + radius;
}

void Customer::setInfected(bool isInfected)
{
	infected = isInfected;
}

void Customer::updateCustomerPos()
{
	float tempXMovement = ((float)rand() / (5)) + 0.1f;
	float tempYMovement = ((float)rand() / (5)) + 0.1f;

	customer_XPos = customer_XPos + tempXMovement;
	customer_YPos = customer_YPos + tempYMovement;
}


float Customer::getBlastRadius()
{
	return blastRadius;
}

double Customer::getCustomerHunger()
{
	return hunger;
}

double Customer::getCustomerHeatTolerance()
{
	return heatTolerance;
}

double Customer::getCustomerMoney()
{
	return customerMoney;
}

float Customer::getCustomer_XPos()
{
	return customer_XPos;
}

float Customer::getCustomer_YPos()
{
	return customer_YPos;
}

bool Customer::getCustomerInfected()
{
	return infected;
}