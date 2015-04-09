#ifndef MSG_DEFINE_H_
#define MSG_DEFINE_H_

#define MSG_CONTENT_MAX_LEN	(510)
#define MSG_HEAD_VALUE	(0xAF)
#define MSG_TAIL_VALUE  (0xFA)
#pragma pack(1)

typedef unsigned char UINT8;
typedef unsigned short UINT16;

/*上位机与单片机通信消息命令类型*/
typedef enum {
	CMD_TYPE_ACK		= 0x01,
	CMD_TYPE_CONNECT	= 0x02,
	CMD_TYPE_LOCK		= 0x03,
	CMD_TYPE_CLEAR		= 0x04,
	CMD_TYPE_SEND		= 0x05,
	CMD_TYPE_RECV		= 0x06,
	CMD_TYPE_RESEND		= 0x07,
	CMD_TYPE_BUTT
}CMD_TYPE;

#define COMMON_MSG_HEADER \
	UINT8	ucHead; \
	UINT8	ucCmdType; \
	UINT16	usReserved; \
	UINT16	usLength; 

#define COMMON_MSG_HEADER_LENGTH 6 

/*定义通用消息结构*/
typedef struct STU_COMMON_MSG{
	COMMON_MSG_HEADER
	UINT8	aucValue[2];
} COMMON_MSG;

/*连接命令*/
typedef struct STU_CONNECT_CMD_CONTENT
{
	UINT16	usChannel;
}CONNECT_CMD_CONTENT;

/*锁定命令*/
typedef struct STU_LOCK_CMD_CONTENT
{
	UINT16	usChannel;
	UINT16	usStartReciverId;
	UINT16	usEndRecverId;
}LOCK_CMD_CONTENT;

/*清楚数据命令*/
typedef struct STU_CLEAR_CMD_CONTENT
{
	UINT16	usChannel;
	UINT16	usStartReciverId;
	UINT16	usEndRecverId;
}CLEAR_CMD_CONTENT;

/*发送数据命令*/
typedef struct STU_SEND_CMD_CONTENT
{
	UINT16	usChannel;
	UINT16  usMsgNum;
	UINT8	aucMsg[512];
}SEND_CMD_CONTENT;

/*接收消息命令*/
typedef struct STU_RECV_CMD_CONTENT
{
	UINT8	aucMsg[512];
}RECV_CMD_CONTENT;

/*重发消息命令*/
typedef struct STU_RESEND_CMD_CONTENT
{
	UINT16	usChannel;
	UINT16  usMsgNum;
	UINT8	aucMsg[512];
}RESEND_CMD_CONTENT;

/*应答消息命令*/
typedef struct STU_ACK_CMD_CONTENT
{
	UINT16	usAckCmdType;
}ACK_CMD_CONTENT;

#pragma pack(4)
#endif 
