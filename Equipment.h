#pragma once
#ifndef EQUIPMENT_H
#define EQUIPMENT_H
#include "Items.h"
class Equipment
{
public:
	Equipment(int, Profession);
	~Equipment();
	bool ChangeItem(Item*);
	Item* weapon_slot;
	Item* armor_slot;
	Item* shield_slot;
	Item* headgear_slot;
	Item* talisman_slot;
private:
	ItemFactory* fac;
};
#endif
