# To the extent possible under law, the author(s) have dedicated all 
# copyright and related and neighboring rights to this software to the 
# public domain worldwide. This software is distributed without any warranty. 
# You should have received a copy of the CC0 Public Domain Dedication along 
# with this software. 
# If not, see <http://creativecommons.org/publicdomain/zero/1.0/>. 

# ~/.bash_profile: executed by bash(1) for login shells.

# The copy in your home directory (~/.bash_profile) is yours, please
# feel free to customise it to create a shell
# environment to your liking.  If you feel a change
# would be benifitial to all, please feel free to send
# a patch to the msys2 mailing list.

# User dependent .bash_profile file

# source the users bashrc if it exists
if [ -f "${HOME}/.bashrc" ] ; then
  source "${HOME}/.bashrc"
fi

# Set PATH so it includes user's private bin if it exists
# if [ -d "${HOME}/bin" ] ; then
#   PATH="${HOME}/bin:${PATH}"
# fi

# Set MANPATH so it includes users' private man if it exists
# if [ -d "${HOME}/man" ]; then
#   MANPATH="${HOME}/man:${MANPATH}"
# fi

# Set INFOPATH so it includes users' private info if it exists
# if [ -d "${HOME}/info" ]; then
#   INFOPATH="${HOME}/info:${INFOPATH}"
# fi

PATH=/usr/local/bin:$PATH

# emacs path 
PATH=$PATH:/c/emacs/bin

# m32c-elf path
PATH=$PATH:/usr/local/m32c-elf/bin

# rx-elf path
PATH=$PATH:/usr/local/rx-elf/bin

# rl78-elf path
PATH=$PATH:/usr/local/rl78-elf/bin

# WinAVR path
PATH=$PATH:/c/avr8-gnu-toolchain/bin

# ESP32 path
PATH=$PATH:/usr/local/xtensa-esp32-elf/bin
export IDF_PATH=/d/ESP32/esp-idf

PATH=$PATH:/c/'Program Files (x86)'/Microchip/'MPLAB C30'/bin

# Graphviz path
PATH=$PATH:/c/'Program Files (x86)'/Graphviz2.34/bin

# prompt
PS1='\h.\w % '
