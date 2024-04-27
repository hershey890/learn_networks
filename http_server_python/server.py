#!/user/bin/env python3
"""Simple TCP server that listens for connections on a given port
"""
from typing import Tuple
from pathlib import Path
from argparse import ArgumentParser
import socket

CLIENT_WHITE_LIST = [
    "localhost",
    "127.0.0.1",  # localhost IP
    "10.0.0.183",  # my laptop IP
]


class TCPServer:
    """Simple TCP server that listens for connections on a given port

    Operates as an echo server i.e. sends back the data it receives from the client.
    Derived classes should implement the _handle_request method to process the data received from the client
    """

    def __init__(
        self, host: str = "localhost", port: int = 8888, whitelist: bool = True
    ):
        self.host = host
        self.port = port
        self.whitelist = whitelist

    def start(self):
        with socket.socket() as s:
            # allow port reuse if the old process hasn't finish being killed
            s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
            s.bind((self.host, self.port))
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
                conn.sendall(self._handle_request(data))  # send data back to client
                conn.close()

    def _handle_request(self, data: bytes) -> bytes:
        """Returns the response to be sent back to the client
        """
        pass


class HTTPServer(TCPServer):

    def _handle_request(self, data: bytes):
        """Returns the response to be sent back to the client
        """
        request_line = data.decode().split("\n")[0].split(" ")
        http_method = request_line[0]
        print("Request received: ", request_line)
        
        header_first_line = "HTTP/1.1 200 OK"
        body = "Request received!"
        try:
            handler = getattr(self, f"_handle_{http_method}")
            header_first_line, body = handler(data)
        except AttributeError:
            header_first_line = "HTTP/1.1 501 Method Not Implemented"
            body = "Error 501: Method not implemented"

        response = f"""{header_first_line}
        Server: Crude Python HTTP Server
        Content-Type: text/html; charset=utf-8

        <html>
            <body>
                <h1>{body}</h1>
            </body>
        </html>""".encode("utf-8")
        return response
    
    def _handle_GET(self, request: bytes) -> Tuple[str, str]:
        """Handle GET requests

        Parameters
        ----------
        request : bytes
            HTTP request data passed to `self._handle_request`

        Return
        ------
        str
            First line of the HTTP response header
        str
            Response body
        """
        lines = request.decode().split("\n")
        request_line = lines[0].split(" ")
        if len(request_line) < 2:
            return "HTTP/1.1 400 Bad Request", "Error 400: Bad Request"
        
        uri = request_line[1]
        filename = uri.lstrip('/')
        if not Path(filename).exists() or Path(filename).is_dir():
            return "HTTP/1.1 404 Not Found", "Error 404: File not found"
        
        with open(filename, "r") as f:
            body = f.read()
        return "HTTP/1.1 200 OK", body


if __name__ == "__main__":
    parser = ArgumentParser(
        description="Simple TCP server that listens for connections. \
            Either connects to localhost:8888 or custom IP:1234"
    )
    parser.add_argument(
        "--host",
        default="localhost",
        choices=["localhost", "custom"],
        help="Host to connect to, default=localhost",
    )
    parser.add_argument(
        "--whitelist",
        default=True,
        action="store_false",
        help="Disable client whitelist",
    )
    args = parser.parse_args()

    if args.host == "localhost":
        server = HTTPServer()
    else:
        server = HTTPServer("", port=1234, localhost=False)
    server.start()