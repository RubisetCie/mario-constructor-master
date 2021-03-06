/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef SPRITEPARATROOPAGREENJ_INCLUDED
#define SPRITEPARATROOPAGREENJ_INCLUDED

#include <SFML/Graphics.hpp>
#include "sprite_paratroopa.hpp"

class Sprite_ParatroopaGreenJ : public Sprite_Paratroopa
{
    public :

        Sprite_ParatroopaGreenJ(const sf::Texture& sprite_texture, sf::Texture* const turtle_texture, sf::Texture* const shell_texture, Collider* collision);

        void flyUpdate();

        void toTurtle();

        ID getID() const;

    private :

        void draw(sf::RenderTarget& target, sf::RenderStates) const;
};

#endif
