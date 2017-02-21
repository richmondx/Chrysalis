#pragma once

#include "Helpers/DesignerEntityComponent.h"
#include "Entities/Interaction/IEntityInteraction.h"

class CEntityInteractionComponent;


class CDRSInteractionComponent final : public CDesignerEntityComponent<>, public IEntityPropertyGroup, public IInteractionDRS
{
	CRY_ENTITY_COMPONENT_INTERFACE_AND_CLASS(CDRSInteractionComponent, "DRSInteractionComponent", 0x5D1C5884B9AD48C3, 0x89F0A9138C04454E);

	virtual ~CDRSInteractionComponent() {}

public:
	// IEntityComponent
	void Initialize() override;
	void ProcessEvent(SEntityEvent& event) override;
	virtual IEntityPropertyGroup* GetPropertyGroup() final { return this; }
	// ~IEntityComponent

	// IEntityPropertyGroup
	virtual const char* GetLabel() const override { return "DRS Component Properties"; }
	virtual void SerializeProperties(Serialization::IArchive& archive) override;
	// ~IEntityPropertyGroup

	// IDRSInteractionEntityComponent
	virtual void OnResetState() final;
	// IDRSInteractionEntityComponent
	
	// IInteractionDRS
	void OnInteractionDRS() override;
	// ~IInteractionDRS

private:
	/** An instance of an interaction component. */
	CEntityInteractionComponent* m_interactor { nullptr };

	/** The main verb for the DRS response. */
	string m_drsResponse;

	/** Track how many properties we need. */
	int m_propertyCount { 4 };

	/** Track how many properties we need. */
	int m_lastPropertyCount { m_propertyCount };
};
