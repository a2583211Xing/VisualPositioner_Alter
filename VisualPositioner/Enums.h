/**************************************************************************************************
// Copyright(C), 2015-2019, MISV Machine Vision Lab.
//
// file:	enums.h
//
// summary:	声明公用枚举类型
 **************************************************************************************************/

#pragma once

enum Camera
{
	LeftCamera = 1,
	RightCamera = 2
};

enum Direction
{
    LeftToRight = 0,
    RightToLeft = 1,
    TopToBottom = 2,
    BottomToTop = 3
};

enum Polarity
{
    BlackToWhite,
    WhiteToBlack
};

enum class PositioningError
{
	NoError,
	SettingsNotLoaded,
	LoadingSettingsFailure,
	SaveSettingsFailure,
	MissingInput,
	IllegalInput,
	CornerDetectionFailure,
	CrossMarkDetectionFailure,
	CameraRegisterationFailure,
	CameraNotRegistered,
	RotationCenterNotCalibrated,
	TranslationDirectionNotCalibrated,
	StandardPositionNotRegistered,
	GeneralError
};
