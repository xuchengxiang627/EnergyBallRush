// Fill out your copyright notice in the Description page of Project Settings.


#include "Energy/Public/Player/EnergyHUD.h"

#include "Blueprint/UserWidget.h"
#include "Energy/Public/UI/EnergyUserWidget.h"
#include "UI/OverlayWidget.h"
#include "UI/Controller/OverlayWidgetController.h"

void AEnergyHUD::InitOverlay(APlayerController* PC, APlayerState* PS)
{
	if (OverlayWidgetClass == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Overlay Widget Class is nullptr in AuraHUD"))
		return;
	}
	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
	OverlayWidget = Cast<UOverlayWidget>(Widget);

	UOverlayWidgetController* WidgetController = GetOverlayWidgetController(PC, PS);

	OverlayWidget->SetWidgetController(WidgetController);
	// // 在界面添加到屏幕之前，获得控制器之后，广播初始值
	WidgetController->BroadcastInitialValues();
	OverlayWidget->AddToViewport();
}

UOverlayWidgetController* AEnergyHUD::GetOverlayWidgetController(APlayerController* PC, APlayerState* PS)
{
	if (OverlayWidgetController == nullptr)
	{
		OverlayWidgetController = NewObject<UOverlayWidgetController>(this, OverlayWidgetControllerClass);
		OverlayWidgetController->SetWidgetControllerParams(PC, PS);
		OverlayWidgetController->BindCallbacksToDependencies();
	}
	return OverlayWidgetController;
}
