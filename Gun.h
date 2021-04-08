#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#define F static_cast<float>

// класс башни, который описывает ее свойства
class Gun : public sf::Drawable
{
private:
	// радиус стрельбы
	float radius_; 
	// скорость стрельбы
	float firing_speed_;
	// цена для улучшения
	float price_upgrade_;

	sf::Vector2f *position_ = new sf::Vector2f;
	sf::Vector2f size_;
	sf::Texture texture_;
	sf::SoundBuffer buffer_;
	sf::Sound *shot_ = new sf::Sound;

	bool *position_fixed_ = new bool(false);

protected:
	// отрисовка пушки
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{
		sf::Sprite sprite;
		sprite.setTexture(texture_);
		sprite.setPosition(*position_);
		sprite.setScale(size_.x / F(texture_.getSize().x), size_.y / F(texture_.getSize().y));
		sprite.setOrigin(size_.x / 2, size_.y / 2);
		target.draw(sprite);
	}

public:
	
	// конструтор для создания пушки
	explicit Gun(const sf::Vector2f size, const float radius = 0, const float firing_speed = 0, const float price_upgrade = 0) :
		size_(size),
		radius_(radius),
		firing_speed_(firing_speed),
		price_upgrade_(price_upgrade)
	{
		texture_.loadFromFile("data/textures/cannon1.png");
	}

	// геттеры и сеттеры
	void setSound()
	{
		buffer_.loadFromFile("sounds/shot.mp3");
		shot_->setBuffer(buffer_);
	}

	void setLevel(int level)
	{
		texture_.loadFromFile("data/textures/cannon" + std::to_string(level) + "png");
	}

	void setRadius(const float radius)
	{
		radius_ = radius;
	}

	float getRadius() const
	{
		return radius_;
	}

	void setFiringSpeed(const float firing_speed)
	{
		firing_speed_ = firing_speed;
	}

	float getFiringSpeed() const
	{
		return firing_speed_;
	}

	void setPriceUpgrade(const float price_upgrade)
	{
		price_upgrade_ = price_upgrade;
	}

	float getPriceUpgrade() const
	{
		return price_upgrade_;
	}

	void setTexture(const sf::Texture& texture)
	{
		texture_ = texture;
	}

	void setPosition(const sf::Vector2i position) const
	{
		position_->x = position.x;
		position_->y = position.y;
	}

	sf::Vector2f getPosition() const
	{
		return *position_;
	}

	void setSize(const sf::Vector2f size)
	{
		size_ = size;
	}

	sf::Vector2f getSize() const
	{
		return size_;
	}

	bool& is_fixed() const
	{
		return *position_fixed_;
	}

	void shot() const
	{
		shot_->play();
	}
};

