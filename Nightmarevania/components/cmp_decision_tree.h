#pragma once

#include <ecm.h>
#include <memory>
#include <vector>

// Node in decision tree
class DecisionTreeNode
{
public:
	virtual ~DecisionTreeNode() = default;
	virtual void makeDecision(Entity *owner) = 0;
};

// Decision point with two nodes, true and false
class BinaryDecision : public DecisionTreeNode
{
protected:
	std::shared_ptr<DecisionTreeNode> _trueNode;
	std::shared_ptr<DecisionTreeNode> _falseNode;
	virtual std::shared_ptr<DecisionTreeNode> getBranch(Entity *owner) = 0;

public:
	BinaryDecision(std::shared_ptr<DecisionTreeNode> trueNode, std::shared_ptr<DecisionTreeNode> falseNode);
	void makeDecision(Entity *owner) override;
};

// Decision point with multiple nodes
class MultiDecision : public DecisionTreeNode
{
protected:
	std::vector<std::shared_ptr<DecisionTreeNode>> _childNodes;
	virtual std::shared_ptr<DecisionTreeNode> getBranch(Entity* owner) = 0;

public:
	MultiDecision(std::vector<std::shared_ptr<DecisionTreeNode>> &childNodes);
	void makeDecision(Entity* owner) override;
};

// Random binary decison
class RandomBinaryDecision : public BinaryDecision
{
protected:
	std::shared_ptr<DecisionTreeNode> getBranch(Entity* owner) final;

public:
	RandomBinaryDecision() = delete;
	explicit RandomBinaryDecision(std::shared_ptr<DecisionTreeNode> trueNode, std::shared_ptr<DecisionTreeNode> falseNode);
};

// Weighted binary decision
class WeightedBinaryDecision : public BinaryDecision
{
protected:
	std::shared_ptr<DecisionTreeNode> getBranch(Entity* owner) final;
	int _weightTrue;

public:
	WeightedBinaryDecision() = delete;
	explicit WeightedBinaryDecision(int, std::shared_ptr<DecisionTreeNode> trueNode, std::shared_ptr<DecisionTreeNode> falseNode);
};

// Random multiple decision
class RandomMultiDecision : public MultiDecision
{
protected:
	std::shared_ptr<DecisionTreeNode> getBranch(Entity* owner) override final;

public:
	RandomMultiDecision() = delete;
	explicit RandomMultiDecision(std::vector<std::shared_ptr<DecisionTreeNode>> &childNodes);
};

// Decision tree component
class DecisionTreeComponent : public Component
{
private:
	std::shared_ptr<DecisionTreeNode> _decisionTree;
	float _waitBeforeNextDecision;

public:
	DecisionTreeComponent() = delete;
	explicit DecisionTreeComponent(Entity *p, std::shared_ptr<DecisionTreeNode> decisionTree);
	void update(double) override;
	void render() override {}

	void setWaitTime(float);
};