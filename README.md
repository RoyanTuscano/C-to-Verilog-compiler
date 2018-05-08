# C-to-Verilog-compiler
A basic compiler to convert High Level C code to Verilog

procedure to run the code
mkdir build                                                                                                                               
cd build                                                                                                                                   
cmake ..                                                                                                                                   
make                                                                                                                                       
./src/hlsyn ../Testing_High_Level_C_File.c 20 sample.v

where 20 is the latency or critical path and sample.v is the output verilog file


