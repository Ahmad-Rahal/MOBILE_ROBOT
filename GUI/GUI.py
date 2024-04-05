import customtkinter as ctk
from robotControl import ROBOT_CONTROL
from Uart_network import UART
from tkinter import messagebox

ctk.set_default_color_theme("dark-blue")
ctk.set_appearance_mode("dark")

class CAN_PROJECT_GUI:
    def __init__(self, root):
        self.root = root
        self.root.title("ROBOT Project")
        self.root.geometry("900x600")
        
        self.root.columnconfigure(0, weight= 1)
        self.root.columnconfigure(1, weight= 2)
        
        self.root.rowconfigure(0, weight=1)
        
        self.dataToSend = [0]

        self.model_script_path= "model.py"
        self.uartConnection = UART()
        self.robotIns = ROBOT_CONTROL()

        #leftFrame
        self.leftFrame = ctk.CTkFrame(master=self.root, fg_color="#333333")
        self.leftFrame.grid(row= 0, column=0, rowspan=2, sticky="nsew")
        self.leftFrame.columnconfigure(0, weight=1)
        self.leftFrame.rowconfigure(0, weight=1)
        self.leftFrame.rowconfigure(1, weight=1)
        self.leftFrame.rowconfigure(2, weight=1)
        self.leftFrame.rowconfigure(3, weight=1)
        self.leftFrame.rowconfigure(4, weight=1)
        self.leftFrame.rowconfigure(5, weight=1)
        self.leftFrame.rowconfigure(6, weight=1)
        
        self.uartPort = ctk.CTkEntry(master=self.leftFrame, placeholder_text="Port")
        self.uartPort.grid(row=0, column=0, padx=10, sticky="ews")
        self.uartPort.insert(0, "COM18")
        self.uartBaudrate = ctk.CTkEntry(master=self.leftFrame, placeholder_text="Baudrate")
        self.uartBaudrate.grid(row=1, column=0, padx=10, sticky="ew")
        self.uartBaudrate.insert(0, "9600")
        self.uartStateSwitch = ctk.CTkSwitch(master=self.leftFrame, text="Uart State", command=self.uartSwitchAction, onvalue="on", offvalue="off")
        self.uartStateSwitch.grid(row=2, column=0, padx=10, sticky="ew")
        self.motorStateSwitch = ctk.CTkSwitch(master=self.leftFrame, text="Robot State", command=self.robotSwitchAction, onvalue="on", offvalue="off")
        self.motorStateSwitch.grid(row=3, column=0, padx=10, sticky="ew")
        self.mode1Radio = ctk.CTkRadioButton(master=self.leftFrame, text="mode 1", command=self.mode1)
        self.mode1Radio.grid(row=4, column=0, padx=10, sticky="ew")
        self.mode1Radio.select()
        self.mode2Radio = ctk.CTkRadioButton(master=self.leftFrame, text="mode 2", command=self.mode2)
        self.mode2Radio.grid(row=5, column=0, padx=10, sticky="ew")
        self.mode3Radio = ctk.CTkRadioButton(master=self.leftFrame, text="mode 3", command=self.mode3)
        self.mode3Radio.grid(row=6, column=0, padx=10, sticky="ew")
        
        
        #motorControlFrame
        self.frame11 = ctk.CTkFrame(master=self.root)
        self.frame11.grid(row=0, column=1, padx=0, pady=0, sticky="nwes")
        self.frame11.columnconfigure(0, weight=1)
        self.frame11.columnconfigure(1, weight=1)
        self.frame11.rowconfigure(0, weight=1)
        self.frame11.rowconfigure(1, weight=1)
        self.frame11.rowconfigure(2, weight=1)
        self.frame11.rowconfigure(3, weight=1)
        self.frame11.rowconfigure(4, weight=1)
        self.frame11.rowconfigure(5, weight=1)
        self.frame11.rowconfigure(6, weight=1)
        
        #label
        self.velocityRight = ctk.CTkLabel(master=self.frame11, text="Velocity Right")
        self.velocityRight.grid(row=0, column=0, padx=(20,0), sticky="w")
        self.velocityLeft = ctk.CTkLabel(master=self.frame11, text="Velocity Left")
        self.velocityLeft.grid(row=0, column=1, padx=(20,0), sticky="w")
        self.direction = ctk.CTkLabel(master=self.frame11, text="Direction")
        self.direction.grid(row=2, column=0, padx=(20,0), sticky="w")

        #dataOutput
        self.velocityRight_output = ctk.CTkLabel(master=self.frame11, fg_color="#333333", corner_radius=5, text="0")
        self.velocityRight_output.grid(row=1, column=0, padx=(10,5), sticky="nwe")
        self.velocityLeft_output = ctk.CTkLabel(master=self.frame11, fg_color="#333333", corner_radius=5, text="0")
        self.velocityLeft_output.grid(row=1, column=1, padx=(10,5), sticky="nwe")
        self.direction_output = ctk.CTkLabel(master=self.frame11, fg_color="#333333", corner_radius=5, text="0")
        self.direction_output.grid(row=3, column=0, padx=(10,5), sticky="nwe", columnspan=2)

        self.rpm_slider = ctk.CTkSlider(master=self.frame11, from_=0, to=100, command=self.rpm_sliderAction)
        self.rpm_slider.grid(row=4, column=0, padx=(10,5), sticky="new", columnspan=2)
        self.rpm_slider.set(0)
        self.rpm_entry = ctk.CTkEntry(master=self.frame11, placeholder_text="Set percentage %")
        self.rpm_entry.grid(row=5, column=0, padx=(10,5), sticky="ew", columnspan=2)
        self.set_rpm_button = ctk.CTkButton(master=self.frame11, text="Set", command=self.rpm_buttonAction)
        self.set_rpm_button.grid(row=6, column=0, padx=(10,5), sticky="nwe", columnspan=2)
        
        self.connectionStateFlag = False
      
        self.whileLoop()

    def uartSwitchAction(self):
        try:
            if(self.uartStateSwitch.get() == "on"):
                self.connectionStateFlag = True
            elif(self.uartStateSwitch.get() == "off"):
                self.connectionStateFlag = False
            self.uartConnection.uartState(self.connectionStateFlag, self.uartPort.get(), self.uartBaudrate.get())
            if(not self.uartConnection.ser.is_open):
                self.uartStateSwitch.deselect()
            self.updateConfiguration()
            self.root.focus()
        except:
            messagebox.askokcancel("Entry Error", "Please Check your entry or Connection")
            self.uartStateSwitch.deselect()
            self.connectionStateFlag = False
            self.root.focus()

    def rpm_sliderAction(self):
        self.value = int(255 * self.rpm_slider.get()/100)
        self.robotIns.setMotorRpm(can=self.canData,value=self.value)

    def rpm_buttonAction(self):
        self.value = int(self.rpm_entry.get())
        # self.rpm_slider.set(int(self.rpm_entry.get()))
        self.robotIns.setMotorRpm(uart=self.uartConnection, value=self.value)
        self.root.focus()

    def robotSwitchAction(self):
        if(self.motorStateSwitch.get() == "on"):
            self.state = True
        elif(self.motorStateSwitch.get() == "off"):
            self.state = False
        self.robotIns.startStopMotor(uart=self.uartConnection, state=self.state)

    def mode1(self):
       if(self.connectionStateFlag ):
            self.mode2Radio.deselect()
            self.mode3Radio.deselect()
            self.robotIns.modeControl(uart=self.uartConnection, mode=1)

    def mode2(self):
        if(self.connectionStateFlag ):
            self.mode1Radio.deselect()
            self.mode3Radio.deselect()
            self.robotIns.modeControl(uart=self.uartConnection, mode=2)

    def mode3(self):
        if(self.connectionStateFlag ):
            self.mode1Radio.deselect()
            self.mode2Radio.deselect()
            self.robotIns.modeControl(uart=self.uartConnection, mode=3)
    
    def updateConfiguration(self):
        self.velocityRight_output.configure(text=f"{self.uartConnection.data[0]}")
        self.velocityLeft_output.configure(text=f"{self.uartConnection.data[0]}")
    

    def whileLoop(self):
        if(self.connectionStateFlag ):
            self.robotIns.keyControl(self.uartConnection)
            # self.uartConnection.receive_data()
            self.updateConfiguration()
        self.root.after(1, self.whileLoop)
        
if __name__ == "__main__":
    root = ctk.CTk()
    app = CAN_PROJECT_GUI(root)
    root.mainloop()        