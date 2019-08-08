/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef SPRITEKOOPABLUE_INCLUDED
#define SPRITEKOOPABLUE_INCLUDED

#include <SFML/Graphics.hpp>
#include "sprite_koopa.hpp"

class Sprite_KoopaBlue : public Sprite_Koopa
{
    public :

        Sprite_KoopaBlue(const sf::Texture& sprite_texture, sf::Texture* const shell_texture, Collider* collision, bool shell);

        void secureUpdate();

    private :

        void draw(sf::RenderTarget& target, sf::RenderStates) const;
};

#endif
