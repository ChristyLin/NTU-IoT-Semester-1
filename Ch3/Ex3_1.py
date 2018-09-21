#!/usr/bin/env python

import sys
import serial
import threading
import time
from queue import Queue

class RxThread(threading.Thread):
    def __init__(self, ser, queue):
        threading.Thread.__init__(self)
        self.ser = ser
        self.sig_temp = 1
        self.count = 0
        self._running = True
        self.freq = queue
        self.freq.put(0)
        self.period = 0
        
    def run(self):
        freq = 0
        tStart = time.time()
        while True:
            while self.ser.inWaiting():
                try:
                    sig = self.ser.readline().decode('utf8')[:-2]
                except:
                    pass
                # print(sig)
                if (sig != '0' and sig != '1') or (self.sig_temp != '0' and self.sig_temp != '1'):
                    pass
                elif sig != self.sig_temp:
                    self.count+=1
                    # print(self.count)
                    # print("not equal")
                    # print("sig = %s" %(sig))
                    # print("previous sig = %s" %(self.sig_temp))
                tEnd = time.time()
                self.period = float(tEnd - tStart)
                # freq = self.count/self.period
                # self.freq.put(freq)
                # self.count = 0
                # tStart = time.time()
                # print('period is %f' % self.period)
                if self.period > 0.3:
                    # print('period is %f' % self.period)
                    freq = (self.count/4)/self.period
                    self.freq.put(freq)
                    # print(self.count)
                    self.count = 0
                    tStart = time.time()
                # print('freq is %f' % freq)
                # print('put queue size:',self.freq.qsize())
                self.sig_temp = sig
                
    def resume(self):
        self._running = True
        
    def stop(self):
        self._running = False

class TxThread(threading.Thread):
    def __init__(self, ser, queue):
        threading.Thread.__init__(self)
        self.ser = ser
        self.freq_target = 0
        self.freq_now = 0
        self.freq = queue
        self._running = True
        
    def run(self):
        pwm = 0
        while True:
            if not self.freq.empty():
                self.freq_now = self.freq.get() 
                
                sys.stdout.flush()
                # sys.stdout.write('\r{:^10} {:^10}'.format('target','current'))
                sys.stdout.write("\r{:^10} {:^10}".format(self.freq_target,self.freq_now))
                # print('get queue size:',self.freq_quene.qsize())
            else:
                pass
            # print('target frequency: %f' % self.freq_target)
            # print('current frequency: %f' % self.freq_now)
            if self.freq_target - self.freq_now > 1:
                if pwm < 255:
                    pwm += 1
                trans = str(pwm)+'o'
                self.ser.write(trans.encode())
                time.sleep(0.1)
            elif self.freq_now - self.freq_target > 1:
                if pwm > 0:
                    pwm -= 1
                trans = str(pwm)+'o'
                self.ser.write(trans.encode())
                time.sleep(0.1)
            else:
                pass
            # print(pwm)

    def resume(self):
        self._running = True

    def stop(self):
        self._running = False
        self.ser.close()

def main():
    print('input rps or q for quit: ')
    print('{:^10} {:^10} {:^10}'.format('target','current','input'))
    # ser = serial.Serial('/dev/ttyACM0', 9600, timeout= 0.5 )
    ser = serial.Serial('COM7', 9600, timeout= 0.5 )
    queue = Queue()
    rx = RxThread(ser, queue)
    tx = TxThread(ser, queue)
    rx.start()
    tx.start()
    

    while True:
        # input_temp = input('\rinput rps or q for quit: ')
        input_temp = input()
        
        # print(rx.freq)
        if input_temp == 'q':
            rx.stop()
            tx.stop()
            ser.close()
            break
        try:
            # print(rx.freq)
            tx.freq_target = float(input_temp)
            # tx.freq_now = float(rx.freq)
        except:
            pass

if __name__=='__main__':
    main()
