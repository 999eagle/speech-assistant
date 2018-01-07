#include "pch.h"
#include "pulseaudio_manager.h"

PulseaudioManager::PulseaudioManager()
{
	this->threaded_mainloop = pa_threaded_mainloop_new();
	pa_threaded_mainloop_start(this->threaded_mainloop);
	this->mainloop_api = pa_threaded_mainloop_get_api(this->threaded_mainloop);
	this->context_props = pa_proplist_new();
	this->context = pa_context_new_with_proplist(this->mainloop_api, "speech-assistant", this->context_props);
	auto notify_callback = [](pa_context * context, void * userdata) { static_cast<PulseaudioManager *>(userdata)->ContextNotifyCallback(); };
	pa_threaded_mainloop_lock(this->threaded_mainloop);
	pa_context_set_state_callback(this->context, notify_callback, this);
	this->connecting = true;
	pa_context_connect(this->context, NULL, PA_CONTEXT_NOFLAGS, NULL);
	while (this->connecting)
		pa_threaded_mainloop_wait(this->threaded_mainloop);
	pa_threaded_mainloop_unlock(this->threaded_mainloop);
}

PulseaudioManager::~PulseaudioManager()
{
	if (this->is_connected)
	{
		pa_context_disconnect(this->context);
	}
	pa_context_unref(this->context);
	this->context = nullptr;
	pa_proplist_free(this->context_props);
	this->context_props = nullptr;
	pa_threaded_mainloop_stop(this->threaded_mainloop);
	pa_threaded_mainloop_free(this->threaded_mainloop);
	this->threaded_mainloop = nullptr;
	this->mainloop_api = nullptr;
}

void PulseaudioManager::ContextNotifyCallback()
{
	auto state = pa_context_get_state(this->context);
	switch (state)
	{
	case PA_CONTEXT_READY:
		printf("context ready\n");
		this->connecting = false;
		this->is_connected = true;
		break;
	case PA_CONTEXT_FAILED:
		printf("connection failed\n");
		this->connecting = false;
		break;
	}
	pa_threaded_mainloop_signal(this->threaded_mainloop, 0);
}
