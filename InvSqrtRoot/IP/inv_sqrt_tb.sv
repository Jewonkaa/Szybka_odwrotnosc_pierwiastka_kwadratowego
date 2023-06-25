`timescale 1ns / 1ps

module inv_sqrt_tb;

reg clock, reset, start;
reg [22:0] in;
wire ready_out;
wire [26:0] out;

real my_result;
real my_in = 0.76311;

inv_sqrt dut(clock, reset, start, ready_out, in , out);

initial
 clock <= 1'b1;
always
    #5 clock <= ~clock;

initial begin
    reset <= 1'b1;
    #20 reset <= 1'b0;
end

initial begin
    in <= my_in * 8388608; 
    start <= 1'b0;
    #40 start <= 1'b1;
    #5 start <= 1'b0;
end

always @ (posedge ready_out) begin
    my_result = out / 8388608.0;
    $display("1/sqrt(x) = %f dla x = %f", my_result, my_in);
end

endmodule
