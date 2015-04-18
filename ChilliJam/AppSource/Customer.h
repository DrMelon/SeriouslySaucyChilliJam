#ifndef CUSTOMER_H
#define CUSTOMER_H


class Customer
{
public:
	Customer();
	~Customer();
	void setCustomerValues();
	void setBlastRadius(float);
	void setInfected(bool);

	void updateCustomerPos();
	
	float getBlastRadius();
	double getCustomerHunger();
	double getCustomerHeatTolerance();
	double getCustomerMoney();
	float  getCustomer_XPos();
	float  getCustomer_YPos();
	bool   getCustomerInfected();


	

private:
	float  customerRadius;
	bool   infected;
	double hunger;
	double heatTolerance;
	float  blastRadius;
	float  customer_XPos;
	float  customer_YPos;
	float  customerSpeed;
	double customerMoney;
	int	   customerGift;
	bool   giftGiven;

};


#endif //SCORE_CONTROLLER_H
