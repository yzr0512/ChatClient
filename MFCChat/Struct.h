/************************************************
File name：Struct.h
Create by：余志荣
CreateDate：2016-08-29
Use：此头文件包含程序用到的大部分结构体以及常量定义
Change by: 2016-08-29 由 余志荣 创建
************************************************/ 

#pragma once

// 各类字符串的上限大小
#define ID_MAX                          13
#define KEY_MAX                         17
#define NAME_MAX						50
#define EMAIL_MAX						33
#define FRIEND_MAX						50
#define MSG_SIZE_MAX					3358

// 注册
#define REGISTER                        5000
#define REGISTER_SUCCESS                5001
#define REGISTER_FAILED                 5002

// 登录
#define LOGIN                           5010
#define LOGIN_SUCCESS                   5011
//#define LOGIN_FAILED                    5012
//#define LOGIN_FORCE                     5013
#define LOGIN_OUT                       5014

// 更改信息 未用到
#define RESET_PASSWORD                  5020
#define RESET_PASSWORD_SUCCESS          5021
#define RESET_PASSWORD_FAILED           5022
#define RESET_USERNAME                  5023
#define RESET_USERNAME_SUCCESS          5024
#define RESET_USERNAME_FAILED           5025

// 聊天
#define CHATING_TEXT_MSG                5030

// 获取好友信息
#define GET_FRIEND_INFO                 5040	// 单个
#define GET_ALL_FRIEND_INFO             5041	// 全部
//#define UPDATE_FRIEND_INFO_BEGIN		5044
//#define UPDATE_FRIEND_INFO_FINISH		5045
#define GET_FRIEND_LIST                 5042
#define GET_STRANGER_INFO               5043

// 添加或删除好友
#define ADD_FRIEND_REQUEST				5050
#define ADD_FRIEND_ANSWER				5051
#define ADD_FRIEND_AGREE				5055
#define ADD_FRIEND_REFUSE				5056
#define ADD_FRIEND_FAILED               5053
#define DELETE_FRIEND                   5054

// 账号状态
#define SET_USER_STATUS					5064
#define STATUS_ONLINE                   5060
#define STATUS_BUSY                     5061
#define STATUS_HIDE                     5062
#define STATUS_OFFLINE                  5063

#define HEARTBEAT						5070

//#define MESSAGE_SYSTEM					5070

//***下面是通信用的结构体***
// 登录消息结构体
struct MSG_LOGIN
{
	int  nType; // LOGIN
	char nID[ID_MAX]; // 账号最多12位
	char Key[KEY_MAX]; // 密码最多16位
	int  nStatus;
};

// 转发消息结构体
struct MSG_TRANSPOND
{
	int  nType; // CHATING_TEXT_MSG
	char ToID[ID_MAX]; // 接收方
	char FromID[ID_MAX]; // 发送方
	char Date[11]; // yyyy-mm-dd
	char Time[9]; // hh-mm-ss
	char Data[200]; // 聊天消息
	int nReturn; // 附加信息
};

// 好友信息


typedef struct MSG_USERINFO
{
	int  nType; // GET_FRIEND_LIST
	int  nStatus; // 在线状态
	char nID[ID_MAX]; // ID
	char Name[NAME_MAX]; // 昵称
	char Email[EMAIL_MAX]; // 邮件 最多32位 ANSI编码 无中文 最后一位是‘\0’
	char Sex; // 性别 一位 m男性 f女性
	int  nAge; // 年龄
	char FriendList[FRIEND_MAX * ID_MAX]; // 最多50个好友 以|分割
}MSG_USERINFO;
// 注册
struct MSG_REGISTER
{
	int  nType; // REGISTER
	char nID[ID_MAX]; // ID 只用于返回
	char PassWord[KEY_MAX]; // 密码
	char Name[NAME_MAX]; // 昵称	
	char Email[EMAIL_MAX]; // 邮件
	char Sex; // 性别
	int  nAge; // 年龄

};
struct MSG_SYS
{
	int nType;
	char nID[ID_MAX];
	UINT nIDPrompt;
};
// 通用数据结构体 用于接收
struct MSG_T
{
	int  nType; // 
	char Data[4092]; // 
};

#define MESSAGE_HEAD  1000
typedef struct MSG_HEAD
{
	int nType; // 
	int nMsgSize; // 
}MSG_HEAD;

struct MSG_ADDFRIEND
{
	int nType;
	char ID1[ID_MAX];
	char ID2[ID_MAX];
	int nReturn; // 返回给客户端消息
};

typedef struct MSG_FRND_INFO
{
	int nType;
	int nNum; // 朋友数目
	int nStatus[FRIEND_MAX];
	char ListID[FRIEND_MAX][ID_MAX];
	char ListName[FRIEND_MAX][NAME_MAX];
}MSG_FRND_INFO;

//文件传输模块
// 传输请求
#define MESSAGE_FILE_REQUEST	6000
#define MESSAGE_FILE_AGREE		6010
#define MESSAGE_FILE_REFUSE		6020
typedef struct MSG_FILE_REQUEST
{
	int  nType;
	char ToID[ID_MAX];
	char FromID[ID_MAX];
	char FileName[512]; // 文件路径+文件名
	int  nFileSize; // 文件大小
	int  nFileID; // 由于文件名太长 所以用数字来代替
}MSG_FILE_REQUEST;

// 文件数据包
#define MESSAGE_FILE_CONTENT	6010
typedef struct MSG_FILE_CONTENT
{
	int  nType;
	char ToID[ID_MAX];	// 发送者ID
	char FromID[ID_MAX]; // 接收者ID
	int  nFileID; // 由于文件名太长 所以用数字来代替
	int  nPacketID; // 数据包编号
	char FileData[2000]; // 文件数据
}MSG_FILE_CONTENT;

// 本地记录
typedef struct FILE_RECORD
{
	bool bType; // true -- 发送  false -- 接收
	char cID[ID_MAX]; // 根据上面来决定ID的含义 若bType为true则ID为接收者
	char FilePath[512];	// 文件路径
	char FileName[512]; // 文件名
	int  nFileID; // 由于文件名太长 所以用数字来代替
	int  nTransType; // 传输状态 0-等待回应 1-进行中 2-暂停 3-已完成 4-取消
	int  nFileSize; // 文件总大小
	int  nProgress; // 文件传输进度 就是已经传输的大小
}FILE_RECORD;

