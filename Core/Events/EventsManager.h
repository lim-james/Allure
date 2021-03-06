#ifndef EVENTS_MANAGER_H
#define EVENTS_MANAGER_H

#include "Event.h"
#include "../Logger/Logger.h"

#include <string>
#include <map>
#include <vector>
#include <functional>

class EventsManager {

	class Callback {

	protected:

		bool subscribed;

	public:

		Callback();

		void Subscribe();
		void Unsubscribe();

	};

	class EmptyCallback : public Callback {

		std::function<void()> callback;

	public:

		template<typename Context>
		void Bind(void(Context::*_callback)(), Context* const _context);
		template<typename Context>
		void Bind(void(Context::*_callback)() const, Context* const _context);

		void operator()() const;

	};

	class EventCallback : public Callback {

		std::function<void(Events::Event*)> callback;

	public:

		template<typename Context>
		void Bind(void(Context::*_callback)(Events::Event*), Context* const _context);
		template<typename Context>
		void Bind(void(Context::*_callback)(Events::Event*) const, Context* const _context);

		void operator()(Events::Event* event) const;

	};

	static EventsManager* instance;

	std::map<std::string, std::vector<EmptyCallback*>> emptyCallbacks;
	std::map<std::string, std::vector<EventCallback*>> eventCallbacks;

	std::map<void*, std::map<std::string, EmptyCallback*>> emptyContextCallbacks;
	std::map<void*, std::map<std::string, EventCallback*>> eventContextCallbacks;

	struct QueuedEvent {
		std::string name;
		Events::Event* event;
	};

	std::vector<QueuedEvent> queuedEvents;

public:

	~EventsManager();

	static EventsManager* Get();
	static void Destroy();

	// subscribe empty callbacks
	template<typename Context>
	void Subscribe(std::string const& name, void(Context::*callback)(), Context* const context);
	template<typename Context>
	void Subscribe(std::string const& name, void(Context::*callback)() const, Context* const context);

	// subscribe event callbacks
	template<typename Context>
	void Subscribe(std::string const& name, void(Context::*callback)(Events::Event*), Context* const context);
	template<typename Context>
	void Subscribe(std::string const& name, void(Context::*callback)(Events::Event*) const, Context* const context);

	template<typename Context>
	void SubscribeContext(Context* const context);

	void UnsubscribeAll();
	void Unsubscribe(std::string const& name);
	template<typename Context>
	void UnsubscribeContext(Context* const context);
	template<typename Context>
	void Unsubscribe(std::string const& name, Context* const context);

	void Trigger(std::string const& name);
	void Trigger(std::string const& name, Events::Event* const event);

	template<typename Context>
	void TriggerContext(std::string const& name, Context* const context);
	template<typename Context>
	void TriggerContext(std::string const& name, Context* const context, Events::Event* const event);

	void Queue(std::string const& name);
	void Queue(std::string const& name, Events::Event* const event);

	void TriggerQueued();

	// TODO: REMOVE
	void Debug() const;

};

template<typename Context>
void EventsManager::EmptyCallback::Bind(void(Context::* _callback)(), Context * const _context) {
	callback = std::bind(_callback, _context);
}

template<typename Context>
void EventsManager::EmptyCallback::Bind(void(Context::* _callback)() const, Context * const _context) {
	callback = std::bind(_callback, _context);
}

template<typename Context>
inline void EventsManager::EventCallback::Bind(void(Context::* _callback)(Events::Event*), Context * const _context) {
	callback = std::bind(_callback, _context, std::placeholders::_1);
}

template<typename Context>
inline void EventsManager::EventCallback::Bind(void(Context::* _callback)(Events::Event*) const, Context * const _context) {
	callback = std::bind(_callback, _context, std::placeholders::_1);
}

template<typename Context>
void EventsManager::Subscribe(std::string const& name, void(Context::*callback)(), Context* const context) {
	auto& c = emptyContextCallbacks[(void*)context][name];

	if (c) {
		c->Subscribe();
	} else {
		EmptyCallback* ec = new EmptyCallback;
		ec->Bind(callback, context);

		emptyCallbacks[name].push_back(ec);
		c = ec;
	}
}

template<typename Context>
void EventsManager::Subscribe(std::string const& name, void(Context::*callback)() const, Context* const context) {
	auto& c = emptyContextCallbacks[(void*)context][name];

	if (c) {
		c->Subscribe();
	} else {
		EmptyCallback* ec = new EmptyCallback;
		ec->Bind(callback, context);

		emptyCallbacks[name].push_back(ec);
		c = ec;
	}
}

template<typename Context>
void EventsManager::Subscribe(std::string const& name, void(Context::*callback)(Events::Event*), Context* const context) {
	auto& c = eventContextCallbacks[(void*)context][name];

	if (c) {
		c->Subscribe();
	} else {
		EventCallback* ec = new EventCallback;
		ec->Bind(callback, context);

		eventCallbacks[name].push_back(ec);
		c = ec;
	}
}

template<typename Context>
void EventsManager::Subscribe(std::string const& name, void(Context::*callback)(Events::Event*) const, Context* const context) {
	auto& c = eventContextCallbacks[(void*)context][name];

	if (c) {
		c->Subscribe();
	} else {
		EventCallback* ec = new EventCallback;
		ec->Bind(callback, context);

		eventCallbacks[name].push_back(ec);
		c = ec;
	}
}

template<typename Context>
void EventsManager::SubscribeContext(Context* const context) {
	void* ptr = (void*)context;

	for (auto& pair : emptyContextCallbacks[ptr])
		pair.second->Subscribe();

	for (auto& pair : eventContextCallbacks[ptr])
		pair.second->Subscribe();
}

template<typename Context>
void EventsManager::UnsubscribeContext(Context* const context) {
	void* ptr = (void*)context;

	for (auto& pair : emptyContextCallbacks[ptr])
		pair.second->Unsubscribe();

	for (auto& pair : eventContextCallbacks[ptr])
		pair.second->Unsubscribe();
}

template<typename Context>
void EventsManager::Unsubscribe(std::string const& name, Context* const context) {
	void* ptr = (void*)context;
	emptyContextCallbacks[ptr][name]->Unsubscribe();
	eventContextCallbacks[ptr][name]->Unsubscribe();
}

template<typename Context>
void EventsManager::TriggerContext(std::string const& name, Context* const context) {
	void* ptr = (void*)context;
	if (emptyContextCallbacks.find(ptr) != emptyContextCallbacks.end()) {
		auto callback = emptyContextCallbacks[ptr][name];
		if (callback) (*callback)();
	}
}

template<typename Context>
void EventsManager::TriggerContext(std::string const& name, Context* const context, Events::Event* const event) {
	void* ptr = (void*)context;

	if (emptyContextCallbacks.find(ptr) != emptyContextCallbacks.end()) {
		auto callback = emptyContextCallbacks[ptr][name];
		if (callback) (*callback)();
	}

	event->name = name;

	if (eventContextCallbacks.find(ptr) != eventContextCallbacks.end()) {
		auto callback = eventContextCallbacks[ptr][name];
		if (callback) (*callback)(event);
	}

	delete event;
}

#endif

