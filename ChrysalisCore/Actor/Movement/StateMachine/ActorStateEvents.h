#pragma once

#include <StateMachine/StateMachine.h>
#include <Actor/ActorState.h>
#include <IActionMapManager.h>


class CActor;
class CMelee;


enum EActorStateEvent
{
	///< Pre-physics handling.
	ACTOR_EVENT_PREPHYSICSUPDATE = STATE_EVENT_CUSTOM,

	///< Update cycle handing.
	ACTOR_EVENT_UPDATE,

	///< Actor is now being played by the local player.
	ACTOR_EVENT_BECOME_LOCAL_PLAYER,

	///< Ragdoll.
	ACTOR_EVENT_RAGDOLL_PHYSICALIZED,

	///< Entry point for player driven actors.
	ACTOR_EVENT_ENTRY,

	///< Entry point for AI driven actors.
	ACTOR_EVENT_ENTRY_AI,

	///< Actor is dead.
	ACTOR_EVENT_DEAD,

	///< Actor is on the ground.
	ACTOR_EVENT_GROUND,

	///< Actor is falling.
	ACTOR_EVENT_FALL,

	///< Actor is jumping.
	ACTOR_EVENT_JUMP,

	///< #TODO: investigate this. Seems to help determine if you're on the ground or falling.
	ACTOR_EVENT_GROUND_COLLIDER_CHANGED,

	///< Enter ledge state?
	ACTOR_EVENT_LEDGE,

	///< Ledge animation has finished.
	ACTOR_EVENT_LEDGE_ANIM_FINISHED,

	///< Seems to be something to do with jumps, slides, sprints and button mashing.
	ACTOR_EVENT_INPUT,

	///< Allow an actor to fly.
	ACTOR_EVENT_FLY,

	///< The actor has switched their weapon.
	ACTOR_EVENT_WEAPONCHANGED,

	///< The actor has switched their stance.
	ACTOR_EVENT_STANCE_CHANGED,

	///< Play or stop a cut-scene for this actor.
	ACTOR_EVENT_CUTSCENE,

	// It seems to be used to go into an animation controlled state.
	ACTOR_EVENT_INTERACTIVE_ACTION,

	///< Don't use, except to chain more events to the end of this list.
	ACTOR_EVENT_LAST,
};


#define eActorStateFlags(f) \
	f(eActorStateFlags_PrePhysicsUpdateAfterEnter) \
	f(eActorStateFlags_DoUpdate) \
	f(eActorStateFlags_IsUpdating) \
	f(eActorStateFlags_Ground) \
	f(eActorStateFlags_Jump) \
	f(eActorStateFlags_Sprinting) \
	f(eActorStateFlags_SprintPressed) \
	f(eActorStateFlags_Swimming) \
	f(eActorStateFlags_InAir) \
	f(eActorStateFlags_PhysicsSaysFlying) \
	f(eActorStateFlags_Ledge) \
	f(eActorStateFlags_CurrentItemIsHeavy) \
	f(eActorStateFlags_InteractiveAction) \
	f(eActorStateFlags_OnLadder)
//f(eActorStateFlags_Sliding) \
//f(eActorStateFlags_ExitingSlide) \
//f(eActorStateFlags_NetSlide) \
//f(eActorStateFlags_NetExitingSlide) \
//f(eActorStateFlags_Spectator) \


AUTOENUM_BUILDFLAGS_WITHZERO(eActorStateFlags, eActorStateFlags_None);


#define LedgeTransitionList(func)	\
	func(eOLT_MidAir)				\
	func(eOLT_Falling)				\
	func(eOLT_VaultOver)			\
	func(eOLT_VaultOverIntoFall)	\
	func(eOLT_VaultOnto)			\
	func(eOLT_HighVaultOver)		\
	func(eOLT_HighVaultOverIntoFall)\
	func(eOLT_HighVaultOnto)		\
	func(eOLT_QuickLedgeGrab)		\


struct SLedgeTransitionData
{
	SLedgeTransitionData(const uint16 ledgeId)
		: m_ledgeTransition(eOLT_None)
		, m_nearestGrabbableLedgeId(ledgeId)
		, m_comingFromOnGround(false)
		, m_comingFromSprint(false)
	{}

	AUTOENUM_BUILDENUMWITHTYPE_WITHINVALID_WITHNUM(EOnLedgeTransition, LedgeTransitionList, eOLT_None, eOLT_MaxTransitions);

	EOnLedgeTransition m_ledgeTransition;
	uint16	m_nearestGrabbableLedgeId;
	bool		m_comingFromOnGround;
	bool		m_comingFromSprint;
};


struct SStateEventLedge : public SStateEvent
{
	explicit SStateEventLedge(const SLedgeTransitionData& ledgeData)
		:
		SStateEvent(ACTOR_EVENT_LEDGE)
	{
		AddData(ledgeData.m_ledgeTransition);
		AddData(ledgeData.m_nearestGrabbableLedgeId);
		AddData(ledgeData.m_comingFromOnGround);
		AddData(ledgeData.m_comingFromSprint);
	}

	SLedgeTransitionData::EOnLedgeTransition GetLedgeTransition() const { return static_cast<SLedgeTransitionData::EOnLedgeTransition> (GetData(0).GetInt()); }
	uint16 GetLedgeId() const { return (uint16) GetData(1).GetInt(); }
	bool GetComingFromOnGround() const { return GetData(2).GetBool(); }
	bool GetComingFromSprint() const { return GetData(3).GetBool(); }
};


struct SActorPrePhysicsData
{
	const float m_frameTime;
	const SActorMovementRequest m_movement;

	SActorPrePhysicsData(float frameTime, const SActorMovementRequest& movementRequest)
		: m_frameTime(frameTime), m_movement(movementRequest)
	{}

private:
	// DO NOT IMPLEMENT!!!
	SActorPrePhysicsData();
};


struct SStateEventActorMovementPrePhysics : public SStateEvent
{
	SStateEventActorMovementPrePhysics(const SActorPrePhysicsData* pPrePhysicsData)
		:
		SStateEvent(ACTOR_EVENT_PREPHYSICSUPDATE)
	{
		AddData(pPrePhysicsData);
	}

	const SActorPrePhysicsData& GetPrePhysicsData() const { return *static_cast<const SActorPrePhysicsData*> (GetData(0).GetPtr()); }

private:
	// DO NOT IMPLEMENT!!!
	SStateEventActorMovementPrePhysics();
};


struct SStateEventUpdate : public SStateEvent
{
	SStateEventUpdate(const float frameTime)
		:
		SStateEvent(ACTOR_EVENT_UPDATE)
	{
		AddData(frameTime);
	}

	const float GetFrameTime() const { return GetData(0).GetFloat(); }

private:
	// DO NOT IMPLEMENT!!!
	SStateEventUpdate();
};


struct SStateEventFly : public SStateEvent
{
	SStateEventFly(const uint8 flyMode)
		:
		SStateEvent(ACTOR_EVENT_FLY)
	{
		AddData(flyMode);
	}

	const int GetFlyMode() const
	{
		return GetData(0).GetInt();
	}

private:
	// DO NOT IMPLEMENT!
	SStateEventFly();
};


struct SStateEventInteractiveAction : public SStateEvent
{
	SStateEventInteractiveAction(const EntityId objectId, bool bUpdateVisibility, int interactionIndex = 0, float actionSpeed = 1.0f)
		:
		SStateEvent(ACTOR_EVENT_INTERACTIVE_ACTION)
	{
		AddData(static_cast<int> (objectId));
		AddData(bUpdateVisibility);
		AddData(actionSpeed);
		AddData(static_cast<int> (interactionIndex));
	}


	SStateEventInteractiveAction(const char* interactionName, bool bUpdateVisibility, float actionSpeed = 1.0f)
		:
		SStateEvent(ACTOR_EVENT_INTERACTIVE_ACTION)
	{
		AddData(interactionName);
		AddData(bUpdateVisibility);
		AddData(actionSpeed);
	}


	int GetObjectEntityID() const
	{
		if (GetData(0).m_type == SStateEventData::eSEDT_int)
		{
			return GetData(0).GetInt();
		}
		return 0;
	}


	const char* GetObjectInteractionName() const
	{
		if (GetData(0).m_type == SStateEventData::eSEDT_voidptr)
		{
			return static_cast<const char*> (GetData(0).GetPtr());
		}
		return NULL;
	}


	int GetInteractionIndex() const
	{
		if (GetData(3).m_type == SStateEventData::eSEDT_int)
		{
			return GetData(3).GetInt();
		}
		return 0;
	}


	bool GetShouldUpdateVisibility() const
	{
		return GetData(1).GetBool();
	}


	float GetActionSpeed() const
	{
		return GetData(2).GetFloat();
	}
};


struct SInputEventData
{
	enum EInputEvent
	{
		EInputEvent_Jump = 0,
		EInputEvent_Slide,
		EInputEvent_Sprint,
		EInputEvent_ButtonMashingSequence,
	};

	SInputEventData(EInputEvent inputEvent)
		: m_inputEvent(inputEvent), m_entityId(0), m_actionId(""), m_activationMode(0), m_value(0.0f)
	{}

	SInputEventData(EInputEvent inputEvent, EntityId entityId, const ActionId& actionId, int activationMode, float value)
		: m_inputEvent(inputEvent), m_entityId(entityId), m_actionId(actionId), m_activationMode(activationMode), m_value(value)
	{}

	EInputEvent m_inputEvent;
	const EntityId m_entityId;
	const ActionId m_actionId;
	const int m_activationMode;
	const float m_value;
};


struct SStateEventActorInput : public SStateEvent
{
	SStateEventActorInput(const SInputEventData* pInputEvent)
		: SStateEvent(ACTOR_EVENT_INPUT)
	{
		AddData(pInputEvent);
	}

	const SInputEventData& GetInputEventData() const { return *static_cast<const SInputEventData*> (GetData(0).GetPtr()); }
};


struct SStateEventStanceChanged : public SStateEvent
{
	SStateEventStanceChanged(const int targetStance)
		:
		SStateEvent(ACTOR_EVENT_STANCE_CHANGED)
	{
		AddData(targetStance);
	}

	ILINE int GetStance() const { return GetData(0).GetInt(); }
};


struct SStateEventJump : public SStateEvent
{
	enum EData
	{
		eD_PrePhysicsData = 0,
		eD_VerticalSpeedMofidier
	};

	SStateEventJump(const SActorPrePhysicsData& data, const float verticalSpeedModifier = 0.0f)
		:
		SStateEvent(ACTOR_EVENT_JUMP)
	{
		AddData(&data);
		AddData(verticalSpeedModifier);
	}

	ILINE const SActorPrePhysicsData& GetPrePhysicsEventData() const { return *static_cast<const SActorPrePhysicsData*> (GetData(eD_PrePhysicsData).GetPtr()); }
	ILINE float GetVerticalSpeedModifier() const { return (GetData(eD_VerticalSpeedMofidier).GetFloat()); }
};


struct SStateEventCutScene : public SStateEvent
{
	explicit SStateEventCutScene(const bool enable)
		:
		SStateEvent(ACTOR_EVENT_CUTSCENE)
	{
		AddData(enable);
	}

	ILINE bool IsEnabling() const { return (GetData(0).GetBool()); }
};


struct SStateEventGroundColliderChanged : public SStateEvent
{
	explicit SStateEventGroundColliderChanged(const bool bChanged)
		: SStateEvent(ACTOR_EVENT_GROUND_COLLIDER_CHANGED)
	{
		AddData(bChanged);
	}

	ILINE const bool OnGround() const { return GetData(0).GetBool(); }
};

