"""
Use CDM not powershell to run this
command to use: py -u fake_sensor.py | py get_data.py

"""

import sqlite3
#sys.stdins for now then when I'll have the real data will use pyserial
import serial
from datetime import datetime

dataBase = "tempdata.db"
createTable = """ 
    CREATE TABLE IF NOT EXISTS Temperature(
        id INTEGER PRIMARY KEY NOT NULL,
        temperature REAL,
        timestamp TEXT
    )
"""

try:
    # conn = sqlite3.connect("tempdata.db") -> doing this I need to manually close the connection
    with sqlite3.connect(dataBase) as conn:
        #using with it close the connection automatically

        try:
            #creating a new table
            cursor = conn.cursor()
            cursor.execute(createTable)
            conn.commit()

            print("Table create succesfully")

            cursor = conn.cursor()

            #open the port to read the data sent from the ADC with UART
            serPort = serial.Serial(
                port = 'COM5', baudrate = 115200, bytesize = 8, timeout = 2
            )
            
            while True:

                #read the line and decode them
                line = serPort.readline()
                decode_line = line.decode('utf-8').strip()

                #check the data read and in case match the format start splitting it, keeping only the number and cast from string back to float
                if "TEMP:" in decode_line:
                    data = decode_line.split("TEMP:")[1]
                    temp = float(data)
                    
                    timest = datetime.now().isoformat()
                    
                    #insert the data and the datetime in the db
                    cursor.execute("INSERT INTO Temperature (temperature, timestamp) VALUES (?, ?)", (temp, timest))

                    conn.commit()

        except sqlite3.OperationalError as e:
            print("Failed to create tables:", e)
        
except sqlite3.OperationalError as e:

    print("Failed to open the db: ", e)
