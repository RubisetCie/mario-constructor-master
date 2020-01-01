/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef BLOCKTOGGLE_INCLUDED
#define BLOCKTOGGLE_INCLUDED

#include <SFML/Graphics.hpp>
#include "../Core/placeable.hpp"

class Bonus_Toggle : public Placeable
{
    public :

        Bonus_Toggle(sf::Texture* blockTexture, unsigned char slot, bool transparent);

        void setPosition(const sf::Vector2f& pos);

        void update();
        void secureUpdate();
        void afterUpdate();

        void attributeSolid(unsigned char* s);

        sf::Vector2f getPosition() const;
        sf::Vector2f getSize() const;

        ID getID() const;

    private :

        void draw(sf::RenderTarget& target, sf::RenderStates) const;

        sf::Sprite m_sprite;

        unsigned char* m_solid;

        bool m_inverted;

        unsigned char m_slot;
};

#endif
