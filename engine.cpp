#include "engine.h"
#include "memory.h"

uint8_t* GameBase = NULL;
uint8_t** GName = NULL;
uint8_t** GWorld = NULL;

ULONG64 Uworld = NULL;
ULONG64 OwningGameInstance = NULL;
ULONG64 LocalPlayers = NULL;
ULONG64 ULocalPlayer = NULL;

ULONG64 ViewportClient = NULL;
void** ViewPortClientVTable = NULL;

UObject* Font;
UObject* K2_DrawLineUFunc;
UObject* K2_DrawTextUFunc;

void(*OPostRender)(UObject* UGameViewportClient, UObject* Canvas) = nullptr;

void UObject::ProcessEvent(void* UFunction, void* Params)
{
	auto vtable = *reinterpret_cast<void***>(this);
	reinterpret_cast<void(*)(void*, void*, void*)>(vtable[68])(this, UFunction, Params);
}


void UObject::K2_DrawLine(FVector2D ScreenPositionA, FVector2D ScreenPositionB, FLOAT Thickness, FLinearColor Color)
{
	struct
	{
		FVector2D ScreenPositionA;
		FVector2D ScreenPositionB;
		FLOAT Thickness;
		FLinearColor Color;
	} Parameters;

	Parameters.ScreenPositionA = ScreenPositionA;
	Parameters.ScreenPositionB = ScreenPositionB;
	Parameters.Thickness = Thickness;
	Parameters.Color = Color;

	ProcessEvent(K2_DrawLineUFunc, &Parameters);
}

void UObject::K2_DrawText(FString RenderText, FVector2D ScreenPosition, FVector2D Scale, FLinearColor RenderColor, float Kerning, FLinearColor ShadowColor, FVector2D ShadowOffset, bool bCentreX, bool bCentreY, bool bOutlined, FLinearColor OutlineColor)
{
	struct {
		UObject* RenderFont; //UFont* 
		FString RenderText;
		FVector2D ScreenPosition;
		FVector2D Scale;
		FLinearColor RenderColor;
		float Kerning;
		FLinearColor ShadowColor;
		FVector2D ShadowOffset;
		bool bCentreX;
		bool bCentreY;
		bool bOutlined;
		FLinearColor OutlineColor;
	} Parameters;

	Parameters.RenderFont = Font;
	Parameters.RenderText = RenderText;
	Parameters.ScreenPosition = ScreenPosition;
	Parameters.Scale = Scale;
	Parameters.RenderColor = RenderColor;
	Parameters.Kerning = Kerning;
	Parameters.ShadowColor = ShadowColor;
	Parameters.ShadowOffset = ShadowOffset;
	Parameters.bCentreX = bCentreX;
	Parameters.bCentreY = bCentreY;
	Parameters.bOutlined = bOutlined;

	ProcessEvent(K2_DrawTextUFunc, &Parameters);
}

void PostRender(UObject* UGameViewportClient, UObject* canvas)
{
	do
	{
		/*std::string Name = Actor->NamePrivate.GetName();
		wchar_t  FName[100];
		swprintf(FName, 100, L"%hs", Name.c_str());*/
		
		wchar_t  FName[100];
		swprintf(FName, 100, L"%hs", "test");


		canvas->K2_DrawText(FString{ FName }, {200,500}, FVector2D{1.0f, 1.0f}, FLinearColor{1.f, 0.f, 0.f, 1.f}, 1.0f, FLinearColor{0, 0, 0, 0}, FVector2D{0, 0}, true, false, false, FLinearColor{0, 0, 0, 0});
		canvas->K2_DrawLine({ 100,200 }, FVector2D{ 200,300 }, 1, FLinearColor{ 1.f, 1.f, 0.f, 1.f });

	} while (false);

	OPostRender(UGameViewportClient, canvas);
}


void EngineInit()
{
	GameBase = (uint8_t*)GetModuleHandleA(0);
	GName = (uint8_t**)(GameBase + 0x58A4F80);
	GWorld = (uint8_t**)(GameBase + 0x5A26FF0);

	Uworld = Ram<ULONG64>((ULONG64)GWorld);
	OwningGameInstance = Ram<ULONG64>(Uworld + 0x180);
	LocalPlayers = Ram<ULONG64>(OwningGameInstance + 0x38);
	ULocalPlayer = Ram<ULONG64>(LocalPlayers);

	ViewportClient = Ram<ULONG64>(ULocalPlayer + 0x70);
	ViewPortClientVTable = Ram<void**>(ViewportClient);

	Font = (UObject*)Ram<ULONG64>((ULONG64)GameBase + 0x57C2BE0);//It is the This pointer of the function Font.
	K2_DrawLineUFunc = (UObject*)Ram<ULONG64>((ULONG64)GameBase +0x5A2A548);//It is the This pointer of the function K2_DrawLine.
	K2_DrawTextUFunc = (UObject*)Ram<ULONG64>((ULONG64)GameBase + 0x5A2A568);//It is the This pointer of the function K2_DrawTextUFunc.

	DWORD protecc;
	VirtualProtect(&ViewPortClientVTable[100], 8, PAGE_EXECUTE_READWRITE, &protecc);

	OPostRender = reinterpret_cast<decltype(OPostRender)>(ViewPortClientVTable[100]);
	ViewPortClientVTable[100] = &PostRender;
}




