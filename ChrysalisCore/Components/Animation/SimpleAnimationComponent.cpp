#include "StdAfx.h"

#include "SimpleAnimationComponent.h"
#include <CrySerialization/Decorators/Resources.h>
#include <CrySerialization/Enum.h>
#include <CryAnimation/ICryAnimation.h>


class SimpleAnimationRegistrator : public IEntityRegistrator
{
	virtual void Register() override
	{
		RegisterEntityWithDefaultComponent<CSimpleAnimationComponent>("SimpleAnimationComponent", "Animation", "physicsobject.bmp", true);
	}
};

SimpleAnimationRegistrator g_SimpleAnimationRegistrator;

CRYREGISTER_CLASS(CSimpleAnimationComponent);


void CSimpleAnimationComponent::Initialize()
{
	// Look to see if there's some geometry. If so, we will assume it's the one this animation is paired with. All
	// geometry components will need to be added to the entity before any animation that depends on them.
	m_pGeometryComponent = GetEntity()->GetComponent<CGeometryComponent>();
	if (m_pGeometryComponent)
	{
		m_pGeometryComponent->AddEventListener(this);
		m_slotId = m_pGeometryComponent->GetSlotId();
	}

	CDesignerEntityComponent::Initialize();
}


void CSimpleAnimationComponent::ProcessEvent(SEntityEvent& event)
{
	CDesignerEntityComponent::ProcessEvent(event);

	switch (event.event)
	{
		case ENTITY_EVENT_HIDE:
			break;

		case ENTITY_EVENT_UNHIDE:
			break;
	}
}


void CSimpleAnimationComponent::SerializeProperties(Serialization::IArchive& archive)
{
	archive(m_animation, "Animation", "Animation");
	archive(m_animationSpeed, "Speed", "Speed");
	archive(m_bLoopAnimation, "Loop", "Loop");

	if (archive.isInput())
	{
		OnResetState();
	}
}


void CSimpleAnimationComponent::OnResetState()
{
	// If they requested animation, give them simple animation playback.
	if (m_animation.size() > 0)
	{
		if (auto* pCharacter = GetEntity()->GetCharacter(m_slotId))
		{
			CryCharAnimationParams animParams;
			animParams.m_fPlaybackSpeed = m_animationSpeed;
			animParams.m_nFlags = m_bLoopAnimation ? CA_LOOP_ANIMATION : 0;
			pCharacter->GetISkeletonAnim()->StartAnimation(m_animation, animParams);
		}
		else
		{
			gEnv->pLog->LogWarning("Animation playback on %s entity failed. No character found.", m_animation.c_str());
		}
	}

	// Notify listeners.
	for (auto& pListener : m_ListenersList)
	{
		pListener->OnSimpleAnimationResetState();
	}
}


void CSimpleAnimationComponent::OnGeometryResetState()
{
	if (m_pGeometryComponent)
	{
		m_slotId = m_pGeometryComponent->GetSlotId();
	}
}
