//
// Created by Bruno on 11/09/2017.
//

#include <RtcDS3231.h>

RtcDS3231<TwoWire> Rtc(Wire);

class DS3231Access {

private:

public:

	DS3231Access() {
	}

	~DS3231Access() {
	}

	void init() {
		Rtc.Begin();
		RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
		if (!Rtc.IsDateTimeValid())
		{
#ifdef INFO
			Serial.println("RTC lost");
#endif 
			Rtc.SetDateTime(compiled);
		}

		if (!Rtc.GetIsRunning())
		{
#ifdef INFO
			Serial.println("RTC Run now");
#endif 
			Rtc.SetIsRunning(true);
		}

		RtcDateTime now = Rtc.GetDateTime();
		if (now < compiled)
		{
#ifdef INFO
			Serial.println("Update RTC");
#endif 
			Rtc.SetDateTime(compiled);
		}
		Rtc.Enable32kHzPin(false);
		Rtc.SetSquareWavePin(DS3231SquareWavePin_ModeNone);
	}

	RtcDateTime getDateTime() {
		RtcDateTime now = Rtc.GetDateTime();
		return now;
	}

	String getDateTimeStr()
	{
		RtcDateTime now = Rtc.GetDateTime();
		return String(now.Day()) + "/" + String(now.Month()) + "/" + String(now.Year()) + " " + String(now.Hour()) + ":" + String(now.Minute()) + ":" + String(now.Second());
	}

	String getDateTimeStrEn()
	{
		RtcDateTime now = Rtc.GetDateTime();
		return String(String(now.Year()) + "-" + String(now.Month()) + "-" + now.Day()) + " " + String(now.Hour()) + ":" + String(now.Minute()) + ":" + String(now.Second());
	}

	String getDateStr()
	{
		RtcDateTime now = Rtc.GetDateTime();
		return String(now.Day()) + "/" + String(now.Month()) + "/" + String(now.Year());
	}

	String getTimeStr() {
		RtcDateTime now = Rtc.GetDateTime();
		return String(now.Hour()) + ":" + String(now.Minute()) + ":" + String(now.Second());
	}

	float getTemperature() {
		RtcTemperature temp = Rtc.GetTemperature();
		return temp.AsFloat();
	}

	void getTime(int *hour, int *minute, int *seconde) {
		RtcDateTime now = Rtc.GetDateTime();
		if (now.Hour() >= 12)
			*hour = now.Hour() - 12;
		else
			*hour = now.Hour();

		*minute = now.Minute();
		*seconde = now.Second();
	}
};




