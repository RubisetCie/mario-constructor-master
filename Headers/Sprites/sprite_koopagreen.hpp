/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef SPRITEKOOPAGREEN_INCLUDED
#define SPRITEKOOPAGREEN_INCLUDED

#include <SFML/Graphics.hpp>
#include "sprite_koopa.hpp"

class Sprite_KoopaGreen : public Sprite_Koopa
{
    public :

        Sprite_KoopaGreen(const sf::Texture& sprite_texture, sf::Texture* const shell_texture, Collider* collision, bool shell);

        void secureUpdate();

    private :

        void draw(sf::RenderTarget& target, sf::RenderStates) const;
};

#endif
