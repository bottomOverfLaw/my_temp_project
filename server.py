from fastapi import FastAPI
from fastapi.staticfiles import StaticFiles

app = FastAPI()

# Mounts the local "dashboard" folder to the "/dashboard" URL path
app.mount("/dashboard", StaticFiles(directory="dashboard"), name="dashboard", html=True)