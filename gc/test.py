from subprocess import call
call(["ls", "-l"])

def execute(cmd, args):
    call([cmd] + [str(x) for x in args])

def main():
    for i in [16, 64, 128]:
        for j in [32, 64, 128, 256]:
            for k in [10000, 50000, 100000]:
                print("--------------------")
                print("cell size: %d\nlist length: %d\niterations: %d" % (i, j, k))
                execute("time", ["./list.out",i, j, k])
                print("--------------------")

if __name__ == "__main__":
    main()
