#pragma once
#include <functional>
#include <iostream>
#include "Gun.h"
#include "Monster.h"

//���� ����
enum class word_types
{
	normal, infinity
};

//����� ����, ������� ����� �������������� ����� � �������
class World final : public sf::Drawable
{
private:
	//����� ������ �����
	unsigned long long* wave_ = new unsigned long long(0);

	//������������ ����� �� ���������
	unsigned long long wave_max_ = 5;

	//������ ��������
	std::vector<Monster>* monsters_ = new std::vector<Monster>;
	//������ �����
	std::vector<Gun>* guns_ = new std::vector<Gun>;
	// �������� ��������� �������
	int spawn_speed_ = 1;
	// ���������� �������� �� ������ �����
	int mob_list_[5] = { 3, 7, 12, 16, 20 };
	//int mob_list_sum_spawns_[5] = {1, 3, 6, 8, 10};
	sf::Window* window_;
	sf::Texture world_texture_;

public:

	// ���������� ��� ������
	explicit World(const word_types type, sf::Window* window)
	{
		if (type == word_types::infinity)
		{
			wave_max_ = ULLONG_MAX;
		}

		window_ = window;

		world_texture_.loadFromFile("data/textures/map.png");

		newWave();
	}

	// ��������� ��� �������� ����� ����� �����
	~World()
	{
		delete wave_;
		delete monsters_;
		delete guns_;
	}



	class
	{
	public:
		sf::Texture texture;

		void setTexture(const std::string& path)
		{
			texture.loadFromFile(path);
		}

		// �����, �� ������� ���������� ������
		sf::Vector2f move_zone[6] = { { 430, 15}, {430, 208}, {790, 208}, {790, 615}, {610, 615}, {610, 680} };

		// ���������� ����� ���������������, ���� ������ ������� ����� �.� ������
		std::vector<sf::Vector2<sf::Vector2f>>* busy_zone = new  std::vector<sf::Vector2<sf::Vector2f>>(
			{
				{{370, 0}, {500, 300}},
				{{500, 170}, {850, 300}},
				{{730, 300}, {845, 575}},
				{{550, 575}, {865, 720}},
			});
	} map;

public:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{
		mobMove();

		// ������ ������� ��������
		std::vector<bool> is_all_dead;
		for (Monster const& monster : *monsters_)
		{
			is_all_dead.push_back(monster.isDead());
		}

		std::vector<bool> is_all_dead_unique;

		std::unique_copy(is_all_dead.begin(), is_all_dead.end(), std::back_inserter(is_all_dead_unique));

		// ���� ��� ������, �� ��������� �� ������ �����
		if (is_all_dead_unique.size() == 1)
		{
			*wave_ += 1;
		}

		// ���������� ����� �� ����� �������
		for (Gun const& gun : *guns_)
		{
			if (!gun.is_fixed())
			{
				gun.setPosition(sf::Mouse::getPosition(*window_));

				if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
				{
					if (std::function<bool()>([=, &gun]()->bool
						{
							// ������ �� ���������� �� ������, ��� �������� �������
							for (sf::Vector2<sf::Vector2f> const& zone : *map.busy_zone)
							{
								if (gun.getPosition().x > zone.x.x && gun.getPosition().x < zone.y.x && gun.getPosition().y > zone.x.y && gun.getPosition().y < zone.y.y)
								{
									return false;
								}

								if (gun.getPosition().x + gun.getSize().x > zone.x.x && gun.getPosition().x + gun.getSize().x < zone.y.x && gun.getPosition().y > zone.x.y && gun.getPosition().y < zone.y.y)
								{
									return false;
								}

								if (gun.getPosition().x > zone.x.x && gun.getPosition().x < zone.y.x && gun.getPosition().y + gun.getSize().y > zone.x.y && gun.getPosition().y + gun.getSize().y < zone.y.y)
								{
									return false;
								}

								if (gun.getPosition().x + gun.getSize().x > zone.x.x && gun.getPosition().x + gun.getSize().x < zone.y.x && gun.getPosition().y + gun.getSize().y > zone.x.y && gun.getPosition().y + gun.getSize().y < zone.y.y)
								{
									return false;
								}

							}
							return true;

						})())
					{
						gun.is_fixed() = true;
						map.busy_zone->push_back(sf::Vector2<sf::Vector2f>
							(gun.getPosition(),
								sf::Vector2f(gun.getPosition().x
									+ gun.getSize().x, gun.getPosition().y
									+ gun.getSize().y)));

					}
				}
			}
		}

		sf::Sprite world_sprite;
		world_sprite.setTexture(world_texture_);
		world_sprite.setScale(F(target.getSize().x) / F(world_texture_.getSize().x), F(target.getSize().y) / F(world_texture_.getSize().y));
		target.draw(world_sprite);

		// ��������� ����� � �����
		for (Monster const& monster : *monsters_)
		{
			target.draw(monster);
		}

		for (Gun const& gun : *guns_)
		{
			target.draw(gun);
		}

	}


	// �������� ����� �����
	void newWave() const
	{
		monsters_->clear();
		if (*wave_ < wave_max_)
		{
			std::cout << *wave_ << std::endl;
			for (auto i = 0; i < *wave_ + 1; i++)
			{
				monsters_->push_back(Monster(monster_type::green, map.move_zone[0]));
			}
		}

	}

	// ������������ �������� �� �����
	void mobMove() const
	{
		for (Monster const& monster : *monsters_)
		{
			if (!monster.isDead())
			{
				if (*monster.move_enum < 5)
				{
					monster.move((map.move_zone[*monster.move_enum + 1].x - map.move_zone[*monster.move_enum].x) * (monster.levels.speed / 1000), (map.move_zone[*monster.move_enum + 1].y - map.move_zone[*monster.move_enum].y) * (monster.levels.speed / 1000));
					*monster.steps += 1;
					if (*monster.steps > 1000 / monster.levels.speed)
					{
						*monster.move_enum += 1;
						*monster.steps = 0;
					}
				}
				else
				{
					monster.isDead() = true;
				}
			}
		}
	}

	// �������� ����� �����
	void newGun() const
	{
		guns_->push_back(Gun({ 100, 100 }));
	}
};