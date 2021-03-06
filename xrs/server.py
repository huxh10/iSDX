#!/usr/bin/env python
#  Author:
#  Muhammad Shahbaz (muhammad.shahbaz@gatech.edu)

from threading import Thread
from multiprocessing import Queue
import socket
import struct

''' bgp server '''
class server(object):

    def __init__(self, logger, endpoint=('localhost', 6000)):
        self.logger = logger
        self.listener = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.listener.bind(endpoint)
        self.listener.listen(10)
        self.port=endpoint[1]

        self.sender_queue = Queue()
        self.receiver_queue = Queue()

    def start(self):
        self.logger.debug('waiting for connection')
        (self.conn, addr) = self.listener.accept()
        self.conn.settimeout(5)
        self.run = True

        self.sender = Thread(target=self._sender, args=(self.conn,self.sender_queue))
        self.sender.setName("sender server " + str(self.port))
        self.sender.start()

        self.receiver = Thread(target=self._receiver, args=(self.conn,self.receiver_queue))
        self.receiver.setName("receiver server " + str(self.port))
        self.receiver.start()

    def stop(self):
        self.run = False

    ''' sender '''
    def _sender(self, conn, queue):
        while self.run:
            try:
                line = queue.get(timeout=5)
                conn.send(struct.pack("H", len(line) + 2) + line)
            except:
                pass

    ''' receiver '''
    def _receiver(self, conn, queue):
        msg_buff = ''
        while self.run:
            try:
                line = conn.recv(4096)
                if not line:
                    conn.close()
                    print "socket closed by sender"
                    exit(0)

                msg_buff += line
                offset = 0
                buff_len = len(msg_buff)
                while buff_len - offset >= 2:
                    msg_len = ord(msg_buff[offset]) | ord(msg_buff[offset + 1]) << 8
                    if buff_len - offset < msg_len:
                        break

                    queue.put(msg_buff[offset + 2: offset + msg_len])
                    offset += msg_len
                msg_buff = msg_buff[offset:]

            except socket.timeout:
                pass

''' main '''
if __name__ == '__main__':
    while True:
        server = server()
        while True:
            try:
                print server.receiver_queue.get()
                server.sender_queue.put('announce route %s next-hop %s as-path [ %s ]' % ('200.0.0.0/16','172.0.0.1','100'))
            except:
                print 'thread ended'
                break
