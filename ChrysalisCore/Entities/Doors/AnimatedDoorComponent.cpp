#include <StdAfx.h>

#include "AnimatedDoorComponent.h"
#include <CrySerialization/Decorators/Resources.h>
#include <Components/Interaction/EntityInteractionComponent.h>
#include <Components/Geometry/GeometryComponent.h>
#include <Components/Lockable/LockableComponent.h>
#include <Components/Animation/ControlledAnimationComponent.h>


CRYREGISTER_CLASS(CAnimatedDoorComponent)

class CDoorRegistrator : public IEntityRegistrator
{
	void Register() override
	{
		RegisterEntityWithDefaultComponent<CAnimatedDoorComponent>("AnimatedDoor", "Doors", "Light.bmp");
	}
};

CDoorRegistrator g_doorRegistrator;


void CAnimatedDoorComponent::Initialize()
{
	auto pEntity = GetEntity();
	
	// Get some geometry.
	m_pGeometryComponent = pEntity->CreateComponent<CGeometryComponent>();

	// Get a controllable animation component.
	m_pControlledAnimationComponent = pEntity->CreateComponent<CControlledAnimationComponent>();

	// Allow locking.
	m_lockableComponent = pEntity->CreateComponent<CLockableComponent>();

	// We want to supply interaction verbs.
	auto m_interactor = pEntity->GetOrCreateComponent<CEntityInteractionComponent>();
	if (m_interactor)
	{
		m_interactor->AddInteraction(std::make_shared<CInteractionOpenableOpen>(this));
		m_interactor->AddInteraction(std::make_shared<CInteractionOpenableClose>(this));
		m_interactor->AddInteraction(std::make_shared<CInteractionLockableLock>(this));
		m_interactor->AddInteraction(std::make_shared<CInteractionLockableUnlock>(this));
	}

	OnResetState();
}


void CAnimatedDoorComponent::SerializeProperties(Serialization::IArchive& archive)
{
	if (archive.isInput())
	{
		OnResetState();
	}
}


void CAnimatedDoorComponent::OnResetState()
{
}
