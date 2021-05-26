import serial
import matplotlib.pyplot as plt
import numpy as np 

# Khoi tao 1 he toa do cau 
# plt.figure() 
plt.polar(0, 700) 
plt.ion() 

# init serial then open it 
serialData = serial.Serial('COM11', 115200)
serialData.close() 
serialData.open() 

# List du lieu ve goc ( don vi radian )
angle_list = list() 

# List du lieu khoang cach 
distance_list = list() 

counter = 0 


# main loop :
while 1:
    # Doc du lieu tu Serial -> chuyen sang type String -> gan cho sSerialData
    sSerialData = serialData.readline().decode('ascii')

    # debug ra terminal 
    print("Received data from serial : " + sSerialData) 
    print(type(sSerialData))  

    # Bat dau Tach du lieu doc duoc thanh list 
    sSerialData_list = sSerialData.split(',')   
    print("\nAfter split : ")
    print(float(sSerialData_list[0]))
    print(float(sSerialData_list[1]))

    # Lay du lieu goc ( theo do )
    fAngle_val_inDegree = float(sSerialData_list[0])  

    # Lay du lieu goc ( theo radian ) 
    fAngle_val_inRadian = fAngle_val_inDegree*2.0*np.pi/360.0

    # Lay du lieu khoang cach 
    # if sSerialData_list[1] 
    fDistance_val = float(sSerialData_list[1])  

    # Dua vao bo dem de hien thi 
    angle_list.append(fAngle_val_inRadian) 
    distance_list.append(fDistance_val)  

    # Cap nhat vao he toa do  
    plt.plot(angle_list, distance_list)

    # Hien thi 
    counter += 1 ; 
    plt.pause(0.001) 
    if counter > 50 :
        counter = 0 
        # plt.show() 





