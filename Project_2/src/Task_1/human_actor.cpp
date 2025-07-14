#include "actor.h"

#include <sstream>
#include <unordered_set>
#include <algorithm>


HumanActor::HumanActor() {}

std::vector<Pet_T> HumanActor::ChooseStartingPet() {
    LOG("You can select three from five (1 for Green, 2 for Red, 3 for Blue, 4 for Yellow, 5 for Pink).");
    LOG("\n");
    std::vector<Pet_T> chosenPets;
    for (std::string order : {"1st", "2nd", "3rd"}) {
        int petId;
        while (true) {
            LOG("Select your", order, "slime: ");
            std::cin >> petId;
            if (petId >= 1 && petId <= 5
            && std::find(chosenPets.begin(), chosenPets.end(), static_cast<Pet_T>(petId)) == chosenPets.end()) {
                break;
            }
        }
        Pet_T choosenPet = static_cast<Pet_T>(petId);
        chosenPets.push_back(choosenPet);
    }
    allPetTs = chosenPets;
    petOnCourt = allPets.at(chosenPets[0]);
    // availPets[chosenPets[0]] = allPets.at(chosenPets[0]);
    availPets[chosenPets[1]] = allPets.at(chosenPets[1]);
    availPets[chosenPets[2]] = allPets.at(chosenPets[2]);
    return chosenPets;
}

Action_T HumanActor::ChooseAction() {
    // returns chosen action
    std::stringstream ss;
    PRINT(ss, "Select your action (");
    int _id = -1;
    for (const auto &pair : allActions) {
        if (pair.first == Action_T::Change && availPets.empty()) continue;
        if (pair.first == Action_T::Potion) continue;
        if (_id == -1) PRINT(ss, pair.first, "for", pair.second.actionName);
        else PRINT(ss, ",", pair.first, "for", pair.second.actionName);
        ++_id;
    }
    PRINT(ss, "): ");
    int actionId;
    while (true) {
        LOG(ss.str());
        std::cin >> actionId;
        if (actionId >= 0 && actionId <= _id) {
            break;
        }
    }
    return static_cast<Action_T>(actionId);
}

Pet_T HumanActor::ChoosePet(bool active) {
    // returns next pet
    // active: true for <主动交换宠物>, false for <场上宠物阵亡后被动交换宠物>

    // return BACK symbol if no available pets
    if (availPets.empty()) return Pet_T::BACK;

    std::stringstream ss;
    PRINT(ss, active ? "Select your next slime (0 for back" : "Select your next slime (");
    bool printFlag = false;
    for (size_t i = 0; i < allPetTs.size(); i ++) {
        Pet_T pet = allPetTs[i];
        // if (pet == petOnCourt.pet) continue;
        if (availPets.find(pet) == availPets.end()) continue;
        if (active || printFlag) {
            PRINT(ss, ", ");
        } else {
            printFlag = true;
        }
        PRINT(ss, i + 1, "for", availPets.at(pet).petName + "-HP" + std::to_string(availPets.at(pet).health));
    }
    PRINT(ss, "): ");
    int petId;
    while (true) {
        LOG(ss.str());
        std::cin >> petId;
        if (petId == 0 && active) return Pet_T::BACK;
        if (1 <= petId && (size_t) petId <= allPetTs.size()
        && allPetTs[petId - 1] != petOnCourt.pet
        && availPets.find(allPetTs[petId - 1]) != availPets.end()) {
            break;
        }
    }
    return allPetTs[petId - 1];
}

Skill_T HumanActor::ChooseSkill() {
    // returns chosen skill

    std::stringstream ss;
    PRINT(ss, "Select the skill (0 for back");
    int _id = 0;
    for (const Skill_T &skill : petOnCourt.skills) {
        PRINT(ss, ",", ++_id, "for", allSkills.at(skill).skillName);
    }
    PRINT(ss, "): ");
    int skillId;
    while (true) {
        LOG(ss.str());
        std::cin >> skillId;
        if (skillId >= 0 && skillId <= _id) {
            break;
        }
    }
    if (skillId == 0) return Skill_T::BACK;
    return petOnCourt.skills[skillId - 1];
}

std::string HumanActor::GetName() {
    return "You";
}
