idf_component_register(
        SRCS "main.cpp" "async_function.cpp"
        INCLUDE_DIRS "."
        REQUIRES rxcpp )
component_compile_options(-std=gnu++17)