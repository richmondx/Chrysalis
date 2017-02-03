#pragma once

#include <CryCore/Platform/platform.h>


struct IInteraction
{
	virtual void OnInteractionStart() {};
	virtual void OnInteractionComplete() {};
	virtual void OnInteractionCancel() {};

	bool IsUseable() { return true; };
	virtual const string GetVerb() const { return "interaction_interact"; };
	virtual const string GetVerbUI() const { return "@" + GetVerb(); };

	bool IsEnabled() { return m_isEnabled; };
	void SetEnabled(bool isEnabled) { m_isEnabled = isEnabled; };
	bool IsHidden() { return m_isHidden; };
	void SetHidden(bool isHidden) { m_isHidden = isHidden; };

private:
	bool m_isEnabled { true };
	bool m_isHidden { false };
};
DECLARE_SHARED_POINTERS(IInteraction);


// ***
// *** Generic interactions e.g. "use", "interact". Good for when then is really only one
// *** option for interacting with an entity.
// ***

struct IInteractionInteract
{
	virtual void OnInteractionInteract() = 0;
};


class CInteractionInteract : public IInteraction
{
public:
	CInteractionInteract(IInteractionInteract* subject) { m_subject = subject; };

	const string GetVerb() const override { return "interaction_interact"; };
	void OnInteractionStart() override { m_subject->OnInteractionInteract(); };

private:
	IInteractionInteract* m_subject { nullptr };
};
DECLARE_SHARED_POINTERS(CInteractionInteract);


// ***
// *** Switches
// ***

struct IInteractionSwitch
{
	virtual void OnInteractionSwitchToggle() = 0;
	virtual void OnInteractionSwitchOff() = 0;
	virtual void OnInteractionSwitchOn() = 0;
};


class CInteractionSwitchToggle : public IInteraction
{
public:
	CInteractionSwitchToggle(IInteractionSwitch* subject) { m_subject = subject; };

	const string GetVerb() const override { return "interaction_switch_toggle"; };
	void OnInteractionStart() override { m_subject->OnInteractionSwitchToggle(); };

private:
	IInteractionSwitch* m_subject { nullptr };
};
DECLARE_SHARED_POINTERS(CInteractionSwitchToggle);


class CInteractionSwitchOn : public IInteraction
{
public:
	CInteractionSwitchOn(IInteractionSwitch* subject) { m_subject = subject; };

	const string GetVerb() const override { return "interaction_switch_on"; };
	void OnInteractionStart() override { m_subject->OnInteractionSwitchOn(); };

private:
	IInteractionSwitch* m_subject { nullptr };
};
DECLARE_SHARED_POINTERS(CInteractionSwitchOn);


class CInteractionSwitchOff : public IInteraction
{
public:
	CInteractionSwitchOff(IInteractionSwitch* subject) { m_subject = subject; };

	const string GetVerb() const override { return "interaction_switch_off"; };
	void OnInteractionStart() override { m_subject->OnInteractionSwitchOff(); };

private:
	IInteractionSwitch* m_subject { nullptr };
};
DECLARE_SHARED_POINTERS(CInteractionSwitchOff);


// ***
// *** Items which can be pickup up, inspected, dropped, etc.
// ***


struct IInteractionItem
{
	virtual void OnInteractionItemPickup() = 0;
	virtual void OnInteractionItemDrop() = 0;
	virtual void OnInteractionItemInspect() = 0;
};


class CInteractionItemPickup : public IInteraction
{
public:
	CInteractionItemPickup(IInteractionItem* subject) { m_subject = subject; };

	const string GetVerb() const override { return "interaction_pickup"; };
	void OnInteractionStart() override { m_subject->OnInteractionItemPickup(); };

private:
	IInteractionItem* m_subject { nullptr };
};
DECLARE_SHARED_POINTERS(CInteractionItemPickup);


class CInteractionItemDrop : public IInteraction
{
public:
	CInteractionItemDrop(IInteractionItem* subject) { m_subject = subject; };

	const string GetVerb() const override { return "interaction_drop"; };
	void OnInteractionStart() override { m_subject->OnInteractionItemDrop(); };

private:
	IInteractionItem* m_subject { nullptr };
};
DECLARE_SHARED_POINTERS(CInteractionItemDrop);


class CInteractionItemInspect : public IInteraction
{
public:
	CInteractionItemInspect(IInteractionItem* subject) { m_subject = subject; };

	const string GetVerb() const override { return "interaction_inspect"; };
	void OnInteractionStart() override { m_subject->OnInteractionItemInspect(); };

private:
	IInteractionItem* m_subject { nullptr };
};
DECLARE_SHARED_POINTERS(CInteractionItemInspect);


// ***
// *** Generic open.
// ***


struct IInteractionOpenable
{
	virtual void OnInteractionOpenableOpen() = 0;
	virtual void OnInteractionOpenableClose() = 0;
};


class CInteractionOpenableOpen : public IInteraction
{
public:
	CInteractionOpenableOpen(IInteractionOpenable* subject) { m_subject = subject; };

	const string GetVerb() const override { return "interaction_openable_open"; };
	void OnInteractionStart() override { m_subject->OnInteractionOpenableOpen(); };

private:
	IInteractionOpenable* m_subject { nullptr };
};
DECLARE_SHARED_POINTERS(CInteractionOpenableOpen);


class CInteractionOpenableClose : public IInteraction
{
public:
	CInteractionOpenableClose(IInteractionOpenable* subject) { m_subject = subject; };

	const string GetVerb() const override { return "interaction_openable_close"; };
	void OnInteractionStart() override { m_subject->OnInteractionOpenableClose(); };

private:
	IInteractionOpenable* m_subject { nullptr };
};
DECLARE_SHARED_POINTERS(CInteractionOpenableClose);


// ***
// *** Lockable.
// ***


struct IInteractionLockable
{
	virtual void OnInteractionLockableLock() = 0;
	virtual void OnInteractionLockableUnlock() = 0;
};


class CInteractionLockableLock : public IInteraction
{
public:
	CInteractionLockableLock(IInteractionLockable* subject) { m_subject = subject; };

	const string GetVerb() const override { return "interaction_lockable_lock"; };
	void OnInteractionStart() override { m_subject->OnInteractionLockableLock(); };

private:
	IInteractionLockable* m_subject { nullptr };
};
DECLARE_SHARED_POINTERS(CInteractionLockableLock);


class CInteractionLockableUnlock : public IInteraction
{
public:
	CInteractionLockableUnlock(IInteractionLockable* subject) { m_subject = subject; };

	const string GetVerb() const override { return "interaction_lockable_unlock"; };
	void OnInteractionStart() override { m_subject->OnInteractionLockableUnlock(); };

private:
	IInteractionLockable* m_subject { nullptr };
};
DECLARE_SHARED_POINTERS(CInteractionLockableUnlock);


//// ***
//// *** Doors.
//// ***
//
//
//struct IInteractionDoor
//{
//	virtual void OnInteractionDoorOpen() = 0;
//	virtual void OnInteractionDoorClose() = 0;
//};
//
//
//class CInteractionDoorOpen : public IInteraction
//{
//public:
//	CInteractionDoorOpen(IInteractionDoor* subject) { m_subject = subject; };
//
//	const string GetVerb() const override { return "interaction_door_open"; };
//	void OnInteractionStart() override { m_subject->OnInteractionDoorOpen(); };
//
//private:
//	IInteractionDoor* m_subject { nullptr };
//};
//DECLARE_SHARED_POINTERS(CInteractionDoorOpen);
//
//
//class CInteractionDoorClose : public IInteraction
//{
//public:
//	CInteractionDoorClose(IInteractionDoor* subject) { m_subject = subject; };
//
//	const string GetVerb() const override { return "interaction_door_close"; };
//	void OnInteractionStart() override { m_subject->OnInteractionDoorClose(); };
//
//private:
//	IInteractionDoor* m_subject { nullptr };
//};
//DECLARE_SHARED_POINTERS(CInteractionDoorClose);
//
//
//// ***
//// *** Containers.
//// ***
//
//
//struct IInteractionContainer
//{
//	virtual void OnInteractionContainerOpen() = 0;
//	virtual void OnInteractionContainerClose() = 0;
//	virtual void OnInteractionContainerLock() = 0;
//	virtual void OnInteractionContainerUnlock() = 0;
//};
//
//
//class CInteractionContainerOpen : public IInteraction
//{
//public:
//	CInteractionContainerOpen(IInteractionContainer* subject) { m_subject = subject; };
//
//	const string GetVerb() const override { return "interaction_container_open"; };
//	void OnInteractionStart() override { m_subject->OnInteractionContainerOpen(); };
//
//private:
//	IInteractionContainer* m_subject { nullptr };
//};
//DECLARE_SHARED_POINTERS(CInteractionContainerOpen);
//
//
//class CInteractionContainerClose : public IInteraction
//{
//public:
//	CInteractionContainerClose(IInteractionContainer* subject) { m_subject = subject; };
//
//	const string GetVerb() const override { return "interaction_container_close"; };
//	void OnInteractionStart() override { m_subject->OnInteractionContainerClose(); };
//
//private:
//	IInteractionContainer* m_subject { nullptr };
//};
//DECLARE_SHARED_POINTERS(CInteractionContainerClose);
//
//
//class CInteractionContainerLock : public IInteraction
//{
//public:
//	CInteractionContainerLock(IInteractionContainer* subject) { m_subject = subject; };
//
//	const string GetVerb() const override { return "interaction_container_lock"; };
//	void OnInteractionStart() override { m_subject->OnInteractionContainerLock(); };
//
//private:
//	IInteractionContainer* m_subject { nullptr };
//};
//DECLARE_SHARED_POINTERS(CInteractionContainerLock);
//
//
//class CInteractionContainerUnlock : public IInteraction
//{
//public:
//	CInteractionContainerUnlock(IInteractionContainer* subject) { m_subject = subject; };
//
//	const string GetVerb() const override { return "interaction_container_unlock"; };
//	void OnInteractionStart() override { m_subject->OnInteractionContainerUnlock(); };
//
//private:
//	IInteractionContainer* m_subject { nullptr };
//};
//DECLARE_SHARED_POINTERS(CInteractionContainerUnlock);
