import os
import sys

def initiateFork():
	print("Initiating fork...\n")
	pid = os.fork()

	if(pid == 0): 
		print("CHILD") 
		print("\tPID is:", os.getpid()) 
		print("\tPARENT PID IS:", os.getppid()) 
		print("\tCHILD: Time to rest.");
		print("\nFork done!")

	else: 
		print("PARENT") 
		print("\tPID is:", os.getpid()) 
		print("\tPARENT PID IS:", os.getppid()) 
		print("\tWaiting for child to terminate...")
		print("\tPARENT: Time to rest.\n")


def initiateExec():
	print("Initiating exec...")
	program = """print(\"\"\"
 _______________________
< KIM CHAEWON BEST GIRL >
 -----------------------
        \   ^__^
         \  (oo)\_______
            (__)\       )\/\
                
                ||     ||

		\"\"\")"""
	exec(program)
	print("Exec done!\n")

if __name__ == '__main__':
	initiateExec()
	initiateFork()