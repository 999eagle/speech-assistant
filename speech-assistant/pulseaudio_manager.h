#pragma once

class PulseaudioManager
{
public:
	PulseaudioManager();
	~PulseaudioManager();
private:
	pa_threaded_mainloop * threaded_mainloop;
	pa_mainloop_api * mainloop_api;
	pa_proplist * context_props;
	pa_context * context;
	bool connecting;
	bool is_connected;

	void ContextNotifyCallback();
};
