#coding=utf-8

#flag{1ae3ed9f-ec9a-48d2-aad8-36b03706e7a7}
def main():
        TestString = "lrua{1uy3yj9l-yw9u-48j2-uuj8-36h03706y7u7}"
        Result = ""
        for i in xrange(0,len(TestString)):
            wchar = ord(TestString[i])
            if(TestString[i] != "{") and (TestString[i] != "}") and not TestString[i].isdigit():
              if( wchar%2 != 0):
                      wchar = wchar+6
              else:
                 wchar = wchar -6
              if(wchar>ord('z')):
                       wchar = wchar-ord('z')+ord('a')-1
              if(wchar<ord('a')):
                     wchar=wchar-ord('a')+ord('z')-1
            Result=Result+chr(wchar)
        print Result
        
            
            
                
                
                
if __name__=="__main__":
        main()
                
            

