import zmq
from time import sleep
from random import randint

context = zmq.Context()
pub = context.socket(zmq.PUB)
pub.connect("tcp://localhost:5555")

while True:
    mensagem = "numero " + str(randint(1,6))
    pub.send_string(mensagem)
    print(f"message: {mensagem}", flush=True)

    sleep(1)

pub.close()
context.close()
