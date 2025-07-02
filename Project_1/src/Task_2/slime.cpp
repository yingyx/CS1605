#include "slime.h"
#include "skill.h"
using namespace std;

Slime::Slime(string name)
    : name(name)
{
    if (name == "Green") {
        property = GRASS;
        hp = 110;
        attack = 10;
        defense = 10;
        speed = 10;
        skills.push_back(Skill(SkillType::TACKLE));
        skills.push_back(Skill(SkillType::LEAF));
    } else if (name == "Red") {
        property = FIRE;
        hp = 100;
        attack = 11;
        defense = 10;
        speed = 11;
        skills.push_back(Skill(SkillType::TACKLE));
        skills.push_back(Skill(SkillType::FLAME));
    } else if (name == "Blue") {
        property = PropertyType::WATER;
        hp = 100;
        attack = 10;
        defense = 11;
        speed = 9;
        skills.push_back(Skill(SkillType::TACKLE));
        skills.push_back(Skill(SkillType::STREAM));
    }
}

Slime::~Slime(){}

string Slime::getName() const {
    return name;
}

bool Slime::isSuppress(Slime* other) const {
    return getRatio(property, other->property) > 1;
}

vector<Skill> Slime::getSkills() const { return skills; }
int Slime::getHp() const { return hp; }
int Slime::getAttack() const { return attack; }
int Slime::getDefense() const { return defense; }
int Slime::getSpeed() const { return speed; }
PropertyType Slime::getProperty() const { return property; }

void Slime::addHp(int delta) {
    hp += delta;
}