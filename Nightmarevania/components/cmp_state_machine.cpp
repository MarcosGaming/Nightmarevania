#include "cmp_state_machine.h"

StateMachineComponent::StateMachineComponent(Entity* p) : _current_state(nullptr), Component(p) {}

void StateMachineComponent::update(double dt)
{
	if (_current_state != nullptr)
	{
		_current_state->execute(_parent, dt);
	}
}

void StateMachineComponent::addState(const std::string& name, std::shared_ptr<State> state) noexcept
{ 
	_states[name] = state; 
}

std::shared_ptr<State> StateMachineComponent::getState(const std::string& name) const noexcept
{
	auto found = _states.find(name);
	if (found != _states.end())
	{
		return found->second;
	}
	else
	{
		return nullptr;
	}
}

void StateMachineComponent::removeState(const std::string& name) noexcept
{
	_states.erase(name);
}

void StateMachineComponent::changeState(const std::string& name) noexcept
{
	auto found = _states.find(name);
	if (found != _states.end())
	{
		_current_state = found->second;
		_current_state_name = name;
	}
}