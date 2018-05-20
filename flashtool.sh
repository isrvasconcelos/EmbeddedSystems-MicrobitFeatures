sudo pyocd-flashtool -d debug -t nrf51 -ce
sudo pyocd-flashtool -d debug -t nrf51 build/zephyr/zephyr.hex 
sudo minicom -D /dev/ttyACM0 -b 115200
