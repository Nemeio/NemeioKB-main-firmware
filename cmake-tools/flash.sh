JLINK_EXE=JLinkExe
DEVICE=STM32L496VG

JLINK_OPTIONS="-device ${DEVICE} -speed 4000 -if SWD"
if [ "x$1" = "xflash" ]; then
	echo "Flashing ..."
	${JLINK_EXE} ${JLINK_OPTIONS} -CommanderScript flash.jlink
elif [ "x$1" = "xerase" ]; then
	echo "Erasing ..."
	${JLINK_EXE} ${JLINK_OPTIONS} -CommanderScript erase.jlink
else
	echo "No option provided"
	exit 1
fi


