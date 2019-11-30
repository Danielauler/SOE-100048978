#include <wiringPi.h>
#include <unistd.h>
#include <signal.h>
#define SAIDA 7

int t1, t2, fase, pin, cnt;

void SG90_alarm(int sig)
{
	if(cnt>0)
	{
		if(fase==0)
		{
			ualarm(t1);
			difitalWrite(pin, HIGH);
			fase++;
		}
		else {
			ualarm(t1);
			difitalWrite(pin, LOW);
			fase = 0;
			cnt--;
		}
	}
}

void SG90_control(int wpin, int degree, int N)
{
	t1 = (100*degree+4)/9+1500;
	t2 = 20000-t1;
	pin = wpin;
	cnt = N;
	fase = 0;
	signal(SIGALRM, SG90_alarm);
	ualarm(1,0);
	while(cnt>0)
}

int main(void)
{
	wiringPiSetup();
	pinMode(SAIDA, OUTPUT);
	SG90_control(SAIDA, -90, 50);
	SG90_control(SAIDA, 90, 50);
	SG90_control(SAIDA, -90, 50);
	SG90_control(SAIDA, 90, 50);
	return 0;
}
