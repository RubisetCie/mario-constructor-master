/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef SPRITELAKITUDARK_INCLUDED
#define SPRITELAKITUDARK_INCLUDED

#include <SFML/Graphics.hpp>
#include "sprite_lakitu.hpp"

class Sprite_LakituDark : public Sprite_Lakitu
{
    public :

        Sprite_LakituDark(const sf::Texture& texture, Collider* collision);

        void thruw();

    private :

        void draw(sf::RenderTarget& target, sf::RenderStates) const;
};

#endif
