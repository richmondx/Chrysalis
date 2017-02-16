#include <StdAfx.h>

#include "ItemInteractionComponent.h"
#include <Components/Interaction/EntityInteractionComponent.h>
#include <Player/Player.h>
#include <Actor/Character/Character.h>


CRYREGISTER_CLASS(CItemInteractionComponent)


class CItemInteractionRegistrator : public IEntityRegistrator
{
	virtual void Register() override
	{
		// Register the class as a factory.
		RegisterEntityWithDefaultComponent<CItemInteractionComponent>("ItemInteractionComponent", "Items", "Item.bmp", true);

		// This should make the entity class invisible in the editor.
		auto cls = gEnv->pEntitySystem->GetClassRegistry()->FindClass("ItemInteractionComponent");
		cls->SetFlags(cls->GetFlags() | ECLF_INVISIBLE);
	}
};

CItemInteractionRegistrator g_ItemInteractionRegistrator;



// ***
// *** IEntityComponent
// ***


void CItemInteractionComponent::Initialize()
{
	// Required for 5.3 to call update.
	GetEntity()->Activate(true);

	// Add new verbs to the interactor.
	m_interactor = GetEntity()->GetOrCreateComponent<CEntityInteractionComponent>();
	if (m_interactor)
	{
		m_interactor->AddInteraction(std::make_shared<CInteractionItemInspect>(this));
		m_interactor->AddInteraction(std::make_shared<CInteractionItemPickup>(this));
		m_interactor->AddInteraction(std::make_shared<CInteractionItemDrop>(this));
		m_interactor->AddInteraction(std::make_shared<CInteractionItemToss>(this));
	}

	// Reset the entity.
	OnResetState();
}


void CItemInteractionComponent::ProcessEvent(SEntityEvent& event)
{
	CDesignerEntityComponent::ProcessEvent(event);

	switch (event.event)
	{
		case ENTITY_EVENT_UPDATE:
			Update();
			break;
	}
}


void CItemInteractionComponent::SerializeProperties(Serialization::IArchive& archive)
{
	if (archive.isInput())
	{
		OnResetState();
	}
}


void CItemInteractionComponent::OnInteractionItemInspect()
{
	gEnv->pLog->LogAlways("OnInteractionItemInspect fired.");
	if (auto pCharacter = CPlayer::GetLocalCharacter())
	{
		// Hack to see the entity move.
		GetEntity()->SetPos(pCharacter->GetEntity()->GetPos() + Vec3(0.0f, 0.0f, 1.5f));
	}
}


void CItemInteractionComponent::OnInteractionItemPickup()
{
	gEnv->pLog->LogAlways("OnInteractionItemPickup fired.");
}


void CItemInteractionComponent::OnInteractionItemDrop()
{
	gEnv->pLog->LogAlways("OnInteractionItemDrop fired.");
}


void CItemInteractionComponent::OnInteractionItemToss()
{
	gEnv->pLog->LogAlways("OnInteractionItemToss fired.");
}


// ***
// *** CItemInteractionComponent
// ***


void CItemInteractionComponent::OnResetState()
{
}


void CItemInteractionComponent::Update()
{
}
