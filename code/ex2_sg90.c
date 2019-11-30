#include <wiringPi.h>
#include <unistd.h>
#define SAIDA 7

void SG90_control(int wpin, int degree, int N)
{
	int t1 = (50*degree+4)/9+1500;
	int t2 = 2000-t1;
	int i;
	while(N--)
	{
		difitalWrite(SAIDA, HIGH);
		usleep(t1);
		difitalWrite(SAIDA, LOW);
		usleep(t2);
	}
}

int main(void)
{
	wiringPiSetup();
	pinMode(SAIDA, OUTPUT);
	SG90_control(SAIDA, -90, 50);
	SG90_control(SAIDA, 90, 50);
	SG90_control(SAIDA, -90, 50);
	SG90_control(SAIDA, 90, 50);
	while(1);
	return 0;
}
