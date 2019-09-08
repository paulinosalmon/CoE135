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
        a = a + str(temp.data) + '\t'
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
        print("Queue Underflow")
        return -1000
    else:
        x = q.head.data
        temp = q.head
        q.head = q.head.next
        del temp
        return x

if __name__ == '__main__':
    q = Queue()

    a = Node(10)
    b = Node(20)
    c = Node(30)

    dequeue(q)
    enqueue(q, a)
    enqueue(q, b)
    enqueue(q, c)

    traversal(q)
    dequeue(q)
    traversal(q)