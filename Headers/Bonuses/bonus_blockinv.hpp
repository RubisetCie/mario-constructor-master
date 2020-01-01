/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef BLOCKINV_INCLUDED
#define BLOCKINV_INCLUDED

#include <SFML/Graphics.hpp>
#include "../Core/placeable.hpp"
#include "../Core/collider.hpp"

class Bonus_BlockInv : public Placeable
{
    public :

        Bonus_BlockInv(sf::Texture* blockTexture, Collider* collision, char once);

        void setPosition(const sf::Vector2f& pos);

        void update();
        void secureUpdate();
        void afterUpdate();

        sf::Vector2f getPosition() const;
        sf::Vector2f getSize() const;

        ID getID() const;

        virtual void hit() = 0;

        sf::Sprite m_sprite;

        char m_once;
        char m_hit;

    protected :

        Collider* m_collision;

        float m_startheight;
        float m_hitspeed;


};

#endif
