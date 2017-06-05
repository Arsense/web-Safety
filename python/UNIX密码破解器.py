#coding=utf-8
"""
暴力破解UNIX的密码，需要输入字典文件和UNIX的密码文件
"""

import crypt
def TestPassword(CryptPassword):
    Salt = CryptPassword[0:2]
    DirectoryFile = open('Directory.txt','r')
    for Word in DirectoryFile.readlines():
        Word = Word.strip('\n') #保留原先的字符 ，不去空格
        CryptWord = crypt.crypt(Word,Salt)
        if CryptPassword == CryptWord:
            print('Found Passed:',Word)
            return
        print('Password not Found!')
        return
def main():

    PasswordFile = open('PassWords.txt','r')  #读取密码文件
    for Line in PasswordFile.readlines():
        User = Line.split(':')[0]
        CryptPassword = Line.split(':')[1].strip('')
        print("Cracking Password For:",User)
        TestPassword(CryptPassword)
if __name__=='__main__':
    main()