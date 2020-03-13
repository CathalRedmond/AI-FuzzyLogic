#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>
#include "Utility.h"
#include "Fuzzy.h"

enum class EnemySizeType
{
	Tiny,
	Small,
	Moderate,
	Large
};

enum class EnemyRangeType
{
	Close,
	Medium,
	Far
};

enum class ThreatLevels
{
	Low,
	Medium,
	High
};

class Game
{
public:
	Game();
	void run();
private:
	void processEvents();
	void update(sf::Time t_deltaTime);
	void render();

	void spawnEnemies();
	void fuzzyLogic();
	void resetSprites();

	static const int S_MAX_ENEMY_SIZE = 30;
	static const int S_MAX_ENEMY_RANGE = 70;

	float m_sizeOfEnemies;
	float m_rangeOfEnemies;

	static const int S_NUMBER_OF_ENEMY_SIZES = 4;
	float m_enemySizes[S_NUMBER_OF_ENEMY_SIZES];

	static const int S_NUMBER_OF_ENEMY_RANGES = 3;
	float m_enemyRanges[S_NUMBER_OF_ENEMY_RANGES];

	static const int S_NUMBER_OF_THREAT_LEVELS = 3;
	float m_enemyThreatLevels[S_NUMBER_OF_THREAT_LEVELS];

	float m_responseForceNumber;

	std::vector<sf::Sprite> m_responseForce;
	std::vector<sf::Sprite> m_enemyForce;

	sf::Texture m_responseForceTexture;
	sf::Texture m_enemyForceTexture;

	sf::RectangleShape m_colourIndicators[2];
	sf::Text m_text;
	sf::Font m_font;

	sf::RenderWindow m_window;
};

