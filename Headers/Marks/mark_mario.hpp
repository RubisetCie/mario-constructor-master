/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef MARKMARIO_INCLUDED
#define MARKMARIO_INCLUDED

#include <SFML/Graphics.hpp>
#include "../Graphics/trace.hpp"
#include "../Graphics/entwarps.hpp"
#include "../Core/pawn.hpp"
#include "../Core/matrix.hpp"
#include "../Miscs/projectile_fireball.hpp"
#include "../Miscs/projectile_beet.hpp"
#include "mark_checkpoint.hpp"

class Mark_Mario : public Pawn, public Trace
{
    public :

        Mark_Mario(sf::Texture* texture[], sf::Texture* invincible_texture);

        void setPosition(const sf::Vector2f& pos);

        void update();
        void secureUpdate();
        void afterUpdate();
        void warpUpdate();
        void deathUpdate();

        void animate();

        void setPower(unsigned char power, bool hurt);

        void moveHorizontal(float dist);

        void fire(int code);
        void bounce(unsigned int maxstrenght, bool spring = false);
        void disorient();

        void pushH(float distance);
        void pushV(float distance);

        void clearCourse();
        void death();

        sf::Vector2f getPosition() const;
        sf::Vector2f getSize() const;

        ID getID() const;

        Mark_CheckPoint* m_checkpoint;
        WarpsData* m_warp;

        Projectile_Fireball m_fireball[2];
        Projectile_Beet     m_beet[2];

        sf::Vector2f m_movedistance;

        unsigned char m_animpowerup;
        unsigned char m_animtrace;

        short m_invincibility;

        bool m_active;

    private :

        void draw(sf::RenderTarget& target, sf::RenderStates) const;

        sf::Sprite m_sprite, m_invincible;
        sf::VertexArray m_warpmario;

        sf::Texture* m_mariotextures[6];

        int m_cleartimer;

        short m_deathtimer;

        bool m_holdjump;
        bool m_bounce;
        bool m_swimming;
        bool m_onfloor;

        unsigned char m_lastpowerup;
        unsigned char m_animpowerups;
        unsigned char m_animspeed;
        unsigned char m_animframes;
        unsigned char m_animtimer;
        unsigned char m_animfire;
        unsigned char m_deltajump;
        unsigned char m_warptimer;

        char m_jumping;
        char m_crouched;
};

#endif
