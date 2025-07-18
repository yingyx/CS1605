#include "graph.h"

#include <easy2d/easy2d.h>

GraphSlime::GraphSlime(Actor* left, Actor* right, BattleField* bf) {
    // TODO: 初始化图像画面

    this->player = left;
    this->enemy = right;
    this->field = bf;
    this->player_slime = new easy2d::Sprite();
    this->enemy_slime = new easy2d::Sprite();
    this->weather_icon = new easy2d::Sprite();
    this->weather_icon->setPos(550, 0);
    this->player_hp_bar = new easy2d::ShapeNode();
    this->enemy_hp_bar = new easy2d::ShapeNode();
    this->player_hp_text = new easy2d::Text();
    this->enemy_hp_text = new easy2d::Text();
    
    easy2d::DrawingStyle style;
    style.mode = easy2d::DrawingStyle::Mode::Fill;
    style.fillColor = easy2d::Color(0x87cefa);
    style.strokeColor = easy2d::Color(0x43677d);
    style.strokeWidth = 1.0;

    this->player_hp_bar->setDrawingStyle(style);
    this->enemy_hp_bar->setDrawingStyle(style);

    if (easy2d::Game::init("Battle of Slimes 2", 1200, 300)) {

        this->battle_scene = new easy2d::Scene;
        // easy2d::SceneManager::enter(this->battle_scene);

        // init background
        battle_scene->addChild(new easy2d::Sprite("./static/Background.png"));
        
        // init slimes
        this->SetSlimeOnPitch(true, 280, 34);
        this->SetSlimeOnPitch(false, 640, 34);
        battle_scene->addChild(player_slime);
        battle_scene->addChild(enemy_slime);

        // init hp
        this->DrawHpBackground();

        this->SetHPBar(player_slime, this->player->petOnCourt.health, allPets.at(this->player->petOnCourt.pet).health, 331, 236, true);
        this->SetHPBar(enemy_slime, this->enemy->petOnCourt.health, allPets.at(this->enemy->petOnCourt.pet).health, 691, 236, false);

        // init available slimes
        for (const auto &pair : left->availPets) {
            if (pair.first == left->petOnCourt.pet) continue;
            easy2d::Sprite *sprite = new easy2d::Sprite(this->GetSlimeImg(&pair.second, true));
            sprite->setScale(0.15);
            this->battle_scene->addChild(sprite);
            this->player_other_slimes.push_back(sprite);
        }
        this->player_other_slimes[0]->setPos(130, 85);
        this->player_other_slimes[1]->setPos(85, 155);
        for (const auto &pair : right->availPets) {
            if (pair.first == right->petOnCourt.pet) continue;
            easy2d::Sprite *sprite = new easy2d::Sprite(this->GetSlimeImg(&pair.second, false));
            sprite->setScale(0.15);
            this->battle_scene->addChild(sprite);
            this->enemy_other_slimes.push_back(sprite);
        }
        this->enemy_other_slimes[0]->setPos(930, 85);
        this->enemy_other_slimes[1]->setPos(980, 155);

        // init weather
        this->battle_scene->addChild(this->weather_icon);

        // init icons
        for (size_t i = 0; i < 2; i++) {
            auto *sprite1 = new easy2d::Sprite(), *sprite2 = new easy2d::Sprite();
            this->battle_scene->addChild(sprite1);
            this->battle_scene->addChild(sprite2);
            this->player_icons.push_back(sprite1);
            this->enemy_icons.push_back(sprite2);
        }
        
        battle_scene->addChild(player_hp_bar);
        battle_scene->addChild(enemy_hp_bar);
        battle_scene->addChild(player_hp_text);
        battle_scene->addChild(enemy_hp_text);
        
        // easy2d::Game::start();

        // easy2d::Timer::add([=]() {
        //     easy2d::Game::quit();
        // }, 0);
    }
}

void GraphSlime::Update() {
    // TODO: 更新图像画面中的所有内容
    
    this->SetSlimeOnPitch(true, 280, 34);
    this->SetSlimeOnPitch(false, 640, 34);

    this->SetHPBar(player_slime, this->player->petOnCourt.health, allPets.at(this->player->petOnCourt.pet).health, 331, 236, true);
    this->SetHPBar(enemy_slime, this->enemy->petOnCourt.health, allPets.at(this->enemy->petOnCourt.pet).health, 691, 236, false);

    this->DrawOtherSlimes();

    this->DrawWeather();

    this->SetIcons(player_slime, this->player->petOnCourt.attr, 300, 80, true);
    this->SetIcons(enemy_slime, this->enemy->petOnCourt.attr, 900, 80, false);

    easy2d::SceneManager::enter(battle_scene);
    easy2d::Game::start();
    battle_scene->removeAllListeners();

    // 游戏运行后立即返回控制台（图像窗口仍存在）
    easy2d::Timer::add([=]() {
        easy2d::Game::quit();
    }, 0);
}

void GraphSlime::DrawHpBackground() {
    easy2d::DrawingStyle style;
    style.mode = easy2d::DrawingStyle::Mode::Solid;
    style.fillColor = easy2d::Color::Black;

    auto shape = new easy2d::Shape(easy2d::Shape::Rect, easy2d::Rect(easy2d::Point(), easy2d::Size(182, 12)));

    easy2d::ShapeNode *shapeNode1 = new easy2d::ShapeNode(shape, style), *shapeNode2 = new easy2d::ShapeNode(shape, style);

    shapeNode1->setPos(329, 234);
    shapeNode2->setPos(689, 234);

    this->battle_scene->addChild(shapeNode1);
    this->battle_scene->addChild(shapeNode2);
}

void GraphSlime::DrawOtherSlimes() {
    int _id = 0;
    for (const auto &pair : this->player->availPets) {
        if (pair.first == this->player->petOnCourt.pet) continue;
        this->player_other_slimes[_id]->setOpacity(1);
        this->player_other_slimes[_id++]->open(this->GetSlimeImg(&pair.second, true));
    }
    for (; _id < 2; _id++) {
        this->player_other_slimes[_id]->setOpacity(0);
    }
    _id = 0;
    for (const auto &pair : this->enemy->availPets) {
        if (pair.first == this->enemy->petOnCourt.pet) continue;
        this->enemy_other_slimes[_id]->setOpacity(1);
        this->enemy_other_slimes[_id++]->open(this->GetSlimeImg(&pair.second, false));
    }
    for (; _id < 2; _id++) {
        this->enemy_other_slimes[_id]->setOpacity(0);
    }
}

easy2d::Sprite* GraphSlime::SetSlimeOnPitch(bool PorE, float x, float y) {  // 参数PorE表示玩家或敌方
    // TODO: 设置史莱姆图片，调用描绘HP图片和状态图标的函数
    easy2d::Sprite *sprite;
    sprite = PorE ? player_slime : enemy_slime;

    sprite->open(this->GetSlimeImg(PorE ? &this->player->petOnCourt : &this->enemy->petOnCourt, PorE));
    sprite->setScale(0.3);
    sprite->setPos(x, y);

    return sprite;
}

void GraphSlime::SetHPBar(easy2d::Sprite* slimeImg, int hp, int hpMax, float x, float y, bool PorE) {
    // TODO: 描绘HP图片（包括HP文字）
    auto bar = new easy2d::Shape(easy2d::Shape::Rect, easy2d::Rect(easy2d::Point(), easy2d::Size(178.0 * hp / hpMax, 8)));
    auto *hpBar = PorE ? player_hp_bar : enemy_hp_bar;
    hpBar->setShape(bar);
    hpBar->setPos(x, y);

    easy2d::DrawingStyle style;
    style.fillColor = easy2d::Color::Black;

    auto *hpText = PorE ? player_hp_text : enemy_hp_text;
    hpText->setText(std::to_string(hp) + "/" + std::to_string(hpMax));
    hpText->setPos(x + 50, y + 15);
    hpText->setDrawingStyle(style);
}

void GraphSlime::SetIcons(easy2d::Sprite* slimeImg, DefaultedMap<std::string, int, 0>& attr, float x, float y, bool PorE) {
    // TODO: 设置状态图标
    if (PorE) {
        int _id = 0;
        for (const auto &pair : attr) {
            if (pair.first != "faint" && pair.first != "painted") continue;
            if (!pair.second) continue;
            this->player_icons[_id]->setOpacity(1);
            this->player_icons[_id]->setPos(x, y + 30 * _id);
            if (pair.first == "faint") {
                this->player_icons[_id++]->open("./static/IconSleep.png");
            } else if (pair.first == "painted") {
                this->player_icons[_id++]->open("./static/IconPaint.png");
            }
        }
        for (; _id < 2; _id++) {
            this->player_icons[_id]->setOpacity(0);
        }
    }
    else {
        int _id = 0;
        for (const auto &pair : attr) {
            if (pair.first != "faint" && pair.first != "painted") continue;
            if (!pair.second) continue;
            this->enemy_icons[_id]->setOpacity(1);
            this->enemy_icons[_id]->setPos(x, y + 30 * _id);
            if (pair.first == "faint") {
                this->enemy_icons[_id++]->open("./static/IconSleep.png");
            } else if (pair.first == "painted") {
                this->enemy_icons[_id++]->open("./static/IconPaint.png");
            }
            
        }
        for (; _id < 2; _id++) {
            this->enemy_icons[_id]->setOpacity(0);
        }
    }
}

void GraphSlime::DrawWeather() {
    int weather = this->field->attr["weather"];
    if (weather == Weather_T::Normal) {
        this->weather_icon->setOpacity(0);
    }
    else {
        if (weather == Weather_T::Sunny) {
            this->weather_icon->open("./static/Sunny.png"); 
        } else {
            this->weather_icon->open("./static/Rainy.png"); 
        }
        this->weather_icon->setOpacity(1);
    }
}

std::string GraphSlime::GetSlimeImg(const Pet* slime, bool PorE) {
    // TODO: 获取史莱姆图片的文件名
    // true for P, false for E
    std::stringstream ss;
    PRINT(ss, "./static/");
    PRINT(ss, "Slime");
    PRINT(ss, typeName.at(slime->type));
    if (!PorE) PRINT(ss, "_r");
    PRINT(ss, ".png");
    return ss.str();
}

GraphSlime::~GraphSlime() {
    delete battle_scene;
}
