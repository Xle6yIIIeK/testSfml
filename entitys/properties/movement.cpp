#include "movement.hpp"

Movement::Movement(sf::RectangleShape& sprite, float accel, float decel, float maxVel, const float grid, Entity* self):
	sprite(sprite), acceleration(accel), deceleration(decel), maxVelocity(maxVel), gridSizeF(grid), m_self(self)
{

}

Movement::~Movement()
{

}

sf::Vector2f Movement::getVelocity()
{
	return this->velocity;
}

void Movement::stopVelocity()
{
	this->velocity = sf::Vector2f();
}
void Movement::stopVelocityX()
{
	this->velocity.x = 0.f;
}
void Movement::stopVelocityY()
{
	this->velocity.y = 0.f;
}
void Movement::move(sf::Vector2f direction, const float& deltatime)
{
	this->velocity.x += this->acceleration * direction.x * deltatime;
	this->velocity.y += this->acceleration * direction.y * deltatime;
}

void Movement::update(const float& deltatime, TileMap* map)
{
	if (this->velocity.x > 0.f) //Check for positive x
	{
		//Deceleration
		this->velocity.x -= this->deceleration * deltatime;
		if (this->velocity.x < 0.f)
			this->velocity.x = 0.f;
		//Max velocity check
		if (this->velocity.x > this->maxVelocity)
			this->velocity.x = this->maxVelocity;
	}
	else if (this->velocity.x < 0.f) //Check for negative x
	{
		//Deceleration
		this->velocity.x += deceleration * deltatime;
		if (this->velocity.x > 0.f)
			this->velocity.x = 0.f;
		//Max velocity check
		if (this->velocity.x < -this->maxVelocity)
			this->velocity.x = -this->maxVelocity;
	}

	if (this->velocity.y > 0.f) //Check for positive y
	{
		//Deceleration
		this->velocity.y -= this->deceleration * deltatime;
		if (this->velocity.y < 0.f)
			this->velocity.y = 0.f;
		//Max velocity check
		if (this->velocity.y > this->maxVelocity)
			this->velocity.y = this->maxVelocity;
	}
	else if (this->velocity.y < 0.f) //Check for negative y
	{
		//Deceleration
		this->velocity.y += deceleration * deltatime;
		if (this->velocity.y > 0.f)
			this->velocity.y = 0.f;
		//Max velocity check
		if (this->velocity.y < -this->maxVelocity)
			this->velocity.y = -this->maxVelocity;
	}
	this->sprite.move(velocity.x, velocity.y);
}

void Movement::updateTileCollision(const float& deltatime, TileMap* map)
{
	int fromX = 0,
		fromY = 0,
		toX = 0,
		toY = 0;

	fromX = this->sprite.getPosition().x / this->gridSizeF - 1;
	if (fromX < 0)
		fromX = 0;
	else if (fromX > map->getMapSizeOnTiles().x)
		fromX = map->getMapSizeOnTiles().x;

	toX = this->sprite.getPosition().x / this->gridSizeF + 3;
	if (toX < 0)
		toX = 0;
	else if (toX > map->getMapSizeOnTiles().x)
		toX = map->getMapSizeOnTiles().x;

	fromY = this->sprite.getPosition().y / this->gridSizeF - 1;
	if (fromY < 0)
		fromY = 0;
	else if (fromY > map->getMapSizeOnTiles().y)
		fromY = map->getMapSizeOnTiles().y;

	toY = this->sprite.getPosition().y / this->gridSizeF + 3;
	if (toY < 0)
		toY = 0;
	else if (toY > map->getMapSizeOnTiles().y)
		toY = map->getMapSizeOnTiles().y;

	for (int x = fromX; x < toX; x++)
	{
		for (int y = fromY; y < toY; y++)
		{

			sf::FloatRect playerBounds = this->sprite.getGlobalBounds();
			sf::FloatRect wallBounds = map->getGlobalBounds(x, y);

			if (map->getCollision(x,y))
			{
				//Bottom collision
				if (playerBounds.top < wallBounds.top
					&& playerBounds.top + playerBounds.height < wallBounds.top + wallBounds.height
					&& playerBounds.left < wallBounds.left + wallBounds.width
					&& playerBounds.left + playerBounds.width > wallBounds.left
					)
				{
					this->stopVelocityY();
					this->sprite.setPosition(playerBounds.left, wallBounds.top - playerBounds.height);
				}

				//Top collision
				else if (playerBounds.top > wallBounds.top
					&& playerBounds.top + playerBounds.height > wallBounds.top + wallBounds.height
					&& playerBounds.left < wallBounds.left + wallBounds.width
					&& playerBounds.left + playerBounds.width > wallBounds.left
					)
				{
					this->stopVelocityY();
					this->sprite.setPosition(playerBounds.left, wallBounds.top + wallBounds.height);
				}

				//Right collision
				if (playerBounds.left < wallBounds.left
					&& playerBounds.left + playerBounds.width < wallBounds.left + wallBounds.width
					&& playerBounds.top < wallBounds.top + wallBounds.height
					&& playerBounds.top + playerBounds.height > wallBounds.top
					)
				{
					this->stopVelocityX();
					this->sprite.setPosition(wallBounds.left - playerBounds.width, playerBounds.top);
				}

				//Left collision
				else if (playerBounds.left > wallBounds.left
					&& playerBounds.left + playerBounds.width > wallBounds.left + wallBounds.width
					&& playerBounds.top < wallBounds.top + wallBounds.height
					&& playerBounds.top + playerBounds.height > wallBounds.top
					)
				{
					this->stopVelocityX();
					this->sprite.setPosition(wallBounds.left + wallBounds.width, playerBounds.top);
				}
			}
		}
	}
}

void Movement::updateWorldCollison(const float& deltatime, TileMap* map)
{
	sf::Vector2f bufferPos = this->sprite.getPosition();
	//chek left bound map
	if (this->sprite.getPosition().x < 0.f)
	{
		this->stopVelocityX();
		this->sprite.setPosition(0.f, bufferPos.y);
	}
	else if (bufferPos.x + this->sprite.getGlobalBounds().width > map->getMapSizeOnFloat().x)
	{
		this->stopVelocityX();
		this->sprite.setPosition(map->getMapSizeOnFloat().x - this->sprite.getGlobalBounds().width, bufferPos.y);
	}

	if (this->sprite.getPosition().y < 0.f)
	{
		this->stopVelocityY();
		this->sprite.setPosition(bufferPos.x, 0.f);
	}
	else if (this->sprite.getPosition().y + this->sprite.getGlobalBounds().height > map->getMapSizeOnFloat().y)
	{
		this->stopVelocityY();
		this->sprite.setPosition(bufferPos.x, map->getMapSizeOnFloat().y - this->sprite.getGlobalBounds().height);
	}
}

void Movement::jump(const float& dealtatime)
{
	if (this->m_state == Idle)
	{
		this->velocity.y = (-1) * this->acceleration * 100 * dealtatime;

		this->m_state = Jump;
	}
	else {}
}
