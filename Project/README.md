# Kernel Keylogger: A CoE 135 Project (Ragmac, Salmon)

## Milestones
### I. Proposal Week
Slides for the topic proposals can be found [here](https://docs.google.com/presentation/d/11GY5wkISu_foV1pBajo2zjU1L5NpFFypY6KHCpzACsk/edit#slide=id.g1f87997393_0_787).  
Topics for the project were as follows:
- Embedding a keylogger in the operating system and creating a hidden log file
- (DoS) Denial-of-service attack on a machine via depleting its resources with the use of a hidden fork bomb attack
- Stealthily enabling the system camera on boot while broadcasting it to a different output interface

Selected topic was: ***Embedding a keylogger in the operating system and creating a hidden log file***

#### Abstract
The goal is to infect the *ls* command of the terminal to activate the keylogger. The keylogger reads every single user keyboard press and secretly writes it to a log file somewhere also stored in the system (can also be stored online like Google Sheets or something, so the activity of the person using the distant machine can be effectively monitored). The keylogger should continue running until the machine’s user removes the activated kernel module/the infected command  in the system.

### II. Review of Related Literature Week
Slides for RRL week can be found [here](https://docs.google.com/presentation/d/1JDXhnF01_KN2hdHF8nI2dt4IFxg6xgxQ7OPMz8s8Zp0/edit?usp=sharing).  
Revised title to: *Embedded Keylogger in a Hijacked System Call*
#### Summary
- [System Call Hooking](https://exploit.ph/linux-kernel-hacking/2014/07/10/system-call-hooking/)
- [Base Keylogger Code](https://github.com/jarun/keysniffer)

### III. Methodology Week
Slides for Methodology week can be found [here](https://drive.google.com/open?id=1hmHRLD-FWdceC8osL4Y4xGquMPpy0-MFUEL7fSqOygE).  
#### Summary
- System Call Hook vs System Call Create
- Modify existing Keylogger code 
- Create Python parser that reads infinite data (run in background)
