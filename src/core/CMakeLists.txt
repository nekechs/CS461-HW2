add_compile_options(-lm)
add_library(core STATIC fileio.c stringappend.c linkedlist.c random.c arraylist.c stat.c dataset_generator.c assert.c)

target_include_directories(core PUBLIC "${PROJECT_SOURCE_DIR}/src/include") 
target_link_libraries(core m)