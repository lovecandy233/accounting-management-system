#define _CRT_SECURE_NO_WARNINGS //消除vs中使用scanf()函数时引发的警告

#include <stdio.h>
#include <stdlib.h>
#include "menu.h"//导入数据定义头文件
#include "card_service.h"



int main()
{
	int nSelection = -1;

	printf("\n");
	printf("欢迎进入计费管理系统\n");

	do{
	outputMenu();//输出系统菜单
	scanf("%d",&nSelection);//输入菜单项编号
	switch(nSelection){
		case 1:{
			add();// printf("添加卡\n");
			break;
		}
		case 2:{
			query();//printf("查询卡\n");
			break;
		}
		case 3:{
			logon();// printf("上机\n");
			break;
		}
		case 4:{
			settle();//printf("下机\n");
			break;
		}
		case 5:{
			addMoney();//printf("充值\n");
			break;
		}
		case 6:{
			refundMoney();//printf("退费\n");
			break;
		}
		case 7:{
			printf("查询统计\n");
			break;
		}
		case 8:{
			annul();//printf("注销卡\n");
			break;
		}
		case 0:{
			exitApp();
			break;
		}
	default:printf("输入菜单序号错误！请选择(0~8)\n");break;
	}
	}while(nSelection != 0);
	
	system("pause");
	return 0;
}

