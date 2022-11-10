find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(GLEW REQUIRED)
find_package(Threads REQUIRED)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/release-1.12.1.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

include_directories( ${GLUT_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS} )

include_directories(${PROJECT_SOURCE_DIR}/src/main)
file(GLOB_RECURSE AgarSources ${PROJECT_SOURCE_DIR}/src/main/agar/*.cpp)
file(GLOB_RECURSE AiSources ${PROJECT_SOURCE_DIR}/src/main/ai/*.cpp)
file(GLOB_RECURSE GraphicsSources ${PROJECT_SOURCE_DIR}/src/main/graphics/*.cpp)
file(GLOB_RECURSE AllSources ${PROJECT_SOURCE_DIR}/src/main/*.cpp)

add_executable(agar.ai ${AllSources})

target_link_libraries(agar.ai OpenGL::GL OpenGL::GLU GLUT::GLUT GLEW::GLEW Threads::Threads)
target_compile_features(agar.ai PUBLIC cxx_std_17)


enable_testing()

file(GLOB_RECURSE TestSources ${PROJECT_SOURCE_DIR}/src/test/*.cpp)

add_executable(agar.test ${TestSources} ${AgarSources} ${AiSources} ${GraphicsSources})
target_link_libraries(agar.test GTest::gtest_main OpenGL::GL OpenGL::GLU GLUT::GLUT GLEW::GLEW Threads::Threads)

include(GoogleTest)
gtest_discover_tests(agar.test)
