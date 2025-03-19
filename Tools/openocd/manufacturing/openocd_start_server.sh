if [ "x$1" = "xjlink" ]; then
    INTERFACE="/usr/share/openocd/scripts/interface/jlink.cfg"
	TRANSPORT="swd"
elif [ "x$1" = "xstlink" ]; then
	echo "Erasing ..."
	INTERFACE="/usr/share/openocd/scripts/interface/stlink.cfg"
	TRANSPORT="hla_swd"
else
	echo "No option provided"
	exit 1
fi

openocd -f ${INTERFACE} -c "transport select ${TRANSPORT}" -f ../scripts/board/stm32l496-nemeio.cfg -f ../scripts/tools/flash.tcl