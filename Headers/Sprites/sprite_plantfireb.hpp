/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef SPRITEPLANTFIREB_INCLUDED
#define SPRITEPLANTFIREB_INCLUDED

#include <SFML/Graphics.hpp>
#include "../Miscs/projectile_plantfireball.hpp"
#include "sprite_plantb.hpp"

class Sprite_PlantFireB : public Sprite_PlantB
{
    public :

        Sprite_PlantFireB(sf::Texture* texture, Collider* collision);

        void update();
        void secureUpdate();
        void afterUpdate();

    private :

        void draw(sf::RenderTarget& target, sf::RenderStates) const;

        Projectile_PlantFireball m_fire[3];

        bool m_killed;
};

#endif
