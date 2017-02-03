#include <StdAfx.h>

#include "LockableComponent.h"


CRYREGISTER_CLASS(CLockableComponent)


class CLockableExtensionRegistrator : public IEntityRegistrator, public CLockableComponent::SExternalCVars
{
	virtual void Register() override
	{
		// Register the entity class.
		RegisterEntityWithDefaultComponent<CLockableComponent>("Lockable", "Locks", "Light.bmp");

		RegisterCVars();
	}

	void RegisterCVars()
	{
		REGISTER_CVAR2("entity_lockable_Debug", &m_debug, 0, VF_CHEAT, "Allow debug display.");
	}
};

CLockableExtensionRegistrator g_LockableExtensionRegistrator;


const CLockableComponent::SExternalCVars& CLockableComponent::GetCVars() const
{
	return g_LockableExtensionRegistrator;
}


void CLockableComponent::OnResetState()
{
}


void CLockableComponent::SerializeProperties(Serialization::IArchive& archive)
{
	archive(m_isLocked, "IsLocked", "IsLocked");

	if (archive.isInput())
	{
		OnResetState();
	}
}
