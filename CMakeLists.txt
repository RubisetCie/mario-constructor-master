cmake_minimum_required(VERSION 3.10)
cmake_policy(SET CMP0074 NEW)
cmake_policy(SET CMP0144 NEW)

# Project declaration
project("Mario Constructor Master")

# C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Option declaration
option(BUILD_EDITOR "Whether to build the main level editor" ON)
option(BUILD_PUBLISHER "Whether to build the game executable (to be shared without the editor)" ON)
option(BUILD_TOOLS "Whether to build the tools (level converter & resources remover)" OFF)
option(INSTALL_EXAMPLES "Whether to install example levels & scenarii" ON)

if(NOT WIN32)
    set(QT_MAIN_VERSION "5" CACHE STRING "Set the major Qt version to look to (5 or 6)")
endif()

# Check if the build type is defined
if(NOT CMAKE_BUILD_TYPE)
    message("-- Build type not defined, setting it to Release")
    set(CMAKE_BUILD_TYPE Release)
endif()

# Set custom build flags
if(MSVC)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /DDEBUGMODE")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /D_HAS_EXCEPTIONS=0 /Oy")
else()
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUGMODE")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fno-exceptions -fdelete-null-pointer-checks -fomit-frame-pointer")
endif()

# Find packages
if(BUILD_EDITOR OR BUILD_PUBLISHER)
    list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/Packages")

    find_package(SFML 2.4 COMPONENTS graphics window system REQUIRED)
    find_package(FMODEx REQUIRED)

    if(NOT WIN32)
        find_package(Qt${QT_MAIN_VERSION} COMPONENTS Core Widgets REQUIRED)
        add_definitions(-DQT_NO_KEYWORDS)  # Prevents 'emit' for being a keyword
    endif()
endif()

# Add the executables
if(BUILD_EDITOR)
    set(EDITOR_SRC
        "Sources/Bonuses/bonus_beetblock.cpp"
        "Sources/Bonuses/bonus_beetblockinv.cpp"
        "Sources/Bonuses/bonus_block.cpp"
        "Sources/Bonuses/bonus_blockinv.cpp"
        "Sources/Bonuses/bonus_brick.cpp"
        "Sources/Bonuses/bonus_coin.cpp"
        "Sources/Bonuses/bonus_coinblock.cpp"
        "Sources/Bonuses/bonus_coinblockinv.cpp"
        "Sources/Bonuses/bonus_coinbrick.cpp"
        "Sources/Bonuses/bonus_flowerblock.cpp"
        "Sources/Bonuses/bonus_flowerblockinv.cpp"
        "Sources/Bonuses/bonus_lifeblock.cpp"
        "Sources/Bonuses/bonus_lifeblockinv.cpp"
        "Sources/Bonuses/bonus_luiblock.cpp"
        "Sources/Bonuses/bonus_luiblockinv.cpp"
        "Sources/Bonuses/bonus_mushroomblock.cpp"
        "Sources/Bonuses/bonus_mushroomblockinv.cpp"
        "Sources/Bonuses/bonus_poisonblock.cpp"
        "Sources/Bonuses/bonus_poisonblockinv.cpp"
        "Sources/Bonuses/bonus_starblock.cpp"
        "Sources/Bonuses/bonus_starblockinv.cpp"
        "Sources/Bonuses/bonus_switch.cpp"
        "Sources/Bonuses/bonus_toggle.cpp"
        "Sources/Core/effect.cpp"
        "Sources/Core/matrix.cpp"
        "Sources/Core/pawn.cpp"
        "Sources/Core/placeable.cpp"
        "Sources/Core/platformfall.cpp"
        "Sources/Core/platformmov.cpp"
        "Sources/Core/projectile.cpp"
        "Sources/Graphics/button.cpp"
        "Sources/Graphics/case.cpp"
        "Sources/Graphics/centipedeentity.cpp"
        "Sources/Graphics/checkbutton.cpp"
        "Sources/Graphics/dragger.cpp"
        "Sources/Graphics/elementbutton.cpp"
        "Sources/Graphics/emitter.cpp"
        "Sources/Graphics/entity.cpp"
        "Sources/Graphics/entityj.cpp"
        "Sources/Graphics/entitymultitext.cpp"
        "Sources/Graphics/entityrotation.cpp"
        "Sources/Graphics/entwarps.cpp"
        "Sources/Graphics/fallplatformentity.cpp"
        "Sources/Graphics/horizontalenemyentity.cpp"
        "Sources/Graphics/horizontalplatformentity.cpp"
        "Sources/Graphics/infobutton.cpp"
        "Sources/Graphics/levelendentity.cpp"
        "Sources/Graphics/lightentity.cpp"
        "Sources/Graphics/markerentity.cpp"
        "Sources/Graphics/podoboodentity.cpp"
        "Sources/Graphics/podobooentity.cpp"
        "Sources/Graphics/podoboolentity.cpp"
        "Sources/Graphics/podoboorentity.cpp"
        "Sources/Graphics/regulator.cpp"
        "Sources/Graphics/rotodiscentity.cpp"
        "Sources/Graphics/spritefont.cpp"
        "Sources/Graphics/tile.cpp"
        "Sources/Graphics/trace.cpp"
        "Sources/Graphics/trigger.cpp"
        "Sources/Graphics/verticalenemyentity.cpp"
        "Sources/Graphics/verticalplatformentity.cpp"
        "Sources/Hazards/hazard_cannon.cpp"
        "Sources/Hazards/hazard_cannonhb.cpp"
        "Sources/Hazards/hazard_cannonhoming.cpp"
        "Sources/Hazards/hazard_cannonhominghb.cpp"
        "Sources/Hazards/hazard_cannonhominght.cpp"
        "Sources/Hazards/hazard_cannonhomingvl.cpp"
        "Sources/Hazards/hazard_cannonhomingvr.cpp"
        "Sources/Hazards/hazard_cannonht.cpp"
        "Sources/Hazards/hazard_cannonvl.cpp"
        "Sources/Hazards/hazard_cannonvr.cpp"
        "Sources/Hazards/hazard_centipede.cpp"
        "Sources/Hazards/hazard_lava.cpp"
        "Sources/Hazards/hazard_lavab.cpp"
        "Sources/Hazards/hazard_laval.cpp"
        "Sources/Hazards/hazard_lavalauncher.cpp"
        "Sources/Hazards/hazard_lavalauncherl.cpp"
        "Sources/Hazards/hazard_lavalauncherr.cpp"
        "Sources/Hazards/hazard_lavalaunchervl.cpp"
        "Sources/Hazards/hazard_lavalaunchervr.cpp"
        "Sources/Hazards/hazard_lavar.cpp"
        "Sources/Hazards/hazard_lavat.cpp"
        "Sources/Hazards/hazard_podoboo.cpp"
        "Sources/Hazards/hazard_podoboob.cpp"
        "Sources/Hazards/hazard_podoboofountain.cpp"
        "Sources/Hazards/hazard_podoboofountainb.cpp"
        "Sources/Hazards/hazard_podoboofountainl.cpp"
        "Sources/Hazards/hazard_podoboofountainr.cpp"
        "Sources/Hazards/hazard_podoboofountaint.cpp"
        "Sources/Hazards/hazard_podobool.cpp"
        "Sources/Hazards/hazard_podoboor.cpp"
        "Sources/Hazards/hazard_podoboot.cpp"
        "Sources/Hazards/hazard_rotodisc.cpp"
        "Sources/Hazards/hazard_rotodiscc.cpp"
        "Sources/Hazards/hazard_rotodisccc.cpp"
        "Sources/Hazards/hazard_rotodiscrc.cpp"
        "Sources/Hazards/hazard_rotodiscrcc.cpp"
        "Sources/Hazards/hazard_spike.cpp"
        "Sources/Hazards/hazard_spikelauncher.cpp"
        "Sources/Hazards/hazard_spikelauncherb.cpp"
        "Sources/Hazards/hazard_spikelaunchert.cpp"
        "Sources/Marks/mark_checkpoint.cpp"
        "Sources/Marks/mark_lavaplane.cpp"
        "Sources/Marks/mark_levelend.cpp"
        "Sources/Marks/mark_mario.cpp"
        "Sources/Marks/mark_platformcloudfall.cpp"
        "Sources/Marks/mark_platformcloudmoveh.cpp"
        "Sources/Marks/mark_platformcloudmovev.cpp"
        "Sources/Marks/mark_platformcloudtouchh.cpp"
        "Sources/Marks/mark_platformcloudtouchv.cpp"
        "Sources/Marks/mark_platformredfall.cpp"
        "Sources/Marks/mark_platformredmoveh.cpp"
        "Sources/Marks/mark_platformredmovev.cpp"
        "Sources/Marks/mark_platformredtouchh.cpp"
        "Sources/Marks/mark_platformredtouchv.cpp"
        "Sources/Marks/mark_platformshortfall.cpp"
        "Sources/Marks/mark_platformshortmoveh.cpp"
        "Sources/Marks/mark_platformshortmovev.cpp"
        "Sources/Marks/mark_platformshorttouchh.cpp"
        "Sources/Marks/mark_platformshorttouchv.cpp"
        "Sources/Marks/mark_platformstonefall.cpp"
        "Sources/Marks/mark_platformstonemoveh.cpp"
        "Sources/Marks/mark_platformstonemovev.cpp"
        "Sources/Marks/mark_platformstonetouchh.cpp"
        "Sources/Marks/mark_platformstonetouchv.cpp"
        "Sources/Marks/mark_spring.cpp"
        "Sources/Marks/mark_waterplane.cpp"
        "Sources/Miscs/effect_coin.cpp"
        "Sources/Miscs/effect_fire.cpp"
        "Sources/Miscs/effect_lavasplash.cpp"
        "Sources/Miscs/effect_score.cpp"
        "Sources/Miscs/effect_shard.cpp"
        "Sources/Miscs/effect_splash.cpp"
        "Sources/Miscs/projectile_beet.cpp"
        "Sources/Miscs/projectile_bulletbill.cpp"
        "Sources/Miscs/projectile_enemybeet.cpp"
        "Sources/Miscs/projectile_enemyfireball.cpp"
        "Sources/Miscs/projectile_fireball.cpp"
        "Sources/Miscs/projectile_flame.cpp"
        "Sources/Miscs/projectile_hammer.cpp"
        "Sources/Miscs/projectile_lava.cpp"
        "Sources/Miscs/projectile_plantfireball.cpp"
        "Sources/Miscs/projectile_podoboo.cpp"
        "Sources/Miscs/projectile_silverhammer.cpp"
        "Sources/Miscs/projectile_spike.cpp"
        "Sources/Sceneries/scenery_bush.cpp"
        "Sources/Sceneries/scenery_cloud.cpp"
        "Sources/Sceneries/scenery_scroll.cpp"
        "Sources/Sceneries/scenery_static.cpp"
        "Sources/Sceneries/scenery_waterfall.cpp"
        "Sources/Scenes/editor.cpp"
        "Sources/Scenes/ingame.cpp"
        "Sources/Scenes/logo.cpp"
        "Sources/Scenes/title.cpp"
        "Sources/Scenes/worldeditor.cpp"
        "Sources/Sprites/fallable.cpp"
        "Sources/Sprites/sprite_beet.cpp"
        "Sources/Sprites/sprite_beetbro.cpp"
        "Sources/Sprites/sprite_bowser.cpp"
        "Sources/Sprites/sprite_brother.cpp"
        "Sources/Sprites/sprite_buzzybeetle.cpp"
        "Sources/Sprites/sprite_cheepcheep.cpp"
        "Sources/Sprites/sprite_cheepcheepblue.cpp"
        "Sources/Sprites/sprite_cheepcheepgreen.cpp"
        "Sources/Sprites/sprite_cheepcheepred.cpp"
        "Sources/Sprites/sprite_cheepcheepyellow.cpp"
        "Sources/Sprites/sprite_firebro.cpp"
        "Sources/Sprites/sprite_flower.cpp"
        "Sources/Sprites/sprite_goomba.cpp"
        "Sources/Sprites/sprite_goombagrey.cpp"
        "Sources/Sprites/sprite_hammerbro.cpp"
        "Sources/Sprites/sprite_koopa.cpp"
        "Sources/Sprites/sprite_koopablue.cpp"
        "Sources/Sprites/sprite_koopagreen.cpp"
        "Sources/Sprites/sprite_koopared.cpp"
        "Sources/Sprites/sprite_koopayellow.cpp"
        "Sources/Sprites/sprite_lakitu.cpp"
        "Sources/Sprites/sprite_lakitudark.cpp"
        "Sources/Sprites/sprite_lakitugold.cpp"
        "Sources/Sprites/sprite_lakituwhite.cpp"
        "Sources/Sprites/sprite_life.cpp"
        "Sources/Sprites/sprite_lui.cpp"
        "Sources/Sprites/sprite_mushroom.cpp"
        "Sources/Sprites/sprite_paratroopa.cpp"
        "Sources/Sprites/sprite_paratroopablueh.cpp"
        "Sources/Sprites/sprite_paratroopabluej.cpp"
        "Sources/Sprites/sprite_paratroopabluev.cpp"
        "Sources/Sprites/sprite_paratroopagreenh.cpp"
        "Sources/Sprites/sprite_paratroopagreenj.cpp"
        "Sources/Sprites/sprite_paratroopagreenv.cpp"
        "Sources/Sprites/sprite_paratrooparedh.cpp"
        "Sources/Sprites/sprite_paratrooparedj.cpp"
        "Sources/Sprites/sprite_paratrooparedv.cpp"
        "Sources/Sprites/sprite_paratroopayellow.cpp"
        "Sources/Sprites/sprite_plantb.cpp"
        "Sources/Sprites/sprite_plantfireb.cpp"
        "Sources/Sprites/sprite_plantfirel.cpp"
        "Sources/Sprites/sprite_plantfirer.cpp"
        "Sources/Sprites/sprite_plantfiret.cpp"
        "Sources/Sprites/sprite_plantgreenb.cpp"
        "Sources/Sprites/sprite_plantgreenl.cpp"
        "Sources/Sprites/sprite_plantgreenr.cpp"
        "Sources/Sprites/sprite_plantgreent.cpp"
        "Sources/Sprites/sprite_plantgreyb.cpp"
        "Sources/Sprites/sprite_plantgreyl.cpp"
        "Sources/Sprites/sprite_plantgreyr.cpp"
        "Sources/Sprites/sprite_plantgreyt.cpp"
        "Sources/Sprites/sprite_plantl.cpp"
        "Sources/Sprites/sprite_plantr.cpp"
        "Sources/Sprites/sprite_plantt.cpp"
        "Sources/Sprites/sprite_poison.cpp"
        "Sources/Sprites/sprite_silverbro.cpp"
        "Sources/Sprites/sprite_spikebro.cpp"
        "Sources/Sprites/sprite_spiny.cpp"
        "Sources/Sprites/sprite_spinyball.cpp"
        "Sources/Sprites/sprite_spinygrey.cpp"
        "Sources/Sprites/sprite_spinygreyball.cpp"
        "Sources/Sprites/sprite_star.cpp"
        "Sources/Sprites/sprite_staticplant.cpp"
        "Sources/Sprites/sprite_throwedcoin.cpp"
        "Sources/Sprites/sprite_thwompb.cpp"
        "Sources/Sprites/sprite_thwompt.cpp"
        "Sources/Sprites/walkable.cpp"
        "Sources/main.cpp"
    )

    if(WIN32)
        list(APPEND EDITOR_SRC Resource/resources.rc)
        add_executable(Editor WIN32 ${EDITOR_SRC})
        target_link_libraries(Editor comdlg32 shlwapi)
    else()
        add_executable(Editor ${EDITOR_SRC})
        target_link_libraries(Editor Qt${QT_MAIN_VERSION}::Core Qt${QT_MAIN_VERSION}::Widgets)
        target_compile_definitions(Editor PRIVATE LINUX)
    endif()

    # Set target properties
    if(WIN32)
        set_target_properties(Editor PROPERTIES OUTPUT_NAME "Mario Constructor Master")
    else()
        set_target_properties(Editor PROPERTIES OUTPUT_NAME "MCM-Editor")
    endif()

    # Link to packages
    target_include_directories(Editor SYSTEM PRIVATE "${SFML_INCLUDE_DIR}" "${FMODEX_INCLUDE_DIR}")
    target_link_libraries(Editor "${SFML_LIBRARIES}" "${FMODEX_LIBRARY}")

    # Configure the desktop file
    if(NOT WIN32)
        configure_file("mcm-editor.desktop.in" "mcm-editor.desktop" @ONLY)
        install(FILES "${CMAKE_BINARY_DIR}/mcm-editor.desktop" "mcm-editor.png" DESTINATION "Editor")
    endif()

    # Installation definitions
    install(TARGETS Editor DESTINATION "Editor")
    install(DIRECTORY "Data" DESTINATION "Editor")
    install(FILES "Readme.txt" "Help Manual.pdf" "contextParams.ini" "inputParams.ini" DESTINATION "Editor")
endif()

if(BUILD_PUBLISHER)
    set(GAME_SRC
        "Sources/Bonuses/bonus_beetblock.cpp"
        "Sources/Bonuses/bonus_beetblockinv.cpp"
        "Sources/Bonuses/bonus_block.cpp"
        "Sources/Bonuses/bonus_blockinv.cpp"
        "Sources/Bonuses/bonus_brick.cpp"
        "Sources/Bonuses/bonus_coin.cpp"
        "Sources/Bonuses/bonus_coinblock.cpp"
        "Sources/Bonuses/bonus_coinblockinv.cpp"
        "Sources/Bonuses/bonus_coinbrick.cpp"
        "Sources/Bonuses/bonus_flowerblock.cpp"
        "Sources/Bonuses/bonus_flowerblockinv.cpp"
        "Sources/Bonuses/bonus_lifeblock.cpp"
        "Sources/Bonuses/bonus_lifeblockinv.cpp"
        "Sources/Bonuses/bonus_luiblock.cpp"
        "Sources/Bonuses/bonus_luiblockinv.cpp"
        "Sources/Bonuses/bonus_mushroomblock.cpp"
        "Sources/Bonuses/bonus_mushroomblockinv.cpp"
        "Sources/Bonuses/bonus_poisonblock.cpp"
        "Sources/Bonuses/bonus_poisonblockinv.cpp"
        "Sources/Bonuses/bonus_starblock.cpp"
        "Sources/Bonuses/bonus_starblockinv.cpp"
        "Sources/Bonuses/bonus_switch.cpp"
        "Sources/Bonuses/bonus_toggle.cpp"
        "Sources/Core/effect.cpp"
        "Sources/Core/matrix.cpp"
        "Sources/Core/pawn.cpp"
        "Sources/Core/placeable.cpp"
        "Sources/Core/platformfall.cpp"
        "Sources/Core/platformmov.cpp"
        "Sources/Core/projectile.cpp"
        "Sources/Graphics/spritefont.cpp"
        "Sources/Graphics/trace.cpp"
        "Sources/Hazards/hazard_cannon.cpp"
        "Sources/Hazards/hazard_cannonhb.cpp"
        "Sources/Hazards/hazard_cannonhoming.cpp"
        "Sources/Hazards/hazard_cannonhominghb.cpp"
        "Sources/Hazards/hazard_cannonhominght.cpp"
        "Sources/Hazards/hazard_cannonhomingvl.cpp"
        "Sources/Hazards/hazard_cannonhomingvr.cpp"
        "Sources/Hazards/hazard_cannonht.cpp"
        "Sources/Hazards/hazard_cannonvl.cpp"
        "Sources/Hazards/hazard_cannonvr.cpp"
        "Sources/Hazards/hazard_centipede.cpp"
        "Sources/Hazards/hazard_lava.cpp"
        "Sources/Hazards/hazard_lavab.cpp"
        "Sources/Hazards/hazard_laval.cpp"
        "Sources/Hazards/hazard_lavalauncher.cpp"
        "Sources/Hazards/hazard_lavalauncherl.cpp"
        "Sources/Hazards/hazard_lavalauncherr.cpp"
        "Sources/Hazards/hazard_lavalaunchervl.cpp"
        "Sources/Hazards/hazard_lavalaunchervr.cpp"
        "Sources/Hazards/hazard_lavar.cpp"
        "Sources/Hazards/hazard_lavat.cpp"
        "Sources/Hazards/hazard_podoboo.cpp"
        "Sources/Hazards/hazard_podoboob.cpp"
        "Sources/Hazards/hazard_podoboofountain.cpp"
        "Sources/Hazards/hazard_podoboofountainb.cpp"
        "Sources/Hazards/hazard_podoboofountainl.cpp"
        "Sources/Hazards/hazard_podoboofountainr.cpp"
        "Sources/Hazards/hazard_podoboofountaint.cpp"
        "Sources/Hazards/hazard_podobool.cpp"
        "Sources/Hazards/hazard_podoboor.cpp"
        "Sources/Hazards/hazard_podoboot.cpp"
        "Sources/Hazards/hazard_rotodisc.cpp"
        "Sources/Hazards/hazard_rotodiscc.cpp"
        "Sources/Hazards/hazard_rotodisccc.cpp"
        "Sources/Hazards/hazard_rotodiscrc.cpp"
        "Sources/Hazards/hazard_rotodiscrcc.cpp"
        "Sources/Hazards/hazard_spike.cpp"
        "Sources/Hazards/hazard_spikelauncher.cpp"
        "Sources/Hazards/hazard_spikelauncherb.cpp"
        "Sources/Hazards/hazard_spikelaunchert.cpp"
        "Sources/Marks/mark_checkpoint.cpp"
        "Sources/Marks/mark_lavaplane.cpp"
        "Sources/Marks/mark_levelend.cpp"
        "Sources/Marks/mark_mario.cpp"
        "Sources/Marks/mark_platformcloudfall.cpp"
        "Sources/Marks/mark_platformcloudmoveh.cpp"
        "Sources/Marks/mark_platformcloudmovev.cpp"
        "Sources/Marks/mark_platformcloudtouchh.cpp"
        "Sources/Marks/mark_platformcloudtouchv.cpp"
        "Sources/Marks/mark_platformredfall.cpp"
        "Sources/Marks/mark_platformredmoveh.cpp"
        "Sources/Marks/mark_platformredmovev.cpp"
        "Sources/Marks/mark_platformredtouchh.cpp"
        "Sources/Marks/mark_platformredtouchv.cpp"
        "Sources/Marks/mark_platformshortfall.cpp"
        "Sources/Marks/mark_platformshortmoveh.cpp"
        "Sources/Marks/mark_platformshortmovev.cpp"
        "Sources/Marks/mark_platformshorttouchh.cpp"
        "Sources/Marks/mark_platformshorttouchv.cpp"
        "Sources/Marks/mark_platformstonefall.cpp"
        "Sources/Marks/mark_platformstonemoveh.cpp"
        "Sources/Marks/mark_platformstonemovev.cpp"
        "Sources/Marks/mark_platformstonetouchh.cpp"
        "Sources/Marks/mark_platformstonetouchv.cpp"
        "Sources/Marks/mark_spring.cpp"
        "Sources/Marks/mark_waterplane.cpp"
        "Sources/Miscs/effect_coin.cpp"
        "Sources/Miscs/effect_fire.cpp"
        "Sources/Miscs/effect_lavasplash.cpp"
        "Sources/Miscs/effect_score.cpp"
        "Sources/Miscs/effect_shard.cpp"
        "Sources/Miscs/effect_splash.cpp"
        "Sources/Miscs/projectile_beet.cpp"
        "Sources/Miscs/projectile_bulletbill.cpp"
        "Sources/Miscs/projectile_enemybeet.cpp"
        "Sources/Miscs/projectile_enemyfireball.cpp"
        "Sources/Miscs/projectile_fireball.cpp"
        "Sources/Miscs/projectile_flame.cpp"
        "Sources/Miscs/projectile_hammer.cpp"
        "Sources/Miscs/projectile_lava.cpp"
        "Sources/Miscs/projectile_plantfireball.cpp"
        "Sources/Miscs/projectile_podoboo.cpp"
        "Sources/Miscs/projectile_silverhammer.cpp"
        "Sources/Miscs/projectile_spike.cpp"
        "Sources/Sceneries/scenery_bush.cpp"
        "Sources/Sceneries/scenery_cloud.cpp"
        "Sources/Sceneries/scenery_scroll.cpp"
        "Sources/Sceneries/scenery_static.cpp"
        "Sources/Sceneries/scenery_waterfall.cpp"
        "Sources/Scenes/glogo.cpp"
        "Sources/Scenes/gtitle.cpp"
        "Sources/Scenes/ingame.cpp"
        "Sources/Sprites/fallable.cpp"
        "Sources/Sprites/sprite_beet.cpp"
        "Sources/Sprites/sprite_beetbro.cpp"
        "Sources/Sprites/sprite_bowser.cpp"
        "Sources/Sprites/sprite_brother.cpp"
        "Sources/Sprites/sprite_buzzybeetle.cpp"
        "Sources/Sprites/sprite_cheepcheep.cpp"
        "Sources/Sprites/sprite_cheepcheepblue.cpp"
        "Sources/Sprites/sprite_cheepcheepgreen.cpp"
        "Sources/Sprites/sprite_cheepcheepred.cpp"
        "Sources/Sprites/sprite_cheepcheepyellow.cpp"
        "Sources/Sprites/sprite_firebro.cpp"
        "Sources/Sprites/sprite_flower.cpp"
        "Sources/Sprites/sprite_goomba.cpp"
        "Sources/Sprites/sprite_goombagrey.cpp"
        "Sources/Sprites/sprite_hammerbro.cpp"
        "Sources/Sprites/sprite_koopa.cpp"
        "Sources/Sprites/sprite_koopablue.cpp"
        "Sources/Sprites/sprite_koopagreen.cpp"
        "Sources/Sprites/sprite_koopared.cpp"
        "Sources/Sprites/sprite_koopayellow.cpp"
        "Sources/Sprites/sprite_lakitu.cpp"
        "Sources/Sprites/sprite_lakitudark.cpp"
        "Sources/Sprites/sprite_lakitugold.cpp"
        "Sources/Sprites/sprite_lakituwhite.cpp"
        "Sources/Sprites/sprite_life.cpp"
        "Sources/Sprites/sprite_lui.cpp"
        "Sources/Sprites/sprite_mushroom.cpp"
        "Sources/Sprites/sprite_paratroopa.cpp"
        "Sources/Sprites/sprite_paratroopablueh.cpp"
        "Sources/Sprites/sprite_paratroopabluej.cpp"
        "Sources/Sprites/sprite_paratroopabluev.cpp"
        "Sources/Sprites/sprite_paratroopagreenh.cpp"
        "Sources/Sprites/sprite_paratroopagreenj.cpp"
        "Sources/Sprites/sprite_paratroopagreenv.cpp"
        "Sources/Sprites/sprite_paratrooparedh.cpp"
        "Sources/Sprites/sprite_paratrooparedj.cpp"
        "Sources/Sprites/sprite_paratrooparedv.cpp"
        "Sources/Sprites/sprite_paratroopayellow.cpp"
        "Sources/Sprites/sprite_plantb.cpp"
        "Sources/Sprites/sprite_plantfireb.cpp"
        "Sources/Sprites/sprite_plantfirel.cpp"
        "Sources/Sprites/sprite_plantfirer.cpp"
        "Sources/Sprites/sprite_plantfiret.cpp"
        "Sources/Sprites/sprite_plantgreenb.cpp"
        "Sources/Sprites/sprite_plantgreenl.cpp"
        "Sources/Sprites/sprite_plantgreenr.cpp"
        "Sources/Sprites/sprite_plantgreent.cpp"
        "Sources/Sprites/sprite_plantgreyb.cpp"
        "Sources/Sprites/sprite_plantgreyl.cpp"
        "Sources/Sprites/sprite_plantgreyr.cpp"
        "Sources/Sprites/sprite_plantgreyt.cpp"
        "Sources/Sprites/sprite_plantl.cpp"
        "Sources/Sprites/sprite_plantr.cpp"
        "Sources/Sprites/sprite_plantt.cpp"
        "Sources/Sprites/sprite_poison.cpp"
        "Sources/Sprites/sprite_silverbro.cpp"
        "Sources/Sprites/sprite_spikebro.cpp"
        "Sources/Sprites/sprite_spiny.cpp"
        "Sources/Sprites/sprite_spinyball.cpp"
        "Sources/Sprites/sprite_spinygrey.cpp"
        "Sources/Sprites/sprite_spinygreyball.cpp"
        "Sources/Sprites/sprite_star.cpp"
        "Sources/Sprites/sprite_staticplant.cpp"
        "Sources/Sprites/sprite_throwedcoin.cpp"
        "Sources/Sprites/sprite_thwompb.cpp"
        "Sources/Sprites/sprite_thwompt.cpp"
        "Sources/Sprites/walkable.cpp"
        "Sources/gmain.cpp"
    )

    if(WIN32)
        list(APPEND GAME_SRC Resource/gresources.rc)
        add_executable(Game WIN32 ${GAME_SRC})
        target_link_libraries(Game comdlg32 shlwapi)
    else()
        add_executable(Game ${GAME_SRC})
        target_link_libraries(Game Qt${QT_MAIN_VERSION}::Core Qt${QT_MAIN_VERSION}::Widgets)
        target_compile_definitions(Game PRIVATE LINUX)

        # Set target properties
        set_target_properties(Game PROPERTIES OUTPUT_NAME "MCM-Game")
    endif()

    # Link to packages
    target_compile_definitions(Game PRIVATE PUBLISHER)
    target_include_directories(Game SYSTEM PRIVATE "${SFML_INCLUDE_DIR}" "${FMODEX_INCLUDE_DIR}")
    target_link_libraries(Game "${SFML_LIBRARIES}" "${FMODEX_LIBRARY}")

    # Configure the desktop file
    if(NOT WIN32)
        configure_file("mcm-game.desktop.in" "mcm-game.desktop" @ONLY)
        install(FILES "${CMAKE_BINARY_DIR}/mcm-game.desktop" "mcm-game.png" DESTINATION "Publisher")
    endif()

    # Installation definitions
    install(TARGETS Game DESTINATION "Publisher")
    install(DIRECTORY "Data/Shaders" DESTINATION "Publisher/Data")
    install(FILES
        "Data/Sfx/1Up.wav"
        "Data/Sfx/Beet.wav"
        "Data/Sfx/BowserDammaged.wav"
        "Data/Sfx/BowserFall.wav"
        "Data/Sfx/BowserFire.wav"
        "Data/Sfx/BowserLastDammage.wav"
        "Data/Sfx/Break.wav"
        "Data/Sfx/Bump.wav"
        "Data/Sfx/CannonHoming.wav"
        "Data/Sfx/CannonLava.wav"
        "Data/Sfx/CannonNormal.wav"
        "Data/Sfx/CannonSpike.wav"
        "Data/Sfx/Centipede.wav"
        "Data/Sfx/Coin.wav"
        "Data/Sfx/Death.wav"
        "Data/Sfx/EnterLevel.wav"
        "Data/Sfx/Fireball.wav"
        "Data/Sfx/GameOver.wav"
        "Data/Sfx/Hammer.wav"
        "Data/Sfx/Jump.wav"
        "Data/Sfx/Kick.wav"
        "Data/Sfx/Lakitu1.wav"
        "Data/Sfx/Lakitu2.wav"
        "Data/Sfx/Lakitu3.wav"
        "Data/Sfx/Menu1.wav"
        "Data/Sfx/Menu2.wav"
        "Data/Sfx/PowerDown.wav"
        "Data/Sfx/PowerUp.wav"
        "Data/Sfx/RaisingWater.wav"
        "Data/Sfx/Spring.wav"
        "Data/Sfx/Sprout.wav"
        "Data/Sfx/Stomp.wav"
        "Data/Sfx/Storm.wav"
        "Data/Sfx/Swim.wav"
        "Data/Sfx/Switch.wav"
        "Data/Sfx/TimeCount.wav"
        "Data/Sfx/TimeWarning.wav"
        DESTINATION "Publisher/Data/Sfx")
    install(FILES
        "Data/Sfx/Musics/Apoplexy.mp3"
        "Data/Sfx/Musics/Battle.mp3"
        "Data/Sfx/Musics/Castle.mp3"
        "Data/Sfx/Musics/Castle2.mp3"
        "Data/Sfx/Musics/Cave.mp3"
        "Data/Sfx/Musics/Fastrun1.mp3"
        "Data/Sfx/Musics/Fastrun2.mp3"
        "Data/Sfx/Musics/GhostHouse.mp3"
        "Data/Sfx/Musics/InDaKitchen.mp3"
        "Data/Sfx/Musics/Map1.mp3"
        "Data/Sfx/Musics/Map2.mp3"
        "Data/Sfx/Musics/Map3.mp3"
        "Data/Sfx/Musics/MapCompleted.mp3"
        "Data/Sfx/Musics/Rockland.mp3"
        "Data/Sfx/Musics/SelectSave.mp3"
        "Data/Sfx/Musics/Starland.mp3"
        "Data/Sfx/Musics/Starman.mp3"
        "Data/Sfx/Musics/TitleScreen.mp3"
        "Data/Sfx/Musics/Underground.mp3"
        "Data/Sfx/Musics/Underwater.mp3"
        "Data/Sfx/Musics/Victory.mp3"
        "Data/Sfx/Musics/World1.mp3"
        "Data/Sfx/Musics/YetAnotherBonus.mp3"
        "Data/Sfx/Musics/Yky.mp3"
        "Data/Sfx/Musics/Yoshi1.mp3"
        "Data/Sfx/Musics/Yoshi2.mp3"
        "Data/Sfx/Musics/Yoshi3.mp3"
        DESTINATION "Publisher/Data/Sfx/Musics")
    install(FILES
        "Data/Gfx/CircleFade.bmp"
        "Data/Gfx/Cursor.png"
        "Data/Gfx/Icon.bmp"
        "Data/Gfx/Loading.bmp"
        "Logo.jpg"
        DESTINATION "Publisher/Data/Gfx")
    install(FILES
        "Data/Gfx/TitleScreen/Button_PlayLevel.png"
        "Data/Gfx/TitleScreen/Button_PlayScenario.png"
        "Data/Gfx/TitleScreen/Button_Quit.png"
        "Data/Gfx/TitleScreen/Title.png"
        "Titlescreen.png"
        DESTINATION "Publisher/Data/Gfx/TitleScreen")
    install(DIRECTORY
        "Data/Gfx/Backgrounds"
        "Data/Gfx/Effects"
        "Data/Gfx/Enemies"
        "Data/Gfx/Hazards"
        "Data/Gfx/Hud"
        "Data/Gfx/Items"
        "Data/Gfx/Mario"
        "Data/Gfx/Marks"
        "Data/Gfx/Sceneries"
        "Data/Gfx/Tilesets"
        "Data/Gfx/Worldmap"
        DESTINATION "Publisher/Data/Gfx")
    install(FILES "game.ini" "contextParams.ini" "inputParams.ini" DESTINATION "Publisher")
endif()

if(BUILD_TOOLS)
    add_executable(ToolConverter "Sources/levelconverter.cpp")
    add_executable(ToolRemover "Sources/resourcesremover.cpp")

    # Set target properties
    set_target_properties(ToolConverter PROPERTIES OUTPUT_NAME "LevelConverter")
    set_target_properties(ToolRemover PROPERTIES OUTPUT_NAME "ResourcesRemover")

    # Installation definitions
    install(TARGETS ToolConverter DESTINATION "Tools")
    install(TARGETS ToolRemover DESTINATION "Tools")
endif()

if(INSTALL_EXAMPLES)
    install(DIRECTORY "Levels" "Worlds" DESTINATION "Examples")
endif()
