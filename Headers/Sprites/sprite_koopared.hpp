/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef SPRITEKOOPARED_INCLUDED
#define SPRITEKOOPARED_INCLUDED

#include <SFML/Graphics.hpp>
#include "sprite_koopa.hpp"

class Sprite_KoopaRed : public Sprite_Koopa
{
    public :

        Sprite_KoopaRed(const sf::Texture& sprite_texture, sf::Texture* const shell_texture, Collider* collision, bool shell);

        void secureUpdate();

        ID getID() const;

    private :

        void draw(sf::RenderTarget& target, sf::RenderStates) const;
};

#endif
