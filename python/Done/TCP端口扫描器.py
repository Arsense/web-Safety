#coding=utf-8
"""
多线程的  命令行 读取失败了
需要输入目标主机名和要扫描的常用端口列表
"""
import socket
import optparse
import threading

#创建临界锁  防止输出时乱打无序  
ScreenLock = threading.Semaphore(value=1)
def ConnectScan(TargetHost,TargetPort):
    try:  
        ConnectSocket = socket.socket(socket.AF_INET,
                                      socket.SOCK_STREAM)
        ConnectSocket((TargetHost,TargetPort))
        ConnectSocket.send('ViolentPython\r\n')
        Results = ConnectSocket.recv(100)
        ScreenLock.acquire()                                                #上锁
        print('[+]%d\tcp open'%TargetPort)
        print('[+]'+str(Results))
        ConnectSocket.close()
    except:
        ScreenLock.acquire()     
        print('[-]%d/tcp closed'%TargetPort)
    finally:
        ScreenLock.release()
        ConnectSocket.close()
def PortScan(TargetHost,TargetPorts):
    try:
        TargetIP = socket.gethostbyname(TargetHost)
    except:
        print("[-] Cannot resolve '%s':Unknown host"%
              TargetHost)
        return
    try:
        TargetName = socket.gethostbyaddr(TargetIP)
        print('\n[+]Scan Results for:' + TargetName[0])
    except:
        print('\n[+]Scan Results for:'+TargetIP)
        socket.setdefaulttimeout(1)
        for TargetPort in TargetPorts:
            print('Scanning port'+str(TargetPort))
            ConnectScan(TargetHost,int(TargetPort))
            Thread = threading.Thread(target=ConnectScan,
                                      args=(TargetHost,int(TargetPort)))
            Thread.start()

def main():
    Parser = optparse.OptionParser('usage %prog -H <target host> -p <target port>')
    Parser.add_option('-H', action='store',dest = 'TargetHost',type='string',
                  help = 'specify target host')
    Parser .add_option('-p', action='store',dest = 'TargetPort',type='int',
                   help = "specify target port")
    (options,args) = Parser.parse_args()
    TargetHost = options.TargetHost
    TargetPort = options.TargetPort
    args.append(TargetPort)
    
    if(TargetHost == None)|(TargetPort == None):
        print('[-]You must specify a target host and Ports[s]!')
        exit(0)
    PortScan(TargetHost,args)
if __name__== "__main__":
   # PortScan('www.baidu.com',[21,25,443,53,23,445])          #测试是否有效
    main()