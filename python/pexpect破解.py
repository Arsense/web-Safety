#coding=utf-8

import pexpect

PROMPT = ['#','>>>','>','$']

def SendCommand(Child,Cmd):
    Child.sendline(Cmd)
    Child.expect(PROMPT)
    print(Child.before)

def Connect(User,Host,Password):
    SshNewkey = 'Are you sure want to continue connecting'

    Connect = 'ssh' + User +'@' + Host
    Child = pexpect.spawn(Connect)
    ret = Child.expect([pexpect.TIMEOUT,SshNewkey,
                        '[P|p]assword:'])
    if ret == 0:
        print("[-]Error  Connecting")
        return
    if ret == 1:
        Child.sendline('yes')
        ret = Child.expect(PROMPT)
        return Child
    if ret == 0:
        print("[-]Error Connecting")
        return
    Child.sendline(Password)
    Child.expect(PROMPT)
    return Child
    
def main():
    Host = 'localHost'
    User = 'root'
    Password = 'tool'
    Child = Connect(User,Host,Password)
    SendCommand(Child,'cat\etc\shadow | grep root')
    
    
if __name__ =="__main__":
    main()
    