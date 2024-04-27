#!/user/bin/env python3
"""Simple TCP server that listens for connections on a given port
"""
import socket

CLIENT_WHITE_LIST = [
    "localhost",
    "127.0.0.1",  # localhost IP
    "10.0.0.183", # my laptop IP
]

class TCPServer:
    """Simple TCP server that listens for connections on a given port

    Operates as an echo server i.e. sends back the data it receives from the client
    """
    def __init__(self, port: int, whitelist: bool=True, localhost: bool=True):
        self.port = port
        self.whitelist = whitelist
        self.localhost = localhost
    
    def start(self):
        with socket.socket() as s:
            if self.localhost:
                s.bind(('localhost', self.port))
            else:
                s.bind(('', self.port))
            s.listen(1)

            print("Listening for connections at ", s.getsockname())

            while True:
                conn, addr = s.accept()
                if args.whitelist and addr[0] not in CLIENT_WHITE_LIST:
                    print("Connection from unauthorized client: ", addr)
                    conn.close()
                    continue
                print("Connected received from: ", addr)
                
                # read first 1024 bytes of data sent from client
                data = conn.recv(1024)
                print("Received data: ", data.decode())
                conn.sendall(data) # send data back to client
                conn.close()

    def handle_request(self, data: bytes):
        pass

if __name__ == '__main__':
    from argparse import ArgumentParser

    parser = ArgumentParser(description="Simple TCP server that listens for connections. Either connects to localhost:8888 or custom IP:1234")
    parser.add_argument('--host', default="localhost", choices=["localhost", "custom"], 
                        help="Host to connect to, default=localhost")
    parser.add_argument('--whitelist', default=True, action='store_false', help="Disable client whitelist")
    args = parser.parse_args()

    if args.host == "localhost":
        server = TCPServer(port=8888)
    else:
        server = TCPServer(port=1234, localhost=False)
    
    server.start()