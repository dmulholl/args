# ------------------------------- #
#  Optionally build all examples  #
# ------------------------------- #

if(BUILD_EXAMPLES)
    add_executable(example1 ${CMAKE_SOURCE_DIR}/src/example1.c)
    add_executable(example2 ${CMAKE_SOURCE_DIR}/src/example2.c)

    target_link_libraries(example1 PRIVATE args)
    target_link_libraries(example2 PRIVATE args)

    target_include_directories(example1 PRIVATE ${CMAKE_SOURCE_DIR}/src)
endif()

# --------------------------- #
#  Optionally build example1  #
# --------------------------- #

if(BUILD_EXAMPLE1 AND NOT BUILD_EXAMPLES)
    add_executable(example1 ${CMAKE_SOURCE_DIR}/src/example1.c)
    target_link_libraries(example1 PRIVATE args)
    target_include_directories(example1 PRIVATE ${CMAKE_SOURCE_DIR}/src)
endif()

# --------------------------- #
#  Optionally build example2  #
# --------------------------- #

if(BUILD_EXAMPLE2 AND NOT BUILD_EXAMPLES)
    add_executable(example2 ${CMAKE_SOURCE_DIR}/src/example2.c)
    target_link_libraries(example2 PRIVATE args)
    target_include_directories(example2 PRIVATE ${CMAKE_SOURCE_DIR}/src)
endif()
