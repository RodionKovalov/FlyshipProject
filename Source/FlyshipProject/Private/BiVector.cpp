// Fill out your copyright notice in the Description page of Project Settings.


#include "BiVector.h"

FBiVector::FBiVector() {}
FBiVector::FBiVector(FVector q, FVector p)
{
	this->q = q;
	this->p = p;
}
FBiVector FBiVector::operator+(const FBiVector& a) const
{
	return FBiVector(a.p + p, a.q + q);
}
FBiVector FBiVector::operator+=(const FBiVector& a)
{
	p += a.p;
	q += a.q;
	return *this;
}
FBiVector FBiVector::operator*(float f) const
{
	return FBiVector(f * p, f * q);
}