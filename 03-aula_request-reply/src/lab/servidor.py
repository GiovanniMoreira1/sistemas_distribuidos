import zmq

context = zmq.Context()
socket = context.socket(zmq.REP)
socket.connect("tcp://broker:5556")

lista = []
print("AAAAAAAAAAAAAAA")
while True:
    message = socket.recv_string()
    print(f"Mensagem recebida: {message}", flush=True)
    if message == "listar":

        resposta = "\n".join(lista)
        socket.send_string(resposta)

    elif message == "cadastrar":
        socket.send_string("ok")
        tarefa = socket.recv_string()
        lista.append(tarefa)
        socket.send_string("Cadastrado com sucesso!")

    elif message == "deletar":
        socket.send_string("ok")
        tarefa = socket.recv_string()
        lista.remove(tarefa)
        socket.send_string("Removido com sucesso!")

