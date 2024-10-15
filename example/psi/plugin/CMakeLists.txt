
add_library(a_csv_reader SHARED reader_a.cpp)
target_link_libraries(a_csv_reader pthread ff_net)
install(TARGETS a_csv_reader DESTINATION lib)

add_library(b_csv_reader SHARED reader_b.cpp)
target_link_libraries(b_csv_reader pthread ff_net)
install(TARGETS b_csv_reader DESTINATION lib)
