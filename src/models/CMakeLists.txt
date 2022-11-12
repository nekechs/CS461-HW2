add_compile_options(-lm)
add_library(models STATIC decisiontree.c linear_regression.c classtree.c)

target_include_directories(models PUBLIC "${PROJECT_SOURCE_DIR}/src/include") 
target_link_libraries(models core m)