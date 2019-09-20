def printQueue(queue):
	for element in queue:
		print(str(element), end = " ")

def insertInt(queue, queueSize):
	try:
		queue[globalLineNumber] = input("Enter integer to insert: ")
		print("Inserted. Queue is now: ")
		printQueue(queue)
	except IndexError:
		printQueue(queue)

def popInt(queue, queueSize):
	try:
		print("Popping first in queue...")
		queue[0] = 0
		iterator = 0

		while(iterator < queueSize - 1):
			queue[iterator] = queue[iterator + 1]
			iterator += 1
		queue[queueSize - 1] = 0

		print("Popped. Queue is now:")
		printQueue(queue);
	except IndexError:
		printQueue(queue)

if __name__ == '__main__':
	globalLineNumber = 0
	queue = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
	queueSize = len(queue)

	while(1):
		choice = input("\nWhat do you want to do?\n1) Insert integer.\n2) Pop integer.\n3) Exit\n")
		if(choice == "1"):
			insertInt(queue, queueSize)
			if(globalLineNumber != 10):
				globalLineNumber += 1
		elif(choice == "2"):
			popInt(queue, queueSize)    
			if(globalLineNumber != 0):
				globalLineNumber -= 1
		else:
			exit(0)
