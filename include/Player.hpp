#pragma once

class Player {
public:
    Player(int hp, int gold) : hp(hp), gold(gold) {}

    void addGold(int amount) { gold += amount; }
    void takeDamage(int amount) { hp -= amount; }
    
    void setGold(int amount){gold=amount;}
    void setHp(int amount){hp=amount;}

    int getHP() const { return hp; }
    int getGold() const { return gold; }

private:
    int hp;
    int gold;
};
