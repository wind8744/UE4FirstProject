// Fill out your copyright notice in the Description page of Project Settings.


#include "SelectPlayerAnimInstance.h"
#include "PlayerCharacter.h"

USelectPlayerAnimInstance::USelectPlayerAnimInstance()
{
	m_AnimType = ESelectPlayerAnimType::Idle;
}

void USelectPlayerAnimInstance::AnimNotify_TransitionIdle() //선택ㄷ됐을때 idle로 전환
{
	m_AnimType = ESelectPlayerAnimType::Idle;
}

