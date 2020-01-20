TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt


SOURCES += \
        main.cpp \
        ssecs/sdl/instances.cpp \
        systems/camerasystem.cpp \
        systems/collisionsystem.cpp \
        systems/debugsystem.cpp \
        systems/playerinputsystem.cpp \
        systems/positionsystem.cpp \
        systems/spritesystem.cpp \
        systems/tilegridsystem.cpp \
        xml/pugixml.cpp

HEADERS += \
    core.hpp \
    game_scene.hpp \
    ssecs/component/camera.hpp \
    ssecs/component/colliders.hpp \
    ssecs/component/components.hpp \
    ssecs/component/sprite.hpp \
    ssecs/component/tilegrid.hpp \
    ssecs/component/tileset.hpp \
    ssecs/component/transform.hpp \
    ssecs/component/vector2d.hpp \
    ssecs/config.hpp \
    ssecs/ecs/config.h \
    ssecs/ecs/config.h \
    ssecs/ecs/ecs.hpp \
    ssecs/ecs/entity.hpp \
    ssecs/ecs/family.hpp \
    ssecs/ecs/fwd.hpp \
    ssecs/ecs/registry.hpp \
    ssecs/ecs/sparse_set.hpp \
    ssecs/ecs/storage.hpp \
    ssecs/ecs/type_traits.hpp \
    ssecs/ecs/utility.hpp \
    ssecs/ecs/view.hpp \
    ssecs/fwd.hpp \
    ssecs/game_engine.hpp \
    ssecs/resource/cache.hpp \
    ssecs/resource/fwd.hpp \
    ssecs/sdl/camera.hpp \
    ssecs/sdl/components.hpp \
    ssecs/sdl/config.hpp \
    ssecs/sdl/events.hpp \
    ssecs/sdl/framerate.hpp \
    ssecs/sdl/fwd.hpp \
    ssecs/sdl/game.hpp \
    ssecs/sdl/graphics.hpp \
    ssecs/sdl/instances.hpp \
    ssecs/sdl/scene.hpp \
    ssecs/sdl/sdl.hpp \
    ssecs/sdl/texture_loader.hpp \
    ssecs/sdl/tileset.hpp \
    ssecs/sdl/timer.hpp \
    ssecs/sdl/vector2d.hpp \
    ssecs/ssecs.hpp \
    systems.hpp \
    systems/camerasystem.hpp \
    systems/collisionsystem.hpp \
    systems/debugsystem.hpp \
    systems/playerinputsystem.hpp \
    systems/positionsystem.hpp \
    systems/spritesystem.hpp \
    systems/systems.hpp \
    systems/tilegridsystem.hpp \
    xml/pugiconfig.hpp \
    xml/pugixml.hpp


# SDL2
LIBS += -LC:/SDL2/SDL2-2.0.10/lib/x86 -lSDL2 -lSDL2main
INCLUDEPATH += C:/SDL2/SDL2-2.0.10/include

# SDL2_ttf
LIBS += -LC:/SDL2/SDL2_ttf-2.0.15/lib/x86 -lSDL2_ttf
INCLUDEPATH += C:/SDL2/SDL2_ttf-2.0.15/include

# SDL2_image
LIBS += -LC:/SDL2/SDL2_image-2.0.5/lib/x86 -lSDL2_image
INCLUDEPATH += C:/SDL2/SDL2_image-2.0.5/include

