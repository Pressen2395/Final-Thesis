import serial
import kociemba
import time

arduino = serial.Serial('COM3',115200,timeout=1)
data=""
cube=""
solution=""
while arduino.is_open:
    data = arduino.readline().decode('ascii')
    print(data)
    if len(data) == 54:
        cube = data
        try:
            solution = kociemba.solve(cube)
            arduino.write(solution.encode('utf-8'))            
            print("Solution sent")           
        except:            
            arduino.close()            
            print("Communication Closed")
            time.sleep(1)            
            exit()
    if data == "Exit":
        exit()
        
