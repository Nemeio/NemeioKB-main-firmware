proc flash_probe { } {
    reset init
    flash probe 2
}

proc dump_flash { output } {
    reset init
    flash read_bank 2 $output
}

proc write_flash { input } {
    reset init
    stmqspi mass_erase 2
    flash write_bank 2 $input
    flash verify_bank 2 $input
}