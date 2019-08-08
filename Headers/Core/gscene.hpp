/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef SCENE_INCLUDED
#define SCENE_INCLUDED

class Scene
{
    public :

        static bool Logo();
        static bool Title();
        static bool Ingame();
};

enum Scenes {SCENE_LOGO,
            SCENE_TITLE,
            SCENE_INGAME};

#endif
