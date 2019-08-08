/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef SPRITELAKITUWHITE_INCLUDED
#define SPRITELAKITUWHITE_INCLUDED

#include <SFML/Graphics.hpp>
#include "sprite_lakitu.hpp"

class Sprite_LakituWhite : public Sprite_Lakitu
{
    public :

        Sprite_LakituWhite(const sf::Texture& texture, Collider* collision);

        void thruw();

    private :

        void draw(sf::RenderTarget& target, sf::RenderStates) const;
};

#endif
