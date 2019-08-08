/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef SPRITECHEEPCHEEPGREEN_INCLUDED
#define SPRITECHEEPCHEEPGREEN_INCLUDED

#include <SFML/Graphics.hpp>
#include "sprite_cheepcheep.hpp"

class Sprite_CheepCheepGreen : public Sprite_CheepCheep
{
    public :

        Sprite_CheepCheepGreen(const sf::Texture& texture, Collider* collision);

        void secureUpdate();

        ID getID() const;

    private :

        void draw(sf::RenderTarget& target, sf::RenderStates) const;
};

#endif
