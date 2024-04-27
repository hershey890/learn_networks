#!/user/bin/env python3
import socket


if __name__ == '__main__':
    from argparse import ArgumentParser
    
    CUSTOM_IP = "10.0.0.41" # raspberry pi ip address

    parser = ArgumentParser(description="Simple TCP client. Either connects to localhost:8888 or custom IP:1234")
    parser.add_argument('--host', default="localhost", choices=["localhost", "custom"], 
                        help="Host to connect to, default=localhost")
    args = parser.parse_args()

    with socket.socket() as s:
        if args.host == "custom":
            s.connect((CUSTOM_IP, 1234))
        else:
            s.connect(("localhost", 8888))
        
        s.send(b'Hello, world!')