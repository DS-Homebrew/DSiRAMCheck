#include <nds.h>
#include <stdio.h>
#include <sys/stat.h>
#include <limits.h>

#include <string.h>
#include <unistd.h>

int numberTestVC = 0;
int numberTestVB = 0;

void VcountHandler() {
	numberTestVC++;
}

void VblankHandler() {
	numberTestVB++;
}

//---------------------------------------------------------------------------------
int main(int argc, char **argv) {
//---------------------------------------------------------------------------------

	defaultExceptionHandler();

	irqSet(IRQ_VCOUNT, VcountHandler);
	irqSet(IRQ_VBLANK, VblankHandler);

	irqEnable( IRQ_VBLANK | IRQ_VCOUNT );

	consoleDemoInit();
	iprintf("Now checking RAM...\n");

	const char* textString = "RAMCheckramcHECK";
	bool ramGood = true;

	for (u32 addr = 0x0C080000; addr < 0x0CFF0000; addr += 16) {
		memcpy((char*)addr, textString, 16);
	}

	for (u32 addr = 0x0C080000; addr < 0x0CFF0000; addr += 16) {
		if (memcmp((char*)addr, textString, 16) != 0) {
			iprintf("Area is write-protected:\n%04lX\n", addr);
			ramGood = false;
			break;
		}
	}

	if (ramGood) {
		iprintf("No problems found!\n");
	}
	iprintf("\nPress START to exit\n");

	while (!(keysDown() & KEY_START)) {
		scanKeys();
		swiWaitForVBlank();
	}

	return 0;
}
