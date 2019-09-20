class Node():
    def __init__(self, data):
        self.data = data
        self.next = None

class Queue():
    def __init__(self):
        self.head = None
        self.tail = None

def traversal(q):
    temp = q.head 

    a = ''
    while temp != None: 
        a = a + str(temp.data) + ' '
        temp = temp.next

    print(a)  

def is_empty(q):
    if q.head == None:
        return True
    return False

def enqueue(q, n):
    if is_empty(q):
        q.head = n
        q.tail = n
    else:
        q.tail.next = n
        q.tail = n

def dequeue(q):
    if is_empty(q):
        traversal(q)
        return -1000
    else:
        x = q.head.data
        temp = q.head
        q.head = q.head.next
        del temp
        return x

if __name__ == '__main__':
    q = Queue()

    while(1):
        choice = input("\nWhat do you want to do?\n1) Insert integer.\n2) Pop integer.\n3) Exit\n")
        if(choice == "1"):
            insertInt = input("Enter integer to insert: ")
            a = Node(insertInt)
            enqueue(q, a)
            print("Inserted. Queue is now: ")
            traversal(q)
        elif(choice == "2"):
            print("Popping first in queue...")
            dequeue(q)
            print("Popped. Queue is now:")
            traversal(q)
        else:
            exit(0)
