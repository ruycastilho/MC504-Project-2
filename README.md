              _______         __                 _____      __      
             /   ___ \  ____ |  |   ____  _____ | ____\_ __|  |   
             \   \  \/ /  _ \|  |  /  _ \|  __ \|  __\  |  |  |    
              \   \___(  |_| )  |_(  |_| )  | \/|  | |  |  |  |__  
               \______/\____/|____/\____/|__|   |__| |____/|____/
                _________                           __   __   
               /   _____/__ __  ______ ____ _____  |  | |  |  
               \_____  <   |  |/  ___// ___\\__  \ |  | |  |   
               /        \___  |\___ \|  \___ / __ \|  |_|  |__
              /_________/\____/\_____|\____/ \____/|____/____/
  
--------------------------------------------------------------------------------
README:

IC - UNICAMP
MC504 - Project #2
http://www.ic.unicamp.br/~islene/1s2017-mc504/proj2/system-call/system-call.html

Colorful Syscall: Changes the font-color of the bash terminal based on a 
randomly generated number and auto-updates (sources) the bash.bashrc file,
in an arch-linux OS.

--------------------------------------------------------------------------------
Implementation:

The syscall is implemented in such a way that the FS state is initially saved, 
then both Linux Kernel funcions and syscalls are used (even though such 
approach should be avoided), to open the /etc/bash.bashrc file, change it based 
on a randomly generated number, saving and closing it, and, finally, using the 
'source' command to update the bash configuration. Before returning, the FS 
state is restored.

For better results, the following lines should be included in the /etc/bash.bashrc 
file:

# Starts here
bashrc_sourced=$(stat -c %Y /etc/bash.bashrc)

PROMPT_COMMAND='
    test $(stat -c %Y /etc/bash.bashrc) -ne $bashrc_sourced && source /etc/bash.bashrc
'
# Ends here

This guarantees that the configurations will be automatically updated when the 
'source' command is executed, even though this happens in another instance of 
a terminal, not the main one, which is visible to the user.

Files included in this project:

    mycall.c :
        File that implements the custom syscall.
    ex-mycall.c :
        File that tests the custom syscall.
 
--------------------------------------------------------------------------------
AUTHOR: Ruy Castilho Barrichelo

--------------------------------------------------------------------------------
References:

 -- I/O in Kernel Space
http://www.linuxjournal.com/article/8110
http://www.linux-mag.com/id/651/
http://benninger.ca/posts/writing-to-a-file-from-the-kernel/

-- Bash Customization
http://misc.flogisoft.com/bash/tip_colors_and_formatting
https://wiki.archlinux.org/index.php/bash

-- Bash commands execution inside syscalls / Kernel space
https://stackoverflow.com/questions/15676667/how-to-execute-shell-command-in-kernel-programming

-- Updating all bash/terminals simultaneously, even though the 'source' command 
-- is executed only one instance
    https://superuser.com/questions/570000/source-new-bashrc-in-all-open-terminals

