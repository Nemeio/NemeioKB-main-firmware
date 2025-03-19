#!/usr/bin/expect
set timeout 20
set ip [lindex $argv 0]
set port [lindex $argv 1]
set file [lindex $argv 2]

spawn telnet $ip $port
expect "Open On-Chip Debugger"
sleep .1
send "write_flash $file\r"
expect ".*>"
exit 1