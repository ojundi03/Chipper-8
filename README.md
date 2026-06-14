# Chipper-8
## To-Do:
- Create project structure.
- Read reference material.
- Create proper project management :[.
## References:
- [Tvil: Guide to Making a Chip-8 Emulator](https://tobiasvl.github.io/blog/write-a-chip-8-emulator/#instructions)
- [SDL3](https://wiki.libsdl.org/SDL3/FrontPage)
- [corax89's chip8-test-rom](https://github.com/corax89/chip8-test-rom)
- [r/EmuDev](https://reddit.com/r/EmuDev)
- [Emulation Development Discord (#chip-8 channel)](https://discord.com/invite/7nuaqZ2)
---
# :)

## High-Level Specification

**Memory**: 4kb (4096 bytes)

**Display**: 64 x 32 px (*128 x 64 for SUPER-CHIP*), Monochrome (*Can do colours other than black/white*)

**Program Counter (PC)**: points at current instruction in memory

**Single 16-bit Register (I)**: Points at locations in memory

**stack**: for 16-bit addresses to call subroutines/functions and return from them

**8-bit delay timer**: decremented at 60hz (60 per sec) till 0

**8-bit sound timer**: same logic, beeps when not 0

**16 8-bit variable registers**: numbered V0 - VF (0 - 15 in hex)
- VF also used as a flag register. Instructions set it to 1 or 0 based on their rules e.g. to use it as a carry flag

## Memory

4KB
I and PC can address 12 bits only (equals 4096 addresses)
All memory is writable, never read only
First Chip-8 interpreter was located in memory from 000 - 1FF. It would expect prgrams to be loaded in after starting at 200 (512 in decimal). Not necessary but some old programs run this way so we should leave the initial space empty, except for font.

## Font
Need built in font with sprites representing HEX numbers from 0 - F
Each char is w: 4px, h: 5px
Store font data in memory, games will draw these like regular sprites. They set I to the chars memory address and draw it. Special instruction for setting I to chars address. We can choose where to put it anywhere in first 512 byes (000-1FF). Convention is 050-09F

Most used Font:
```
0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
0x20, 0x60, 0x20, 0x20, 0x70, // 1
0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
0x90, 0x90, 0xF0, 0x10, 0x10, // 4
0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
0xF0, 0x10, 0x20, 0x40, 0x40, // 7
0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
0xF0, 0x90, 0xF0, 0x90, 0x90, // A
0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
0xF0, 0x80, 0x80, 0x80, 0xF0, // C
0xE0, 0x90, 0x90, 0x90, 0xE0, // D
0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
0xF0, 0x80, 0xF0, 0x80, 0x80  // F
```

## Display

64 x 32 px
pixels either on / Off (True/ False or 1/ 0)
We can pick our colours (Option to design so colours can be chosen from some config)
60HZ
Redraw only when instruction that changes display data executed
**DXYN**: Drawing instruction used to draw sprites
**Sprite**: 8-bit (byte), each bit is a horizontal pixel. Each sprite 1-15 bytes tall. Drawn by treating all 0 bits as transpart. 1 bits XOR bits at that location they are drawn to.
Flickering will happen (to redraw, erase from screen first then draw again).

## Stack

Use Stack or Array.
Used to call and return from subroutines / functions. (NO OTHER PURPOSE, addresses saved there as a result). 16-bit numbers (technically only 12-bit).
Variable can be used outside emulated memory.
Either limit to 16 2-byte entries (or keep unlimited). Shouldn't matter as most Chip-8 programs won't overflow stack.

## Timers

Delay and sound. Both work the same way (60hz) decrement by 60 every second till 0.
Independent of fetch/decode/execute loop.
Sound timer needs to beep.
Delay timer does not "delay" interpreter (none-blocking).
Games will check delay timer and delay if need be manually.

## Keypad

Hex keypads. 16 keys 0 - F in 4x4 grid
```
1	2	3	C
4	5	6	D
7	8	9	E
A	0	B	F
```

On Qwerty keyboards the convention is:
```
1	2	3	4
Q	W	E	R
A	S	D	F
Z	X	C	V
```

*Left-Handed mode (For Dom, down the line)*:
```
7	8	9	0
U	I	O	P
J	K	L	;
M	,	.	/
```

Use keyboard scancodes, not key string consts (To support other layouts like AZERTY)

## Fetch/Decode/Execute Loop

Infinite loop, 3 tasks in order:
- **Fetch**: instruction from memory at PC
- **Deconde**: instruction
- **Execute** instruction

### Loop Timing

Configurable as early computers were 1MHZ eventually up to 4MHZ. Different games expect different speeds. Make it configurable.

Generally 700 instructiosn per second is good enough.

### Fetch

Read instruction at PC
Instruction is 2-bytes (read two bytes, combine into 16-bit instruction)
Immediately increment PC by 2 for next opcode

### Decode

Instructions divided into stages.
First nibble (4-bits, half-byte) denotes what stage.
Do switch statement based on nibble.
Mask off (binary AND)  the first number in instruction. One case per number.
Rest of the nibbles can have different meanings. Can be any hex number from 0-F:
- **X**: second nibble. Used to look up one of 16 registers (VX) V0 - VF
- **Y**: third nibble. Does the same as X.
- **N**: Fourth nibble. A 4-bit number.
- **NN**: Second byte (third and fourth nibbles (YN)). An 8-bit immediate number.
- **NNN**: 12-bit immediate memory address. (second, third, fourth nibbles (XYN))

Extract values from the opcode before decoding (as they'll be used multiple times, rather than doing it inside each instruction). Avoids code duplication.

Use #Define to make things easier.

X and Y always used to look up values in registers. Don't use actualy value X, get value at key X in register map. (N operands used directly without lookup).

### Execute

Do directly what each instruction says in each switch case.

## Instructions

Start with these instructions first!:
- **00E0** (clear screen)
- **1NNN** (jump)
- **6XNN** (set register VX)
- **7XNN** (add value to register VX)
- **ANNN** (set index register I)
- **DXYN** (display/draw)

Test these by running IBM Logo program (find online)

Other tests:
- BonCoder/BestCoder test, (BC_Test)
- [corax89's chip8-test-rom](https://github.com/corax89/chip8-test-rom)

### 0NNN: Execute machine language routine
Skip this one unless emulating DREAM 6800, M6800, COSMAC VIP, ETI-660.

This pauses execution to call a subroutine at address NNN.

### 00E0: Clear screen

Clears Screen (All pixels set to 0)

### 1NNN: Jump

Set PC to NNN. **Do not increment PC**

### 00EE and 2NNN: Subroutines

**2NNN** calls subroutine at memory address NNN. Push PC to stack then Set PC to NNN. To return do **00EE**. 

**00EE** pops last address from stack and sets PC to it.

### 3XNN, 4XNN, 5XY0 and 9XY0: Skip conditionally

All of these do one of two things:
- nothing
- skip 1 2-byte instruction (increment PC by 2) if some condition is true

So, execute next instruction, if and only if condition is *not* true.

Since they only skip one instruction, usually followed by either a **1NNN/ 2NNN** (jump/ call) instruction. Jumps to the if code block that should be executed if condition is true.

**3XNN**: skips one instruction if value in VX == NN
**4XNN**: skips one instruction if value in VM != NN
**5XY0**: skips if value at VX == value at VY
**9XY0**: skips if value at VX != value at VY

### 6XNN: Set
Set VX to NN

### 7XNN: Add
Add NN to VX

### Logical and arithmetic instructions
First group of instructions with nested switch cases (additional decoding past first nibble)
All logical or arithmetic (decided by last nibble of opcode)

#### 8XY0: Set
Set VX to value of VY
VY stays as is

#### 8XY1: Binary OR
Set VX to VX OR VY
VY stays as is

#### 8XY2: Binary AND
Set VX to VX AND VY
VY stays as is

#### 8XY3: Logical XOR
Set VX to VX XOR VY
VY stays as is

#### 8XY4: Add
Set VX to VX + value of VY
VY stays as is

Affects carry flag:
- If VX overflows (result is > 255) then VF set to 1, else VF set to 0

#### 8XY5 and 8XY7: Subtract
**8XY5** sets VX to VX - VY
**8XY7** sets VX to VY - VX

Affects carry flag:
- If we underflow VF set to 0, else 1 (backwards to above)

#### 8XY6 and 8XYE: Shift
**NOTE: This is an ambigous instruction. Different games expect different behaviour.**

steps:
0. (Optional/ configurable) Set VX to value of VY
1. Shift value of VX one bit to the right **8XY6** or left **8XYE**
2. Set VF to 1 if shifted bit was 1, else set to 0

### ANNN: Set index
Sets I to NNN

### BNNN: Jump with offset
**NOTE: This is an ambigous instruction. Different games expect different behaviour.**

Either do:
- jump to NNN + value in V0

Or:
- jump to address XNN + value in VX e.g. B220 jumps to 220 + valut at V2

Second way is less used. If we only choose one go with first option.

### CXNN: Random
Generate random number R
Set VX to R AND NN (Make sure to AND with NN, don't just set VX to R)

### DXYN: Display
**Most difficult instruction**

Draws N pixels tall sprite from memory address at I at X coord in VX and Y coord in VY.
Ensure that any on pixels (1) XOR pixels they are drawn to. 
If any pixels turned off by this (set to 0) set VF to 1, else set VF to 0.

First thing to do is get coords from VX and VY. (don't use them directly, get values from them)

Sprites wrap. X of 5 is same as X of 68 (64 px width), so coords are module (binary AND) the size of display when counting from 0. However drawing of Sprites does not wrap. If they are drawn near the edge of the screen, clip them, don't wrap.

Skip this if you want to try to implement it yourself first, but here’s a step by step summary of what this instruction should do:
```
Set the X coord to value in VX modulo 64 (VX & 63)
Set the Y coord to value in VY modulo 32 (VY & 31)
Set VF to 0

For N rows:
    Get Nth byte of sprite data, counting from memory address in I (I not incremented)
    For each 8 pixels/bits in this sprite row (from left to right, ie. from most to least significant bit):
        If the current pixel in the sprite row is on and the pixel at coordinates X,Y on the screen is also on, turn off the pixel and set VF to 1
        Or if the current pixel in the sprite row is on and the screen pixel is not, draw the pixel at the X and Y coordinates
        If you reach the right edge of the screen, stop drawing this row
            Increment X (VX not incremented)
    Increment Y (VY not incremented)
    Stop if you reach the bottom edge of the screen
```

### EX9E and EXA1: Skip if key
**EX9E**: skips one instruction (increment PC by 2) if key corresponding to value in VX is pressed.
**EXA1**: skips if key correspoding to value in VX is *not* pressed.

keypad is hex so values are 0-F

### FX07, FX15 and FX18: Timers
All manipulate timers:
- **FX07**: sets VX to value of delay timer
- **FX15**: sets delay timer to value in VX
- **FX18**: sets sound timer to value in VX

No instruction to read sound timer as all it does is beep when > 0

### FX1E: Add to index
I += value of VX

Historically VF not affected even if overflow happens but one game (SpaceFight 2091) relies on the overflow (I overflows from 0FFF to above 1000) causing VF to be set to 1.

If we want to play this game we need this behaviour, otherwise no.

### FX0A: Get key
Blocks (stops execution) and waits for key input.

Decrement PC unless key pressed, then increment.

Timers should still decrease.

If key pressed then VX set to hex value of key.

**On the original COSMAC VIP key only registered after release**

### FX29: Font character
I set to hex char in VX, or point I to right character in first 512 bytes of memory.

### FX33: Binary-coded decimal conversion
**Bit difficult**
Takes value of VX (one byte so 0-255) and converts to 3 decimal digits, storing these digits in memory at address in I e.g. if VX contains 156 (9C in hex) then put 1 at I, 5 at I + 1, and 6 at I + 2.

### FX55 and FX65: Store and load memory
**Ambigous instructions**
These store to or load from memory respectively.

**FX55**: Value sfrom V0 - VX (inclusive) stored in successive memory addresses starting at I e.g. V0 at I, V1 at I + 1 ... VX at I + X

**FX65**: Does the opposite, takes values at memory addresses and loads them into variable registers.

The original interpreter incremented I while it worked. Each time it stored/ loaded, then I++. Once finished I set to I + X + 1

modern interpreters used a temp var for indexing, so once finished I would not be changed.

If we only choose one behaviour go with modern, this lets us run common Chip-8 games and what common test ROMs depend on (legacy behaviour will fail tests), but to run older games from 70s/80s then we should have a configurable option to switch to legacy mode.

## Troubleshooting
Use debugger (probably switch to Clion rather than VSCode).
Add error handling for at least unknown instructions.
Run as many chip-8 test programs as we can find once we can draw IBM Logo.


[r/EmuDev](https://reddit.com/r/EmuDev)
[Emulation Development Discord (#chip-8 channel)](https://discord.com/invite/7nuaqZ2)

## What's Next?
- Make chip-8 game
- Add SUPER-CHIP support
    - Expands resolution to 128 x 64
    - Adds new instructions for scrolling display and drawing larger sprites
    - changes semantics of some Chip-8 instructions [Mastering Super-CHIP](http://johnearnest.github.io/Octo/docs/SuperChip.html)
- Add Debug Capabilities [example debug interface](https://twitter.com/kraptor/status/1153936421209509888)
- Add XO-CHIP Support [Read more about XO-CHIP](http://johnearnest.github.io/Octo/docs/XO-ChipSpecification.html)
- Make Chip-8 emulator for an obscure system


## [The specification taken from TobiasVLs Blog :)](https://tobiasvl.github.io/blog/write-a-chip-8-emulator/#8xy0-set)
