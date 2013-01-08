#ifndef XKILL_IO_WRITETIME_H
#define XKILL_IO_WRITETIME_H

//! A timestamp marking when a file was written to.
/*!
\ingroup xkill-mesh-io
*/
struct WriteTimeUTC
{
	int year;
	int month;
	int day;
	int hour;
	int min;
	int sec;

	WriteTimeUTC();
	~WriteTimeUTC();

	bool operator==(const WriteTimeUTC &right) const
	{
		if(year != right.year)
			return false;
		if(month != right.month)
			return false;
		if(day != right.day)
			return false;
		if(hour != right.hour)
			return false;
		if(min != right.min)
			return false;
		if(sec != right.sec)
			return false;

		return true;
	}
};

#endif //XKILL_IO_WRITETIME_H