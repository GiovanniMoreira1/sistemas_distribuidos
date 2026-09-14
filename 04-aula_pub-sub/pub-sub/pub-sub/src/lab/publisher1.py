import zmq
from time import sleep
from datetime import datetime

context = zmq.Context()
pub = context.socket(zmq.PUB)
pub.connect("tcp://localhost:5555")

while True:
    data_formatada = "data " + datetime.now().strftime("%d/%m/%Y %H:%M")
    print(f"message: {data_formatada}", flush=True)
    pub.send_string(data_formatada)

    sleep(1)

pub.close()
context.close()
