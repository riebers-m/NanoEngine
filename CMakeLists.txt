cmake_minimum_required(VERSION 3.28)
project(custom_game_engine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(cmake/CPM.cmake)

message(STATUS "prefix path: ${CMAKE_PREFIX_PATH}")

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_mixer REQUIRED)

CPMAddPackage(
        NAME glm
        GITHUB_REPOSITORY g-truc/glm
        GIT_TAG 1.0.1
        OPTIONS "GLM_BUILD_TESTS OFF" "BUILD_SHARED_LIBS OFF"
)

CPMAddPackage(
        NAME lua
        GITHUB_REPOSITORY walterschell/Lua
        VERSION 5.4.5
)

CPMAddPackage(
        NAME sol
        GITHUB_REPOSITORY ThePhD/sol2
        VERSION 3.3.0
)

CPMAddPackage(
        NAME spdlog
        GITHUB_REPOSITORY gabime/spdlog
        VERSION 1.14.1
)

set(IMGUI_SRC
        libs/imgui-1913/imconfig.h
        libs/imgui-1913/imgui.cpp
        libs/imgui-1913/imgui_demo.cpp
        libs/imgui-1913/imgui_draw.cpp
        libs/imgui-1913/imgui_internal.h
        libs/imgui-1913/imgui_tables.cpp
        libs/imgui-1913/imgui_widgets.cpp
        libs/imgui-1913/imstb_rectpack.h
        libs/imgui-1913/imstb_textedit.h
        libs/imgui-1913/imstb_truetype.h
        libs/imgui-1913/backends/imgui_impl_sdl2.h
        libs/imgui-1913/backends/imgui_impl_sdl2.cpp
        libs/imgui-1913/backends/imgui_impl_sdlrenderer2.h
        libs/imgui-1913/backends/imgui_impl_sdlrenderer2.cpp

)

add_executable(custom_game_engine
        src/main.cpp
        ${IMGUI_SRC}
        src/game/Game.cpp
        src/game/Game.hpp
        src/game/Engine.cpp
        src/game/Engine.hpp
        src/const/Const.hpp
        src/timer/Timer.cpp
        src/timer/Timer.hpp
)
target_link_libraries(custom_game_engine
        SDL2::SDL2 SDL2::SDL2main SDL2_image::SDL2_image SDL2_ttf::SDL2_ttf SDL2_mixer::SDL2_mixer
        lua_static
        sol2::sol2
        glm::glm
        spdlog::spdlog_header_only
)
target_include_directories(custom_game_engine PRIVATE libs/imgui-1913 libs/imgui-1913/backends src)

if (WIN32)
    # Copy SDL2
    add_custom_command(TARGET custom_game_engine POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${SDL2_DIR}/../lib/x64/SDL2.dll" $<TARGET_FILE_DIR:custom_game_engine>)
    # Copy SDL2_image
    add_custom_command(TARGET custom_game_engine POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${SDL2_IMAGE_DLL_DIR}/SDL2_image.dll" $<TARGET_FILE_DIR:custom_game_engine>)
    # Copy SDL2_ttf
    add_custom_command(TARGET custom_game_engine POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${SDL2_TTF_DLL_DIR}/SDL2_ttf.dll" $<TARGET_FILE_DIR:custom_game_engine>)
    # Copy SDL2_mixer
    add_custom_command(TARGET custom_game_engine POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${SDL2_MIXER_DLL_DIR}/SDL2_mixer.dll" $<TARGET_FILE_DIR:custom_game_engine>)
endif ()