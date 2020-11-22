cmake_minimum_required(VERSION 3.0)

project(Hach C)

# Précision pour cmake: forcer l'ajout des chemins d'installation pour l'exécution
# voir https://cmake.org/cmake/help/latest/variable/CMAKE_INSTALL_RPATH_USE_LINK_PATH.html
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH true)

find_package(List REQUIRED)
include_directories(${LIST_INCLUDE_DIRS})
link_directories(${LIST_LIB_DIRS})

add_library(hash SHARED hash.c hash.h hash.inc)
target_link_libraries(hash ${LIST_LIBRARIES})

install(
	TARGETS hash
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
	RUNTIME DESTINATION bin
)

install(
	FILES hash.h
	DESTINATION include
)

include(CMakePackageConfigHelpers)
configure_package_config_file(
	${CMAKE_CURRENT_SOURCE_DIR}/HashConfig.cmake.in
	${CMAKE_CURRENT_BINARY_DIR}/HashConfig.cmake
	INSTALL_DESTINATION cmake
)
install(
	FILES ${CMAKE_CURRENT_BINARY_DIR}/HashConfig.cmake
	DESTINATION cmake
)

enable_testing()

find_program(VALGRIND "valgrind")

# Récupération dans la variable FILES des fichiers de test
file(GLOB FILES "${CMAKE_CURRENT_SOURCE_DIR}/test-*.c")

# Pour tous les fichiers de test
foreach(FILENAME ${FILES})
	# Récupération dans la variable SRC du nom sans le préfixe du dossier
	get_filename_component(SRC ${FILENAME} NAME)
	# Récupération dans la variable TEST du nom sans le préfixe du dossier
	# et sans le suffixe
	get_filename_component(TEST ${FILENAME} NAME_WE)
	# Ajout d'un exécutable dépendant également de hash.h, hash.inc
	add_executable(${TEST} ${SRC} hash.h hash.inc)
	# Précision de l'ordre de construction: le programme de test doit se faire
	# après la librairie
	add_dependencies(${TEST} hash)
	# Assemblage du programme de test avec la librairie hash
	target_link_libraries(${TEST} hash)
	if(VALGRIND)
		# Ajout d'un test en utilisant valgrind
		add_test("${TEST}[valgrind]" ${VALGRIND} --leak-check=full --quiet --error-exitcode=1 ./${TEST})
		# Ajout d'un test normal
    	add_test("${TEST}[normal]" ./${TEST})
    else()
    	# Ajout d'un test simple
    	add_test("${TEST}" ./${TEST})
	endif()
endforeach()

set(CPACK_SOURCE_GENERATOR "ZIP")
set(CPACK_SOURCE_IGNORE_FILES "~$;${CPACK_SOURCE_IGNORE_FILES}")
include(CPack)



