/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef SPRITEPLANTFIREL_INCLUDED
#define SPRITEPLANTFIREL_INCLUDED

#include <SFML/Graphics.hpp>
#include "../Miscs/projectile_plantfireball.hpp"
#include "sprite_plantl.hpp"

class Sprite_PlantFireL : public Sprite_PlantL
{
    public :

        Sprite_PlantFireL(sf::Texture* texture, Collider* collision);

        void update();
        void secureUpdate();
        void afterUpdate();

    private :

        void draw(sf::RenderTarget& target, sf::RenderStates) const;

        Projectile_PlantFireball m_fire[3];

        bool m_killed;
};

#endif
