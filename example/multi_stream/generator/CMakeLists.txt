add_executable(gen_data gen_data.cpp)
target_link_libraries(gen_data ff_net pthread core)

add_executable(gen_param gen_param.cpp)
target_link_libraries(gen_param core boost_program_options ff_net pthread)
