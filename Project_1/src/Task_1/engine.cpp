#include "engine.h"
#include "slime.h"
#include "action.h"
#include <iomanip>
#include <iostream>
#include <memory>
#include <algorithm>
#include <cmath>
using namespace std;

void init(istream &is, ostream &os) {
    os << "Welcome to Battle of Slimes!" << endl;
    os << "You have Green, Red and Blue. So does Enemy." << endl;
}

vector<bool> checkChoosable(vector<Slime> slimes, Slime* currSlime) {
    vector<bool> choosable;
    for (size_t i = 0; i < slimes.size(); i ++) {
        // choosable if: not current slime & is alive
        choosable.push_back(slimes[i].getHp() > 0 && slimes[i].getName() != currSlime->getName());
    }
    return choosable;
}

Slime* chooseSlime(istream &is, ostream &os, vector<Slime> &slimes, bool isStarting, vector<bool> choosable) {
    int slimeId;
    while (true) {
        os << "Select your " << (isStarting ? "starting" : "next") << " slime (";
        vector<string> options;
        for (size_t i = 0; i < slimes.size(); i ++) {
            if (!choosable[i]) continue;
            options.push_back(to_string(i + 1) + " for " + slimes[i].getName());
        }
        for (size_t i = 0; i < options.size(); i++) {
            os << options[i];
            if (i < options.size() - 1) os << ", ";
        }
        os << "): ";
        is >> slimeId;
        if (slimeId >= 1 && (size_t) slimeId <= slimes.size() && choosable[slimeId - 1]) break;
    }
    return &slimes[slimeId - 1];
}

Slime* enemyChooseSlime(Slime* playerSlime, vector<Slime> &enemySlimes, vector<bool> choosable) {
    for (size_t i = 0; i < enemySlimes.size(); i ++) {
        if (choosable[i] && enemySlimes[i].isSuppress(playerSlime)) return &enemySlimes[i];
    }
    for (size_t i = 0; i < enemySlimes.size(); i ++) {
        if (choosable[i] && enemySlimes[i].getName() == playerSlime->getName()) return &enemySlimes[i];
    }
    return nullptr;
}

int chooseAction(istream &is, ostream &os, vector<bool> choosable) {
    bool noChoice = true;
    for (size_t i = 0; i < choosable.size(); i ++) {
        if (choosable[i]) noChoice = false;
    }
    int actionId;
    while (true) {
        os << "Select your action (1 for skill" << (noChoice ? "): " : ", 2 for change): ");
        is >> actionId;
        if (actionId == 1 || (!noChoice && actionId == 2)) break;
    }
    return actionId;
}

Skill chooseSkill(istream &is, ostream &os, Slime* slime) {
    int skillId;
    while (true) {
        os << "Select the skill (";
        for (size_t i = 0; i < slime->getSkills().size(); i ++) {
            os << i + 1 << " for " << slime->getSkills()[i].getName();
            if (i < slime->getSkills().size() - 1) os << ", ";
        }
        os << "): ";
        is >> skillId;
        if (skillId >= 1 && (size_t) skillId <= slime->getSkills().size()) break;
    }
    return slime->getSkills()[skillId - 1];
}

void printHp(istream &is, ostream &os, Slime* &playerSlime, Slime* &enemySlime) {
    os << "Your " << playerSlime->getName() << ": HP " << playerSlime->getHp();
    os << " || ";
    os << "Enemy's " << enemySlime->getName() << ": HP " << enemySlime->getHp();
    os << endl;
}

enum round_result {WIN, LOSE, DRAW, NONE};

round_result checkRoundResult(vector<Slime> playerSlimes, vector<Slime> enemySlimes, int round) {
    if (round >= 100) return DRAW;
    bool playerFlag = true, enemyFlag = true; // whether player / enemy's slimes are all dead
    for (size_t i = 0; i < playerSlimes.size(); i ++) {
        if (playerSlimes[i].getHp() > 0) {
            playerFlag = false; // not all dead
            break;
        }
    }
    for (size_t i = 0; i < enemySlimes.size(); i ++) {
        if (enemySlimes[i].getHp() > 0) {
            enemyFlag = false; // not all dead
            break;
        }
    }
    if (playerFlag && !enemyFlag) return LOSE;
    if (enemyFlag && !playerFlag) return WIN;
    return NONE;
}

bool compareActions(const shared_ptr<Action>& a, const shared_ptr<Action>& b) {
    if (a->getPriority() != b->getPriority()) return a->getPriority() > b->getPriority();
    if (a->getSlime()->getSpeed() != b->getSlime()->getSpeed()) return a->getSlime()->getSpeed() > b->getSlime()->getSpeed();
    return b->getIsPlayer();
}

void play(istream &is, ostream &os) {
    vector<Slime> playerSlimes = {Slime("Green"), Slime("Red"), Slime("Blue")}, enemySlimes = {Slime("Green"), Slime("Red"), Slime("Blue")};
    vector<bool> allTrue = {true, true, true};
    Slime* playerSlime = chooseSlime(is, os, playerSlimes, true, allTrue);
    Slime* enemySlime = enemyChooseSlime(playerSlime, enemySlimes, allTrue);
    int roundNumber = 1;
    os << "You starts with " + playerSlime->getName() << endl;
    os << "Enemy starts with " + enemySlime->getName() << endl;
    printHp(is, os, playerSlime, enemySlime);
    os << "Battle starts!" << endl;
    while (is) {
        // init
        vector<shared_ptr<Action>> actions;

        // print round info
        os << "--------------------------------------------------" << endl;
        os << "Round " << roundNumber << endl;

        // player choose action
        int actionId = chooseAction(is, os, checkChoosable(playerSlimes, playerSlime));
        if (actionId == 1) {
            Skill skill = chooseSkill(is, os, playerSlime);
            actions.push_back(make_shared<SkillAction>(playerSlime, true, skill));
        } else if (actionId == 2) {
            Slime* nxtSlime = chooseSlime(is, os, playerSlimes, false, checkChoosable(playerSlimes, playerSlime));
            actions.push_back(make_shared<ChangeAction>(nxtSlime, true));
        }
        // enemy choose action
        int enemySkillId = 0;
        if (enemySlime->isSuppress(playerSlime)) enemySkillId = 1;
        actions.push_back(make_shared<SkillAction>(enemySlime, false, enemySlime->getSkills()[enemySkillId]));
        
        // sort actions
        sort(actions.begin(), actions.end(), compareActions);
        // act
        for (size_t i = 0; i < actions.size(); i ++) {
            Action& act = *actions[i];
            Slime *from = enemySlime, *to = playerSlime;
            if (act.getIsPlayer()) {
                from = playerSlime;
                to = enemySlime;
            }
            if (act.getType() == SKILL) {
                if (from->getHp() <= 0) continue; // do not use skill if slime is dead
                Skill skill = act.getSkill();
                int damage = round(skill.getForce() * from->getAttack() * getRatio(skill.getProperty(), to->getProperty()) * 1.0 / to->getDefense()); // count damage
                to->addHp(-damage);
                os << (act.getIsPlayer() ? "Your " : "Enemy's ") << from->getName() << " uses " << skill.getName() << "! Damage: " << damage << endl;
            } else if (act.getType() == CHANGE) {
                Slime* slime = act.getSlime();
                if (act.getIsPlayer()) playerSlime = slime;
                else enemySlime = slime;
                os << (act.getIsPlayer() ? "You " : "Enemy ") << "sends " << slime->getName() << endl;
            }
        }

        // check round result
        round_result roundResult = checkRoundResult(playerSlimes, enemySlimes, roundNumber);

        // change dead slimes
        if (playerSlime->getHp() <= 0) {
            os << "Your " << playerSlime->getName() << " is beaten" << endl;
            if (roundResult == NONE) {
                playerSlime = chooseSlime(is, os, playerSlimes, false, checkChoosable(playerSlimes, playerSlime));
                os << "You sends " << playerSlime->getName() << endl;
            }
        }
        if (enemySlime->getHp() <= 0) {
            os << "Enemy's " << enemySlime->getName() << " is beaten" << endl;
            if (roundResult == NONE) {
                enemySlime = enemyChooseSlime(playerSlime, enemySlimes, checkChoosable(enemySlimes, enemySlime));
                os << "Enemy sends " << enemySlime->getName() << endl;
            }
        }

        // print game result
        if (roundResult == WIN) {
            os << "Enemy Lose You Win" << endl;
            break;
        } else if (roundResult == LOSE) {
            os << "Enemy Win You Lose" << endl;
            break;
        }
        
        // print hp
        printHp(is, os, playerSlime, enemySlime);

        if (roundResult == DRAW) {
            os << "Draw" << endl;
            break;
        }
        
        roundNumber += 1;
    }
}