
module s3 (
	clk_clk,
	ledr_ext_export,
	reset_reset_n,
	seg_hi_ext_export,
	seg_lo_ext_export,
	switch_ext_in_export);	

	input		clk_clk;
	output	[9:0]	ledr_ext_export;
	input		reset_reset_n;
	output	[20:0]	seg_hi_ext_export;
	output	[20:0]	seg_lo_ext_export;
	input	[9:0]	switch_ext_in_export;
endmodule
