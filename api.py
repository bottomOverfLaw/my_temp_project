from fastapi import FastAPI
import sqlite3
from fastapi.middleware.cors import CORSMiddleware

app = FastAPI()
#The "*" means "allow everything" — fine for local dev
app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],
    allow_methods=["*"],
    allow_headers=["*"],
)

dataBase = "tempdata.db"

@app.get("/readings")
def get_readings():
    with sqlite3.connect(dataBase) as conn:
        cur = conn.cursor()
        res = cur.execute("SELECT temperature, timestamp FROM Temperature")
        rows = res.fetchall()

        return [{"temperature" : row[0], "timestamp" : row[1]} for row in rows]

