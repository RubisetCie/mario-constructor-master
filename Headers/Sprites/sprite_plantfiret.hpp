/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef SPRITEPLANTFIRET_INCLUDED
#define SPRITEPLANTFIRET_INCLUDED

#include <SFML/Graphics.hpp>
#include "../Miscs/projectile_plantfireball.hpp"
#include "sprite_plantt.hpp"

class Sprite_PlantFireT : public Sprite_PlantT
{
    public :

        Sprite_PlantFireT(sf::Texture* texture, Collider* collision);

        void update();
        void secureUpdate();
        void afterUpdate();

    private :

        void draw(sf::RenderTarget& target, sf::RenderStates) const;

        Projectile_PlantFireball m_fire[3];

        bool m_killed;
};

#endif
