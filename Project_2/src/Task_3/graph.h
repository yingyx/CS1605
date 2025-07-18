#ifndef GRAPH_H
#define GRAPH_H

#include "battlefield.h"
#include <easy2d/easy2d.h>

#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 300
#define SLIME_X_PLAYER 280
#define SLIME_X_ENEMY 640
#define SLIME_Y 30

class GraphSlime {
private:
	easy2d::Scene* battle_scene;
	easy2d::Sprite* player_slime;
	easy2d::Sprite* enemy_slime;
	easy2d::Sprite* weather_icon;
	std::vector<easy2d::Sprite*> player_other_slimes;
	std::vector<easy2d::Sprite*> enemy_other_slimes;
	std::vector<easy2d::Sprite*> player_icons;
	std::vector<easy2d::Sprite*> enemy_icons;
	easy2d::Sprite* weather;
	easy2d::ShapeNode* player_hp_bar;
	easy2d::ShapeNode* enemy_hp_bar;
	easy2d::Text* player_hp_text;
	easy2d::Text* enemy_hp_text;
	Actor* player;
	Actor* enemy;
	BattleField* field;

	void DrawHpBackground();
	void DrawOtherSlimes();
	void DrawWeather();
	easy2d::Sprite* SetSlimeOnPitch(bool PorE, float x, float y);
	void SetHPBar(easy2d::Sprite* slimeImg, int hp, int hpMax, float x, float y, bool PorE);
	void SetIcons(easy2d::Sprite* slimeImg, DefaultedMap<std::string, int, 0>& attr, float x, float y, bool PorE);
	std::string GetSlimeImg(const Pet* slime, bool PorE);
public:
	GraphSlime(Actor* left, Actor* right, BattleField* bf);
	~GraphSlime();
	void Update();
};

#endif //GRAPH_H
