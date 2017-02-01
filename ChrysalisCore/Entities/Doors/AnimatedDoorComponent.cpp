#include <StdAfx.h>

#include "AnimatedDoorComponent.h"
#include <Entities/Lockable/LockableComponent.h>
#include <CrySerialization/Decorators/Resources.h>


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
	
	m_pGeometryComponent = GetEntity()->CreateComponent<CGeometryComponent>();
	m_pControlledAnimationComponent = GetEntity()->CreateComponent<CControlledAnimationComponent>();
	m_lockableExtension = pEntity->CreateComponent<CLockableComponent>();

	// We want to supply interaction verbs.
	auto m_interactor = pEntity->GetOrCreateComponent<CEntityInteractionComponent>();
	if (m_interactor)
	{
		m_interactor->AddInteraction(std::make_shared<CInteractionOpen>(this));
		m_interactor->AddInteraction(std::make_shared<CInteractionClose>(this));
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
