#include <cstdio>
#include <iostream>
#include <SDL2/SDL.h>

class chip8
{
public:
	unsigned char mem[0x1000];  //memory
	unsigned char v[16];	    //registers
	unsigned short stack[16];   //stack
	unsigned short I;		    //special register
	unsigned short pc;		    //program counter
	unsigned short sp;          //stack pointer
	unsigned char gfx[64][32];  //screen
	unsigned int keys[16];		//keys
	unsigned char delayTimer; 	//delay timer
	unsigned char soundTimer;	//sound timer. it beeps
	bool drawFlag;

	unsigned char chip8_fontset[80] =
	{ 
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
	};

	void printDebug(){
		printf("-Registers-\n");
		for (int i = 0; i < 16; ++i){
			printf("%d: %d\n", i, this->v[i]);
		}
		printf("Special Reg: %d\n", I);
	}

	void init(){
		for (int i = 0; i < 0x1000; ++i){
			this->mem[i] = 0;
		}
		for (int i = 0; i < 16; ++i){
			this->v[i] = 0;
		}
		for (int y = 0; y < 32; ++y){
			for (int x = 0; x < 64; ++x)
			{
				this->gfx[x][y] = 0;
			}
		}
		this->pc = 0x200;
		this->sp = 0;
		this->I = 0;
		this->drawFlag = true;
		this->delayTimer = 0;
		this->soundTimer = 0;
		srand(time(NULL));
	}

	void loadRom(char rom[], int len){
		for(int i = 0; i < len; ++i){
		    this->mem[i + 0x200] = rom[i];
		}
		for (int i = 0; i < 80; ++i)
		{
			mem[i] = chip8_fontset[i]; 
		}
	}

	void processOpcode(){
		unsigned short opcode = mem[pc] << 8 | mem[pc + 1];
		unsigned short x = (opcode & 0x0F00) >> 8;
		unsigned short y = (opcode & 0x00F0) >> 4;
		printf("0x%x\n", opcode);
		switch(opcode & 0xF000){
			case 0x0000:
				switch(opcode & 0x000F){
					case 0x0000:
						for (int y = 0; y < 32; ++y){
							for (int x = 0; x < 64; ++x)
							{
								this->gfx[x][y] = 0;
							}
						}
						drawFlag = true;
						pc+=2;
					break;
					case 0x000E:
						--sp;
						pc = stack[sp];
						pc += 2;
					break;
				}
			break;
			case 0x1000:
				pc = opcode & 0x0FFF;	
			break;
			case 0x2000:
				stack[sp] = pc;
				++sp;
				pc = opcode & 0x0FFF;
			break;
			case 0x3000:
				if (v[x] == (opcode & 0x00FF)){
					pc += 4;
				}
				else{
					pc += 2;
				}
			break;
			case 0x4000:
				if (v[x] != (opcode & 0x00FF)){
					pc += 4;
				}
				else{
					pc += 2;
				}
			break;
			case 0x5000:
				if (v[x] == v[y]){
					pc += 4;
				}
				else{
					pc +=2;
				}
			break;
			case 0x6000:
				v[x] = opcode & 0x00FF;
				pc += 2;
			break;
			case 0x7000:
				v[x] += opcode & 0x00FF;
				pc += 2;
			break;
			case 0x8000:
				switch(opcode & 0x000F){
					case 0x0000:
						v[x] = v[y];
						pc += 2;
					break;
					case 0x0001:
						v[x] |= v[y];
						pc += 2;
					break;
					case 0x0002:
						v[x] &= v[y];
						pc += 2;
					break;
					case 0x0003:
						v[x] ^= v[y];
						pc += 2;
					break;
					case 0x0004:
						if(v[y] > (0xFF - v[x])){
							v[0xF] = 1;
						}
						else{
							v[0xF] = 0;
						}
						v[x] += v[y];
						pc += 2;
					break;
					case 0x0005: 
						if (v[x] > v[y]){
							v[0xF] = 0;
						}
						else{
							v[0xF] = 1;
						}
						v[x] -= v[y];
						pc += 2;
					break;
					case 0x0006:
						v[0xF] = v[x] & 0x1;
						v[x] >>= 1;
						pc += 2;
					break;
					case 0x0007:
						if (v[y] > v[x]){
							v[0xF] = 0;
						}
						else{
							v[0xF] = 1;
						}
						v[x] = v[y] - v[x];
						pc += 2;
					break;
					case 0x000E:
						v[0xF] = v[x] >> 7;
						v[x] <<= 1;
						pc += 2;
					break;
				}
			break;
			case 0x9000:
				if (v[x] != v[y]){
					pc += 4;
				}
				else{
					pc += 2;
				}
			break;
			case 0xA000:
				I = opcode & 0x0FFF;
				pc += 2;
			break;
			case 0xB000:
				pc = (opcode & 0x0FFF) + v[0];
			break;
			case 0xC000:
				v[x] = (rand() % 0xFF) & (opcode & 0x00FF);
				pc += 2;
			break;
			case 0xD000:
			{
			  unsigned short x = v[(opcode & 0x0F00) >> 8];
			  unsigned short y = v[(opcode & 0x00F0) >> 4];
			  unsigned short height = opcode & 0x000F;
			  unsigned short pixel;
			  v[0xF] = 0;
			  for (int yline = 0; yline < height; yline++)
			  {
			    pixel = mem[I + yline];
			    for(int xline = 0; xline < 8; xline++)
			    {
			      if((pixel & (0x80 >> xline)) != 0)
			      {
			        if(gfx[x + xline][y + yline] == 1){
			          v[0xF] = 1;                                 
			        }
			        gfx[x + xline][y + yline] ^= 1;
			      }
			    }
			  }
			  drawFlag = true;
			  pc += 2;
			}
			break;
			case 0xE000:
				switch(opcode & 0x00FF){
					case 0x009E:
						if (keys[v[x]] != 0){
							pc += 4;
						}
						else{
							pc += 2;
						}
					break;
					case 0x00A1:
						if (keys[v[x]] == 0){
							pc += 4;
						}
						else{
							pc += 2;
						}
					break;
				}
			break;
			case 0xF000:
				switch(opcode & 0x00FF){
					case 0x0007:
						v[x] = delayTimer;
						pc += 2;
					break;
					case 0x000A:
						bool press;
						for (int i = 0; i < 16; ++i)
						{
							if (keys[i] == 1)
							{
								v[x] = i;
								keys[i] = 0;
								press = true;
							}
						}
						if (!press)
						{
							break;
						}
						pc += 2;
					break;
					case 0x0015:
						delayTimer = v[x];
						pc += 2;
					break;
					case 0x0018:
						soundTimer = v[x];
						pc += 2;
					break;
					case 0x001E:
						if (I + v[x] > 0xFF)
						{
							v[0xF] = 1;
						}
						else
						{
							v[0xF] = 0;
						}
						I += v[x];
						pc +=2;
					break;
					case 0x0029:
						I = v[x] * 0x5;
						pc += 2;
					break;
					case 0x0033:
						mem[I] = v[x] / 100;
						mem[I + 1] = (v[x] / 10) % 10;
						mem[I + 2] = (v[x] % 100) % 10;					
						pc += 2;
					break;
					case 0x0055:
						for (int i = 0; i <= x; ++i)
						{
							mem[I + i] = v[i];
						}
						I += x + 1;
						pc += 2;
					break;
					case 0x0065:
						for (int i = 0; i <= x; ++i)
						{
							v[i] = mem[I + i];
						}
						I += x + 1;
						pc += 2;
					break;
				}
			break;
			default:
				printf("UNKOWN OPCODE: %x\n", opcode);
				pc += 2;
			break;
		}
		if (delayTimer > 0)
		{
			--delayTimer;
		}
		if (soundTimer > 0)
		{
			if (soundTimer == 1)
			{
				//dothings
			}
			--soundTimer;
		}
	}
};
