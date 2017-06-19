from subprocess import call
call(["ls", "-l"])

def execute(cmd, args):
    call([cmd] + [str(x) for x in args])

def main():
    for i in range(16, 128, 16):
        for j in range(32, 256, 16):
            for k in range(10000, 100000, 10000):
                print("--------------------")
                print("cell size: %d\nlist length: %d\niterations: %d" % (i, j, k))
                execute("time", ["./list",i, j, k])
                print("--------------------")

if __name__ == "__main__":
    main()
