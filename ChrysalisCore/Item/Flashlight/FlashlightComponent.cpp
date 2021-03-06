#include <StdAfx.h>

#include "FlashlightComponent.h"
#include <GameXmlParamReader.h>
#include <Entities/Interaction/EntityInteractionComponent.h>
#include <CryAnimation/ICryAnimation.h>


CRYREGISTER_CLASS(CFlashlightComponent)


class CFlashlightRegistrator : public IEntityRegistrator
{
	virtual void Register() override
	{
		// Register the class as a factory.
//		gEnv->pGameFramework->RegisterFactory("Flashlight", static_cast<CFlashlightComponent*> (nullptr), false, static_cast<CFlashlightComponent*> (nullptr));
		RegisterEntityWithDefaultComponent<CFlashlightComponent>("FlashlightComponent", "Items", "physicsobject.bmp", true);

		// This should make the entity class invisible in the editor.
		//auto cls = gEnv->pEntitySystem->GetClassRegistry()->FindClass("CFlashlight");
		//cls->SetFlags(cls->GetFlags() | ECLF_INVISIBLE);
	}
};

CFlashlightRegistrator g_flashlightRegistrator;



// ***
// *** IEntityComponent
// ***


void CFlashlightComponent::Initialize()
{
	m_pGeometryComponent = GetEntity()->GetOrCreateComponent<CGeometryComponent>();
	m_pGeometryComponent->AddEventListener(this);
	m_pDynamicLightComponent = GetEntity()->GetOrCreateComponent<CDynamicLightComponent>();
	m_pDynamicLightComponent->AddEventListener(this);

	// We want to supply interaction verbs.
	m_interactor = GetEntity()->GetOrCreateComponent<CEntityInteractionComponent>();
	if (m_interactor)
	{
		auto switchToggleInteractPtr = std::make_shared<CInteractionSwitchToggle>(this);
		m_interactor->AddInteraction(switchToggleInteractPtr);

		auto switchOnInteractPtr = std::make_shared<CInteractionSwitchOn>(this);
		m_interactor->AddInteraction(switchOnInteractPtr);

		auto switchOffInteractPtr = std::make_shared<CInteractionSwitchOff>(this);
		m_interactor->AddInteraction(switchOffInteractPtr);

		auto inspectInteractPtr = std::make_shared<CInteractionInspect>(this);
		m_interactor->AddInteraction(inspectInteractPtr);

		auto interactPtr = std::make_shared<CInteractionInteract>(this);
		m_interactor->AddInteraction(interactPtr);

		auto pickupInteractPtr = std::make_shared<CInteractionPickup>(this);
		m_interactor->AddInteraction(pickupInteractPtr);

		auto dropInteractPtr = std::make_shared<CInteractionDrop>(this);
		m_interactor->AddInteraction(dropInteractPtr);
	}

	// Reset the entity.
	OnResetState();
}


void CFlashlightComponent::ProcessEvent(SEntityEvent& event)
{
	switch (event.event)
	{
		// Physicalize on level start for Launcher
		case ENTITY_EVENT_START_LEVEL:

			// Editor specific, physicalize on reset, property change or transform change
		case ENTITY_EVENT_RESET:
		case ENTITY_EVENT_EDITOR_PROPERTY_CHANGED:
		case ENTITY_EVENT_XFORM_FINISHED_EDITOR:
			OnResetState();
			break;
	}
}


void CFlashlightComponent::SerializeProperties(Serialization::IArchive& archive)
{
	archive(m_batteryLevel, "BatteryLevel", "Battery Level");
	archive(m_isSwitchedOn, "IsSwitchedOn", "Is Switched On?");

	if (archive.isInput())
	{
		OnResetState();
	}
}


// ***
// *** CFlashlightComponent
// ***


CFlashlightComponent::CFlashlightComponent()
{}


CFlashlightComponent::~CFlashlightComponent()
{
}


void CFlashlightComponent::OnResetState()
{
	// Check to see if the light should be on, and switch it on if needed.
	Switch(m_isSwitchedOn);
}


void CFlashlightComponent::OnGeometryResetState()
{
	SEntitySlotInfo slotInfo;
	const bool validSlot = GetEntity()->GetSlotInfo(m_pGeometryComponent->GetSlotId(), slotInfo);

	if (validSlot)
	{
		if (slotInfo.pStatObj)
		{
			// Grab the position and rotation of the helper. Light rotation is X forward, so we need to adjust it
			// for a Y forward scenario.
			m_lightLocalPosition = slotInfo.pStatObj->GetHelperPos("light_term");
			m_lightLocalRotation = kRightToForward * Quat(slotInfo.pStatObj->GetHelperTM("light_term")).GetNormalized();
		}
		else if (slotInfo.pCharacter)
		{
			IAttachmentManager *pAttachmentManager = slotInfo.pCharacter->GetIAttachmentManager();
			IAttachment *pAttachment = pAttachmentManager->GetInterfaceByName("light_term");

			if (pAttachment)
			{
				// #TODO: Get pos + rot for character.

				// Grab the position and rotation of the helper. Light rotation is X forward, so we need to adjust it
				// for a Y forward scenario.
				// kRightToForward * something
			}
		}
	}
}


void CFlashlightComponent::OnDynamicLightResetState()
{
	// Create a matrix for the scale, rotation and position of the light, locally.
	Matrix34 localMatrix = Matrix34(Vec3 (1.0f, 1.0f, 1.0f), m_lightLocalRotation, m_lightLocalPosition);

	// Move the light to the new position and rotation.
	m_pDynamicLightComponent->SetLocalTM(localMatrix);
}


//void CFlashlightComponent::GetSharedParameters(XmlNodeRef rootParams)
//{
//	m_itemFlashlightParameterShared = SharedParameters::GetSharedParameters<SItemFlashlightParameterShared>(
//		rootParams, "item", GetEntity()->GetClass()->GetName(), "flashlight");
//	m_dynamicLightParameterShared = SharedParameters::GetSharedParameters<SDynamicLight>(
//		rootParams, "item", GetEntity()->GetClass()->GetName(), "dynamic_light");
//}


//void CFlashlightComponent::LoadFromXML()
//{
//	// We use the class name to determine which XML file contains our parameters. The item system scan means it's
//	// location is already known.
//	auto pItemSystem = gEnv->pGameFramework->GetIItemSystem();
//	auto xmlFilename = pItemSystem->GetItemParamsDescriptionFile(GetEntity()->GetClass()->GetName());
//	XmlNodeRef rootParams = gEnv->pSystem->LoadXmlFromFile(xmlFilename);
//
//	if (rootParams)
//	{
//		CItem::GetSharedParameters(rootParams);
//		GetSharedParameters(rootParams);
//	}
//	else
//	{
//		GameWarning("Item description file %s doesn't exist for item %s", xmlFilename, GetEntity()->GetClass()->GetName());
//	}
//}


void CFlashlightComponent::ToggleSwitch()
{
	m_isSwitchedOn = !m_isSwitchedOn;
	Switch(m_isSwitchedOn);
}


void CFlashlightComponent::Switch(bool isSwitchedOn)
{
	m_isSwitchedOn = isSwitchedOn;

	if (isSwitchedOn)
	{
		m_pDynamicLightComponent->SetActive(true);
		m_pDynamicLightComponent->OnResetState();
	}
	else
	{
		m_pDynamicLightComponent->SetActive(false);
		m_pDynamicLightComponent->OnResetState();
	}
}
