/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef SPRITELAKITUGOLD_INCLUDED
#define SPRITELAKITUGOLD_INCLUDED

#include <SFML/Graphics.hpp>
#include "sprite_lakitu.hpp"

class Sprite_LakituGold : public Sprite_Lakitu
{
    public :

        Sprite_LakituGold(const sf::Texture& texture, Collider* collision);

        void thruw();

    private :

        void draw(sf::RenderTarget& target, sf::RenderStates) const;
};

#endif
