#
# default .bashrc for users for computer sciences users
# 
# $Id: .bashrc,v 1.7 2013/07/16 14:58:44 pdickey Exp $
#

# only set /s/std/bin if it's not a root user
if [ $UID -ne 0 ]; then
    PATH=/s/std/bin:$PATH
fi

# Set paths. Add custom paths in .bashrc.local
if [ -d "/s" ]; then
    # set linux paths
    PATH=$PATH:/usr/afsws/bin # AFS SOFTWARE
    PATH=$PATH:/bin # standard programs
    PATH=$PATH:/usr/bin # standard programs
else
    # set cygwin paths
    PATH=$PATH:/cygdrive/c/WINDOWS
    PATH=$PATH:/cygdrive/c/Progra~1/OpenAFS/Client/Program
fi

# Add Mac OS X Paths
if [ `uname` = "Darwin" ]; then
    PATH=$PATH:"/usr/local/git/bin"
fi

# The following line should be removed for security, but too
# many people get confused if it isn't there. If you remove it, "." (the
# current directory will not be in your path, and you will have to type
# (for example)
# "./a.out" instead of "a.out" in order to run the a.out program
PATH=$PATH:.

export PATH

# remove domain name (.cs.wisc.edu in our case) from hostname, if domain name
# is in hostname (otherwise, just hostname).
export HOSTNAME=`hostname | sed -e s/\\..\*//`

# Add aliases to the .bashrc.local file.
#
# For more information on aliases, and shell commands in general,
# type "man bash"

alias rm="rm -i" # always ask before removing a file
alias mv="mv -i" # always ask before moving a file onto another file
alias cp="cp -i" # always ask before copying a file onto another file
alias logout="exit" # make "logout" work

if [ `uname` = "Linux" ]; then
    alias ls="ls -bF --color" 
        # "b" will show control characters as '?'
        # "F" will show a trailing "/" after directories
        # and a "*" after executables
        # --color will do color coding for different file types
    alias grep="grep --color=auto" # default to a colorful grep output.
        # this alias is safe to remove if you prefer plaintext.
elif [ `uname` = "Darwin" ]; then
    alias ls="ls -bFG" # same as linux ls, but --color is -G in OSX
    alias grep="grep --color=auto" # same as linux grep.
else
    alias ls="ls -bF"
fi

if [ "$PS1" ]; then
    set -o ignoreeof # ignore ^D (^D Will not exit the shell)
    set -o noclobber # prevent > to existing files
    set -o notify # Report change in job status

    export PS1="[\u@\h] (\#)\$ " # Set our prompt (man bash for more info)
    # the line below will generate a very colorful prompt for those who
    # like such things
    #export PS1="\[\033[01;39m\][\u@\h]\[\033[01;34m\] (\#)\$ \[\033[00m\]"

    if [ -n "$TERM" ]; then
        if [ "$TERM" = "xterm" ]; then
            # define 'name', 'icon', and 'title' to allow the user to give
            # xterm windows names by hand.

            function name() { echo -en "\033]0;$@\007"; }
            function icon() { echo -en "\033]1;$@\007"; }
            function title() { echo -en "\033]2;$@\007"; }

            # use xrs (resize) to reset window size after
            # resizing an xterm
            alias xrs='set -o noglob; eval `resize`; unset noglob'
        fi
    fi
fi;

if [ -f ~/.bashrc.local ]; then
    source ~/.bashrc.local
fi
