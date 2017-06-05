#include <DeviceDriver/RTC_Controller.h>

void kReadRTCTime(BYTE* pbHour, BYTE* pbMinute, BYTE* pbSecond)
{
	BYTE bData;

	kWriteToPort(RTC_CMOSADDRESS, RTC_ADDRESS_HOUR);
	bData = kReadFromPort(RTC_CMOSDATA);
	*pbHour = RTC_BCDTOBINARY(bData);

	kWriteToPort(RTC_CMOSADDRESS, RTC_ADDRESS_MINUTE);
	bData = kReadFromPort(RTC_CMOSDATA);
	*pbMinute = RTC_BCDTOBINARY(bData);

	kWriteToPort(RTC_CMOSADDRESS, RTC_ADDRESS_SECOND);
	bData = kReadFromPort(RTC_CMOSDATA);
	*pbSecond = RTC_BCDTOBINARY(bData);
}


void kReadRTCDate(WORD* pwYear, BYTE* pbMonth, BYTE* pbDayOfMonth, BYTE* pbDayOfWeek) {
	BYTE bData;

	kWriteToPort(RTC_CMOSADDRESS, RTC_ADDRESS_YEAR);
	bData = kReadFromPort(RTC_CMOSDATA);
	*pwYear = RTC_BCDTOBINARY(bData) + 2000;

	kWriteToPort(RTC_CMOSADDRESS, RTC_ADDRESS_MONTH);
	bData = kReadFromPort(RTC_CMOSDATA);
	*pbMonth = RTC_BCDTOBINARY(bData);

	kWriteToPort(RTC_CMOSADDRESS, RTC_ADDRESS_DAYOFMONTH);
	bData = kReadFromPort(RTC_CMOSDATA);
	*pbDayOfMonth = RTC_BCDTOBINARY(bData);

	kWriteToPort(RTC_CMOSADDRESS, RTC_ADDRESS_DAYOFWEEK);
	bData = kReadFromPort(RTC_CMOSDATA);
	*pbDayOfWeek = RTC_BCDTOBINARY(bData);
}

char* kConvertDayOfWeekToString(BYTE bDayOfWeek)
{
	static char* vpcDayOfWeekString[8] = { "Error", "Sunday", "Monday",
			"Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" };

	if ( bDayOfWeek >= 8) {
		return vpcDayOfWeekString[0];
	};

	return vpcDayOfWeekString[bDayOfWeek];
}
