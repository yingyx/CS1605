#ifndef SLIME_H_INCLUDED
#define SLIME_H_INCLUDED

#include <string>
#include <vector>
#include "skill.h"

class Slime {
private:
    std::string name;
    PropertyType property;
    int hp, attack, defense, speed;
    std::vector<Skill> skills;
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

    void addHp(int hp);
};


#endif // SLIME_H_INCLUDED