#Before F5 this file open CMD and start get_data.py after making the board work
#Go on the project file directory and use this two commands:
#Then on a new cmd start py -m uvicorn server:app --port 8001 --reload 
#New cmd py -m uvicorn api:app --port 8000 --reload

from fastapi import FastAPI
from fastapi.staticfiles import StaticFiles

app = FastAPI()

# Mounts the local "dashboard" folder to the "/dashboard" URL path
app.mount("/dashboard", StaticFiles(directory="dashboard", html=True), name="dashboard")