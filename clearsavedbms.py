import sys, os, stat, string

if (__name__=="__main__"):
    name = "madedit.cfg"
    try:
        fd = open(name, "r") 
        #fd = os.open(name,os.O_RDONLY)
        data = fd.read(os.stat(name)[stat.ST_SIZE])
        fd.close()
        data = data.split('\n')
        # Function header
        newcfg = ""

        for line in data:
            tags = line.split('\x0b')
            if len(tags) > 1:
                #print tags
                #print tags[0]
                newcfg += tags[0] + '\n'
            else:
                if len(line) > 2:
                    newcfg += line + '\n'
        #print newcfg
        fd = open(name,"wb")
        fd.write(newcfg)
        fd.close()
    except (OSError,IOError,KeyboardInterrupt):
        print '** Halted **' 
