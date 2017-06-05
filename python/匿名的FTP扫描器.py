#coding=utf-8
import ftplib

def Login(HostName):
    try:
        Ftp = ftplib.FTP(HostName)
        Ftp.login('anonymous','me@your.com')
        print('\n[*]'+str(HostName)+'FTP Anonymous'
                                    'Login Successd!')
        Ftp.quit()
        return True
    except Exception as e:
        print('\n[-]'+str(HostName)+'FTP Anoymous'
                                    'Logon Failed!')
        return False
Host = '192.168.95.179'
Login(Host)
    