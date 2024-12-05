# ---------------------------- #
#  Optionally build the tests  #
# ---------------------------- #

if(BUILD_TESTS)
    add_executable(tests ${CMAKE_SOURCE_DIR}/src/tests.c)
    target_link_libraries(tests PRIVATE args)
    target_include_directories(tests PRIVATE ${CMAKE_SOURCE_DIR}/src)

    enable_testing() ## Enables the possibility to run ctest
    add_test(NAME tests COMMAND tests)
endif()
