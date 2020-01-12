TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt


SOURCES += \
        main.cpp \
        systems.cpp

HEADERS += \
    components.hpp \
    game_process.hpp \
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
    ssecs/sdl/components.hpp \
    ssecs/sdl/config.hpp \
    ssecs/sdl/core_data.hpp \
    ssecs/sdl/fwd.hpp \
    ssecs/sdl/game_engine.hpp \
    ssecs/sdl/game_engine.hpp \
    ssecs/sdl/loop_process.hpp \
    ssecs/sdl/sdl.hpp \
    ssecs/sdl/texture_loader.hpp \
    ssecs/sdl/timer.hpp \
    ssecs/ssecs.hpp \
    systems.hpp


# SDL2
LIBS += -LC:/SDL2/SDL2-2.0.10/lib/x86 -lSDL2 -lSDL2main
INCLUDEPATH += C:/SDL2/SDL2-2.0.10/include

# SDL2_ttf
LIBS += -LC:/SDL2/SDL2_ttf-2.0.15/lib/x86 -lSDL2_ttf
INCLUDEPATH += C:/SDL2/SDL2_ttf-2.0.15/include

# SDL2_image
LIBS += -LC:/SDL2/SDL2_image-2.0.5/lib/x86 -lSDL2_image
INCLUDEPATH += C:/SDL2/SDL2_image-2.0.5/include
