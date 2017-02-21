#include "StdAfx.h"

#include "DRSInteractionEntity.h"
#include <Components/Interaction/EntityInteractionComponent.h>
#include <Components/Interaction/DRSInteractionComponent.h>
#include <Components/Geometry/GeometryComponent.h>
#include <CrySerialization/Decorators/Resources.h>
#include <CrySerialization/Enum.h>


class DRSInteractionEntityRegistrator : public IEntityRegistrator
{
	virtual void Register() override
	{
		RegisterEntityWithDefaultComponent<CDRSInteractionEntity>("DRSInteractionEntity", "Interaction", "physicsobject.bmp", true);
	}
};

DRSInteractionEntityRegistrator g_DRSInteractionEntityRegistrator;

CRYREGISTER_CLASS(CDRSInteractionEntity);


void CDRSInteractionEntity::Initialize()
{
	GetEntity()->CreateComponent<CDRSInteractionComponent>();
	GetEntity()->CreateComponent<CGeometryComponent>();

	// Reset the entity.
	OnResetState();
}


void CDRSInteractionEntity::SerializeProperties(Serialization::IArchive& archive)
{
//	archive(m_propertyCount, "PropertyCount", "Number of Properties");

	if (archive.isInput())
	{
		OnResetState();
	}
}


void CDRSInteractionEntity::OnResetState()
{
}
