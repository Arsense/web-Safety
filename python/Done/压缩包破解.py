#coding=utf-8   这里主要是zipfile模块的应用
"""
垃圾版  完全靠字典枚举
用字典暴力破解ZIP压缩文件密码
"""
import zipfile
import threading


def ExtractFile(zFile,Password):
    try:
        zFile.extractall(pwd=Password)
        print("Found Password:",Password)
        return Password
    except:
        pass

def main():
    zFile = zipfile.ZipFile('Desktop.zip')   #目标文件名
    PasswordFile = open('Directory.txt')#字典文件名
    for Line in PasswordFile.readlines():
        Password = Line.strip('\n')
        Target =  threading.Thread(target=ExtractFile, args=(zFile, Password)) 
        Target.start()
    Guess= ExtractFile(zFile,Password)
    if Guess:
         print('Password=',Password)
         return
    else:
        print("can`t find password")
        return
if __name__=='__main__':
    main()
