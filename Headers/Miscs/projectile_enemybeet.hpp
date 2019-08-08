/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef PROJECTILEEENEMYBEET_INCLUDED
#define PROJECTILEEENEMYBEET_INCLUDED

#include <SFML/Graphics.hpp>
#include "../Core/projectile.hpp"

class Projectile_EnemyBeet : public Projectile
{
    public :

        Projectile_EnemyBeet(sf::Texture* texture, sf::Vector2f position, sf::Vector2f speed);

        void cast(sf::Vector2f position, bool right);
        void update();

        bool m_destroyed;

    private :

        void draw(sf::RenderTarget& target, sf::RenderStates) const;

        sf::Sprite m_sprite;
        sf::Vector2f m_movedistance;

        unsigned char m_remindings;
};

#endif
