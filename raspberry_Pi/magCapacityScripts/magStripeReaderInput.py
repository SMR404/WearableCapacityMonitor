
import socket
import threading
import time

tLock = threading.Lock()
shutdown = False

def receving(name, sock):
    while not shutdown:
        try:
            tLock.acquire()
            while True:
                data, addr = sock.recvfrom(1024)
                print str(data)
        except:
            pass
        finally:
            tLock.release()

host = '127.0.0.1'
port = 0

server = ('127.0.0.1',5000)

s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
s.bind((host, port))
s.setblocking(0)

rT = threading.Thread(target=receving, args=("RecvThread",s))
rT.start()

#time.sleep(5)
try:
    message = raw_input("-> ")
except EOFError:
    print "Error: EOF or empty input!"
    message = "FAILED"

while True:
    s.sendto(message, server)
    if message == 'Quit':
        break
    tLock.acquire()
    message = raw_input("-> ")
    tLock.release()
    time.sleep(0.2)

shutdown = True
rT.join()
s.close()
