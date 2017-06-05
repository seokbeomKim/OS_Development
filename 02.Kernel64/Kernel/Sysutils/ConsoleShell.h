#ifndef __CONSOLE_SHELL_H__
#define __CONSOLE_SHELL_H__

#include <DataType/Types.h>

#define CMD_BUFFER_LIMIT	300
#define PROMPT_MESSAGE		"OS64 > "

typedef void (*CommandFunction) (const char* pcParameter);

#pragma pack (push, 1)

typedef struct kShellCommandEntryStruct {
	char* pcCommand;	// command 문자열
	char* pcHelp;		// command 도움말
	CommandFunction pfFunction;	// 커맨드 수행하는 함수의 포인터
} SHELLCOMMANDENTRY;

// 파라미터 처리 위한 자료구조
typedef struct kParameterListStruct {
	const char* pcBuffer;
	int iLength;
	int iCurrentPosition;
} PARAMETERLIST;

#pragma pack (pop)

void	kStartConsoleShell(void);
void	kExecuteCommand(const char* pcCommandBuffer);
void	kInitializeParameter(PARAMETERLIST* pstList, const char* pcParameter);
int		kGetNextParameter(PARAMETERLIST* pstList, char* pcParameter);

// 커맨드 처리 함수
void	kHelp(const char* pcParameterBuffer);
void	kCls(const char* pcParameterBuffer);
void	kShowTotalRAMSize(const char* pcParameterBuffer);
void	kStringToDecimalHexTest(const char* pcParameterBuffer);
void 	kShutdown(const char* pcParameterBuffer);
void	kSetTimer( const char* pcParameterBuffer );
void	kWaitUsingPIT( const char* pcParameterBuffer );
void	kReadTimeStampCounter( const char* pcParameterBuffer );
void	kMeasureProcessorSpeed( const char* pcParameterBuffer );
void	kShowDateAndTime( const char* pcParameterBuffer );
void	kCreateTestTask(const char* pcParameterBuffer);

#endif
