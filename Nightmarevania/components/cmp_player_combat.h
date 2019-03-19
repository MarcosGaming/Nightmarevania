
#pragma once
#include <ecm.h>

class PlayerCombatComponent : public Component 
{
protected:
	bool _attacking;
	bool _defending;

public:
	PlayerCombatComponent() = delete;
	explicit PlayerCombatComponent(Entity* p);

	void update(double dt) override;
	void render() override {}

	bool isAttacking() const;
	void setAttacking(bool attacking);

	bool isDefending() const;
	void setDefending(bool defending);
};
