cmake_minimum_required(VERSION 3.16)
project(Arena VERSION 1.0.0)

set (CMAKE_C_FLAGS "--std=c11 ${CMAKE_C_FLAGS}")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS OFF)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/build)

add_executable(Arena 
    render/button.c
    render/button.h
    render/graphicsettings.h
    render/render_text.c
    render/render_text.h
    mkb/mkb.c
    mkb/mkb.h
    gst/info.h
    gst/info.c
    gst/fxs.c
    gst/fxs.h
    gst/gst.c
    gst/gst.h
    gst/map.c
    gst/map.h
    gst/units.c
    gst/units.h
    hud/hud.c
    hud/hud.h
    hud/hud_views.c
    hud/hud_views.h
    json/jsmn.h
    json/jsonparse.c
    json/jsonparse.h
    net/net.c
    net/net.h
    umath/vec.c
    umath/vec.h
    umath/intersect.c
    umath/intersect.h
    main.c)

target_include_directories(Arena PUBLIC "C:/MinGW_libs/include")
target_include_directories(Arena PUBLIC "C:/MinGW_libs/include/plibsys")
target_link_libraries(Arena -LC:/MinGW_libs/lib)
target_link_libraries(Arena -lSDL2main -lSDL2 -lSDL2_mixer -lplibsys)