#pragma once

#include "components\cmp_decision_tree.h"
#include "components\cmp_state_machine.h"
#include <random>
#include <chrono>

class DistanceDecision : public BinaryDecision
{
private:
	std::shared_ptr<Entity> _player;
	float _distance;

protected:
	std::shared_ptr<DecisionTreeNode> getBranch(Entity* owner) override final
	{
		float dist = (float)sf::length(owner->getPosition() - _player->getPosition());
		if (dist < _distance)
		{
			return _trueNode;
		}
		else
		{
			return _falseNode;
		}
	}

public:
	DistanceDecision(std::shared_ptr<Entity> player, float distance, std::shared_ptr<DecisionTreeNode> trueNode, std::shared_ptr<DecisionTreeNode> falseNode)
		: _player(player), _distance(distance), BinaryDecision(trueNode, falseNode) { }
};

class HealthDecision : public BinaryDecision
{
private:
	std::shared_ptr<Entity> _boss;
	int _health;

protected:
	std::shared_ptr<DecisionTreeNode> getBranch(Entity* owner) override final
	{
		auto AI = _boss->get_components<BossAIComponent>()[0];
		if (AI->getHealthTaken() < _health)
		{
			return _trueNode;
		}
		else
		{
			return _falseNode;
		}
	}

public:
	HealthDecision(std::shared_ptr<Entity> boss, int health, std::shared_ptr<DecisionTreeNode> trueNode, std::shared_ptr<DecisionTreeNode> falseNode)
		: _boss(boss), _health(health), BinaryDecision(trueNode, falseNode) { }
};

class DeathDecision : public DecisionTreeNode
{
	void makeDecision(Entity* owner) override final
	{
		auto sm = owner->get_components<StateMachineComponent>();
		sm[0]->changeState("Death");
	}
};

class StationaryDecision : public DecisionTreeNode
{
public:
	void makeDecision(Entity *owner) override final 
	{
		auto decisionTree = owner->get_components<DecisionTreeComponent>();
		decisionTree[0]->setWaitTime(1.5f);
		auto sm = owner->get_components<StateMachineComponent>();
		sm[0]->changeState("Stationary");
	}
};

class SeekSlowDecision : public DecisionTreeNode
{
public:
	void makeDecision(Entity *owner) override final 
	{
		auto decisionTree = owner->get_components<DecisionTreeComponent>();
		decisionTree[0]->setWaitTime(1.0f);
		auto sm = owner->get_components<StateMachineComponent>();
		sm[0]->changeState("SeekSlow");
	}
};

class SeekFastDecision : public DecisionTreeNode
{
public:
	void makeDecision(Entity *owner) override final
	{
		auto decisionTree = owner->get_components<DecisionTreeComponent>();
		decisionTree[0]->setWaitTime(0.8f);
		auto sm = owner->get_components<StateMachineComponent>();
		sm[0]->changeState("SeekFast");
	}
};

class AttackDecision : public DecisionTreeNode
{
public:
	void makeDecision(Entity *owner) override final
	{
		auto decisionTree = owner->get_components<DecisionTreeComponent>();
		decisionTree[0]->setWaitTime(1.5f);
		auto sm = owner->get_components<StateMachineComponent>();
		sm[0]->changeState("Attack");
	}
};

class ReviveDecision : public DecisionTreeNode
{
public:
	void makeDecision(Entity *owner) override final
	{
		auto decisionTree = owner->get_components<DecisionTreeComponent>();
		decisionTree[0]->setWaitTime(2.0f);
		auto sm = owner->get_components<StateMachineComponent>();
		sm[0]->changeState("Revive");
	}
};