/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef PROJECTILEFLAME_INCLUDED
#define PROJECTILEFLAME_INCLUDED

#include <SFML/Graphics.hpp>

class Projectile_Flame : public sf::Drawable
{
    public :

        Projectile_Flame(sf::Texture* texture, sf::Vector2f position, float speed, float height, bool chasing = false);

        void update();

        sf::FloatRect m_aabb;

        bool m_destroyed;

    private :

        void draw(sf::RenderTarget& target, sf::RenderStates) const;

        sf::Sprite m_sprite;
        sf::Vector2f m_movedistance;

        float m_targetheight;

        bool m_chasing;
};

#endif
