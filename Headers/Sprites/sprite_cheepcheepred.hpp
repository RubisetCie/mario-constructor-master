/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef SPRITECHEEPCHEEPRED_INCLUDED
#define SPRITECHEEPCHEEPRED_INCLUDED

#include <SFML/Graphics.hpp>
#include "sprite_cheepcheep.hpp"

class Sprite_CheepCheepRed : public Sprite_CheepCheep
{
    public :

        Sprite_CheepCheepRed(const sf::Texture& texture, Collider* collision);

        void secureUpdate();

        ID getID() const;

    private :

        void draw(sf::RenderTarget& target, sf::RenderStates) const;
};

#endif
