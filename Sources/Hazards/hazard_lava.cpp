/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>

#ifdef DEBUGMODE
#include <iostream>

using namespace std;
#endif // DEBUGMODE

#include "../../Headers/Hazards/hazard_lava.hpp"
#include "../../Headers/globals.hpp"

using namespace sf;

void Hazard_Lava::update()
{
    if (frame_Lava % 8 == 0)
        m_sprite.setTextureRect(IntRect(0, (frame_Lava/8) * 34, 32, 34));
}

void Hazard_Lava::secureUpdate()
{
}

void Hazard_Lava::afterUpdate()
{
}

ID Hazard_Lava::getID() const
{
    return ID_USELESS;
}
