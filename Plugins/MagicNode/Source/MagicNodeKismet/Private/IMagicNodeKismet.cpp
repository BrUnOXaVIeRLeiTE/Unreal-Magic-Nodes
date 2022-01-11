//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
///			Copyright 2021 (C) Bruno Xavier B. Leite
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "IMagicNodeKismet.h"
#include "MagicNodeKismet.h"

#include "KMGC_KismetTypes.h"
#include "KMGC_NodeStyle.h"
#include "MagicNodeKismet_Shared.h"

#if WITH_EDITOR
#include "ISettingsContainer.h"
#include "ISettingsModule.h"
#include "ISettingsSection.h"
#endif

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define LOCTEXT_NAMESPACE "Synaptech"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class FMagicNodeKismet : public IMagicNodeKismet
{
  private:
    bool HandleSettingsSaved()
    {
#if WITH_EDITOR
        const auto &Settings = GetMutableDefault<UKMGC_Settings>();
        Settings->SaveConfig();
        return true;
#endif
        return false;
    }

    void RegisterSettings()
    {
#if WITH_EDITOR
        if (ISettingsModule *SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
        {
            ISettingsContainerPtr SettingsContainer = SettingsModule->GetContainer("Project");
            SettingsContainer->DescribeCategory(
                "Synaptech", LOCTEXT("SynaptechCategoryName", "Synaptech"),
                LOCTEXT("SynaptechCategoryDescription", "Configuration of Synaptech Systems."));

            ISettingsSectionPtr SettingsSection = SettingsModule->RegisterSettings(
                "Project", "Synaptech", "KMGC_Settings", LOCTEXT("KMGC_SettingsName", "Magic Node (C++) Settings"),
                LOCTEXT("KMGC_SettingsDescription", "Kismet Settings for the Magic Node Plugin"),
                GetMutableDefault<UKMGC_Settings>());

            if (SettingsSection.IsValid())
            {
                SettingsSection->OnModified().BindRaw(this, &FMagicNodeKismet::HandleSettingsSaved);
            }
        }
#endif
    }

    void UnregisterSettings()
    {
#if WITH_EDITOR
        if (ISettingsModule *SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
        {
            SettingsModule->UnregisterSettings("Project", "Synaptech", "KMGC_Settings");
        }
#endif
    }

  public:
    virtual void StartupModule() override
    {
        RegisterSettings();
        FKMGC_NodeStyle::Initialize();
    }

    virtual void ShutdownModule() override
    {
        FKMGC_NodeStyle::Shutdown();
        if (UObjectInitialized())
        {
            UnregisterSettings();
        }
    }

    virtual bool SupportsDynamicReloading() override
    {
        return false;
    }
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#undef LOCTEXT_NAMESPACE

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

IMPLEMENT_MODULE(FMagicNodeKismet, MagicNodeKismet);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
