# Mario Constructor Master (Source Repository)

*Mario Constructor Master* is a game in which you can create your own customized levels or worldmaps. Everything is easy-to-use to make the level editor very simple. The game is developped in C++ with Code::Blocks IDE.

https://www.youtube.com/watch?v=TVHdkTk4rAo

## Authors

* Matthieu 'Rubisetcie' Carteron (https://www.youtube.com/channel/UCA_MAZN2bUObJRp7UHC85VQ)
* TheMarioVariable (https://www.youtube.com/user/Mariovariable3410)

## Libraries

*Mario Constructor Master* is based on two free libraries. You have to link them in order to compile properly :

* Simple and Fast Multimedia Library ([Website](https://www.sfml-dev.org/index.php))
* FModEx ([Download](https://github.com/alexey-lysiuk/fmodex-zdoom/tree/master/4.26))

## How to Build

*Mario Constructor Master* is built using the CMake subsystem :

```
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=<prefix> -DSFML_ROOT=<sfml-root> -DFMODEX_ROOT=<fmodex-root>
```

The following options are available :

* `BUILD_EDITOR`: To control the build of the main level editor (installed in the "Editor" sub-directory).
* `BUILD_PUBLISHER`: To control the build of the publisher (installed in the "Publisher" sub-directory).

After the configuration, run the simple `make` command:

```
make
make install
```
