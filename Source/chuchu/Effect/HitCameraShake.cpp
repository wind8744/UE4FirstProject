// Fill out your copyright notice in the Description page of Project Settings.


#include "HitCameraShake.h"

UHitCameraShake::UHitCameraShake()
{
	//생성자에서 값 세팅
	OscillationDuration = 1.f; //0.3f

	OscillationBlendInTime = 0.1f;
	OscillationBlendOutTime = 0.1f;

	LocOscillation.Y.Amplitude = 3.f; //강도 1.f
	LocOscillation.Y.Frequency = 100.f; //속도
	LocOscillation.Y.InitialOffset = EInitialOscillatorOffset::EOO_OffsetZero; //

	LocOscillation.Z.Amplitude = 3.f; //1.f
	LocOscillation.Z.Frequency = 100.f;
	LocOscillation.Z.InitialOffset = EInitialOscillatorOffset::EOO_OffsetZero;

	bSingleInstance = true;
}