from fastapi import FastAPI, Request
import motor.motor_asyncio
import pydantic
from bson import ObjectId
from fastapi.middleware.cors import CORSMiddleware
import  datetime 
import pytz

import requests

app = FastAPI()

origins = [
    "http://localhost.80000"
]
app.add_middleware(
    CORSMiddleware,
    allow_origins=origins,
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)

client = motor.motor_asyncio.AsyncIOMotorClient("mongodb+srv://ecsebot:Re0E5l79cJUNrf2u@cluster0.2yokb04.mongodb.net/?retryWrites=true&w=majority")
db = client.smart_devices



pydantic.json.ENCODERS_BY_TYPE[ObjectId]=str


@app.get("/api/state")
async def get_state():
  state = await db["data"].find_one({"poke": "mon"})

  if state == None:
    return {"fan": False, "light": False}
  return state



@app.put("/api/state")
async def capture(request: Request): 
  
  update = await request.json()

  sunset = requests.get('https://ecse-sunset-api.onrender.com/api/sunset').json()

  s_time = datetime.datetime.strptime(sunset['sunset'], '%Y-%m-%dT%H:%M:%S.%f').time()
  current_time = datetime.datetime.now(pytz.timezone('Jamaica')).time()
  datetime1 = datetime.datetime.strptime(str(s_time),"%H:%M:%S")
  datetime2 = datetime.datetime.strptime(str(current_time),"%H:%M:%S.%f")


  
  update["light"] = (datetime1<datetime2)
  update["fan"] = (float(update["temperature"]) >= 28.0) 

  find = await db["data"].find_one({"poke": "mon"})
  
  if find:
    await db["data"].update_one(({"poke": "mon"}), {"$set": update})
    find = await db["data"].find_one({"poke": "mon"})
    return find,204
  else:
    await db["data"].insert_one({**update, "poke": "mon"})
    find = await db["data"].find_one({"poke": "mon"})
    return find,204