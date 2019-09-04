def fileOpen():
	print("FOPEN starting...")
	filePointer = open("inputPY.txt", "w+")
	print("FOPEN done!\n")
	return filePointer

def fileWrite(filePointer):
	string = "KIM CHAEWON BEST GIRL"
	print("FWRITE starting...")
	print("Writing \"" + string + "\" in file...")
	filePointer.write(string)
	print("FWRITE done!\n")

def fileRead(filePointer):
	print("FREAD starting...")
	print("String inside file is: \"" + filePointer.read() + "\"")
	print("FREAD done!")

def fileClose(filePointer):
	print("FCLOSE starting...")
	filePointer.close()
	print("FCLOSE done!\n")

if __name__ == '__main__':
	filePointer = fileOpen()
	fileWrite(filePointer)
	fileClose(filePointer)
	filePointer = open("inputPY.txt", "r")
	fileRead(filePointer)