from datetime import datetime

from influxdb_client import InfluxDBClient, Point, WritePrecision
from influxdb_client.client.write_api import SYNCHRONOUS
import serial
import pandas as pd
from util import BalanceDataProcessor

port = 'COM5'

connection = serial.Serial(port,9600)
token = "KAH-e3iGekooVCaTC5UuyF9rzG5oajS19z-vtxZb_O4mEe35_OLBDWDQEKTxQPKnKLJzYBjot4sGfnlI5ATGlw=="
org = "sayin54@gmail.com"
bucket = "Embedded_SYS"




while(True):
    x=connection.readline().decode().replace("\n","")

    print(x)
# You can generate a Token from the "Tokens Tab" in the UI
    client = InfluxDBClient(url="https://eu-central-1-1.aws.cloud2.influxdata.com", token=token)
    write_api = client.write_api(write_options=SYNCHRONOUS)

    point = Point("mem").tag("host", "host1").field("used_percent", x).time(datetime.utcnow(), WritePrecision.NS)

    write_api.write(bucket, org, point)

#write_api.write(bucket, org, [{"measurement": "h2o_feet",
  #                             "tags": {"location": "coyote_creek"},
   #                            "fields": {xDatanew},
    #                           "time": timestampnew}])