#define _CRT_SECURE_NO_WARNINGS //����vs��ʹ��scanf()����ʱ�����ľ���

#include <stdio.h>
#include <stdlib.h>
#include "menu.h"//�������ݶ���ͷ�ļ�
#include "card_service.h"



int main()
{
	int nSelection = -1;

	printf("\n");
	printf("��ӭ����Ʒѹ���ϵͳ\n");

	do{
	outputMenu();//���ϵͳ�˵�
	scanf("%d",&nSelection);//����˵�����
	switch(nSelection){
		case 1:{
			add();// printf("��ӿ�\n");
			break;
		}
		case 2:{
			query();//printf("��ѯ��\n");
			break;
		}
		case 3:{
			logon();// printf("�ϻ�\n");
			break;
		}
		case 4:{
			settle();//printf("�»�\n");
			break;
		}
		case 5:{
			addMoney();//printf("��ֵ\n");
			break;
		}
		case 6:{
			refundMoney();//printf("�˷�\n");
			break;
		}
		case 7:{
			printf("��ѯͳ��\n");
			break;
		}
		case 8:{
			annul();//printf("ע����\n");
			break;
		}
		case 0:{
			exitApp();
			break;
		}
	default:printf("����˵���Ŵ�����ѡ��(0~8)\n");break;
	}
	}while(nSelection != 0);
	
	system("pause");
	return 0;
}

