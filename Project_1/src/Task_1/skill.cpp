#include "skill.h"
#include <string>

using namespace std;

Skill::Skill(SkillType skillType) : force(20) {
    switch (skillType) {
        case TACKLE:
            name = "Tackle";
            property = NORMAL;
            break;
        case LEAF:
            name = "Leaf";
            property = GRASS;
            break;
        case FLAME:
            name = "Flame";
            property = FIRE;
            break;
        case STREAM:
            name = "Stream";
            property = WATER;
            break;
    }
}

Skill::~Skill(){};

string Skill::getName() const { return name; }
int Skill::getForce() const { return force; }
PropertyType Skill::getProperty() const { return property; }