cd build
rm -rf *
cmake -DBOARD=bbc_microbit ..
make -j4
cd ..
sh flashtool.sh
