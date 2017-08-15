
import socket
import time
import select

from bluetooth import *

localhost = '127.0.0.1'
localport = 5000




while True:
    counter = 0;
    badResponses = 0;
    
    #localhost socket
    local = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    local.bind((localhost,localport))
    local.setblocking(0)

    #Bluetooth Socket
    bt_server_sock=BluetoothSocket( RFCOMM )
    bt_server_sock.bind(("",PORT_ANY))
    bt_server_sock.listen(1)

    bt_port = bt_server_sock.getsockname()[1]
    uuid = "94f39d29-7d6d-437d-973b-fba39e49d4ee"

    advertise_service( bt_server_sock, "SampleServer",
                       service_id = uuid,
                       service_classes = [ uuid, SERIAL_PORT_CLASS ],
                       profiles = [ SERIAL_PORT_PROFILE ], 
    #                   protocols = [ OBEX_UUID ] 
                        )

    #accept bt connection
    print "Waiting for connection on RFCOMM channel %d" % bt_port
    bt_client_sock, bt_client_info = bt_server_sock.accept()
    print "Accepted connection from ", bt_client_info

    BTconnected = True

    print "Server Started."
    #Socket Commincation Loop
    while True:
        if (BTconnected == False):
            print "Connection Lost, closing and reopening socket"
            break
        try:
            #select is used in socket timeout
            readyLocal = select.select([local], [], [], 1)
            
            readyBT = select.select([bt_client_sock], [], [], .1)
            
            
            
            if readyLocal[0]:
                dataLocal, addrLocal = local.recvfrom(1024)       #local host socket
                if dataLocal == 'Closing Connection':
                    break
                
                bt_client_sock.send(dataLocal[2:4])                  #bluetooth socket
                
                print time.ctime(time.time()) + str(addr) + ": :" + str(data)

            else:
                print 'No Capacity Value Received'
                
                try:
                    bt_client_sock.send('No Capacity Data Received')
                    print "Message Sent Successfully"
                except:
                    BTconnected = False
                

            ###########################
            try:
                if readyBT[0]:
                    dataBT, addrBT = bt_client_sock.recvfrom(1024)       #bluetooth host socket
                    #if dataBT == 'Closing Connection':
                    if "Closing Connection" in dataBT:
                        print "The BT client has closed the connection, closing and reopening socket"
                        break
                    #ALL OTHER CASES GO HERE

                    
                    print dataBT
                else:
                    print "No Bluetooth Message" #likely never happens
            except:
                BTconnected = False
                
            
                








                

        except:
            pass
    local.close()
    #bt_client_sock.send('Closing Connection')
    bt_client_sock.close()
    bt_server_sock.close()



