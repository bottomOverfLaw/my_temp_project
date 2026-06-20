"""
Use CDM not powershell to run this
command to use: py -u fake_sensor.py | py get data

"""

import sqlite3
import sys #sys.stdins for now then when I'll have the real data will use pyserial
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
            for line in sys.stdin:
                if "TEMP:" in line:
                    data = line.split("TEMP:")[1]
                    temp = float(data)
                    
                    timest = datetime.now().isoformat()
                    
                    cursor.execute("INSERT INTO Temperature (temperature, timestamp) VALUES (?, ?)", (temp, timest))

                    conn.commit()
                    #print("Insert ok")
        except sqlite3.OperationalError as e:
            print("Failed to create tables:", e)
        
except sqlite3.OperationalError as e:

    print("Failed to open the db: ", e)
