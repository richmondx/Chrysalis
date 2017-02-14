/**
Declares the actor movement controller class.
**/
#pragma once

#include <IGameObject.h>
#include <IMovementController.h>


class CActor;
struct SActorMovementRequest;


class CActorMovementController : public IMovementController
{
public:

	// ***
	// *** IMovementController
	// ***


	/**
	Once an input system has determined the type of movement required, it calls a RequestMovement routine.
	RequestMovement will aggregate the new request with the current movements for this controller. Input is sanitised,
	aggregated, and generally tweaked. If that results in a valid new state for the movement controller, then the new
	state is stored as a member for processing further down the chain.

	\param [in,out]	request The request.

	\return true if it succeeds, false if it fails.
	**/
	bool RequestMovement(CMovementRequest& request) override;


	/**
	Gets movement state.

	\param [in,out]	state The state.
	**/
	ILINE void GetMovementState(SMovementState& state) override { state = m_movementState; };


	/**
	Gets stance state.

	Returns the description of the stance as if the specified stance would be set right now.

	If the parameter 'defaultPose' is set to false, the current aim and look target info is used to calculate the stance
	info, else a default pose facing along positive Y-axis is returned.

	Returns false if the description cannot be queried.

	\param	query		  The query.
	\param [in,out]	state The state.

	\return true if it succeeds, false if it fails.
	**/
	bool GetStanceState(const SStanceStateQuery& query, SStanceState& state) override;


	// ***
	// *** CActorMovementController
	// ***


	CActorMovementController(CActor* pActor);
	virtual ~CActorMovementController();


	/** Resets this object. */
	void OnResetState();


	/**
	Updates the movement parameters, based on our internal state.

	\param	frameTime The frame time.
	\param	movement    A variable-length parameters list containing parameters.
	**/
	void UpdateActorMovementRequest(SActorMovementRequest& movementRequest, float frameTime);


	/** Each frame we need to compute the acceptable values based on user input and store these
	for later retrieval. */
	void Compute();


	/**
	Full serialize.

	\param	ser The ser.
	**/
	void FullSerialize(TSerialize ser);

private:

	/** Calculates the movement request. */
	void ComputeMovementRequest();

	/** Recalculates / updates the movement state. This can take a while, so it's should be cached. */
	void ComputeMovementState();

	/** The actor. */
	CActor* m_pActor;

	/** The movement request. */
	CMovementRequest m_movementRequest;

	/** State of the movement. */
	SMovementState m_movementState;

	/** The desired speed. */
	float m_desiredSpeed;

	/** The look target. */
	Vec3 m_lookTarget;

	/** The aim target. */
	Vec3 m_aimTarget;

	/** The fire target. */
	Vec3 m_fireTarget;

	/** true to use the look at target. */
	bool m_bAtTarget;

	/** true to look using IK. */
	bool m_bUsingLookIK;

	/** true to using aim IK. */
	bool m_bUsingAimIK;

	/** true if aiming is clamped. */
	bool m_bAimClamped;

	/** The stance we would like to move into. */
	EStance m_targetStance;

	/** The animation target speed. */
	float m_animTargetSpeed { -1.0f };

	/** The animation target speed counter. */
	int m_animTargetSpeedCounter;

	/** true if the actor was turning. */
	bool m_bWasTurning;

	/** The time turning in same dir x coordinate. */
	float m_timeTurningInSameDirX { 0.0f };

	/** The time turning in same dir z coordinate. */
	float m_timeTurningInSameDirZ { 0.0f };

	/** The last request turn speed. */
	float m_lastReqTurnSpeed;

	/** The rotation rate around the Z coordinate axis. */
	float m_rotationRateZAxis;


	/**
	Determines a suitable rotation for the lower body based on the movement direction being requested. This should
	typically be combined with look / aim poses or perhaps an inverse of this to turn the upper body suitably.
	
	a
	\return The body rotation in degrees relative to the movement direction in the state flags.
	**/
	const float GetLowerBodyRotation(uint32 movementStateFlags) const;
};