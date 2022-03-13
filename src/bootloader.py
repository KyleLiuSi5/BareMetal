import serial
com = serial.Serial('/dev/pts/12',115200)
file = open("kernel1.img", "rb") #2464 Bytes
print("Start")
#while True:
#    com.write(f.encode())

byte = file.read(1)
while byte:
    #print(byte)
    com.write(byte)
    byte = file.read(1)
print("End")

# using function pointer to jump to new function

"""remember to uncommand move_kernel() in main()
1. make
2. make runboot
3. python3 bootloader.py 
4. screen /dev/pts/12 115200"""
