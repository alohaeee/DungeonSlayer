TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt


SOURCES += \
        source/main.cpp \
        source/sdl/instances.cpp \
        source/systems/camerasystem.cpp \
        source/systems/collisionsystem.cpp \
        source/systems/debugsystem.cpp \
        source/systems/enemysystem.cpp \
        source/systems/playerinputsystem.cpp \
        source/systems/positionsystem.cpp \
        source/systems/spritesystem.cpp \
        source/systems/tilegridsystem.cpp \
        xml/pugixml.cpp

HEADERS += \
    core.hpp \
    entt/config/config.h \
    entt/config/version.h \
    entt/core/algorithm.hpp \
    entt/core/attribute.h \
    entt/core/family.hpp \
    entt/core/hashed_string.hpp \
    entt/core/ident.hpp \
    entt/core/monostate.hpp \
    entt/core/type_info.hpp \
    entt/core/type_traits.hpp \
    entt/core/utility.hpp \
    entt/entity/actor.hpp \
    entt/entity/entity.hpp \
    entt/entity/fwd.hpp \
    entt/entity/group.hpp \
    entt/entity/helper.hpp \
    entt/entity/observer.hpp \
    entt/entity/registry.hpp \
    entt/entity/runtime_view.hpp \
    entt/entity/snapshot.hpp \
    entt/entity/sparse_set.hpp \
    entt/entity/storage.hpp \
    entt/entity/utility.hpp \
    entt/entity/view.hpp \
    entt/entt.hpp \
    entt/fwd.hpp \
    entt/locator/locator.hpp \
    entt/meta/factory.hpp \
    entt/meta/meta.hpp \
    entt/meta/policy.hpp \
    entt/process/process.hpp \
    entt/process/scheduler.hpp \
    entt/resource/cache.hpp \
    entt/resource/fwd.hpp \
    entt/resource/handle.hpp \
    entt/resource/loader.hpp \
    entt/signal/delegate.hpp \
    entt/signal/dispatcher.hpp \
    entt/signal/emitter.hpp \
    entt/signal/fwd.hpp \
    entt/signal/sigh.hpp \
    game_scene.hpp \
    source/component/ai.hpp \
    source/component/camera.hpp \
    source/component/colliders.hpp \
    source/component/components.hpp \
    source/component/hierachy.hpp \
    source/component/sprite.hPP \
    source/component/tilegrid.hpp \
    source/component/tileset.hpp \
    source/component/transform.hpp \
    source/component/vector2d.hpp \
    source/config.hpp \
    source/core.hpp \
    source/fwd.hpp \
    source/game_scene.hpp \
    source/resource/cache.hpp \
    source/resource/fwd.hpp \
    source/sdl/config.hpp \
    source/sdl/events.hpp \
    source/sdl/framerate.hpp \
    source/sdl/fwd.hpp \
    source/sdl/game.hpp \
    source/sdl/graphics.hpp \
    source/sdl/instances.hpp \
    source/sdl/scene.hpp \
    source/sdl/sdl.hpp \
    source/sdl/texture_loader.hpp \
    source/sdl/timer.hpp \
    source/systems/camerasystem.hpp \
    source/systems/collisionsystem.hpp \
    source/systems/debugsystem.hpp \
    source/systems/enemysystem.hpp \
    source/systems/playerinputsystem.hpp \
    source/systems/positionsystem.hpp \
    source/systems/spritesystem.hpp \
    source/systems/systems.hpp \
    source/systems/tilegridsystem.hpp \
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

DISTFILES += \
    resources/fonts/dpcomic.ttf \
    resources/fonts/license.txt \
    resources/score.txt \
    resources/sprites/spritesheet.png \
    resources/sprites/tilemap.png \
    resources/tiled_files/map.tmx \
    resources/tiled_files/sprite.tsx \
    resources/tiled_files/tile.tsx

