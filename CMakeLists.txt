cmake_minimum_required(VERSION 3.27)
project(UniboCG23-Elaborato-2-v2 VERSION 0.5.0 LANGUAGES CXX)

include(CTest)
enable_testing()

find_package(OpenGL REQUIRED)

set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
set(BUILD_SHARED_LIBS OFF)
set(CMAKE_EXE_LINKER_FLAGS "-static")

add_link_options(-static-libgcc -static-libstdc++ -static -lpthread)
set(ASSIMP_WARNINGS_AS_ERRORS OFF)
set(FT_DISABLE_BROTLI ON)
set(FT_DISABLE_HARFBUZZ ON)

set(MANUAL_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/lib/glew/include
                        ${PROJECT_SOURCE_DIR}/lib/freeglut/include)

add_subdirectory("${PROJECT_SOURCE_DIR}/lib/glew/build/cmake" EXCLUDE_FROM_ALL)
add_subdirectory("${PROJECT_SOURCE_DIR}/lib/freeglut" EXCLUDE_FROM_ALL)
add_subdirectory("${PROJECT_SOURCE_DIR}/lib/glm" EXCLUDE_FROM_ALL)
add_subdirectory("${PROJECT_SOURCE_DIR}/lib/freetype" EXCLUDE_FROM_ALL)
add_subdirectory("${PROJECT_SOURCE_DIR}/lib/assimp" EXCLUDE_FROM_ALL)

include_directories(${MANUAL_INCLUDE_DIRS})

file(GLOB SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")

add_executable(UniboCG23-Elaborato-2-v2 ${SOURCES})

target_link_libraries(UniboCG23-Elaborato-2-v2 PUBLIC glew)
target_link_libraries(UniboCG23-Elaborato-2-v2 PUBLIC freeglut)
target_link_libraries(UniboCG23-Elaborato-2-v2 PUBLIC glm)
target_link_libraries(UniboCG23-Elaborato-2-v2 PUBLIC freetype)
target_link_libraries(UniboCG23-Elaborato-2-v2 PUBLIC assimp)

set_target_properties(UniboCG23-Elaborato-2-v2
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin/"
    LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin/"
    RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin/"
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}"/bin/")

add_custom_command(TARGET UniboCG23-Elaborato-2-v2 POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:UniboCG23-Elaborato-2-v2> $<TARGET_FILE_DIR:UniboCG23-Elaborato-2-v2>
  COMMAND_EXPAND_LISTS
  )

include(CPack)
