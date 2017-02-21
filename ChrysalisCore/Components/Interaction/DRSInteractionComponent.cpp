#include "StdAfx.h"

#include "DRSInteractionComponent.h"
#include <Components/Interaction/EntityInteractionComponent.h>
#include <CrySerialization/Decorators/Resources.h>
#include <CrySerialization/Enum.h>
#include <CryDynamicResponseSystem/IDynamicResponseSystem.h>


class DRSInteractionComponentRegistrator : public IEntityRegistrator
{
	virtual void Register() override
	{
		RegisterEntityWithDefaultComponent<CDRSInteractionComponent>("DRSInteractionComponent", "Interaction", "physicsobject.bmp", true);

		// This should make the entity class invisible in the editor.
		//auto cls = gEnv->pEntitySystem->GetClassRegistry()->FindClass("DRSInteractionComponent");
		//cls->SetFlags(cls->GetFlags() | ECLF_INVISIBLE);
	}
};

DRSInteractionComponentRegistrator g_DRSInteractionComponentRegistrator;

CRYREGISTER_CLASS(CDRSInteractionComponent);


void CDRSInteractionComponent::Initialize()
{
	// Add new verbs to the interactor.
	if (auto m_interactor = GetEntity()->GetOrCreateComponent<CEntityInteractionComponent>())
	{
		m_interactor->AddInteraction(std::make_shared<CInteractionDRS>(this));
	}

	// Reset the entity.
	OnResetState();
}


void CDRSInteractionComponent::ProcessEvent(SEntityEvent& event)
{
	switch (event.event)
	{
		// Start the level.
		case ENTITY_EVENT_START_LEVEL:

			// Properties might have changed.
		case ENTITY_EVENT_RESET:
		case ENTITY_EVENT_EDITOR_PROPERTY_CHANGED:
		case ENTITY_EVENT_XFORM_FINISHED_EDITOR:
			OnResetState();
			break;
	}
}


void CDRSInteractionComponent::SerializeProperties(Serialization::IArchive& archive)
{
	archive(m_drsResponse, "DRSResponse", "DRS Response (verb)");
	archive.doc("Verb to pass into DRS e.g. interaction_play_audio.");
	archive(m_propertyCount, "PropertyCount", "Number of Properties");
	archive.doc("The number of DRS properties this component should provide space for.");

	if (archive.isEdit())
	{
		if (archive.openBlock("properties", "Child Properties"))
		{
			// You can serialise a map / vector directly.
		}
	}
	else
	{
	}

	if (archive.isInput())
	{
		OnResetState();
	}
}


void CDRSInteractionComponent::OnResetState()
{
}


void CDRSInteractionComponent::OnInteractionDRS()
{
	if (m_drsResponse.IsEmpty())
	{
		CryLogAlways("Interaction DRS has no verb.");
	}
	else
	{
		CryLogAlways("Interaction DRS");

		// Default to self as the target entity.
		auto pTargetEntity = GetEntity();

		// Make sure the target entity has a DRS proxy.
		auto pDrsProxy = crycomponent_cast<IEntityDynamicResponseComponent*> (pTargetEntity->CreateProxy(ENTITY_PROXY_DYNAMICRESPONSE));

		// Create a context variable collection and populate it based on information from the target entity.
		DRS::IVariableCollectionSharedPtr pContextVariableCollection = gEnv->pDynamicResponseSystem->CreateContextCollection();

		// It might be useful to know which verb triggered the interaction.
		pContextVariableCollection->CreateVariable("Verb", CHashedString(m_drsResponse));

		// Bad example of adding more variables to the collection.
		//pContextVariableCollection->CreateVariable("CharacterId", static_cast<int>(GetEntityId()));

		// #HACK: For testing!
		// #TODO: We need a solidly thought out way to populate the variable collections using our entities.
		pContextVariableCollection->CreateVariable("MuffinMan", CHashedString("dooropen"));

		// Queue it and let the DRS handle it now.
		pDrsProxy->GetResponseActor()->QueueSignal(m_drsResponse, pContextVariableCollection);
	}
}
