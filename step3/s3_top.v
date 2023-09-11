module s3_top(input CLOCK_50, input[3:0] USER_PB, output[3:0] USER_LED);
	s3 nios_sys (CLOCK_50, USER_PB[0]);
endmodule
	