require 'rubygems'
require 'serialport'

sp = SerialPort.new("/dev/tty.uart-AEFF467AF9D70234", 9600, 8, 1, SerialPort::NONE)
while chr = sp.readline.chomp
  next unless chr.length == 8

  p chr.chomp.unpack('nnnn').map { |x| x / 10.0 }
end
