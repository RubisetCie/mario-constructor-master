/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef SPRITEPARATROOPABLUEJ_INCLUDED
#define SPRITEPARATROOPABLUEJ_INCLUDED

#include <SFML/Graphics.hpp>
#include "sprite_paratroopa.hpp"

class Sprite_ParatroopaBlueJ : public Sprite_Paratroopa
{
    public :

        Sprite_ParatroopaBlueJ(const sf::Texture& sprite_texture, sf::Texture* const turtle_texture, sf::Texture* const shell_texture, Collider* collision);

        void flyUpdate();

        void toTurtle();

        ID getID() const;

    private :

        void draw(sf::RenderTarget& target, sf::RenderStates) const;
};

#endif
