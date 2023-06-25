`timescale 1ns / 1ps

module design_tb();
    reg clk_n, clk_p;
    wire [22:0] in;
    wire [26:0] out;
    wire ready_out;
    reg reset, reset_n;
    real r_in = 0.5566 * 8388608;
    real r_out;
    // Dip switches stimulus
    assign in = r_in;
    // Reset stimulus
initial
begin
reset = 1'b1;
reset_n = 1'b0;
#30 reset = 1'b0;
reset_n = 1'b1;
end
    // Clocks stimulus
    initial begin
        clk_n = 1'b0; //set clk to 0
        clk_p = 1'b1;
    end
    
    always begin
        #5 clk_n = ~clk_n; //toggle clk every 5 time units
        clk_p = ~clk_p; //toggle clk every 5 time units
    end
    //Put sin and cos as real values
    always @* begin
        r_out = out;
        $display("1/sqrt(x) = %f dla x = %f", r_out/8388608.0, r_in/8388608.0);
    end
    //Instantiate tested module
    design_1_wrapper dut( clk_n, clk_p, in, out, ready_out, reset, reset_n);
endmodule
