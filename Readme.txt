- Thank you for download : Mario Constructor Master ! -

Description		:

	Mario Constructor Master is an unofficial level/game constructor for Mario Forever.
	Mario Forever is the property of Softendo 	: http://www.softendo.com/
	Mario is the property of Nintendo			: https://www.nintendo.com/

	We hope you will enjoy the game !
	If you have any question or problem found, you can send me an email at :
	"rubisetcie@gmail.com" (without the quotes).

The publisher	:

	The "publisher" is a way to share a stand-alone executable of your game without sharing
	the editor itself.

	To create executable :
		- Unzip "Publisher.zip" in a full new directory.
		- Edit the file "game.ini" with a text editor (like notepad).
		- Set the "playable_type" parameter to match your requirements :
			0 : to load a Level
			1 : to load a Scenario (Suite or Compiled)
			2 : to load a World map
		- Set the "playable_file" parameter to match the path of your playable (e.g : "Levels/(levelname.cml)").
		- You are free to replace any graphics or sounds to your new executable (all assets are in "Data").

The resources remover :

	The "resources remover" is a little tool that clear all the custom resources from a level, a scenario or a world map.
	Drag and drop the file you want to clear into "ResourcesRemover.exe". Use this tool if you have trouble to load a level
	because of corrupted custom resources files path.

Change Controls	:

	Edit the file "inputParams.ini" and change the number right to the assigned control to bind another key :
		A	: 0			R	 : 17			Num8	  : 34			Equal	  :	55			Numpad1	: 76
		B	: 1			S	 : 18			Num9	  : 35			Dash	  :	56			Numpad2	: 77
		C	: 2			T	 : 19			Escape	  : 36			Space	  :	57			Numpad3	: 78
		D	: 3			U	 : 20			LControl  :	37			Return	  :	58			Numpad4	: 79
		E	: 4			V	 : 21			LShift	  :	38			Backspace :	59			Numpad5	: 80
		F	: 5			W	 : 22			LAlt	  :	39			Tab		  :	60			Numpad6	: 81
		G	: 6			X	 : 23			LSystem	  :	40			PageUp	  :	61			Numpad7	: 82
		H	: 7			Y	 : 24			Menu	  :	45			PageDown  :	62			Numpad8	: 83
		I	: 8			Z	 : 25			LBracket  :	46			End		  :	63			Numpad9	: 84
		J	: 9			Num0 : 26			RBracket  :	47			Home	  :	64			Left	: 71
		K	: 10		Num1 : 27			SemiColon :	48			Insert	  :	65			Right	: 72
		L	: 11		Num2 : 28			Comma	  :	49			Delete	  :	66			Up		: 73
		M	: 12		Num3 : 29			Period	  :	50			Add		  :	67			Down	: 74
		N	: 13		Num4 : 30			Quote	  :	51			Subtract  : 68
		O	: 14		Num5 : 31			Slash	  :	52			Multiply  : 69
		P	: 15		Num6 : 32			Backshash :	53			Divide	  : 70
		Q	: 16		Num7 : 33			Tilde	  :	54			Numpad0	  : 75

Troubleshoots 	:

	If too fast :

	In some old computers, the GPU won't be compatible or won't allow Vertical Synchronization.
	To solve this problem, open the file "contextParams.ini" (same directory) :
		Then, set the line "vsync = 1" to "vsync = 0" (without the quotes).

	If too slow (Windows 10 users) :

	In some Windows 10 computers, the OS will launch the game without Hardware Acceleration.
	To solve this problem, launch the game by :
		Right-click -> Launch with the graphic driver : -> (then, choose another option than the default one).

Authors			:

	Rubisetcie			: Programming and Project leader.
						  https://www.youtube.com/user/rubisetcie

	Mariovariable3410	: Graphics assets and beta-testing.
						  https://www.youtube.com/user/Mariovariable3410

	Radel1996			: For some useful remarks and beta-testing.
						  https://www.youtube.com/user/radel999

Thrid-Party		:

	SFML	: A C++ graphic library using OpenGL, created by Laurent Gomila.
			  http://www.sfml-dev.org/

	FMODex	: A proprietary audio library.
			  http://www.fmod.org/

	MinGW	: Minimalist GNU for Windows.
			  http://www.mingw.org/
