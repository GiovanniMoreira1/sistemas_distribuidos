import zmq
from time import sleep

context = zmq.Context()
socket = context.socket(zmq.REQ)
socket.connect("tcp://broker:5555")

i = 0

while True:
    while True:
        opcao = input("Digite a opção que você gostaria de seguir:\n 1-listar | 2-Cadastrar | 3-Excluir: ")
        
        if opcao == "1":
            socket.send_string("listar")
            mensagem = socket.recv_string()
            print(f"{mensagem}")
            sleep(0.5)

        elif opcao == "2":
            tarefa = input("Digite sua tarefa: ")
            socket.send(b"cadastrar")
            mensagem = socket.recv_string()
            print(f"mensagem recebida do cliente: {mensagem}")
            if mensagem == "ok":
                socket.send_string(tarefa)
                mensagem = socket.recv_string()
                print(f"{mensagem}")
                sleep(0.5)
            else:
                print("Deu tudo errado")
                mensagem = socket.recv()
                print(f"{mensagem}")
                sleep(0.5)

        elif opcao == "3":
            tarefa = input("Digite qual tarefa deletar: ")
            socket.send_string("deletar")
            mensagem = socket.recv_string()
            if mensagem == "ok":
                socket.send_string(tarefa)
                mensagem = socket.recv_string()
                print(f"{mensagem}")
                sleep(0.5)
            else:
                print("Deu tudo errado")
                mensagem = socket.recv_string()
                print(f"{mensagem}")
                sleep(0.5)

