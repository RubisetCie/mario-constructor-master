/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef SPRITECHEEPCHEEPYELLOW_INCLUDED
#define SPRITECHEEPCHEEPYELLOW_INCLUDED

#include <SFML/Graphics.hpp>
#include "../Core/placeable.hpp"
#include "gotcollision.hpp"

class Sprite_CheepCheepYellow : public GotCollision, public Placeable
{
    public :

        Sprite_CheepCheepYellow(const sf::Texture& texture, Collider* collision);

        void setPosition(const sf::Vector2f& pos);

        void update();
        void secureUpdate();
        void afterUpdate();

        sf::Vector2f getPosition() const;
        sf::Vector2f getSize() const;

        ID getID() const;

        sf::Vector2f m_movedistance;

        sf::FloatRect m_aabb;

    private :

        void draw(sf::RenderTarget& target, sf::RenderStates) const;

        sf::Sprite m_sprite;

        bool m_killed, m_swimming, m_active;
};

#endif
