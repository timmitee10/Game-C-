#pragma once
class Bandage
{
public:
	Bandage();
	~Bandage();
	float GetHealAmount() const { return healAmount; };
private:
	float healAmount;
};