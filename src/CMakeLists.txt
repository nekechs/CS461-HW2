# file(GLOB RELATIVE EXEC_LIST *.c)

set(EXEC_LIST 
"main.c" 
"1-3.c" 
"dtree_depth_test.c" 
"dtree_ssize_test.c" 
"default_generator.c" 
"regression_test.c"
"arraylist_test.c"
"regression_minweight.c"
"ctree_test.c"
"ctree_ssize_test.c"
"ctree_sigma_test.c"
)

add_subdirectory(core)
add_subdirectory(models)

find_package(GSL REQUIRED)

foreach(sourcefile ${EXEC_LIST})
    string(REPLACE ".c" "" testname ${sourcefile})
    add_executable(${testname} ${sourcefile})
    target_include_directories(${testname} PUBLIC "{PROJECT_SOURCE_DIR}/src/include")
    target_link_libraries(${testname} core models GSL::gsl)
endforeach(sourcefile ${EXEC_LIST})

# add_executable(${PROJECT_NAME} main.c)
# add_executable(1-3 1-3.c)
# add_executable(al_test arraylist_test.c)


# target_include_directories(${PROJECT_NAME} PUBLIC "${PROJECT_SOURCE_DIR}/src/include")
# target_include_directories(1-3 PUBLIC "${PROJECT_SOURCE_DIR}/src/include")
# target_include_directories(al_test PUBLIC "${PROJECT_SOURCE_DIR}/src/include")


# target_link_libraries(${PROJECT_NAME} math_library core)
# target_link_libraries(1-3 core)
# target_link_libraries(al_test core)