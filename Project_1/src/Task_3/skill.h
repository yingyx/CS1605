#ifndef SKILL_H_INCLUDED
#define SKILL_H_INCLUDED

#include "property.h"
#include <string>

enum SkillType {TACKLE, LEAF, FLAME, STREAM};

class Skill {
private:
    PropertyType property;
    std::string name;
    int force;
public:
    Skill(SkillType skillType);
    ~Skill();

    std::string getName() const;
    PropertyType getProperty() const;
    int getForce() const;
};

#endif // SKILL_H_INCLUDED