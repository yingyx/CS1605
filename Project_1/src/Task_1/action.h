#ifndef ACTION_H_DEFINED
#define ACTION_H_DEFINED

#include "slime.h"

enum ActionType {SKILL, CHANGE};

class Action {
protected:
    int priority;
    bool isPlayer;
public:
    Action(bool isPlayer) : priority(0), isPlayer(isPlayer) {}
    virtual ~Action() {}
    bool getIsPlayer() const { return isPlayer; }
    virtual int getPriority() const { return priority; }
    virtual const Skill& getSkill() const {}
    virtual Slime* getSlime() const {}
    virtual const ActionType getType() const {}
};

class SkillAction : public Action {
    Slime* slime;
    Skill skill;
public:
    SkillAction(Slime* s, bool isPlayer, Skill skill) : Action(isPlayer), slime(s), skill(skill) { priority = 0; }
    int getPriority() const override { return 0; }
    const Skill& getSkill() const { return skill; }
    Slime* getSlime() const { return slime; }
    const ActionType getType() const override { return SKILL; }
};

class ChangeAction : public Action {
    Slime* slime;
public:
    ChangeAction(Slime* s, bool isPlayer) : Action(isPlayer), slime(s) { priority = 6; }
    int getPriority() const override { return 6; }
    Slime* getSlime() const { return slime; }
    const ActionType getType() const override { return CHANGE; }
};

#endif