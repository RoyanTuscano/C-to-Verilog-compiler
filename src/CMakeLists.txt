include_directories(${CMAKECAD_SOURCE_DIR}/src)
link_directories(${CMAKECAD_BINARY_DIR}/src)

add_executable(hlsyn main.cpp)
add_library(read_write read_write.cpp)
add_library(allnodes allnodes.cpp)
add_library(creategraph creategraph.cpp)
add_library(List List.cpp)
add_library(ToVerilog print_to_verilog.cpp ToVerilog.cpp)
add_library(Total_forces Total_forces.cpp Comp_Prob.cpp  ASAP.cpp ALAP.cpp error_check.cpp)


target_link_libraries(hlsyn read_write allnodes  creategraph List Total_forces ToVerilog)



