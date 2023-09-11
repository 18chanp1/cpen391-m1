
module s3_top(input CLOCK_50, 
			  input[3:0] KEY,
			  input [9:0] SW,
			  output [9:0] LEDR,
			  output [6:0] HEX0,
			  output [6:0] HEX1,
			  output [6:0] HEX2,
			  output [6:0] HEX3,
			  output [6:0] HEX4,
			  output [6:0] HEX5);
	s3 nios_sys (
		.clk_clk(CLOCK_50),
		.ledr_ext_export(LEDR [9:0]),
		.reset_reset_n(KEY[0]),
		.seg_hi_ext_export({HEX5,HEX4,HEX3}),
		.seg_lo_ext_export({HEX2, HEX1, HEX0}),
		.switch_ext_in_export(SW)
		);
endmodule
	