#pragma once
#include <list>
#include <map>
#include <memory>
#include <typeindex>

#include "Event.h"
#include "../Logger/Logger.h"


class IEventCallback
{
public:
	virtual ~IEventCallback() = default;

	void Execute(Event& e)
	{
		Invoke(e);
	}
private:
	virtual void Invoke(Event& e) = 0;
};

/** \brief contains callback function pointer, that needs to be invoked */
template <typename TOwner, typename TEvent>
class EventCallback: public IEventCallback
{
private:
	typedef void (TOwner::*CallbackFunction)(TEvent&);

	TOwner* ownerInstance;
	CallbackFunction callbackFunction;
	
	virtual void Invoke(Event& e) override
	{
		// Cast the event to the correct type and call the function
		std::invoke(callbackFunction, ownerInstance, static_cast<TEvent&>(e));
	}
	
public:
	EventCallback(TOwner* owner, CallbackFunction callback)
		: ownerInstance(owner), callbackFunction(callback) {}

	~EventCallback() override = default;
};

typedef std::list<std::unique_ptr<IEventCallback>> HandlerList;

class EventManager
{
public:
	EventManager(){	Logger::Info("EventManager Constructor called"); }
	
	~EventManager(){ Logger::Info("EventManager Destructor called"); }

	//Clear all subscribers
	void Reset() { subscribers.clear(); }
	
	/** \brief Subscribe to an event type <T> */
	template <typename TOwner,typename TEvent>
	void SubscribeToEvent(TOwner* ownerInstance, void (TOwner::*callbackFunction)(TEvent&))
	{
		if(!subscribers[typeid(TEvent)].get())
		{
			subscribers[typeid(TEvent)] = std::make_unique<HandlerList>();
		}
		auto subscriber = std::make_unique<EventCallback<TOwner, TEvent>>(ownerInstance, callbackFunction);
		subscribers[typeid(TEvent)]->push_back(std::move(subscriber));
	}

	/** \brief Emit an event of type <T> */
	template <typename TEvent, typename ...TArgs>
	void EmitEvent(TArgs&& ...args)
	{
		auto handlers = subscribers[typeid(TEvent)].get();
		if(handlers)
			//for (auto &handler : *handlers)
			//	handler->Execute(TEvent(std::forward<TArgs>(args)...));
			for(auto it = handlers->begin(); it != handlers->end(); ++it)
			{
				auto handler = it->get();
				TEvent event(std::forward<TArgs>(args)...);
				handler->Execute(event);
			}
	}

private:
	std::map<std::type_index, std::unique_ptr<HandlerList>> subscribers;
};
