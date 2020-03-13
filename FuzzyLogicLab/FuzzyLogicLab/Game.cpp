#include "Game.h"

Game::Game()
{
	m_window.create(sf::VideoMode(Utility::S_SCREEN_WIDTH, Utility::S_SCREEN_HEIGHT), "Fuzzy Logic", sf::Style::None);
	
	if (!m_enemyForceTexture.loadFromFile("assets\\images\\enemy.png"))
	{
		std::cout << "Error Loading Enemy Texture" << std::endl;
	}
	if (!m_responseForceTexture.loadFromFile("assets\\images\\player.png"))
	{
		std::cout << "Error Loading Player Texture" << std::endl;
	}
	if (!m_font.loadFromFile("assets\\fonts\\comic.ttf"))
	{
		std::cout << "Error Loading Font" << std::endl;
	}

	m_text.setFont(m_font);
	m_text.setFillColor(sf::Color::Black);
	m_text.setCharacterSize(20);
	m_text.setPosition(sf::Vector2f(0, 0));
 
	m_text.setString("Enemy Range :\nEnemy Size :\nPlayer Response Size :");

	m_colourIndicators[0].setSize(sf::Vector2f(Utility::S_SCREEN_WIDTH / 4.0f, Utility::S_SCREEN_HEIGHT));
	m_colourIndicators[1].setSize(sf::Vector2f(Utility::S_SCREEN_WIDTH * (3.0f / 4.0f), Utility::S_SCREEN_HEIGHT));

	m_colourIndicators[0].setPosition(sf::Vector2f(0, 0));
	m_colourIndicators[1].setPosition(sf::Vector2f(Utility::S_SCREEN_WIDTH / 4.0f, 0));

	m_colourIndicators[0].setFillColor(sf::Color(127,255,127,255));
	m_colourIndicators[1].setFillColor(sf::Color(255, 127, 127, 255));
}

void Game::run()
{
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	const float fps{ 60.0f };
	sf::Time timePerFrame = sf::seconds(1.0f / fps);
	while (m_window.isOpen())
	{
		processEvents();
		timeSinceLastUpdate += clock.restart();
		while (timeSinceLastUpdate > timePerFrame)
		{
			timeSinceLastUpdate -= timePerFrame;
			processEvents();
			update(timePerFrame);
		}
		render();
	}
}

void Game::processEvents()
{
	sf::Event event;
	while (m_window.pollEvent(event))
	{
		if (event.type == sf::Event::KeyPressed)
		{
			if (event.key.code == sf::Keyboard::Escape)
			{
				m_window.close();
			}
			else if (event.key.code == sf::Keyboard::Space)
			{
				spawnEnemies();
				fuzzyLogic();
				resetSprites();
			}
		}
		else if (event.type == sf::Event::Closed)
		{
			m_window.close();
		}
	}
}

void Game::update(sf::Time t_deltaTime)
{

}

void Game::render()
{
	m_window.clear(sf::Color::Black);

	for (int index = 0; index < 2; index++)
	{
		m_window.draw(m_colourIndicators[index]);
	}
	m_window.draw(m_text);
 	for (sf::Sprite& playerSprite : m_responseForce)
	{
		m_window.draw(playerSprite);
	}
	for (sf::Sprite& enemySprite : m_enemyForce)
	{
		m_window.draw(enemySprite);
	}

	m_window.display();
}

void Game::spawnEnemies()
{
	std::random_device random;
	std::mt19937 number(random());
	std::uniform_int_distribution<std::mt19937::result_type> rangeOfEnemyRange(1, S_MAX_ENEMY_RANGE);
	std::uniform_int_distribution<std::mt19937::result_type> rangeOfEnemyForce(1, S_MAX_ENEMY_SIZE);

	m_sizeOfEnemies = rangeOfEnemyForce(number);
	m_rangeOfEnemies = rangeOfEnemyRange(number);
}

void Game::fuzzyLogic()
{
	m_enemySizes[static_cast<int>(EnemySizeType::Tiny)] = Fuzzy::FuzzyTriangle(m_sizeOfEnemies, -10, 0, 10);
	m_enemySizes[static_cast<int>(EnemySizeType::Small)] = Fuzzy::FuzzyTrapezoid(m_sizeOfEnemies, 2.5, 10, 15, 20);
	m_enemySizes[static_cast<int>(EnemySizeType::Moderate)] = Fuzzy::FuzzyTrapezoid(m_sizeOfEnemies, 15, 20, 25, 30);
	m_enemySizes[static_cast<int>(EnemySizeType::Large)] = Fuzzy::FuzzyGrade(m_sizeOfEnemies, 25, 30);

	m_enemyRanges[static_cast<int>(EnemyRangeType::Close)] = Fuzzy::FuzzyTriangle(m_rangeOfEnemies, -30, 0, 30);
	m_enemyRanges[static_cast<int>(EnemyRangeType::Medium)] = Fuzzy::FuzzyTrapezoid(m_rangeOfEnemies, 10, 30, 50, 70);
	m_enemyRanges[static_cast<int>(EnemyRangeType::Far)] = Fuzzy::FuzzyGrade(m_rangeOfEnemies, 50, 70);


	// if range is medium and force is tiny or small OR range is far and force is not large
	m_enemyThreatLevels[static_cast<int>(ThreatLevels::Low)] = Fuzzy::OR(Fuzzy::OR(Fuzzy::AND(m_enemyRanges[static_cast<int>(EnemyRangeType::Medium)], m_enemySizes[static_cast<int>(EnemySizeType::Tiny)]),
																				   Fuzzy::AND(m_enemyRanges[static_cast<int>(EnemyRangeType::Medium)], m_enemySizes[static_cast<int>(EnemySizeType::Small)])),
																		 Fuzzy::AND(m_enemyRanges[static_cast<int>(EnemyRangeType::Far)], Fuzzy::NOT(m_enemySizes[static_cast<int>(EnemySizeType::Large)])));
	//if range is close and force is tiny OR range is medium and force is moderate OR range is far and force is Large
	m_enemyThreatLevels[static_cast<int>(ThreatLevels::Medium)] = Fuzzy::OR(Fuzzy::OR(Fuzzy::AND(m_enemyRanges[static_cast<int>(EnemyRangeType::Close)], m_enemySizes[static_cast<int>(EnemySizeType::Tiny)]),
																					  Fuzzy::AND(m_enemyRanges[static_cast<int>(EnemyRangeType::Medium)], m_enemySizes[static_cast<int>(EnemySizeType::Moderate)])),
																			Fuzzy::AND(m_enemyRanges[static_cast<int>(EnemyRangeType::Far)], m_enemySizes[static_cast<int>(EnemySizeType::Large)]));
	//if range is medium and force is large OR if range is close and force is not tiny
	m_enemyThreatLevels[static_cast<int>(ThreatLevels::High)] = Fuzzy::OR(Fuzzy::AND(m_enemyRanges[static_cast<int>(EnemyRangeType::Medium)], m_enemySizes[static_cast<int>(EnemySizeType::Large)]),
																		 (Fuzzy::AND(m_enemyRanges[static_cast<int>(EnemyRangeType::Close)], Fuzzy::NOT(m_enemySizes[static_cast<int>(EnemySizeType::Tiny)]))));


	m_responseForceNumber = ((10 * m_enemyThreatLevels[static_cast<int>(ThreatLevels::Low)]) + (30 * m_enemyThreatLevels[static_cast<int>(ThreatLevels::Medium)]) + (50 * m_enemyThreatLevels[static_cast<int>(ThreatLevels::High)]))
						/ (m_enemyThreatLevels[static_cast<int>(ThreatLevels::Low)] + m_enemyThreatLevels[static_cast<int>(ThreatLevels::Medium)] + m_enemyThreatLevels[static_cast<int>(ThreatLevels::High)]);

	m_responseForceNumber = std::ceil(m_responseForceNumber);
}

void Game::resetSprites()
{
	m_responseForce.clear();
	for (int index = 0; index < m_responseForceNumber; index++)
	{
		sf::Sprite player;
		player.setTexture(m_responseForceTexture);
		player.setPosition(sf::Vector2f(Utility::S_SCREEN_WIDTH / 8.0f, 120 + (index * m_responseForceTexture.getSize().y / 2.0f)));
		player.setPosition(player.getPosition().x + (index % 2 ? -5 : 5), player.getPosition().y);
		m_responseForce.push_back(player);
	}

	m_enemyForce.clear();
	for (int index = 0; index < m_sizeOfEnemies; index++)
	{
		sf::Sprite enemy;
		enemy.setTexture(m_enemyForceTexture);
		enemy.setPosition(sf::Vector2f(Utility::S_SCREEN_WIDTH / 4.0f + (m_rangeOfEnemies / S_MAX_ENEMY_RANGE * (Utility::S_SCREEN_WIDTH * 3.0f/4.0f) ), 120 + (index * m_enemyForceTexture.getSize().y / 2.0f)));
		enemy.setPosition(enemy.getPosition().x + (index % 2 ? -5 : 5), enemy.getPosition().y);
		m_responseForce.push_back(enemy);
	}
	m_text.setString("Press Space To Spawn/Respawn Enemies\nEnemy Range :" + std::to_string(m_rangeOfEnemies) + "\nEnemy Size :" + std::to_string(m_sizeOfEnemies) + "\nPlayer Response Size :" + std::to_string(m_responseForceNumber));
}
