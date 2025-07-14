#include "actor.h"
#include "battlefield.h"


std::vector<Pet_T> SunnyActor::ChooseStartingPet() {
    // 晴天队有PGR，永远首发P
    // availPets[Pet_T::P] = allPets.at(Pet_T::P);
    availPets[Pet_T::G] = allPets.at(Pet_T::G);
    availPets[Pet_T::R] = allPets.at(Pet_T::R);
    petOnCourt = allPets.at(Pet_T::P);
    return {Pet_T::P, Pet_T::G, Pet_T::R};
}

Action_T SunnyActor::ChooseAction() {
    // returns chosen action
    if (petOnCourt.pet == Pet_T::G && petOnCourt.health * 1.0 < 0.5 * allPets.at(Pet_T::G).health) return Action_T::Change;
    return Action_T::Skill;
}

Pet_T SunnyActor::ChoosePet(bool active) {
    // returns next pet
    if (active) return Pet_T::R;
    return Pet_T::G;
}

Skill_T SunnyActor::ChooseSkill() {
    // returns chosen skill
    
    if (petOnCourt.pet == Pet_T::P) {
        if (opp->petOnCourt.health * 1.0 < allPets.at(opp->petOnCourt.pet).health * 1.0 / 3.0
        && (opp->petOnCourt.attr["faint"] || opp->petOnCourt.attr["painted"])) {
            return Skill_T::Slap;
        }
        if (!opp->petOnCourt.attr["faint"]) return Skill_T::Sing;
        // faint below this line
        return opp->petOnCourt.attr["painted"] ? Skill_T::Slap : Skill_T::Paint;
    }
    
    else if (petOnCourt.pet == Pet_T::R) {
        if (opp->petOnCourt.health * 1.0 < allPets.at(opp->petOnCourt.pet).health * 1.0 / 5.0
        && (opp->petOnCourt.type == Type_T::Water || opp->petOnCourt.type == Type_T::Fire)) {
            return Skill_T::Claw;
        }
        return field->attr["weather"] == Weather_T::Sunny ? Skill_T::Flame : Skill_T::Sunny;
    }

    else if (petOnCourt.pet == Pet_T::G) {
        if (petOnCourt.health * 1.0 >= 0.5 * allPets.at(petOnCourt.pet).health
        || (petOnCourt.health > opp->petOnCourt.health && availPets.empty())) {
            return opp->petOnCourt.type == Type_T::Water ? Skill_T::Leaf : Skill_T::Tackle;
        }
        return Skill_T::Synthesis;
    }
}

std::string SunnyActor::GetName() {
    return "Enemy";//"Sunny Team";
}
