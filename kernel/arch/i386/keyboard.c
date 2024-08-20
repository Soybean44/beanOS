#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <kernel/tty.h>
#include "interrupts/idt.h"
#include "util.h"
#include "keyboard.h"


bool capsOn;
bool capsLock;

const uint32_t scancode_to_lowercase[128] = {
	KEY_UNKNOWN,KEY_ESC,'1','2','3','4','5','6','7','8',
	'9','0','-','=','\b','\t','q','w','e','r',
	't','y','u','i','o','p','[',']','\n',KEY_CTRL,
	'a','s','d','f','g','h','j','k','l',';',
	'\'','`',KEY_LSHFT,'\\','z','x','c','v','b','n','m',',',
	'.','/',KEY_RSHFT,'*',KEY_ALT,' ',KEY_CAPS,KEY_F1,KEY_F2,KEY_F3,KEY_F4,KEY_F5,KEY_F6,KEY_F7,KEY_F8,KEY_F9,KEY_F10,KEY_NUMLCK,KEY_SCRLCK,KEY_HOME,KEY_UP,KEY_PGUP,'-',KEY_LEFT,KEY_UNKNOWN,KEY_RIGHT,
	'+',KEY_END,KEY_DOWN,KEY_PGDOWN,KEY_INS,KEY_DEL,KEY_UNKNOWN,KEY_UNKNOWN,KEY_UNKNOWN,KEY_F11,KEY_F12,KEY_UNKNOWN,KEY_UNKNOWN,KEY_UNKNOWN,KEY_UNKNOWN,KEY_UNKNOWN,KEY_UNKNOWN,
	KEY_UNKNOWN,KEY_UNKNOWN,KEY_UNKNOWN,KEY_UNKNOWN,KEY_UNKNOWN,KEY_UNKNOWN,KEY_UNKNOWN,KEY_UNKNOWN,KEY_UNKNOWN,KEY_UNKNOWN,KEY_UNKNOWN,KEY_UNKNOWN,KEY_UNKNOWN,KEY_UNKNOWN,
	KEY_UNKNOWN,KEY_UNKNOWN,KEY_UNKNOWN,KEY_UNKNOWN,KEY_UNKNOWN,KEY_UNKNOWN,KEY_UNKNOWN,KEY_UNKNOWN,KEY_UNKNOWN,KEY_UNKNOWN,KEY_UNKNOWN,KEY_UNKNOWN,KEY_UNKNOWN,KEY_UNKNOWN,
	KEY_UNKNOWN,KEY_UNKNOWN,KEY_UNKNOWN,KEY_UNKNOWN,KEY_UNKNOWN
};

const uint32_t scancode_to_uppercase[128] = {
	KEY_UNKNOWN,KEY_ESC,'!','@','#','$','%','^','&','*','(',')','_','+','\b','\t','Q','W','E','R',
	'T','Y','U','I','O','P','{','}','\n',KEY_CTRL,'A','S','D','F','G','H','J','K','L',':','"','~',KEY_LSHFT,'|','Z','X','C',
	'V','B','N','M','<','>','?',KEY_RSHFT,'*',KEY_ALT,' ',KEY_CAPS,KEY_F1,KEY_F2,KEY_F3,KEY_F4,KEY_F5,KEY_F6,KEY_F7,KEY_F8,KEY_F9,KEY_F10,KEY_NUMLCK,KEY_SCRLCK,KEY_HOME,KEY_UP,KEY_PGUP,'-',
	KEY_LEFT,KEY_UNKNOWN,KEY_RIGHT,'+',KEY_END,KEY_DOWN,KEY_PGDOWN,KEY_INS,KEY_DEL,KEY_UNKNOWN,KEY_UNKNOWN,KEY_UNKNOWN,KEY_F11,KEY_F12,KEY_UNKNOWN,KEY_UNKNOWN,KEY_UNKNOWN,KEY_UNKNOWN,
	KEY_UNKNOWN,KEY_UNKNOWN,KEY_UNKNOWN,KEY_UNKNOWN,KEY_UNKNOWN,KEY_UNKNOWN,KEY_UNKNOWN,KEY_UNKNOWN,KEY_UNKNOWN,KEY_UNKNOWN,KEY_UNKNOWN,KEY_UNKNOWN,KEY_UNKNOWN,KEY_UNKNOWN,
	KEY_UNKNOWN,KEY_UNKNOWN,KEY_UNKNOWN,KEY_UNKNOWN,KEY_UNKNOWN,KEY_UNKNOWN,KEY_UNKNOWN,KEY_UNKNOWN,KEY_UNKNOWN,KEY_UNKNOWN,KEY_UNKNOWN,KEY_UNKNOWN,KEY_UNKNOWN,KEY_UNKNOWN,
	KEY_UNKNOWN,KEY_UNKNOWN,KEY_UNKNOWN,KEY_UNKNOWN,KEY_UNKNOWN,KEY_UNKNOWN,KEY_UNKNOWN
};


void initKeyboard() {
	irq_install_handler(1, &keyboardHandler);
}

void keyboardHandler(struct InterruptRegisters *regs) {
	char scanCode = inPortB(0x60) & 0x7F; // What key is pressed
	char press = inPortB(0x60) & 0x80; // Press down or released

	switch(scanCode) {
	case 1:
	case 29:
	case 56:
	case 59:
	case 60:
	case 61:
	case 62:
	case 63:
	case 64:
	case 65:
	case 66:
	case 67:
	case 68:
	case 87:
	case 88:
		break;
	case 14:
		if (press == 0)
			terminal_backspace();
	case 42:
		//shift key
		if (press == 0) {
			capsOn = true;
		} else {
			capsOn = false;
		}
		break;
	case 58:
		if (!capsLock && press == 0) {
			capsLock = true;
		} else if (capsLock && press == 0) {
			capsLock = false;
		}
		break;
	default:
		if (press == 0) {
			if (capsOn ^ capsLock) {
				printf("%c", scancode_to_uppercase[scanCode]);
			} else {
				printf("%c", scancode_to_lowercase[scanCode]);
			}
		}

	}

}
