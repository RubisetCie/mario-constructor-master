/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef SPRITEPLANTFIRER_INCLUDED
#define SPRITEPLANTFIRER_INCLUDED

#include <SFML/Graphics.hpp>
#include "../Miscs/projectile_plantfireball.hpp"
#include "sprite_plantr.hpp"

class Sprite_PlantFireR : public Sprite_PlantR
{
    public :

        Sprite_PlantFireR(sf::Texture* texture, Collider* collision);

        void update();
        void secureUpdate();
        void afterUpdate();

    private :

        void draw(sf::RenderTarget& target, sf::RenderStates) const;

        Projectile_PlantFireball m_fire[3];

        bool m_killed;
};

#endif
