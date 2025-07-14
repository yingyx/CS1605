#include "actor.h"
#include "battlefield.h"

RainyActor::RainyActor() {
    this->attr["revivalPotion"] = 1;
}

std::vector<Pet_T> RainyActor::ChooseStartingPet() {
    // 雨天队有BGY，永远首发B
    // availPets[Pet_T::P] = allPets.at(Pet_T::P);
    availPets[Pet_T::G] = allPets.at(Pet_T::G);
    availPets[Pet_T::R] = allPets.at(Pet_T::Y);
    petOnCourt = allPets.at(Pet_T::B);
    return {Pet_T::B, Pet_T::G, Pet_T::Y};
}

Action_T RainyActor::ChooseAction() {
    // returns chosen action
    if (this->attr["revivalPotion"] && (grave.find(Pet_T::B) != grave.end() || grave.size() == (size_t) 2)) {
        return Action_T::Potion;
    }
    if (this->ChooseSkill() != Skill_T::BACK) return Action_T::Skill;
    return Action_T::Change;
}

Pet_T RainyActor::ChoosePet(bool active) {
    // returns next pet
    if (availPets.empty()) return Pet_T::BACK;
    if (!active) {
        if (petOnCourt.pet == Pet_T::B) {
            return availPets.find(Pet_T::G) != availPets.end() ? Pet_T::G : Pet_T::Y;
        } else {
            return availPets.find(Pet_T::B) != availPets.end() ? Pet_T::B : (petOnCourt.pet == Pet_T::G ? Pet_T::Y : Pet_T::G);
        }
    }
    if (active) {
        if (petOnCourt.pet == Pet_T::B
        && ((field->attr["weather"] != Weather_T::Sunny && opp->petOnCourt.type == Type_T::Thunder)
        || (field->attr["weather"] == Weather_T::Rainy && opp->petOnCourt.type == Type_T::Grass))) {
            if (grave.find(Pet_T::G) == grave.end()) {
                return Pet_T::G;
            }
            if (field->attr["weather"] == Weather_T::Rainy && grave.find(Pet_T::Y) == grave.end()) {
                return Pet_T::Y;
            }
        }
        if (petOnCourt.pet == Pet_T::G && opp->petOnCourt.type == Type_T::Fire && grave.find(Pet_T::B) == grave.end()) {
            return Pet_T::B;
        }
        if (petOnCourt.pet == Pet_T::Y
        && opp->petOnCourt.health * 1.0 >= 0.2 * allPets.at(opp->petOnCourt.pet).health) {
            if (opp->petOnCourt.type == Type_T::Fire && grave.find(Pet_T::B) == grave.end()) {
                return Pet_T::B;
            }
            if (opp->petOnCourt.type != Type_T::Fire
            && field->attr["weather"] != Weather_T::Rainy
            && grave.find(Pet_T::G) == grave.end()) {
                return Pet_T::G;
            }
        }
    }
    return Pet_T::BACK;
}

Skill_T RainyActor::ChooseSkill() {
    // returns chosen skill
    
    if (petOnCourt.pet == Pet_T::B) {
        if (field->attr["weather"] == Weather_T::Sunny) return Skill_T::Rainy;
        // not sunny below this line
        if (opp->petOnCourt.type == Type_T::Thunder
        || (field->attr["weather"] == Weather_T::Rainy && opp->petOnCourt.type == Type_T::Grass)) {
            if (grave.find(Pet_T::G) != grave.end()) { // green in grave
                if (field->attr["weather"] == Weather_T::Rainy && grave.find(Pet_T::Y) == grave.end()) { // rainy && yellow not in grave
                    return Skill_T::BACK;
                } else {
                    return Skill_T::Stream;
                }
            }
            return Skill_T::BACK; // green not in grave
        }
        if (field->attr["weather"] != Weather_T::Sunny && opp->petOnCourt.type == Type_T::Water) {
            return Skill_T::Tackle;
        }
        return field->attr["weather"] == Weather_T::Rainy ? Skill_T::Stream : Skill_T::Rainy;
    }
    
    else if (petOnCourt.pet == Pet_T::G) {
        if (opp->petOnCourt.type == Type_T::Fire) {
            if (grave.find(Pet_T::B) != grave.end()) { // blue in grave
                return petOnCourt.health * 1.0 >= 2.0 / 3.0 * allPets.at(petOnCourt.pet).health
                ? Skill_T::Tackle
                : Skill_T::Synthesis;
            }
            return Skill_T::BACK;
        }
        if (opp->petOnCourt.type == Type_T::Grass) {
            return petOnCourt.health * 1.0 >= 0.5 * allPets.at(petOnCourt.pet).health
            ? Skill_T::Tackle
            : Skill_T::Synthesis;
        }
        if (petOnCourt.health * 1.0 >= 0.5 * allPets.at(petOnCourt.pet).health
        || opp->petOnCourt.health * 1.0 < 0.2 * allPets.at(opp->petOnCourt.pet).health) {
            return Skill_T::Leaf;
        }
        return Skill_T::Synthesis;
    }

    else if (petOnCourt.pet == Pet_T::Y) {
        if (opp->petOnCourt.health * 1.0 < 0.2 * allPets.at(opp->petOnCourt.pet).health) {
            return Skill_T::Quick;
        }
        if (opp->petOnCourt.type == Type_T::Fire) {
            if (grave.find(Pet_T::B) != grave.end()) {
                return Skill_T::Volt;
            }
            return Skill_T::BACK;
        }
        if (opp->petOnCourt.type != Type_T::Fire) {
            if (field->attr["weather"] == Weather_T::Rainy) {
                return Skill_T::Thunder;
            } else {
                if (grave.find(Pet_T::G) != grave.end()) { // green in grave
                    if (opp->petOnCourt.type == Type_T::Thunder || opp->petOnCourt.type == Type_T::Grass) {
                        return Skill_T::Quick;
                    } else {
                        return Skill_T::Volt;
                    }
                }
                return Skill_T::BACK;
            }
        }
    }

    return Skill_T::BACK;
}

PotionInfo RainyActor::ChoosePotion() {
    // returns chosen potion and target
    return PotionInfo{
        Potion_T::Revival,
        // (grave.find(Pet_T::B) != grave.end()) ? Pet_T::B : Pet_T::G
        Pet_T::B,
    };
}

std::string RainyActor::GetName() {
    return "Enemy";//"Rainy Team";
}
