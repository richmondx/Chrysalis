#include "StdAfx.h"

#include "CompassComponent.h"
#include <CrySerialization/Decorators/Resources.h>
#include <CrySerialization/Enum.h>


class CCompassEntityRegistrator : public IEntityRegistrator
{
	virtual void Register() override
	{
		RegisterEntityWithDefaultComponent<CCompassComponent>("CompassComponent", "Compass", "physicsobject.bmp", true);
	}
};

CCompassEntityRegistrator g_CompassEntityRegistrator;

CRYREGISTER_CLASS(CCompassComponent);


void CCompassComponent::Initialize()
{
	// Get some geometry.
	m_pGeometryComponent = GetEntity()->CreateComponent<CGeometryComponent>();

	// Get a controllable animation component.
	m_pControlledAnimationComponent = GetEntity()->CreateComponent<CControlledAnimationComponent>();

	// We want to supply interaction verbs.
	auto m_interactor = GetEntity()->GetOrCreateComponent<CEntityInteractionComponent>();
	if (m_interactor)
	{
		m_interactor->AddInteraction(std::make_shared<CInteractionInspect>(this));
		m_interactor->AddInteraction(std::make_shared<CInteractionPickup>(this));
		m_interactor->AddInteraction(std::make_shared<CInteractionDrop>(this));
	}

	CDesignerEntityComponent::Initialize();
}


void CCompassComponent::SerializeProperties(Serialization::IArchive& archive)
{
	if (archive.isInput())
	{
		OnResetState();
	}
}


void CCompassComponent::OnResetState()
{
}


void CCompassComponent::Pickup()
{
	gEnv->pLog->LogAlways("Pickup fired.");
}


void CCompassComponent::Drop()
{
	gEnv->pLog->LogAlways("Drop fired.");
}


void CCompassComponent::Inspect()
{
	gEnv->pLog->LogAlways("Inspect fired.");
}
