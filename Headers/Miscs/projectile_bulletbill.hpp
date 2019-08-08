/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef PROJECTILEBULLETBILL_INCLUDED
#define PROJECTILEBULLETBILL_INCLUDED

#include <SFML/Graphics.hpp>

class Projectile_Bulletbill : public sf::Drawable
{
    public :

        Projectile_Bulletbill(sf::Texture* texture, sf::Vector2f position, sf::Vector2f speed, bool inverted, bool chasing = false);

        void update();

        sf::FloatRect m_aabb;

        bool m_destroyed;

    private :

        void draw(sf::RenderTarget& target, sf::RenderStates) const;

        sf::Sprite m_sprite;
        sf::Vector2f m_movedistance;

        bool m_chasing, m_killed;

        unsigned char m_frame;
};

#endif
