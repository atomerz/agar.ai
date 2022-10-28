find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(GLEW REQUIRED)
find_package(Threads REQUIRED)

include_directories( ${GLUT_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS} )

file(GLOB_RECURSE CppSources ./*.cpp)
file(GLOB_RECURSE CppHeaders ./*.h)

add_executable(agar.ai ${CppSources})

target_link_libraries(agar.ai OpenGL::GL OpenGL::GLU GLUT::GLUT GLEW::GLEW Threads::Threads)

target_compile_features(agar.ai PUBLIC cxx_std_17)
