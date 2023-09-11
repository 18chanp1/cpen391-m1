	component s3 is
		port (
			clk_clk              : in  std_logic                     := 'X';             -- clk
			ledr_ext_export      : out std_logic_vector(9 downto 0);                     -- export
			reset_reset_n        : in  std_logic                     := 'X';             -- reset_n
			seg_hi_ext_export    : out std_logic_vector(20 downto 0);                    -- export
			seg_lo_ext_export    : out std_logic_vector(20 downto 0);                    -- export
			switch_ext_in_export : in  std_logic_vector(9 downto 0)  := (others => 'X')  -- export
		);
	end component s3;

	u0 : component s3
		port map (
			clk_clk              => CONNECTED_TO_clk_clk,              --           clk.clk
			ledr_ext_export      => CONNECTED_TO_ledr_ext_export,      --      ledr_ext.export
			reset_reset_n        => CONNECTED_TO_reset_reset_n,        --         reset.reset_n
			seg_hi_ext_export    => CONNECTED_TO_seg_hi_ext_export,    --    seg_hi_ext.export
			seg_lo_ext_export    => CONNECTED_TO_seg_lo_ext_export,    --    seg_lo_ext.export
			switch_ext_in_export => CONNECTED_TO_switch_ext_in_export  -- switch_ext_in.export
		);

