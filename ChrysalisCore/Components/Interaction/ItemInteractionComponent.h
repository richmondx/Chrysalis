#pragma once

#include "Helpers/DesignerEntityComponent.h"
#include <Entities/Interaction/IEntityInteraction.h>

class CEntityInteractionComponent;


/**
Supplies interaction ability to game world items.

\sa CDesignerEntityComponent
\sa IEntityPropertyGroup
\sa IInteractionItem
**/
class CItemInteractionComponent : public CDesignerEntityComponent<>, public IEntityPropertyGroup, public IInteractionItem
{
	CRY_ENTITY_COMPONENT_INTERFACE_AND_CLASS(CItemInteractionComponent, "ItemInteractionComponent", 0xd7c88abd16d44f0f, 0xbf9c328ae7623a89)

public:
	// IEntityComponent
	void Initialize() override;
	uint64 GetEventMask() const override { return CDesignerEntityComponent::GetEventMask() | BIT64(ENTITY_EVENT_UPDATE); }
	void ProcessEvent(SEntityEvent& event) override;
	struct IEntityPropertyGroup* GetPropertyGroup() override { return this; }
	// ~IEntityComponent

	// IEntityPropertyGroup
	const char* GetLabel() const override { return "Item Interaction Properties"; };
	void SerializeProperties(Serialization::IArchive& archive) override;
	// ~IEntityPropertyGroup

	// IInteractionItem
	void OnInteractionItemInspect() override;;
	void OnInteractionItemPickup() override;;
	void OnInteractionItemDrop() override;;
	void OnInteractionItemToss() override;;
	// ~IInteractionItem


	// ***
	// *** CItemInteractionComponent
	// ***

	CItemInteractionComponent() {};
	virtual ~CItemInteractionComponent() {};

private:

	enum class InspectionState
	{
		eNone,
		eStarting,
		eInProgress,
		eDroped,
		eTossed,
		eCancelled,
	};

	InspectionState m_inspectionState { InspectionState::eNone };

	void OnResetState() override;
	void Update();

	CEntityInteractionComponent* m_interactor { nullptr };
};