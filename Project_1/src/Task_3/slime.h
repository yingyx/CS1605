#ifndef SLIME_H_INCLUDED
#define SLIME_H_INCLUDED

#include <string>
#include <vector>
#include "skill.h"

class Slime {
private:
    std::string name;
    PropertyType property;
    int hp, attack, defense, speed, maxHp;
    std::vector<Skill> skills;
    bool doubleAttack = false;
public:
    Slime(std::string name);
    ~Slime();
    std::string getName() const;
    int getHp() const;
    bool isSuppress(Slime* other) const;
    std::vector<Skill> getSkills() const;
    int getSpeed() const;
    int getAttack() const;
    int getDefense() const;
    PropertyType getProperty() const;
    bool getDoubleAttack() const;

    void addHp(int hp);
    void revive();

    bool setDoubleAttack(bool doubleAttack);
};


#endif // SLIME_H_INCLUDED