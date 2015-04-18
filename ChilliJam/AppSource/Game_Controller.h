#ifndef GAME_CONTROLLER_H
#define GAME_CONTROLLER_H


class Game_Controller
{
public:
	Game_Controller();
	~Game_Controller();
	
	void gameLoop();

	/*
	Values needed

	total money
	total juice
	time of day
	customers servered

	//chilli
		beef
		spice
		alien juice
		gift
		quantity

	*/

	void setGameValues(int, int);
	void setMoney(int);
	void setJuice(int);
	void countInfected();
	

	
private:
	int	total_Money;

	float roundTime;
	int customersServed;

	int total_Beef;
	int total_Juice;
	int total_Spice;
	int gift;
	int day;


};


#endif //GAME_CONTROLLER_H
