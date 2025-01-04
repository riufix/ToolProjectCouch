#pragma once

#include "Engine.h"
#include "UnrealEd.h"
#include "Modules/ModuleManager.h"
#include "Modules/ModuleInterface.h"

DECLARE_LOG_CATEGORY_EXTERN(EditorTools, Log, All);

class FEditorToolsModule : public IModuleInterface
{
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
};