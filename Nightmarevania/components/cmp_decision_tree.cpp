#include "cmp_decision_tree.h"
#include <random>
#include <chrono>

// Binary decision point
BinaryDecision::BinaryDecision(std::shared_ptr<DecisionTreeNode> trueNode, std::shared_ptr<DecisionTreeNode> falseNode) : _trueNode(trueNode), _falseNode(falseNode) {};
void BinaryDecision::makeDecision(Entity* owner)
{
	getBranch(owner)->makeDecision(owner);
}

// Multiple decision point
MultiDecision::MultiDecision(std::vector<std::shared_ptr<DecisionTreeNode>> &childNodes) : _childNodes(childNodes) {}
void MultiDecision::makeDecision(Entity* owner)
{
	getBranch(owner)->makeDecision(owner);
}

// Random binary decision
RandomBinaryDecision::RandomBinaryDecision(std::shared_ptr<DecisionTreeNode> trueNode, std::shared_ptr<DecisionTreeNode> falseNode) : BinaryDecision(trueNode, falseNode) {}
std::shared_ptr<DecisionTreeNode> RandomBinaryDecision::getBranch(Entity* owner)
{
	// Set up random number generator
	static std::random_device rd;
	static std::default_random_engine e(rd());
	static std::uniform_int_distribution<int> dist(0, 1);
	// Random choice
	bool choice = dist(e) == 0;
	if (choice)
	{
		return _trueNode;
	}
	else
	{
		return _falseNode;
	}

}

// Weighted binary decision
WeightedBinaryDecision::WeightedBinaryDecision(int weight, std::shared_ptr<DecisionTreeNode> trueNode, std::shared_ptr<DecisionTreeNode> falseNode) : _weightTrue(weight), BinaryDecision(trueNode, falseNode) {}
std::shared_ptr<DecisionTreeNode> WeightedBinaryDecision::getBranch(Entity* owner)
{
	// Set up random number generator
	static std::random_device rd;
	static std::default_random_engine e(rd());
	static std::uniform_int_distribution<int> dist(0, 100);
	// Random choice
	if (dist(e) < _weightTrue)
	{
		return _trueNode;
	}
	else
	{
		return _falseNode;
	}

}

// Random multiple decision 
RandomMultiDecision::RandomMultiDecision(std::vector<std::shared_ptr<DecisionTreeNode>> &childNodes) : MultiDecision(childNodes) {}
std::shared_ptr<DecisionTreeNode> RandomMultiDecision::getBranch(Entity* owner)
{
	// Set up random number generator
	static std::random_device rd;
	static std::default_random_engine e(rd());
	static std::uniform_int_distribution<int> dist(0, _childNodes.size() -1);
	// Random choice
	return _childNodes[dist(e)];
}

// Decision Tree Component
DecisionTreeComponent::DecisionTreeComponent(Entity* p, std::shared_ptr<DecisionTreeNode> decisionTree) : _decisionTree(decisionTree), _waitBeforeNextDecision(0.0f), Component(p) {}
void DecisionTreeComponent::update(double dt)
{
	_waitBeforeNextDecision -= dt;
	if (_parent->isDead())
	{
		_waitBeforeNextDecision = 0.0f;
	}
	if(_waitBeforeNextDecision <= 0.0f)
	{
		_decisionTree->makeDecision(_parent);
	}
}

void DecisionTreeComponent::setWaitTime(float wait) { _waitBeforeNextDecision = wait; }