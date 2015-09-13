// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"

#if _MSC_VER
#define FUNC_NAME    __FUNCTION__
#else // FIXME - GCC?
#define FUNC_NAME    TEXT("")
#endif

#define TRACE(Format, ...) \
{ \
    SET_WARN_COLOR( COLOR_CYAN );\
    FString Msg = FString::Printf(TEXT(Format), ##__VA_ARGS__ ); \
    UE_LOG( client, Log, TEXT("%s() : %s"), FUNC_NAME, *Msg);\
    CLEAR_WARN_COLOR();\
}

DECLARE_LOG_CATEGORY_EXTERN(client, Log, Log);