rm hw.zip
rm -rf hw
cd 53lab
mkdir hw
zip hw.zip hw.c t1.run t2.run
python3 autocov-1.py