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
		for (const auto& queued : queuedEvents)
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

	void EventsManager::Unsubscribe(const std::string& name) {
		for (auto& callback : emptyCallbacks[name])
			callback->Unsubscribe();

		for (auto& callback : eventCallbacks[name])
			callback->Unsubscribe();
	}

	void EventsManager::Trigger(const std::string& name) {
		for (auto& callback : emptyCallbacks[name])
			(*callback)();
	}

	void EventsManager::Trigger(const std::string& name, Event* const event) {
		for (auto& callback : emptyCallbacks[name])
			(*callback)();

		event->name = name;
		for (auto& callback : eventCallbacks[name])
			(*callback)(event);

		delete event;
	}

	void EventsManager::Queue(const std::string& name) {
		queuedEvents.push_back({ name, nullptr });
	}

	void EventsManager::Queue(const std::string& name, Event* const event) {
		queuedEvents.push_back({ name, event });
	}

	void EventsManager::TriggerQueued() {
		for (const auto& queued : queuedEvents) {
			if (queued.event)
				Trigger(queued.name, queued.event);
			else
				Trigger(queued.name);
		}

		queuedEvents.clear();
	}

	void EventsManager::Debug() const {
		int count = 0;
		Console::Warn << "Empty callbacks:\n";

		for (auto& callback : emptyCallbacks) {
			Console::Warn << callback.first << ": " << callback.second.size() << '\n';
			count += callback.second.size();
		}

		Console::Warn << "Count: " << count << "\n\n";

		count = 0;
		Console::Warn << "Event callbacks:\n";

		for (auto& callback : eventCallbacks) {
			Console::Warn << callback.first << ": " << callback.second.size() << '\n';
			count += callback.second.size();
		}

		Console::Warn << "Count: " << count << "\n\n";

		count = 0;
		Console::Warn << "Empty context callbacks:\n";

		for (auto& context : emptyContextCallbacks) {
			Console::Warn << context.first << ": ";
			for (auto& callback : context.second) {
				std::cout << callback.first << ", ";
				++count;
			}

			std::cout << '\n';
			Console::Warn << "Count: " << count << "\n\n";
			count = 0;
		}

		count = 0;
		Console::Warn << "Event context callbacks:\n";

		for (auto& context : eventContextCallbacks) {
			Console::Warn << context.first << ": ";
			for (auto& callback : context.second) {
				std::cout << callback.first << ", ";
				++count;
			}

			std::cout << '\n';
			Console::Warn << "Count: " << count << "\n\n";
			count = 0;
		}
	}

}
