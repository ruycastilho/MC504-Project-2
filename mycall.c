/*
MC504 Projeto 2
Implementação de Chamadas de Sistema

Colorful Syscall / Syscall Colorida

Ruy Castilho Barrichelo RA 177012


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

*/
#include <linux/unistd.h>
#include <linux/linkage.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/string.h>
#include <linux/fcntl.h>
#include <linux/types.h>
#include <asm/uaccess.h>
#include <linux/file.h>
#include <linux/stat.h>
#include <linux/syscalls.h>
#include <linux/kmod.h>
#include <linux/random.h>

/*
Colorful Syscall: Changes the font-color of the bash terminal based on a randomly generated number and auto-updates (sources) the bash.bashrc file;
Parameters: void;
Return: zero;
*/
asmlinkage long sys_mycall(void) {

    // Strings containing the configurations and
    // commands to be executed on bash.
    char *envp[] = { "HOME=/", NULL };
    char *argv1[] = { "/bin/bash", "source /etc/bash.bashrc" };
    
    // Strings to be concatenated and written in bash.bashrc
    char new_bash[113];
    
    char *bash_begin = "#\n# If not running interactively, don't do anything\n[[ $- != *i* ]] && return\n\nPS1=\'[\\e[38;5;";
    
    char color[5];
        
    char *bash_end = "m \\u@\\h \\W]\\$ \'\n";
        
    // File descritor and file variables
    int bash;
    struct file *file;
    
    // Randomly generated number
    unsigned int random_number;

    // FIle system segment variables
    loff_t pos = 0; 
    mm_segment_t fs;
    fs = get_fs();      // Saves previous FS
    set_fs (KERNEL_DS); // Sets new FS as kernel limit


    // Opens the bash file in write-omly mode.
    bash = sys_open("/etc/bash.bashrc", O_WRONLY, 0644);
    
    // Checks if opening was successfull
    if ( bash >= 0 ) {
    
        printk("File was open.\n");
                
        // Randomly generates number that represents the new color
        get_random_bytes(&random_number, sizeof(int));
        
        // Converts the number (int) to a string
        // Number is limited to 2 digits to avoid overwriting in .bashrc
        snprintf(color, sizeof(int), "%u", random_number % 100);
        
        // String operations. Concatenates all parts into the final one
        strcpy(new_bash, bash_begin);
        strcat(new_bash, color);
        strcat(new_bash, bash_end);
        
        // Writing string to opened file
        sys_write(bash, new_bash, strlen(new_bash));
        file = fget(bash);
        
        if (file) {
            vfs_write(file, new_bash, strlen(new_bash), &pos);
            fput(file);
            printk("File updated.\n");
        
        }

        // Finalizing process. Closing files
        sys_close(bash);
        printk("File was closed.\n");
        
        call_usermodehelper(argv1[0], argv1, envp, UMH_WAIT_EXEC);
        //call_usermodehelper(argv2[0], argv2, envp, UMH_WAIT_EXEC);
        printk("Bash updated!\n");
    
    
    }

    // Restoring previous state before returning
    set_fs(fs);
    
    return 0;
}

