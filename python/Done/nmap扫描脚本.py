#coiding=utf-8
import optparse
import nmap


def NmapScan(TargetHost,TargetPort):
    NmapScan = nmap.PortScanner()
    Results = NmapScan.scan(TargetHost,TargetPort)
    Status = Results['scan'][TargetHost]['tcp'][int(TargetPort)]['state']
    print("[*]+TargetHost"+"tcp/"+TargetPort+""+Status)


def main():
    Parser = optparse.OptionParser('usage %prog -H '
                                   '<target host> -p <target port>')
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
    for TargetPort in args:
        NmapScan(TargetHost,TargetPort)
if __name__ == "__main__":
     main()