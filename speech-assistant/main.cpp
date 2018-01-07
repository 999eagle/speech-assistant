#include "pch.h"

#include "pulseaudio_manager.h"

int main(int argc, char ** argv)
{
	auto pa_mgr = new PulseaudioManager();
	printf("pa initialized\n");
	printf("exiting\n");
	delete pa_mgr;
	return 0;
}
