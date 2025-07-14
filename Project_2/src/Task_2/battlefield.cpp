#include "battlefield.h"

#include "define.h"
#include "helper.h"

#include <string>


BattleField::BattleField(int maxRounds): maxRounds_(maxRounds) {}

void BattleField::Reset() {
    this->rounds_ = 0;
    this->actors_[0] = this->actors_[1] = nullptr;
    this->attr.clear();
}

void BattleField::Run() {
    this->BeforeGame_();

    bool ending = false;
    while (!ending) {
        LOG("--------------------------------------------------\n");
        this->StateCheck_();
        LOG("Round", ++this->rounds_); LOG('\n');
        std::vector<ActionInfo> actions = this->ChooseActionPhase_();
        ending = this->PerformActionPhase_(actions);
        if (ending) break;
        this->HandleBeatenPetsPhase_();
        ending = this->ShowInformationPhase_();
    }

    // system("pause");
}

void BattleField::SetActor(Actor *a, Actor *b) {
    this->actors_[0] = a; this->actors_[1] = b;
    a->opp = b; b->opp = a;
    a->field = b->field = this;
}

void BattleField::BeforeGame_() {
    // welcome messages
    LOG("Welcome to Battle of Slimes 2!\n");

    // enemy choose pets
    Actor* enemyActor = this->actors_[1];
    std::vector<Pet_T> enemyPets = enemyActor->ChooseStartingPet();
    LOG("Enemy has", allPets.at(enemyPets[0]).petName + ",", allPets.at(enemyPets[1]).petName + ",", allPets.at(enemyPets[2]).petName + ",", "starting with", allPets.at(enemyPets[0]).petName);
    LOG("\n");

    // player choose pets
    Actor* humanActor = this->actors_[0];
    std::vector<Pet_T> humanPets = humanActor->ChooseStartingPet();
    LOG("You have", allPets.at(humanPets[0]).petName + ",", allPets.at(humanPets[1]).petName + ",", allPets.at(humanPets[2]).petName + ",", "starting with", allPets.at(humanPets[0]).petName);
    LOG("\n");

    // print hp
    this->ShowInformationPhase_();

    // game start messages
    LOG("Battle starts!\n");
}



void BattleField::StateCheck_() {
    // TODO: 每回合开始前，检查天气、昏睡回合数等
    if (attr["weatherCnt"]) attr["weatherCnt"] -= 1;
    if (!attr["weatherCnt"] && attr["weather"] != Weather::Normal) {
        attr["weather"] = Weather::Normal;
        LOG("Weather becomes normal\n");
    }


    for (Actor* actor : this->actors_) {
        if (actor->petOnCourt.attr["faintCnt"]) actor->petOnCourt.attr["faintCnt"] -= 1;
        if (!actor->petOnCourt.attr["faintCnt"] && actor->petOnCourt.attr["faint"]) {
            actor->petOnCourt.attr["faint"] = 0;
            LOG(this->Whose(actor->GetName()), actor->petOnCourt.petName, "wakes up\n");
        }

        actor->petOnCourt.attr["usedSkill"] = 0;
    }
    
    
}

std::vector<ActionInfo> BattleField::ChooseActionPhase_() {
    // returns actions choosed by the actors
    std::vector<ActionInfo> actions;

    for (Actor* actor : this->actors_) {
        while (true) {
            Action_T chosenAction = actor->ChooseAction();
            union {
                Pet_T pet;
                Skill_T skill;
                PotionInfo potion;
            } u;
            if (chosenAction == Action_T::Change) {
                u.pet = actor->ChoosePet(true);
                if (u.pet == Pet_T::BACK) continue;
            } else if (chosenAction == Action_T::Skill) {
                u.skill = actor->ChooseSkill();
                if (u.skill == Skill_T::BACK) continue;
            }
            ActionInfo actionInfo = ActionInfo{
                chosenAction,
                actor,
                chosenAction == Action_T::Skill ? allSkills.at(u.skill).priority : allActions.at(chosenAction).priority,
                u.pet,
            };
            actions.push_back(actionInfo);
            break;
        }
    }

    return actions;
}

bool compareActions(const ActionInfo &a, const ActionInfo &b) {
    if (a.priority != b.priority) return a.priority > b.priority;
    if (a.action == Action_T::Skill
        && b.action == Action_T::Skill
        && a.actor->petOnCourt.speed != b.actor->petOnCourt.speed) {
        return a.actor->petOnCourt.speed > b.actor->petOnCourt.speed;
    }
    return a.actor->GetName() != "You";
}

bool BattleField::PerformActionPhase_(std::vector<ActionInfo> &actions) {
    // sort actions
    std::sort(actions.begin(), actions.end(), compareActions);

    // perform actions
    for (ActionInfo &actionInfo : actions) {
        if (actionInfo.actor->petOnCourt.health <= 0) continue;
        
        if (actionInfo.action == Action_T::Escape) {
            LOG("You Escape");
            return true;
        } else if (actionInfo.action == Action_T::Skill) {
            if (actionInfo.actor->petOnCourt.attr["faint"]) {
                LOG(this->Whose(actionInfo.actor->GetName()), actionInfo.actor->petOnCourt.petName, "is sleeping!\n");
                continue;
            }
            Skill skill = allSkills.at(actionInfo.u.skill);
            SkillCbReturn skillCbReturn = skill.cb(this, actionInfo.actor, &skill);
            if (skillCbReturn.success) {
                actionInfo.actor->opp->petOnCourt.health -= skillCbReturn.damage;
            }
            LOG(this->Whose(actionInfo.actor->GetName()), actionInfo.actor->petOnCourt.petName, "uses", skill.skillName + "!", skillCbReturn.prompt);
            LOG("\n");
            if (skillCbReturn.success) {
                actionInfo.actor->petOnCourt.attr["usedSkill"] = 1;
            }
        } else if (actionInfo.action == Action_T::Change) {
            actionInfo.actor->availPets[actionInfo.actor->petOnCourt.pet] = actionInfo.actor->petOnCourt;
            actionInfo.actor->petOnCourt = actionInfo.actor->availPets.at(actionInfo.u.pet);
            actionInfo.actor->availPets.erase(actionInfo.u.pet);
            LOG(actionInfo.actor->GetName(), "sends", actionInfo.actor->petOnCourt.petName);
            LOG("\n");
        }
    }
    
    return false;
}

void BattleField::HandleBeatenPetsPhase_() {
    // handle beaten pets
    for (Actor *actor : this->actors_) {
        if (actor->petOnCourt.health <= 0) {
            LOG(this->Whose(actor->GetName()), actor->petOnCourt.petName, "is beaten\n");
            actor->grave[actor->petOnCourt.pet] = actor->petOnCourt;
            Pet_T newPet = actor->ChoosePet(false);
            if (newPet == Pet_T::BACK) continue;
            actor->petOnCourt = actor->availPets.at(newPet);
            actor->availPets.erase(actor->petOnCourt.pet);
            LOG(actor->GetName(), "sends", actor->petOnCourt.petName);
            LOG("\n");
        }
    }
}

bool BattleField::ShowInformationPhase_() {
    // print hp & returns win status
    for (Actor *actor : this->actors_) {
        if (actor->availPets.empty() && actor->petOnCourt.health <= 0
        && (!actor->opp->availPets.empty() || actor->petOnCourt.health > 0)) {
            LOG("You", actor->opp->GetName() == "You" ? "Win" : "Lose");
            LOG("\n");
            return true;
        }
    }
    if (rounds_ >= maxRounds_) {
        LOG("Draw");
        return true;
    }

    LOG(Whose(this->actors_[0]->GetName()), this->actors_[0]->petOnCourt.petName + ":", "HP", this->actors_[0]->petOnCourt.health);
    LOG(" || ");
    LOG(Whose(this->actors_[1]->GetName()), this->actors_[1]->petOnCourt.petName + ":", "HP", this->actors_[1]->petOnCourt.health);
    LOG("\n");

    return false;
}

std::string BattleField::Whose(std::string name) {
    return (name == "You" ? "Your" : name + "'s");
}
