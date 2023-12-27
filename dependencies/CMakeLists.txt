include(FetchContent)
set(FETCH_SRC "$ENV{HOME}/code/fetch-src")

set(FETCHCONTENT_SOURCE_DIR_SFML ${FETCH_SRC}/SFML-2.6.1)
FetchContent_Declare(
    sfml
    GIT_REPOSITORY "https://github.com/SFML/SFML"
    GIT_TAG 69ea0cd863aed1d4092b970b676924a716ff718b #2.6.1
)
add_subdirectory(sfml)

set(FETCHCONTENT_SOURCE_DIR_IMGUI ${FETCH_SRC}/imgui-1.90)
FetchContent_Declare(
    imgui
    GIT_REPOSITORY "https://github.com/ocornut/imgui"
    GIT_TAG b81bd7ed984ce095c20a059dd0f4d527e006998f #1.90.0
)

FetchContent_MakeAvailable(imgui)

set(FETCHCONTENT_SOURCE_DIR_IMGUI-SFML ${FETCH_SRC}/imgui-sfml-2.6)
FetchContent_Declare(
    imgui-sfml
    GIT_REPOSITORY "https://github.com/SFML/imgui-sfml"
    GIT_TAG de565ac8f2b795dedc0307b60830cb006afd2ecd #2.6
)

add_subdirectory(imgui-sfml)
