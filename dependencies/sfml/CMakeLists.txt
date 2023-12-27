message(STATUS "Fetching SFML...")

# set(SFML_BUILD_AUDIO FALSE)
set(SFML_BUILD_NETWORK FALSE)
# option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

FetchContent_MakeAvailable(sfml)
