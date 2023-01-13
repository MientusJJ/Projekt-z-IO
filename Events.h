#pragma once
#ifndef EVENTS_H
#define EVENTS_H
#include "Character.h"
#include <vector>

#include "Strategy.h"

class Chest {
protected:
    shared_ptr<Item> item;

public:
    Chest(shared_ptr<Hero>& hero);
    void openBox(shared_ptr<Hero>& hero);
};

class DescriptionVisitor;

// Controller
class Event {
protected:
    shared_ptr<Hero> h;
    string description; //do przeniesienia (?)
public:
    virtual void DisplayDescription(DescriptionVisitor visitor) = 0;
    virtual void Action();
};

class EndPoint : public Event {
public:
    EndPoint(shared_ptr<Hero>& h);
    void DisplayDescription(DescriptionVisitor visitor);
    void Action();
};

class EnterToMonsterRoom : public Event {
public:
    EnterToMonsterRoom(shared_ptr<Hero>& h);
    void DisplayDescription(DescriptionVisitor visitor);
    void Action();
};

class Fight : public Event {
protected:
    shared_ptr<monster> _monster;
public:
    Fight(shared_ptr<Hero>& hero, shared_ptr<monster> m);
    void DisplayDescription(DescriptionVisitor visitor);
    void Action();
};

class RunAway : public Event {
public:
    RunAway(shared_ptr<Hero>& h);
    void DisplayDescription(DescriptionVisitor visitor);
    void Action();
};

class CheckChest : public Event {
protected:
    shared_ptr<Chest> _chest;
public:
    CheckChest(shared_ptr<Hero>& hero, shared_ptr<Chest>& chest);
    void DisplayDescription(DescriptionVisitor visitor);
    void Action();
};

class EnterToTrapRoom : public Event {
public:
    EnterToTrapRoom(shared_ptr<Hero>& h);
    void DisplayDescription(DescriptionVisitor visitor);
    void Action();
};

class ActiveTheTrap : public Event {
public:
    ActiveTheTrap(shared_ptr<Hero>& h);
    void DisplayDescription(DescriptionVisitor visitor);
    void Action();
};

class EnterToPotionRoom : public Event {
public:
    EnterToPotionRoom(shared_ptr<Hero>& h);
    void DisplayDescription(DescriptionVisitor visitor);
    void Action();
};

class DrinkPotion : public Event {
public:
    DrinkPotion(shared_ptr<Hero>& h);
    void DisplayDescription(DescriptionVisitor visitor);
    void Action();
};

class EnterToTreasureRoom : public Event {
public:
    EnterToTreasureRoom(shared_ptr<Hero>& h);
    void DisplayDescription(DescriptionVisitor visitor);
    void Action();
};

class EnterToHealthRoom : public Event {
public:
    EnterToHealthRoom(shared_ptr<Hero>& h);
    void DisplayDescription(DescriptionVisitor visitor);
    void Action();
};

class HealthYourself : public Event {
public:
    HealthYourself(shared_ptr<Hero>& h);
    void DisplayDescription(DescriptionVisitor visitor);
    void Action();
};

class EnterToTraderRoom : public Event {
public:
    EnterToTraderRoom(shared_ptr<Hero>& h);
    void DisplayDescription(DescriptionVisitor visitor);
    void Action();
};

class SeeItems : public Event {
public:
    SeeItems(shared_ptr<Hero>& h, shared_ptr<Item>& i1, shared_ptr<Item>& i2, shared_ptr<Item>& i3);
    void DisplayDescription(DescriptionVisitor visitor);
    void Action();
private:
    shared_ptr<Item> _item1;
    shared_ptr<Item> _item2;
    shared_ptr<Item> _item3;
};

class BuyItems : public Event {
public:
    BuyItems(shared_ptr<Hero>& h, shared_ptr<Item>& i1, shared_ptr<Item>& i2, shared_ptr<Item>& i3,shared_ptr<Strategy>& p_strategy);
    void DisplayDescription(DescriptionVisitor visitor);
    void Action();
private:
    bool buyOneItem(int num);
    shared_ptr<Strategy> _buyingStrategy;
    shared_ptr<Item> _item1;
    shared_ptr<Item> _item2;
    shared_ptr<Item> _item3;
};

class EnterToEmptyRoom : public Event {
public:
    EnterToEmptyRoom(shared_ptr<Hero>& h);
    void DisplayDescription(DescriptionVisitor visitor);
    void Action();
};

class EnterToStartingRoom : public Event {
public:
    EnterToStartingRoom(shared_ptr<Hero>& h);
    void DisplayDescription(DescriptionVisitor visitor);
    void Action();
};

class EnterToBossRoom : public Event {
public:
    EnterToBossRoom(shared_ptr<Hero>& h);
    void DisplayDescription(DescriptionVisitor visitor);
    void Action();
};


class EventNode {
public:
    EventNode(shared_ptr<Event>&& curr);
    shared_ptr<Event> current;
    vector <shared_ptr<EventNode>> AllNexts;
};

// Controller
class DescriptionVisitor {
public:
    void visitEndPoint(EndPoint event);
    void visitEnterToMonsterRoom(EnterToMonsterRoom event);
    void visitFight(Fight event);
    void visitRunAway(RunAway event);
    void visitCheckChest(CheckChest event);
    void visitEnterToTrapRoom(EnterToTrapRoom event);
    void visitActiveTheTrap(ActiveTheTrap event);
    void visitEnterToPotionRoom(EnterToPotionRoom event);
    void visitDrinkPotion(DrinkPotion event);
    void visitEnterToTreasureRoom(EnterToTreasureRoom event);
    void visitEnterToHealthRoom(EnterToHealthRoom event);
    void visitHealthYourself(HealthYourself event);
    void visitEnterToTraderRoom(EnterToTraderRoom event);
    void visitSeeItems(SeeItems event);
    void visitBuyItems(BuyItems event);
    void visitEnterToEmptyRoom(EnterToEmptyRoom event);
    void visitEnterToStartingRoom(EnterToStartingRoom event);
    void visitEnterToBossRoom(EnterToBossRoom event);

private:
    void eventTransitionFunction(shared_ptr<EventNode>& start, shared_ptr<Hero>& h);
};

#endif
