import keyboard

class ROBOT_CONTROL:

    def __init__(self):
        self.dataToSend = [0, 0]
        self.mode = 0

    def startStopMotor(self, uart, state):
        self.motorStateFlag = state
        if(self.motorStateFlag):
            self.dataToSend[0] = 0
            self.dataToSend[1] = 1
            uart.send_data(self.dataToSend)
        else:
            self.dataToSend[0] = 0
            self.dataToSend[1] = 0
            uart.send_data(self.dataToSend)
            
    def modeControl(self, uart, mode):
        self.mode = mode
        self.dataToSend[0] = 1
        self.dataToSend[1] = mode
        uart.send_data(self.dataToSend)

    def setMotorRpm(self, uart, value):
        self.dataToSend[0] = 2
        self.dataToSend[1] = value
        uart.send_data(self.dataToSend)
        
    def keyControl(self, uart):
        if self.mode == 2:
            try:
                if keyboard.is_pressed('w'):
                    self.dataToSend[0] = 3
                    self.dataToSend[1] = 1
                    uart.send_data(self.dataToSend)
                elif keyboard.is_pressed('a'):
                    self.dataToSend[0] = 3
                    self.dataToSend[1] = 2
                    uart.send_data(self.dataToSend)
                elif keyboard.is_pressed('d'):
                    self.dataToSend[0] = 3
                    self.dataToSend[1] = 3
                    uart.send_data(self.dataToSend)
                elif keyboard.is_pressed('s'):
                    self.dataToSend[0] = 3
                    self.dataToSend[1] = 4
                    uart.send_data(self.dataToSend)
            except:
                None