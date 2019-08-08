/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef SPRITESPIKEBRO_INCLUDED
#define SPRITESPIKEBRO_INCLUDED

#include <SFML/Graphics.hpp>
#include <list>
#include "../Miscs/projectile_spike.hpp"
#include "sprite_brother.hpp"

class Sprite_SpikeBro : public Sprite_Brother
{
    public :

        Sprite_SpikeBro(const sf::Texture& texture, Collider* collision);
        virtual ~Sprite_SpikeBro();

        void update();
        void secureUpdate();
        void afterUpdate();

        void thruw();
        void dead();

        sf::Vector2f getPosition() const;
        sf::Vector2f getSize() const;

    private :

        void draw(sf::RenderTarget& target, sf::RenderStates) const;

        std::list<Projectile_Spike*> m_spike;
};

#endif
