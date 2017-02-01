#pragma once

#include "Helpers/DesignerEntityComponent.h"
#include <Entities/Interaction/EntityInteractionComponent.h>
#include <Components/Geometry/GeometryComponent.h>
#include <Components/Animation/ControlledAnimationComponent.h>


class CLockableComponent;


/**
An animated door.

\sa IEntityComponent
\sa IEntityPropertyGroup
\sa IInteractionContainer
**/
class CAnimatedDoorComponent : public CDesignerEntityComponent<>, public IEntityPropertyGroup, public IInteractionContainer
{
	CRY_ENTITY_COMPONENT_INTERFACE_AND_CLASS(CAnimatedDoorComponent, "AnimatedDoor", 0xD246E11FE7E248F0, 0xB512402908F84496)

public:
	// IEntityComponent
	void Initialize() override;
	struct IEntityPropertyGroup* GetPropertyGroup() override { return this; }
	// ~IEntityComponent
	
	// IEntityPropertyGroup
	const char* GetLabel() const override { return "AnimatedDoor Properties"; };
	void SerializeProperties(Serialization::IArchive& archive) override;
	// ~IEntityPropertyGroup

	// IInteractionContainer
	void ContainerOpen() override { gEnv->pLog->LogAlways("ContainerOpen fired."); };
	void ContainerClose() override { gEnv->pLog->LogAlways("ContainerClose fired."); };
	void ContainerLock() override { gEnv->pLog->LogAlways("ContainerLock fired."); };
	void ContainerUnlock() override { gEnv->pLog->LogAlways("ContainerUnlock fired."); };
	// ~IInteractionContainer

	// CAnimatedDoorComponent
	CAnimatedDoorComponent() {};
	virtual ~CAnimatedDoorComponent() {}

private:
	// Called on entity spawn, or when the state of the entity changes in Editor
	void OnResetState() override;

	/** Model for the geometry. */
	CGeometryComponent* m_pGeometryComponent { nullptr };

	/** Animation for the geometry. */
	CControlledAnimationComponent* m_pControlledAnimationComponent { nullptr };

	/** Doors should be lockable. */
	CLockableComponent* m_lockableExtension { nullptr };

	/** This entity should be interactive. */
	CEntityInteractionComponent* m_interactor { nullptr };
};