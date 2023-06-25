`timescale 1ns / 1ps

module inv_sqrt(clock, reset, start, ready_out, in , out);

input clock, reset, start;
input [22:0] in;
output reg ready_out;
output reg [26:0] out;

reg signed [31:0] magic_num = 32'h5f3759df;

parameter S1 = 4'h00,  S2 = 4'h01,  S3 = 4'h02,  S4 = 4'h03,
          S5 = 4'h04,  S6 = 4'h05,  S7 = 4'h06,  S8 = 4'h07, 
          S9 = 4'h08,  S10 = 4'h09, S11 = 4'h0A, S12 = 4'h0B,
          S13 = 4'h0C, S14 = 4'h0D;
          
parameter M1 = 2'h00, M2 = 2'h01, M3 = 2'h02, M4 = 2'h03;

reg [3:0] state;
reg [1:0] mul_state;
reg[4:0] counter;

reg signed [31:0] float_i, float_y, float_nshift;
reg signed [7:0] exponent;
reg signed [22:0] mantissa;
reg signed [26:0] fixed, threehalfs, substraction, divided_x, mul_in_2, padding;
reg [53:0] internal_result, result, mul_in_1;
reg [7:0] coeff;

always @ (posedge clock) begin
    if(reset == 1'b1) begin
        ready_out <= 1'b0;
        state <= S1;
    end
    else begin
        case(state)
            S1: begin
                if(start == 1'b1)
                    state <= S2;
            end
            S2: begin
                mantissa <= in;
                divided_x <= in;
                exponent <= 0'd127;
                coeff <= 0'd127;
                fixed <= 0'b00100000000000000000000000;
                counter <= 0;
                threehalfs <= 0'b00110000000000000000000000;
                state <= S3;
                out <= 0;
                padding <= 0;
                mul_state <= M1;
            end
            S3: begin
               if(mantissa[22] == 0) begin
                    mantissa <= mantissa <<< 1;
                    counter <= counter + 1;
                    state <= S3;
               end
               else
                    state <= S4;
            end
            S4:begin
                mantissa <= mantissa <<< 1;
                counter <= counter + 1;
                state <= S5;
            end
            S5: begin
                exponent <= exponent - counter;
                state <= S6;
            end
            S6:begin
                float_nshift <= {1'b0, exponent, mantissa};
                state <= S7;
            end
            S7: begin
                float_i <= float_nshift >> 1; 
                state <= S8;
            end
            S8:begin
                float_y <= magic_num - float_i; 
                state <= S9;
            end
            S9: begin
                fixed[22:0] <= float_y[22:0];
                exponent <= float_y[30:23];
                state <= S10;
            end
            S10: begin
               if(exponent - coeff > 0)
                    fixed <= fixed << (exponent - coeff);
               else
                    fixed <= fixed >> (exponent - coeff);
               state <= S11;
            end
            S11: begin
                case(mul_state)
                    M1: begin
                        mul_in_1 <= {padding, fixed[26:0]};
                        mul_in_2 <= fixed;
                        divided_x <= divided_x >> 1;
                        mul_state <= M2;
                        state <= S12;
                    end
                    M2: begin
                        mul_in_1 <= {padding, result[26:0]};
                        mul_in_2 <= divided_x;
                        mul_state <= M3;
                        state <= S12;
                    end
                    M3: begin
                        substraction <= threehalfs - result[26:0];
                        mul_state <= M4;
                    end
                    M4: begin
                        mul_in_1 <= {padding, substraction[26:0]};
                        mul_in_2 <= fixed;
                        state <= S12;
                    end
                endcase
                out <= result;
                counter <= 0;
                internal_result <= 0;
            end
            S12: begin
                if(mul_in_2[0] == 1)
                    internal_result <= internal_result + mul_in_1;
                if(counter <= 25) begin
                    counter <= counter + 1;
                    state <= S13;
                end
                else begin
                    result <= internal_result >> 23;
                    if( mul_state == M4)
                        state <= S14;
                    else                 
                        state <= S11;
                end
            end
            S13: begin
                mul_in_2 <= mul_in_2 >> 1;
                mul_in_1 <= mul_in_1 << 1;
                state <= S12;
            end
            S14: begin
                out <= result;
                ready_out <= 1;
                state <= S1;
            end
        endcase
    end
end

endmodule
