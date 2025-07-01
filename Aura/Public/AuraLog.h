#pragma once

DECLARE_LOG_CATEGORY_EXTERN(Aura, Log, All);

#define LOG_POS (*FString(__FUNCTION__))

// 간단하게, 정수, 실수, bool, String만 사용
template<typename T>
void CheckValueOnScreen(const FString& ValueName, const T& Value);

template<typename T>
void CheckValueOnScreen(const FString& ValueName, const T& Value)
{
	if (GEngine)
	{
        FString Msg;

        if constexpr (std::is_same_v<T, bool>)
        {
            Msg = FString::Printf(TEXT("%s : %s"), *ValueName, Value ? TEXT("True") : TEXT("False"));
        }
        else if constexpr (std::is_floating_point_v<T>)
        {
            Msg = FString::Printf(TEXT("%s : %0.2f"), *ValueName, Value);
        }
        else if constexpr (std::is_integral_v<T>)
        {
            Msg = FString::Printf(TEXT("%s : %d"), *ValueName, Value);
        }
        else if constexpr (std::is_same_v<T, FString>)
        {
            Msg = FString::Printf(TEXT("%s : %s"), *ValueName, *Value);
        }
        else
        {
            Msg = "Int, Float, String Only";
        }

		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, Msg);
	}
}

#define SHOW_FUNC_NAME() ShowFuncNameOnScreen(ANSI_TO_TCHAR(__FUNCTION__))

inline void ShowFuncNameOnScreen(const TCHAR* FunctionName)
{
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString::Printf(TEXT("Function: %s"), FunctionName));
    }
}

