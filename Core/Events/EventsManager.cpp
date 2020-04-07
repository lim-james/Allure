#include "EventsManager.h"

namespace Events {

	EventsManager::Callback::Callback() : subscribed(true) {}

	void EventsManager::Callback::Subscribe() {
		subscribed = true;
	}

	void EventsManager::Callback::Unsubscribe() {
		subscribed = false;
	}

	void EventsManager::EmptyCallback::operator()() const {
		if (subscribed && callback) callback();
	}

	void EventsManager::EventCallback::operator()(Event* event) const {
		if (subscribed && callback) callback(event);
	}

	EventsManager* EventsManager::instance = nullptr;

	EventsManager::~EventsManager() {
		for (auto const& queued : queuedEvents)
			if (queued.event)
				delete queued.event;

		for (auto& pair : emptyCallbacks)
			for (auto& callback : pair.second)
				delete callback;

		for (auto& pair : eventCallbacks)
			for (auto& callback : pair.second)
				delete callback;
	}

	EventsManager* EventsManager::GetInstance() {
		if (!instance)
			instance = new EventsManager;

		return instance;
	}

	void EventsManager::Destroy() {
		delete instance;
	}

	void EventsManager::UnsubscribeAll() {
		for (auto& pair : emptyCallbacks)
			for (auto& callback : pair.second)
				callback->Unsubscribe();

		for (auto& pair : eventCallbacks)
			for (auto& callback : pair.second)
				callback->Unsubscribe();
	}

	void EventsManager::Unsubscribe(std::string const& name) {
		for (auto& callback : emptyCallbacks[name])
			callback->Unsubscribe();

		for (auto& callback : eventCallbacks[name])
			callback->Unsubscribe();
	}

	void EventsManager::Trigger(std::string const& name) {
		for (auto& callback : emptyCallbacks[name])
			(*callback)();
	}

	void EventsManager::Trigger(std::string const& name, Event* const event) {
		for (auto& callback : emptyCallbacks[name])
			(*callback)();

		event->name = name;
		for (auto& callback : eventCallbacks[name])
			(*callback)(event);

		delete event;
	}

	void EventsManager::Queue(std::string const& name) {
		queuedEvents.push_back({ name, nullptr });
	}

	void EventsManager::Queue(std::string const& name, Event* const event) {
		queuedEvents.push_back({ name, event });
	}

	void EventsManager::TriggerQueued() {
		for (auto const& queued : queuedEvents) {
			if (queued.event)
				Trigger(queued.name, queued.event);
			else
				Trigger(queued.name);
		}

		queuedEvents.clear();
	}

	void EventsManager::Debug() const {
		int count = 0;
		Debug::Warn << "Empty callbacks:\n";

		for (auto& callback : emptyCallbacks) {
			Debug::Warn << callback.first << ": " << callback.second.size() << '\n';
			count += callback.second.size();
		}

		Debug::Warn << "Count: " << count << "\n\n";

		count = 0;
		Debug::Warn << "Event callbacks:\n";

		for (auto& callback : eventCallbacks) {
			Debug::Warn << callback.first << ": " << callback.second.size() << '\n';
			count += callback.second.size();
		}

		Debug::Warn << "Count: " << count << "\n\n";

		count = 0;
		Debug::Warn << "Empty context callbacks:\n";

		for (auto& context : emptyContextCallbacks) {
			Debug::Warn << context.first << ": ";
			for (auto& callback : context.second) {
				std::cout << callback.first << ", ";
				++count;
			}

			std::cout << '\n';
			Debug::Warn << "Count: " << count << "\n\n";
			count = 0;
		}

		count = 0;
		Debug::Warn << "Event context callbacks:\n";

		for (auto& context : eventContextCallbacks) {
			Debug::Warn << context.first << ": ";
			for (auto& callback : context.second) {
				std::cout << callback.first << ", ";
				++count;
			}

			std::cout << '\n';
			Debug::Warn << "Count: " << count << "\n\n";
			count = 0;
		}
	}

}
