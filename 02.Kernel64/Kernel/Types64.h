#ifndef __TYPES_H__
#define __TYPES_H__

#define BYTE	unsigned char
#define BOOL	unsigned char
#define WORD	unsigned short
#define DWORD	unsigned int
#define QWORD	unsigned long

#define TRUE	1
#define FALSE	0
#define NULL	0x00

#pragma pack(push, 1)
typedef struct kChracterStruct
{
	BYTE bCharacter;
	BYTE bAttribute;
} CHARACTER;
#pragma pack(pop)
#endif /* __TYPE_H__ */
