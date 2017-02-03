#pragma once

#include "Helpers/DesignerEntityComponent.h"


/**
A lock extension.

\sa IEntityComponent
\sa IEntityPropertyGroup
**/
class CLockableComponent : public CDesignerEntityComponent<>, public IEntityPropertyGroup
{
	CRY_ENTITY_COMPONENT_INTERFACE_AND_CLASS(CLockableComponent, "Lockable", 0x819A4630CA1840F9, 0xB4C7F93B3F13A69A)

public:
	// IEntityComponent
	struct IEntityPropertyGroup* GetPropertyGroup() override { return this; }
	// ~IEntityComponent

	// IEntityPropertyGroup
	const char* GetLabel() const override { return "Lockable Properties"; };
	void SerializeProperties(Serialization::IArchive& archive) override;
	// ~IEntityPropertyGroup

	// ***
	// *** CLockableComponent
	// ***

	CLockableComponent() {};
	virtual ~CLockableComponent() {};

	struct SExternalCVars
	{
		int m_debug;
	};
	const SExternalCVars &GetCVars() const;

private:
	bool m_isLocked { true };

	void OnResetState();
};