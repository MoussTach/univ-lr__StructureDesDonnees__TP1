cmake_minimum_required(VERSION 3.0)

set(CMAKE_INSTALL_RPATH_USE_LINK_PATH true)

project(List C)

# add_executable(toto list.c list.h)
add_library(list SHARED list.c list.h)

install(
	TARGETS list
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
	RUNTIME DESTINATION bin
)

install(
	FILES list.h
	DESTINATION include
)

# Ajout d'un fichier de configuration de type pkgconfig
configure_file(
	${CMAKE_CURRENT_SOURCE_DIR}/list.pc.in
	${CMAKE_CURRENT_BINARY_DIR}/list.pc
	@ONLY
)
install(
	FILES ${CMAKE_CURRENT_BINARY_DIR}/list.pc
	DESTINATION share/pkgconfig
	COMPONENT "PkgConfig"
)

#  Ajout d'un fichier de configuration de type cmake
include(CMakePackageConfigHelpers)
configure_package_config_file(
	${CMAKE_CURRENT_SOURCE_DIR}/ListConfig.cmake.in
	${CMAKE_CURRENT_BINARY_DIR}/ListConfig.cmake
	INSTALL_DESTINATION cmake
)
install(
	FILES ${CMAKE_CURRENT_BINARY_DIR}/ListConfig.cmake
	DESTINATION cmake
)

# Ajout d'un exécutable dépendant également de list.h
add_executable(test-list test-list.c list.h)
# Précision de l'ordre de construction: le programme de test doit se faire
# après la librairie
add_dependencies(test-list list)
# Aggrégation du programme de test avec la librairie list
target_link_libraries(test-list list)

# Activation des tests
enable_testing()
# Ajout d'un test
add_test(test-list ./test-list)
