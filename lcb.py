# Jacopo Grandi, 15/08/2020
# Link - Compile - Build
# mingw command creation and execution

try:
    import os
    import sys

    if len(sys.argv) > 1:
        os.chdir(sys.argv[1])
        
    if not("main.c" in os.listdir(".")): 
        # try searching father folder
        os.chdir("../")
        if not("main.c" in os.listdir(".")): 
            print("no main.c found, press any key to abort...")
            import msvcrt as m
            m.getch()
            quit()

    dirs = "-I. "
    fs = ""
    for d in os.listdir("."):
        if d.find(".") == -1:
            flag = False
            for f in os.listdir("./"+d+"/"):
                if f.endswith(".c"):
                    fs += d+"/"+f+" "
                    flag = True
            if flag: dirs += "-I" + d + " "

    cons = ""
    f = open("main.c", "r");
    mainraw = f.read(); f.close();
    if "#define LCB_NO_CONSOLE" in mainraw.splitlines(): 
        cons = "-Wl,-subsystem,windows"

    os.system("mkdir build")
    cmd = "g++ "+\
        "main.c "+\
        fs+\
        dirs+\
        "-IC:\MinGW_libs\include\SDL2 "+\
        "-IC:\MinGW_libs\include\plibsys "+\
        "-LC:\MinGW_libs\lib "+\
        "-w "+\
        "-lmingw32 -lSDL2main -lSDL2 -lSDL2_mixer -lplibsys "+\
        "-o build/test "+\
        cons
    print(cmd)
    r = os.system(cmd)

    if r == 0:
        print("build successfull")
        os.chdir("build")
        os.system("test.exe")
    else:
        os.system("pause")
except e: 
    import traceback; 
    open("error.txt", "w").write(''.join(tb.format_exception(None, e, e.__traceback__)))