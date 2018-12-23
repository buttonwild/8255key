/*******************************/
/*          ������             */
/*******************************/
#include <stdio.h>
#include <conio.h>

#include "ApiExusb.h"
#pragma comment(lib,"ApiExusb.lib")
//unsigned short time[]={0xf5,0x4c,0xd9,0x2f,0xf7,0xe0,0xe8,0x99};//����ֵ�ĵ�λ
//unsigned short time1[]={0x0e,0x0d,0x0b,0x0b,0x9,0x8,0x7,0x7};//����ֵ�ĸ�λ
unsigned short time[3][7] = { {0xd1,0x92,0xac,0x73,0xee,0xc1,0xd0},{0xf5,0x4c,0xd9,0x2f,0xf7,0xe0,0xe8},{0x78,0xa6,0xed,0x97,0xfb,0x70,0xf4} };
unsigned short time1[3][7] = { {0x1d,0x1a,0x17,0x16,0x13,0x11,0x0f},{0x0e,0x0d,0x0b,0xb,0x9,0x8,0x7},{0x7,0x6,0x5,0x5,0x4,0x4,0x3} };
unsigned short double_tiger[2][17]={{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0},{7,6,7,6,7,6,5,3,5,6,6,7,6,5,3,5,2}};
unsigned short buildmusic[2][100] = {0};          //��Ϊ���㣬��һ�����������ڶ���������
//{3829,3404,3033,2863,2551,2272,2024,1945};//*8253����ͬ���ļ�������ֵ�����任��Ϊ����16����
//1000000/�����ױ��Ƶ��ֵ��������ֵ
char lcd2[16]={0xb6,0xa0,0xb6,0xa0,0xb6,0xa0,0xb1,0xe0,0xbc,0xad};
char lcd3[16]={0xb6,0xa0,0xb6,0xa0,0xb1,0xe0,0xbc,0xad,0xbd,0xe1,0xca,0xf9};
char lcd4[16]={0xb6,0xa0,0xb6,0xa0,0xb6,0xa0,0xb2,0xa5,0xb7,0xc5};
char lcd5[16]={0xb6,0xa0,0xb6,0xa0,0xb6,0xa0,0xd4,0xdd,0xcd,0xa3};
char lcd6[16]={0xb6,0xa0,0xd7,0xd4,0xb6,0xa8,0xd2,0xe5,0xd2,0xf4,0xc0,0xd6};
int bkn = 0;                                    //���������
int bsy = 0;					//�Ƿ�༭���ֱ�־λ
int bms = 0;                    //����������Ƿ񲥷ţ�������ͣ
void de_lay(unsigned short i, unsigned short j);//unsigned
void music(unsigned short k, unsigned short t);
void checkkey();
void clear();
void buildstart();
void buildend();
void buildmusicstart();
void buildmusiccut_or_con();
void rebuild() ;
void builddel() ;
void smusic();
void cmdsetup();
void datasetup();
byte n,m;
int sms=0;
void main()
{
	byte tone = 0;        //���е�����
	int i, k;
	if (!Startup())		/*���豸*/
	{
		printf("ERROR: Open Device Error!\n");
		return;
	}
	while (1)
	{
		checkkey();
		switch (n)                   //ɨ�赽���̱����  �����¼�
		{
		case 0x77: {m = 3; music(m, tone);         //bsy����  �༭ģʽ������Ǳ༭ģʽ����ȷ�ϵ��������浽���顣
			if (bsy) {
				buildmusic[0][bkn] = tone;
				buildmusic[1][bkn] = m;
				bkn++;
			}                                   
			break; }
		case 0x7b: {m = 4; music(m, tone);
			if (bsy) {
				buildmusic[0][bkn] = tone;
				buildmusic[1][bkn] = m;
				bkn++;
			} break;}
		case 0x7d: {m = 5; music(m, tone);
			if (bsy) {
				buildmusic[0][bkn] = tone;
				buildmusic[1][bkn] = m;
				bkn++; 
			}break;}
		case 0x7e: {m = 6; music(m, tone);
			if (bsy) {
				buildmusic[0][bkn] = tone;
				buildmusic[1][bkn] = m;
				bkn++; 
			}break;}
		case 0xb7: { tone++;
			if(tone==3){tone=0;}
			break; }             //�����ĸı�   ��һ��  ��һ
		case 0xbb: {m = 0; music(m, tone);
			if (bsy) {
				buildmusic[0][bkn] = tone;
				buildmusic[1][bkn] = m;
				bkn++; 
			}break;}
		case 0xbd: {m = 1; music(m, tone);
			if (bsy) {
				buildmusic[0][bkn] = tone;
				buildmusic[1][bkn] = m;
				bkn++; 
			}break;}
		case 0xbe: {m = 2; music(m, tone);
			if (bsy) {
				buildmusic[0][bkn] = tone;
				buildmusic[1][bkn] = m;
				bkn++;
			} break;}
		
		default:break;
		}   
		if (bms)
		{
			
			for (int i = 0; i < bkn; i++)
			{
				checkkey();
				if(bms)
				{ 
					music(buildmusic[1][i], buildmusic[0][i]); 
				}
			}
			bms=0;
		}
	}
	Cleanup();		/*�ر��豸*/
}
void de_lay(unsigned short i, unsigned short j)    //��ʱ
{
	PortWriteByte(0x280, i);		/*�������ֵ��λ*/
	Sleep(50);
	PortWriteByte(0x280, j);		/*�������ֵ��λ*/
}
void music(unsigned short k, unsigned short t)
{
	PortWriteByte(0x283, 0x36);//00010110,8254������,�ָߵ�λ����
	Sleep(10);
	de_lay(time[t][k], time1[t][k]);
	Sleep(10);
	PortWriteByte(0x28b, 0x80);//10000000��8255������
	Sleep(10);
	PortWriteByte(0x289, 0x03);//����8255B�ڣ���������
	Sleep(500);				/*��ʱ*/
	PortWriteByte(0x289, 0x00);//����8255B�ڣ���������
}
void checkkey() {
byte data;
	int keep;
	PortWriteByte(0x28b, 0x81);		/*����8255��A��C�ھ�Ϊ���*/
	PortWriteByte(0x28a, 0x0f);
	PortReadByte(0x28a, &data);
	n = data;
	if (n != 0x0f)
	{
		Sleep(50);
		PortWriteByte(0x28b, 0x88);
		PortWriteByte(0x28a, 0xf0);
		PortReadByte(0x28a, &data);
		keep = data;
		n = n | data;
		switch (n) {
		case 0xd7: {rebuild(); break; }
		case 0xdb: {builddel(); break; }
		case 0xdd: {smusic();	break; }
		case 0xde: {sms=!sms;	break; }
		case 0xe7: {buildstart(); 
			PortWriteByte(0x28b,0x80);	/*��8255Ϊ��ʽ0,C�����,A�����*/
			clear();
			PortWriteByte(0x288,0x90);
			cmdsetup();
			Sleep(10);
       for (int i=0;i<10;i++)
          {
            PortWriteByte(0x288,lcd2[i]);
            datasetup();  
           };break; }
		case 0xeb: {buildend();	 break; }
		case 0xed: {PortWriteByte(0x28b,0x80);	/*��8255Ϊ��ʽ0,C�����,A�����*/
			clear();
			PortWriteByte(0x288,0x90);
			cmdsetup();
			Sleep(10);
       for (int a=0;a<10;a++)
          {
            PortWriteByte(0x288,lcd4[a]);
            datasetup();  
           };
			buildmusicstart(); 	break; }
		case 0xee: {
			PortWriteByte(0x28b,0x80);	/*��8255Ϊ��ʽ0,C�����,A�����*/
			clear();
			PortWriteByte(0x288,0x90);
			cmdsetup();
			Sleep(10);
       for (int i=0;i<10;i++)
          {
            PortWriteByte(0x288,lcd5[i]);
            datasetup();  
           };
			buildmusiccut_or_con(); 	
			break; }
		default:break;
		}

	}
}
void buildstart()                    //��ʼ�༭����
{
	bsy = 1;
}
void buildend() {					//ֹͣ�༭����
	bsy = 0;
}
void buildmusicstart() {
	bms = 1;
}			//�༭���ֲ���
void buildmusiccut_or_con() {
	bms = !bms;
}		//�༭������ͣ
void rebuild() {					//�ؽ��༭����
	bkn = 0;
}
void builddel() {					//ɾ����һλ�༭��
	bkn--;
}
void smusic()
{
	PortWriteByte(0x28b,0x80);	/*��8255Ϊ��ʽ0,C�����,A�����*/
			clear();
			PortWriteByte(0x288,0x90);
			cmdsetup();
			Sleep(10);
       for (int a=0;a<10;a++)
          {
            PortWriteByte(0x288,lcd6[a]);
            datasetup();  
           };	
	sms=1;
	for (int i = 0; i < 17; i++)
			{
				checkkey();
				if(sms)
				{ 
					music(double_tiger[1][i], double_tiger[0][i]); 
				}
				
			}
}
void cmdsetup()
   {
     PortWriteByte(0x28a,0x00);
     Sleep(1);	
     PortWriteByte(0x28a,0x04);
     Sleep(1);	
     PortWriteByte(0x28a,0x00);
     Sleep(1);	
   }
void datasetup()
   {
     PortWriteByte(0x28a,0x01);
    Sleep(1);	
     PortWriteByte(0x28a,0x05);	
      Sleep(1);
      PortWriteByte(0x28a,0x01);	
     Sleep(1);
   }
void  clear()
{
     PortWriteByte(0x288,0x0c);	
      cmdsetup();

}