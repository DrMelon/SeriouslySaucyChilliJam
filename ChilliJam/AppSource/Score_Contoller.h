#ifndef SCORE_CONTROLLER_H
#define SCORE_CONTROLLER_H


class Score_Controller
{
public:
	Score_Controller();
	~Score_Controller();
	void addMoneyToTotal(int moneyEarned);
	void addJuiceToTotal(int juiceEarned);
	void removeFromTotalMoney(int moneyLost);
	void setPrevTotalMoney(int);
	void setPrevTotalJuice(int);
	void removeFromTotalJuice(int juiceLost);
	int getTotalMoney();
	int getTotalJuice();
	int getPrevTotalMoney();
	int getPrevTotalJuice();
private:
	int prev_TotalMoney;
	int prev_TotalJuice;
	int totalMoney;
	int totalJuice;
};


#endif //SCORE_CONTROLLER_H
