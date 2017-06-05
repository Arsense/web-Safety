#coding=utf-8
import socket
#example 1
"""
def ReturnBanner(IP,Port):
    try:
        socket.setdefaulttimeout(2)
        Socket = socket.socket()
        Socket.connet((IP,Port))
        Banner = Socket.recv(1024)
        return Banner
    except:
        return
def main():
        IP1 = '192.168.95.148'
        IP2 =  '192.168.95.149'
        Port = 21
        Banner1 = ReturnBanner(IP1,Port)
        if Banner1:
            print ('[+]'+IP2+':'+Banner1)
        Banner2 = ReturnBanner(IP2,Port)
        if Banner2:
            print('[+]'+IP2+':'+Banner2)
if __name__ == '__main__':
    main()"""








#example 2  迭代
"""
for x in range(1,255):
     print ("192.168.95."+str(x))"""
#example 3 遍历端口列表
"""
PortList = [21,22,252,252,441]
for Port in PortList:
    print (Port)
    """


def checkVulns(banner):
		f = open("vuln_banners.txt",'r')   #打开文本
		for line in f.readlines():        #数readlines()遍历文件的每一行  ，注意我们必须用方法.strip(‘\r’)去掉每行的回车符
	        if line.strip('\n') in banner:
		    print ("[+] Server is vulnerable: "+banner.strip('\n'))