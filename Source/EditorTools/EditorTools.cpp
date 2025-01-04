#include "EditorTools.h"

#include "Modules/ModuleManager.h"
#include "Modules/ModuleInterface.h"

#include "Misc/MessageDialog.h"
#include "Editor.h"
#include <Subsystems/EditorActorSubsystem.h>
#include "Arena/ArenaPlayerStart.h"
#include "GameFramework/PlayerStart.h"
#include <unordered_map>


IMPLEMENT_MODULE(FEditorToolsModule, EditorTools);

void FEditorToolsModule::StartupModule() {


	FMessageDialog::Open(EAppMsgCategory::Info,EAppMsgType::Ok, FText::FromString("EditorTools module has started!"));

	FEditorDelegates::BeginPIE.AddLambda([](bool bIsSimulating) {

		//FString message = "EditorTools is checking the spawns ! \n";

		TArray<AActor*> FoundActors = GEditor->GetEditorSubsystem<UEditorActorSubsystem>()->GetAllLevelActors();

		TArray<AArenaPlayerStart*> ArenaPlayerStarts;

		for (auto Actor : FoundActors)
		{

			APlayerStart* PlayerStart = Cast<APlayerStart>(Actor);

			if (PlayerStart == nullptr) continue;


			AArenaPlayerStart* ArenaPlayerStart = Cast<AArenaPlayerStart>(PlayerStart);

			if (ArenaPlayerStart == nullptr) continue;

			//message += Actor->GetName() + " : " + FString::Printf(TEXT("%d"), ArenaPlayerStart->SpawnPair) + "\n";

			ArenaPlayerStarts.Add(ArenaPlayerStart);
		}

		TArray<int> SpawnPairsNumbers;

		int numberOfPairs = 0;

		for (AArenaPlayerStart* start : ArenaPlayerStarts)
		{			
			if (start->SpawnPair+1 > numberOfPairs)
			{
				numberOfPairs = start->SpawnPair+1;
			}
		}

		for (int i = 0; i < numberOfPairs; i++)
		{
			SpawnPairsNumbers.Add(0);
		}
		

		for (AArenaPlayerStart* ArenaPlayerStart : ArenaPlayerStarts)
		{
			SpawnPairsNumbers[ArenaPlayerStart->SpawnPair]++;
		}

		FString ErrorString = "\n ERRORS DETECTED IN THE SPAWNS: \n";

		bool areProblems = false;

		for (int i = 0; i < SpawnPairsNumbers.Num(); i++)
		{
			if (SpawnPairsNumbers[i] > 2)
			{
				areProblems = true;

				ErrorString += "Pair " + FString::Printf(TEXT("%d"), i) + " has " + FString::Printf(TEXT("%d"), SpawnPairsNumbers[i]) + " spawns (only 2 needed)\n";
			}

			if (SpawnPairsNumbers[i] < 2)
			{
				areProblems = true;

				ErrorString += "Pair " + FString::Printf(TEXT("%d"), i) + " has " + FString::Printf(TEXT("%d"), SpawnPairsNumbers[i]) + " spawns (2 needed)\n";
			}
		}

		if (areProblems) {
			FMessageDialog::Open(EAppMsgCategory::Error, EAppMsgType::Ok, FText::FromString(ErrorString));
		}

		});
	

	UE_LOG(LogTemp, Warning, TEXT("EditorTools module has started!"));
}

void FEditorToolsModule::ShutdownModule()
{
	UE_LOG(LogTemp, Warning, TEXT("EditorTools module has shut down!"));
}
