#include <iostream>
#include <cstdlib>
#include "Chambers.h"
using namespace std;
int chambersWithoutMonsters;
int chambersWithoutTrader;

ItemType getRandomItemType(Hero* h) {
    int type = rand() % 4;

    if (h->getProf() == warrior) {
        type += rand() % 2;
    }

    ItemType itemType;
    if (type == 0)
        itemType = weapon;
    else if (type == 1)
        itemType = armor;
    else if (type == 2)
        itemType = headgear;
    else if (type == 3)
        itemType = talisman;
    else
        itemType = shield;

    return itemType;
}
Chest::Chest(Hero* h) {
    ItemType itemType = getRandomItemType(h);

    item = ItemFactory::createItem(h->getlevel(), itemType, h->getProf());
}


Chamber::Chamber(Hero* h) {
	chamber_ID = rand() % 1000;
	hero = h->getInstance();
}

void Chamber::takeAction(Hero *h) {}

int Chamber::getChamberID() {
	return chamber_ID;
}

string Chamber::getName() {
	return name;
}

template<typename Base, typename T>
inline bool instanceof(const T *ptr) {
    return dynamic_cast<const Base*>(ptr) != nullptr;
}

void Chamber::eventTransitionFunction(EventNode* start, Hero* h) {
    EventNode* curr = start;
    while (true) {
        curr->current->Action();

        if (h->getcurrentHealth() == 0)
            break;

        int numOfNexts = curr->AllNexts.size();
        if (numOfNexts == 1)
            curr = curr->AllNexts[0];
        else {
            for (int i = 0; i < curr->AllNexts.size(); i++) {
                cout << "Option [" << i+1 << "]: ";
                curr->AllNexts[i]->current->DisplayDescription();
                cout << endl;
            }

            string str;
            cin >> str;

            int choice = stoi(str);
            while (choice < 1 || choice > numOfNexts) {
                cout << "Character not recognized, please retype" << endl;
                cin >> choice;
            }

            curr = curr->AllNexts[choice-1];
        }

        if (instanceof<EndPoint>(curr->current))
            break;
    }
}

BossChamber::BossChamber(Hero *h) : Chamber(h) {
	int lvl = h->getlevel() * 3 / 2;
	boss_monster = new monster(lvl, "Great BOSS");
}

void BossChamber::takeAction(Hero *h) {
    EndPoint* e0 = new EndPoint(h);
    EnterToBossRoom* e1 = new EnterToBossRoom(h);
    Fight* e2 = new Fight(h, boss_monster);

    EventNode* start = new EventNode(e1);
    EventNode* fightBoss = new EventNode(e2);
    EventNode* end = new EventNode(e0);

    start->AllNexts.push_back(fightBoss);
    fightBoss->AllNexts.push_back(end);

    eventTransitionFunction(start, h);

	if (h->getcurrentHealth() > 0)
		cout << "Congratulations! You finished the game" << endl;
	else
		cout << "It was very close..." << endl;
}


PassageChamber::PassageChamber(Hero *h) : Chamber(h) {}
void PassageChamber::takeAction(Hero* h) {}


NormalChamber::NormalChamber(Hero *h) : PassageChamber(h) {}
void NormalChamber::takeAction(Hero* h) {}


SafeChamber::SafeChamber(Hero *h) : PassageChamber(h) {}
void SafeChamber::takeAction(Hero* h) {}


MonsterRoom::MonsterRoom(Hero *h) : NormalChamber(h) {
    chambersWithoutMonsters = 0;
    chambersWithoutTrader++;
    normal_monster = new monster(h->getlevel(), 0);
    chest = new Chest(h);
}

void MonsterRoom::takeAction(Hero *h) {
    //przygotowanie przejść:
    EndPoint* e0 = new EndPoint(h);
    EnterToMonsterRoom* e1 = new EnterToMonsterRoom(h);
    Fight* e2 = new Fight(h, normal_monster);
    RunAway* e3 = new RunAway(h);
    CheckChest* e4 = new CheckChest(h, chest);

    EventNode* start = new EventNode(e1);
    EventNode* fight = new EventNode(e2);
    EventNode* runAway = new EventNode(e3);
    EventNode* openBox = new EventNode(e4);
    EventNode* end = new EventNode(e0);

    start->AllNexts.push_back(fight);
    start->AllNexts.push_back(runAway);
    fight->AllNexts.push_back(openBox);
    fight->AllNexts.push_back(end);
    openBox->AllNexts.push_back(end);
    runAway->AllNexts.push_back(end);

    //funkcja przejścia:
    eventTransitionFunction(start, h);
}


TrapRoom::TrapRoom(Hero *h) : NormalChamber(h) {
    chambersWithoutMonsters++;
    chambersWithoutTrader++;
}

void TrapRoom::takeAction(Hero *h) {
    EndPoint* e0 = new EndPoint(h);
    EnterToTrapRoom* e1 = new EnterToTrapRoom(h);
    ActiveTheTrap* e2 = new ActiveTheTrap(h);

    EventNode* start = new EventNode(e1);
    EventNode* trap = new EventNode(e2);
    EventNode* end = new EventNode(e0);

    start->AllNexts.push_back(trap);
    trap->AllNexts.push_back(end);

    eventTransitionFunction(start, h);
}


PotionRoom::PotionRoom(Hero *h) : NormalChamber(h) {
    chambersWithoutMonsters++;
    chambersWithoutTrader++;
}

void PotionRoom::takeAction(Hero *h) {
    EndPoint* e0 = new EndPoint(h);
    EnterToPotionRoom* e1 = new EnterToPotionRoom(h);
    DrinkPotion* e2 = new DrinkPotion(h);

    EventNode* start = new EventNode(e1);
    EventNode* potion = new EventNode(e2);
    EventNode* end = new EventNode(e0);

    start->AllNexts.push_back(potion);
    start->AllNexts.push_back(end);
    potion->AllNexts.push_back(end);

    eventTransitionFunction(start, h);
}


TreasureRoom::TreasureRoom(Hero *h) : SafeChamber(h) {
    chambersWithoutMonsters++;
    chambersWithoutTrader++;
    chest = new Chest(h);
}

void TreasureRoom::takeAction(Hero *h) {
    EndPoint* e0 = new EndPoint(h);
    EnterToTreasureRoom* e1 = new EnterToTreasureRoom(h);
    CheckChest* e2 = new CheckChest(h, chest);

    EventNode* start = new EventNode(e1);
    EventNode* chest = new EventNode(e2);
    EventNode* end = new EventNode(e0);

    start->AllNexts.push_back(chest);
    start->AllNexts.push_back(end);
    chest->AllNexts.push_back(end);

    eventTransitionFunction(start, h);
}


HealthRoom::HealthRoom(Hero *h) : SafeChamber(h) {
    chambersWithoutMonsters++;
    chambersWithoutTrader++;
}

void HealthRoom::takeAction(Hero *h) {
    EndPoint* e0 = new EndPoint(h);
    EnterToHealthRoom* e1 = new EnterToHealthRoom(h);
    HealthYourself* e2 = new HealthYourself(h);

    EventNode* start = new EventNode(e1);
    EventNode* health = new EventNode(e2);
    EventNode* end = new EventNode(e0);

    start->AllNexts.push_back(health);
    start->AllNexts.push_back(end);
    health->AllNexts.push_back(end);

    eventTransitionFunction(start, h);
}


TraderRoom::TraderRoom(Hero* h) : SafeChamber(h) {
    chambersWithoutMonsters++;
    chambersWithoutTrader = 0;
	item1 = ItemFactory::createItem(h->getlevel(), getRandomItemType(h), h->getProf());
	item2 = ItemFactory::createItem(h->getlevel(), getRandomItemType(h), h->getProf());
	item3 = ItemFactory::createItem(h->getlevel(), getRandomItemType(h), h->getProf());
};

void TraderRoom::takeAction(Hero* h) {
    EndPoint* e0 = new EndPoint(h);
    EnterToTraderRoom* e1 = new EnterToTraderRoom(h);
    SeeItems* e2 = new SeeItems(h, item1, item2, item3);
    BuyItems* e3 = new BuyItems(h, item1, item2, item3);

    EventNode* start = new EventNode(e1);
    EventNode* see = new EventNode(e2);
    EventNode* buy = new EventNode(e3);
    EventNode* end = new EventNode(e0);

    start->AllNexts.push_back(see);
    start->AllNexts.push_back(end);
    see->AllNexts.push_back(buy);
    see->AllNexts.push_back(end);
    buy->AllNexts.push_back(end);

    eventTransitionFunction(start, h);
}


EmptyRoom::EmptyRoom(Hero *h) : SafeChamber(h) {
    chambersWithoutMonsters++;
    chambersWithoutTrader++;
}

void EmptyRoom::takeAction(Hero* h) {
    EndPoint* e0 = new EndPoint(h);
    EnterToEmptyRoom* e1 = new EnterToEmptyRoom(h);

    EventNode* start = new EventNode(e1);
    EventNode* end = new EventNode(e0);

    start->AllNexts.push_back(end);

    eventTransitionFunction(start, h);
}


StartingRoom::StartingRoom(Hero *h) : SafeChamber(h) {
	//numOfChamber = 0;
    chambersWithoutMonsters = 0;
    chambersWithoutTrader = 0;
}

void StartingRoom::takeAction(Hero *h) {
    EndPoint* e0 = new EndPoint(h);
    EnterToStartingRoom* e1 = new EnterToStartingRoom(h);

    EventNode* start = new EventNode(e1);
    EventNode* end = new EventNode(e0);

    start->AllNexts.push_back(end);

    eventTransitionFunction(start, h);
}


ChamberNode::ChamberNode(Chamber* curr) {
    current = curr;
    option1 = nullptr;
    option2 = nullptr;
}