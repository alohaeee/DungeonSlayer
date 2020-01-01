TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        event/events.cpp \
        main.cpp \
        storage.cpp \
        tools/sparceset.cpp

HEADERS += \
    component.h \
    ecs/component.h \
    ecs/entity.h \
    ecs/family.h \
    ecs/storage.h \
    entity.h \
    event/events.h \
    family.h \
    storage.h \
    tools/sparceset.h
