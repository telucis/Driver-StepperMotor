#include <reg51.h>
#include <absacc.h>
//#include <intrins.h>
#define uchar unsigned char
#define uint unsigned int
#define ulong unsigned long

#define ALS  XBYTE[0x0000]
#define AMS  XBYTE[0x0001]
#define BLS  XBYTE[0x0002]
#define BMS  XBYTE[0x0003]
#define CLS	 XBYTE[0x4000]
#define CMS	 XBYTE[0x4004]
#define DLS	 XBYTE[0x4008]
#define DMS	 XBYTE[0x400C]

sbit dir1=P1^4;
sbit dir2=P1^5;
sbit dir3=P1^6;
sbit dir4=P1^7;

uint d1;
uint d2;
uint d3;
uint d4;

uint flag=1;
uint flagconst=128;
uint trig=1;
uint i=0;
uint time1_count=0;
uint motor_mod=0;
uint reset=1;

uint motor_start=64;
//晶振11.0592M
//转速：10°/秒	1.8°/步 

/*
1
	ALS=
	AMS=
0.981
0.924
0.831
0.707
0.556
0.383
0.195
0

s1: a	2.5	0.195*2.5+2.5	0.383*2.5+2.5	0.556*2.5+2.5	0.707*2.5+2.5	0.831*2.5+2.5	0.924*2.5+2.5	0.981*2.5+2.5
		2.5	0.4875+2.5		0.9575+2.5		1.39+2.5		1.7675+2.5		2.0775+2.5		2.31+2.5		2.4525+2.5
	b	5	0.981*2.5+2.5	0.924*2.5+2.5	0.831*2.5+2.5	0.707*2.5+2.5	0.556*2.5+2.5	0.383*2.5+2.5	0.195*2.5+2.5
			2.4525+2.5		2.31+2.5		2.0775+2.5		1.7675+2.5		1.39+2.5		0.9575+2.5		0.4875+2.5
		5	4.9525			4.81			4.5775			4.2675			3.89			3.4575			2.9875

s2:	a	5	4.9525			4.81			4.5775			4.2675			3.89			3.4575			2.9875
	b	2.5	2.0125			1.5425			1.11			0.7325			0.4225			0.19			0.0475

s3	a	2.5	2.0125			1.5425			1.11			0.7325			0.4225			0.19			0.0475
	b	0	0.0475			0.19			0.4225			0.7325			1.11			1.5425			2.0125

s4	a	0	0.0475			0.19			0.4225			0.7325			1.11			1.5425			2.0125
	b	2.5	2.9875			3.4575			3.89			4.2675			4.5775			4.81			4.9525

	0	1	2	3	4	5	6	7	8		9		10		11		12		13		14		15		16
	0	19	78	173	300	455	632	824	1024	1223	1416	1593	1748	1874	1970	2028	2048
0x	000	013	04e	0ad	12c	1c7	278	338	400		4c7		588		639		6d4		752		7b2		7ec		800

	0	1	2	3	4	5	6	7	8		9		10		11		12		13		14		15		16
8	000	019	076	169	293	444	617	805	1000
	001	026	09b	15a	258	38d	4ef	670	800		98f		b10		c72		da7		ea5		f64		fd9		fff
*/

/*
0
0.098
0.195
0.290
0.383
0.471
0.556
0.634
0.707
0.773
0.831
0.882
0.924
0.957
0.981
0.995
1
*/

/*
0
0.005
0.019
0.043	058
0.076	09b
0.118	0f1
0.169
0.227
0.293
0.366
0.444
0.529
0.617
0.710
0.805
0.902
1
1.098
1.195
1.290
1.383
1.471
1.556
1.634
1.707
1.773
1.831
1.882
1.924
1.957
1.981
1.995
2
*/
/*
uint code sm[33]={0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x02,0x02,0x03,0x04,0x04,0x05,0x06,0x07,
				  0x07,0x08,0x09,0x0a,0x0b,0x0b,0x0c,0x0d,0x0d,0x0e,0x0e,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f};
uint code sl[33]={0x01,0x0a,0x26,0x58,0x9b,0xf1,0x5a,0xd0,0x57,0xed,0x8d,0x3b,0xef,0xad,0x70,0x36,
				  0xff,0xc8,0x8e,0x51,0x0f,0xc3,0x71,0x11,0xa7,0x2e,0xa4,0x0d,0x63,0xa6,0xd8,0xf4,0xff};
*/
uint code sm[129]={0x00,	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x01,0x01,0x01,
							0x01,0x01,0x01,0x01,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x03,
							0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x04,0x04,0x04,0x04,0x04,0x04,
							0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x05,0x05,0x05,0x05,0x05,0x05,0x05,0x05,0x05,
							0x05,0x05,0x05,0x05,0x05,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x06,
							0x06,0x06,0x06,0x06,0x06,0x06,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,
							0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,
							0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07};
							
uint code sl[129]={0x00,	0x19,0x32,0x4b,0x64,0x7d,0x96,0xaf,0xc8,0xe1,0xfa,0x13,0x18,0x28,0x3d,0x76,0x8f,
							0xa8,0xc0,0xd9,0xf1,0x09,0x22,0x3a,0x52,0x6a,0x82,0x9a,0xb1,0xc9,0xe0,0xf8,0x0f,
							0x26,0x3d,0x54,0x6b,0x82,0x98,0xae,0xc5,0xdb,0xf1,0x06,0x1c,0x32,0x47,0x5c,0x71,
							0x86,0x9a,0xaf,0xc3,0xd7,0xeb,0xff,0x12,0x26,0x39,0x4c,0x5f,0x71,0x83,0x95,0xa7,
							0xb9,0xca,0xdc,0xed,0xfd,0x0e,0x1e,0x2e,0x3e,0x4e,0x5d,0x6c,0x7b,0x8a,0x98,0xa6,
							0xb4,0xc1,0xcf,0xdc,0xe8,0xf5,0x01,0x0d,0x19,0x24,0x30,0x3a,0x45,0x4f,0x59,0x63,
							0x6d,0x76,0x7f,0x87,0x90,0x98,0x9f,0xa7,0xae,0xb5,0xbb,0xc2,0xc8,0xcd,0xd3,0xd8,
							0xdc,0xe1,0xe5,0xe9,0xec,0xf0,0xf3,0xf5,0xf7,0xf9,0xfb,0xfd,0xfe,0xfe,0xff,0xff};
			
uint code startH[65]={  0xd7,0xd4,0xd0,0xcd,0xca,0xc6,0xc3,0xc0,0xbd,0xb9,0xb6,0xb3,0xaf,0xac,0xa9,0xa5,0xa2,
						0x9f,0x9c,0x98,0x95,0x92,0x8e,0x8b,0x88,0x84,0x81,0x7e,0x7a,0x77,0x74,0x71,0x6d,0x6a,
						0x67,0x63,0x60,0x5d,0x59,0x56,0x53,0x50,0x4c,0x49,0x46,0x42,0x3f,0x3c,0x38,0x35,0x32,
						0x2e,0x2b,0x28,0x25,0x21,0x1e,0x1b,0x17,0x14,0x11,0x0d,0x0a,0x07,0x04};

uint code startL[65]={  0x80,0x32,0xe4,0x96,0x48,0xfa,0xac,0x5e,0x10,0xc2,0x74,0x26,0xd8,0x8a,0x3c,0xee,0xa0,
						0x52,0x04,0xb6,0x68,0x1a,0xcc,0x7e,0x30,0xe2,0x94,0x45,0xf8,0xaa,0x5c,0x0d,0xbf,0x72,
						0x24,0xd5,0x87,0x3a,0xec,0x9d,0x50,0x02,0xb3,0x66,0x18,0xc9,0x7c,0x2d,0xe0,0x92,0x43,
						0xf6,0xa8,0x5a,0x0c,0xbd,0x70,0x22,0xd3,0x86,0x38,0xea,0x9c,0x4d,0x00};
			
uint am;
uint al;
uint bm;
uint bl;
uint cm;
uint cl;
uint dm;
uint dl;

/*
uint s0m=0x00;
uint s0l=0x01;
uint s1m=0x00;
uint s1l=0x0a;
uint s2m=0x00;
uint s2l=0x26;
uint s3m=0x00;
uint s3l=0x58;
uint s4m=0x00;
uint s4l=0x9b;
uint s5m=0x00;
uint s5l=0xf1;
uint s6m=0x01;
uint s6l=0x5a;
uint s7m=0x01;
uint s7l=0xd0;
uint s8m=0x02;
uint s8l=0x57;
uint s9m=0x02;
uint s9l=0xed;
uint s10m=0x03;
uint s10l=0x8d;
uint s11m=0x04;
uint s11l=0x3b;
uint s12m=0x04;
uint s12l=0xef;
uint s13m=0x05;
uint s13l=0xad;
uint s14m=0x06;
uint s14l=0x70;
uint s15m=0x07;
uint s15l=0x36;
uint s16m=0x07;
uint s16l=0xff;
uint s17m=0x08;
uint s17l=0xc8;
uint s18m=0x09;
uint s18l=0x8e;
uint s19m=0x0a;
uint s19l=0x51;
uint s20m=0x0b;
uint s20l=0x0f;
uint s21m=0x0b;
uint s21l=0xc3;
uint s22m=0x0c;
uint s22l=0x71;
uint s23m=0x0d;
uint s23l=0x11;
uint s24m=0x0d;
uint s24l=0xa7;
		uint s25m=0x0e;
		uint s25l=0x2e;
uint s26m=0x0e;
uint s26l=0xa4;
uint s27m=0x0f;
uint s27l=0x0d;
uint s28m=0x0f;
uint s28l=0x63;
uint s29m=0x0f;
uint s29l=0xa6;
uint s30m=0x0f;
uint s30l=0xd8;
uint s31m=0x0f;
uint s31l=0xf4;
uint s32m=0x0f;
uint s32l=0xff;
*/

void phase1_reverse(){
	d1=1-d1;
	d2=1-d2;
}
void phase2_reverse(){
	d3=1-d3;
	d4=1-d4;
}

void onesecond(){
	/*if(i<285){		//3s
		i++;
	}
	else{
		i=0;
	}*/
	AMS=sm[64];
	ALS=sl[64];
	BMS=sm[64];
	BLS=sl[64];
	CLS=sm[64];
	CMS=sl[64];
	DLS=sm[64];
	DMS=sl[64];
	//ALS=0xff;
	//AMS=0x03;
	//BLS=0xff;
	//BMS=0x03;
	//CLS=0xff;
	//CMS=0x03;
	//DLS=0xff;
	//DMS=0x03;
	flag=1;
	//d1=0;
	//d2=1;
	//d3=0;
	//d4=1;
	//motor_start=64;
}
void motor_reverse(){
		if(flag==0){
			am=64;
			al=64;
			bm=64;
			bl=64;	
			flag=64;
		}
		if(d1==0&&d2==1){
			d1=1;
			d2=1;
			dir1=d1;
			dir2=d2;
			if(flag==1){
				am=1;
				al=1;
				bm=128;
				bl=128;			
			}			
			if(flag!=flagconst){
				AMS=sm[am];
				ALS=sl[al];
				BMS=sm[bm];
				BLS=sl[bl];
				flag++;
				am++;
				al++;
				bm--;
				bl--;
				d1=0;
				d2=1;
			}
			else{
				flag=1;
				d1=1;
				d2=1;
				AMS=sm[am];
				ALS=sl[al];
				BMS=sm[bm];
				BLS=sl[bl];
				//if(flagconst%2==1)hight();
				//else low();
			}

		}
		else if(d1==1&&d2==1){
			d1=1;
			d2=0;
			dir1=d1;
			dir2=d2;
			if(flag==1){
				am=128;
				al=128;
				bm=1;
				bl=1;		
			}
			if(flag!=flagconst){
				AMS=sm[am];
				ALS=sl[al];
				BMS=sm[bm];
				BLS=sl[bl];
				flag++;
				am--;
				al--;
				bm++;
				bl++;
				d1=1;
				d2=1;
			}
			else{
				flag=1;
				d1=1;
				d2=0;
				AMS=sm[am];
				ALS=sl[al];
				BMS=sm[bm];
				BLS=sl[bl];
			}

		}
		else if(d1==1&&d2==0){
			d1=0;
			d2=0;
			dir1=d1;
			dir2=d2;
			if(flag==1){
				am=1;
				al=1;
				bm=128;
				bl=128;
			}
			if(flag!=flagconst){
				AMS=sm[am];
				ALS=sl[al];
				BMS=sm[bm];
				BLS=sl[bl];
				flag++;
				am++;
				al++;
				bm--;
				bl--;
				d1=1;
				d2=0;
			}
			else{
				flag=1;
				d1=0;
				d2=0;
				AMS=sm[am];
				ALS=sl[al];
				BMS=sm[bm];
				BLS=sl[bl];
			}

		}
		else if(d1==0&&d2==0){
			d1=0;
			d2=1;
			dir1=d1;
			dir2=d2;
			if(flag==1){
				am=128;
				al=128;
				bm=1;
				bl=1;
			}
			if(flag!=flagconst){
				AMS=sm[am];
				ALS=sl[al];
				BMS=sm[bm];
				BLS=sl[bl];
				flag++;
				am--;
				al--;
				bm++;
				bl++;
				d1=0;
				d2=0;
			}
			else{
				flag=1;
				d1=0;
				d2=1;
				AMS=sm[am];
				ALS=sl[al];
				BMS=sm[bm];
				BLS=sl[bl];
			}

		}
}
void motor_forward(){
		if(flag==0){
			am=64;
			al=64;
			bm=64;
			bl=64;	
			flag=64;
		}
		if(d1==1&&d2==1){
			d1=0;
			d2=1;
			dir1=d1;
			dir2=d2;
			if(flag==1){
				am=1;
				al=1;
				bm=128;
				bl=128;			
			}			
			if(flag!=flagconst){
				AMS=sm[am];
				ALS=sl[al];
				BMS=sm[bm];
				BLS=sl[bl];
				flag++;
				am++;
				al++;
				bm--;
				bl--;
				d1=1;
				d2=1;
			}
			else{
				flag=1;
				d1=0;
				d2=1;
				AMS=sm[am];
				ALS=sl[al];
				BMS=sm[bm];
				BLS=sl[bl];
				//if(flagconst%2==1)hight();
				//else low();
			}

		}
		else if(d1==0&&d2==1){
			d1=0;
			d2=0;
			dir1=d1;
			dir2=d2;
			if(flag==1){
				am=128;
				al=128;
				bm=1;
				bl=1;		
			}
			if(flag!=flagconst){
				AMS=sm[am];
				ALS=sl[al];
				BMS=sm[bm];
				BLS=sl[bl];
				flag++;
				am--;
				al--;
				bm++;
				bl++;
				d1=0;
				d2=1;
			}
			else{
				flag=1;
				d1=0;
				d2=0;
				AMS=sm[am];
				ALS=sl[al];
				BMS=sm[bm];
				BLS=sl[bl];
			}

		}
		else if(d1==0&&d2==0){
			d1=1;
			d2=0;
			dir1=d1;
			dir2=d2;
			if(flag==1){
				am=1;
				al=1;
				bm=128;
				bl=128;
			}
			if(flag!=flagconst){
				AMS=sm[am];
				ALS=sl[al];
				BMS=sm[bm];
				BLS=sl[bl];
				flag++;
				am++;
				al++;
				bm--;
				bl--;
				d1=0;
				d2=0;
			}
			else{
				flag=1;
				d1=1;
				d2=0;
				AMS=sm[am];
				ALS=sl[al];
				BMS=sm[bm];
				BLS=sl[bl];
			}

		}
		else if(d1==1&&d2==0){
			d1=1;
			d2=1;
			dir1=d1;
			dir2=d2;
			if(flag==1){
				am=128;
				al=128;
				bm=1;
				bl=1;
			}
			if(flag!=flagconst){
				AMS=sm[am];
				ALS=sl[al];
				BMS=sm[bm];
				BLS=sl[bl];
				flag++;
				am--;
				al--;
				bm++;
				bl++;
				d1=1;
				d2=0;
			}
			else{
				flag=1;
				d1=1;
				d2=1;
				AMS=sm[am];
				ALS=sl[al];
				BMS=sm[bm];
				BLS=sl[bl];
			}

		}
}


void motor2_reverse(){
		if(flag==0){
			cm=64;
			cl=64;
			dm=64;
			dl=64;	
			flag=64;
		}
		if(d3==0&&d4==1){
			d3=1;
			d4=1;
			dir3=d3;
			dir4=d4;
			if(flag==1){
				cm=1;
				cl=1;
				dm=128;
				dl=128;			
			}
			if(flag!=flagconst){
				CMS=sm[cm];
				CLS=sl[cl];
				DMS=sm[dm];
				DLS=sl[dl];
				flag++;
				cm++;
				cl++;
				dm--;
				dl--;
				d3=0;
				d4=1;
			}
			else{
				flag=1;
				d3=1;
				d4=1;
				CMS=sm[cm];
				CLS=sl[cl];
				DMS=sm[dm];
				DLS=sl[dl];
				//if(flagconst%2==1)hight();
				//else low();
			}

		}
		else if(d3==1&&d4==1){
			d3=1;
			d4=0;
			dir3=d3;
			dir4=d4;
			if(flag==1){
				cm=128;
				cl=128;
				dm=1;
				dl=1;		
			}
			if(flag!=flagconst){
				CMS=sm[cm];
				CLS=sl[cl];
				DMS=sm[dm];
				DLS=sl[dl];
				flag++;
				cm--;
				cl--;
				dm++;
				dl++;
				d3=1;
				d4=1;
			}
			else{
				flag=1;
				d3=1;
				d4=0;
				CMS=sm[cm];
				CLS=sl[cl];
				DMS=sm[dm];
				DLS=sl[dl];
			}

		}
		else if(d3==1&&d4==0){
			d3=0;
			d4=0;
			dir3=d3;
			dir4=d4;
			if(flag==1){
				cm=1;
				cl=1;
				dm=128;
				dl=128;
			}
			if(flag!=flagconst){
				CMS=sm[cm];
				CLS=sl[cl];
				DMS=sm[dm];
				DLS=sl[dl];
				flag++;
				cm++;
				cl++;
				dm--;
				dl--;
				d3=1;
				d4=0;
			}
			else{
				flag=1;
				d3=0;
				d4=0;
				CMS=sm[cm];
				CLS=sl[cl];
				DMS=sm[dm];
				DLS=sl[dl];
			}

		}
		else if(d3==0&&d4==0){
			d3=0;
			d4=1;
			dir3=d3;
			dir4=d4;
			if(flag==1){
				cm=128;
				cl=128;
				dm=1;
				dl=1;
			}
			if(flag!=flagconst){
				CMS=sm[cm];
				CLS=sl[cl];
				DMS=sm[dm];
				DLS=sl[dl];
				flag++;
				cm--;
				cl--;
				dm++;
				dl++;
				d3=0;
				d4=0;
			}
			else{
				flag=1;
				d3=0;
				d4=1;
				CMS=sm[cm];
				CLS=sl[cl];
				DMS=sm[dm];
				DLS=sl[dl];
			}

		}
}
void motor2_forward(){
		if(flag==0){
			cm=64;
			cl=64;
			dm=64;
			dl=64;	
			flag=64;
		}
		if(d3==1&&d4==1){
			d3=0;
			d4=1;
			dir3=d3;
			dir4=d4;
			if(flag==1){
				cm=1;
				cl=1;
				dm=128;
				dl=128;			
			}
			if(flag!=flagconst){
				CMS=sm[cm];
				CLS=sl[cl];
				DMS=sm[dm];
				DLS=sl[dl];
				flag++;
				cm++;
				cl++;
				dm--;
				dl--;
				d3=1;
				d4=1;
			}
			else{
				flag=1;
				d3=0;
				d4=1;
				CMS=sm[cm];
				CLS=sl[cl];
				DMS=sm[dm];
				DLS=sl[dl];
				//if(flagconst%2==1)hight();
				//else low();
			}
		}
		else if(d3==0&&d4==1){
			d3=0;
			d4=0;
			dir3=d3;
			dir4=d4;
			if(flag==1){
				cm=128;
				cl=128;
				dm=1;
				dl=1;		
			}
			if(flag!=flagconst){
				CMS=sm[cm];
				CLS=sl[cl];
				DMS=sm[dm];
				DLS=sl[dl];
				flag++;
				cm--;
				cl--;
				dm++;
				dl++;
				d3=0;
				d4=1;
			}
			else{
				flag=1;
				d3=0;
				d4=0;
				CMS=sm[cm];
				CLS=sl[cl];
				DMS=sm[dm];
				DLS=sl[dl];
			}

		}
		else if(d3==0&&d4==0){
			d3=1;
			d4=0;
			dir3=d3;
			dir4=d4;
			if(flag==1){
				cm=1;
				cl=1;
				dm=128;
				dl=128;
			}
			if(flag!=flagconst){
				CMS=sm[cm];
				CLS=sl[cl];
				DMS=sm[dm];
				DLS=sl[dl];
				flag++;
				cm++;
				cl++;
				dm--;
				dl--;
				d3=0;
				d4=0;
			}
			else{
				flag=1;
				d3=1;
				d4=0;
				CMS=sm[cm];
				CLS=sl[cl];
				DMS=sm[dm];
				DLS=sl[dl];
			}

		}
		else if(d3==1&&d4==0){
			d3=1;
			d4=1;
			dir3=d3;
			dir4=d4;
			if(flag==1){
				cm=128;
				cl=128;
				dm=1;
				dl=1;
			}
			if(flag!=flagconst){
				CMS=sm[cm];
				CLS=sl[cl];
				DMS=sm[dm];
				DLS=sl[dl];
				flag++;
				cm--;
				cl--;
				dm++;
				dl++;
				d3=1;
				d4=0;
			}
			else{
				flag=1;
				d3=1;
				d4=1;
				CMS=sm[cm];
				CLS=sl[cl];
				DMS=sm[dm];
				DLS=sl[dl];
			}

		}
}


void service_timer0() interrupt 1 using 2{
	ET0=0;
	/*if(motor_start!=0){
		TH0=startH[motor_start];
		TL0=startL[motor_start];
		motor_start--;}
	else{
		TH0=0xd8;
		TL0=0x38;}*/
		
		TH0=0xfa;
		TL0=0xf0;
		
	if(motor_mod==0)
		//onesecond();
		;
	else if(motor_mod==1)
		motor_forward();		//T板 向外
	else if(motor_mod==2)
		motor_reverse();		//T板 向内
	else if(motor_mod==3)
		motor2_forward();		//L板 向内
	else if(motor_mod==4)
		motor2_reverse();		//L板 向外
	ET0=1;
}
void service_timer1() interrupt 3 using 2{
	ET1=0;
	TH1=0x4c;
	TL1=0x00;
	if(reset==1){
		if(time1_count<20){
			motor_mod=0;
			onesecond();
			d1=1;
			d2=0;
			time1_count++;
		}
		else if(time1_count>=20 & time1_count<180){			//20=1s	11s=220  8s=160
			if(time1_count==20)flag=0;
			motor_mod=1;									//A正 B正
			time1_count++;
		}
		else if(time1_count>=180  & time1_count<200 ){
			motor_mod=0;
			onesecond();
			d3=0;
			d4=1;
			time1_count++;
		}
		else if(time1_count>=200 & time1_count<360){		//
			if(time1_count==200)flag=0;
			motor_mod=4;
			time1_count++;
		}
		else{
			time1_count=0;
			reset=0;
		}
	}
	else{
		if(time1_count<20){
			motor_mod=0;
			onesecond();
			time1_count++;
		}
		else if(time1_count>=20  & time1_count<100 ){		//7.5s=150
			if(time1_count==20){
				phase1_reverse();
				flag=0;
			}
			motor_mod=2;									//A正 B负
			time1_count++;
		}
		else if(time1_count>=100 & time1_count<120){
			motor_mod=0;
			onesecond();
			time1_count++;
		}
		else if(time1_count>=120 & time1_count<200){
			if(time1_count==120){
				phase1_reverse();
				flag=0;
			}
			motor_mod=1;
			time1_count++;
		}
		
		else if(time1_count>=200 & time1_count<220){
			motor_mod=0;
			onesecond();
			time1_count++;
		}
		else if(time1_count>=220  & time1_count<300 ){
			if(time1_count==220){
				phase2_reverse();
				flag=0;
			}
			motor_mod=3;
			time1_count++;
		}
		else if(time1_count>=300 & time1_count<320){
			motor_mod=0;
			onesecond();
			time1_count++;
		}
		else if(time1_count>=320 & time1_count<400){
			if(time1_count==320){
				phase2_reverse();
				flag=0;
			}
			motor_mod=4;
			time1_count++;
		}
		else time1_count=0;
	}
	ET1=1;
}

main(){

	TMOD=TMOD&0xf0;
	TMOD=TMOD|0x01;
	TH0=0xfa;		//定时器0，方式1，11.25ms d780
	TL0=0xf0;		//11.05ms  d838
	
	TMOD=TMOD&0x0f;
	TMOD=TMOD|0x10;
	TH1=0x4c;
	TL1=0x00;
	
	EA=1;
	ET0=1;
	TR0=1;
	ET1=1;
	TR1=1;
	
	//dir1=d1;
	//dir2=d2;
	//dir3=d3;
	//dir4=d4;

	while(1){

	}
}
